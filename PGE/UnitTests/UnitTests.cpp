/*
    ###############################################
    UnitTests.cpp
    Collection of unit tests for PGE
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ################################################
*/

#include "UnitTest.h"  // PCH

#include <memory>  // for std::unique_ptr; requires cpp11
#include <vector>

#ifndef WINPROOF88_ALLOW_CONTROLS_AND_DIALOGS
#define WINPROOF88_ALLOW_CONTROLS_AND_DIALOGS
#endif
#ifndef WINPROOF88_ALLOW_MSG_USER_WINMESSAGES
#define WINPROOF88_ALLOW_MSG_USER_WINMESSAGES
#endif
#include "../../../PFL/PFL/winproof88.h"

#include "../../../Console/CConsole/src/CConsole.h"

#include "PFLTest.h"
#include "PGEcfgVariableTest.h"
#include "PGEcfgFileTest.h"
#include "PGEcfgProfilesTest.h"
#include "PgeOldNewValueTest.h"
#include "PgePacketTest.h"
#include "WeaponsTest.h"
#include "PGEBulletTest.h"
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

#define CON_TITLE "Unit tests for PGE"

using namespace std;

#pragma warning(disable:4100)  /* unreferenced formal parameter */

static CConsole& getConsole()
{
    return CConsole::getConsoleInstance();
}

/**
    Entry point for UnitTests.

    This directly uses WinAPI.
*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    getConsole().Initialize(CON_TITLE, true);
    //getConsole().SetLoggingState("4LLM0DUL3S", true);
    getConsole().SetErrorsAlwaysOn(false);
    getConsole().OLn(CON_TITLE);
    getConsole().L();
    getConsole().OLn("");
    
    vector<std::unique_ptr<UnitTest>> tests;
    
       
    tests.push_back(std::unique_ptr<UnitTest>(new PFLTest));
    /*
    tests.push_back(std::unique_ptr<UnitTest>(new PGEcfgVariableTest));
    tests.push_back(std::unique_ptr<UnitTest>(new PGEcfgFileTest));
    tests.push_back(std::unique_ptr<UnitTest>(new PGEcfgProfilesTest));
    tests.push_back(std::unique_ptr<UnitTest>(new PgeOldNewValueTest));
    */
    
    //tests.push_back(std::unique_ptr<UnitTest>(new PgePacketTest));
    
    /*  
    tests.push_back(std::unique_ptr<UnitTest>(new WeaponsTest));
    tests.push_back(std::unique_ptr<UnitTest>(new PGEBulletTest));
    */
    
    /*   
    tests.push_back(std::unique_ptr<UnitTest>(new PureScreenTest));
    tests.push_back(std::unique_ptr<UnitTest>(new PureSharedSettingsTest));
    */
    
    
    /* 
    tests.push_back(std::unique_ptr<UnitTest>(new PurePosUpTargetTest));
    tests.push_back(std::unique_ptr<UnitTest>(new PureVectorTest));
    tests.push_back(std::unique_ptr<UnitTest>(new PureMatrixTest));
    tests.push_back(std::unique_ptr<UnitTest>(new PureTransformMatrixTest));
    tests.push_back(std::unique_ptr<UnitTest>(new PureAxisAlignedBoundingBoxTest));
    tests.push_back(std::unique_ptr<UnitTest>(new PureOctreeTest));
    tests.push_back(std::unique_ptr<UnitTest>(new PureBoundingVolumeHierarchyTest));
    tests.push_back(std::unique_ptr<UnitTest>(new PureScissorTest));

    tests.push_back(std::unique_ptr<UnitTest>(new PureColorTest));
    tests.push_back(std::unique_ptr<UnitTest>(new PureManagerTest));
    tests.push_back(std::unique_ptr<UnitTest>(new PureManagedTest));
    tests.push_back(std::unique_ptr<UnitTest>(new PureFiledManagerTest));
    tests.push_back(std::unique_ptr<UnitTest>(new PureFiledManagedTest));

    tests.push_back(std::unique_ptr<UnitTest>(new PureHwInfoTest));
    tests.push_back(std::unique_ptr<UnitTest>(new PurehwCentralProcessorTest));
    tests.push_back(std::unique_ptr<UnitTest>(new PureHwSystemMemoryTest));
    tests.push_back(std::unique_ptr<UnitTest>(new PureHwAudioTest));
    tests.push_back(std::unique_ptr<UnitTest>(new PureHwVideoTest));

    tests.push_back(std::unique_ptr<UnitTest>(new PureImageManagerTest));
    tests.push_back(std::unique_ptr<UnitTest>(new PureImageTest));
    tests.push_back(std::unique_ptr<UnitTest>(new PureTextureManagerTest));
    tests.push_back(std::unique_ptr<UnitTest>(new PureTextureTest));
    tests.push_back(std::unique_ptr<UnitTest>(new PureMaterialManagerTest));
    tests.push_back(std::unique_ptr<UnitTest>(new PureMaterialTest));

    tests.push_back(std::unique_ptr<UnitTest>(new PureCameraTest));

    tests.push_back(std::unique_ptr<UnitTest>(new PureMesh3DManagerTest));
    tests.push_back(std::unique_ptr<UnitTest>(new PureMesh3DTest));
    tests.push_back(std::unique_ptr<UnitTest>(new PureVertexTransferTest));
    tests.push_back(std::unique_ptr<UnitTest>(new PureObject3DManagerTest));
    tests.push_back(std::unique_ptr<UnitTest>(new PureObject3DTest));
     */
    
    /*   
    tests.push_back(std::unique_ptr<UnitTest>(new PureWindowTest));
    tests.push_back(std::unique_ptr<UnitTest>(new PureWindowTest2));
    */

    //tests.push_back(std::unique_ptr<UnitTest>(new PureRendererHWfixedPipeTest));
    
    /*  
    tests.push_back(std::unique_ptr<UnitTest>(new PR00FsUltimateRenderingEngineTest));
    tests.push_back(std::unique_ptr<UnitTest>(new PR00FsUltimateRenderingEngineTest2));
    */
    

    vector<std::unique_ptr<UnitTest>>::size_type nSucceededTests = 0;
    vector<std::unique_ptr<UnitTest>>::size_type nTotalSubTests = 0;
    vector<std::unique_ptr<UnitTest>>::size_type nTotalPassedSubTests = 0;
    for (vector<std::unique_ptr<UnitTest>>::size_type i = 0; i < tests.size(); ++i)
    {
        getConsole().OLn("Running test %d / %d ... ", i+1, tests.size());
        tests[i]->run();
    }

    // summarizing
    getConsole().OLn("");
    for (vector<std::unique_ptr<UnitTest>>::size_type i = 0; i < tests.size(); ++i)
    {
        if ( tests[i]->isPassed() )
        {
            ++nSucceededTests;
            getConsole().SOn();
            if ( tests[i]->getName().empty() )
            {
                getConsole().OLn("Test passed: %s(%d)!", tests[i]->getFile().c_str(), tests[i]->getSubTestCount());
            }
            else if ( tests[i]->getFile().empty() )
            {
                getConsole().OLn("Test passed: %s(%d)!", tests[i]->getName().c_str(), tests[i]->getSubTestCount());
            }
            else
            {
                getConsole().OLn("Test passed: %s(%d) in %s!", tests[i]->getName().c_str(), tests[i]->getSubTestCount(), tests[i]->getFile().c_str());
            }
            getConsole().SOff();
        }
        else
        {
            getConsole().EOn();
            if ( tests[i]->getName().empty() )
            {
                getConsole().OLn("Test failed: %s", tests[i]->getFile().c_str());
            }
            else if ( tests[i]->getFile().empty() )
            {
                getConsole().OLn("Test failed: %s", tests[i]->getName().c_str());
            }
            else
            {
                getConsole().OLn("Test failed: %s in %s", tests[i]->getName().c_str(), tests[i]->getFile().c_str());
            }
            getConsole().Indent();
            for (vector<string>::size_type j = 0; j < tests[i]->getMessages().size(); ++j)
            {
                getConsole().OLn("%s", tests[i]->getMessages()[j].c_str());
            }
            getConsole().Outdent();
            getConsole().EOff();
        }
        nTotalSubTests += tests[i]->getSubTestCount();
        nTotalPassedSubTests += tests[i]->getPassedSubTestCount();
    }

    getConsole().OLn("");
    getConsole().OLn("========================================================");
    if ( nSucceededTests == tests.size() )
    {
        getConsole().SOn();
    }
    else
    {
        getConsole().EOn();
    }
    getConsole().OLn("Passed tests: %d / %d (SubTests: %d / %d)", nSucceededTests, tests.size(), nTotalPassedSubTests, nTotalSubTests);
    getConsole().NOn();
    getConsole().OLn("========================================================");
    
    system("pause");

    getConsole().Deinitialize();

    return 0;

} // WinMain()