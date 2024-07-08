/*
    ###################################################################################
    PgeAudio.cpp
    This file is part of PGE.
    PR00F's Game Engine audio subsystem
    Made by PR00F88
    ###################################################################################
*/

#include "PureBaseIncludes.h"  // PCH

#include "PgeAudio.h"


// ############################### PUBLIC ################################


const char* pge_audio::PgeAudio::getLoggerModuleName()
{
    return "PgeAudio";
}

CConsole& pge_audio::PgeAudio::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
}

pge_audio::PgeAudio::PgeAudio()
{
}


/**
    Initialize the audio subsystem.

    @return The result of the initialization. True on success, false otherwise.
*/
bool pge_audio::PgeAudio::initialize()
{
    // could use getConsole() now but since this code is copy-pasted and might change the class in future, now getConsoleInstance() stays

    if (isInitialized())
    {
        CConsole::getConsoleInstance("PgeAudio").SOLn("Already initialized SoLoud version %d!", SOLOUD_VERSION);
        return true;
    }

    const SoLoud::result res = m_SoLoudCore.init(SoLoud::Soloud::CLIP_ROUNDOFF, SoLoud::Soloud::MINIAUDIO);
    m_bInitialized = (SoLoud::SOLOUD_ERRORS::SO_NO_ERROR == res);
    if (m_bInitialized)
    {
        CConsole::getConsoleInstance("PgeAudio").SOLn("Initialized SoLoud version %d!", SOLOUD_VERSION);
        CConsole::getConsoleInstance("PgeAudio").OLn(
            "Backend ID: %d, Name: %s",
            m_SoLoudCore.getBackendId(),
            m_SoLoudCore.getBackendString());
        CConsole::getConsoleInstance("PgeAudio").OLn(
            "Channels: %d, Sample Rate: %d Hz, Buffer Size: %d",
            m_SoLoudCore.getBackendChannels(),
            m_SoLoudCore.getBackendSamplerate(),
            m_SoLoudCore.getBackendBufferSize());
    }
    else
    {
        CConsole::getConsoleInstance("PgeAudio").EOLn("Failed to initialize SoLoud version %d, code: %d!", SOLOUD_VERSION, res);
    }

    return m_bInitialized;
}

/**
    Stops the audio subsystem.
    No effect before initialization.
    After shutdown, initialize() can be called again.

    @return True on successful shutdown, false otherwise.
*/
bool pge_audio::PgeAudio::shutdown()
{
    if (!isInitialized())
    {
        return true;
    }

    m_SoLoudCore.deinit();
    m_bInitialized = false;

    return true;
}

/**
    Gets the state of the audio subsystem.
    @return True if the audio subsystem is successfully initialized, false before initialization or after shutdown.
*/
bool pge_audio::PgeAudio::isInitialized() const
{
    return m_bInitialized;
}

SoLoud::Soloud& pge_audio::PgeAudio::getAudioEngineCore()
{
    return m_SoLoudCore;
}

void pge_audio::PgeAudio::loadSound(SoLoud::Wav& snd, const std::string& sFname)
{
    if (!isInitialized())
    {
        getConsole().EOLn("%s: Audio subsystem is NOT initialized!", __func__);
        return;
    }

    const SoLoud::result resSoloud = snd.load(sFname.c_str());
    if (resSoloud == SoLoud::SOLOUD_ERRORS::SO_NO_ERROR)
    {
        getConsole().OLn("%s: %s loaded, length: %f secs!", __func__, sFname.c_str(), static_cast<float>(snd.getLength()));
    }
    else
    {
        getConsole().EOLn("%s: %s load error: %d!", __func__, sFname.c_str(), resSoloud);
    }
}


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################

