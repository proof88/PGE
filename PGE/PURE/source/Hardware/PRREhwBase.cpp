/*
    ###################################################################################
    PurehwBase.cpp
    This file is part of Pure.
    Base class for Pure hardware classes.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PurebaseIncludes.h"  // PCH
#include "../../include/external/Hardware/PurehwBase.h"
#include "../../include/internal/Purepragmas.h"


using namespace std;


/*
   PurehwBase::PurehwBaseImpl
   ###########################################################################
*/

class PurehwBase::PurehwBaseImpl
{

public:

    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    CConsole&  getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */

    virtual ~PurehwBaseImpl();

    void      Initialize(); 
    void      Deinitialize();
    TPurebool isInitialized() const;

protected:

    // ---------------------------------------------------------------------------

private:
    TPurebool bInitialized;       /**< Is the instance initialized? False by default.
                                       Will be true if initializeBase() of descendant class succeeds.
                                       Will be false again if Deinitialize() gets called. */

    PurehwBase* _parent;

    // ---------------------------------------------------------------------------

    explicit PurehwBaseImpl(PurehwBase* parent);
    PurehwBaseImpl(const PurehwBaseImpl&);
    PurehwBaseImpl& operator=(const PurehwBaseImpl&);

    friend class PurehwBase;

};


// ############################### PUBLIC ################################


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& PurehwBase::PurehwBaseImpl::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* PurehwBase::PurehwBaseImpl::getLoggerModuleName()
{
    return "PurehwBase";
} // getLoggerModuleName()


/**
    Initializes the instance.
*/
void PurehwBase::PurehwBaseImpl::Initialize()
{
    getConsole().OIOLn("PurehwBase::Initialize() ...");
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
void PurehwBase::PurehwBaseImpl::Deinitialize()
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
TPurebool PurehwBase::PurehwBaseImpl::isInitialized() const
{
    return bInitialized;
} // isInitialized()



// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


/** 
    Sets members to default values.

    @param parent Public class instance owning this pimpl object.
*/
PurehwBase::PurehwBaseImpl::PurehwBaseImpl(PurehwBase* parent)
{
    bInitialized = false;
    if ( _parent = parent )
    {
        _parent->PreInitialize();
    }
} // PurehwBase()


PurehwBase::PurehwBaseImpl::PurehwBaseImpl(const PurehwBaseImpl&)
{

} 


PurehwBase::PurehwBaseImpl& PurehwBase::PurehwBaseImpl::operator=(const PurehwBaseImpl&)
{
    return *this;
}
                                             

PurehwBase::PurehwBaseImpl::~PurehwBaseImpl()
{
    Deinitialize();
    _parent = NULL;
} // ~PurehwBase




/*
   PurehwBase
   ###########################################################################
*/


// ############################### PUBLIC ################################


/**
    Initializes the instance.
*/
void PurehwBase::Initialize()
{
    pImpl->Initialize();
} // Initialize()


/**
    Deinitializes the instance.
*/
void PurehwBase::Deinitialize()
{
    pImpl->Deinitialize();
} // Deinitialize()


/**
    Returns whether the instance is successfully initialized.
*/
TPurebool PurehwBase::isInitialized() const
{
    return pImpl->isInitialized();
} // isInitialized()


// ############################## PROTECTED ##############################


/** 
    Sets members to default values.
*/
PurehwBase::PurehwBase()
{
    pImpl = new PurehwBase::PurehwBaseImpl(this);
} // PurehwBase()


PurehwBase::PurehwBase(const PurehwBase&)
{

} 


PurehwBase& PurehwBase::operator=(const PurehwBase&)
{
    return *this;
}
                                             

PurehwBase::~PurehwBase()
{
    delete pImpl;
    pImpl = NULL;
} // ~PurehwBase


/**
    Preinitializes members.
    Being called from ctor and Deinitialize().
*/
void PurehwBase::PreInitialize()
{

}      


// ############################### PRIVATE ###############################
