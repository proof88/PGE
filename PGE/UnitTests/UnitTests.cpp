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
#include "PGESysCFGTest.h"
#include "WeaponsTest.h"
#include "PGEBulletTest.h"
#include "PR00FsReducedRenderingEngineTest.h"
#include "PR00FsReducedRenderingEngineTest2.h"
#include "PRREAxisAlignedBoundingBoxTest.h"
#include "PRREOctreeTest.h"
#include "PRREBoundingVolumeHierarchyTest.h"
#include "PRRECameraTest.h"
#include "PRREColorTest.h"
#include "PRREFiledManagedTest.h"
#include "PRREFiledManagerTest.h"
#include "PRREhwAudioTest.h"
#include "PRREhwCentralProcessorTest.h"
#include "PRREhwInfoTest.h"
#include "PRREhwSystemMemoryTest.h"
#include "PRREhwVideoTest.h"
#include "PRREImageTest.h"
#include "PRREImageManagerTest.h"
#include "PRREManagedTest.h"
#include "PRREManagerTest.h"
#include "PRREMaterialTest.h"
#include "PRREMaterialManagerTest.h"
#include "PRREMatrixTest.h"
#include "PRREMesh3DTest.h"
#include "PRREMesh3DManagerTest.h"
#include "PRREObject3DTest.h"
#include "PRREObject3DManagerTest.h"
#include "PRREPosUpTargetTest.h"
#include "PRREScissorTest.h"
#include "PRREScreenTest.h"
#include "PRRESharedSettingsTest.h"
#include "PRRETextureTest.h"
#include "PRRETextureManagerTest.h"
#include "PRRETransformMatrixTest.h"
#include "PRREVectorTest.h"
#include "PRREVertexTransferTest.h"
#include "PRREWindowTest.h"
#include "PRREWindowTest2.h"
#include "PRRERendererHWfixedPipeTest.h"

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
    PGESysCFGTest              testPGESysCFG; 
    WeaponsTest                testWeapons;
    PGEBulletTest              testPGEBullet;
    
    PRREScreenTest                  testScreen;
    PRRESharedSettingsTest          testSharedSettings;

    PRREVectorTest                  testPRREVector;
    PRREPosUpTargetTest             testPosUpTarget;
    PRREMatrixTest                  testPRREMatrix;
    PRRETransformMatrixTest         testPRRETransformMatrix;
    PRREAxisAlignedBoundingBoxTest  testAABB;
    PRREOctreeTest                  testOctree;
    PRREBoundingVolumeHierarchyTest testBVH;
    PRREScissorTest                 testPRREScissor;
    PRREColorTest                   testPRREColor;
    PRREManagerTest                 testPRREManager;
    PRREManagedTest                 testPRREManaged;
    PRREFiledManagerTest            testPRREFiledManager;
    PRREFiledManagedTest            testPRREFiledManaged;
    PRREhwInfoTest                  testPRREhwInfo;
    PRREhwCentralProcessorTest      testPRREhwCentralProcessor;
    PRREhwSystemMemoryTest          testPRREhwSystemMemory;
    PRREhwAudioTest                 testPRREhwAudio;
    PRREhwVideoTest                 testPRREhwVideo;
    PRREImageManagerTest            testPRREImageManager;
    PRREImageTest                   testPRREImage;
    PRRETextureManagerTest          testPRRETextureManager;
    PRRETextureTest                 testPRRETexture; 
    PRREMaterialManagerTest         testPRREMaterialManager;
    PRREMaterialTest                testPRREMaterial;

    PRRECameraTest                  testPRRECamera;
    PRREMesh3DManagerTest           testPRREMesh3DManager;
    PRREMesh3DTest                  testPRREMesh3D;
    PRREVertexTransferTest          testPRREVertexTransfer;
    PRREObject3DManagerTest         testPRREObject3DManager;
    PRREObject3DTest                testPRREObject3D;
    PRREWindowTest                  testPRREWindow;
    PRREWindowTest2                 testPRREWindow2;

    PRRERendererHWfixedPipeTest     testPRRERendererHWfixedPipe;
    
    PR00FsReducedRenderingEngineTest  testPRRE;
    PR00FsReducedRenderingEngineTest2 testPRRE2;
    
    vector<UnitTest*> tests;
    
    
    //tests.push_back(&testPFL);
    
    tests.push_back(&testPGEcfgVariable);
    tests.push_back(&testPGEcfgFile);
    tests.push_back(&testPGESysCFG);
    
    //tests.push_back(&testWeapons);
    //tests.push_back(&testPGEBullet);
    
    /*
    tests.push_back(&testScreen);
    tests.push_back(&testSharedSettings);
    */
    
    /*  
    tests.push_back(&testPosUpTarget);
    tests.push_back(&testPRREVector);  
    tests.push_back(&testPRREMatrix);  
    tests.push_back(&testPRRETransformMatrix);
    tests.push_back(&testAABB);
    tests.push_back(&testOctree);
    tests.push_back(&testBVH);
    tests.push_back(&testPRREScissor);
         
    tests.push_back(&testPRREColor);
    tests.push_back(&testPRREManager);
    tests.push_back(&testPRREManaged);
    tests.push_back(&testPRREFiledManager);
    tests.push_back(&testPRREFiledManaged);
      
    tests.push_back(&testPRREhwInfo);
    tests.push_back(&testPRREhwCentralProcessor);
    tests.push_back(&testPRREhwSystemMemory);
    tests.push_back(&testPRREhwAudio);
    tests.push_back(&testPRREhwVideo); 
    
    tests.push_back(&testPRREImageManager);
    tests.push_back(&testPRREImage); 
    tests.push_back(&testPRRETextureManager);
    tests.push_back(&testPRRETexture);
    tests.push_back(&testPRREMaterialManager);
    tests.push_back(&testPRREMaterial);
    
    tests.push_back(&testPRRECamera);  
       
    tests.push_back(&testPRREMesh3DManager);
    tests.push_back(&testPRREMesh3D);
    
    tests.push_back(&testPRREVertexTransfer);
    
    tests.push_back(&testPRREObject3DManager);
    tests.push_back(&testPRREObject3D);  
    */

    /* 
    tests.push_back(&testPRREWindow);
    tests.push_back(&testPRREWindow2);
    */
    
    //tests.push_back(&testPRRERendererHWfixedPipe);
    
    /*
    tests.push_back(&testPRRE);
    tests.push_back(&testPRRE2);
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