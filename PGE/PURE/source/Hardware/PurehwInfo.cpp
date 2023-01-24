/*
    ###################################################################################
    PureHwInfo.cpp
    This file is part of PURE.
    Pure hardware information class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/


#include "PureBaseIncludes.h"  // PCH
#include "../../include/external/Hardware/PureHwInfo.h"
#include "../../include/internal/PurePragmas.h"


using namespace std;


/*
   PureHwInfoImpl
   ###########################################################################
*/

class PureHwInfoImpl :
    public PureHwInfo
{

public:
    static PureHwInfoImpl& get();

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

    PureHwCentralProcessor& getCentralProcessor() const;
    PureHwSystemMemory&     getMemory() const;
    PureHwVideo&            getVideo() const;
    PureHwAudio&            getAudio() const;

    void WriteStats();

private:

    static PureHwInfoImpl hwInfoInstance;
    static TPureBool  bInitialized;       /**< Is the singleton instance initialized? */

    // ---------------------------------------------------------------------------

    PureHwCentralProcessor& hwCentralProcessor;
    PureHwSystemMemory&     hwMemory;
    PureHwVideo&            hwVideo;
    PureHwAudio&            hwAudio;

    // ---------------------------------------------------------------------------

    PureHwInfoImpl();                  /**< Sets members to default values. */

    PureHwInfoImpl(const PureHwInfoImpl&);
    PureHwInfoImpl& operator=(const PureHwInfoImpl&);

    virtual ~PureHwInfoImpl();

    /** Preinitializes members. */
    void PreInitialize();   

};


// ############################### PUBLIC ################################


/**
    Gets the singleton instance.
*/
PureHwInfoImpl& PureHwInfoImpl::get()
{
    return hwInfoInstance;
} // get()


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& PureHwInfoImpl::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* PureHwInfoImpl::getLoggerModuleName()
{
    return "PureHwInfo";
} // getLoggerModuleName()


/**
    Sets members to real values within the singleton instance.
    Initializes PureHwCentralProcessor, PurehwMemory, PureHwVideo and PureHwAudio instances.
*/
void PureHwInfoImpl::Initialize(
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
    Deinitializes PureHwCentralProcessor, PurehwMemory, PureHwVideo and PureHwAudio instances.
*/
void PureHwInfoImpl::Deinitialize()
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
TPureBool PureHwInfoImpl::isInitialized() const
{
    return bInitialized;
} // isInitialized()


/**
    Gives access to PureHwCentralProcessor instance.
*/
PureHwCentralProcessor& PureHwInfoImpl::getCentralProcessor() const
{
    return hwCentralProcessor;
} // getCentralProcessor()


/**
    Gives access to PureHwSystemMemory instance.
*/
PureHwSystemMemory& PureHwInfoImpl::getMemory() const
{
    return hwMemory;
} // getMemory()


/**
    Gives access to PureHwVideo instance.
*/
PureHwVideo& PureHwInfoImpl::getVideo() const
{
    return hwVideo;
} // getVideo()


/**
    Gives access to PureHwAudio instance.
*/
PureHwAudio& PureHwInfoImpl::getAudio() const
{
    return hwAudio;
} // getAudio()


/**
    Writes statistics to the console.
*/
void PureHwInfoImpl::WriteStats()
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


PureHwInfoImpl PureHwInfoImpl::hwInfoInstance;
TPureBool PureHwInfoImpl::bInitialized = false;


/** 
    Sets members to default values.
*/
PureHwInfoImpl::PureHwInfoImpl() :
    hwCentralProcessor( PureHwCentralProcessor::get() ),
    hwMemory( PureHwSystemMemory::get() ),
    hwVideo( PureHwVideo::get() ),
    hwAudio( PureHwAudio::get() )
{
    PreInitialize();
} // PureHwInfo()


PureHwInfoImpl::PureHwInfoImpl(const PureHwInfoImpl&) :
    hwCentralProcessor( PureHwCentralProcessor::get() ),
    hwMemory( PureHwSystemMemory::get() ),
    hwVideo( PureHwVideo::get() ),
    hwAudio( PureHwAudio::get() )
{

}


PureHwInfoImpl& PureHwInfoImpl::operator=(const PureHwInfoImpl&)
{
    return *this;
}


PureHwInfoImpl::~PureHwInfoImpl()
{
    Deinitialize();
} // ~PureHwInfo


/**
    Preinitializes members.
*/
void PureHwInfoImpl::PreInitialize()
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
    return PureHwInfoImpl::get();
} // get()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


