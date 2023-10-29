set(PROJECT_NAME SoloudStatic)

################################################################################
# Source groups
################################################################################
set(Sources
    "../../src/audiosource/monotone/soloud_monotone.cpp"
    "../../src/audiosource/noise/soloud_noise.cpp"
    "../../src/audiosource/openmpt/soloud_openmpt.cpp"
    "../../src/audiosource/openmpt/soloud_openmpt_dll.c"
    "../../src/audiosource/sfxr/soloud_sfxr.cpp"
    "../../src/audiosource/speech/darray.cpp"
    "../../src/audiosource/speech/klatt.cpp"
    "../../src/audiosource/speech/resonator.cpp"
    "../../src/audiosource/speech/soloud_speech.cpp"
    "../../src/audiosource/speech/tts.cpp"
    "../../src/audiosource/tedsid/sid.cpp"
    "../../src/audiosource/tedsid/soloud_tedsid.cpp"
    "../../src/audiosource/tedsid/ted.cpp"
    "../../src/audiosource/vic/soloud_vic.cpp"
    "../../src/audiosource/vizsn/soloud_vizsn.cpp"
    "../../src/audiosource/wav/dr_impl.cpp"
    "../../src/audiosource/wav/soloud_wav.cpp"
    "../../src/audiosource/wav/soloud_wavstream.cpp"
    "../../src/audiosource/wav/stb_vorbis.c"
    "../../src/backend/miniaudio/soloud_miniaudio.cpp"
    "../../src/backend/nosound/soloud_nosound.cpp"
    "../../src/backend/null/soloud_null.cpp"
    "../../src/backend/openal/soloud_openal.cpp"
    "../../src/backend/openal/soloud_openal_dll.c"
    "../../src/backend/portaudio/soloud_portaudio.cpp"
    "../../src/backend/portaudio/soloud_portaudio_dll.c"
    "../../src/backend/sdl/soloud_sdl1.cpp"
    "../../src/backend/sdl/soloud_sdl1_dll.c"
    "../../src/backend/sdl/soloud_sdl2.cpp"
    "../../src/backend/sdl/soloud_sdl2_dll.c"
    "../../src/backend/wasapi/soloud_wasapi.cpp"
    "../../src/backend/winmm/soloud_winmm.cpp"
    "../../src/core/soloud.cpp"
    "../../src/core/soloud_audiosource.cpp"
    "../../src/core/soloud_bus.cpp"
    "../../src/core/soloud_core_3d.cpp"
    "../../src/core/soloud_core_basicops.cpp"
    "../../src/core/soloud_core_faderops.cpp"
    "../../src/core/soloud_core_filterops.cpp"
    "../../src/core/soloud_core_getters.cpp"
    "../../src/core/soloud_core_setters.cpp"
    "../../src/core/soloud_core_voicegroup.cpp"
    "../../src/core/soloud_core_voiceops.cpp"
    "../../src/core/soloud_fader.cpp"
    "../../src/core/soloud_fft.cpp"
    "../../src/core/soloud_fft_lut.cpp"
    "../../src/core/soloud_file.cpp"
    "../../src/core/soloud_filter.cpp"
    "../../src/core/soloud_misc.cpp"
    "../../src/core/soloud_queue.cpp"
    "../../src/core/soloud_thread.cpp"
    "../../src/filter/soloud_bassboostfilter.cpp"
    "../../src/filter/soloud_biquadresonantfilter.cpp"
    "../../src/filter/soloud_dcremovalfilter.cpp"
    "../../src/filter/soloud_echofilter.cpp"
    "../../src/filter/soloud_fftfilter.cpp"
    "../../src/filter/soloud_flangerfilter.cpp"
    "../../src/filter/soloud_freeverbfilter.cpp"
    "../../src/filter/soloud_lofifilter.cpp"
    "../../src/filter/soloud_robotizefilter.cpp"
    "../../src/filter/soloud_waveshaperfilter.cpp"
)
source_group("Sources" FILES ${Sources})

set(ALL_FILES
    ${Sources}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} STATIC ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE SoloudStatic)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
################################################################################
# Target name
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    TARGET_NAME_DEBUG               "soloud_static_x86_d"
    TARGET_NAME_DEBUGPR00FPS        "soloud_static_x86_d"
    TARGET_NAME_DEBUGTEST_PROOFPSDD "soloud_static_x86_d"
    TARGET_NAME_EV2008P03DEBUG      "soloud_static_x86_d"
    TARGET_NAME_EV2008P03RELEASE    "soloud_static_x86"
    TARGET_NAME_RELEASE             "soloud_static_x86"
    TARGET_NAME_RELEASEPR00FPS      "soloud_static_x86"
)
################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_DIRECTORY_DEBUG               "${CMAKE_CURRENT_SOURCE_DIR}/../../lib"
    OUTPUT_DIRECTORY_DEBUGPR00FPS        "${CMAKE_CURRENT_SOURCE_DIR}/../../lib"
    OUTPUT_DIRECTORY_DEBUGTEST_PROOFPSDD "${CMAKE_CURRENT_SOURCE_DIR}/../../lib"
    OUTPUT_DIRECTORY_EV2008P03DEBUG      "${CMAKE_CURRENT_SOURCE_DIR}/../../lib"
    OUTPUT_DIRECTORY_EV2008P03RELEASE    "${CMAKE_CURRENT_SOURCE_DIR}/../../lib"
    OUTPUT_DIRECTORY_RELEASE             "${CMAKE_CURRENT_SOURCE_DIR}/../../lib"
    OUTPUT_DIRECTORY_RELEASEPR00FPS      "${CMAKE_CURRENT_SOURCE_DIR}/../../lib"
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    PDB_OUTPUT_DIRECTORY_DEBUG               "${OUTPUT_DIRECTORY}"
    PDB_OUTPUT_DIRECTORY_DEBUGPR00FPS        "${OUTPUT_DIRECTORY}"
    PDB_OUTPUT_DIRECTORY_DEBUGTEST_PROOFPSDD "${OUTPUT_DIRECTORY}"
    PDB_OUTPUT_DIRECTORY_EV2008P03DEBUG      "${OUTPUT_DIRECTORY}"
)
################################################################################
# MSVC runtime library
################################################################################
get_property(MSVC_RUNTIME_LIBRARY_DEFAULT TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY)
string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
    $<$<CONFIG:Debug>:
        MultiThreadedDebug
    >
    $<$<CONFIG:DebugPR00FPS>:
        MultiThreadedDebug
    >
    $<$<CONFIG:DebugTest_PRooFPSdd>:
        MultiThreadedDebug
    >
    $<$<CONFIG:EV2008P03Debug>:
        MultiThreadedDebug
    >
    $<$<CONFIG:EV2008P03Release>:
        MultiThreaded
    >
    $<$<CONFIG:Release>:
        MultiThreaded
    >
    $<$<CONFIG:ReleasePR00FPS>:
        MultiThreaded
    >
    $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:DebugPR00FPS>,$<CONFIG:DebugTest_PRooFPSdd>,$<CONFIG:EV2008P03Debug>,$<CONFIG:EV2008P03Release>,$<CONFIG:Release>,$<CONFIG:ReleasePR00FPS>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
)
set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY ${MSVC_RUNTIME_LIBRARY_STR})

################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/../../src/audiosource;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../src/backend;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../src/core;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../src/c_api;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../src/filter;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../src/tools;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../src/audiosource/monotone;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../src/audiosource/noise;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../src/audiosource/openmpt;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../src/audiosource/sfxr;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../src/audiosource/speech;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../src/audiosource/tedsid;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../src/audiosource/vic;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../src/audiosource/vizsn;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../src/audiosource/wav;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../src/backend/alsa;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../src/backend/coreaudio;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../src/backend/jack;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../src/backend/miniaudio;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../src/backend/nosound;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../src/backend/null;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../src/backend/openal;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../src/backend/opensles;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../src/backend/oss;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../src/backend/portaudio;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../src/backend/sdl;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../src/backend/sdl2_static;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../src/backend/sdl_static;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../src/backend/vita_homebrew;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../src/backend/wasapi;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../src/backend/winmm;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../src/backend/xaudio2;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../src/tools/codegen;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../src/tools/lutgen;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../src/tools/resamplerlab;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../src/tools/sanity;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../src/tools/tedsid2dump;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../../../../libraries/openal/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../../../../libraries/portaudio/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../../../../libraries/sdl2/include"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Debug>:"
        "DEBUG"
    ">"
    "$<$<CONFIG:DebugPR00FPS>:"
        "DEBUG"
    ">"
    "$<$<CONFIG:DebugTest_PRooFPSdd>:"
        "DEBUG"
    ">"
    "$<$<CONFIG:EV2008P03Debug>:"
        "DEBUG"
    ">"
    "$<$<CONFIG:EV2008P03Release>:"
        "NDEBUG"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG"
    ">"
    "$<$<CONFIG:ReleasePR00FPS>:"
        "NDEBUG"
    ">"
    "NOMINMAX;"
    "_CRT_SECURE_NO_WARNINGS;"
    "WITH_MINIAUDIO;"
    "WITH_NOSOUND;"
    "WITH_NULL"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /Od;
            /RTC1;
            /ZI
        >
        $<$<CONFIG:DebugPR00FPS>:
            /Od;
            /RTC1;
            /ZI
        >
        $<$<CONFIG:DebugTest_PRooFPSdd>:
            /Od;
            /RTC1;
            /ZI
        >
        $<$<CONFIG:EV2008P03Debug>:
            /Od;
            /RTC1;
            /ZI
        >
        $<$<CONFIG:EV2008P03Release>:
            /O2;
            /GF;
            /arch:SSE2;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT}
        >
        $<$<CONFIG:Release>:
            /O2;
            /GF;
            /arch:SSE2;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT}
        >
        $<$<CONFIG:ReleasePR00FPS>:
            /O2;
            /GF;
            /arch:SSE2;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT}
        >
        /MP;
        /Gm-;
        /Gy;
        /W3;
        /GR-;
        /diagnostics:caret;
        /Y-
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:EV2008P03Release>:
            /OPT:REF;
            /OPT:ICF
        >
        $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF
        >
        $<$<CONFIG:ReleasePR00FPS>:
            /OPT:REF;
            /OPT:ICF
        >
        /SUBSYSTEM:WINDOWS
    )
endif()

