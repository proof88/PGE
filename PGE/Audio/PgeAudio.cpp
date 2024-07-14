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

pge_audio::PgeAudio::PgeAudio(PGEcfgProfiles& cfgProfiles) :
    m_cfgProfiles(cfgProfiles)
{
}


/**
    Initialize the audio subsystem.

    @return The result of the initialization. True on success, false otherwise.
*/
bool pge_audio::PgeAudio::initialize()
{
    // could use getConsole() now but since this code is copy-pasted and might change the class in future, now getConsoleInstance() stays

    if (m_cfgProfiles.getVars()[CVAR_SFX_ENABLED].getAsString().empty())
    {
        m_cfgProfiles.getVars()[CVAR_SFX_ENABLED].Set(true);
        getConsole().EOLn("PgeAudio::%s(): Missing audio in config, defaulting to: %b!", __func__, m_cfgProfiles.getVars()[CVAR_SFX_ENABLED].getAsBool());
    }
    
    if (!m_cfgProfiles.getVars()[CVAR_SFX_ENABLED].getAsBool())
    {
        // I'm aware about SoLoud's NOSOUND and NULLDRIVER backends, but as I understand, the former actually plays sounds without actual
        // hearable result, and about the latter I'm not sure, but I want to actually CUT communication with SoLoud if audio is
        // disabled in config.
        // So I'm maintaining this PgeAudio as wrapper layer over SoLoud, this way is the best to actually
        // test if there is any performance drop with enabled audio in certain situations or not.
        // Also, having a wrapper is good just in case I want to replace SoLoud in the future with some other audio lib, or just
        // offer selection to the user.
        // Anyway, both NOSOUND and NULLDRIVER can be easily tested anytime by just replacing the backendId below for init().
        getConsole().EOLn("PgeAudio::%s(): Audio disabled by config!", __func__);
        return true;
    }
    
    if (isInitialized())
    {
        getConsole().SOLn("PgeAudio::%s(): Already initialized SoLoud version %d!", __func__, SOLOUD_VERSION);
        return true;
    }

    /* Since PURE uses left-handed Cartesian coordinate system, SoLoud shall also use the same. */
    const SoLoud::result res = m_SoLoudCore.init(SoLoud::Soloud::CLIP_ROUNDOFF | SoLoud::Soloud::LEFT_HANDED_3D, SoLoud::Soloud::MINIAUDIO);
    m_bInitialized = (SoLoud::SOLOUD_ERRORS::SO_NO_ERROR == res);
    if (m_bInitialized)
    {
        getConsole().SOLn("PgeAudio::%s(): Initialized SoLoud version %d!", __func__, SOLOUD_VERSION);
        getConsole().OLn(
            "Backend ID: %d, Name: %s",
            m_SoLoudCore.getBackendId(),
            m_SoLoudCore.getBackendString());
        getConsole().OLn(
            "Channels: %d, Sample Rate: %d Hz, Buffer Size: %d frames (dont know how many Bytes per frame)",
            m_SoLoudCore.getBackendChannels(),
            m_SoLoudCore.getBackendSamplerate(),
            m_SoLoudCore.getBackendBufferSize());
    }
    else
    {
        getConsole().EOLn("PgeAudio::%s():Failed to initialize SoLoud version %d, code: %d!", __func__, SOLOUD_VERSION, res);
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
        getConsole().SOLn("PgeAudio::%s(): was already shut down!", __func__);
        return true;
    }

    m_SoLoudCore.deinit();
    m_bInitialized = false;

    getConsole().SOLn("PgeAudio::%s(): done!", __func__);

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
        getConsole().EOLn("%s: Audio subsystem is NOT initialized (config-state: %b)!", __func__, m_cfgProfiles.getVars()[CVAR_SFX_ENABLED].getAsBool());
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

SoLoud::handle pge_audio::PgeAudio::playSound(SoLoud::Wav& snd)
{
    // It is very important to have play wrappers, because as soon as you invoke play() on an AudioSource such as Wav,
    // the AudioSource's mSoloud member will be set, which will trigger stop() in the dtor of Wav and/or AudioSource,
    // that could cause us some trouble upon deleting them, latest when we exit from the application.
    // However, stop() does nothing if mSoloud is not set, so we are indirectly avoid setting it by avoiding play() here
    // if audio is not initialized, like in the case of it is disabled in config.
    // It is safe to call property settings though on AudioSource instances even if we are not initialized, that is why
    // currently I dont have wrappers for them now.

    if (!isInitialized())
    {
        // dont spam log with error message, since loadSound() already logs error
        return SoLoud::handle{};
    }

    return m_SoLoudCore.play(snd);
}

SoLoud::handle pge_audio::PgeAudio::play3dSound(SoLoud::Wav& snd, const float& posX, const float& posY, const float& posZ)
{
    // see comment in playSound() to understand why it is always required to use wrapper for play functions!

    if (!isInitialized())
    {
        // dont spam log with error message, since loadSound() already logs error
        return SoLoud::handle{};
    }

    // https://solhsa.com/soloud/core3d.html
    // https://solhsa.com/soloud/concepts3d.html

    const auto sndHandle = m_SoLoudCore.play3d(
        snd,
        posX, posY, posZ,
        0.f, 0.f, 0.f, /* velocity xyz */
        0.f /* volume */);

    /* hack: set initial volume 0 and then set it back to 1, as WA for issue: https://github.com/jarikomppa/soloud/issues/175 */
    m_SoLoudCore.setVolume(sndHandle, 1.f);

    return sndHandle;
}

SoLoud::handle pge_audio::PgeAudio::play3dSound(SoLoud::Wav& snd, const PureVector& pos)
{
    return play3dSound(snd, pos.getX(), pos.getY(), pos.getZ());
}

SoLoud::handle pge_audio::PgeAudio::play3dSound(SoLoud::Wav& snd, const TXYZ& pos)
{
    return play3dSound(snd, pos.x, pos.y, pos.z);
}

void pge_audio::PgeAudio::stopSoundInstance(const SoLoud::handle& sndHandle)
{
    if (!isInitialized())
    {
        // dont spam log with error message, since loadSound() already logs error
        return;
    }

    m_SoLoudCore.stop(sndHandle);
}


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################

