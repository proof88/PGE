/*
    ###############################################
    UnitTests.cpp
    Collection of unit tests for PGE
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ################################################
*/

#include "UnitTest.h"  // PCH

#include <vector>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#include "../../../CConsole/CConsole/src/CConsole.h"

#include "PFLTest.h"
#include "PGEcfgVariableTest.h"
#include "PGEcfgFileTest.h"
#include "PGEcfgProfilesTest.h"
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

    PFLTest                    testPFL;
    PGEcfgVariableTest         testPGEcfgVariable;
    PGEcfgFileTest             testPGEcfgFile;
    PGEcfgProfilesTest              testPGESysCFG; 
    WeaponsTest                testWeapons;
    PGEBulletTest              testPGEBullet;
    
    PureScreenTest                  testScreen;
    PureSharedSettingsTest          testSharedSettings;

    PureVectorTest                  testPureVector;
    PurePosUpTargetTest             testPosUpTarget;
    PureMatrixTest                  testPureMatrix;
    PureTransformMatrixTest         testPureTransformMatrix;
    PureAxisAlignedBoundingBoxTest  testAABB;
    PureOctreeTest                  testOctree;
    PureBoundingVolumeHierarchyTest testBVH;
    PureScissorTest                 testPureScissor;
    PureColorTest                   testPureColor;
    PureManagerTest                 testPureManager;
    PureManagedTest                 testPureManaged;
    PureFiledManagerTest            testPureFiledManager;
    PureFiledManagedTest            testPureFiledManaged;
    PureHwInfoTest                  testPureHwInfo;
    PurehwCentralProcessorTest      testPurehwCentralProcessor;
    PureHwSystemMemoryTest          testPureHwSystemMemory;
    PureHwAudioTest                 testPureHwAudio;
    PureHwVideoTest                 testPureHwVideo;
    PureImageManagerTest            testPureImageManager;
    PureImageTest                   testPureImage;
    PureTextureManagerTest          testPureTextureManager;
    PureTextureTest                 testPureTexture; 
    PureMaterialManagerTest         testPureMaterialManager;
    PureMaterialTest                testPureMaterial;

    PureCameraTest                  testPureCamera;
    PureMesh3DManagerTest           testPureMesh3DManager;
    PureMesh3DTest                  testPureMesh3D;
    PureVertexTransferTest          testPureVertexTransfer;
    PureObject3DManagerTest         testPureObject3DManager;
    PureObject3DTest                testPureObject3D;
    PureWindowTest                  testPureWindow;
    PureWindowTest2                 testPureWindow2;

    PureRendererHWfixedPipeTest     testPureRendererHWfixedPipe;
    
    PR00FsUltimateRenderingEngineTest  testPure;
    PR00FsUltimateRenderingEngineTest2 testPure2;
    
    vector<UnitTest*> tests;
    
    
    //tests.push_back(&testPFL);
    
    tests.push_back(&testPGEcfgVariable);
    tests.push_back(&testPGEcfgFile);
    tests.push_back(&testPGESysCFG);
    
    tests.push_back(&testWeapons);
    //tests.push_back(&testPGEBullet);
    
    /*
    tests.push_back(&testScreen);
    tests.push_back(&testSharedSettings);
    */
    
    /*  
    tests.push_back(&testPosUpTarget);
    tests.push_back(&testPureVector);  
    tests.push_back(&testPureMatrix);  
    tests.push_back(&testPureTransformMatrix);
    tests.push_back(&testAABB);
    tests.push_back(&testOctree);
    tests.push_back(&testBVH);
    tests.push_back(&testPureScissor);
         
    tests.push_back(&testPureColor);
    tests.push_back(&testPureManager);
    tests.push_back(&testPureManaged);
    tests.push_back(&testPureFiledManager);
    tests.push_back(&testPureFiledManaged);
      
    tests.push_back(&testPureHwInfo);
    tests.push_back(&testPurehwCentralProcessor);
    tests.push_back(&testPureHwSystemMemory);
    tests.push_back(&testPureHwAudio);
    tests.push_back(&testPureHwVideo); 
    
    tests.push_back(&testPureImageManager);
    tests.push_back(&testPureImage); 
    tests.push_back(&testPureTextureManager);
    tests.push_back(&testPureTexture);
    tests.push_back(&testPureMaterialManager);
    tests.push_back(&testPureMaterial);
    
    tests.push_back(&testPureCamera);  
       
    tests.push_back(&testPureMesh3DManager);
    tests.push_back(&testPureMesh3D);
    
    tests.push_back(&testPureVertexTransfer);
    
    tests.push_back(&testPureObject3DManager);
    tests.push_back(&testPureObject3D);  
    */

    /* 
    tests.push_back(&testPureWindow);
    tests.push_back(&testPureWindow2);
    */
    
    //tests.push_back(&testPureRendererHWfixedPipe);
    
    /*
    tests.push_back(&testPure);
    tests.push_back(&testPure2);
    */
    
    vector<UnitTest*>::size_type nSucceededTests = 0;
    vector<UnitTest*>::size_type nTotalSubTests = 0;
    vector<UnitTest*>::size_type nTotalPassedSubTests = 0;
    for (vector<UnitTest*>::size_type i = 0; i < tests.size(); ++i)
    {
        getConsole().OLn("Running test %d / %d ... ", i+1, tests.size());
        tests[i]->run();
    }

    // summarizing
    getConsole().OLn("");
    for (vector<UnitTest*>::size_type i = 0; i < tests.size(); ++i)
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