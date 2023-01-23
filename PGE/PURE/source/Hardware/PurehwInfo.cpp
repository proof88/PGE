/*
    ###################################################################################
    PureHwInfo.cpp
    This file is part of Pure.
    Pure hardware information class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/


#include "PurebaseIncludes.h"  // PCH
#include "../../include/external/Hardware/PureHwInfo.h"
#include "../../include/internal/Purepragmas.h"


using namespace std;


/*
   PurehwInfoImpl
   ###########################################################################
*/

class PurehwInfoImpl :
    public PureHwInfo
{

public:
    static PurehwInfoImpl& get();

    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    CConsole&  getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */

    void      Initialize(
        HGLRC rc, HDC wnd_dc,
        TPureUInt nResX, TPureUInt nResY,
        TPureInt nColorBits, TPureInt nDepthBits,
        TPureInt nStencilBits, TPureInt nFSAAlevel );
    void      Deinitialize();
    TPureBool isInitialized() const;

    PurehwCentralProcessor& getCentralProcessor() const;
    PurehwSystemMemory&     getMemory() const;
    PureHwVideo&            getVideo() const;
    PurehwAudio&            getAudio() const;

    void WriteStats();

private:

    static PurehwInfoImpl hwInfoInstance;
    static TPureBool  bInitialized;       /**< Is the singleton instance initialized? */

    // ---------------------------------------------------------------------------

    PurehwCentralProcessor& hwCentralProcessor;
    PurehwSystemMemory&     hwMemory;
    PureHwVideo&            hwVideo;
    PurehwAudio&            hwAudio;

    // ---------------------------------------------------------------------------

    PurehwInfoImpl();                  /**< Sets members to default values. */

    PurehwInfoImpl(const PurehwInfoImpl&);
    PurehwInfoImpl& operator=(const PurehwInfoImpl&);

    virtual ~PurehwInfoImpl();

    /** Preinitializes members. */
    void PreInitialize();   

};


// ############################### PUBLIC ################################


/**
    Gets the singleton instance.
*/
PurehwInfoImpl& PurehwInfoImpl::get()
{
    return hwInfoInstance;
} // get()


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& PurehwInfoImpl::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* PurehwInfoImpl::getLoggerModuleName()
{
    return "PureHwInfo";
} // getLoggerModuleName()


/**
    Sets members to real values within the singleton instance.
    Initializes PurehwCentralProcessor, PurehwMemory, PureHwVideo and PurehwAudio instances.
*/
void PurehwInfoImpl::Initialize(
    HGLRC rc, HDC wnd_dc,
    TPureUInt nResX, TPureUInt nResY,
    TPureInt nColorBits, TPureInt nDepthBits,
    TPureInt nStencilBits, TPureInt nFSAAlevel )
{

    getConsole().OIOLn("PureHwInfo::Initialize() ...");
    getConsole().OI();
    if ( bInitialized )
    {
        getConsole().OLnOO("Already initialized!");     
        getConsole().OO();
        return;
    }

    if ( rc == NULL )
    {
        getConsole().OLn("Warning: rc is NULL!");
    }
    
    PreInitialize();
    hwCentralProcessor.Initialize();
    hwMemory.Initialize();
    hwVideo.Initialize(rc, wnd_dc, nResX, nResY, nColorBits, nDepthBits, nStencilBits, nFSAAlevel);
    hwAudio.Initialize();
    bInitialized = hwCentralProcessor.isInitialized() &&
        hwMemory.isInitialized() &&
        hwVideo.isInitialized() &&
        hwAudio.isInitialized();

    if ( bInitialized )
    {
        getConsole().OOSOLn("done!");
        getConsole().OO();
    }
    else
    {
        getConsole().OOEOLn("Failed to initialize 1 or more hardware class(es)!");
        getConsole().OO();
        WriteStats();
        return;
    }

} // Initialize(...)


/**
    Deinitializes the singleton instance.
    Deinitializes PurehwCentralProcessor, PurehwMemory, PureHwVideo and PurehwAudio instances.
*/
void PurehwInfoImpl::Deinitialize()
{
    if ( !bInitialized )
        return;

    // release resources right before PreInitialize()
    hwCentralProcessor.Deinitialize();
    hwMemory.Deinitialize();
    hwVideo.Deinitialize();
    hwAudio.Deinitialize();
    PreInitialize();
} // Deinitialize()


/**
    Returns whether the singleton instance is successfully initialized.
*/
TPureBool PurehwInfoImpl::isInitialized() const
{
    return bInitialized;
} // isInitialized()


/**
    Gives access to PurehwCentralProcessor instance.
*/
PurehwCentralProcessor& PurehwInfoImpl::getCentralProcessor() const
{
    return hwCentralProcessor;
} // getCentralProcessor()


/**
    Gives access to PurehwSystemMemory instance.
*/
PurehwSystemMemory& PurehwInfoImpl::getMemory() const
{
    return hwMemory;
} // getMemory()


/**
    Gives access to PureHwVideo instance.
*/
PureHwVideo& PurehwInfoImpl::getVideo() const
{
    return hwVideo;
} // getVideo()


/**
    Gives access to PurehwAudio instance.
*/
PurehwAudio& PurehwInfoImpl::getAudio() const
{
    return hwAudio;
} // getAudio()


/**
    Writes statistics to the console.
*/
void PurehwInfoImpl::WriteStats()
{
    getConsole().OLn("PureHwInfo::WriteStats()");
    getConsole().L();

    getConsole().OI();

    if ( isInitialized() )
        getConsole().SOn();
    else getConsole().EOn();
    getConsole().OLnOI("Overall initialized state: %b", isInitialized());

    if ( hwCentralProcessor.isInitialized() )
        getConsole().SOn();
    else getConsole().EOn();
    getConsole().OLn("Central Processor inited: %b", hwCentralProcessor.isInitialized());

    if ( hwMemory.isInitialized() )
        getConsole().SOn();
    else getConsole().EOn();
    getConsole().OLn("System Memory inited: %b", hwMemory.isInitialized());

    if ( hwVideo.isInitialized() )
        getConsole().SOn();
    else getConsole().EOn();
    getConsole().OLn("Video inited: %b", hwVideo.isInitialized());

    if ( hwAudio.isInitialized() )
        getConsole().SOn();
    else getConsole().EOn();
    getConsole().OLn("Audio inited: %b", hwAudio.isInitialized());

    getConsole().NOn();

    getConsole().OO();
    getConsole().OLnOO("");
} // WriteStats()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


PurehwInfoImpl PurehwInfoImpl::hwInfoInstance;
TPureBool PurehwInfoImpl::bInitialized = false;


/** 
    Sets members to default values.
*/
PurehwInfoImpl::PurehwInfoImpl() :
    hwCentralProcessor( PurehwCentralProcessor::get() ),
    hwMemory( PurehwSystemMemory::get() ),
    hwVideo( PureHwVideo::get() ),
    hwAudio( PurehwAudio::get() )
{
    PreInitialize();
} // PureHwInfo()


PurehwInfoImpl::PurehwInfoImpl(const PurehwInfoImpl&) :
    hwCentralProcessor( PurehwCentralProcessor::get() ),
    hwMemory( PurehwSystemMemory::get() ),
    hwVideo( PureHwVideo::get() ),
    hwAudio( PurehwAudio::get() )
{

}


PurehwInfoImpl& PurehwInfoImpl::operator=(const PurehwInfoImpl&)
{
    return *this;
}


PurehwInfoImpl::~PurehwInfoImpl()
{
    Deinitialize();
} // ~PureHwInfo


/**
    Preinitializes members.
*/
void PurehwInfoImpl::PreInitialize()
{
    bInitialized = false;
}


/*
   PureHwInfo
   ###########################################################################
*/


// ############################### PUBLIC ################################


/**
    Gets the singleton instance.
*/
PureHwInfo& PureHwInfo::get()
{
    return PurehwInfoImpl::get();
} // get()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


