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
#include "PRREpragmas.h"
#include "PRREFiledManager.h"

using namespace std;


/*
   PRREFiledManagedImpl
   ###########################################################################
*/



/**
    Managed loaded from / saved to a file.

    This class doesn't use any API directly.
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
   PRREFiledManager
   ###########################################################################
*/


// ############################### PUBLIC ################################


PRREFiledManager::PRREFiledManager() : PRREManager()
{

} // PRREFiledManager()


PRREFiledManager::~PRREFiledManager()
{

} // ~PRREFiledManager


/** 
    Gets the FiledManaged by the specified filename.

    @return FiledManaged instance found by filename, or PGENULL if not found.
*/
PRREFiledManaged* PRREFiledManager::getByFilename(const char* filename) const
{
    for (int i = 0; i < getCount(); i++)
    {
        PRREFiledManaged* managed = (PRREFiledManaged*) getAttachedAt(i);
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
PRREFiledManaged* PRREFiledManager::createFromFile(const char* filename)
{
    return PGENULL;
} // createFromFile()


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

