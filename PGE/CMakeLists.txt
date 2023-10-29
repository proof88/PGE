set(PROJECT_NAME PGE)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "PGE.h"
    "PGEallHeaders.h"
    "PGEconsts.h"
    "PGEincludes.h"
    "PGEInputHandler.h"
    "PGEpragmas.h"
    "PGESysGFX.h"
    "PGESysSFX.h"
    "PGEWorld.h"
    "PGEWorldTime.h"
    "PGEWorldWeather.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Header_Files__Audio__SoLoud
    "Audio/soloud-RELEASE_20200207/include/soloud.h"
    "Audio/soloud-RELEASE_20200207/include/soloud_audiosource.h"
    "Audio/soloud-RELEASE_20200207/include/soloud_bassboostfilter.h"
    "Audio/soloud-RELEASE_20200207/include/soloud_biquadresonantfilter.h"
    "Audio/soloud-RELEASE_20200207/include/soloud_bus.h"
    "Audio/soloud-RELEASE_20200207/include/soloud_c.h"
    "Audio/soloud-RELEASE_20200207/include/soloud_dcremovalfilter.h"
    "Audio/soloud-RELEASE_20200207/include/soloud_echofilter.h"
    "Audio/soloud-RELEASE_20200207/include/soloud_error.h"
    "Audio/soloud-RELEASE_20200207/include/soloud_fader.h"
    "Audio/soloud-RELEASE_20200207/include/soloud_fft.h"
    "Audio/soloud-RELEASE_20200207/include/soloud_fftfilter.h"
    "Audio/soloud-RELEASE_20200207/include/soloud_file.h"
    "Audio/soloud-RELEASE_20200207/include/soloud_file_hack_off.h"
    "Audio/soloud-RELEASE_20200207/include/soloud_file_hack_on.h"
    "Audio/soloud-RELEASE_20200207/include/soloud_filter.h"
    "Audio/soloud-RELEASE_20200207/include/soloud_flangerfilter.h"
    "Audio/soloud-RELEASE_20200207/include/soloud_freeverbfilter.h"
    "Audio/soloud-RELEASE_20200207/include/soloud_internal.h"
    "Audio/soloud-RELEASE_20200207/include/soloud_lofifilter.h"
    "Audio/soloud-RELEASE_20200207/include/soloud_misc.h"
    "Audio/soloud-RELEASE_20200207/include/soloud_monotone.h"
    "Audio/soloud-RELEASE_20200207/include/soloud_noise.h"
    "Audio/soloud-RELEASE_20200207/include/soloud_openmpt.h"
    "Audio/soloud-RELEASE_20200207/include/soloud_queue.h"
    "Audio/soloud-RELEASE_20200207/include/soloud_robotizefilter.h"
    "Audio/soloud-RELEASE_20200207/include/soloud_sfxr.h"
    "Audio/soloud-RELEASE_20200207/include/soloud_speech.h"
    "Audio/soloud-RELEASE_20200207/include/soloud_tedsid.h"
    "Audio/soloud-RELEASE_20200207/include/soloud_thread.h"
    "Audio/soloud-RELEASE_20200207/include/soloud_vic.h"
    "Audio/soloud-RELEASE_20200207/include/soloud_vizsn.h"
    "Audio/soloud-RELEASE_20200207/include/soloud_wav.h"
    "Audio/soloud-RELEASE_20200207/include/soloud_waveshaperfilter.h"
    "Audio/soloud-RELEASE_20200207/include/soloud_wavstream.h"
)
source_group("Header Files\\Audio\\SoLoud" FILES ${Header_Files__Audio__SoLoud})

set(Header_Files__CConsole
    "../../Console/CConsole/src/CConsole.h"
)
source_group("Header Files\\CConsole" FILES ${Header_Files__CConsole})

set(Header_Files__Config
    "Config/PGEcfgFile.h"
    "Config/PGEcfgIHandler.h"
    "Config/PGEcfgProfiles.h"
    "Config/PGEcfgVariable.h"
    "Config/PgeOldNewValue.h"
)
source_group("Header Files\\Config" FILES ${Header_Files__Config})

set(Header_Files__Network
    "Network/PgeClient.h"
    "Network/PgeGnsClient.h"
    "Network/PgeGnsServer.h"
    "Network/PgeGnsWrapper.h"
    "Network/PgeIServerClient.h"
    "Network/PgeNetwork.h"
    "Network/PgePacket.h"
    "Network/PgeServer.h"
)
source_group("Header Files\\Network" FILES ${Header_Files__Network})

set(Header_Files__Network__GameNetworkingSockets-1.4.0
    "Network/GameNetworkingSockets-1.4.0/include/steam/isteamnetworkingmessages.h"
    "Network/GameNetworkingSockets-1.4.0/include/steam/isteamnetworkingsockets.h"
    "Network/GameNetworkingSockets-1.4.0/include/steam/isteamnetworkingutils.h"
    "Network/GameNetworkingSockets-1.4.0/include/steam/steam_api_common.h"
    "Network/GameNetworkingSockets-1.4.0/include/steam/steamclientpublic.h"
    "Network/GameNetworkingSockets-1.4.0/include/steam/steamnetworkingcustomsignaling.h"
    "Network/GameNetworkingSockets-1.4.0/include/steam/steamnetworkingsockets.h"
    "Network/GameNetworkingSockets-1.4.0/include/steam/steamnetworkingsockets_flat.h"
    "Network/GameNetworkingSockets-1.4.0/include/steam/steamnetworkingtypes.h"
    "Network/GameNetworkingSockets-1.4.0/include/steam/steamtypes.h"
    "Network/GameNetworkingSockets-1.4.0/include/steam/steamuniverse.h"
)
source_group("Header Files\\Network\\GameNetworkingSockets-1.4.0" FILES ${Header_Files__Network__GameNetworkingSockets-1.4.0})

set(Header_Files__PFL
    "../../PFL/PFL/PFL.h"
    "../../PFL/PFL/winproof88.h"
)
source_group("Header Files\\PFL" FILES ${Header_Files__PFL})

set(Header_Files__PURE__include__external
    "PURE/include/external/PR00FsUltimateRenderingEngine.h"
    "PURE/include/external/PureAllHeaders.h"
    "PURE/include/external/PureCamera.h"
    "PURE/include/external/PureFiledManager.h"
    "PURE/include/external/PureManager.h"
    "PURE/include/external/PureTypes.h"
    "PURE/include/external/PureUiManager.h"
)
source_group("Header Files\\PURE\\include\\external" FILES ${Header_Files__PURE__include__external})

set(Header_Files__PURE__include__external__Display
    "PURE/include/external/Display/PureScreen.h"
    "PURE/include/external/Display/PureWindow.h"
)
source_group("Header Files\\PURE\\include\\external\\Display" FILES ${Header_Files__PURE__include__external__Display})

set(Header_Files__PURE__include__external__Hardware
    "PURE/include/external/Hardware/PureHwAudio.h"
    "PURE/include/external/Hardware/PureHwBase.h"
    "PURE/include/external/Hardware/PureHwCentralProcessor.h"
    "PURE/include/external/Hardware/PureHwInfo.h"
    "PURE/include/external/Hardware/PureHwSystemMemory.h"
    "PURE/include/external/Hardware/PureHwVideo.h"
)
source_group("Header Files\\PURE\\include\\external\\Hardware" FILES ${Header_Files__PURE__include__external__Hardware})

set(Header_Files__PURE__include__external__Material
    "PURE/include/external/Material/PureColor.h"
    "PURE/include/external/Material/PureImageManager.h"
    "PURE/include/external/Material/PureMaterialManager.h"
    "PURE/include/external/Material/PureTextureManager.h"
)
source_group("Header Files\\PURE\\include\\external\\Material" FILES ${Header_Files__PURE__include__external__Material})

set(Header_Files__PURE__include__external__Math
    "PURE/include/external/Math/PureMatrix.h"
    "PURE/include/external/Math/PurePosUpTarget.h"
    "PURE/include/external/Math/PureTransformMatrix.h"
    "PURE/include/external/Math/PureVector.h"
)
source_group("Header Files\\PURE\\include\\external\\Math" FILES ${Header_Files__PURE__include__external__Math})

set(Header_Files__PURE__include__external__Object3D
    "PURE/include/external/Object3D/PureMesh3DManager.h"
    "PURE/include/external/Object3D/PureObject3DManager.h"
    "PURE/include/external/Object3D/PureVertexTransferMode.h"
)
source_group("Header Files\\PURE\\include\\external\\Object3D" FILES ${Header_Files__PURE__include__external__Object3D})

set(Header_Files__PURE__include__external__Render
    "PURE/include/external/Render/PureIIncrementalRenderPipeline.h"
    "PURE/include/external/Render/PureIRenderable.h"
    "PURE/include/external/Render/PureIRenderer.h"
    "PURE/include/external/Render/PureRendererHWfixedPipe.h"
    "PURE/include/external/Render/PureRendererSWincremental.h"
)
source_group("Header Files\\PURE\\include\\external\\Render" FILES ${Header_Files__PURE__include__external__Render})

set(Header_Files__PURE__include__internal
    "PURE/include/internal/PureBaseIncludes.h"
    "PURE/include/internal/PureConsts.h"
    "PURE/include/internal/PureGLextensionFuncs.h"
    "PURE/include/internal/PureGLsafeFuncs.h"
    "PURE/include/internal/PureGLsnippets.h"
    "PURE/include/internal/PureManagedImpl.h"
    "PURE/include/internal/PurePragmas.h"
    "PURE/include/internal/PureUiFontWin.h"
    "PURE/include/internal/PureUiText.h"
)
source_group("Header Files\\PURE\\include\\internal" FILES ${Header_Files__PURE__include__internal})

set(Header_Files__PURE__include__internal__Display
    "PURE/include/internal/Display/PureSharedSettings.h"
)
source_group("Header Files\\PURE\\include\\internal\\Display" FILES ${Header_Files__PURE__include__internal__Display})

set(Header_Files__PURE__include__internal__GUI
    "PURE/include/internal/GUI/imgui-1.88/imconfig.h"
    "PURE/include/internal/GUI/imgui-1.88/imgui.h"
    "PURE/include/internal/GUI/imgui-1.88/imgui_internal.h"
    "PURE/include/internal/GUI/imgui-1.88/imstb_rectpack.h"
    "PURE/include/internal/GUI/imgui-1.88/imstb_textedit.h"
    "PURE/include/internal/GUI/imgui-1.88/imstb_truetype.h"
)
source_group("Header Files\\PURE\\include\\internal\\GUI" FILES ${Header_Files__PURE__include__internal__GUI})

set(Header_Files__PURE__include__internal__GUI__Backends
    "PURE/include/internal/GUI/imgui-1.88/backends/imgui_impl_opengl2.h"
    "PURE/include/internal/GUI/imgui-1.88/backends/imgui_impl_win32.h"
)
source_group("Header Files\\PURE\\include\\internal\\GUI\\Backends" FILES ${Header_Files__PURE__include__internal__GUI__Backends})

set(Header_Files__PURE__include__internal__Hardware
    "PURE/include/internal/Hardware/PureHwVideoDiscoverOpenGL_1_1.h"
    "PURE/include/internal/Hardware/PureHwVideoDiscoverOpenGL_1_2.h"
    "PURE/include/internal/Hardware/PureHwVideoDiscoverOpenGL_1_3.h"
    "PURE/include/internal/Hardware/PureHwVideoDiscoverOpenGL_1_4.h"
    "PURE/include/internal/Hardware/PureHwVideoDiscoverOpenGL_1_5.h"
    "PURE/include/internal/Hardware/PureHwVideoDiscoverOpenGL_2_0.h"
    "PURE/include/internal/Hardware/PureHwVideoDiscoverOpenGL_2_1.h"
    "PURE/include/internal/Hardware/PureHwVideoDiscoverOpenGL_3_0.h"
    "PURE/include/internal/Hardware/PureHwVideoDiscoverOpenGL_3_1.h"
    "PURE/include/internal/Hardware/PureHwVideoDiscoverOpenGL_3_2.h"
    "PURE/include/internal/Hardware/PureHwVideoDiscoverOpenGL_3_3.h"
    "PURE/include/internal/Hardware/PureHwVideoDiscoverOpenGL_4_0.h"
    "PURE/include/internal/Hardware/PureHwVideoDiscoverOpenGL_4_1.h"
    "PURE/include/internal/Hardware/PureHwVideoDiscoverOpenGL_4_2.h"
    "PURE/include/internal/Hardware/PureHwVideoDiscoverOpenGL_4_3.h"
    "PURE/include/internal/Hardware/PureHwVideoDiscoverOpenGLbase.h"
)
source_group("Header Files\\PURE\\include\\internal\\Hardware" FILES ${Header_Files__PURE__include__internal__Hardware})

set(Header_Files__PURE__include__internal__Material
    "PURE/include/internal/Material/PureImageImpl.h"
    "PURE/include/internal/Material/PureMaterialImpl.h"
    "PURE/include/internal/Material/PureTextureImpl.h"
)
source_group("Header Files\\PURE\\include\\internal\\Material" FILES ${Header_Files__PURE__include__internal__Material})

set(Header_Files__PURE__include__internal__Math
    "PURE/include/internal/Math/PureScissor.h"
)
source_group("Header Files\\PURE\\include\\internal\\Math" FILES ${Header_Files__PURE__include__internal__Math})

set(Header_Files__PURE__include__internal__Object3D
    "PURE/include/internal/Object3D/PureMesh3DImpl.h"
    "PURE/include/internal/Object3D/PureMesh3DManagerImpl.h"
    "PURE/include/internal/Object3D/PureObject3DImpl.h"
    "PURE/include/internal/Object3D/PureVertexTransferModeImpl.h"
)
source_group("Header Files\\PURE\\include\\internal\\Object3D" FILES ${Header_Files__PURE__include__internal__Object3D})

set(Header_Files__PURE__include__internal__SpatialStructures
    "PURE/include/internal/SpatialStructures/PureAxisAlignedBoundingBox.h"
    "PURE/include/internal/SpatialStructures/PureBoundingVolumeHierarchy.h"
    "PURE/include/internal/SpatialStructures/PureOctree.h"
)
source_group("Header Files\\PURE\\include\\internal\\SpatialStructures" FILES ${Header_Files__PURE__include__internal__SpatialStructures})

set(Header_Files__PURE__include__internal__gl
    "PURE/include/internal/gl/GL.h"
    "PURE/include/internal/gl/GLAux.h"
    "PURE/include/internal/gl/glext.h"
    "PURE/include/internal/gl/GLU.h"
    "PURE/include/internal/gl/glut.h"
    "PURE/include/internal/gl/glxext.h"
    "PURE/include/internal/gl/wglext.h"
)
source_group("Header Files\\PURE\\include\\internal\\gl" FILES ${Header_Files__PURE__include__internal__gl})

set(Header_Files__Weapons
    "Weapons/WeaponManager.h"
)
source_group("Header Files\\Weapons" FILES ${Header_Files__Weapons})

set(Source_Files
    "PGE.cpp"
    "PGEInputHandler.cpp"
    "PGESysGFX.cpp"
    "PGESysSFX.cpp"
    "PGEWorld.cpp"
    "PGEWorldTime.cpp"
    "PGEWorldWeather.cpp"
    "PURE/source/PR00FsUltimateRenderingEngine.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(Source_Files__Config
    "Config/PGEcfgFile.cpp"
    "Config/PGEcfgProfiles.cpp"
    "Config/PGEcfgVariable.cpp"
)
source_group("Source Files\\Config" FILES ${Source_Files__Config})

set(Source_Files__Network
    "Network/PgeClient.cpp"
    "Network/PgeGnsClient.cpp"
    "Network/PgeGnsServer.cpp"
    "Network/PgeGnsWrapper.cpp"
    "Network/PgeNetwork.cpp"
    "Network/PgePacket.cpp"
    "Network/PgeServer.cpp"
)
source_group("Source Files\\Network" FILES ${Source_Files__Network})

set(Source_Files__PURE
    "PURE/source/PureBaseIncludes.cpp"
    "PURE/source/PureCamera.cpp"
    "PURE/source/PureFiledManager.cpp"
    "PURE/source/PureGLextensionFuncs.cpp"
    "PURE/source/PureGLsafeFuncs.cpp"
    "PURE/source/PureGLsnippets.cpp"
    "PURE/source/PureManager.cpp"
    "PURE/source/PureUiFontWin.cpp"
    "PURE/source/PureUiManager.cpp"
    "PURE/source/PureUiText.cpp"
)
source_group("Source Files\\PURE" FILES ${Source_Files__PURE})

set(Source_Files__PURE__Display
    "PURE/source/Display/PureScreen.cpp"
    "PURE/source/Display/PureSharedSettings.cpp"
    "PURE/source/Display/PureWindow.cpp"
)
source_group("Source Files\\PURE\\Display" FILES ${Source_Files__PURE__Display})

set(Source_Files__PURE__GUI
    "PURE/include/internal/GUI/imgui-1.88/imgui.cpp"
    "PURE/include/internal/GUI/imgui-1.88/imgui_demo.cpp"
    "PURE/include/internal/GUI/imgui-1.88/imgui_draw.cpp"
    "PURE/include/internal/GUI/imgui-1.88/imgui_tables.cpp"
    "PURE/include/internal/GUI/imgui-1.88/imgui_widgets.cpp"
)
source_group("Source Files\\PURE\\GUI" FILES ${Source_Files__PURE__GUI})

set(Source_Files__PURE__GUI__Backends
    "PURE/include/internal/GUI/imgui-1.88/backends/imgui_impl_opengl2.cpp"
    "PURE/include/internal/GUI/imgui-1.88/backends/imgui_impl_win32.cpp"
)
source_group("Source Files\\PURE\\GUI\\Backends" FILES ${Source_Files__PURE__GUI__Backends})

set(Source_Files__PURE__Hardware
    "PURE/source/Hardware/PurehwAudio.cpp"
    "PURE/source/Hardware/PurehwBase.cpp"
    "PURE/source/Hardware/PurehwCentralProcessor.cpp"
    "PURE/source/Hardware/PurehwInfo.cpp"
    "PURE/source/Hardware/PurehwSystemMemory.cpp"
    "PURE/source/Hardware/PurehwVideo.cpp"
    "PURE/source/Hardware/PurehwVideoDiscoverOpenGLbase.cpp"
)
source_group("Source Files\\PURE\\Hardware" FILES ${Source_Files__PURE__Hardware})

set(Source_Files__PURE__Material
    "PURE/source/Material/PureColor.cpp"
    "PURE/source/Material/PureImage.cpp"
    "PURE/source/Material/PureImageManager.cpp"
    "PURE/source/Material/PureMaterial.cpp"
    "PURE/source/Material/PureMaterialManager.cpp"
    "PURE/source/Material/PureTexture.cpp"
    "PURE/source/Material/PureTextureManager.cpp"
)
source_group("Source Files\\PURE\\Material" FILES ${Source_Files__PURE__Material})

set(Source_Files__PURE__Math
    "PURE/source/Math/PureMatrix.cpp"
    "PURE/source/Math/PurePosUpTarget.cpp"
    "PURE/source/Math/PUREScissor.cpp"
    "PURE/source/Math/PureTransformMatrix.cpp"
    "PURE/source/Math/PureVector.cpp"
)
source_group("Source Files\\PURE\\Math" FILES ${Source_Files__PURE__Math})

set(Source_Files__PURE__Object3D
    "PURE/source/Object3D/PureMesh3D.cpp"
    "PURE/source/Object3D/PureMesh3DManager.cpp"
    "PURE/source/Object3D/PureObject3D.cpp"
    "PURE/source/Object3D/PureObject3DManager.cpp"
    "PURE/source/Object3D/PureVertexTransferMode.cpp"
)
source_group("Source Files\\PURE\\Object3D" FILES ${Source_Files__PURE__Object3D})

set(Source_Files__PURE__Render
    "PURE/source/Render/PureRendererHWfixedPipe.cpp"
    "PURE/source/Render/PureRendererSWincremental.cpp"
)
source_group("Source Files\\PURE\\Render" FILES ${Source_Files__PURE__Render})

set(Source_Files__PURE__SpatialStructures
    "PURE/source/SpatialStructures/PureAxisAlignedBoundingBox.cpp"
    "PURE/source/SpatialStructures/PureBoundingVolumeHierarchy.cpp"
    "PURE/source/SpatialStructures/PureOctree.cpp"
)
source_group("Source Files\\PURE\\SpatialStructures" FILES ${Source_Files__PURE__SpatialStructures})

set(Source_Files__Weapons
    "Weapons/WeaponManager.cpp"
)
source_group("Source Files\\Weapons" FILES ${Source_Files__Weapons})

set(ALL_FILES
    ${Header_Files}
    ${Header_Files__Audio__SoLoud}
    ${Header_Files__CConsole}
    ${Header_Files__Config}
    ${Header_Files__Network}
    ${Header_Files__Network__GameNetworkingSockets-1.4.0}
    ${Header_Files__PFL}
    ${Header_Files__PURE__include__external}
    ${Header_Files__PURE__include__external__Display}
    ${Header_Files__PURE__include__external__Hardware}
    ${Header_Files__PURE__include__external__Material}
    ${Header_Files__PURE__include__external__Math}
    ${Header_Files__PURE__include__external__Object3D}
    ${Header_Files__PURE__include__external__Render}
    ${Header_Files__PURE__include__internal}
    ${Header_Files__PURE__include__internal__Display}
    ${Header_Files__PURE__include__internal__GUI}
    ${Header_Files__PURE__include__internal__GUI__Backends}
    ${Header_Files__PURE__include__internal__Hardware}
    ${Header_Files__PURE__include__internal__Material}
    ${Header_Files__PURE__include__internal__Math}
    ${Header_Files__PURE__include__internal__Object3D}
    ${Header_Files__PURE__include__internal__SpatialStructures}
    ${Header_Files__PURE__include__internal__gl}
    ${Header_Files__Weapons}
    ${Source_Files}
    ${Source_Files__Config}
    ${Source_Files__Network}
    ${Source_Files__PURE}
    ${Source_Files__PURE__Display}
    ${Source_Files__PURE__GUI}
    ${Source_Files__PURE__GUI__Backends}
    ${Source_Files__PURE__Hardware}
    ${Source_Files__PURE__Material}
    ${Source_Files__PURE__Math}
    ${Source_Files__PURE__Object3D}
    ${Source_Files__PURE__Render}
    ${Source_Files__PURE__SpatialStructures}
    ${Source_Files__Weapons}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} STATIC ${ALL_FILES})

target_precompile_headers(${PROJECT_NAME} PRIVATE
    "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/PURE/include/internal/PureBaseIncludes.h>"
)

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE PGE)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASE             "TRUE"
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
    "${CMAKE_CURRENT_SOURCE_DIR}/PURE/include/internal;"
    "${CMAKE_CURRENT_SOURCE_DIR}/PURE/include/internal/GUI/imgui-1.88"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Debug>:"
        "_ALLOW_RTCc_IN_STL;"
        "_DEBUG"
    ">"
    "$<$<CONFIG:DebugPR00FPS>:"
        "_ALLOW_RTCc_IN_STL;"
        "_DEBUG"
    ">"
    "$<$<CONFIG:DebugTest_PRooFPSdd>:"
        "_ALLOW_RTCc_IN_STL;"
        "_DEBUG"
    ">"
    "$<$<CONFIG:EV2008P03Debug>:"
        "_ALLOW_RTCc_IN_STL;"
        "_DEBUG"
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
    "WIN32;"
    "_LIB;"
    "_MBCS"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /Od;
            /Gy;
            /W4
        >
        $<$<CONFIG:DebugPR00FPS>:
            /Od;
            /Gy;
            /W4
        >
        $<$<CONFIG:DebugTest_PRooFPSdd>:
            /Od;
            /Gy;
            /W4
        >
        $<$<CONFIG:EV2008P03Debug>:
            /Od;
            /Gy;
            /W4
        >
        $<$<CONFIG:EV2008P03Release>:
            /GL-;
            /Gm-;
            /O2;
            /Oi;
            /Gy-;
            /W3
        >
        $<$<CONFIG:Release>:
            /GL-;
            /O2;
            /Oi;
            /Gy-;
            /W3
        >
        $<$<CONFIG:ReleasePR00FPS>:
            /GL-;
            /Gm-;
            /O2;
            /Oi;
            /Gy-;
            /W3
        >
        /std:c++17;
        /WX;
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
        /GR;
        /Zc:__cplusplus;
        ${DEFAULT_CXX_EXCEPTION_HANDLING}
    )
    string(CONCAT FILE_CL_OPTIONS
        "/Y-"
    )
    source_file_compile_options(PURE/include/internal/GUI/imgui-1.88/backends/imgui_impl_opengl2.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "/Y-"
    )
    source_file_compile_options(PURE/include/internal/GUI/imgui-1.88/backends/imgui_impl_win32.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "/Y-"
    )
    source_file_compile_options(PURE/include/internal/GUI/imgui-1.88/imgui.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "/Y-"
    )
    source_file_compile_options(PURE/include/internal/GUI/imgui-1.88/imgui_demo.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "/Y-"
    )
    source_file_compile_options(PURE/include/internal/GUI/imgui-1.88/imgui_draw.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "/Y-"
    )
    source_file_compile_options(PURE/include/internal/GUI/imgui-1.88/imgui_tables.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "/Y-"
    )
    source_file_compile_options(PURE/include/internal/GUI/imgui-1.88/imgui_widgets.cpp ${FILE_CL_OPTIONS})
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            opengl32.lib;
            glu32.lib
        >
        $<$<CONFIG:DebugPR00FPS>:
            opengl32.lib;
            glu32.lib
        >
        $<$<CONFIG:DebugTest_PRooFPSdd>:
            opengl32.lib;
            glu32.lib
        >
        $<$<CONFIG:EV2008P03Debug>:
            opengl32.lib;
            glu32.lib
        >
        $<$<CONFIG:EV2008P03Release>:
            opengl32.lib;
            glu32.lib
        >
        $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF
        >
        $<$<CONFIG:ReleasePR00FPS>:
            opengl32.lib;
            glu32.lib
        >
        /SUBSYSTEM:WINDOWS
    )
endif()

################################################################################
# Dependencies
################################################################################
add_dependencies(${PROJECT_NAME}
    CConsole
    PFL
    SoloudStatic
)

set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "$<$<CONFIG:Debug>:"
        "soloud_static_x86_d"
    ">"
    "$<$<CONFIG:DebugPR00FPS>:"
        "soloud_static_x86_d"
    ">"
    "$<$<CONFIG:DebugTest_PRooFPSdd>:"
        "soloud_static_x86_d"
    ">"
    "$<$<CONFIG:EV2008P03Debug>:"
        "soloud_static_x86_d"
    ">"
    "$<$<CONFIG:EV2008P03Release>:"
        "soloud_static_x86"
    ">"
    "$<$<CONFIG:Release>:"
        "soloud_static_x86"
    ">"
    "$<$<CONFIG:ReleasePR00FPS>:"
        "soloud_static_x86"
    ">"
    "CConsole;"
    "GameNetworkingSockets"
)
target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_SOURCE_DIR}/$<CONFIG>;"
    "${CMAKE_CURRENT_SOURCE_DIR}/Network/GameNetworkingSockets-1.4.0/build/src/;"
    "${CMAKE_CURRENT_SOURCE_DIR}/Audio/soloud-RELEASE_20200207/lib/"
)

