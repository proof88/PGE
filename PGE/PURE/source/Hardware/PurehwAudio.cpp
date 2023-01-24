/*
    ###################################################################################
    PureHwAudio.cpp
    This file is part of PURE.
    Pure audio capabilities class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/


#include "PureBaseIncludes.h"  // PCH
#include "../../include/external/Hardware/PureHwAudio.h"
#include "../../include/internal/PurePragmas.h"


using namespace std;


/*
   PureHwAudioImpl
   ###########################################################################
*/

class PureHwAudioImpl :
    public PureHwAudio
{

public:

    static PureHwAudioImpl& get();

    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    CConsole&  getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */

    void WriteStats();

protected:

private:
    static PureHwAudioImpl hwAudioInstance;

    // ---------------------------------------------------------------------------

    PureHwAudioImpl();                /**< Sets members to default values. */

    PureHwAudioImpl(const PureHwAudioImpl&);
    PureHwAudioImpl& operator=(const PureHwAudioImpl&);

    virtual ~PureHwAudioImpl();

    void      PreInitialize();     /**< Preinitializes members. */
    TPureBool initializeBase();    /**< Does the real initializations. */
    void      DeinitializeBase();  /**< Does the real deinitialization. */

};


// ############################### PUBLIC ################################


/** 
    Gets the singleton instance.
*/
PureHwAudioImpl& PureHwAudioImpl::get()
{
    return hwAudioInstance;
} // get()


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& PureHwAudioImpl::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* PureHwAudioImpl::getLoggerModuleName()
{
    return "PureHwAudio";
} // getLoggerModuleName()


/**
    Writes statistics to the console.
*/
void PureHwAudioImpl::WriteStats()
{
    getConsole().OLn("PureHwAudio::WriteStats()");
    getConsole().L();
} // WriteStats()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


PureHwAudioImpl PureHwAudioImpl::hwAudioInstance;


/** 
    Sets members to default values.
*/
PureHwAudioImpl::PureHwAudioImpl()
{
    PreInitialize();
} // PureHwInfo()


PureHwAudioImpl::PureHwAudioImpl(const PureHwAudioImpl&)
{

}


PureHwAudioImpl& PureHwAudioImpl::operator=(const PureHwAudioImpl&)
{
    return *this;
}
                                             

PureHwAudioImpl::~PureHwAudioImpl()
{
    Deinitialize();
} // ~PureHwAudio


/**
    Preinitializes members.
*/
void PureHwAudioImpl::PreInitialize()
{

} // PreInitialize()


/**
    Does the real initialization.
*/
TPureBool PureHwAudioImpl::initializeBase()
{
    return true;
} // initializeBase()


/**
    Does the real deinitialization.
*/
void PureHwAudioImpl::DeinitializeBase()
{

} // deinitializeBase()




/*
   PureHwAudio
   ###########################################################################
*/


// ############################### PUBLIC ################################


/** 
    Gets the singleton instance.
*/
PureHwAudio& PureHwAudio::get()
{
    return PureHwAudioImpl::get();
} // get()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################