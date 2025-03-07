/*
    ###############################################
    UnitTests.cpp
    Collection of unit tests for PGE
    Made by PR00F88
    ################################################
*/

// need to define this macro so we can use Test::runTests() with Console lib
#ifndef TEST_WITH_CCONSOLE
#define TEST_WITH_CCONSOLE
#endif
#include "Test.h"  // PCH

#ifndef WINPROOF88_ALLOW_CONTROLS_AND_DIALOGS
#define WINPROOF88_ALLOW_CONTROLS_AND_DIALOGS
#endif
#ifndef WINPROOF88_ALLOW_MSG_USER_WINMESSAGES
#define WINPROOF88_ALLOW_MSG_USER_WINMESSAGES
#endif
// otherwise SoLoud.h will have problem due to colliding macro NOSOUND!
#ifndef WINPROOF88_ALLOW_SOUND
#define WINPROOF88_ALLOW_SOUND
#endif
#include "winproof88.h"   // part of PFL lib: https://github.com/proof88/PFL

#include "PFLTest.h"
#include "PFLFixFIFOTest.h"
#include "PgeObjectPoolTest.h"
#include "PGEcfgVariableTest.h"
#include "PGEcfgFileTest.h"
#include "PGEcfgProfilesTest.h"
#include "PgeOldNewValueTest.h"
#include "PgePacketTest.h"
#include "PGEBulletTest.h"
#include "PgeWeaponsTest.h"
#include "PR00FsUltimateRenderingEngineTest.h"
#include "PR00FsUltimateRenderingEngineTest2.h"
#include "PureAxisAlignedBoundingBoxTest.h"
#include "PureOctreeTest.h"
#include "PureBoundingVolumeHierarchyTest.h"
#include "PureCameraTest.h"
#include "PureColorTest.h"
#include "PureFiledManagedTest.h"
#include "PureFiledManagerTest.h"
#include "PureHwAudioTest.h"
#include "PurehwCentralProcessorTest.h"
#include "PureHwInfoTest.h"
#include "PureHwSystemMemoryTest.h"
#include "PureHwVideoTest.h"
#include "PureImageTest.h"
#include "PureImageManagerTest.h"
#include "PureManagedTest.h"
#include "PureManagerTest.h"
#include "PureMaterialTest.h"
#include "PureMaterialManagerTest.h"
#include "PureMatrixTest.h"
#include "PureMesh3DTest.h"
#include "PureMesh3DManagerTest.h"
#include "PureObject3DTest.h"
#include "PureObject3DManagerTest.h"
#include "PurePosUpTargetTest.h"
#include "PureProjectionTest.h"
#include "PureScissorTest.h"
#include "PureScreenTest.h"
#include "PureSharedSettingsTest.h"
#include "PureTextureTest.h"
#include "PureTextureManagerTest.h"
#include "PureTransformMatrixTest.h"
#include "PureVectorTest.h"
#include "PureVertexTransferTest.h"
#include "PureWindowTest.h"
#include "PureWindowTest2.h"
#include "PureRendererHWfixedPipeTest.h"

static CConsole& getConsole()
{
    return CConsole::getConsoleInstance();
}

/**
    Entry point for UnitTests.
*/
int WINAPI WinMain(_In_ HINSTANCE /*hInstance*/, _In_opt_ HINSTANCE /*hPrevInstance*/, _In_ LPSTR /*lpCmdLine*/, _In_ int /*nCmdShow*/)
{
    constexpr const char* const CON_TITLE = "Unit tests for PGE";

    getConsole().Initialize(CON_TITLE, true);
    //getConsole().SetLoggingState("4LLM0DUL3S", true);
    getConsole().SetErrorsAlwaysOn(false);

    getConsole().OLn("");
    // Expecting NDEBUG to be reliable: https://man7.org/linux/man-pages/man3/assert.3.html
#ifdef NDEBUG
    const char* const szBuildType = "Release";
#else
    const char* const szBuildType = "Debug";
#endif 
    getConsole().OLn("%s. Build Type: %s, Timestamp: %s @ %s", CON_TITLE, szBuildType, __DATE__, __TIME__);

    std::vector<std::unique_ptr<Test>> tests;
    
    //tests.push_back(std::unique_ptr<Test>(new PFLTest));
    //tests.push_back(std::unique_ptr<Test>(new PFLFixFIFOTest));

    //tests.push_back(std::unique_ptr<Test>(new PgeObjectPoolTest));
    
    /*    
    tests.push_back(std::unique_ptr<Test>(new PGEcfgVariableTest));
    tests.push_back(std::unique_ptr<Test>(new PGEcfgFileTest));
    tests.push_back(std::unique_ptr<Test>(new PGEcfgProfilesTest));  
    tests.push_back(std::unique_ptr<Test>(new PgeOldNewValueTest));
    */
    
    //tests.push_back(std::unique_ptr<Test>(new PgePacketTest));
    
    //tests.push_back(std::unique_ptr<Test>(new PgeWeaponsTest));
    //tests.push_back(std::unique_ptr<Test>(new PGEBulletTest));
    /**/
    
    /*  
    tests.push_back(std::unique_ptr<Test>(new PureScreenTest));
    tests.push_back(std::unique_ptr<Test>(new PureSharedSettingsTest));
    */
    
    /*  */
    //tests.push_back(std::unique_ptr<Test>(new PurePosUpTargetTest));
    //tests.push_back(std::unique_ptr<Test>(new PureVectorTest));
    //tests.push_back(std::unique_ptr<Test>(new PureMatrixTest));
    //tests.push_back(std::unique_ptr<Test>(new PureTransformMatrixTest));
    tests.push_back(std::unique_ptr<Test>(new PureAxisAlignedBoundingBoxTest));
    tests.push_back(std::unique_ptr<Test>(new PureOctreeTest));
    tests.push_back(std::unique_ptr<Test>(new PureBoundingVolumeHierarchyTest));
    //tests.push_back(std::unique_ptr<Test>(new PureScissorTest));
    //tests.push_back(std::unique_ptr<Test>(new PureProjectionTest));
    

    /*   
    tests.push_back(std::unique_ptr<Test>(new PureColorTest));
    tests.push_back(std::unique_ptr<Test>(new PureManagerTest));
    tests.push_back(std::unique_ptr<Test>(new PureManagedTest));
    tests.push_back(std::unique_ptr<Test>(new PureFiledManagerTest));
    tests.push_back(std::unique_ptr<Test>(new PureFiledManagedTest));

    tests.push_back(std::unique_ptr<Test>(new PureHwInfoTest));
    tests.push_back(std::unique_ptr<Test>(new PurehwCentralProcessorTest));
    tests.push_back(std::unique_ptr<Test>(new PureHwSystemMemoryTest));
    tests.push_back(std::unique_ptr<Test>(new PureHwAudioTest));
    tests.push_back(std::unique_ptr<Test>(new PureHwVideoTest));

    tests.push_back(std::unique_ptr<Test>(new PureImageManagerTest));
    tests.push_back(std::unique_ptr<Test>(new PureImageTest));
    tests.push_back(std::unique_ptr<Test>(new PureTextureManagerTest));
    tests.push_back(std::unique_ptr<Test>(new PureTextureTest));   
    tests.push_back(std::unique_ptr<Test>(new PureMaterialManagerTest));
    tests.push_back(std::unique_ptr<Test>(new PureMaterialTest));
    */

    //tests.push_back(std::unique_ptr<Test>(new PureCameraTest));
    
    /*  
    tests.push_back(std::unique_ptr<Test>(new PureMesh3DManagerTest));
    tests.push_back(std::unique_ptr<Test>(new PureMesh3DTest));
    tests.push_back(std::unique_ptr<Test>(new PureVertexTransferTest));
    tests.push_back(std::unique_ptr<Test>(new PureObject3DManagerTest));
    tests.push_back(std::unique_ptr<Test>(new PureObject3DTest));
    */
    
    /*   
    tests.push_back(std::unique_ptr<Test>(new PureWindowTest));
    tests.push_back(std::unique_ptr<Test>(new PureWindowTest2));
    */

    //tests.push_back(std::unique_ptr<Test>(new PureRendererHWfixedPipeTest));
    
    /*  
    tests.push_back(std::unique_ptr<Test>(new PR00FsUltimateRenderingEngineTest));
    tests.push_back(std::unique_ptr<Test>(new PR00FsUltimateRenderingEngineTest2));
    */

    Test::runTests(tests, getConsole, "Running Unit Tests ...");
    system("pause");

    getConsole().Deinitialize();

    return 0;

} // WinMain()
