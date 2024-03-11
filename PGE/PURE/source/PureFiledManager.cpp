/*
    ###################################################################################
    PureFiledManager.cpp
    This file is part of PURE.
    PureFiledManager and PureFiledManaged classes.
    Manager loading from / saving to a file.
    Managed loaded from / saved to a file.
    Made by PR00F88
    ###################################################################################
*/

#include "PureBaseIncludes.h"  // PCH
#include "../include/external/PureFiledManager.h"
#include "../include/internal/PurePragmas.h"

using namespace std;


/*
   PureFiledManagedImpl
   ###########################################################################
*/



/**
    Managed loaded from / saved to a file.
*/
class PureFiledManaged::PureFiledManagedImpl
{
public:

    virtual ~PureFiledManagedImpl();

    const std::string& getFilename() const; 
    void SetFilename(const std::string& filename); 

protected:

    // ---------------------------------------------------------------------------

private:

    // ---------------------------------------------------------------------------

    std::string sFilename;  /**< Filename. */

    PureFiledManagedImpl();
    PureFiledManagedImpl(const PureFiledManagedImpl&);
    PureFiledManagedImpl& operator=(const PureFiledManagedImpl&);

    friend class PureFiledManaged;

}; // class PureFiledManagedImpl


// ############################### PUBLIC ################################


PureFiledManaged::PureFiledManagedImpl::~PureFiledManagedImpl()
{

} // ~PureFiledManagedImpl


/**
    Gets the filename.
*/
const string& PureFiledManaged::PureFiledManagedImpl::getFilename() const
{
    return sFilename;
} // getFilename()


/**
    Sets the filename.
*/
void PureFiledManaged::PureFiledManagedImpl::SetFilename(const string& filename)
{
    sFilename = filename;
} // SetFilename()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


PureFiledManaged::PureFiledManagedImpl::PureFiledManagedImpl()
{

} // PureFiledManagedImpl()


PureFiledManaged::PureFiledManagedImpl::PureFiledManagedImpl(const PureFiledManaged::PureFiledManagedImpl&)
{

}


PureFiledManaged::PureFiledManagedImpl& PureFiledManaged::PureFiledManagedImpl::operator=(const PureFiledManaged::PureFiledManagedImpl&)
{
    return *this;
}


/*
   PureFiledManaged
   ###########################################################################
*/


// ############################### PUBLIC ################################


/**
    @exception std::bad_alloc - This class or its ancestor dynamically allocates memory with operator new, in case of failure the exception is not handled but propagated to caller.
*/
PureFiledManaged::PureFiledManaged()
{
    pImpl = new PureFiledManagedImpl();
} // PureFiledManaged()


PureFiledManaged::~PureFiledManaged()
{
    delete pImpl;
    pImpl = NULL;
} // ~PureFiledManaged


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& PureFiledManaged::getManagedConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* PureFiledManaged::getLoggerModuleName()
{
    return "PureFiledManaged";
} // getLoggerModuleName()


/**
    Gets the amount of allocated system memory for this managed.

    @return Amount of used system memory in Bytes.
*/
TPureUInt PureFiledManaged::getUsedSystemMemory() const
{
    // both PureManaged::getUsedSystemMemory() and sizeof(*this) includes sizeof(PureManaged) hence we subtract that
    return PureManaged::getUsedSystemMemory() - sizeof(PureManaged) + sizeof(*this) + sizeof(*pImpl) + pImpl->sFilename.capacity() * sizeof(char);
} // getUsedSystemMemory()


/**
    Gets the filename.

    @return Gets the filename set during initialization.
*/
const string& PureFiledManaged::getFilename() const
{
    return pImpl->getFilename();
} // getFilename()


// ############################## PROTECTED ##############################


/**
    @exception std::bad_alloc - This class or its ancestor dynamically allocates memory with operator new, in case of failure the exception is not handled but propagated to caller.
*/
PureFiledManaged::PureFiledManaged(const PureFiledManaged& other)
{
    pImpl = new PureFiledManagedImpl();
    SetFilename(other.getFilename());
}


PureFiledManaged& PureFiledManaged::operator=(const PureFiledManaged& other)
{
    SetFilename(other.getFilename());
    return *this;
}


/**
    Sets the filename.
*/
void PureFiledManaged::SetFilename(const string& filename)
{
    pImpl->SetFilename(filename);
} // SetFilename()


// ############################### PRIVATE ###############################



/*
   PureFiledManager::PureFiledManagerImpl
   ###########################################################################
*/


class PureFiledManager::PureFiledManagerImpl
{
public:
    virtual ~PureFiledManagerImpl();

    PureFiledManaged*
        getByFilename(const char* filename) const;  /**< Gets the FiledManaged by the specified filename. */
    
    virtual PureFiledManaged*
        createFromFile( const char* filename);      /**< Should create a new managed from the given file. */

protected:
    explicit PureFiledManagerImpl(PureFiledManager* parent);

    PureFiledManagerImpl(const PureFiledManagerImpl&);
    PureFiledManagerImpl& operator=(const PureFiledManagerImpl&);

private:

    PureFiledManager* _pOwner;       /**< The owner public object who creates this pimpl object. */

    friend class PureFiledManager;

}; // class PureFiledManager::PureFiledManagerImpl


// ############################### PUBLIC ################################


PureFiledManager::PureFiledManagerImpl::~PureFiledManagerImpl()
{

} // ~PureFiledManagerImpl


/** 
    Gets the FiledManaged by the specified filename.

    @return FiledManaged instance found by filename, or PGENULL if not found.
*/
PureFiledManaged* PureFiledManager::PureFiledManagerImpl::getByFilename(const char* filename) const
{
    for (int i = 0; i < _pOwner->getCount(); i++)
    {
        PureFiledManaged* managed = (PureFiledManaged*) _pOwner->getAttachedAt(i);
        if ( managed != PGENULL )
        {
            if ( strcmp(managed->getFilename().c_str(), filename) == 0 )
                return managed;
        }
    }
    return PGENULL;
} // getByFilename()


/**
    Should create a new managed from the given file.
    This should be overrid in the descendant.

    @return Always PGENULL. Descendant is responsible for proper implementation.
*/
PureFiledManaged* PureFiledManager::PureFiledManagerImpl::createFromFile(const char* filename)
{
    return PGENULL;
} // createFromFile()


// ############################## PROTECTED ##############################


PureFiledManager::PureFiledManagerImpl::PureFiledManagerImpl(PureFiledManager* parent)
{
    _pOwner = parent;
} // PureFiledManagerImpl()


PureFiledManager::PureFiledManagerImpl::PureFiledManagerImpl(const PureFiledManagerImpl&)
{

}


PureFiledManager::PureFiledManagerImpl& PureFiledManager::PureFiledManagerImpl::operator=(const PureFiledManagerImpl&)
{
    return *this;
}


// ############################### PRIVATE ###############################



/*
   PureFiledManager
   ###########################################################################
*/


// ############################### PUBLIC ################################


PureFiledManager::PureFiledManager() : PureManager()
{
    pImpl = new PureFiledManagerImpl(this);
} // PureFiledManager()


PureFiledManager::~PureFiledManager()
{
    delete pImpl;
    pImpl = NULL;
} // ~PureFiledManager


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& PureFiledManager::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* PureFiledManager::getLoggerModuleName()
{
    return "PureFiledManager";
} // getLoggerModuleName()


/** 
    Gets the FiledManaged by the specified filename.

    @return FiledManaged instance found by filename, or PGENULL if not found.
*/
PureFiledManaged* PureFiledManager::getByFilename(const char* filename) const
{
    return pImpl->getByFilename(filename);
} // getByFilename()


/**
    Should create a new managed from the given file.
    This should be overrid in the descendant.

    @return Always PGENULL. Descendant is responsible for proper implementation.
*/
PureFiledManaged* PureFiledManager::createFromFile(const char* filename)
{
    return pImpl->createFromFile(filename);
} // createFromFile()


/**
    From PureManager, adding logging file names.
*/
void PureFiledManager::WriteList() const
{
    getConsole().OLn("PureFiledManager::WriteList()");
    getConsole().OLn("===============================");
    getConsole().OLnOI("");
    PureManager::WriteList();
    getConsole().OO();
} // WriteList()


// ############################## PROTECTED ##############################


PureFiledManager::PureFiledManager(const PureFiledManager&)
{

}


PureFiledManager& PureFiledManager::operator=(const PureFiledManager&)
{
    return *this;
}


/**
    From PureManager, printing filename.
*/
void PureFiledManager::WriteListCallback(const PureManaged& mngd) const
{
    PureManager::WriteListCallback(mngd);
    PureFiledManaged& fmngd = (PureFiledManaged&) mngd;
    getConsole().OIOLnOO("filename: %s", (fmngd.getFilename().empty() ? "*none*" : fmngd.getFilename().c_str()));
} // WriteListCallback()


// ############################### PRIVATE ###############################

