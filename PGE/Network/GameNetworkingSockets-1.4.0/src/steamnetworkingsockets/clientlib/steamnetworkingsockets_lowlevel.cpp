//====== Copyright Valve Corporation, All rights reserved. ====================

#if defined( _MSC_VER ) && ( _MSC_VER <= 1800 )
	#pragma warning( disable: 4244 )
	// 1>C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\include\chrono(749): warning C4244: '=' : conversion from '__int64' to 'time_t', possible loss of data (steamnetworkingsockets_lowlevel.cpp)
#endif

#ifdef __GNUC__
	// src/public/tier0/basetypes.h:104:30: error: assuming signed overflow does not occur when assuming that (X + c) < X is always false [-Werror=strict-overflow]
	// current steamrt:scout gcc "g++ (SteamRT 4.8.4-1ubuntu15~12.04+steamrt1.2+srt1) 4.8.4" requires this at the top due to optimizations
	#pragma GCC diagnostic ignored "-Wstrict-overflow"
#endif

#include <thread>
#include <mutex>
#include <atomic>

#ifdef POSIX
#include <pthread.h>
#include <sched.h>
#endif

#include "steamnetworkingsockets_lowlevel.h"
#include "../steamnetworkingsockets_platform.h"
#include "../steamnetworkingsockets_internal.h"
#include "../steamnetworkingsockets_thinker.h"
#include "steamnetworkingsockets_connections.h"
#include <vstdlib/random.h>
#include <tier1/utlpriorityqueue.h>
#include <tier1/utllinkedlist.h>
#include "crypto.h"

#include <tier0/memdbgoff.h>

// Ugggggggggg MSVC VS2013 STL bug: try_lock_for doesn't actually respect the timeout, it always ends up using an infinite timeout.
// And even in 2015, the code is calling the timer to get current time, to convert a relative time to an absolute time, and then
// waiting until that absolute time, which then calls the timer again....and subtracts it back off....It's really bad. Just go
// directly to the underlying Win32 primitives.  Looks like the Visual Studio 2017 STL implementation is sane, though.
#if defined(_MSC_VER) && _MSC_VER < 1914
	// NOTE - we could implement our own mutex here.
	#error "std::recursive_timed_mutex doesn't work"
#endif

#ifdef _XBOX_ONE
	#include <combaseapi.h>
#endif

// Time low level send/recv calls and packet processing
//#define STEAMNETWORKINGSOCKETS_LOWLEVEL_TIME_SOCKET_CALLS

#include <tier0/memdbgon.h>

namespace SteamNetworkingSocketsLib {

constexpr int k_msMaxPollWait = 1000;
constexpr SteamNetworkingMicroseconds k_usecMaxTimestampDelta = k_msMaxPollWait * 1100;

static void FlushSystemSpew();
static void FlushSystemSpewLocked();

int g_cbUDPSocketBufferSize = 256*1024;

/// Global lock for all local data structures
static Lock<RecursiveTimedMutexImpl> s_mutexGlobalLock( "global", 0 );

#if STEAMNETWORKINGSOCKETS_LOCK_DEBUG_LEVEL > 0

// By default, complain if we hold the lock for more than this long
constexpr SteamNetworkingMicroseconds k_usecDefaultLongLockHeldWarningThreshold = 5*1000;

// Debug the locks active on the cu
struct ThreadLockDebugInfo
{
	static constexpr int k_nMaxHeldLocks = 8;
	static constexpr int k_nMaxTags = 32;

	int m_nHeldLocks = 0;
	int m_nTags = 0;

	SteamNetworkingMicroseconds m_usecLongLockWarningThreshold;
	SteamNetworkingMicroseconds m_usecIgnoreLongLockWaitTimeUntil;
	SteamNetworkingMicroseconds m_usecOuterLockStartTime; // Time when we started waiting on outermost lock (if we don't have it yet), or when we aquired the lock (if we have it)

	const LockDebugInfo *m_arHeldLocks[ k_nMaxHeldLocks ];
	struct Tag_t
	{
		const char *m_pszTag;
		int m_nCount;
	};
	Tag_t m_arTags[ k_nMaxTags ];
};

static void (*s_fLockAcquiredCallback)( const char *tags, SteamNetworkingMicroseconds usecWaited );
static void (*s_fLockHeldCallback)( const char *tags, SteamNetworkingMicroseconds usecWaited );
static SteamNetworkingMicroseconds s_usecLockWaitWarningThreshold = 2*1000;

/// Get the per-thread debug info
static ThreadLockDebugInfo &GetThreadDebugInfo()
{
	// Apple seems to hate thread_local.  Is there some sort of feature
	// define a can check here?  It's a shame because it's really very
	// efficient on MSVC, gcc, and clang on Windows and linux.
    //
    // Apple seems to support thread_local starting with Xcode 8.0
	#if defined(__APPLE__) && __clang_major__ < 8

		static pthread_key_t key;
		static pthread_once_t key_once = PTHREAD_ONCE_INIT;

		// One time init the TLS key
		pthread_once( &key_once,
			[](){ // Initialization code to run once
				pthread_key_create(
					&key,
					[](void *ptr) { free(ptr); } // Destructor function
				);
			}
		);

		// Get current object
		void *result = pthread_getspecific(key);
		if ( unlikely( result == nullptr ) )
		{
			result = malloc( sizeof(ThreadLockDebugInfo) );
			memset( result, 0, sizeof(ThreadLockDebugInfo) );
			pthread_setspecific(key, result);
		}
		return *static_cast<ThreadLockDebugInfo *>( result );
	#else

		// Use thread_local
		thread_local ThreadLockDebugInfo tls_lockDebugInfo;
		return tls_lockDebugInfo;
	#endif
}

/// If non-NULL, add a "tag" to the lock journal for the current thread.
/// This is useful so that if we hold a lock for a long time, we can get
/// an idea what sorts of operations were taking a long time.
static void AddThreadLockTag( const char *pszTag )
{
	if ( !pszTag )
		return;

	ThreadLockDebugInfo &t = GetThreadDebugInfo();
	Assert( t.m_nHeldLocks > 0 ); // Can't add a tag unless we are locked!

	for ( int i = 0 ; i < t.m_nTags ; ++i )
	{
		if ( t.m_arTags[i].m_pszTag == pszTag )
		{
			++t.m_arTags[i].m_nCount;
			return;
		}
	}

	if ( t.m_nTags >= ThreadLockDebugInfo::k_nMaxTags )
		return;

	t.m_arTags[ t.m_nTags ].m_pszTag = pszTag;
	t.m_arTags[ t.m_nTags ].m_nCount = 1;
	++t.m_nTags;
}

LockDebugInfo::~LockDebugInfo()
{
	// We should not be locked!  If we are, remove us
	ThreadLockDebugInfo &t = GetThreadDebugInfo();
	for ( int i = t.m_nHeldLocks-1 ; i >= 0 ; --i )
	{
		if ( t.m_arHeldLocks[i] == this )
		{
			AssertMsg( false, "Lock '%s' being destroyed while it is held!", m_pszName );
			AboutToUnlock();
		}
	}
}

void LockDebugInfo::AboutToLock( bool bTry )
{
	ThreadLockDebugInfo &t = GetThreadDebugInfo();

	// First lock held by this thread?
	if ( t.m_nHeldLocks == 0 )
	{
		// Remember when we started trying to lock
		t.m_usecOuterLockStartTime = SteamNetworkingSockets_GetLocalTimestamp();
	}
	else
	{

		// We already hold a lock.  Make sure it's legal for us to take another!

		// Global lock *must* always be the outermost lock.  (It is legal to take other locks in
		// between and then lock the global lock recursively.)
		const bool bHoldGlobalLock = t.m_arHeldLocks[ 0 ] == &s_mutexGlobalLock;
		AssertMsg(
			bHoldGlobalLock || this != &s_mutexGlobalLock,
			"Taking global lock while already holding lock '%s'", t.m_arHeldLocks[ 0 ]->m_pszName
		);

		// Check for taking locks in such a way that might lead to deadlocks.
		// If they are only "trying", then we do allow out of order behaviour.
		if ( !bTry )
		{
			const LockDebugInfo *pTopLock = t.m_arHeldLocks[ t.m_nHeldLocks-1 ];

			// Once we take a "short duration" lock, we must not
			// take any additional locks!  (Including a recursive lock.)
			AssertMsg( !( pTopLock->m_nFlags & LockDebugInfo::k_nFlag_ShortDuration ), "Taking lock '%s' while already holding lock '%s'", m_pszName, pTopLock->m_pszName );

			// If the global lock isn't held, then no more than one
			// object lock is allowed, since two different threads
			// might take them in different order.
			constexpr int k_nObjectFlags = LockDebugInfo::k_nFlag_Connection | LockDebugInfo::k_nFlag_PollGroup;
			if (
				( !bHoldGlobalLock && ( m_nFlags & k_nObjectFlags ) != 0 )
				//|| ( m_nFlags & k_nFlag_Table ) // We actually do this in one place when we know it's OK.  Not wirth it right now to get this situation exempted from the checking.
			) {
				// We must not already hold any existing object locks (except perhaps this one)
				for ( int i = 0 ; i < t.m_nHeldLocks ; ++i )
				{
					const LockDebugInfo *pOtherLock = t.m_arHeldLocks[ i ];
					AssertMsg( pOtherLock == this || !( pOtherLock->m_nFlags & k_nObjectFlags ),
						"Taking lock '%s' and then '%s', while not holding the global lock", pOtherLock->m_pszName, m_pszName );
				}
			}
		}
	}
}

void LockDebugInfo::OnLocked( const char *pszTag )
{
	ThreadLockDebugInfo &t = GetThreadDebugInfo();

	Assert( t.m_nHeldLocks < ThreadLockDebugInfo::k_nMaxHeldLocks );
	t.m_arHeldLocks[ t.m_nHeldLocks++ ] = this;

	if ( t.m_nHeldLocks == 1 )
	{
		SteamNetworkingMicroseconds usecNow = SteamNetworkingSockets_GetLocalTimestamp();
		SteamNetworkingMicroseconds usecTimeSpentWaitingOnLock = usecNow - t.m_usecOuterLockStartTime;
		t.m_usecLongLockWarningThreshold = k_usecDefaultLongLockHeldWarningThreshold;
		t.m_nTags = 0;

		if ( usecTimeSpentWaitingOnLock > s_usecLockWaitWarningThreshold && usecNow > t.m_usecIgnoreLongLockWaitTimeUntil )
		{
			if ( pszTag )
				SpewWarning( "Waited %.1fms for SteamNetworkingSockets lock [%s]", usecTimeSpentWaitingOnLock*1e-3, pszTag );
			else
				SpewWarning( "Waited %.1fms for SteamNetworkingSockets lock", usecTimeSpentWaitingOnLock*1e-3 );
			ETW_LongOp( "lock wait", usecTimeSpentWaitingOnLock, pszTag );
		}

		auto callback = s_fLockAcquiredCallback; // save to temp, to prevent very narrow race condition where variable is cleared after we null check it, and we call null
		if ( callback )
			callback( pszTag, usecTimeSpentWaitingOnLock );

		t.m_usecOuterLockStartTime = usecNow;
	}

	AddThreadLockTag( pszTag );
}

void LockDebugInfo::AboutToUnlock()
{
	char tags[ 256 ];

	SteamNetworkingMicroseconds usecElapsed = 0;
	SteamNetworkingMicroseconds usecElapsedTooLong = 0;
	auto lockHeldCallback = s_fLockHeldCallback;

	ThreadLockDebugInfo &t = GetThreadDebugInfo();
	Assert( t.m_nHeldLocks > 0 );

	// Unlocking the last lock?
	if ( t.m_nHeldLocks == 1 )
	{

		// We're about to do the final release.  How long did we hold the lock?
		usecElapsed = SteamNetworkingSockets_GetLocalTimestamp() - t.m_usecOuterLockStartTime;

		// Too long?  We need to check the threshold here because the threshold could
		// change by another thread immediately after we release the lock.  Also, if
		// we're debugging, all bets are off.  They could have hit a breakpoint, and
		// we don't want to create a bunch of confusing spew with spurious asserts
		if ( usecElapsed >= t.m_usecLongLockWarningThreshold && !Plat_IsInDebugSession() )
		{
			usecElapsedTooLong = usecElapsed;
		}

		if ( usecElapsedTooLong > 0 || lockHeldCallback )
		{
			char *p = tags;
			char *end = tags + sizeof(tags) - 1;
			for ( int i = 0 ; i < t.m_nTags && p+5 < end ; ++i )
			{
				if ( p > tags )
					*(p++) = ',';

				const ThreadLockDebugInfo::Tag_t &tag = t.m_arTags[i];
				int taglen = std::min( int(end-p), (int)V_strlen( tag.m_pszTag ) );
				memcpy( p, tag.m_pszTag, taglen );
				p += taglen;

				if ( tag.m_nCount > 1 )
				{
					int l = end-p;
					if ( l <= 5 )
						break;
					p += V_snprintf( p, l, "(x%d)", tag.m_nCount );
				}
			}
			*p = '\0';
		}

		t.m_nTags = 0;
		t.m_usecOuterLockStartTime = 0; // Just for grins.
	}

	if ( usecElapsed > 0 && lockHeldCallback )
	{
		lockHeldCallback(tags, usecElapsed);
	}

	// Yelp if we held the lock for longer than the threshold.
	if ( usecElapsedTooLong != 0 )
	{
		SpewWarning(
			"SteamNetworkingSockets lock held for %.1fms.  (Performance warning.)  %s\n"
			"This is usually a symptom of a general performance problem such as thread starvation.",
			usecElapsedTooLong*1e-3, tags
		);
		ETW_LongOp( "lock held", usecElapsedTooLong, tags );
	}

	// NOTE: We are allowed to unlock out of order!  We specifically
	// do this with the table lock!
	for ( int i = t.m_nHeldLocks-1 ; i >= 0 ; --i )
	{
		if ( t.m_arHeldLocks[i] == this )
		{
			--t.m_nHeldLocks;
			if ( i < t.m_nHeldLocks ) // Don't do the memmove in the common case of stack pop
				memmove( &t.m_arHeldLocks[i], &t.m_arHeldLocks[i+1], (t.m_nHeldLocks-i) * sizeof(t.m_arHeldLocks[0]) );
			t.m_arHeldLocks[t.m_nHeldLocks] = nullptr; // Just for grins
			return;
		}
	}

	AssertMsg( false, "Unlocked a lock '%s' that wasn't held?", m_pszName );
}

void LockDebugInfo::_AssertHeldByCurrentThread( const char *pszFile, int line, const char *pszTag ) const
{
	ThreadLockDebugInfo &t = GetThreadDebugInfo();
	for ( int i = t.m_nHeldLocks-1 ; i >= 0 ; --i )
	{
		if ( t.m_arHeldLocks[i] == this )
		{
			AddThreadLockTag( pszTag );
			return;
		}
	}

	AssertMsg( false, "%s(%d): Lock '%s' not held", pszFile, line, m_pszName );
}

void SteamNetworkingGlobalLock::SetLongLockWarningThresholdMS( const char *pszTag, int msWarningThreshold )
{
	AssertHeldByCurrentThread( pszTag );
	SteamNetworkingMicroseconds usecWarningThreshold = SteamNetworkingMicroseconds{msWarningThreshold}*1000;
	ThreadLockDebugInfo &t = GetThreadDebugInfo();
	if ( t.m_usecLongLockWarningThreshold < usecWarningThreshold )
	{
		t.m_usecLongLockWarningThreshold = usecWarningThreshold;
		t.m_usecIgnoreLongLockWaitTimeUntil = SteamNetworkingSockets_GetLocalTimestamp() + t.m_usecLongLockWarningThreshold;
	}
}

void SteamNetworkingGlobalLock::_AssertHeldByCurrentThread( const char *pszFile, int line )
{
	s_mutexGlobalLock._AssertHeldByCurrentThread( pszFile, line );
}

void SteamNetworkingGlobalLock::_AssertHeldByCurrentThread( const char *pszFile, int line, const char *pszTag )
{
	s_mutexGlobalLock._AssertHeldByCurrentThread( pszFile, line );
	AddThreadLockTag( pszTag );
}

#endif // #if STEAMNETWORKINGSOCKETS_LOCK_DEBUG_LEVEL > 0

void SteamNetworkingGlobalLock::Lock( const char *pszTag )
{
	s_mutexGlobalLock.lock( pszTag );
}

bool SteamNetworkingGlobalLock::TryLock( const char *pszTag, int msTimeout )
{
	return s_mutexGlobalLock.try_lock_for( msTimeout, pszTag );
}

void SteamNetworkingGlobalLock::Unlock()
{
	s_mutexGlobalLock.unlock();
}

static void SeedWeakRandomGenerator()
{

	// Seed cheesy random number generator using true source of entropy
	int temp;
	CCrypto::GenerateRandomBlock( &temp, sizeof(temp) );
	WeakRandomSeed( temp );
}

static std::atomic<long long> s_usecTimeLastReturned;

// Start with an offset so that a timestamp of zero is always pretty far in the past.
// But round it up to nice round number, so that looking at timestamps in the debugger
// is easy to read.
const long long k_nInitialTimestampMin = k_nMillion*24*3600*30;
const long long k_nInitialTimestamp = 3000000000000ll;
COMPILE_TIME_ASSERT( 2000000000000ll < k_nInitialTimestampMin );
COMPILE_TIME_ASSERT( k_nInitialTimestampMin < k_nInitialTimestamp );
static std::atomic<long long> s_usecTimeOffset( k_nInitialTimestamp );

static std::atomic<int> s_nLowLevelSupportRefCount(0);
static volatile bool s_bManualPollMode;

/////////////////////////////////////////////////////////////////////////////
//
// Task lists
//
/////////////////////////////////////////////////////////////////////////////

ShortDurationLock s_lockTaskQueue( "TaskQueue" );

CTaskTarget::~CTaskTarget()
{
	CancelQueuedTasks();

	// Set to invalid value so we will crash if we have use after free
	m_pFirstTask = (CQueuedTask *)~(uintptr_t)0;
}

void CTaskTarget::CancelQueuedTasks()
{

	// If we have any queued tasks, we need to cancel them
	if ( m_pFirstTask )
	{
		ShortDurationScopeLock scopeLock( s_lockTaskQueue );
		CQueuedTask *pTask = m_pFirstTask;
		while ( pTask )
		{
			CQueuedTask *pNext = pTask->m_pNextTaskForTarget;
			Assert( !pNext || pNext->m_pPrevTaskForTarget == pTask );
			Assert( pTask->m_pTarget == this );
			Assert( pTask->m_eTaskState == CQueuedTask::k_ETaskState_Queued );
			pTask->m_eTaskState = CQueuedTask::k_ETaskState_ReadyToDelete;
			pTask->m_pTarget = nullptr;
			pTask->m_pPrevTaskForTarget = nullptr;
			pTask->m_pNextTaskForTarget = nullptr;
			pTask = pNext;
		}
		m_pFirstTask = nullptr;
	}
}

void CTaskList::QueueTask( CQueuedTask *pTask )
{
	Assert( pTask->m_eTaskState == CQueuedTask::k_ETaskState_Init );
	Assert( !pTask->m_pPrevTaskForTarget );
	Assert( !pTask->m_pNextTaskForTarget );
	Assert( !pTask->m_pNextTaskInQueue );

	ShortDurationScopeLock scopeLock( s_lockTaskQueue );

	// If we have a target, add to list of target's tasks that need to be deleted
	CTaskTarget *pTarget = pTask->m_pTarget;
	if ( pTarget )
	{
		if ( pTarget->m_pFirstTask )
		{
			Assert( pTarget->m_pFirstTask->m_pPrevTaskForTarget == nullptr );
			pTarget->m_pFirstTask->m_pPrevTaskForTarget = pTask;
		}
		pTask->m_pPrevTaskForTarget = nullptr;
		pTask->m_pNextTaskForTarget = pTarget->m_pFirstTask;
		pTarget->m_pFirstTask = pTask;
	}

	if ( m_pLastTask )
	{
		Assert( m_pFirstTask );
		Assert( !m_pLastTask->m_pNextTaskInQueue );
		m_pLastTask->m_pNextTaskInQueue = pTask;
	}
	else
	{
		Assert( !m_pFirstTask );
		m_pFirstTask = pTask;
	}

	m_pLastTask = pTask;

	// Mark task as queued
	pTask->m_eTaskState = CQueuedTask::k_ETaskState_Queued;
}

void CTaskList::RunTasks()
{
	// Quick check for no tasks
	if ( !m_pFirstTask )
		return;

	// Detach the linked list
	s_lockTaskQueue.lock();
	CQueuedTask *pFirstTask = m_pFirstTask;
	m_pFirstTask = nullptr;
	m_pLastTask = nullptr;
	s_lockTaskQueue.unlock();

	// Process items
	CQueuedTask *pTask = pFirstTask;
	while ( pTask )
	{

		// We might have to loop due to lock contention
		for (;;)
		{

			// Already deleted?
			if ( pTask->m_eTaskState != CQueuedTask::k_ETaskState_Queued )
			{
				Assert( pTask->m_eTaskState == CQueuedTask::k_ETaskState_ReadyToDelete );
				Assert( pTask->m_pTarget == nullptr );
				break;
			}

			ShortDurationScopeLock scopeLock;

			// We'll need to lock the queue if they have a target.
			// If their target does not have a locking mechanism, then we
			// assume that it cannot be deleted here.  However, we always need
			// to protect against other tasks getting queued against the target,
			// and we allow that to be done without locking the target.
			if ( pTask->m_pTarget )
				scopeLock.Lock( s_lockTaskQueue );

// FIXME - has not been tested, and also does not have a good mechanism for unlocking.
//			// Do we have a lock we need to take?
//			if ( pTask->m_lockFunc )
//			{
//
//				int msTimeOut = 10;
//				if ( pTask->m_pTarget )
//				{
//					scopeLock.Lock( s_lockTaskQueue );
//
//					// Deleted while we locked?
//					if ( pTask->m_eTaskState != CQueuedTask::k_ETaskState_Queued )
//					{
//						Assert( pTask->m_eTaskState == CQueuedTask::k_ETaskState_ReadyToDelete );
//						Assert( pTask->m_pTarget == nullptr );
//						break;
//					}
//
//					// Use a short timeout and loop, in case we might deadlock
//					msTimeOut = 1;
//				}
//
//				if ( !(*pTask->m_lockFunc)( pTask->m_lockFuncArg, msTimeOut, pTask->m_pszTag ) )
//				{
//					// Other object is busy, or perhaps we are deadlocked?
//					continue;
//				}
//
//				// Deleted while we locked?
//				if ( pTask->m_eTaskState != CQueuedTask::k_ETaskState_Queued )
//				{
//					Assert( pTask->m_eTaskState == CQueuedTask::k_ETaskState_ReadyToDelete );
//					Assert( pTask->m_pTarget == nullptr );
//					break;
//				}
//			}

			// OK, we've got the locks we need and are ready to run.
			// Unlink from the target, if we have one
			CTaskTarget *pTarget = pTask->m_pTarget;
			if ( pTarget )
			{
				CQueuedTask *p = pTask->m_pPrevTaskForTarget;
				CQueuedTask *n = pTask->m_pNextTaskForTarget;
				if ( p )
				{
					Assert( p->m_pTarget == pTarget );
					Assert( p->m_pNextTaskForTarget == pTask );
					p->m_pNextTaskForTarget = n;
					pTask->m_pPrevTaskForTarget = nullptr;
				}
				else
				{
					Assert( pTarget->m_pFirstTask == pTask );
					pTarget->m_pFirstTask = n;
				}
				if ( n )
				{
					Assert( n->m_pPrevTaskForTarget == pTask );
					n->m_pPrevTaskForTarget = p;
					pTask->m_pNextTaskForTarget = nullptr;
				}

				// Note: we must leave the target pointer set
			}

			// We can release this lock now if we took it
			scopeLock.Unlock();

			// !KLUDGE! Deluxe
			if ( this == &g_taskListRunWithGlobalLock )
			{
				// Make sure we hold the lock, and also set the tag for debugging purposes
				SteamNetworkingGlobalLock::AssertHeldByCurrentThread( pTask->m_pszTag );
			}

			// Run the task
			pTask->m_eTaskState = CQueuedTask::k_ETaskState_Running;
			pTask->Run();

			// Mark us as finished
			pTask->m_eTaskState = CQueuedTask::k_ETaskState_ReadyToDelete;
			pTask->m_pTarget = nullptr;
			break;
		}

		// Done, we can delete the task
		CQueuedTask *pNext = pTask->m_pNextTaskInQueue;
		pTask->m_pNextTaskInQueue = nullptr;
		delete pTask;
		pTask = pNext;
	}
}

CTaskList g_taskListRunWithGlobalLock;
CTaskList g_taskListRunInBackground;

CQueuedTask::~CQueuedTask()
{
	Assert( m_eTaskState == k_ETaskState_Init || m_eTaskState == k_ETaskState_ReadyToDelete );
	Assert( !m_pNextTaskInQueue );
	Assert( !m_pPrevTaskForTarget );
	Assert( !m_pNextTaskForTarget );
}

void CQueuedTask::SetTarget( CTaskTarget *pTarget )
{
	// Can only call this once
	Assert( m_eTaskState == k_ETaskState_Init );
	Assert( m_pTarget == nullptr );
	m_pTarget = pTarget;

	// NOTE: We wait to link task to the target until we actually queue it
}

bool CQueuedTask::RunWithGlobalLockOrQueue( const char *pszTag )
{
	Assert( m_eTaskState == k_ETaskState_Init );
	Assert( m_lockFunc == nullptr );

	// Check if lock is available immediately
	if ( !SteamNetworkingGlobalLock::TryLock( pszTag, 0 ) )
	{
		QueueToRunWithGlobalLock( pszTag );
		return false;
	}

	// Service the queue so we always do items in order
	g_taskListRunWithGlobalLock.RunTasks();

	// Let derived class do work
	m_eTaskState = k_ETaskState_Running;
	Run();

	// Go ahead and unlock now
	SteamNetworkingGlobalLock::Unlock();

	// Self destruct
	m_eTaskState = k_ETaskState_ReadyToDelete;
	m_pTarget = nullptr;
	delete this;

	// We have run
	return true;
}

void CQueuedTask::QueueToRunWithGlobalLock( const char *pszTag )
{
	Assert( m_lockFunc == nullptr );
	if ( pszTag )
		m_pszTag = pszTag;
	g_taskListRunWithGlobalLock.QueueTask( this );

	// Make sure service thread will wake up to do something with this
	WakeSteamDatagramThread();

	// NOTE: At this point we are subject to being run or deleted at any time!
}

void CQueuedTask::QueueToRunInBackground()
{
	g_taskListRunInBackground.QueueTask( this );

	//if ( s_bManualPollMode || ( s_pThreadSteamDatagram && s_pThreadSteamDatagram->get_id() != std::this_thread::get_id() ) )
	WakeSteamDatagramThread();
}

// Try to guess if the route the specified address is probably "local".
// This is difficult to do in general.  We want something that mostly works.
//
// False positives: VPNs and IPv6 addresses that appear to be nearby but are not.
// False negatives: We can't always tell if a route is local.
bool IsRouteToAddressProbablyLocal( netadr_t addr )
{

	// Assume that if we are able to send to any "reserved" route, that is is local.
	// Note that this will be true for VPNs, too!
	if ( addr.IsReservedAdr() )
		return true;

	// But other cases might also be local routes.  E.g. two boxes with public IPs.
	// Convert to sockaddr struct so we can ask the operating system
	addr.SetPort(0);
	sockaddr_storage sockaddrDest;
	addr.ToSockadr( &sockaddrDest );

	#ifdef _WINDOWS

		//
		// These functions were added with Vista, so load dynamically
		// in case
		//

		typedef
		DWORD
		(WINAPI *FnGetBestInterfaceEx)(
			struct sockaddr *pDestAddr,
			PDWORD           pdwBestIfIndex
			);
		typedef 
		NETIO_STATUS
		(NETIOAPI_API_*FnGetBestRoute2)(
			NET_LUID *InterfaceLuid,
			NET_IFINDEX InterfaceIndex,
			CONST SOCKADDR_INET *SourceAddress,
			CONST SOCKADDR_INET *DestinationAddress,
			ULONG AddressSortOptions,
			PMIB_IPFORWARD_ROW2 BestRoute,
			SOCKADDR_INET *BestSourceAddress
			);

		static HMODULE hModule = LoadLibraryA( "Iphlpapi.dll" );
		static FnGetBestInterfaceEx pGetBestInterfaceEx = hModule ? (FnGetBestInterfaceEx)GetProcAddress( hModule, "GetBestInterfaceEx" ) : nullptr;
		static FnGetBestRoute2 pGetBestRoute2 = hModule ? (FnGetBestRoute2)GetProcAddress( hModule, "GetBestRoute2" ) : nullptr;;
		if ( !pGetBestInterfaceEx || !pGetBestRoute2 )
			return false;

		NET_IFINDEX dwBestIfIndex;
		DWORD r = (*pGetBestInterfaceEx)( (sockaddr *)&sockaddrDest, &dwBestIfIndex );
		if ( r != NO_ERROR )
		{
			AssertMsg2( false, "GetBestInterfaceEx failed with result %d for address '%s'", r, CUtlNetAdrRender( addr ).String() );
			return false;
		}

		MIB_IPFORWARD_ROW2 bestRoute;
		SOCKADDR_INET bestSourceAddress;
		r = (*pGetBestRoute2)(
			nullptr, // InterfaceLuid
			dwBestIfIndex, // InterfaceIndex
			nullptr, // SourceAddress
			(SOCKADDR_INET *)&sockaddrDest, // DestinationAddress
			0, // AddressSortOptions
			&bestRoute, // BestRoute
			&bestSourceAddress // BestSourceAddress
		);
		if ( r != NO_ERROR )
		{
			AssertMsg2( false, "GetBestRoute2 failed with result %d for address '%s'", r, CUtlNetAdrRender( addr ).String() );
			return false;
		}
		if ( bestRoute.Protocol == MIB_IPPROTO_LOCAL )
			return true;
		netadr_t nextHop;
		if ( !nextHop.SetFromSockadr( &bestRoute.NextHop ) )
		{
			AssertMsg( false, "GetBestRoute2 returned invalid next hop address" );
			return false;
		}

		nextHop.SetPort( 0 );

		// https://docs.microsoft.com/en-us/windows/win32/api/netioapi/ns-netioapi-mib_ipforward_row2:
		//   For a remote route, the IP address of the next system or gateway en route.
		//   If the route is to a local loopback address or an IP address on the local
		//   link, the next hop is unspecified (all zeros). For a local loopback route,
		//   this member should be an IPv4 address of 0.0.0.0 for an IPv4 route entry
		//   or an IPv6 address address of 0::0 for an IPv6 route entry.
		if ( !nextHop.HasIP() )
			return true;
		if ( nextHop == addr )
			return true;

		// If final destination is on the same IPv6/56 prefix, then assume
		// it's a local route.  This is an arbitrary prefix size to use,
		// but it's a compromise.  We think that /64 probably has too
		// many false negatives, but /48 has have too many false positives.
		if ( addr.GetType() == k_EIPTypeV6 )
		{
			if ( nextHop.GetType() == k_EIPTypeV6 )
			{
				if ( memcmp( addr.GetIPV6Bytes(), nextHop.GetIPV6Bytes(), 7 ) == 0 )
					return true;
			}
			netadr_t netdrBestSource;
			if ( netdrBestSource.SetFromSockadr( &bestSourceAddress ) && netdrBestSource.GetType() == k_EIPTypeV6 )
			{
				if ( memcmp( addr.GetIPV6Bytes(), netdrBestSource.GetIPV6Bytes(), 7 ) == 0 )
					return true;
			}
		}

	#else
		// FIXME - Writeme
	#endif

	// Nope
	return false;
}

/////////////////////////////////////////////////////////////////////////////
//
// Raw sockets
//
/////////////////////////////////////////////////////////////////////////////

static double s_flFakeRateLimit_Send_tokens;
static double s_flFakeRateLimit_Recv_tokens;
static SteamNetworkingMicroseconds s_usecFakeRateLimitBucketUpdateTime;

static void InitFakeRateLimit()
{
	s_usecFakeRateLimitBucketUpdateTime = SteamNetworkingSockets_GetLocalTimestamp();
	s_flFakeRateLimit_Send_tokens = (double)INT_MAX;
	s_flFakeRateLimit_Recv_tokens = (double)INT_MAX;
}

static void UpdateFakeRateLimitTokenBuckets( SteamNetworkingMicroseconds usecNow )
{
	float flElapsed = ( usecNow - s_usecFakeRateLimitBucketUpdateTime ) * 1e-6;
	s_usecFakeRateLimitBucketUpdateTime = usecNow;

	if ( g_Config_FakeRateLimit_Send_Rate.Get() <= 0 )
	{
		s_flFakeRateLimit_Send_tokens = (double)INT_MAX;
	}
	else
	{
		s_flFakeRateLimit_Send_tokens += flElapsed*g_Config_FakeRateLimit_Send_Rate.Get();
		s_flFakeRateLimit_Send_tokens = std::min( s_flFakeRateLimit_Send_tokens, (double)g_Config_FakeRateLimit_Send_Burst.Get() );
	}

	if ( g_Config_FakeRateLimit_Recv_Rate.Get() <= 0 )
	{
		s_flFakeRateLimit_Recv_tokens = (double)INT_MAX;
	}
	else
	{
		s_flFakeRateLimit_Recv_tokens += flElapsed*g_Config_FakeRateLimit_Recv_Rate.Get();
		s_flFakeRateLimit_Recv_tokens = std::min( s_flFakeRateLimit_Recv_tokens, (double)g_Config_FakeRateLimit_Recv_Burst.Get() );
	}
}

inline IRawUDPSocket::IRawUDPSocket() {}
inline IRawUDPSocket::~IRawUDPSocket() {}

class CRawUDPSocketImpl final : public IRawUDPSocket
{
public:
	STEAMNETWORKINGSOCKETS_DECLARE_CLASS_OPERATOR_NEW

	~CRawUDPSocketImpl()
	{
		closesocket( m_socket );
	}

	/// Descriptor from the OS
	SOCKET m_socket;

	/// What address families are supported by this socket?
	int m_nAddressFamilies;

	/// Who to notify when we receive a packet on this socket.
	/// This is set to null when we are asked to close the socket.
	CRecvPacketCallback m_callback;


	// Implements IRawUDPSocket
	virtual bool BSendRawPacketGather( int nChunks, const iovec *pChunks, const netadr_t &adrTo ) const override;
	virtual void Close() override;

	//// Send a packet, for really realz right now.  (No checking for fake loss or lag.)
	inline bool BReallySendRawPacket( int nChunks, const iovec *pChunks, const netadr_t &adrTo ) const
	{
		Assert( m_socket != INVALID_SOCKET );

		// Add a tag.  If we end up holding the lock for a long time, this tag
		// will tell us how many packets were sent
		SteamNetworkingGlobalLock::AssertHeldByCurrentThread( "SendUDPacket" );

		// Convert address to BSD interface
		struct sockaddr_storage destAddress;
		socklen_t addrSize;
		if ( m_nAddressFamilies & k_nAddressFamily_IPv6 )
		{
			addrSize = sizeof(sockaddr_in6);
			adrTo.ToSockadrIPV6( &destAddress );
		}
		else
		{
			addrSize = (socklen_t)adrTo.ToSockadr( &destAddress );
		}

		#ifdef STEAMNETWORKINGSOCKETS_ENABLE_ETW
		{
			int cbTotal = 0;
			for ( int i = 0 ; i < nChunks ; ++i )
				cbTotal += (int)pChunks[i].iov_len;
			ETW_UDPSendPacket( adrTo, cbTotal );
		}
		#endif

		if ( g_Config_PacketTraceMaxBytes.Get() >= 0 )
		{
			TracePkt( true, adrTo, nChunks, pChunks );
		}

		#ifdef STEAMNETWORKINGSOCKETS_LOWLEVEL_TIME_SOCKET_CALLS
			SteamNetworkingMicroseconds usecSendStart = SteamNetworkingSockets_GetLocalTimestamp();
		#endif

		#ifdef WIN32
			// Confirm that iovec and WSABUF are indeed bitwise equivalent
			COMPILE_TIME_ASSERT( sizeof( iovec ) == sizeof( WSABUF ) );
			COMPILE_TIME_ASSERT( offsetof( iovec, iov_len ) == offsetof( WSABUF, len ) );
			COMPILE_TIME_ASSERT( offsetof( iovec, iov_base ) == offsetof( WSABUF, buf ) );

			DWORD numberOfBytesSent;
			int r = WSASendTo(
				m_socket,
				(WSABUF *)pChunks,
				(DWORD)nChunks,
				&numberOfBytesSent,
				0, // flags
				(const sockaddr *)&destAddress,
				addrSize,
				nullptr, // lpOverlapped
				nullptr // lpCompletionRoutine
			);
			bool bResult = ( r == 0 );
			if ( !bResult )
			{
				SpewWarning( "WSASendTo %s failed, returned %d, last error=0x%x\n", CUtlNetAdrRender( adrTo ).String(), r, GetLastSocketError() );
			}
		#else
			msghdr msg;
			msg.msg_name = (sockaddr *)&destAddress;
			msg.msg_namelen = addrSize;
			msg.msg_iov = const_cast<struct iovec *>( pChunks );
			msg.msg_iovlen = nChunks;
			msg.msg_control = nullptr;
			msg.msg_controllen = 0;
			msg.msg_flags = 0;

			int r = ::sendmsg( m_socket, &msg, 0 );
			bool bResult = ( r >= 0 ); // just check for -1 for error, since we don't want to take the time here to scan the iovec and sum up the expected total number of bytes sent
		#endif

		#ifdef STEAMNETWORKINGSOCKETS_LOWLEVEL_TIME_SOCKET_CALLS
			SteamNetworkingMicroseconds usecSendEnd = SteamNetworkingSockets_GetLocalTimestamp();
			if ( usecSendEnd > s_usecIgnoreLongLockWaitTimeUntil )
			{
				SteamNetworkingMicroseconds usecSendElapsed = usecSendEnd - usecSendStart;
				if ( usecSendElapsed > 1000 )
				{
					SpewWarning( "UDP send took %.1fms\n", usecSendElapsed*1e-3 );
					ETW_LongOp( "UDP send", usecSendElapsed );
				}
			}
		#endif

		return bResult;
	}

	void TracePkt( bool bSend, const netadr_t &adrRemote, int nChunks, const iovec *pChunks ) const
	{
		int cbTotal = 0;
		for ( int i = 0 ; i < nChunks ; ++i )
			cbTotal += pChunks[i].iov_len;
		if ( bSend )
		{
			ReallySpewTypeFmt( k_ESteamNetworkingSocketsDebugOutputType_Msg, "[Trace Send] %s -> %s | %d bytes\n",
				SteamNetworkingIPAddrRender( m_boundAddr ).c_str(), CUtlNetAdrRender( adrRemote ).String(), cbTotal );
		}
		else
		{
			ReallySpewTypeFmt( k_ESteamNetworkingSocketsDebugOutputType_Msg, "[Trace Recv] %s <- %s | %d bytes\n",
				SteamNetworkingIPAddrRender( m_boundAddr ).c_str(), CUtlNetAdrRender( adrRemote ).String(), cbTotal );
		}
		int l = std::min( cbTotal, g_Config_PacketTraceMaxBytes.Get() );
		const uint8 *p = (const uint8 *)pChunks->iov_base;
		int cbChunkLeft = pChunks->iov_len;
		while ( l > 0 )
		{
			// How many bytes to print on thie row?
			int row = std::min( 16, l );
			l -= row;

			char buf[256], *d = buf;
			do {

				// Check for end of this chunk
				while ( cbChunkLeft == 0 )
				{
					++pChunks;
					p = (const uint8 *)pChunks->iov_base;
					cbChunkLeft = pChunks->iov_len;
				}

				// print the byte
				static const char hexdigit[] = "0123456789abcdef";
				*(d++) = ' ';
				*(d++) = hexdigit[ *p >> 4 ];
				*(d++) = hexdigit[ *p & 0xf ];

				// Advance to next byte
				++p;
				--cbChunkLeft;
			} while (--row > 0 );
			*d = '\0';

			// Emit the row
			ReallySpewTypeFmt( k_ESteamNetworkingSocketsDebugOutputType_Msg, "    %s\n", buf );
		}
	}

	#ifdef STEAMNETWORKINGSOCKETS_ENABLE_DUALWIFI
	CRawUDPSocketImpl *m_pDualWifiPartner = nullptr;
	virtual IRawUDPSocket *GetDualWifiSecondarySocket( int nEnableSetting ) override;
	#endif

private:

	void InternalAddToCleanupQueue();
};

/// We don't expect to have enough sockets, and open and close them frequently
/// enough, such that an occasional linear search will kill us.
static CUtlVector<CRawUDPSocketImpl *> s_vecRawSockets;

/// Are any sockets pending destruction?
static bool s_bRawSocketPendingDestruction;

/// POSIX polling.  This list will be recreated any time a socket is created or destroyed
#ifndef _WIN32
static CUtlVector<pollfd> s_vecPollFDs;
static bool s_bRecreatePollList = true;
#endif

/// Track packets that have fake lag applied and are pending to be sent/received
class CPacketLagger : private IThinker
{
public:
	~CPacketLagger() { Clear(); }

	void LagPacket( CRawUDPSocketImpl *pSock, const netadr_t &adr, int msDelay, int nChunks, const iovec *pChunks )
	{
		SteamNetworkingGlobalLock::AssertHeldByCurrentThread( "LagPacket" );

		int cbPkt = 0;
		for ( int i = 0 ; i < nChunks ; ++i )
			cbPkt += pChunks[i].iov_len;
		if ( cbPkt > k_cbSteamNetworkingSocketsMaxUDPMsgLen )
		{
			AssertMsg( false, "Tried to lag a packet that w as too big!" );
			return;
		}

		// Make sure we never queue a packet that is queued for destruction!
		if ( pSock->m_socket == INVALID_SOCKET || !pSock->m_callback.m_fnCallback )
		{
			AssertMsg( false, "Tried to lag a packet on a socket that has already been closed and is pending destruction!" );
			return;
		}

		if ( msDelay < 1 )
		{
			AssertMsg( false, "Packet lag time must be positive!" );
			msDelay = 1;
		}

		// Limit to something sane
		msDelay = std::min( msDelay, 5000 );
		const SteamNetworkingMicroseconds usecTime = SteamNetworkingSockets_GetLocalTimestamp() + msDelay * 1000;

		// Find the right place to insert the packet.  This is a dumb linear search, but in
		// the steady state where the delay is constant, this search loop won't actually iterate,
		// and we'll always be adding to the end of the queue
		LaggedPacket *pkt = nullptr;
		for ( CUtlLinkedList< LaggedPacket >::IndexType_t i = m_list.Tail(); i != m_list.InvalidIndex(); i = m_list.Previous( i ) )
		{
			if ( usecTime >= m_list[ i ].m_usecTime )
			{
				pkt = &m_list[ m_list.InsertAfter( i ) ];
				break;
			}
		}
		if ( pkt == nullptr )
		{
			pkt = &m_list[ m_list.AddToHead() ];
		}
	
		pkt->m_pSockOwner = pSock;
		pkt->m_adrRemote = adr;
		pkt->m_usecTime = usecTime;
		pkt->m_cbPkt = cbPkt;

		// Gather them into buffer
		char *d = pkt->m_pkt;
		for ( int i = 0 ; i < nChunks ; ++i )
		{
			int cbChunk = pChunks[i].iov_len;
			memcpy( d, pChunks[i].iov_base, cbChunk );
			d += cbChunk;
		}

		Schedule();
	}

	/// Periodic processing
	virtual void Think( SteamNetworkingMicroseconds usecNow ) OVERRIDE
	{

		// Just always process packets in queue order.  This means there could be some
		// weird burst or jankiness if the delay time is changed, but that's OK.
		while ( !m_list.IsEmpty() )
		{
			LaggedPacket &pkt = m_list[ m_list.Head() ];
			if ( pkt.m_usecTime > usecNow )
				break;

			// Make sure socket is still in good shape.
			CRawUDPSocketImpl *pSock = pkt.m_pSockOwner;
			if ( pSock )
			{
				if ( pSock->m_socket == INVALID_SOCKET || !pSock->m_callback.m_fnCallback )
				{
					AssertMsg( false, "Lagged packet remains in queue after socket destroyed or queued for destruction!" );
				}
				else
				{
					ProcessPacket( pkt, usecNow );
				}
			}
			m_list.RemoveFromHead();
		}

		Schedule();
	}

	/// Nuke everything
	void Clear()
	{
		m_list.RemoveAll();
		IThinker::ClearNextThinkTime();
	}

	/// Called when we're about to destroy a socket
	void AboutToDestroySocket( const CRawUDPSocketImpl *pSock )
	{
		// Just do a dumb linear search.  This list should be empty in
		// production situations, and socket destruction is relatively rare,
		// so its not worth making this complicated.
		int idx = m_list.Head();
		while ( idx != m_list.InvalidIndex() )
		{
			int idxNext = m_list.Next( idx );
			if ( m_list[idx].m_pSockOwner == pSock )
				m_list[idx].m_pSockOwner = nullptr;
			idx = idxNext;
		}
	}

protected:

	/// Set the next think time as appropriate
	void Schedule()
	{
		if ( m_list.IsEmpty() )
			ClearNextThinkTime();
		else
			SetNextThinkTime( m_list[ m_list.Head() ].m_usecTime );
	}

	struct LaggedPacket
	{
		CRawUDPSocketImpl *m_pSockOwner;
		netadr_t m_adrRemote;
		SteamNetworkingMicroseconds m_usecTime; /// Time when it should be sent or received
		int m_cbPkt;
		char m_pkt[ k_cbSteamNetworkingSocketsMaxUDPMsgLen ];
	};
	CUtlLinkedList<LaggedPacket> m_list;

	/// Do whatever we're supposed to do with the next packet
	virtual void ProcessPacket( const LaggedPacket &pkt, SteamNetworkingMicroseconds usecNow ) = 0;
};

class CPacketLaggerSend final : public CPacketLagger
{
public:
	virtual void ProcessPacket( const LaggedPacket &pkt, SteamNetworkingMicroseconds usecNow ) override
	{
		iovec temp;
		temp.iov_len = pkt.m_cbPkt;
		temp.iov_base = (void *)pkt.m_pkt;
		pkt.m_pSockOwner->BReallySendRawPacket( 1, &temp, pkt.m_adrRemote );
	}
};

class CPacketLaggerRecv final : public CPacketLagger
{
public:
	virtual void ProcessPacket( const LaggedPacket &pkt, SteamNetworkingMicroseconds usecNow ) override
	{
		// Copy data out of queue into local variables, just in case a
		// packet is queued while we're in this function.  We don't want
		// our list to shift in memory, and the pointer we pass to the
		// caller to dangle.
		char temp[ k_cbSteamNetworkingSocketsMaxUDPMsgLen ];
		memcpy( temp, pkt.m_pkt, pkt.m_cbPkt );
		//pkt.m_pSockOwner->m_callback( RecvPktInfo_t{ temp, pkt.m_cbPkt, usecNow, pkt.m_usecTime, 0, pkt.m_adrRemote, pkt.m_pSockOwner } );
		pkt.m_pSockOwner->m_callback( RecvPktInfo_t{ temp, pkt.m_cbPkt, usecNow, pkt.m_adrRemote, pkt.m_pSockOwner } );
	}
};

static CPacketLaggerSend s_packetLagQueueSend;
static CPacketLaggerRecv s_packetLagQueueRecv;

/// Object used to wake our background thread efficiently
#if defined( _WIN32 )
	static HANDLE s_hEventWakeThread = INVALID_HANDLE_VALUE;
#elif defined( NN_NINTENDO_SDK )
	static int s_hEventWakeThread = INVALID_SOCKET;
#else
	static SOCKET s_hSockWakeThreadRead = INVALID_SOCKET;
	static SOCKET s_hSockWakeThreadWrite = INVALID_SOCKET;
#endif

static std::thread *s_pThreadSteamDatagram = nullptr;

void WakeSteamDatagramThread()
{
	#if defined( _WIN32 )
		if ( s_hEventWakeThread != INVALID_HANDLE_VALUE )
			SetEvent( s_hEventWakeThread );
	#elif defined( NN_NINTENDO_SDK )
		// Sorry, but this code is covered under NDA with Nintendo, and
		// we don't have permission to distribute it.
	#else
		if ( s_hSockWakeThreadWrite != INVALID_SOCKET )
		{
			char buf[1] = {0};
			send( s_hSockWakeThreadWrite, buf, 1, 0 );
		}
	#endif
}

bool CRawUDPSocketImpl::BSendRawPacketGather( int nChunks, const iovec *pChunks, const netadr_t &adrTo ) const
{
	SteamNetworkingGlobalLock::AssertHeldByCurrentThread();

	// Silently ignore a request to send a packet anytime we're in the process of shutting down the system
	if ( s_nLowLevelSupportRefCount.load(std::memory_order_acquire) <= 0 )
		return true;

	// Check simulated global rate limit.  Make sure this is fast
	// when the limit is not in use
	if ( unlikely( g_Config_FakeRateLimit_Send_Rate.Get() > 0 ) )
	{

		// Check if bucket already has tokens in it, which
		// will be common.  If so, we can avoid reading the
		// timer
		if ( s_flFakeRateLimit_Send_tokens <= 0.0f )
		{

			// Update bucket with tokens
			UpdateFakeRateLimitTokenBuckets( SteamNetworkingSockets_GetLocalTimestamp() );

			// Still empty?
			if ( s_flFakeRateLimit_Send_tokens <= 0.0f )
				return true;
		}

		// Spend tokens
		int cbTotal = 0;
		for ( int i = 0 ; i < nChunks ; ++i )
			cbTotal += (int)pChunks[i].iov_len;
		s_flFakeRateLimit_Send_tokens -= cbTotal;
	}

	// Fake loss?
	if ( RandomBoolWithOdds( g_Config_FakePacketLoss_Send.Get() ) )
		return true;

	// Fake lag?
	int32 nPacketFakeLagTotal = g_Config_FakePacketLag_Send.Get();

	// Check for simulating random packet reordering
	if ( RandomBoolWithOdds( g_Config_FakePacketReorder_Send.Get() ) )
	{
		nPacketFakeLagTotal += g_Config_FakePacketReorder_Time.Get();
	}

	// Check for simulating random packet duplication
	if ( RandomBoolWithOdds( g_Config_FakePacketDup_Send.Get() ) )
	{
		int32 nDupLag = nPacketFakeLagTotal + WeakRandomInt( 0, g_Config_FakePacketDup_TimeMax.Get() );
		nDupLag = std::max( 1, nDupLag );
		s_packetLagQueueSend.LagPacket( const_cast<CRawUDPSocketImpl *>( this ), adrTo, nDupLag, nChunks, pChunks );
	}

	// Lag the original packet?
	if ( nPacketFakeLagTotal > 0 )
	{
		s_packetLagQueueSend.LagPacket( const_cast<CRawUDPSocketImpl *>( this ), adrTo, nPacketFakeLagTotal, nChunks, pChunks );
		return true;
	}

	// Now really send it
	return BReallySendRawPacket( nChunks, pChunks, adrTo );
}

void CRawUDPSocketImpl::InternalAddToCleanupQueue()
{

	/// Clear the callback, to ensure that no further callbacks will be executed.
	/// This marks the socket as pending destruction.
	Assert( m_callback.m_fnCallback );
	m_callback.m_fnCallback = nullptr;
	Assert( m_socket != INVALID_SOCKET );

	// Set global flag to remember that at least once socket needs to be cleaned up
	s_bRawSocketPendingDestruction = true;

	// Clean up lagged packets, if any
	s_packetLagQueueSend.AboutToDestroySocket( this );
	s_packetLagQueueRecv.AboutToDestroySocket( this );
}

void CRawUDPSocketImpl::Close()
{
	SteamNetworkingGlobalLock::AssertHeldByCurrentThread( "IRawUDPSocket::Close" );

	// Mark the callback as detached, and put us in the queue for cleanup when it's safe.
	InternalAddToCleanupQueue();

	// Check for Dual Wifi
	#ifdef STEAMNETWORKINGSOCKETS_ENABLE_DUALWIFI
		if ( m_pDualWifiPartner )
		{
			Assert( m_pDualWifiPartner->m_pDualWifiPartner == this );

			if ( m_eDualWifiStatus == k_EDualWifi_Primary )
			{
				Assert( m_pDualWifiPartner->m_eDualWifiStatus == k_EDualWifi_Secondary );
				m_pDualWifiPartner->InternalAddToCleanupQueue();
			}
			else
			{
				// People shouldn't do this, but if they do, let's not crash
				AssertMsg( false, "Closed secondary dual Wifi socket directly?" );
			}

			m_pDualWifiPartner->m_eDualWifiStatus = k_EDualWifi_Done;
			m_pDualWifiPartner->m_pDualWifiPartner = nullptr;

			m_eDualWifiStatus = k_EDualWifi_Done;
			m_pDualWifiPartner = nullptr;
		}
	#endif

	// Make sure we don't delay doing this too long
	if ( s_bManualPollMode || ( s_pThreadSteamDatagram && s_pThreadSteamDatagram->get_id() != std::this_thread::get_id() ) )
	{
		// Another thread might be polling right now
		WakeSteamDatagramThread();
	}
	else
	{
		// We can take care of it right now
		ProcessPendingDestroyClosedRawUDPSockets();
	}
}

static SOCKET OpenUDPSocketBoundToSockAddr( const void *sockaddr, size_t len, SteamDatagramErrMsg &errMsg, int *pnIPv6AddressFamilies, int nBindInterface = -1 )
{
	unsigned int opt;

	const sockaddr_in *inaddr = (const sockaddr_in *)sockaddr;

	// Select socket type.  For linux, use the "close on exec" flag, so that the
	// socket will not be inherited by any child process that we spawn.
	int sockType = SOCK_DGRAM;
	#ifdef LINUX
		sockType |= SOCK_CLOEXEC;
	#endif
	#if defined( NN_NINTENDO_SDK ) && !defined( _WIN32 )
		sockType |= SOCK_NONBLOCK;
	#endif

	// Try to create a UDP socket using the specified family
	SOCKET sock = socket( inaddr->sin_family, sockType, IPPROTO_UDP );
	if ( sock == INVALID_SOCKET )
	{
		V_sprintf_safe( errMsg, "socket() call failed.  Error code 0x%08x.", GetLastSocketError() );
		return INVALID_SOCKET;
	}

	// We always use nonblocking IO
	#if !defined( NN_NINTENDO_SDK ) || defined( _WIN32 )
		opt = 1;
		if ( ioctlsocket( sock, FIONBIO, (unsigned long*)&opt ) == -1 )
		{
			V_sprintf_safe( errMsg, "Failed to set socket nonblocking mode.  Error code 0x%08x.", GetLastSocketError() );
			closesocket( sock );
			return INVALID_SOCKET;
		}
	#endif

	// Set buffer sizes
	opt = g_cbUDPSocketBufferSize;
	if ( setsockopt( sock, SOL_SOCKET, SO_SNDBUF, (char *)&opt, sizeof(opt) ) )
	{
		V_sprintf_safe( errMsg, "Failed to set socket send buffer size.  Error code 0x%08x.", GetLastSocketError() );
		closesocket( sock );
		return INVALID_SOCKET;
	}
	opt = g_cbUDPSocketBufferSize;
	if ( setsockopt( sock, SOL_SOCKET, SO_RCVBUF, (char *)&opt, sizeof(opt) ) == -1 )
	{
		V_sprintf_safe( errMsg, "Failed to set socket recv buffer size.  Error code 0x%08x.", GetLastSocketError() );
		closesocket( sock );
		return INVALID_SOCKET;
	}

	// Handle IP v6 dual stack?
	if ( pnIPv6AddressFamilies )
	{

		// Enable dual stack?
		opt = ( *pnIPv6AddressFamilies == k_nAddressFamily_IPv6 ) ? 1 : 0;
		if ( setsockopt( sock, IPPROTO_IPV6, IPV6_V6ONLY, (char *)&opt, sizeof( opt ) ) != 0 )
		{
			if ( *pnIPv6AddressFamilies == k_nAddressFamily_IPv6 )
			{
				// Spew a warning, but continue
				SpewWarning( "Failed to set socket for IPv6 only (IPV6_V6ONLY=1).  Error code 0x%08X.  Continuing anyway.\n", GetLastSocketError() );
			}
			else
			{
				// Dual stack required, or only requested?
				if ( *pnIPv6AddressFamilies == k_nAddressFamily_DualStack )
				{
					V_sprintf_safe( errMsg, "Failed to set socket for dual stack (IPV6_V6ONLY=0).  Error code 0x%08X.", GetLastSocketError() );
					closesocket( sock );
					return INVALID_SOCKET;
				}

				// Let caller know we're IPv6 only, and spew about this.
				SpewWarning( "Failed to set socket for dual stack (IPV6_V6ONLY=0).  Error code 0x%08X.  Continuing using IPv6 only!\n", GetLastSocketError() );
				*pnIPv6AddressFamilies = k_nAddressFamily_IPv6;
			}
		}
		else
		{
			// Tell caller what they've got
			*pnIPv6AddressFamilies = opt ? k_nAddressFamily_IPv6 : k_nAddressFamily_DualStack;
		}
	}

	// Bind to particular interface
	if ( nBindInterface >= 0 )
	{
		#ifdef _WIN32
			Assert( nBindInterface != 0 ); // 0 is reserved, invalid value in Windows.

			// Bind to particular interface for IPv4
			if ( inaddr->sin_family == AF_INET || ( pnIPv6AddressFamilies && ( *pnIPv6AddressFamilies & k_nAddressFamily_IPv4 ) ) )
			{
				// WARNING: interface index should be in network byte order for IPPROTO_IP
				const DWORD value = htonl(nBindInterface);
				const int length = sizeof(value);
				const auto error = setsockopt( sock , IPPROTO_IP, IP_UNICAST_IF, reinterpret_cast<const char*>(&value), length);
				if (ERROR_SUCCESS != error)
				{
					V_sprintf_safe( errMsg, "sockopt(IP_PROTO_IP, IP_UNICAST_IF, %d) failed with error code 0x%08X.", nBindInterface, GetLastSocketError() );
					closesocket( sock );
					return INVALID_SOCKET;
				}
				SpewVerbose( "sockopt(IP_PROTO_IP, IP_UNICAST_IF, %d) OK\n", nBindInterface );
			}

			// Bind to particular interface for IPv6
			if ( inaddr->sin_family == AF_INET6 || ( pnIPv6AddressFamilies && ( *pnIPv6AddressFamilies & k_nAddressFamily_IPv6 ) ) )
			{
				// WARNING: interface index should be in host byte order for IPPROTO_IPV6
				auto length = static_cast<int>(sizeof(nBindInterface));
				const auto error = setsockopt( sock, IPPROTO_IPV6, IPV6_UNICAST_IF, reinterpret_cast<const char*>(&nBindInterface), length);
				if (ERROR_SUCCESS != error)
				{
					V_sprintf_safe( errMsg, "sockopt(IPPROTO_IPV6, IPV6_UNICAST_IF, %d) failed with error code 0x%08X.", nBindInterface, GetLastSocketError() );
					closesocket( sock );
					return INVALID_SOCKET;
				}
				SpewVerbose( "sockopt(IPPROTO_IPV6, IPV6_UNICAST_IF, %d) OK\n", nBindInterface );
			}
		#endif
	}

	// Bind it to specific desired local port/IP
	if ( bind( sock, (struct sockaddr *)sockaddr, (socklen_t)len ) == -1 )
	{
		V_sprintf_safe( errMsg, "Failed to bind socket.  Error code 0x%08X.", GetLastSocketError() );
		closesocket( sock );
		return INVALID_SOCKET;
	}

	// All good
	return sock;
}

static CRawUDPSocketImpl *OpenRawUDPSocketInternal( CRecvPacketCallback callback, SteamDatagramErrMsg &errMsg, const SteamNetworkingIPAddr *pAddrLocal, int *pnAddressFamilies, int nBindInterface = -1 )
{
	// Creating a socket *should* be fast, but sometimes the OS might need to do some work.
	// We shouldn't do this too often, give it a little extra time.
	SteamNetworkingGlobalLock::SetLongLockWarningThresholdMS( "OpenRawUDPSocketInternal", 100 );

	// Make sure have been initialized
	if ( s_nLowLevelSupportRefCount.load(std::memory_order_acquire) <= 0 )
	{
		V_strcpy_safe( errMsg, "Internal order of operations bug.  Can't create socket, because low level systems not initialized" );
		AssertMsg( false, errMsg );
		return nullptr;
	}

	// Supply defaults
	int nAddressFamilies = pnAddressFamilies ? *pnAddressFamilies : k_nAddressFamily_Auto;
	SteamNetworkingIPAddr addrLocal;
	if ( pAddrLocal )
		addrLocal = *pAddrLocal;
	else
		addrLocal.Clear();

	// Check that the request makes sense
	if ( addrLocal.IsIPv4() )
	{
		// Only IPv4 family allowed, don't even try IPv6
		if ( nAddressFamilies == k_nAddressFamily_Auto )
		{
			nAddressFamilies = k_nAddressFamily_IPv4;
		}
		else if ( nAddressFamilies != k_nAddressFamily_IPv4 )
		{
			V_strcpy_safe( errMsg, "Invalid address family request when binding to IPv4 address" );
			return nullptr;
		}
	}
	else if ( addrLocal.IsIPv6AllZeros() )
	{
		// We can try IPv6 dual stack, and fallback to IPv4 if requested.
		// Just make sure they didn't request a totally bogus value
		if ( nAddressFamilies == 0 )
		{
			V_strcpy_safe( errMsg, "Invalid address families" );
			return nullptr;
		}
	}
	else
	{
		// Only IPv6 family allowed, cannot try IPv4
		if ( nAddressFamilies == k_nAddressFamily_Auto )
		{
			nAddressFamilies = k_nAddressFamily_IPv6;
		}
		else if ( nAddressFamilies != k_nAddressFamily_IPv6 )
		{
			V_strcpy_safe( errMsg, "Invalid address family request when binding to IPv6 address" );
			return nullptr;
		}
	}

	// Try IPv6?
	SOCKET sock = INVALID_SOCKET;
	if ( nAddressFamilies & k_nAddressFamily_IPv6 )
	{
		sockaddr_in6 address6;
		memset( &address6, 0, sizeof(address6) );
		address6.sin6_family = AF_INET6;
		memcpy( address6.sin6_addr.s6_addr, addrLocal.m_ipv6, 16 );
		address6.sin6_port = BigWord( addrLocal.m_port );

		// Try to get socket
		int nIPv6AddressFamilies = nAddressFamilies;
		sock = OpenUDPSocketBoundToSockAddr( &address6, sizeof(address6), errMsg, &nIPv6AddressFamilies, nBindInterface );

		if ( sock == INVALID_SOCKET )
		{
			// Allowing fallback to IPv4?
			if ( nAddressFamilies != k_nAddressFamily_Auto )
				return nullptr;

			// Continue below, we'll try IPv4
		}
		else
		{
			nAddressFamilies = nIPv6AddressFamilies;
		}
	}

	// Try IPv4?
	if ( sock == INVALID_SOCKET )
	{
		Assert( nAddressFamilies & k_nAddressFamily_IPv4 ); // Otherwise, we should have already failed above

		sockaddr_in address4;
		memset( &address4, 0, sizeof(address4) );
		address4.sin_family = AF_INET;
		address4.sin_addr.s_addr = BigDWord( addrLocal.GetIPv4() );
		address4.sin_port = BigWord( addrLocal.m_port );

		// Try to get socket
		sock = OpenUDPSocketBoundToSockAddr( &address4, sizeof(address4), errMsg, nullptr, nBindInterface );

		// If we failed, well, we have no other options left to try.
		if ( sock == INVALID_SOCKET )
			return nullptr;

		// We re IPv4 only
		nAddressFamilies = k_nAddressFamily_IPv4;
	}

	// Read back address we actually bound to.
	sockaddr_storage addrBound;
	socklen_t cbAddress = sizeof(addrBound);
	if ( getsockname( sock, (struct sockaddr *)&addrBound, &cbAddress ) != 0 )
	{
		V_sprintf_safe( errMsg, "getsockname failed.  Error code 0x%08X.", GetLastSocketError() );
		closesocket( sock );
		return nullptr;
	}
	if ( addrBound.ss_family == AF_INET )
	{
		const sockaddr_in *boundaddr4 = (const sockaddr_in *)&addrBound;
		addrLocal.SetIPv4( BigDWord( boundaddr4->sin_addr.s_addr ), BigWord( boundaddr4->sin_port ) );
	}
	else if ( addrBound.ss_family == AF_INET6 )
	{
		const sockaddr_in6 *boundaddr6 = (const sockaddr_in6 *)&addrBound;
		addrLocal.SetIPv6( boundaddr6->sin6_addr.s6_addr, BigWord( boundaddr6->sin6_port ) );
	}
	else
	{
		Assert( false );
		V_sprintf_safe( errMsg, "getsockname returned address with unexpected family %d", addrBound.ss_family );
		closesocket( sock );
		return nullptr;
	}

	// Allocate a bookkeeping structure
	CRawUDPSocketImpl *pSock = new CRawUDPSocketImpl;
	pSock->m_socket = sock;
	pSock->m_boundAddr = addrLocal;
	pSock->m_callback = callback;
	pSock->m_nAddressFamilies = nAddressFamilies;

	// On windows, tell the socket to set our global "wake" event whenever there is data to read
	#ifdef _WIN32
		Assert( s_hEventWakeThread != INVALID_HANDLE_VALUE );
		if ( WSAEventSelect( pSock->m_socket, s_hEventWakeThread, FD_READ ) != 0 )
		{
			delete pSock;
			V_sprintf_safe( errMsg, "WSAEventSelect() failed.  Error code 0x%08X.", GetLastSocketError() );
			return nullptr;
		}
	#endif

	// Add to master list.  (Hopefully we usually won't have that many.)
	s_vecRawSockets.AddToTail( pSock );

	// Rebuild our pollfd list next time
	#ifndef _WIN32
		s_bRecreatePollList = true;
	#endif

	// Wake up background thread so we can start receiving packets on this socket immediately
	WakeSteamDatagramThread();

	// Give back info on address families
	if ( pnAddressFamilies )
		*pnAddressFamilies = nAddressFamilies;

	// Give them something they can use
	return pSock;
}

IRawUDPSocket *OpenRawUDPSocket( CRecvPacketCallback callback, SteamDatagramErrMsg &errMsg, SteamNetworkingIPAddr *pAddrLocal, int *pnAddressFamilies )
{
	return OpenRawUDPSocketInternal( callback, errMsg, pAddrLocal, pnAddressFamilies );
}

static inline void AssertGlobalLockHeldExactlyOnce()
{
	#if STEAMNETWORKINGSOCKETS_LOCK_DEBUG_LEVEL > 0
		ThreadLockDebugInfo &t = GetThreadDebugInfo();
		Assert( t.m_nHeldLocks == 1 && t.m_arHeldLocks[0] == &s_mutexGlobalLock );
	#endif
}

/// Wait on our events / sockets.
/// Returns true if wake event is set or some socket is ready to be read.
/// Otherwise, we are idle, and returns false.
static bool WaitForSocketsOrWakeEvent( int nMaxTimeoutMS )
{

	// Wait for data on one of the sockets, or for us to be asked to wake up
	#if defined( WIN32 )
		DWORD r = WaitForSingleObject( s_hEventWakeThread, nMaxTimeoutMS );
		if ( r == WAIT_TIMEOUT )
			return false; // Idle
		Assert( r == WAIT_OBJECT_0 );
		return true;
	#else
		int r = poll( s_vecPollFDs.Base(), s_vecPollFDs.Count(), nMaxTimeoutMS );
		if ( r == 0 )
			return false; // Idle
		Assert( r >= 0 );
		return true;
	#endif
}


/// Poll all of our sockets, and dispatch the packets received.
/// This will return true if we own the lock, or false if we detected
/// a shutdown request and bailed without re-squiring the lock.
static bool PollRawUDPSockets( int nMaxTimeoutMS, bool bManualPoll )
{
	// This should only ever be called from our one thread proc,
	// and we assume that it will have locked the lock exactly once.
	AssertGlobalLockHeldExactlyOnce();

	// Sanity check all of our sockets are alive
	for ( CRawUDPSocketImpl *pSock: s_vecRawSockets )
	{
		Assert( pSock->m_callback.m_fnCallback );
	}

	const int nSocketsToPoll = s_vecRawSockets.Count();

	// Recreate pollfd list if needed
	#ifndef _WIN32
		if ( s_bRecreatePollList )
		{
			s_bRecreatePollList = false;
			s_vecPollFDs.EnsureCapacity( nSocketsToPoll+1 );
			s_vecPollFDs.SetCount(0);
			#define ADD_POLL_FD(f) { \
				pollfd &p = *s_vecPollFDs.AddToTailGetPtr(); \
				Assert( f != INVALID_SOCKET ); \
				p.fd = f; \
				p.events = POLLRDNORM; \
				p.revents = -1; /* Make sure kernel is clearing events properly */ \
			}

			for ( CRawUDPSocketImpl *pSock: s_vecRawSockets )
				ADD_POLL_FD( pSock->m_socket );

			#ifdef NN_NINTENDO_SDK
				ADD_POLL_FD( s_hEventWakeThread );
			#else
				ADD_POLL_FD( s_hSockWakeThreadRead );
			#endif
		}
		Assert( s_vecPollFDs.Count() == nSocketsToPoll+1 );
	#endif

	// Release lock while we're asleep
	SteamNetworkingGlobalLock::Unlock();

	// Run idle tasks since we don't have the lock.
	// !FIXME! Should we move this to another thread?
	g_taskListRunInBackground.RunTasks();

	// If we have spewed, flush to disk.
	// We probably could use the background task system for this.
	FlushSystemSpew();

	// Shutdown request?
	if ( s_nLowLevelSupportRefCount.load(std::memory_order_acquire) <= 0 || s_bManualPollMode != bManualPoll )
		return false; // ABORT THREAD

	// Wait for data on one of the sockets, or for us to be asked to wake up
	WaitForSocketsOrWakeEvent( nMaxTimeoutMS );

	// We're back awake.  Grab the lock again
	SteamNetworkingMicroseconds usecStartedLocking = SteamNetworkingSockets_GetLocalTimestamp();
	UpdateFakeRateLimitTokenBuckets( usecStartedLocking );
	for (;;)
	{

		// Shutdown request?  We've potentially been waiting a long time.
		// Don't attempt to grab the lock again if we know we want to shutdown,
		// that is just a waste of time.
		if ( s_nLowLevelSupportRefCount.load(std::memory_order_acquire) <= 0 || s_bManualPollMode != bManualPoll )
			return false;

		// Try to acquire the lock.  But don't wait forever, in case the other thread has
		// the lock and then makes a shutdown request while we're waiting on the lock here.
		if ( SteamNetworkingGlobalLock::TryLock( "ServiceThread", 20 ) )
			break;
	}

	// If we waited a long time, then that's probably bad.  Spew about it
	{
		SteamNetworkingMicroseconds usecElapsedWaitingForLock = SteamNetworkingSockets_GetLocalTimestamp() - usecStartedLocking;
		// Hm, if another thread indicated that they expected to hold the lock for a while,
		// perhaps we should ignore this assert?
		AssertMsg1( usecElapsedWaitingForLock < 50*1000 || Plat_IsInDebugSession(),
			"SteamnetworkingSockets service thread waited %dms for lock!  This directly adds to network latency!  It could be a bug, but it's usually caused by general performance problem such as thread starvation or a debug output handler taking too long.", int( usecElapsedWaitingForLock/1000 ) );
	}

	// Recv socket data from any sockets that might have data, and execute the callbacks.
	char buf[ k_cbSteamNetworkingSocketsMaxUDPMsgLen + 1024 ];

	// Iterate all sockets
	for ( int idx = 0 ; idx < nSocketsToPoll ; ++idx )
	{
		CRawUDPSocketImpl *pSock = s_vecRawSockets[ idx ];

		// Check if this socket has anything
		#ifdef _WIN32
			WSANETWORKEVENTS wsaEvents;
			if ( WSAEnumNetworkEvents( pSock->m_socket, NULL, &wsaEvents ) != 0 )
			{
				AssertMsg1( false, "WSAEnumNetworkEvents failed.  Error code %08x", WSAGetLastError() );
				continue;
			}
			if ( !(wsaEvents.lNetworkEvents & FD_READ) )
				continue;
		#else
			if ( !( s_vecPollFDs[ idx ].revents & POLLRDNORM ) )
				continue;
			Assert( s_vecPollFDs[ idx ].revents != -1 ); // Make sure kernel actually populated this
		#endif

		// Drain the socket.  But if the callback gets cleared, that
		// indicates that the socket is pending destruction and is
		// logically closed to the calling code.
		while ( pSock->m_callback.m_fnCallback )
		{
			if ( s_nLowLevelSupportRefCount.load(std::memory_order_acquire) <= 0 )
				return true; // current thread owns the lock

			#ifdef STEAMNETWORKINGSOCKETS_LOWLEVEL_TIME_SOCKET_CALLS
				SteamNetworkingMicroseconds usecRecvFromStart = SteamNetworkingSockets_GetLocalTimestamp();
			#endif

			sockaddr_storage from;
			socklen_t fromlen = sizeof(from);
			int ret = ::recvfrom( pSock->m_socket, buf, sizeof( buf ), 0, (sockaddr *)&from, &fromlen );
			SteamNetworkingMicroseconds usecRecvFromEnd = SteamNetworkingSockets_GetLocalTimestamp();

			#ifdef STEAMNETWORKINGSOCKETS_LOWLEVEL_TIME_SOCKET_CALLS
				if ( usecRecvFromEnd > s_usecIgnoreLongLockWaitTimeUntil )
				{
					SteamNetworkingMicroseconds usecRecvFromElapsed = usecRecvFromEnd - usecRecvFromStart;
					if ( usecRecvFromElapsed > 1000 )
					{
						SpewWarning( "recvfrom took %.1fms\n", usecRecvFromElapsed*1e-3 );
						ETW_LongOp( "UDP recvfrom", usecRecvFromElapsed );
					}
				}
			#endif

			// Negative value means nothing more to read.
			//
			// NOTE 1: We're not checking the cause of failure.  Usually it would be "EWOULDBLOCK",
			// meaning no more data.  However if there was some socket error (i.e. somebody did something
			// to reset the network stack, etc) we could make the code more robust by detecting this.
			// It would require us plumbing through this failure somehow, and all we have here is a callback
			// for processing packets.  Probably not worth the effort to handle this relatively common case.
			// It will just appear to the app that the cord is cut on this socket.
			//
			// NOTE 2: 0 byte datagram is possible, and in this case recvfrom will return 0.
			// (But all of our protocols enforce a minimum packet size, so if we get a zero byte packet,
			// it's a bogus.  We could drop it here but let's send it through the normal mechanism to
			// be handled/reported in the same way as any other bogus packet.)
			if ( ret < 0 )
				break;

			// Add a tag.  If we end up holding the lock for a long time, this tag
			// will tell us how many packets were processed
			SteamNetworkingGlobalLock::AssertHeldByCurrentThread( "RecvUDPPacket" );

			// Check simulated global rate limit.  Make sure this is fast
			// when the limit is not in use
			if ( unlikely( g_Config_FakeRateLimit_Recv_Rate.Get() > 0 ) )
			{

				// Check if bucket already has tokens in it, which
				// will be common.  If so, we can avoid reading the
				// timer
				if ( s_flFakeRateLimit_Recv_tokens <= 0.0f )
				{

					// Update bucket with tokens
					UpdateFakeRateLimitTokenBuckets( usecRecvFromEnd );

					// Still empty?
					if ( s_flFakeRateLimit_Recv_tokens <= 0.0f )
						continue;
				}

				// Spend tokens
				s_flFakeRateLimit_Recv_tokens -= ret;
			}

			// Check for simulating random packet loss
			if ( RandomBoolWithOdds( g_Config_FakePacketLoss_Recv.Get() ) )
				continue;

			RecvPktInfo_t info;
			info.m_adrFrom.SetFromSockadr( &from );

			// If we're dual stack, convert mapped IPv4 back to ordinary IPv4
			if ( pSock->m_nAddressFamilies == k_nAddressFamily_DualStack )
				info.m_adrFrom.BConvertMappedToIPv4();

			// Check for tracing
			if ( g_Config_PacketTraceMaxBytes.Get() >= 0 )
			{
				iovec tmp;
				tmp.iov_base = buf;
				tmp.iov_len = ret;
				pSock->TracePkt( false, info.m_adrFrom, 1, &tmp );
			}

			int32 nPacketFakeLagTotal = g_Config_FakePacketLag_Recv.Get();

			// Check for simulating random packet reordering
			if ( RandomBoolWithOdds( g_Config_FakePacketReorder_Recv.Get() ) )
			{
				nPacketFakeLagTotal += g_Config_FakePacketReorder_Time.Get();
			}

			// Check for simulating random packet duplication
			if ( RandomBoolWithOdds( g_Config_FakePacketDup_Recv.Get() ) )
			{
				int32 nDupLag = nPacketFakeLagTotal + WeakRandomInt( 0, g_Config_FakePacketDup_TimeMax.Get() );
				nDupLag = std::max( 1, nDupLag );
				iovec temp;
				temp.iov_len = ret;
				temp.iov_base = buf;
				s_packetLagQueueRecv.LagPacket( pSock, info.m_adrFrom, nDupLag, 1, &temp );
			}

			// Check for simulating lag
			if ( nPacketFakeLagTotal > 0 )
			{
				iovec temp;
				temp.iov_len = ret;
				temp.iov_base = buf;
				s_packetLagQueueRecv.LagPacket( pSock, info.m_adrFrom, nPacketFakeLagTotal, 1, &temp );
			}
			else
			{
				ETW_UDPRecvPacket( info.m_adrFrom, ret );

				info.m_pPkt = buf;
				info.m_cbPkt = ret;
				info.m_usecNow = usecRecvFromEnd;
				info.m_pSock = pSock;
				pSock->m_callback( info );
			}

			#ifdef STEAMNETWORKINGSOCKETS_LOWLEVEL_TIME_SOCKET_CALLS
				SteamNetworkingMicroseconds usecProcessPacketEnd = SteamNetworkingSockets_GetLocalTimestamp();
				if ( usecProcessPacketEnd > s_usecIgnoreLongLockWaitTimeUntil )
				{
					SteamNetworkingMicroseconds usecProcessPacketElapsed = usecProcessPacketEnd - usecRecvFromEnd;
					if ( usecProcessPacketElapsed > 1000 )
					{
						SpewWarning( "process packet took %.1fms\n", usecProcessPacketElapsed*1e-3 );
						ETW_LongOp( "process packet", usecProcessPacketElapsed );
					}
				}
			#endif
		}
	}

	// On POSIX, check for draining the thread wake "socket"
	#ifndef _WIN32
		pollfd &wake = s_vecPollFDs[ nSocketsToPoll ];
		if ( wake.revents & POLLRDNORM )
		{
			Assert( wake.revents != -1 );

			// It's a wake request.  Pull a single packet out of the queue.
			// We want one wake request to always result in exactly one wake up.
			// Wake request are relatively rare, and we don't want to skip any
			// or combine them.  That would result in complicated race conditions
			// where we stay asleep a lot longer than we should.
			#ifdef NN_NINTENDO_SDK
				// Sorry, but this code is covered under NDA with Nintendo, and
				// we don't have permission to distribute it.
			#else
				Assert( wake.fd == s_hSockWakeThreadRead );
				::recv( s_hSockWakeThreadRead, buf, sizeof(buf), 0 );
			#endif
		}
	#endif

	// We retained the lock
	return true;
}

void ProcessPendingDestroyClosedRawUDPSockets()
{
	SteamNetworkingGlobalLock::AssertHeldByCurrentThread();

	if ( !s_bRawSocketPendingDestruction )
		return;
	s_bRawSocketPendingDestruction = false;

	for ( int i = s_vecRawSockets.Count()-1 ; i >= 0 ; --i )
	{
		if ( !s_vecRawSockets[i]->m_callback.m_fnCallback )
		{
			delete s_vecRawSockets[i];
			s_vecRawSockets.Remove( i );
		}
	}
	Assert( !s_bRawSocketPendingDestruction );

	#ifndef _WIN32
		s_bRecreatePollList = true;
	#endif

}

static void ProcessDeferredOperations()
{
	SteamNetworkingGlobalLock::AssertHeldByCurrentThread();

	// Tasks that were queued to be run while we hold the lock
	g_taskListRunWithGlobalLock.RunTasks();

	// Process any connections queued for delete
	CSteamNetworkConnectionBase::ProcessDeletionList();

	// Close any sockets pending delete, if we discarded a server
	// We can close the sockets safely now, because we know we're
	// not polling on them and we know we hold the lock
	ProcessPendingDestroyClosedRawUDPSockets();
}

/////////////////////////////////////////////////////////////////////////////
//
// Dual Wifi band support
//
/////////////////////////////////////////////////////////////////////////////

#ifdef STEAMNETWORKINGSOCKETS_ENABLE_DUALWIFI

#include <wlanapi.h>
static int s_ifaceDualWifiSecondary = -1; // -1 means we haven't tried yet.  Any other negative value means, we tried and failed
static HANDLE wlanHandle = INVALID_HANDLE_VALUE;

static HMODULE hModuleWlanAPI = NULL;

static 
DWORD
(WINAPI *pWlanOpenHandle)(
    DWORD dwClientVersion,
    PVOID pReserved,
    PDWORD pdwNegotiatedVersion,
    PHANDLE phClientHandle
);

static
DWORD
(WINAPI *pWlanCloseHandle)(
    HANDLE hClientHandle,
    PVOID pReserved
);

static
DWORD
(WINAPI *pWlanEnumInterfaces)(
    HANDLE hClientHandle,
    PVOID pReserved,
    PWLAN_INTERFACE_INFO_LIST *ppInterfaceList
);

static
DWORD
( WINAPI *pWlanSetInterface)(
    HANDLE hClientHandle,
    CONST GUID *pInterfaceGuid,
    WLAN_INTF_OPCODE OpCode,
    DWORD dwDataSize,
    CONST PVOID pData,
    PVOID pReserved
);

static
DWORD
( WINAPI *pWlanQueryInterface)(
    HANDLE hClientHandle,
    CONST GUID *pInterfaceGuid,
    WLAN_INTF_OPCODE OpCode,
    PVOID pReserved,
    PDWORD pdwDataSize,
    PVOID *ppData,
    PWLAN_OPCODE_VALUE_TYPE pWlanOpcodeValueType
);



void DualWifiShutdown()
{
	if ( wlanHandle != INVALID_HANDLE_VALUE )
	{
		(*pWlanCloseHandle)( wlanHandle, nullptr );
		wlanHandle = INVALID_HANDLE_VALUE;
	}
	s_ifaceDualWifiSecondary = -1;
}

#if WDK_NTDDI_VERSION < 0x0A00000B	// These definitions are available in wlanapi.h in Windows SDK 10.0.22000.0
// !KLUDGE! Pasted from an early version of wlan.h.
namespace wlan_new
{
#ifdef __midl
// use the 4-byte enum
typedef [v1_enum] enum _WLAN_INTF_OPCODE {
#else
typedef enum _WLAN_INTF_OPCODE {
#endif
    wlan_intf_opcode_autoconf_start = 0x000000000,
    wlan_intf_opcode_autoconf_enabled,
    wlan_intf_opcode_background_scan_enabled,
    wlan_intf_opcode_media_streaming_mode,
    wlan_intf_opcode_radio_state,
    wlan_intf_opcode_bss_type,
    wlan_intf_opcode_interface_state,
    wlan_intf_opcode_current_connection,
    wlan_intf_opcode_channel_number,
    wlan_intf_opcode_supported_infrastructure_auth_cipher_pairs,
    wlan_intf_opcode_supported_adhoc_auth_cipher_pairs,
    wlan_intf_opcode_supported_country_or_region_string_list,
    wlan_intf_opcode_current_operation_mode,
    wlan_intf_opcode_supported_safe_mode,
    wlan_intf_opcode_certified_safe_mode,
    wlan_intf_opcode_hosted_network_capable,
    wlan_intf_opcode_management_frame_protection_capable,
    wlan_intf_opcode_secondary_sta_interfaces,
    wlan_intf_opcode_secondary_sta_synchronized_connections,
    wlan_intf_opcode_autoconf_end = 0x0fffffff,
    wlan_intf_opcode_msm_start = 0x10000100,
    wlan_intf_opcode_statistics,
    wlan_intf_opcode_rssi,
    wlan_intf_opcode_msm_end = 0x1fffffff,
    wlan_intf_opcode_security_start = 0x20010000,
    wlan_intf_opcode_security_end = 0x2fffffff,
    wlan_intf_opcode_ihv_start = 0x30000000,
    wlan_intf_opcode_ihv_end = 0x3fffffff
} WLAN_INTF_OPCODE, *PWLAN_INTF_OPCODE;
}
const WLAN_INTF_OPCODE wlan_intf_opcode_secondary_sta_synchronized_connections = (WLAN_INTF_OPCODE)wlan_new::wlan_intf_opcode_secondary_sta_synchronized_connections;
const WLAN_INTF_OPCODE wlan_intf_opcode_secondary_sta_interfaces = (WLAN_INTF_OPCODE)wlan_new::wlan_intf_opcode_secondary_sta_interfaces;
#endif // WDK_NTDDI_VERSION < 0x0A00000B

static void DualWifiInitFailed( const char *fmt, ... )
{
	va_list ap;
	va_start( ap, fmt );
	char buf[ 512 ];
	V_vsprintf_safe( buf, fmt, ap );
	SpewMsg( "DualWifi not detected.  We won't try again.  %s\n", buf );
	DualWifiShutdown();
	s_ifaceDualWifiSecondary = -2; // but remember that we failed
}

static int ConvertInterfaceGuidToIndex(const GUID& interfaceGuid)
{
	//
	// These functions were added with Vista, so load dynamically
	// in case
	//

	typedef
	NETIO_STATUS
	(NETIOAPI_API_*FnConvertInterfaceGuidToLuid)(
		_In_ CONST GUID *InterfaceGuid,
		_Out_ PNET_LUID InterfaceLuid
		);
	typedef 
	NETIO_STATUS
	(NETIOAPI_API_*FnConvertInterfaceLuidToIndex)(
		_In_ CONST NET_LUID *InterfaceLuid,
		_Out_ PNET_IFINDEX InterfaceIndex
    );

	static HMODULE hModule = LoadLibraryA( "Iphlpapi.dll" );
	static FnConvertInterfaceGuidToLuid pConvertInterfaceGuidToLuid = hModule ? (FnConvertInterfaceGuidToLuid)GetProcAddress( hModule, "ConvertInterfaceGuidToLuid" ) : nullptr;
	static FnConvertInterfaceLuidToIndex pConvertInterfaceLuidToIndex = hModule ? (FnConvertInterfaceLuidToIndex)GetProcAddress( hModule, "ConvertInterfaceLuidToIndex" ) : nullptr;;
	if ( !pConvertInterfaceGuidToLuid || !pConvertInterfaceLuidToIndex )
	{
		AssertMsg( false, "How did I get here?" );
		return -1;
	}

    NET_LUID interfaceLuid{};
    auto error = (pConvertInterfaceGuidToLuid)(&interfaceGuid, &interfaceLuid);
    if ( ERROR_SUCCESS != error )
	{
		AssertMsg( false, "ConvertInterfaceGuidToLuid failed 0x%x", error );
		return -1;
	}

    NET_IFINDEX interfaceIndex = 0;
    error = (*pConvertInterfaceLuidToIndex)(&interfaceLuid, &interfaceIndex);
    if ( ERROR_SUCCESS != error )
	{
		AssertMsg( false, "ConvertInterfaceLuidToIndex failed 0x%x", error );
		return -1;
	}

    return static_cast<int>(interfaceIndex);
}

class RenderGUID
{
	char buf[64];
public:
	RenderGUID( const GUID &guid )
	{
		// https://stackoverflow.com/a/18114061/8004137
		V_sprintf_safe( buf, "{%08lX-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX}",
			guid.Data1, guid.Data2, guid.Data3, 
			guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
			guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
	}
	const char *c_str() const { return buf; }
};

template <typename F>
bool MyGetProcAddress( F& fn, HMODULE hm, const char *pszName )
{
	if ( hm == NULL )
		fn = nullptr;
	else
		fn = (F)GetProcAddress( hm, pszName );
	return fn != nullptr;
}

static int GetDualWifiSecondaryInterfaceIndex( int nSimulateMode )
{
	// First time attempt?
	// FIXME - it's not clear to me when I should retry
	if ( s_ifaceDualWifiSecondary != -1 )
		return s_ifaceDualWifiSecondary;

	// Dynamically load wlanapi.dll the first time.
	if ( hModuleWlanAPI == NULL )
	{
		hModuleWlanAPI = LoadLibraryA( "wlanapi.dll" );
		if (
			!MyGetProcAddress( pWlanOpenHandle, hModuleWlanAPI, "WlanOpenHandle" )
			|| !MyGetProcAddress( pWlanCloseHandle, hModuleWlanAPI, "WlanCloseHandle" )
			|| !MyGetProcAddress( pWlanEnumInterfaces, hModuleWlanAPI, "WlanEnumInterfaces" )
			|| !MyGetProcAddress( pWlanSetInterface, hModuleWlanAPI, "WlanSetInterface" )
			|| !MyGetProcAddress( pWlanQueryInterface, hModuleWlanAPI, "WlanQueryInterface" )
		) {
			DualWifiInitFailed( "Failed to load wlanAPI.DLL" );
			return -1;
		}
	}

	// First time we need to open Wlan session
	if ( wlanHandle == INVALID_HANDLE_VALUE )
	{
		DWORD clientVersion = 2; // Vista+ APIs
		DWORD curVersion = 0;
		DWORD error = (*pWlanOpenHandle)(clientVersion, nullptr, &curVersion, &wlanHandle );
		if ( ERROR_SUCCESS != error || wlanHandle == INVALID_HANDLE_VALUE )
		{
			DualWifiInitFailed( "WlanOpenHandle failed 0x%x.", error );
			return -1;
		}
	}

	PWLAN_INTERFACE_INFO_LIST primaryInterfaceList = nullptr;
	DWORD error = (*pWlanEnumInterfaces)(wlanHandle, nullptr, &primaryInterfaceList);
	if ( ERROR_SUCCESS != error || !primaryInterfaceList )
	{
		DualWifiInitFailed( "WlanEnumInterfaces failed 0x%x.", error );
		return -1;
	}

	if ( nSimulateMode == k_nDualWifiEnable_DoNotEnumerate )
	{
		DualWifiInitFailed( "Not really checking for capable adapters as per DualWifi_Enable=%d", nSimulateMode );
		s_ifaceDualWifiSecondary = -1;
		return -1;
	}

	// Look for the first Wireless interface that we can enable DualSTA for
	for ( DWORD idxPrimary = 0 ; idxPrimary < primaryInterfaceList->dwNumberOfItems ; ++idxPrimary )
	{
		const GUID primaryInterfaceGuid = primaryInterfaceList->InterfaceInfo[idxPrimary].InterfaceGuid;

		// Get adapter name in UTF8
		char szInterfaceDescription[ 256 ];
		memset( szInterfaceDescription, 0, sizeof(szInterfaceDescription) );
		WideCharToMultiByte(
			CP_UTF8, // codepage
			0, // flags
			primaryInterfaceList->InterfaceInfo[idxPrimary].strInterfaceDescription, -1, // input string and length
			szInterfaceDescription, sizeof(szInterfaceDescription)-1, // output buffer and SIZE in bytes
			nullptr, nullptr // no default char, and we don't care if it was used
		);

		// Try to enable the feature on this adapter.  This is where most adapters should fail.
		BOOL enable = TRUE;
		error = (*pWlanSetInterface)(
			wlanHandle, &primaryInterfaceGuid, wlan_intf_opcode_secondary_sta_synchronized_connections, sizeof(BOOL), static_cast<PVOID>(&enable), nullptr);
		if ( ERROR_SUCCESS != error )
		{
			SpewVerbose( "Dual Wifi support not detected on adapter '%s' (wlan_intf_opcode_secondary_sta_synchronized_connections returned 0x%x)\n", szInterfaceDescription, error );
			continue;
		}

		// Feature is detected!
		SpewMsg( "Dual Wifi support enabled successfully on adapter '%s'\n", szInterfaceDescription );

		PWLAN_INTERFACE_INFO_LIST secondaryInterfaceList = nullptr;
		DWORD dataSize = 0;
		error = (*pWlanQueryInterface)(
			wlanHandle,
			&primaryInterfaceGuid,
			wlan_intf_opcode_secondary_sta_interfaces,
			nullptr,
			&dataSize,
			reinterpret_cast<PVOID*>(&secondaryInterfaceList),
			nullptr);
		if ( ERROR_SUCCESS != error || !secondaryInterfaceList )
		{
			AssertMsg( false, "wlan_intf_opcode_secondary_sta_synchronized_connections succeeded, but wlan_intf_opcode_secondary_sta_interfaces failed 0x%x?", error );
			continue;
		}

		for ( DWORD idxSecondary = 0 ; idxSecondary < secondaryInterfaceList->dwNumberOfItems ; ++idxSecondary )
		{
			s_ifaceDualWifiSecondary = ConvertInterfaceGuidToIndex( secondaryInterfaceList->InterfaceInfo[ idxSecondary ].InterfaceGuid );
			if ( s_ifaceDualWifiSecondary >= 0 )
			{
				SpewMsg( "Primary DualSTA interfaces %s matched to secondary interface %s, index %d\n",
					RenderGUID( primaryInterfaceGuid ).c_str(), RenderGUID( secondaryInterfaceList->InterfaceInfo[ idxSecondary ].InterfaceGuid ).c_str(),
					s_ifaceDualWifiSecondary );
				return s_ifaceDualWifiSecondary;
			}
		}

		AssertMsg( false, "Could not find secondary wifi adapter, even though wlan_intf_opcode_secondary_sta_synchronized_connections succeeded" );
	}

	// Failed.  This should be common
	DualWifiInitFailed( "Didn't find any Dual-Wifi-capable Wifi adapters" );
	return -1;
}

IRawUDPSocket *CRawUDPSocketImpl::GetDualWifiSecondarySocket( int nEnableSetting )
{
	SteamDatagramErrMsg errMsg;

	switch ( m_eDualWifiStatus )
	{
		case k_EDualWifi_NotAttempted:
		{
			Assert( m_pDualWifiPartner == nullptr );

			// Locate the secondary interface, if any.
			int ifaceIndex = -1;
			if ( nEnableSetting == k_nDualWifiEnable_ForceSimulate )
			{
				SpewMsg( "Not actually checking for Dual-wifi support, just creating simulating support, as per DualWifi_Enable=%d\n", nEnableSetting );
			}
			else
			{
				ifaceIndex = GetDualWifiSecondaryInterfaceIndex( nEnableSetting );
				if ( ifaceIndex < 0 && nEnableSetting == k_nDualWifiEnable_Enable )
				{
					// not found, and we don't want to simulate support.
					// This will be common!  Don't retry.
					m_eDualWifiStatus = k_EDualWifi_Done;
					break;
				}
			}

			if ( nEnableSetting == k_nDualWifiEnable_DoNotBind )
			{
				SpewMsg( "Not actually creating secondary socket as per DualWifi_Enable=%d\n", nEnableSetting );
				m_eDualWifiStatus = k_EDualWifi_Done;
				break;
			}

			// Create the second socket, binding it the interface as appropriate
			int nTempAddressFamiles = m_nAddressFamilies;
			m_pDualWifiPartner = OpenRawUDPSocketInternal( m_callback, errMsg, nullptr, &nTempAddressFamiles, ifaceIndex );
			if ( !m_pDualWifiPartner )
			{
				SpewWarning( "Failed to create dual Wifi secondary socket.  %s\n", errMsg );
				m_eDualWifiStatus = k_EDualWifi_Done; // Don't retry
				break;
			}

			m_eDualWifiStatus = k_EDualWifi_Primary;
			m_pDualWifiPartner->m_eDualWifiStatus = k_EDualWifi_Secondary;
			m_pDualWifiPartner->m_pDualWifiPartner = this;

			SpewMsg( "Created %sdual Wifi secondary socket OK.  Primary local address is %s, secondary is %s\n",
				ifaceIndex < 0 ? "simulated " : "",
				SteamNetworkingIPAddrRender( m_boundAddr ).c_str(), SteamNetworkingIPAddrRender( m_pDualWifiPartner->m_boundAddr ).c_str() );
			return m_pDualWifiPartner;
		}

		case k_EDualWifi_Primary:
			Assert( m_pDualWifiPartner );
			return m_pDualWifiPartner;

		default:
		case k_EDualWifi_Secondary:
			Assert( false );
			break;

		case k_EDualWifi_Done:
			break;
	}

	return nullptr;
}

#else

void DualWifiShutdown() {}

#endif

/////////////////////////////////////////////////////////////////////////////
//
// Service thread
//
/////////////////////////////////////////////////////////////////////////////

//
// Polling function.
// On entry: lock is held *exactly once*
// Returns: true - we want to keep running, lock is held
// Returns: false - stop request detected, lock no longer held
//
static bool SteamNetworkingSockets_InternalPoll( int msWait, bool bManualPoll )
{
	AssertGlobalLockHeldExactlyOnce();

	// Figure out how long to sleep
	SteamNetworkingMicroseconds usecNextWakeTime = IThinker::Thinker_GetNextScheduledThinkTime();
	if ( usecNextWakeTime < k_nThinkTime_Never )
	{

		// Calc wait time to wake up as late as possible,
		// rounded up to the nearest millisecond.
		SteamNetworkingMicroseconds usecNow = SteamNetworkingSockets_GetLocalTimestamp();
		int64 usecUntilNextThinkTime = usecNextWakeTime - usecNow;

		if ( usecNow >= usecNextWakeTime )
		{
			// Earliest thinker in the queue is ready to go now.
			// There is no point in going to sleep
			msWait = 0;
		}
		else
		{

			// Set wake time to wake up at the target time.  We assume the scheduler
			// only has 1ms precision, so we round to the nearest ms, so that we don't
			// always wake up exactly 1ms early, go to sleep and wait for 1ms.
			//
			// NOTE: On linux, we have a precise timer and we could probably do better
			// than this.  On windows, we could use an alertable timer, and presumably when
			// we set the we could use a high precision relative time, and Windows could do
			// smart stuff.
			int msTaskWait = ( usecUntilNextThinkTime + 500 ) / 1000;

			// We must wait at least 1 ms
			msTaskWait = std::max( 1, msTaskWait );

			// Limit to what the caller has requested
			msWait = std::min( msWait, msTaskWait );
		}
	}

	// Don't ever sleep for too long, just in case.  This timeout
	// is long enough so that if we have a bug where we really need to
	// be explicitly waking the thread for good perf, we will notice
	// the delay.  But not so long that a bug in some rare 
	// shutdown race condition (or the like) will be catastrophic
	msWait = std::min( msWait, k_msMaxPollWait );

	// Poll sockets
	if ( !PollRawUDPSockets( msWait, bManualPoll ) )
	{
		// Shutdown request, and they did NOT re-acquire the lock
		return false;
	}

	AssertGlobalLockHeldExactlyOnce();

	// Shutdown request?
	if ( s_nLowLevelSupportRefCount.load(std::memory_order_acquire) <= 0 || s_bManualPollMode != bManualPoll )
	{
		SteamNetworkingGlobalLock::Unlock();
		return false; // Shutdown request, we have released the lock
	}

	// Check for periodic processing
	IThinker::Thinker_ProcessThinkers();

	// Check for various deferred operations
	ProcessDeferredOperations();
	return true;
}

static void SteamNetworkingThreadProc()
{

	// This is an "interrupt" thread.  When an incoming packet raises the event,
	// we need to take priority above normal threads and wake up immediately
	// to process the packet.  We should be asleep most of the time waiting
	// for packets to arrive.
	#if defined(_WIN32)
		DbgVerify( SetThreadPriority( GetCurrentThread(), THREAD_PRIORITY_HIGHEST ) );
	#elif defined(POSIX)
		// This probably won't work on Linux, because you cannot raise thread priority
		// without being root.  But on some systems it works.  So we try, and if it
		// works, great.
		struct sched_param sched;
		int policy;
		pthread_t thread = pthread_self();
		if ( pthread_getschedparam(thread, &policy, &sched) == 0 )
		{
			// Make sure we're not already at max.  No matter what, we don't
			// want to lower our priority!  On linux, it appears that what happens
			// is that the current and max priority values here are 0.
			int max_priority = sched_get_priority_max(policy);
			//printf( "pthread_getschedparam worked, policy=%d, pri=%d, max_pri=%d\n", policy, sched.sched_priority, max_priority );
			if ( max_priority > sched.sched_priority )
			{

				// Determine new priority.
				int min_priority = sched_get_priority_min(policy);
				sched.sched_priority = std::max( sched.sched_priority+1, (min_priority + max_priority*3) / 4 );

				// Try to set it
				pthread_setschedparam( thread, policy, &sched );
			}
		}
	#endif

	#if defined(_WIN32) && !defined(__GNUC__)

		#pragma warning( disable: 6132 ) // Possible infinite loop:  use of the constant EXCEPTION_CONTINUE_EXECUTION in the exception-filter expression of a try-except.  Execution restarts in the protected block.

		typedef struct tagTHREADNAME_INFO
		{
			DWORD dwType;
			LPCSTR szName;
			DWORD dwThreadID;
			DWORD dwFlags;
		} THREADNAME_INFO;


		THREADNAME_INFO info;
		{
			info.dwType = 0x1000;
			info.szName = "SteamNetworking";
			info.dwThreadID = GetCurrentThreadId();
			info.dwFlags = 0;
		}
		__try
		{
			RaiseException( 0x406D1388, 0, sizeof(info)/sizeof(DWORD), (ULONG_PTR*)&info );
		}
		__except(EXCEPTION_CONTINUE_EXECUTION)
		{
		}

	#else
		// Help!  Really we should do this for all platforms.  Seems it's not
		// totally straightforward the correct way to do this on Linux.
	#endif

	// In the loop, we will always hold global lock while we're awake.
	// So go ahead and acquire it now.  But watch out for a race condition
	// where we want to shut down immediately after starting the thread
	do
	{
		if ( s_nLowLevelSupportRefCount.load(std::memory_order_acquire) <= 0 || s_bManualPollMode )
			return;
	} while ( !SteamNetworkingGlobalLock::TryLock( "ServiceThread", 10 ) );

	// Random number generator may be per thread!  Make sure and see it for
	// this thread, if so
	SeedWeakRandomGenerator();

	SpewVerbose( "Service thread running.\n" );

	// Keep looping until we're asked to terminate
	while ( SteamNetworkingSockets_InternalPoll( 5000, false ) )
	{
		// If they activate manual poll mode, then bail!
		if ( s_bManualPollMode )
		{
			SteamNetworkingGlobalLock::Unlock();
			break;
		}
	}

	SpewVerbose( "Service thread exiting.\n" );
}

static void StopSteamDatagramThread()
{
	// They should have set some sort of flag that will cause us the thread to stop
	Assert( s_nLowLevelSupportRefCount.load(std::memory_order_acquire) == 0 || s_bManualPollMode );

	// Send wake up signal
	WakeSteamDatagramThread();

	// Wait for thread to finish
	s_pThreadSteamDatagram->join();

	// Clean up
	delete s_pThreadSteamDatagram;
	s_pThreadSteamDatagram = nullptr;
}

/////////////////////////////////////////////////////////////////////////////
//
// Bound sockets / socket sharing
//
/////////////////////////////////////////////////////////////////////////////

class CDedicatedBoundSocket : public IBoundUDPSocket
{
private:
	inline virtual ~CDedicatedBoundSocket() {}
public:
	CDedicatedBoundSocket( IRawUDPSocket *pRawSock, const netadr_t &adr )
	: IBoundUDPSocket( pRawSock, adr ) {}

	CRecvPacketCallback m_callback;

	virtual void Close() OVERRIDE
	{
		m_pRawSock->Close();
		m_pRawSock = nullptr;
		delete this;
	}
};

static void DedicatedBoundSocketCallback( const RecvPktInfo_t &info, CDedicatedBoundSocket *pSock )
{

	// Make sure that it's from the guy we are supposed to be talking to.
	if ( info.m_adrFrom != pSock->GetRemoteHostAddr() )
	{
		// Packets from random internet hosts happen all the time,
		// especially on a LAN where all sorts of people have broadcast
		// discovery protocols.  So this probably isn't a bug or a problem.
		SpewVerbose( "Ignoring stray packet from %s received on port %d.  Should only be talking to %s on that port.\n",
			CUtlNetAdrRender( info.m_adrFrom ).String(), pSock->GetRawSock()->m_boundAddr.m_port,
			CUtlNetAdrRender( pSock->GetRemoteHostAddr() ).String() );
		return;
	}

	// Now execute their callback.
	// Passing the address in this context is sort of superfluous.
	// Should we use a different signature here so that the user
	// of our API doesn't write their own useless code to check
	// the from address?
	pSock->m_callback( info );
}

IBoundUDPSocket *OpenUDPSocketBoundToHost( const netadr_t &adrRemote, CRecvPacketCallback callback, SteamDatagramErrMsg &errMsg )
{
	SteamNetworkingGlobalLock::AssertHeldByCurrentThread();

	// Select local address to use.
	// Since we know the remote host, let's just always use a single-stack socket
	// with the specified family
	int nAddressFamilies = ( adrRemote.GetType() == k_EIPTypeV6 ) ? k_nAddressFamily_IPv6 : k_nAddressFamily_IPv4;

	// Create a socket, bind it to the desired local address
	CDedicatedBoundSocket *pTempContext = nullptr; // don't yet know the context
	CRawUDPSocketImpl *pRawSock = OpenRawUDPSocketInternal( CRecvPacketCallback( DedicatedBoundSocketCallback, pTempContext ), errMsg, nullptr, &nAddressFamilies );
	if ( !pRawSock )
		return nullptr;

	// Return wrapper interface that can only talk to this remote host
	CDedicatedBoundSocket *pBoundSock = new CDedicatedBoundSocket( pRawSock, adrRemote );
	pRawSock->m_callback.m_pContext = pBoundSock;
	pBoundSock->m_callback = callback;

	return pBoundSock;
}

bool CreateBoundSocketPair( CRecvPacketCallback callback1, CRecvPacketCallback callback2, IBoundUDPSocket **ppOutSockets, SteamDatagramErrMsg &errMsg )
{
	SteamNetworkingGlobalLock::AssertHeldByCurrentThread();

	SteamNetworkingIPAddr localAddr;

	// Create two socket UDP sockets, bound to (IPv4) loopback IP, but allow OS to choose ephemeral port
	CRawUDPSocketImpl *pRawSock[2];
	uint32 nLocalIP = 0x7f000001; // 127.0.0.1
	CDedicatedBoundSocket *pTempContext = nullptr; // don't yet know the context
	localAddr.SetIPv4( nLocalIP, 0 );
	pRawSock[0] = OpenRawUDPSocketInternal( CRecvPacketCallback( DedicatedBoundSocketCallback, pTempContext ), errMsg, &localAddr, nullptr );
	if ( !pRawSock[0] )
		return false;
	localAddr.SetIPv4( nLocalIP, 0 );
	pRawSock[1] = OpenRawUDPSocketInternal( CRecvPacketCallback( DedicatedBoundSocketCallback, pTempContext ), errMsg, &localAddr, nullptr );
	if ( !pRawSock[1] )
	{
		delete pRawSock[0];
		return false;
	}

	// Return wrapper interfaces that can only talk to each other
	for ( int i = 0 ; i < 2 ; ++i )
	{
		auto s = new CDedicatedBoundSocket( pRawSock[i], netadr_t( nLocalIP, pRawSock[1-i]->m_boundAddr.m_port ) );
		pRawSock[i]->m_callback.m_pContext = s;
		s->m_callback = (i == 0 ) ? callback1 : callback2;
		ppOutSockets[i] = s;
	}

	return true;
}

CSharedSocket::CSharedSocket()
{
	m_pRawSock = nullptr;
}

CSharedSocket::~CSharedSocket()
{
	Kill();
}

void CSharedSocket::CallbackRecvPacket( const RecvPktInfo_t &info, CSharedSocket *pSock )
{
	// Locate the client
	int idx = pSock->m_mapRemoteHosts.Find( info.m_adrFrom );

	// Select the callback to invoke, ether client-specific, or the default
	const CRecvPacketCallback &callback = ( idx == pSock->m_mapRemoteHosts.InvalidIndex() ) ? pSock->m_callbackDefault : pSock->m_mapRemoteHosts[ idx ]->m_callback;

	// Execute the callback
	callback( info );
}

bool CSharedSocket::BInit( const SteamNetworkingIPAddr &localAddr, CRecvPacketCallback callbackDefault, SteamDatagramErrMsg &errMsg )
{
	SteamNetworkingGlobalLock::AssertHeldByCurrentThread();

	Kill();

	SteamNetworkingIPAddr bindAddr = localAddr;
	m_pRawSock = OpenRawUDPSocket( CRecvPacketCallback( CallbackRecvPacket, this ), errMsg, &bindAddr, nullptr );
	if ( m_pRawSock == nullptr )
		return false;

	m_callbackDefault = callbackDefault;
	return true;
}

void CSharedSocket::Kill()
{
	SteamNetworkingGlobalLock::AssertHeldByCurrentThread();

	m_callbackDefault.m_fnCallback = nullptr;
	if ( m_pRawSock )
	{
		m_pRawSock->Close();
		m_pRawSock = nullptr;
	}
	FOR_EACH_HASHMAP( m_mapRemoteHosts, idx )
	{
		CloseRemoteHostByIndex( idx );
	}
}

void CSharedSocket::CloseRemoteHostByIndex( int idx )
{
	SteamNetworkingGlobalLock::AssertHeldByCurrentThread();

	delete m_mapRemoteHosts[ idx ];
	m_mapRemoteHosts[idx] = nullptr; // just for grins
	m_mapRemoteHosts.RemoveAt( idx );
}

IBoundUDPSocket *CSharedSocket::AddRemoteHost( const netadr_t &adrRemote, CRecvPacketCallback callback )
{
	SteamNetworkingGlobalLock::AssertHeldByCurrentThread();

	if ( m_mapRemoteHosts.HasElement( adrRemote ) )
	{
		AssertMsg1( false, "Already talking to %s on this shared socket, cannot add another remote host!", CUtlNetAdrRender( adrRemote ).String() );
		return nullptr;
	}
	RemoteHost *pRemoteHost = new RemoteHost( m_pRawSock, adrRemote );
	pRemoteHost->m_pOwner = this;
	pRemoteHost->m_callback = callback;
	m_mapRemoteHosts.Insert( adrRemote, pRemoteHost );

	return pRemoteHost;
}

void CSharedSocket::RemoteHost::Close()
{
	SteamNetworkingGlobalLock::AssertHeldByCurrentThread();

	int idx = m_pOwner->m_mapRemoteHosts.Find( m_adr );
	if ( idx == m_pOwner->m_mapRemoteHosts.InvalidIndex() || m_pOwner->m_mapRemoteHosts[idx] != this )
	{
		AssertMsg( false, "CSharedSocket client table corruption!" );
		delete this;
	}
	else
	{
		m_pOwner->CloseRemoteHostByIndex( idx );
	}
}

/////////////////////////////////////////////////////////////////////////////
//
// Spew
//
/////////////////////////////////////////////////////////////////////////////

static ShortDurationLock s_systemSpewLock( "SystemSpew" );
SteamNetworkingMicroseconds g_usecLastRateLimitSpew;
int g_nRateLimitSpewCount;
ESteamNetworkingSocketsDebugOutputType g_eSystemSpewLevel = k_ESteamNetworkingSocketsDebugOutputType_None; // Option selected by the "system" (environment variable, etc)
ESteamNetworkingSocketsDebugOutputType g_eAppSpewLevel = k_ESteamNetworkingSocketsDebugOutputType_Msg; // Option selected by app
ESteamNetworkingSocketsDebugOutputType g_eDefaultGroupSpewLevel = k_ESteamNetworkingSocketsDebugOutputType_Msg; // Effective value
FSteamNetworkingSocketsDebugOutput g_pfnDebugOutput = nullptr;
void (*g_pfnPreFormatSpewHandler)( ESteamNetworkingSocketsDebugOutputType eType, bool bFmt, const char* pstrFile, int nLine, const char *pMsg, va_list ap ) = SteamNetworkingSockets_DefaultPreFormatDebugOutputHandler;
static bool s_bSpewInitted = false;

static FILE *g_pFileSystemSpew;
static SteamNetworkingMicroseconds g_usecSystemLogFileOpened;
static bool s_bNeedToFlushSystemSpew = false;;

static void InitSpew()
{
	ShortDurationScopeLock scopeLock( s_systemSpewLock );

	// First time, check environment variables and set system spew level
	if ( !s_bSpewInitted )
	{
		s_bSpewInitted = true;

		const char *STEAMNETWORKINGSOCKETS_LOG_LEVEL = getenv( "STEAMNETWORKINGSOCKETS_LOG_LEVEL" );
		if ( !V_isempty( STEAMNETWORKINGSOCKETS_LOG_LEVEL ) )
		{
			switch ( atoi( STEAMNETWORKINGSOCKETS_LOG_LEVEL ) )
			{
				case 0: g_eSystemSpewLevel = k_ESteamNetworkingSocketsDebugOutputType_None; break;
				case 1: g_eSystemSpewLevel = k_ESteamNetworkingSocketsDebugOutputType_Warning; break;
				case 2: g_eSystemSpewLevel = k_ESteamNetworkingSocketsDebugOutputType_Msg; break;
				case 3: g_eSystemSpewLevel = k_ESteamNetworkingSocketsDebugOutputType_Verbose; break;
				case 4: g_eSystemSpewLevel = k_ESteamNetworkingSocketsDebugOutputType_Debug; break;
				case 5: g_eSystemSpewLevel = k_ESteamNetworkingSocketsDebugOutputType_Everything; break;
			}

			if ( g_eSystemSpewLevel > k_ESteamNetworkingSocketsDebugOutputType_None )
			{

				// What log file to use?
				const char *pszLogFile = getenv( "STEAMNETWORKINGSOCKETS_LOG_FILE" );
				if ( !pszLogFile )
					pszLogFile = "steamnetworkingsockets.log" ;

				// Try to open file.  Use binary mode, since we want to make sure we control
				// when it is flushed to disk
				g_pFileSystemSpew = fopen( pszLogFile, "wb" );
				if ( g_pFileSystemSpew )
				{
					g_usecSystemLogFileOpened = SteamNetworkingSockets_GetLocalTimestamp();
					time_t now = time(nullptr);
					fprintf( g_pFileSystemSpew, "Log opened, time %lld %s", (long long)now, ctime( &now ) );

					// if they ask for verbose, turn on some other groups, by default
					if ( g_eSystemSpewLevel >= k_ESteamNetworkingSocketsDebugOutputType_Verbose )
					{
						g_ConfigDefault_LogLevel_P2PRendezvous.m_value.m_defaultValue = g_eSystemSpewLevel;
						g_ConfigDefault_LogLevel_P2PRendezvous.m_value.Set( g_eSystemSpewLevel );

						g_ConfigDefault_LogLevel_PacketGaps.m_value.m_defaultValue = g_eSystemSpewLevel-1;
						g_ConfigDefault_LogLevel_PacketGaps.m_value.Set( g_eSystemSpewLevel-1 );
					}
				}
				else
				{
					// Failed
					g_eSystemSpewLevel = k_ESteamNetworkingSocketsDebugOutputType_None;
				}
			}
		}
	}

	g_eDefaultGroupSpewLevel = std::max( g_eSystemSpewLevel, g_eAppSpewLevel );

}

static void KillSpew()
{
	ShortDurationScopeLock scopeLock( s_systemSpewLock );
	g_eDefaultGroupSpewLevel = g_eSystemSpewLevel = g_eAppSpewLevel = k_ESteamNetworkingSocketsDebugOutputType_None;
	g_pfnDebugOutput = nullptr;
	s_bSpewInitted = false;
	s_bNeedToFlushSystemSpew = false;
	if ( g_pFileSystemSpew )
	{
		fclose( g_pFileSystemSpew );
		g_pFileSystemSpew = nullptr;
	}
}

static void FlushSystemSpewLocked()
{
	s_systemSpewLock.AssertHeldByCurrentThread();
	if ( s_bNeedToFlushSystemSpew )
	{
		if ( g_pFileSystemSpew )
			fflush( g_pFileSystemSpew );
		s_bNeedToFlushSystemSpew = false;
	}
}

static void FlushSystemSpew()
{
	if ( s_bNeedToFlushSystemSpew ) // Read the flag without taking the lock first as an optimization, as most of the time it will not be set
	{
		ShortDurationScopeLock scopeLock( s_systemSpewLock );
		FlushSystemSpewLocked();
	}
}


void ReallySpewTypeFmt( int eType, const char *pMsg, ... )
{
	va_list ap;
	va_start( ap, pMsg );
	(*g_pfnPreFormatSpewHandler)( ESteamNetworkingSocketsDebugOutputType(eType), true, nullptr, 0, pMsg, ap );
	va_end( ap );
}

bool BSteamNetworkingSocketsLowLevelAddRef( SteamDatagramErrMsg &errMsg )
{
	SteamNetworkingGlobalLock::AssertHeldByCurrentThread();

	// Make sure and call time function at least once
	// just before we start up our thread, so we don't lurch
	// on our first reading after the thread is running and
	// take action to correct this.
	SteamNetworkingSockets_GetLocalTimestamp();

	// First time init?
	if ( s_nLowLevelSupportRefCount.load(std::memory_order_acquire) == 0 )
	{
		InitSpew();

		CCrypto::Init();

		// Initialize event tracing
		ETW_Init();

		// Give us a extra time here.  This is a one-time init function and the OS might
		// need to load up libraries and stuff.
		SteamNetworkingGlobalLock::SetLongLockWarningThresholdMS( "BSteamNetworkingSocketsLowLevelAddRef", 500 );

		// Initialize COM
		#ifdef _XBOX_ONE
		{
			HRESULT hr = ::CoInitializeEx( nullptr, COINIT_MULTITHREADED );
			if ( !SUCCEEDED( hr ) )
			{
				V_sprintf_safe( errMsg, "CoInitializeEx returned %x", hr );
				return false;
			}
		}
		#endif

		// Initialize sockets
		#ifdef _WIN32
		{
			#pragma comment( lib, "ws2_32.lib" )
			WSAData wsaData;
			if ( ::WSAStartup( MAKEWORD(2, 2), &wsaData ) != 0 ) 
			{
				#ifdef _XBOX_ONE
					::CoUninitialize();
				#endif
				V_strcpy_safe( errMsg, "WSAStartup failed" );
				return false;
			}

			#pragma comment( lib, "winmm.lib" )
			if ( ::timeBeginPeriod( 1 ) != 0 )
			{
				::WSACleanup();
				#ifdef _XBOX_ONE
					::CoUninitialize();
				#endif
				V_strcpy_safe( errMsg, "timeBeginPeriod failed" );
				return false;
			}
		}
		#endif

		// Initialize fake rate limit token buckets
		InitFakeRateLimit();

		// Make sure random number generator is seeded
		SeedWeakRandomGenerator();

		// Create thread communication object used to wake the background thread efficiently
		// in case a thinker priority changes or we want to shutdown
		#if defined( _WIN32 )
			Assert( s_hEventWakeThread == INVALID_HANDLE_VALUE );

			// Note: Using "automatic reset" style event.
			s_hEventWakeThread = CreateEvent( nullptr, false, false, nullptr );
			if ( s_hEventWakeThread == NULL || s_hEventWakeThread == INVALID_HANDLE_VALUE )
			{
				s_hEventWakeThread = INVALID_HANDLE_VALUE;
				V_sprintf_safe( errMsg, "CreateEvent() call failed.  Error code 0x%08x.", GetLastError() );
				return false;
			}
		#elif defined( NN_NINTENDO_SDK )
			// Sorry, but this code is covered under NDA with Nintendo, and
			// we don't have permission to distribute it.
		#else
			Assert( s_hSockWakeThreadRead == INVALID_SOCKET );
			Assert( s_hSockWakeThreadWrite == INVALID_SOCKET );
			int sockType = SOCK_DGRAM;
			#ifdef LINUX
				sockType |= SOCK_CLOEXEC;
			#endif
			int sock[2];
			if ( socketpair( AF_LOCAL, sockType, 0, sock ) != 0 )
			{
				V_sprintf_safe( errMsg, "socketpair() call failed.  Error code 0x%08x.", GetLastSocketError() );
				return false;
			}

			s_hSockWakeThreadRead = sock[0];
			s_hSockWakeThreadWrite = sock[1];

			unsigned int opt;
			opt = 1;
			if ( ioctlsocket( s_hSockWakeThreadRead, FIONBIO, (unsigned long*)&opt ) != 0 )
			{
				AssertMsg1( false, "Failed to set socket nonblocking mode.  Error code 0x%08x.", GetLastSocketError() );
			}
			opt = 1;
			if ( ioctlsocket( s_hSockWakeThreadWrite, FIONBIO, (unsigned long*)&opt ) != 0 )
			{
				AssertMsg1( false, "Failed to set socket nonblocking mode.  Error code 0x%08x.", GetLastSocketError() );
			}
		#endif

		SpewMsg( "Initialized low level socket/threading support.\n" );
	}

	//extern void KludgePrintPublicKey();
	//KludgePrintPublicKey();

	s_nLowLevelSupportRefCount.fetch_add(1, std::memory_order_acq_rel);

	// Make sure the thread is running, if it should be
	if ( !s_bManualPollMode && !s_pThreadSteamDatagram )
		s_pThreadSteamDatagram = new std::thread( SteamNetworkingThreadProc );

	// Install an axexit handler, so that if static destruction is triggered without
	// cleaning up the library properly, we won't crash.
	static bool s_bInstalledAtExitHandler = false;
	if ( !s_bInstalledAtExitHandler )
	{
		s_bInstalledAtExitHandler = true;
		atexit( []{
			SteamNetworkingGlobalLock scopeLock( "atexit" );

			// Static destruction is about to happen.  If we have a thread,
			// we need to nuke it
			KillSpew();
			while ( s_nLowLevelSupportRefCount.load(std::memory_order_acquire) > 0 )
				SteamNetworkingSocketsLowLevelDecRef();
		} );
	}

	return true;
}

void SteamNetworkingSocketsLowLevelDecRef()
{
	SteamNetworkingGlobalLock::AssertHeldByCurrentThread();

	// Last user is now done?
	int nLastRefCount = s_nLowLevelSupportRefCount.fetch_sub(1, std::memory_order_acq_rel);
	Assert( nLastRefCount > 0 );
	if ( nLastRefCount > 1 )
		return;

	SpewMsg( "Shutting down low level socket/threading support.\n" );

	// Give us a extra time here.  This is a one-time shutdown function.
	// There is a potential race condition / deadlock with the service thread,
	// that might cause us to have to wait for it to timeout.  And the OS
	// might need to do stuff when we close a bunch of sockets (and WSACleanup)
	SteamNetworkingGlobalLock::SetLongLockWarningThresholdMS( "SteamNetworkingSocketsLowLevelDecRef", 500 );

	// Stop the service thread, if we have one
	if ( s_pThreadSteamDatagram )
		StopSteamDatagramThread();

	// Destory wake communication objects
	#if defined( _WIN32 )
		if ( s_hEventWakeThread != INVALID_HANDLE_VALUE )
		{
			CloseHandle( s_hEventWakeThread );
			s_hEventWakeThread = INVALID_HANDLE_VALUE;
		}
	#elif defined( NN_NINTENDO_SDK )
		// Sorry, but this code is covered under NDA with Nintendo, and
		// we don't have permission to distribute it.
	#else
		if ( s_hSockWakeThreadRead != INVALID_SOCKET )
		{
			closesocket( s_hSockWakeThreadRead );
			s_hSockWakeThreadRead = INVALID_SOCKET;
		}
		if ( s_hSockWakeThreadWrite != INVALID_SOCKET )
		{
			closesocket( s_hSockWakeThreadWrite );
			s_hSockWakeThreadWrite = INVALID_SOCKET;
		}
	#endif

	// Check for any leftover tasks that were queued to be run while we hold the lock
	ProcessDeferredOperations();

	// At this point, we shouldn't have any remaining sockets
	if ( s_vecRawSockets.IsEmpty() )
	{
		s_vecRawSockets.Purge();
	}
	else
	{
		AssertMsg( false, "Trying to close low level socket support, but we still have sockets open!" );
	}

	// Nuke packet lagger queues and make sure we are not registered to think
	s_packetLagQueueRecv.Clear();
	s_packetLagQueueSend.Clear();

	// Shutdown event tracing
	ETW_Kill();

	// Shutdown Dual wifi support
	DualWifiShutdown();

	// Nuke sockets and COM
	#ifdef _WIN32
		::timeEndPeriod( 1 );
		::WSACleanup();
	#endif
	#ifdef _XBOX_ONE
		::CoUninitialize();
	#endif

	KillSpew();
}

#ifdef DBGFLAG_VALIDATE
void SteamNetworkingSocketsLowLevelValidate( CValidator &validator )
{
	ValidateRecursive( s_vecRawSockets );
}
#endif

void SteamNetworkingSockets_SetDebugOutputFunction( ESteamNetworkingSocketsDebugOutputType eDetailLevel, FSteamNetworkingSocketsDebugOutput pfnFunc )
{
	if ( pfnFunc && eDetailLevel > k_ESteamNetworkingSocketsDebugOutputType_None )
	{
		SteamNetworkingSocketsLib::g_pfnDebugOutput = pfnFunc;
		SteamNetworkingSocketsLib::g_eAppSpewLevel = ESteamNetworkingSocketsDebugOutputType( eDetailLevel );
	}
	else
	{
		SteamNetworkingSocketsLib::g_pfnDebugOutput = nullptr;
		SteamNetworkingSocketsLib::g_eAppSpewLevel = k_ESteamNetworkingSocketsDebugOutputType_None;
	}

	SteamNetworkingSocketsLib::InitSpew();
}

SteamNetworkingMicroseconds SteamNetworkingSockets_GetLocalTimestamp()
{
	SteamNetworkingMicroseconds usecResult;
	long long usecLastReturned;
	for (;;)
	{
		// Fetch values into locals (probably registers)
		usecLastReturned = SteamNetworkingSocketsLib::s_usecTimeLastReturned;
		long long usecOffset = SteamNetworkingSocketsLib::s_usecTimeOffset;

		// Read raw timer
		uint64 usecRaw = Plat_USTime();

		// Add offset to get value in "SteamNetworkingMicroseconds" time
		usecResult = usecRaw + usecOffset;

		// How much raw timer time (presumed to be wall clock time) has elapsed since
		// we read the timer?
		SteamNetworkingMicroseconds usecElapsed = usecResult - usecLastReturned;
		Assert( usecElapsed >= 0 ); // Our raw timer function is not monotonic!  We assume this never happens!
		if ( usecElapsed <= k_usecMaxTimestampDelta )
		{
			// Should be the common case - only a relatively small of time has elapsed
			break;
		}
		if ( SteamNetworkingSocketsLib::s_nLowLevelSupportRefCount.load(std::memory_order_acquire) <= 0 )
		{
			// We don't have any expectation that we should be updating the timer frequently,
			// so  a big jump in the value just means they aren't calling it very often
			break;
		}

		// NOTE: We should only rarely get here, and probably as a result of running under the debugger

		// Adjust offset so that delta between timestamps is limited
		long long usecNewOffset = usecOffset - ( usecElapsed - k_usecMaxTimestampDelta );
		usecResult = usecRaw + usecNewOffset;

		// Save the new offset.
		if ( SteamNetworkingSocketsLib::s_usecTimeOffset.compare_exchange_strong( usecOffset, usecNewOffset ) )
			break;

		// Race condition which should be extremely rare.  Some other thread changed the offset, in the time
		// between when we fetched it and now.  (So, a really small race window!)  Just start all over from
		// the beginning.
	}

	// Save the last value returned.  Unless another thread snuck in there while we were busy.
	// If so, that's OK.
	SteamNetworkingSocketsLib::s_usecTimeLastReturned.compare_exchange_strong( usecLastReturned, usecResult );

	return usecResult;
}

} // namespace SteamNetworkingSocketsLib

using namespace SteamNetworkingSocketsLib;

STEAMNETWORKINGSOCKETS_INTERFACE void SteamNetworkingSockets_SetManualPollMode( bool bFlag )
{
	if ( s_bManualPollMode == bFlag )
		return;
	SteamNetworkingGlobalLock scopeLock( "SteamNetworkingSockets_SetManualPollMode" );
	s_bManualPollMode = bFlag;

	// Check for starting/stopping the thread
	if ( s_pThreadSteamDatagram )
	{
		// Thread is active.  Should it be?
		if ( s_nLowLevelSupportRefCount.load(std::memory_order_acquire) <= 0 || s_bManualPollMode )
		{
			SpewMsg( "Service thread is running, and manual poll mode actiavted.  Stopping service thread.\n" );
			StopSteamDatagramThread();
		}
	}
	else
	{
		if ( s_nLowLevelSupportRefCount.load(std::memory_order_acquire) > 0 && !s_bManualPollMode )
		{
			// Start up the thread
			SpewMsg( "Service thread is not running, and manual poll mode was turned off, starting service thread.\n" );
			s_pThreadSteamDatagram = new std::thread( SteamNetworkingThreadProc );
		}
	}
}

STEAMNETWORKINGSOCKETS_INTERFACE void SteamNetworkingSockets_Poll( int msMaxWaitTime )
{
	if ( !s_bManualPollMode )
	{
		AssertMsg( false, "Not in manual poll mode!" );
		return;
	}
	Assert( s_nLowLevelSupportRefCount.load(std::memory_order_acquire) > 0 );

	while ( !SteamNetworkingGlobalLock::TryLock( "SteamNetworkingSockets_Poll", 1 ) )
	{
		if ( --msMaxWaitTime <= 0 )
			return;
	}

	bool bStillLocked = SteamNetworkingSockets_InternalPoll( msMaxWaitTime, true );
	if ( bStillLocked )
		SteamNetworkingGlobalLock::Unlock();
}

STEAMNETWORKINGSOCKETS_INTERFACE void SteamNetworkingSockets_SetLockWaitWarningThreshold( SteamNetworkingMicroseconds usecTheshold )
{
	#if STEAMNETWORKINGSOCKETS_LOCK_DEBUG_LEVEL > 0
		s_usecLockWaitWarningThreshold = usecTheshold;
	#else
		// Should we assert here?
	#endif
}

STEAMNETWORKINGSOCKETS_INTERFACE void SteamNetworkingSockets_SetLockAcquiredCallback( void (*callback)( const char *tags, SteamNetworkingMicroseconds usecWaited ) )
{
	#if STEAMNETWORKINGSOCKETS_LOCK_DEBUG_LEVEL > 0
		s_fLockAcquiredCallback = callback;
	#else
		// Should we assert here?
	#endif
}

STEAMNETWORKINGSOCKETS_INTERFACE void SteamNetworkingSockets_SetLockHeldCallback( void (*callback)( const char *tags, SteamNetworkingMicroseconds usecWaited ) )
{
	#if STEAMNETWORKINGSOCKETS_LOCK_DEBUG_LEVEL > 0
		s_fLockHeldCallback = callback;
	#else
		// Should we assert here?
	#endif
}

STEAMNETWORKINGSOCKETS_INTERFACE void SteamNetworkingSockets_SetPreFormatDebugOutputHandler(
	ESteamNetworkingSocketsDebugOutputType eDetailLevel,
	void (*pfn_Handler)( ESteamNetworkingSocketsDebugOutputType eType, bool bFmt, const char* pstrFile, int nLine, const char *pMsg, va_list ap )
)
{
	g_eDefaultGroupSpewLevel = eDetailLevel;
	g_pfnPreFormatSpewHandler = pfn_Handler;
}

STEAMNETWORKINGSOCKETS_INTERFACE void SteamNetworkingSockets_DefaultPreFormatDebugOutputHandler( ESteamNetworkingSocketsDebugOutputType eType, bool bFmt, const char* pstrFile, int nLine, const char *pMsg, va_list ap )
{
	// Do the formatting
	char buf[ 2048 ];
	int szBuf = sizeof(buf);
	char *msgDest = buf;
	if ( pstrFile )
	{

		// Skip to "/src/"
		for (char const* s = pstrFile; *s; ++s)
		{
			if (
				(s[0] == '/' || s[0] == '\\')
				&& s[1] == 's'
				&& s[2] == 'r'
				&& s[3] == 'c'
				&& (s[4] == '/' || s[4] == '\\')
			) {
				pstrFile = s + 1;
				break;
			}
		}

		int l = V_sprintf_safe( buf, "%s(%d): ", pstrFile, nLine );
		szBuf -= l;
		msgDest += l;
	}

	if ( bFmt )
		V_vsnprintf( msgDest, szBuf, pMsg, ap );
	else
		V_strncpy( msgDest, pMsg, szBuf );

	// Gah, some, but not all, of our code has newlines on the end
	V_StripTrailingWhitespaceASCII( buf );

	// Spew to log file?
	if ( eType <= g_eSystemSpewLevel && g_pFileSystemSpew )
	{
		ShortDurationScopeLock scopeLock( s_systemSpewLock ); // WARNING - these locks are not re-entrant, so if we assert while holding it, we could deadlock!
		if ( eType <= g_eSystemSpewLevel && g_pFileSystemSpew )
		{

			// Write
			SteamNetworkingMicroseconds usecLogTime = SteamNetworkingSockets_GetLocalTimestamp() - g_usecSystemLogFileOpened;
			fprintf( g_pFileSystemSpew, "%8.3f %s\n", usecLogTime*1e-6, buf );

			// Queue to flush when we we think we can afford to hit the disk synchronously
			s_bNeedToFlushSystemSpew = true;

			// Flush certain critical messages things immediately
			if ( eType <= k_ESteamNetworkingSocketsDebugOutputType_Error )
				FlushSystemSpewLocked();
		}
	}

	// Invoke callback
	FSteamNetworkingSocketsDebugOutput pfnDebugOutput = g_pfnDebugOutput;
	if ( pfnDebugOutput )
		pfnDebugOutput( eType, buf );
}


/////////////////////////////////////////////////////////////////////////////
//
// memory override
//
/////////////////////////////////////////////////////////////////////////////

#include <tier0/memdbgoff.h>

#ifdef STEAMNETWORKINGSOCKETS_ENABLE_MEM_OVERRIDE

static bool s_bHasAllocatedMemory = false;

static void* (*s_pfn_malloc)( size_t s ) = malloc;
static void (*s_pfn_free)( void *p ) = free;
static void* (*s_pfn_realloc)( void *p, size_t s ) = realloc;

void *SteamNetworkingSockets_Malloc( size_t s )
{
	s_bHasAllocatedMemory = true;
	return (*s_pfn_malloc)( s );
}

void *SteamNetworkingSockets_Realloc( void *p, size_t s )
{
	s_bHasAllocatedMemory = true;
	return (*s_pfn_realloc)( p, s );
}

void SteamNetworkingSockets_Free( void *p )
{
	(*s_pfn_free)( p );
}

STEAMNETWORKINGSOCKETS_INTERFACE void SteamNetworkingSockets_SetCustomMemoryAllocator(
	void* (*pfn_malloc)( size_t s ),
	void (*pfn_free)( void *p ),
	void* (*pfn_realloc)( void *p, size_t s )
) {
	Assert( !s_bHasAllocatedMemory ); // Too late!

	s_pfn_malloc = pfn_malloc;
	s_pfn_free = pfn_free;
	s_pfn_realloc = pfn_realloc;
}
#endif
