/*
    ###################################################################################
    PRREhwInfo.cpp
    This file is part of PRRE.
    PRRE hardware information class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/


#include "PRREbaseIncludes.h"  // PCH
#include "../../include/external/Hardware/PRREhwInfo.h"
#include "../../include/internal/PRREpragmas.h"


using namespace std;


/*
   PRREhwInfoImpl
   ###########################################################################
*/

class PRREhwInfoImpl :
    public PRREhwInfo
{

public:
    static PRREhwInfoImpl& get();

    // ---------------------------------------------------------------------------

    void      Initialize(
        HGLRC rc, HDC wnd_dc,
        TPRREuint nResX, TPRREuint nResY,
        TPRREint nColorBits, TPRREint nDepthBits,
        TPRREint nStencilBits, TPRREint nFSAAlevel );
    void      Deinitialize();
    TPRREbool isInitialized() const;

    PRREhwCentralProcessor& getCentralProcessor() const;
    PRREhwSystemMemory&     getMemory() const;
    PRREhwVideo&            getVideo() const;
    PRREhwAudio&            getAudio() const;

    void WriteStats();

private:

    static PRREhwInfoImpl hwInfoInstance;
    static TPRREbool  bInitialized;       /**< Is the singleton instance initialized? */

    // ---------------------------------------------------------------------------

    PRREhwCentralProcessor& hwCentralProcessor;
    PRREhwSystemMemory&     hwMemory;
    PRREhwVideo&            hwVideo;
    PRREhwAudio&            hwAudio;

    // ---------------------------------------------------------------------------

    PRREhwInfoImpl();                  /**< Sets members to default values. */

    PRREhwInfoImpl(const PRREhwInfoImpl&);
    PRREhwInfoImpl& operator=(const PRREhwInfoImpl&);

    virtual ~PRREhwInfoImpl();

    /** Preinitializes members. */
    void PreInitialize();   

};


// ############################### PUBLIC ################################


/**
    Gets the singleton instance.
*/
PRREhwInfoImpl& PRREhwInfoImpl::get()
{
    return hwInfoInstance;
} // get()


/**
    Sets members to real values within the singleton instance.
    Initializes PRREhwCentralProcessor, PRREhwMemory, PRREhwVideo and PRREhwAudio instances.
*/
void PRREhwInfoImpl::Initialize(
    HGLRC rc, HDC wnd_dc,
    TPRREuint nResX, TPRREuint nResY,
    TPRREint nColorBits, TPRREint nDepthBits,
    TPRREint nStencilBits, TPRREint nFSAAlevel )
{

    getConsole().OIOLn("PRREhwInfo::Initialize() ...");
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
    Deinitializes PRREhwCentralProcessor, PRREhwMemory, PRREhwVideo and PRREhwAudio instances.
*/
void PRREhwInfoImpl::Deinitialize()
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
TPRREbool PRREhwInfoImpl::isInitialized() const
{
    return bInitialized;
} // isInitialized()


/**
    Gives access to PRREhwCentralProcessor instance.
*/
PRREhwCentralProcessor& PRREhwInfoImpl::getCentralProcessor() const
{
    return hwCentralProcessor;
} // getCentralProcessor()


/**
    Gives access to PRREhwSystemMemory instance.
*/
PRREhwSystemMemory& PRREhwInfoImpl::getMemory() const
{
    return hwMemory;
} // getMemory()


/**
    Gives access to PRREhwVideo instance.
*/
PRREhwVideo& PRREhwInfoImpl::getVideo() const
{
    return hwVideo;
} // getVideo()


/**
    Gives access to PRREhwAudio instance.
*/
PRREhwAudio& PRREhwInfoImpl::getAudio() const
{
    return hwAudio;
} // getAudio()


/**
    Writes statistics to the console.
*/
void PRREhwInfoImpl::WriteStats()
{
    getConsole().OLn("PRREhwInfo::WriteStats()");
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

    getConsole().OO();
    getConsole().OO();
} // WriteStats()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


PRREhwInfoImpl PRREhwInfoImpl::hwInfoInstance;
TPRREbool PRREhwInfoImpl::bInitialized = false;


/** 
    Sets members to default values.
*/
PRREhwInfoImpl::PRREhwInfoImpl() :
    hwCentralProcessor( PRREhwCentralProcessor::get() ),
    hwMemory( PRREhwSystemMemory::get() ),
    hwVideo( PRREhwVideo::get() ),
    hwAudio( PRREhwAudio::get() )
{
    PreInitialize();
} // PRREhwInfo()


PRREhwInfoImpl::PRREhwInfoImpl(const PRREhwInfoImpl&) :
    hwCentralProcessor( PRREhwCentralProcessor::get() ),
    hwMemory( PRREhwSystemMemory::get() ),
    hwVideo( PRREhwVideo::get() ),
    hwAudio( PRREhwAudio::get() )
{

}


PRREhwInfoImpl& PRREhwInfoImpl::operator=(const PRREhwInfoImpl&)
{
    return *this;
}


PRREhwInfoImpl::~PRREhwInfoImpl()
{
    Deinitialize();
} // ~PRREhwInfo


/**
    Preinitializes members.
*/
void PRREhwInfoImpl::PreInitialize()
{
    bInitialized = false;
}


/*
   PRREhwInfo
   ###########################################################################
*/


// ############################### PUBLIC ################################


/**
    Gets the singleton instance.
*/
PRREhwInfo& PRREhwInfo::get()
{
    return PRREhwInfoImpl::get();
} // get()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


