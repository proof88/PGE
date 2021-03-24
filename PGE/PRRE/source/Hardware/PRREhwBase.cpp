/*
    ###################################################################################
    PRREhwBase.cpp
    This file is part of PRRE.
    Base class for PRRE hardware classes.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREbaseIncludes.h"  // PCH
#include "../../include/external/Hardware/PRREhwBase.h"
#include "../../include/internal/PRREpragmas.h"


using namespace std;


/*
   PRREhwBase::PRREhwBaseImpl
   ###########################################################################
*/

class PRREhwBase::PRREhwBaseImpl
{

public:

    // ---------------------------------------------------------------------------

    CConsole&  getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */
    const char* getLoggerModuleName() const;          /**< Returns the logger module name of this class. */

    virtual ~PRREhwBaseImpl();

    void      Initialize(); 
    void      Deinitialize();
    TPRREbool isInitialized() const;

protected:

    // ---------------------------------------------------------------------------

private:
    TPRREbool bInitialized;       /**< Is the instance initialized? False by default.
                                       Will be true if initializeBase() of descendant class succeeds.
                                       Will be false again if Deinitialize() gets called. */

    PRREhwBase* _parent;

    // ---------------------------------------------------------------------------

    explicit PRREhwBaseImpl(PRREhwBase* parent);
    PRREhwBaseImpl(const PRREhwBaseImpl&);
    PRREhwBaseImpl& operator=(const PRREhwBaseImpl&);

    friend class PRREhwBase;

};


// ############################### PUBLIC ################################


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& PRREhwBase::PRREhwBaseImpl::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* PRREhwBase::PRREhwBaseImpl::getLoggerModuleName() const
{
    return "PRREhwBase";
} // getLoggerModuleName()


/**
    Initializes the instance.
*/
void PRREhwBase::PRREhwBaseImpl::Initialize()
{
    getConsole().OIOLn("PRREhwBase::Initialize() ...");
    getConsole().OI();
    if ( bInitialized )
    {
        getConsole().OLnOO("Already initialized!");
        getConsole().OO();
        return;
    }

    _parent->PreInitialize();
    if ( bInitialized = _parent->initializeBase() )
    {
        getConsole().OOSOLn("done!");
    }
    else
    {
        _parent->PreInitialize();
        getConsole().OOEOLn("failed!");
    }

    getConsole().OO();
} // Initialize()


/**
    Deinitializes the instance.
*/
void PRREhwBase::PRREhwBaseImpl::Deinitialize()
{
    if ( !bInitialized )
        return;

    _parent->DeinitializeBase();
    _parent->PreInitialize();
    bInitialized = false;
} // Deinitialize()


/**
    Returns whether the instance is successfully initialized.
*/
TPRREbool PRREhwBase::PRREhwBaseImpl::isInitialized() const
{
    return bInitialized;
} // isInitialized()



// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


/** 
    Sets members to default values.

    @param parent Public class instance owning this pimpl object.
*/
PRREhwBase::PRREhwBaseImpl::PRREhwBaseImpl(PRREhwBase* parent)
{
    bInitialized = false;
    if ( _parent = parent )
    {
        _parent->PreInitialize();
    }
} // PRREhwBase()


PRREhwBase::PRREhwBaseImpl::PRREhwBaseImpl(const PRREhwBaseImpl&)
{

} 


PRREhwBase::PRREhwBaseImpl& PRREhwBase::PRREhwBaseImpl::operator=(const PRREhwBaseImpl&)
{
    return *this;
}
                                             

PRREhwBase::PRREhwBaseImpl::~PRREhwBaseImpl()
{
    Deinitialize();
    _parent = NULL;
} // ~PRREhwBase




/*
   PRREhwBase
   ###########################################################################
*/


// ############################### PUBLIC ################################


/**
    Initializes the instance.
*/
void PRREhwBase::Initialize()
{
    pImpl->Initialize();
} // Initialize()


/**
    Deinitializes the instance.
*/
void PRREhwBase::Deinitialize()
{
    pImpl->Deinitialize();
} // Deinitialize()


/**
    Returns whether the instance is successfully initialized.
*/
TPRREbool PRREhwBase::isInitialized() const
{
    return pImpl->isInitialized();
} // isInitialized()


// ############################## PROTECTED ##############################


/** 
    Sets members to default values.
*/
PRREhwBase::PRREhwBase()
{
    pImpl = new PRREhwBase::PRREhwBaseImpl(this);
} // PRREhwBase()


PRREhwBase::PRREhwBase(const PRREhwBase&)
{

} 


PRREhwBase& PRREhwBase::operator=(const PRREhwBase&)
{
    return *this;
}
                                             

PRREhwBase::~PRREhwBase()
{
    delete pImpl;
    pImpl = NULL;
} // ~PRREhwBase


/**
    Preinitializes members.
    Being called from ctor and Deinitialize().
*/
void PRREhwBase::PreInitialize()
{

}      


// ############################### PRIVATE ###############################
