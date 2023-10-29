set(PROJECT_NAME UnitTests)

################################################################################
# Source groups
################################################################################
set(no_group_source_files
    "PurehwAudioTest.h"
    "PurehwSystemMemoryTest.h"
)
source_group("" FILES ${no_group_source_files})

set(Header_Files
    "PFLTest.h"
    "PGEBulletTest.h"
    "PGEcfgFileTest.h"
    "PGEcfgProfilesTest.h"
    "PGEcfgVariableTest.h"
    "PgeOldNewValueTest.h"
    "PgePacketTest.h"
    "PR00FsUltimateRenderingEngineTest.h"
    "PR00FsUltimateRenderingEngineTest2.h"
    "PureAxisAlignedBoundingBoxTest.h"
    "PureBoundingVolumeHierarchyTest.h"
    "PureCameraTest.h"
    "PureColorTest.h"
    "PureFiledManagedTest.h"
    "PureFiledManagerTest.h"
    "PureHardwareInfoTest.h"
    "PurehwCentralProcessorTest.h"
    "PurehwInfoTest.h"
    "PurehwVideoTest.h"
    "PureImageManagerTest.h"
    "PureImageTest.h"
    "PureManagedTest.h"
    "PureManagerTest.h"
    "PureMaterialManagerTest.h"
    "PureMaterialTest.h"
    "PureMatrixTest.h"
    "PureMesh3DManagerTest.h"
    "PureMesh3DTest.h"
    "PureObject3DManagerTest.h"
    "PureObject3DTest.h"
    "PureOctreeTest.h"
    "PurePosUpTargetTest.h"
    "PureRendererHWfixedPipeTest.h"
    "PureScissorTest.h"
    "PureScreenTest.h"
    "PureSharedSettingsTest.h"
    "PureTextureManagerTest.h"
    "PureTextureTest.h"
    "PureTransformMatrixTest.h"
    "PureVectorTest.h"
    "PureVertexTransferTest.h"
    "PureWindowTest.h"
    "PureWindowTest2.h"
    "UnitTest.h"
    "WeaponsTest.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Header_Files__CConsole
    "../../../Console/CConsole/src/CConsole.h"
)
source_group("Header Files\\CConsole" FILES ${Header_Files__CConsole})

set(Header_Files__PFL
    "../../../PFL/PFL/PFL.h"
    "../../../PFL/PFL/winproof88.h"
)
source_group("Header Files\\PFL" FILES ${Header_Files__PFL})

set(Header_Files__PGE
    "../PGE.h"
    "../PGEallHeaders.h"
    "../PGEInputHandler.h"
    "../PGEWorld.h"
    "../PGEWorldTime.h"
    "../PGEWorldWeather.h"
)
source_group("Header Files\\PGE" FILES ${Header_Files__PGE})

set(Header_Files__PGE__Config
    "../Config/PGEcfgFile.h"
    "../Config/PGEcfgProfiles.h"
    "../Config/PGEcfgVariable.h"
    "../Config/PgeOldNewValue.h"
)
source_group("Header Files\\PGE\\Config" FILES ${Header_Files__PGE__Config})

set(Header_Files__PGE__Network
    "../Network/PgeClient.h"
    "../Network/PgeIServerClient.h"
    "../Network/PgeNetwork.h"
    "../Network/PgePacket.h"
    "../Network/PgeServer.h"
)
source_group("Header Files\\PGE\\Network" FILES ${Header_Files__PGE__Network})

set(Header_Files__PGE__PURE
    "../PURE/include/external/PR00FsUltimateRenderingEngine.h"
    "../PURE/include/external/PureAllHeaders.h"
    "../PURE/include/external/PureCamera.h"
    "../PURE/include/external/PureFiledManager.h"
    "../PURE/include/external/PureManager.h"
    "../PURE/include/external/PureTypes.h"
    "../PURE/include/external/PureUiManager.h"
)
source_group("Header Files\\PGE\\PURE" FILES ${Header_Files__PGE__PURE})

set(Header_Files__PGE__PURE__Display
    "../PURE/include/external/Display/PureScreen.h"
    "../PURE/include/external/Display/PureWindow.h"
)
source_group("Header Files\\PGE\\PURE\\Display" FILES ${Header_Files__PGE__PURE__Display})

set(Header_Files__PGE__PURE__Hardware
    "../PURE/include/external/Hardware/PureHwAudio.h"
    "../PURE/include/external/Hardware/PureHwBase.h"
    "../PURE/include/external/Hardware/PureHwCentralProcessor.h"
    "../PURE/include/external/Hardware/PureHwInfo.h"
    "../PURE/include/external/Hardware/PureHwSystemMemory.h"
    "../PURE/include/external/Hardware/PureHwVideo.h"
)
source_group("Header Files\\PGE\\PURE\\Hardware" FILES ${Header_Files__PGE__PURE__Hardware})

set(Header_Files__PGE__PURE__Material
    "../PURE/include/external/Material/PureColor.h"
    "../PURE/include/external/Material/PureImageManager.h"
    "../PURE/include/external/Material/PureMaterialManager.h"
    "../PURE/include/external/Material/PureTextureManager.h"
)
source_group("Header Files\\PGE\\PURE\\Material" FILES ${Header_Files__PGE__PURE__Material})

set(Header_Files__PGE__PURE__Math
    "../PURE/include/external/Math/PureMatrix.h"
    "../PURE/include/external/Math/PurePosUpTarget.h"
    "../PURE/include/external/Math/PureTransformMatrix.h"
    "../PURE/include/external/Math/PureVector.h"
)
source_group("Header Files\\PGE\\PURE\\Math" FILES ${Header_Files__PGE__PURE__Math})

set(Header_Files__PGE__PURE__Object3D
    "../PURE/include/external/Object3D/PureMesh3DManager.h"
    "../PURE/include/external/Object3D/PureObject3DManager.h"
    "../PURE/include/external/Object3D/PureVertexTransferMode.h"
)
source_group("Header Files\\PGE\\PURE\\Object3D" FILES ${Header_Files__PGE__PURE__Object3D})

set(Header_Files__PGE__PURE__Render
    "../PURE/include/external/Render/PureIIncrementalRenderPipeline.h"
    "../PURE/include/external/Render/PureIRenderable.h"
    "../PURE/include/external/Render/PureIRenderer.h"
    "../PURE/include/external/Render/PureRendererHWfixedPipe.h"
    "../PURE/include/external/Render/PureRendererSWincremental.h"
)
source_group("Header Files\\PGE\\PURE\\Render" FILES ${Header_Files__PGE__PURE__Render})

set(Source_Files
    "UnitTest.cpp"
    "UnitTests.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${no_group_source_files}
    ${Header_Files}
    ${Header_Files__CConsole}
    ${Header_Files__PFL}
    ${Header_Files__PGE}
    ${Header_Files__PGE__Config}
    ${Header_Files__PGE__Network}
    ${Header_Files__PGE__PURE}
    ${Header_Files__PGE__PURE__Display}
    ${Header_Files__PGE__PURE__Hardware}
    ${Header_Files__PGE__PURE__Material}
    ${Header_Files__PGE__PURE__Math}
    ${Header_Files__PGE__PURE__Object3D}
    ${Header_Files__PGE__PURE__Render}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

target_precompile_headers(${PROJECT_NAME} PRIVATE
    "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/UnitTest.h>"
)

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE UnitTests)

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
    "_WINDOWS;"
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
        /WX;
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
        /GR;
        /Zc:__cplusplus;
        ${DEFAULT_CXX_EXCEPTION_HANDLING}
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /DEBUG;
            /OPT:NOREF;
            /OPT:NOICF;
            /INCREMENTAL
        >
        $<$<CONFIG:DebugPR00FPS>:
            /DEBUG;
            /OPT:NOREF;
            /OPT:NOICF;
            /INCREMENTAL
        >
        $<$<CONFIG:DebugTest_PRooFPSdd>:
            /DEBUG;
            /OPT:NOREF;
            /OPT:NOICF;
            /INCREMENTAL
        >
        $<$<CONFIG:EV2008P03Debug>:
            /DEBUG;
            /OPT:NOREF;
            /OPT:NOICF;
            /INCREMENTAL
        >
        $<$<CONFIG:EV2008P03Release>:
            /OPT:REF;
            /LTCG;
            /OPT:ICF;
            /INCREMENTAL:NO
        >
        $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF;
            /INCREMENTAL:NO
        >
        $<$<CONFIG:ReleasePR00FPS>:
            /OPT:REF;
            /LTCG;
            /OPT:ICF;
            /INCREMENTAL:NO
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
    PGE
)

set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "kernel32;"
    "user32;"
    "gdi32;"
    "winspool;"
    "comdlg32;"
    "advapi32;"
    "shell32;"
    "ole32;"
    "oleaut32;"
    "uuid;"
    "odbc32;"
    "odbccp32;"
    "opengl32;"
    "glu32;"
    "dwmapi;"
    "PGE"
)
target_link_libraries(${PROJECT_NAME} PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_SOURCE_DIR}/$<CONFIG>"
)

