/*
    ###################################################################################
    PureHwBase.cpp
    This file is part of PURE.
    Base class for Pure hardware classes.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PureBaseIncludes.h"  // PCH
#include "../../include/external/Hardware/PureHwBase.h"
#include "../../include/internal/PurePragmas.h"


using namespace std;


/*
   PureHwBase::PurehwBaseImpl
   ###########################################################################
*/

class PureHwBase::PurehwBaseImpl
{

public:

    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    CConsole&  getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */

    virtual ~PurehwBaseImpl();

    void      Initialize(); 
    void      Deinitialize();
    TPureBool isInitialized() const;

protected:

    // ---------------------------------------------------------------------------

private:
    TPureBool bInitialized;       /**< Is the instance initialized? False by default.
                                       Will be true if initializeBase() of descendant class succeeds.
                                       Will be false again if Deinitialize() gets called. */

    PureHwBase* _parent;

    // ---------------------------------------------------------------------------

    explicit PurehwBaseImpl(PureHwBase* parent);
    PurehwBaseImpl(const PurehwBaseImpl&);
    PurehwBaseImpl& operator=(const PurehwBaseImpl&);

    friend class PureHwBase;

};


// ############################### PUBLIC ################################


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& PureHwBase::PurehwBaseImpl::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* PureHwBase::PurehwBaseImpl::getLoggerModuleName()
{
    return "PureHwBase";
} // getLoggerModuleName()


/**
    Initializes the instance.
*/
void PureHwBase::PurehwBaseImpl::Initialize()
{
    getConsole().OIOLn("PureHwBase::Initialize() ...");
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
void PureHwBase::PurehwBaseImpl::Deinitialize()
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
TPureBool PureHwBase::PurehwBaseImpl::isInitialized() const
{
    return bInitialized;
} // isInitialized()



// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


/** 
    Sets members to default values.

    @param parent Public class instance owning this pimpl object.
*/
PureHwBase::PurehwBaseImpl::PurehwBaseImpl(PureHwBase* parent)
{
    bInitialized = false;
    if ( _parent = parent )
    {
        _parent->PreInitialize();
    }
} // PureHwBase()


PureHwBase::PurehwBaseImpl::PurehwBaseImpl(const PurehwBaseImpl&)
{

} 


PureHwBase::PurehwBaseImpl& PureHwBase::PurehwBaseImpl::operator=(const PurehwBaseImpl&)
{
    return *this;
}
                                             

PureHwBase::PurehwBaseImpl::~PurehwBaseImpl()
{
    Deinitialize();
    _parent = NULL;
} // ~PureHwBase




/*
   PureHwBase
   ###########################################################################
*/


// ############################### PUBLIC ################################


/**
    Initializes the instance.
*/
void PureHwBase::Initialize()
{
    pImpl->Initialize();
} // Initialize()


/**
    Deinitializes the instance.
*/
void PureHwBase::Deinitialize()
{
    pImpl->Deinitialize();
} // Deinitialize()


/**
    Returns whether the instance is successfully initialized.
*/
TPureBool PureHwBase::isInitialized() const
{
    return pImpl->isInitialized();
} // isInitialized()


// ############################## PROTECTED ##############################


/** 
    Sets members to default values.
*/
PureHwBase::PureHwBase()
{
    pImpl = new PureHwBase::PurehwBaseImpl(this);
} // PureHwBase()


PureHwBase::PureHwBase(const PureHwBase&)
{

} 


PureHwBase& PureHwBase::operator=(const PureHwBase&)
{
    return *this;
}
                                             

PureHwBase::~PureHwBase()
{
    delete pImpl;
    pImpl = NULL;
} // ~PureHwBase


/**
    Preinitializes members.
    Being called from ctor and Deinitialize().
*/
void PureHwBase::PreInitialize()
{

}      


// ############################### PRIVATE ###############################
