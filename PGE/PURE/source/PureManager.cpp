/*
    ###################################################################################
    PureManager.cpp
    This file is part of PURE.
    Ancestor classes for manager and managed classes.
    Made by PR00F88
    ###################################################################################
*/

#include "PureBaseIncludes.h"  // PCH
#include <stdexcept>
#include "../include/external/PureManager.h"
#include "../include/internal/PureManagedImpl.h"
#include "../include/internal/PurePragmas.h"


/*
   PureManaged::PureManagedImpl
   ###########################################################################
*/

           
// ############################### PUBLIC ################################


PureManaged::PureManagedImpl::~PureManagedImpl()
{
    _pOwner->getManagedConsole().OLnOI("~PureManaged()");
    DetachFrom();
    nManagedsTotal--;
    _pOwner->getManagedConsole().OO();
} // ~PureManagedImpl()


/**
    Removes the managed from its manager.
    Actually it calls the public Detach() method of the manager with the this pointer.
*/
void PureManaged::PureManagedImpl::DetachFrom()
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
PureManager* PureManaged::PureManagedImpl::getManager() const
{
    return pParentMgr;
} // getManager()


/**
    Gets the name.
*/
const std::string& PureManaged::PureManagedImpl::getName() const
{
    return sName;
}


/**
    Sets the name.
*/
void PureManaged::PureManagedImpl::SetName(const std::string& name)
{
    sName = name;
}


/**
    This can be used if the specialized managed object has some resources
    that may be released from memory when not needed anymore.
*/
void PureManaged::PureManagedImpl::FlushResources()
{

} // FlushResources()


/**
    Gets the amount of allocated system memory for this managed.
    Memory usage of owned managed objects are NOT calculated here.

    @return Amount of used system memory in Bytes.
*/
TPureUInt PureManaged::PureManagedImpl::getUsedSystemMemory() const
{
    return sizeof(*this) + sName.capacity() * sizeof(char);
} // getUsedSystemMemory()


// ############################## PROTECTED ##############################


PureManaged::PureManagedImpl::PureManagedImpl(PureManaged* owner)
{
    _pOwner = owner;
    pParentMgr = PGENULL;
    pUtiliser = PGENULL;
    nManagedsTotal++;
    _pOwner->getManagedConsole().OLn("PureManaged(con)");
} // PureManagedImpl(...)


PureManaged::PureManagedImpl::PureManagedImpl(const PureManagedImpl& other)
{
}


PureManaged::PureManagedImpl& PureManaged::PureManagedImpl::operator=(const PureManagedImpl& other)
{
    return *this;
}


// ############################### PRIVATE ###############################


TPureInt PureManaged::PureManagedImpl::nManagedsTotal = 0;


/*
   PureManaged
   ###########################################################################
*/


// ############################### PUBLIC ################################


PureManaged::PureManaged()
{
    pImpl = new PureManagedImpl(this);
} // PureManaged(...)


PureManaged::~PureManaged()
{
    delete pImpl;
    pImpl = NULL;
} // ~PureManaged()


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& PureManaged::getManagedConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* PureManaged::getLoggerModuleName()
{
    return "PureManaged";
} // getLoggerModuleName()


/**
    Returns the managed that utilizes this managed.
    The utiliser is another PureManaged instance, usually having different class, and that class becomes complete with the utilised managed instance.
    For example: every PureMesh3D instance has an associated PureMaterial instance, in this case PureMesh3D instance utilizes a PureMaterial instance,
    so the latter instance's getUtiliser() returns the PureMesh3D instance.
    A PureManaged instance can have multiple utilised PureManaged instances, but any PureManaged instance can have maximum 1 utiliser instance.

    @return The managed that utilizes this managed.
*/
const PureManaged* PureManaged::getUtiliser() const
{
    // BACKLOG: FTR: a PureManaged instance might have multiple utiliser instances ...
    return pImpl->pUtiliser;
} // getUtiliser()


/**
    Returns the managed that utilizes this managed.
    The utiliser is another PureManaged instance, usually having different class, and that class becomes complete with the utilised managed instance.
    For example: every PureMesh3D instance has an associated PureMaterial instance, in this case PureMesh3D instance utilizes a PureMaterial instance,
    so the latter instance's getUtiliser() returns the PureMesh3D instance.
    A PureManaged instance can have multiple utilised PureManaged instances, but any PureManaged instance can have maximum 1 utiliser instance.

    @return The managed that utilizes this managed.
*/
PureManaged* PureManaged::getUtiliser()
{
    // BACKLOG: FTR: a PureManaged instance might have multiple utiliser instances ...
    return pImpl->pUtiliser;
} // getUtiliser()


/**
    Sets the managed that utilizes this managed.
    PGENULL is also accepted.
*/
void PureManaged::SetUtiliser(PureManaged* pUtiliser)
{
    if ( pUtiliser == this )
    {
        getManagedConsole().EOLn("PureManaged::SetUtiliser() ERROR: cannot set self!");
        return;
    }

    pImpl->pUtiliser = pUtiliser;
} // SetUtiliser()


/**
    Removes the managed from its manager.
    Actually it calls the public Detach() method of the manager with the this pointer.
*/
void PureManaged::DetachFrom()
{
    pImpl->DetachFrom();
} // DetachFrom()


/**
    Gets the manager of the managed.
    @return Pointer to manager.
*/
PureManager* PureManaged::getManager() const
{
    return pImpl->getManager();
} // getManager()


/**
    Gets the name.
*/
const std::string& PureManaged::getName() const
{
    return pImpl->getName();
}


/**
    Sets the name.
*/
void PureManaged::SetName(const std::string& name)
{
    pImpl->SetName(name);
}


/**
    This can be used if the specialized managed object has some resources
    that may be released from memory when not needed anymore.
*/
void PureManaged::FlushResources()
{
    pImpl->FlushResources();
} // FlushResources()


/**
    Gets the amount of allocated system memory for this managed.

    @return Amount of used system memory in Bytes.
*/
TPureUInt PureManaged::getUsedSystemMemory() const
{
    return sizeof(*this) + pImpl->getUsedSystemMemory();
} // getUsedSystemMemory()


// ############################## PROTECTED ##############################


PureManaged::PureManaged(const PureManaged& other)
{

}


PureManaged& PureManaged::operator=(const PureManaged& other)
{
    return *this;
}


// ############################### PRIVATE ###############################


/*
   PureManagerImpl
   ###########################################################################
*/


class PureManager::PureManagerImpl
{

public:

    virtual ~PureManagerImpl();

    TPureInt     getCount() const;                       
    TPureBool    isEmpty() const;                         
    TPureInt     getSize() const;                         
    PureManaged* getAttachedAt(TPureInt ind) const;
    TPureInt     getAttachedIndex(const PureManaged& m) const;
    void         PreAlloc(TPureInt count);                
    TPureBool    hasAttached(const PureManaged& m) const; 
    void         Attach(PureManaged& m);                  
    void         Detach(PureManaged& m);                  
    void         DeleteAttachedInstance(PureManaged& m);  
    void         DeleteAll();   
    TPureUInt    getUsedSystemMemory() const;

protected:

    // ---------------------------------------------------------------------------

private:
    PureManager*      _pOwner;            /**< The owner public object who creates this pimpl object. */
    TPureInt          nManagedCount;      /**< Number of managed objects, only readable for descendants: getCount(). */
    TPureInt          nManagedAllocated;  /**< Number of allocated slots for managed objects, nManagedAllocated >= nManagedCount, only readable for descendants: getSize(). */
    PureManaged**     pManageds;          /**< Pointer to array of pointers to managed objects, free slots contain NULLs, only readable: getAttachedAt(). */

    // ---------------------------------------------------------------------------

    explicit PureManagerImpl(PureManager* pOwner);  
    PureManagerImpl(const PureManagerImpl&);
    PureManagerImpl& operator=(const PureManagerImpl&);

    TPureInt preAlloc(TPureInt count);  /**< Extends the array by the given value. */
    TPureInt createManaged();           /**< Gets an index to a free slot for a new managed object. */
    void     Detach(TPureInt ind);      /**< Removes the given indexed object from the array (but doesn't destructs it!). */

    friend class PureManager;

};


// ############################### PUBLIC ################################


PureManager::PureManagerImpl::~PureManagerImpl()
{
    _pOwner->getConsole().OLnOI("~PureManager() ...");
    DeleteAll();
    _pOwner->getConsole().OO();
    _pOwner = NULL;
} // ~PureManagerImpl()


TPureInt PureManager::PureManagerImpl::getCount() const
{
    return nManagedCount;
} // getCount()


TPureBool PureManager::PureManagerImpl::isEmpty() const
{
    return (nManagedCount == 0);
} // isEmpty()


TPureInt PureManager::PureManagerImpl::getSize() const
{
    return nManagedAllocated;
} // getSize()


PureManaged* PureManager::PureManagerImpl::getAttachedAt(TPureInt ind) const
{
    if ( (ind > -1) && (ind < nManagedAllocated) )
        return pManageds[ind];
    else
        return PGENULL;
} // getAttachedAt()


TPureInt PureManager::PureManagerImpl::getAttachedIndex(const PureManaged& m) const
{
    for (TPureInt i = 0; i < nManagedAllocated; i++)
    {
        if ( pManageds[i] == &m )
            return i;
    }
    return -1;
} // getAttachedIndex()


void PureManager::PureManagerImpl::PreAlloc(TPureInt count)
{
    preAlloc(count);
} // PreAlloc()


TPureBool PureManager::PureManagerImpl::hasAttached(const PureManaged& m) const
{
    return ( getAttachedIndex(m) != -1 );
} // hasAttached()


void PureManager::PureManagerImpl::Attach(PureManaged& m)
{
    _pOwner->getConsole().OLnOI("PureManager::Attach(...)");
    if ( m.getManager() == PGENULL )
    {
        TPureInt newIndex = createManaged();
        pManageds[newIndex] = &m;
        m.pImpl->pParentMgr = _pOwner;
        nManagedCount++;
        _pOwner->getConsole().SOLn("> Attach Done!");
    }
    else
    {
        // _pOwner->errLast = PURE_ERR_NOTMANAGEDBY;
        //getConsole().EOLn("ERROR: managed is already managed by another manager!");
    }
    _pOwner->getConsole().OO();
} // Attach()


void PureManager::PureManagerImpl::Detach(PureManaged& m)
{
    _pOwner->getConsole().OLnOI("public PureManager::Detach(...)");
    Detach( getAttachedIndex(m) );
    _pOwner->getConsole().OO();
} // Detach()


void PureManager::PureManagerImpl::DeleteAttachedInstance(PureManaged& m)
{
    _pOwner->getConsole().OLn("PureManager::DeleteAttachedInstance(...)");

    bool found = false;
    for (TPureInt i = 0; (i < nManagedAllocated) && !found; i++)
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
        // errLast = PURE_ERR_NOTMANAGEDBY;
    }
    _pOwner->getConsole().OLn("");
} // DeleteAttachedInstance()


void PureManager::PureManagerImpl::DeleteAll()
{
    _pOwner->getConsole().OLn("PureManager::DeleteAll()");
    if ( pManageds == PGENULL )
        return;

    for (TPureInt i = 0; i < nManagedAllocated; i++)
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


TPureUInt PureManager::PureManagerImpl::getUsedSystemMemory() const
{
    TPureUInt sum = 0;
    for (TPureInt i = 0; i < getSize(); i++)
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
PureManager::PureManagerImpl::PureManagerImpl(PureManager* pOwner)
{
    _pOwner = pOwner;
    nManagedCount = 0;
    nManagedAllocated = 0;
    pManageds = PGENULL;
    _pOwner->getConsole().SOLn("PureManager(owner) done!");
} // PureManagerImpl(...)


PureManager::PureManagerImpl::PureManagerImpl(const PureManager::PureManagerImpl& other)
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


PureManager::PureManagerImpl& PureManager::PureManagerImpl::operator=(const PureManager::PureManagerImpl& other)
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
TPureInt PureManager::PureManagerImpl::preAlloc(TPureInt count)
{
    _pOwner->getConsole().OLn("PureManager::preAlloc(%d) ...", count);
    if ( count > 0 )
    {
        TPureInt prev_h = nManagedAllocated;
        nManagedAllocated += count;
        PureManaged** pNewStorage = PGENULL;
        try
        {
            pNewStorage = new PureManaged*[nManagedAllocated];
        }
        catch (const std::bad_alloc&)
        {
            _pOwner->getConsole().EOLnOO("ERROR: failed to alloc new storage!");
            return -1;
        }
        memcpy(pNewStorage, pManageds, prev_h*sizeof(PureManaged*));
        delete[] pManageds;
        pManageds = pNewStorage;
        for (TPureInt i = prev_h; i < nManagedAllocated; i++)                  
            pManageds[i] = PGENULL;
        return prev_h;
    }
    
    _pOwner->getConsole().EOLn("PureManager::preAlloc(%d) ERROR!", count);
    return -1;
} // preAlloc()


/**
    Gets an index to a free slot for a new managed object.
    If there is 1 free slot somewhere, that's index will be returned, otherwise it'll allocate a new slot.

    @return Non-zero index to a free slot, -1 on error.
*/
TPureInt PureManager::PureManagerImpl::createManaged()
{
    if ( getCount() < getSize() )
    {   // we have a free slot, find it ...
        for (TPureInt i = 0; i < getSize(); i++)
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
void PureManager::PureManagerImpl::Detach(TPureInt ind)
{
    _pOwner->getConsole().OLn("protected PureManager::Detach(%d)", ind);
    if ( ind > -1 )
    {
        pManageds[ ind ]->pImpl->pParentMgr = PGENULL;
        pManageds[ ind ] = PGENULL;
        nManagedCount--;
    }
} // Detach()


/*
   PureManager
   ###########################################################################
*/


// ############################### PUBLIC ################################


/**
    @exception std::bad_alloc - This class or its ancestor dynamically allocates memory with operator new, in case of failure the exception is not handled but propagated to caller.
*/
PureManager::PureManager()
{
    pImpl = new PureManagerImpl(this);
} // PureManager(...)


PureManager::~PureManager()
{
    delete pImpl;
    pImpl = NULL;
} // ~PureManager()


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& PureManager::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* PureManager::getLoggerModuleName()
{
    return "PureManager";
} // getLoggerModuleName()


/**
    Gets the number of managed objects.
    @return Number of managed objects.
*/
TPureInt PureManager::getCount() const
{
    return pImpl->getCount();
} // getCount()


/**
    Is the number of manageds 0?
    @return True, if number of manageds is 0, false otherwise.
*/
TPureBool PureManager::isEmpty() const
{
    return pImpl->isEmpty();
} // isEmpty()


/**
    Gets the number of allocated slots for managed objects; getSize() >= getCount().
    This doesn't tell about number of managed objects, just number of slots.

    @return Number of allocated slots, a non-negative value.
*/
TPureInt PureManager::getSize() const
{
    return pImpl->getSize();
} // getSize()


/**
    Gets the pointer to the managed at the given index.
    @return Pointer to the managed at the given index (can be PGENULL in case of invalid index).
*/
PureManaged* PureManager::getAttachedAt(TPureInt ind) const
{
    return pImpl->getAttachedAt(ind);
} // getAttachedAt()


/**
    Gets the index of the given managed.
    @return Index of the given object. -1, if the given managed is not managed by this manager.
*/
TPureInt PureManager::getAttachedIndex(const PureManaged& m) const
{
    return pImpl->getAttachedIndex(m);
}


/**
    Allocates count free slots for manageds.
*/
void PureManager::PreAlloc(TPureInt count)
{
    pImpl->PreAlloc(count);
} // PreAlloc()


/**
    Gets whether the given managed is managed by this manager.
    @return True if the given managed is managed by this manager, false otherwise.
*/
TPureBool PureManager::hasAttached(const PureManaged& m) const
{
    return pImpl->hasAttached(m);
} // hasAttached()


/** 
    Adds the given managed to the manager, if the managed has no manager yet.
    No effect if given managed already has a manager.
*/
void PureManager::Attach(PureManaged& m)
{
    pImpl->Attach(m);
} // Attach()


/**
    Removes the given managed from the manager, so the managed will have no manager.
    A managed calls this in its destructor: notifies the manager about the destruction so the manager will remove it from itself.
    See: protected Detach().
*/
void PureManager::Detach(PureManaged& m)
{
    pImpl->Detach(m);
} // Detach()


/**
    Removes the given managed from the manager and destructs it.
*/
void PureManager::DeleteAttachedInstance(PureManaged& m)
{
    pImpl->DeleteAttachedInstance(m);
} // DeleteAttachedInstance()


/**
    Removes and destructs every managed element in the manager.
*/
void PureManager::DeleteAll()
{
    pImpl->DeleteAll();
} // DeleteAll()


/**
    Should be invoked when a managed's property got changed from a different kind of manager or managed.
    The manager of the managed might be interested in this property change and may implement a change handling functionality by overriding this.
    For example: PureObject3D has an associated PureMaterial instance. When user changes the blending mode of the material, the PureMaterial
    instance invokes the PureObject3D instance's getManager()->HandleManagedPropertyChanged() which will be actually PureObject3DManager's
    implemented change handler, that will handle the changed blend mode of its PureObject3D instance, e.g.: change drawing order in its list, etc.
*/
void PureManager::HandleManagedPropertyChanged(PureManaged& m)
{
    // stays empty in this ancestor class
} // HandleManagedPropertyChanged()


/**
    Gets the amount of allocated system memory for all manageds owner by this manager.
*/
TPureUInt PureManager::getUsedSystemMemory() const
{
    return pImpl->getUsedSystemMemory();
} // getUsedSystemMemory()


/**
    Writes a list of managed objects to the console. 
*/
void PureManager::WriteList() const
{
    getConsole().OLn("PureManager::WriteList()");
    getConsole().OLn("========================");
    const TPureUInt nSysMemTotal = getUsedSystemMemory();

    getConsole().OIOLn("total slots == %d, # of manageds == %d", getSize(), getCount());
    for (TPureInt i = 0; i < getSize(); i++)
    {
        const PureManaged* const pMngd = getAttachedAt(i);
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
PureManager::PureManager(const PureManager& other)
{
    pImpl = new PureManagerImpl(this);
    *pImpl = *(other.pImpl);
}


PureManager& PureManager::operator=(const PureManager& other)
{
    *pImpl = *(other.pImpl);
    return *this;
}


/**
    WriteList() calls this for every single managed object.
    Just for debugging purposes.
    This should be also overridden to provide more precise info about the descendant managed object.
*/
void PureManager::WriteListCallback(const PureManaged& mngd) const
{
    getConsole().OI();

    if (mngd.getManager() == (PureManager*)this)
    {
        getConsole().SOLn("%s parent is OK!", mngd.getName().c_str());
    }
    else
    {
        if ( mngd.getManager() == PGENULL )
            getConsole().EOLn("%s parent is PGENULL!", mngd.getName().c_str());
        else
            getConsole().EOLn("%s parent is VERY BAD (cannot happen)!", mngd.getName().c_str());
    }
    
    getConsole().OO();
} // WriteListCallback()


// ############################### PRIVATE ###############################



