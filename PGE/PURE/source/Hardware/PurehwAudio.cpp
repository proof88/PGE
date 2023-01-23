/*
    ###################################################################################
    PurehwAudio.cpp
    This file is part of Pure.
    Pure audio capabilities class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/


#include "PurebaseIncludes.h"  // PCH
#include "../../include/external/Hardware/PurehwAudio.h"
#include "../../include/internal/Purepragmas.h"


using namespace std;


/*
   PurehwAudioImpl
   ###########################################################################
*/

class PurehwAudioImpl :
    public PurehwAudio
{

public:

    static PurehwAudioImpl& get();

    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    CConsole&  getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */

    void WriteStats();

protected:

private:
    static PurehwAudioImpl hwAudioInstance;

    // ---------------------------------------------------------------------------

    PurehwAudioImpl();                /**< Sets members to default values. */

    PurehwAudioImpl(const PurehwAudioImpl&);
    PurehwAudioImpl& operator=(const PurehwAudioImpl&);

    virtual ~PurehwAudioImpl();

    void      PreInitialize();     /**< Preinitializes members. */
    TPureBool initializeBase();    /**< Does the real initializations. */
    void      DeinitializeBase();  /**< Does the real deinitialization. */

};


// ############################### PUBLIC ################################


/** 
    Gets the singleton instance.
*/
PurehwAudioImpl& PurehwAudioImpl::get()
{
    return hwAudioInstance;
} // get()


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& PurehwAudioImpl::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* PurehwAudioImpl::getLoggerModuleName()
{
    return "PurehwAudio";
} // getLoggerModuleName()


/**
    Writes statistics to the console.
*/
void PurehwAudioImpl::WriteStats()
{
    getConsole().OLn("PurehwAudio::WriteStats()");
    getConsole().L();
} // WriteStats()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


PurehwAudioImpl PurehwAudioImpl::hwAudioInstance;


/** 
    Sets members to default values.
*/
PurehwAudioImpl::PurehwAudioImpl()
{
    PreInitialize();
} // PureHwInfo()


PurehwAudioImpl::PurehwAudioImpl(const PurehwAudioImpl&)
{

}


PurehwAudioImpl& PurehwAudioImpl::operator=(const PurehwAudioImpl&)
{
    return *this;
}
                                             

PurehwAudioImpl::~PurehwAudioImpl()
{
    Deinitialize();
} // ~PurehwAudio


/**
    Preinitializes members.
*/
void PurehwAudioImpl::PreInitialize()
{

} // PreInitialize()


/**
    Does the real initialization.
*/
TPureBool PurehwAudioImpl::initializeBase()
{
    return true;
} // initializeBase()


/**
    Does the real deinitialization.
*/
void PurehwAudioImpl::DeinitializeBase()
{

} // deinitializeBase()




/*
   PurehwAudio
   ###########################################################################
*/


// ############################### PUBLIC ################################


/** 
    Gets the singleton instance.
*/
PurehwAudio& PurehwAudio::get()
{
    return PurehwAudioImpl::get();
} // get()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################