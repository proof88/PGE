/*
    ###################################################################################
    PRREManager.cpp
    This file is part of PRRE.
    Ancestor classes for manager and managed classes.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREbaseIncludes.h"  // PCH
#include <stdexcept>
#include "../include/external/PRREManager.h"
#include "../include/internal/PRREManagedImpl.h"
#include "../include/internal/PRREpragmas.h"


/*
   PRREManaged::PRREManagedImpl
   ###########################################################################
*/

           
// ############################### PUBLIC ################################


PRREManaged::PRREManagedImpl::~PRREManagedImpl()
{
    _pOwner->getManagedConsole().OLnOI("~PRREManaged()");
    DetachFrom();
    nManagedsTotal--;
    _pOwner->getManagedConsole().OO();
} // ~PRREManagedImpl()


/**
    Removes the managed from its manager.
    Actually it calls the public Detach() method of the manager with the this pointer.
*/
void PRREManaged::PRREManagedImpl::DetachFrom()
{
    if ( pParentMgr != PGENULL )
    {
        pParentMgr->Detach(*_pOwner); // !!! pParentMgr becomes NULL here!
    }
} // DetachFrom()


/**
    Gets the manager of the managed.
    @return Pointer to manager.
*/
PRREManager* PRREManaged::PRREManagedImpl::getManager() const
{
    return pParentMgr;
} // getManager()


/**
    Gets the name.
*/
const std::string& PRREManaged::PRREManagedImpl::getName() const
{
    return sName;
}


/**
    Sets the name.
*/
void PRREManaged::PRREManagedImpl::SetName(const std::string& name)
{
    sName = name;
}


/**
    This can be used if the specialized managed object has some resources
    that may be released from memory when not needed anymore.
*/
void PRREManaged::PRREManagedImpl::FlushResources()
{

} // FlushResources()


/**
    Gets the amount of allocated system memory for this managed.
    Memory usage of owned managed objects are NOT calculated here.

    @return Amount of used system memory in Bytes.
*/
TPRREuint PRREManaged::PRREManagedImpl::getUsedSystemMemory() const
{
    return sizeof(*this) + sName.capacity() * sizeof(char);
} // getUsedSystemMemory()


// ############################## PROTECTED ##############################


PRREManaged::PRREManagedImpl::PRREManagedImpl(PRREManaged* owner)
{
    _pOwner = owner;
    pParentMgr = PGENULL;
    nManagedsTotal++;
    _pOwner->getManagedConsole().OLn("PRREManaged(con)");
} // PRREManagedImpl(...)


PRREManaged::PRREManagedImpl::PRREManagedImpl(const PRREManagedImpl& other)
{
}


PRREManaged::PRREManagedImpl& PRREManaged::PRREManagedImpl::operator=(const PRREManagedImpl& other)
{
    return *this;
}


// ############################### PRIVATE ###############################


TPRREint PRREManaged::PRREManagedImpl::nManagedsTotal = 0;


/*
   PRREManaged
   ###########################################################################
*/


// ############################### PUBLIC ################################


PRREManaged::PRREManaged()
{
    pImpl = new PRREManagedImpl(this);
} // PRREManaged(...)


PRREManaged::~PRREManaged()
{
    delete pImpl;
    pImpl = NULL;
} // ~PRREManaged()


CConsole& PRREManaged::getManagedConsole() const
{
    return CConsole::getConsoleInstance("PRREManaged");
} // getConsole()


/**
    Removes the managed from its manager.
    Actually it calls the public Detach() method of the manager with the this pointer.
*/
void PRREManaged::DetachFrom()
{
    pImpl->DetachFrom();
} // DetachFrom()


/**
    Gets the manager of the managed.
    @return Pointer to manager.
*/
PRREManager* PRREManaged::getManager() const
{
    return pImpl->getManager();
} // getManager()


/**
    Gets the name.
*/
const std::string& PRREManaged::getName() const
{
    return pImpl->getName();
}


/**
    Sets the name.
*/
void PRREManaged::SetName(const std::string& name)
{
    pImpl->SetName(name);
}


/**
    This can be used if the specialized managed object has some resources
    that may be released from memory when not needed anymore.
*/
void PRREManaged::FlushResources()
{
    pImpl->FlushResources();
} // FlushResources()


/**
    Gets the amount of allocated system memory for this managed.

    @return Amount of used system memory in Bytes.
*/
TPRREuint PRREManaged::getUsedSystemMemory() const
{
    return sizeof(*this) + pImpl->getUsedSystemMemory();
} // getUsedSystemMemory()


// ############################## PROTECTED ##############################


PRREManaged::PRREManaged(const PRREManaged& other)
{

}


PRREManaged& PRREManaged::operator=(const PRREManaged& other)
{
    return *this;
}


// ############################### PRIVATE ###############################


/*
   PRREManagerImpl
   ###########################################################################
*/


class PRREManager::PRREManagerImpl
{

public:

    virtual ~PRREManagerImpl();

    TPRREint     getCount() const;                       
    TPRREbool    isEmpty() const;                         
    TPRREint     getSize() const;                         
    PRREManaged* getAttachedAt(TPRREint ind) const;
    TPRREint     getAttachedIndex(const PRREManaged& m) const;
    void         PreAlloc(TPRREint count);                
    TPRREbool    hasAttached(const PRREManaged& m) const; 
    void         Attach(PRREManaged& m);                  
    void         Detach(PRREManaged& m);                  
    void         DeleteAttachedInstance(PRREManaged& m);  
    void         DeleteAll();   
    TPRREuint    getUsedSystemMemory() const;

protected:

    // ---------------------------------------------------------------------------

private:
    PRREManager*      _pOwner;            /**< The owner public object who creates this pimpl object. */
    TPRREint          nManagedCount;      /**< Number of managed objects, only readable for descendants: getCount(). */
    TPRREint          nManagedAllocated;  /**< Number of allocated slots for managed objects, nManagedAllocated >= nManagedCount, only readable for descendants: getSize(). */
    PRREManaged**     pManageds;          /**< Pointer to array of pointers to managed objects, free slots contain NULLs, only readable: getAttachedAt(). */

    // ---------------------------------------------------------------------------

    explicit PRREManagerImpl(PRREManager* pOwner);  
    PRREManagerImpl(const PRREManagerImpl&);
    PRREManagerImpl& operator=(const PRREManagerImpl&);

    TPRREint preAlloc(TPRREint count);  /**< Extends the array by the given value. */
    TPRREint createManaged();           /**< Gets an index to a free slot for a new managed object. */
    void     Detach(TPRREint ind);      /**< Removes the given indexed object from the array (but doesn't destructs it!). */

    friend class PRREManager;

};


// ############################### PUBLIC ################################


PRREManager::PRREManagerImpl::~PRREManagerImpl()
{
    _pOwner->getConsole().OLnOI("~PRREManager() ...");
    DeleteAll();
    _pOwner->getConsole().OO();
    _pOwner = NULL;
} // ~PRREManagerImpl()


TPRREint PRREManager::PRREManagerImpl::getCount() const
{
    return nManagedCount;
} // getCount()


TPRREbool PRREManager::PRREManagerImpl::isEmpty() const
{
    return (nManagedCount == 0);
} // isEmpty()


TPRREint PRREManager::PRREManagerImpl::getSize() const
{
    return nManagedAllocated;
} // getSize()


PRREManaged* PRREManager::PRREManagerImpl::getAttachedAt(TPRREint ind) const
{
    if ( (ind > -1) && (ind < nManagedAllocated) )
        return pManageds[ind];
    else
        return PGENULL;
} // getAttachedAt()


TPRREint PRREManager::PRREManagerImpl::getAttachedIndex(const PRREManaged& m) const
{
    for (TPRREint i = 0; i < nManagedAllocated; i++)
    {
        if ( pManageds[i] == &m )
            return i;
    }
    return -1;
} // getAttachedIndex()


void PRREManager::PRREManagerImpl::PreAlloc(TPRREint count)
{
    preAlloc(count);
} // PreAlloc()


TPRREbool PRREManager::PRREManagerImpl::hasAttached(const PRREManaged& m) const
{
    return ( getAttachedIndex(m) != -1 );
} // hasAttached()


void PRREManager::PRREManagerImpl::Attach(PRREManaged& m)
{
    _pOwner->getConsole().OLnOI("PRREManager::Attach(...)");
    if ( m.getManager() == PGENULL )
    {
        TPRREint newIndex = createManaged();
        pManageds[newIndex] = &m;
        m.pImpl->pParentMgr = _pOwner;
        nManagedCount++;
        _pOwner->getConsole().SOLn("> Attach Done!");
    }
    else
    {
        // _pOwner->errLast = PRRE_ERR_NOTMANAGEDBY;
        //getConsole().EOLn("ERROR: managed is already managed by another manager!");
    }
    _pOwner->getConsole().OO();
} // Attach()


void PRREManager::PRREManagerImpl::Detach(PRREManaged& m)
{
    _pOwner->getConsole().OLnOI("public PRREManager::Detach(...)");
    Detach( getAttachedIndex(m) );
    _pOwner->getConsole().OO();
} // Detach()


void PRREManager::PRREManagerImpl::DeleteAttachedInstance(PRREManaged& m)
{
    _pOwner->getConsole().OLn("PRREManager::DeleteAttachedInstance(...)");

    bool found = false;
    for (TPRREint i = 0; (i < nManagedAllocated) && !found; i++)
    {
        if ( pManageds[i] == &m )
        {
            _pOwner->getConsole().SOLn("  > Found managed instance @ pManageds[%d]!", i);
            delete pManageds[i];
            // no need to set element to NULL because manageds' dtor always calls DetachFrom() which takes care of that!
            found = true;
            break;
        }
    }
    if ( !found )
    {
        _pOwner->getConsole().EOLn("  ERROR: Managed not found!");
        // errLast = PRRE_ERR_NOTMANAGEDBY;
    }
    _pOwner->getConsole().OLn("");
} // DeleteAttachedInstance()


void PRREManager::PRREManagerImpl::DeleteAll()
{
    _pOwner->getConsole().OLn("PRREManager::DeleteAll()");
    if ( pManageds == PGENULL )
        return;

    for (TPRREint i = 0; i < nManagedAllocated; i++)
    {
        if ( pManageds[i] != PGENULL )
            delete pManageds[i];
    }
    delete[] pManageds;
    pManageds = PGENULL;
    nManagedAllocated = 0; 
    if ( nManagedCount != 0 )
    {   // should be 0 because manageds' dtor always calls DetachFrom()
        _pOwner->getConsole().EOLn("  ERROR: nManagedCount != 0 !!!");
    }
} // DeleteAll()


TPRREuint PRREManager::PRREManagerImpl::getUsedSystemMemory() const
{
    TPRREuint sum = 0;
    for (TPRREint i = 0; i < getSize(); i++)
    {
        if ( pManageds[i] == PGENULL )
          continue;

        sum += pManageds[i]->getUsedSystemMemory();
    }
    return sum;
} // getUsedSystemMemory()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


/**
    @param pOwner The public class owning this pimpl object.
*/
PRREManager::PRREManagerImpl::PRREManagerImpl(PRREManager* pOwner)
{
    _pOwner = pOwner;
    _pOwner->getConsole().O("PRREManager(console) ... ");
    nManagedCount = 0;
    nManagedAllocated = 0;
    pManageds = PGENULL;
    _pOwner->getConsole().SOLn("done!");
} // PRREManagerImpl(...)


PRREManager::PRREManagerImpl::PRREManagerImpl(const PRREManager::PRREManagerImpl& other)
{
    _pOwner = NULL;  // DO NOT copy the owner, set it to NULL. The owner should set this afterwards.
    /*
    nManagedCount = other.nManagedCount;
    nManagedAllocated = other.nManagedAllocated;
    PreAlloc(other.nManagedAllocated);
    for (int i = 0; i < other.nManagedAllocated; i++)
    {
        if ( other.getAttachedAt(i) != PGENULL )
        {
            // TODO: dont know if the manageds should be simply attached which modify their pparent,
            // or the manageds should be duplicated too and duplicates should be attached here ...
        }
    }
    */
}


PRREManager::PRREManagerImpl& PRREManager::PRREManagerImpl::operator=(const PRREManager::PRREManagerImpl& other)
{
    return *this;
}


/** 
    Extends the array by the given value.
    So if it has 4 manageds and count is 10, then the total number of slots will be 14.
    Only positive value is accepted.
    Sometimes its good to preallocate many slots only once instead of allocation over and over a few new slots, to avoid memory fragmentation.

    @return The first free slot of the NEW slots. 
            -1 if the given parameter is not positive or allocation error occurred.
*/
TPRREint PRREManager::PRREManagerImpl::preAlloc(TPRREint count)
{
    _pOwner->getConsole().OLn("PRREManager::preAlloc(%d) ...", count);
    if ( count > 0 )
    {
        TPRREint prev_h = nManagedAllocated;
        nManagedAllocated += count;
        PRREManaged** pNewStorage = PGENULL;
        try
        {
            pNewStorage = new PRREManaged*[nManagedAllocated];
        }
        catch (const std::bad_alloc&)
        {
            _pOwner->getConsole().EOLnOO("ERROR: failed to alloc new storage!");
            return -1;
        }
        memcpy(pNewStorage, pManageds, prev_h*sizeof(PRREManaged*));
        delete[] pManageds;
        pManageds = pNewStorage;
        for (TPRREint i = prev_h; i < nManagedAllocated; i++)                  
            pManageds[i] = PGENULL;
        return prev_h;
    }
    
    _pOwner->getConsole().EOLn("PRREManager::preAlloc(%d) ERROR!", count);
    return -1;
} // preAlloc()


/**
    Gets an index to a free slot for a new managed object.
    If there is 1 free slot somewhere, that's index will be returned, otherwise it'll allocate a new slot.

    @return Non-zero index to a free slot, -1 on error.
*/
TPRREint PRREManager::PRREManagerImpl::createManaged()
{
    if ( getCount() < getSize() )
    {   // we have a free slot, find it ...
        for (TPRREint i = 0; i < getSize(); i++)
        {
            if ( pManageds[i] == PGENULL )
                return i;
        }
        // we should never reach this point ...
        return -1;
    }
    else return preAlloc(1);
} // createManaged()


/**
    Removes the given indexed object from the array (but doesn't destructs it!).
    This is called by the public version of this function and by DeleteManagedInstance().
    Nulls parentMgr because maybe we don't want to destruct the managed, just release it from the manager.
*/
void PRREManager::PRREManagerImpl::Detach(TPRREint ind)
{
    _pOwner->getConsole().OLn("protected PRREManager::Detach(%d)", ind);
    if ( ind > -1 )
    {
        pManageds[ ind ]->pImpl->pParentMgr = PGENULL;
        pManageds[ ind ] = PGENULL;
        nManagedCount--;
    }
} // Detach()


/*
   PRREManager
   ###########################################################################
*/


// ############################### PUBLIC ################################


/**
    @exception std::bad_alloc - This class or its ancestor dynamically allocates memory with operator new, in case of failure the exception is not handled but propagated to caller.
*/
PRREManager::PRREManager()
{
    pImpl = new PRREManagerImpl(this);
} // PRREManager(...)


PRREManager::~PRREManager()
{
    delete pImpl;
    pImpl = NULL;
} // ~PRREManager()


CConsole& PRREManager::getConsole() const
{
    return CConsole::getConsoleInstance("PRREManaged");
} // getConsole()


/**
    Gets the number of managed objects.
    @return Number of managed objects.
*/
TPRREint PRREManager::getCount() const
{
    return pImpl->getCount();
} // getCount()


/**
    Is the number of manageds 0?
    @return True, if number of manageds is 0, false otherwise.
*/
TPRREbool PRREManager::isEmpty() const
{
    return pImpl->isEmpty();
} // isEmpty()


/**
    Gets the number of allocated slots for managed objects; getSize() >= getCount().
    This doesn't tell about number of managed objects, just number of slots.

    @return Number of allocated slots, a non-negative value.
*/
TPRREint PRREManager::getSize() const
{
    return pImpl->getSize();
} // getSize()


/**
    Gets the pointer to the managed at the given index.
    @return Pointer to the managed at the given index (can be PGENULL in case of invalid index).
*/
PRREManaged* PRREManager::getAttachedAt(TPRREint ind) const
{
    return pImpl->getAttachedAt(ind);
} // getAttachedAt()


/**
    Gets the index of the given managed.
    @return Index of the given object. -1, if the given managed is not managed by this manager.
*/
TPRREint PRREManager::getAttachedIndex(const PRREManaged& m) const
{
    return pImpl->getAttachedIndex(m);
}


/**
    Allocates count free slots for manageds.
*/
void PRREManager::PreAlloc(TPRREint count)
{
    pImpl->PreAlloc(count);
} // PreAlloc()


/**
    Gets whether the given managed is managed by this manager.
    @return True if the given managed is managed by this manager, false otherwise.
*/
TPRREbool PRREManager::hasAttached(const PRREManaged& m) const
{
    return pImpl->hasAttached(m);
} // hasAttached()


/** 
    Adds the given managed to the manager, if the managed has no manager yet.
    If m is a nullpointer, value of getLastError() will be PRRE_ERR_NULLPOINTER.
    If the given managed already has a manager, value of getLastError() will be PRRE_ERR_NOTMANAGEDBY.
*/
void PRREManager::Attach(PRREManaged& m)
{
    pImpl->Attach(m);
} // Attach()


/**
    Removes the given managed from the manager, so the managed will have no manager.
    A managed calls this in its destructor: notifies the manager about the destruction so the manager will remove it from itself.
    See: protected Detach().
*/
void PRREManager::Detach(PRREManaged& m)
{
    pImpl->Detach(m);
} // Detach()


/**
    Removes the given managed from the manager and destructs it.
    If the given managed is not found, value of getLastError() will be PRRE_ERR_NOTMANAGEDBY.
*/
void PRREManager::DeleteAttachedInstance(PRREManaged& m)
{
    pImpl->DeleteAttachedInstance(m);
} // DeleteAttachedInstance()


/**
    Removes and destructs every managed element in the manager.
*/
void PRREManager::DeleteAll()
{
    pImpl->DeleteAll();
} // DeleteAll()


TPRREuint PRREManager::getUsedSystemMemory() const
{
    return pImpl->getUsedSystemMemory();
} // getUsedSystemMemory()


/**
    Writes a list of managed objects to the console. 
*/
void PRREManager::WriteList() const
{
    getConsole().OLn("PRREManager::WriteList()");
    getConsole().OLn("========================");
    const TPRREuint nSysMemTotal = getUsedSystemMemory();

    getConsole().OIOLn("total slots == %d, # of manageds == %d", getSize(), getCount());
    for (TPRREint i = 0; i < getSize(); i++)
    {
        const PRREManaged* const pMngd = getAttachedAt(i);
        if ( pMngd != PGENULL )
        {    
            getConsole().OLn("pManageds[%d], sys mem: %d Bytes:", i, pMngd->getUsedSystemMemory());
            WriteListCallback( *pMngd );
        }
        else
            getConsole().OLn("pManageds[%d] == PGENULL (free slot)", i);
    }
    getConsole().OLnOO("> total sys mem = %d Bytes <= %d kBytes <= %d MBytes",
                      nSysMemTotal, (int)ceil(nSysMemTotal/1024.0f), (int)ceil(nSysMemTotal/1024.0f/1024.0f));
    getConsole().OLn("");
} // WriteList()


// ############################## PROTECTED ##############################


/**
    @exception std::bad_alloc - This class or its ancestor dynamically allocates memory with operator new, in case of failure the exception is not handled but propagated to caller.
*/
PRREManager::PRREManager(const PRREManager& other)
{
    pImpl = new PRREManagerImpl(this);
    *pImpl = *(other.pImpl);
}


PRREManager& PRREManager::operator=(const PRREManager& other)
{
    *pImpl = *(other.pImpl);
    return *this;
}


/**
    WriteList() calls this for every single managed object.
    Just for debugging purposes.
    This should be also overridden to provide more precise info about the descendant managed object.
*/
void PRREManager::WriteListCallback(const PRREManaged& mngd) const
{
    getConsole().OI();
    getConsole().O("%s, ", mngd.getName().c_str());

    if ( mngd.getManager() == (PRREManager*)this )
        getConsole().SOLn("parent is OK!");
    else
    {
        if ( mngd.getManager() == PGENULL )
            getConsole().EOLn("parent is PGENULL!");
        else
            getConsole().EOLn("parent is VERY BAD (cannot happen)!");
    }
    
    getConsole().OO();
} // WriteListCallback()


// ############################### PRIVATE ###############################



