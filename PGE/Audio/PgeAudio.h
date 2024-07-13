#pragma once

/*
    ###################################################################################
    PgeAudio.h
    This file is part of PGE.
    External header.
    PR00F's Game Engine audio subsystem
    Made by PR00F88
    ###################################################################################
*/

#include <string>

#include "../../../Console/CConsole/src/CConsole.h"

#include "../PGEallHeaders.h"

// backends for SoLoud: https://sol.gfxile.net/soloud/backends.html
// in case of problem with MiniAudio, try PortAudio!
#define WITH_MINIAUDIO
// TODO: later fix the soloud include path, same as described in PGE.h !!!
#include "../Audio/soloud-RELEASE_20200207/include/soloud.h"
#include "../Audio/soloud-RELEASE_20200207/include/soloud_wav.h"

#include "../PURE/include/external/Math/PureVector.h"

namespace pge_audio
{

    class PgeAudio
    {
    public:

        static const char* getLoggerModuleName();

        // ---------------------------------------------------------------------------

        CConsole& getConsole() const;

        PgeAudio();

        PgeAudio(const PgeAudio&) = delete;
        PgeAudio& operator=(const PgeAudio&) = delete;
        PgeAudio(PgeAudio&&) = delete;
        PgeAudio&& operator=(PgeAudio&&) = delete;

        // might be "virtual"-ized as in PgeNetwork
        bool initialize();                   /**< Initialize the audio subsystem. */
        bool shutdown();                     /**< Stops the audio subsystem. */
        bool isInitialized() const;          /**< Gets the state of the audio subsystem. */

        SoLoud::Soloud& getAudioEngineCore();

        void loadSound(SoLoud::Wav& snd, const std::string& sFname);
        SoLoud::handle play3dSound(
            SoLoud::Wav& snd,
            const float& posX,
            const float& posY,
            const float& posZ);
        SoLoud::handle play3dSound(SoLoud::Wav& snd, const PureVector& pos);
        SoLoud::handle play3dSound(SoLoud::Wav& snd, const TXYZ& pos);

    protected:

    private:

        bool m_bInitialized = false;
        SoLoud::Soloud m_SoLoudCore;

    }; // class PgeAudio

} // namespace pge_audio
