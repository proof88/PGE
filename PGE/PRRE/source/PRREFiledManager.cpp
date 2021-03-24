/*
    ###################################################################################
    PRREFiledManager.cpp
    This file is part of PRRE.
    PRREFiledManager and PRREFiledManaged classes.
    Manager loading from / saving to a file.
    Managed loaded from / saved to a file.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREbaseIncludes.h"  // PCH
#include "../include/external/PRREFiledManager.h"
#include "../include/internal/PRREpragmas.h"

using namespace std;


/*
   PRREFiledManagedImpl
   ###########################################################################
*/



/**
    Managed loaded from / saved to a file.
*/
class PRREFiledManaged::PRREFiledManagedImpl
{
public:

    virtual ~PRREFiledManagedImpl();

    const std::string& getFilename() const; 
    void SetFilename(const std::string& filename); 

protected:

    // ---------------------------------------------------------------------------

private:

    // ---------------------------------------------------------------------------

    std::string sFilename;  /**< Filename. */

    PRREFiledManagedImpl();
    PRREFiledManagedImpl(const PRREFiledManagedImpl&);
    PRREFiledManagedImpl& operator=(const PRREFiledManagedImpl&);

    friend class PRREFiledManaged;

}; // class PRREFiledManagedImpl


// ############################### PUBLIC ################################


PRREFiledManaged::PRREFiledManagedImpl::~PRREFiledManagedImpl()
{

} // ~PRREFiledManagedImpl


/**
    Gets the filename.
*/
const string& PRREFiledManaged::PRREFiledManagedImpl::getFilename() const
{
    return sFilename;
} // getFilename()


/**
    Sets the filename.
*/
void PRREFiledManaged::PRREFiledManagedImpl::SetFilename(const string& filename)
{
    sFilename = filename;
} // SetFilename()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


PRREFiledManaged::PRREFiledManagedImpl::PRREFiledManagedImpl()
{

} // PRREFiledManagedImpl()


PRREFiledManaged::PRREFiledManagedImpl::PRREFiledManagedImpl(const PRREFiledManaged::PRREFiledManagedImpl&)
{

}


PRREFiledManaged::PRREFiledManagedImpl& PRREFiledManaged::PRREFiledManagedImpl::operator=(const PRREFiledManaged::PRREFiledManagedImpl&)
{
    return *this;
}


/*
   PRREFiledManaged
   ###########################################################################
*/


// ############################### PUBLIC ################################


/**
    @exception std::bad_alloc - This class or its ancestor dynamically allocates memory with operator new, in case of failure the exception is not handled but propagated to caller.
*/
PRREFiledManaged::PRREFiledManaged()
{
    pImpl = new PRREFiledManagedImpl();
} // PRREFiledManaged()


PRREFiledManaged::~PRREFiledManaged()
{
    delete pImpl;
    pImpl = NULL;
} // ~PRREFiledManaged


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& PRREFiledManaged::getManagedConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* PRREFiledManaged::getLoggerModuleName() const
{
    return "PRREFiledManaged";
} // getLoggerModuleName()


/**
    Gets the amount of allocated system memory for this managed.

    @return Amount of used system memory in Bytes.
*/
TPRREuint PRREFiledManaged::getUsedSystemMemory() const
{
    // both PRREManaged::getUsedSystemMemory() and sizeof(*this) includes sizeof(PRREManaged) hence we subtract that
    return PRREManaged::getUsedSystemMemory() - sizeof(PRREManaged) + sizeof(*this) + sizeof(*pImpl) + pImpl->sFilename.capacity() * sizeof(char);
} // getUsedSystemMemory()


/**
    Gets the filename.

    @return Gets the filename set during initialization.
*/
const string& PRREFiledManaged::getFilename() const
{
    return pImpl->getFilename();
} // getFilename()


// ############################## PROTECTED ##############################


/**
    @exception std::bad_alloc - This class or its ancestor dynamically allocates memory with operator new, in case of failure the exception is not handled but propagated to caller.
*/
PRREFiledManaged::PRREFiledManaged(const PRREFiledManaged& other)
{
    pImpl = new PRREFiledManagedImpl();
    SetFilename(other.getFilename());
}


PRREFiledManaged& PRREFiledManaged::operator=(const PRREFiledManaged& other)
{
    SetFilename(other.getFilename());
    return *this;
}


/**
    Sets the filename.
*/
void PRREFiledManaged::SetFilename(const string& filename)
{
    pImpl->SetFilename(filename);
} // SetFilename()


// ############################### PRIVATE ###############################



/*
   PRREFiledManager::PRREFiledManagerImpl
   ###########################################################################
*/


class PRREFiledManager::PRREFiledManagerImpl
{
public:
    virtual ~PRREFiledManagerImpl();

    PRREFiledManaged*
        getByFilename(const char* filename) const;  /**< Gets the FiledManaged by the specified filename. */
    
    virtual PRREFiledManaged*
        createFromFile( const char* filename);      /**< Should create a new managed from the given file. */

protected:
    explicit PRREFiledManagerImpl(PRREFiledManager* parent);

    PRREFiledManagerImpl(const PRREFiledManagerImpl&);
    PRREFiledManagerImpl& operator=(const PRREFiledManagerImpl&);

private:

    PRREFiledManager* _pOwner;       /**< The owner public object who creates this pimpl object. */

    friend class PRREFiledManager;

}; // class PRREFiledManager::PRREFiledManagerImpl


// ############################### PUBLIC ################################


PRREFiledManager::PRREFiledManagerImpl::~PRREFiledManagerImpl()
{

} // ~PRREFiledManagerImpl


/** 
    Gets the FiledManaged by the specified filename.

    @return FiledManaged instance found by filename, or PGENULL if not found.
*/
PRREFiledManaged* PRREFiledManager::PRREFiledManagerImpl::getByFilename(const char* filename) const
{
    for (int i = 0; i < _pOwner->getCount(); i++)
    {
        PRREFiledManaged* managed = (PRREFiledManaged*) _pOwner->getAttachedAt(i);
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
PRREFiledManaged* PRREFiledManager::PRREFiledManagerImpl::createFromFile(const char* filename)
{
    return PGENULL;
} // createFromFile()


// ############################## PROTECTED ##############################


PRREFiledManager::PRREFiledManagerImpl::PRREFiledManagerImpl(PRREFiledManager* parent)
{
    _pOwner = parent;
} // PRREFiledManagerImpl()


PRREFiledManager::PRREFiledManagerImpl::PRREFiledManagerImpl(const PRREFiledManagerImpl&)
{

}


PRREFiledManager::PRREFiledManagerImpl& PRREFiledManager::PRREFiledManagerImpl::operator=(const PRREFiledManagerImpl&)
{
    return *this;
}


// ############################### PRIVATE ###############################



/*
   PRREFiledManager
   ###########################################################################
*/


// ############################### PUBLIC ################################


PRREFiledManager::PRREFiledManager() : PRREManager()
{
    pImpl = new PRREFiledManagerImpl(this);
} // PRREFiledManager()


PRREFiledManager::~PRREFiledManager()
{
    delete pImpl;
    pImpl = NULL;
} // ~PRREFiledManager


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& PRREFiledManager::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* PRREFiledManager::getLoggerModuleName() const
{
    return "PRREFiledManager";
} // getLoggerModuleName()


/** 
    Gets the FiledManaged by the specified filename.

    @return FiledManaged instance found by filename, or PGENULL if not found.
*/
PRREFiledManaged* PRREFiledManager::getByFilename(const char* filename) const
{
    return pImpl->getByFilename(filename);
} // getByFilename()


/**
    Should create a new managed from the given file.
    This should be overrid in the descendant.

    @return Always PGENULL. Descendant is responsible for proper implementation.
*/
PRREFiledManaged* PRREFiledManager::createFromFile(const char* filename)
{
    return pImpl->createFromFile(filename);
} // createFromFile()


void PRREFiledManager::WriteList() const
{
    getConsole().OLn("PRREFiledManager::WriteList()");
    getConsole().OLn("===============================");
    getConsole().OLnOI("");
    PRREManager::WriteList();
    getConsole().OO();
} // WriteList()


// ############################## PROTECTED ##############################


PRREFiledManager::PRREFiledManager(const PRREFiledManager&)
{

}


PRREFiledManager& PRREFiledManager::operator=(const PRREFiledManager&)
{
    return *this;
}


/**
    From PRREManager, printing filename.
*/
void PRREFiledManager::WriteListCallback(const PRREManaged& mngd) const
{
    PRREManager::WriteListCallback(mngd);
    PRREFiledManaged& fmngd = (PRREFiledManaged&) mngd;
    getConsole().OIOLnOO("filename: %s", (fmngd.getFilename().empty() ? "*none*" : fmngd.getFilename().c_str()));
} // WriteListCallback()


// ############################### PRIVATE ###############################

