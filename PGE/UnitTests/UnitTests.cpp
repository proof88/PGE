/*
    ###############################################
    UnitTests.cpp
    Collection of unit tests for PGE
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ################################################
*/

#include "UnitTest.h"  // PCH

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#include <vector>

#include "../../../CConsole/CConsole/src/CConsole.h"


#include "PFLTest.h"
#include "PGEcfgVariableTest.h"
#include "PGESysCFGTest.h"
#include "PRREScreenTest.h"
#include "PRRESharedSettingsTest.h"
#include "PRREVectorTest.h"
#include "PRREPosUpTargetTest.h"
#include "PRREMatrixTest.h"
#include "PRRETransformMatrixTest.h"
#include "PRREColorTest.h"
#include "PRREBaseClassTest.h"
#include "PRREManagerTest.h"
#include "PRREManagedTest.h"
#include "PRREFiledManagerTest.h"
#include "PRREFiledManagedTest.h"
#include "PRREhwInfoTest.h"
#include "PRREhwCentralProcessorTest.h"
#include "PRREhwSystemMemoryTest.h"
#include "PRREhwVideoTest.h"
#include "PRREhwAudioTest.h"
#include "PRREImageManagerTest.h"
#include "PRREImageTest.h"
#include "PRRETextureManagerTest.h"
#include "PRRETextureTest.h"
#include "PRREMaterialManagerTest.h"
#include "PRREMaterialTest.h"
#include "PRRECameraTest.h"
#include "PRREObject3DManagerTest.h"
#include "PRREObject3DTest.h"
#include "PRREWindowTest.h"
#include "PRREWindowTest2.h"
#include "PRREScissorTest.h"
#include "PR00FsReducedRenderingEngineTest.h"
#include "PR00FsReducedRenderingEngineTest2.h"



#define CON_TITLE "Unit tests for PGE"

using namespace std;

#pragma warning(disable:4100)  /* unreferenced formal parameter */

/**
    Entry point for UnitTests.

    This directly uses WinAPI.
*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

    CConsole& con = CConsole::getConsoleInstance();
    con.Initialize(CON_TITLE, true);
    con.OLn(CON_TITLE);
    con.L();
    con.OLn("");

    PFLTest                    testPFL;
    PGEcfgVariableTest         testPGEcfgVariable;
    PGESysCFGTest              testPGESysCFG; 
    PRREScreenTest             testScreen;
    PRRESharedSettingsTest     testSharedSettings;
    PRREVectorTest             testPRREVector;
    PRREPosUpTargetTest        testPosUpTarget;
    PRREMatrixTest             testPRREMatrix;
    PRRETransformMatrixTest    testPRRETransformMatrix;
    PRREScissorTest            testPRREScissor;
    PRREColorTest              testPRREColor;
    PRREBaseClassTest          testPRREBaseClass;
    PRREManagerTest            testPRREManager;
    PRREManagedTest            testPRREManaged;
    PRREFiledManagerTest       testPRREFiledManager;
    PRREFiledManagedTest       testPRREFiledManaged;
    PRREhwInfoTest             testPRREhwInfo;
    PRREhwCentralProcessorTest testPRREhwCentralProcessor;
    PRREhwSystemMemoryTest     testPRREhwSystemMemory;
    PRREhwAudioTest            testPRREhwAudio;
    PRREhwVideoTest            testPRREhwVideo;
    PRREImageManagerTest       testPRREImageManager;
    PRREImageTest              testPRREImage;
    PRRETextureManagerTest     testPRRETextureManager;
    PRRETextureTest            testPRRETexture; 
    PRREMaterialManagerTest    testPRREMaterialManager;
    PRREMaterialTest           testPRREMaterial;

    PRRECameraTest             testPRRECamera;
    PRREObject3DManagerTest    testPRREObject3DManager;
    PRREObject3DTest           testPRREObject3D;
    PRREWindowTest             testPRREWindow;
    PRREWindowTest2            testPRREWindow2;
    
    PR00FsReducedRenderingEngineTest  testPRRE;
    PR00FsReducedRenderingEngineTest2 testPRRE2;
    
    vector<UnitTest*> tests;
    
    
    tests.push_back(&testPFL);
    /*       
    tests.push_back(&testPGEcfgVariable);
    tests.push_back(&testPGESysCFG);
    */

    /*    
    tests.push_back(&testScreen);
    tests.push_back(&testSharedSettings);
    */
    
    /* */
    tests.push_back(&testPosUpTarget);
    tests.push_back(&testPRREVector);  
    tests.push_back(&testPRREMatrix);  
    tests.push_back(&testPRRETransformMatrix);
    tests.push_back(&testPRREScissor);
    
    /* */
    tests.push_back(&testPRREColor);
    tests.push_back(&testPRREBaseClass);
    tests.push_back(&testPRREManager);
    tests.push_back(&testPRREManaged);
    tests.push_back(&testPRREFiledManager);
    tests.push_back(&testPRREFiledManaged);
    
    tests.push_back(&testPRREhwInfo);
    tests.push_back(&testPRREhwCentralProcessor);
    tests.push_back(&testPRREhwSystemMemory);
    tests.push_back(&testPRREhwAudio);
    tests.push_back(&testPRREhwVideo); 

    /* */   
    tests.push_back(&testPRREImageManager);
    tests.push_back(&testPRREImage);
    tests.push_back(&testPRRETextureManager);
    tests.push_back(&testPRRETexture);   
    tests.push_back(&testPRREMaterialManager);
    tests.push_back(&testPRREMaterial);
    tests.push_back(&testPRRECamera);
    tests.push_back(&testPRREObject3DManager);
    tests.push_back(&testPRREObject3D);

    /*  
    tests.push_back(&testPRREWindow);
    tests.push_back(&testPRREWindow2);
    */

    /* 
    tests.push_back(&testPRRE);
    tests.push_back(&testPRRE2);
    */ 

    vector<UnitTest*>::size_type nSucceededTests = 0;
    vector<UnitTest*>::size_type nTotalSubTests = 0;
    vector<UnitTest*>::size_type nTotalPassedSubTests = 0;
    for (vector<UnitTest*>::size_type i = 0; i < tests.size(); ++i)
    {
        tests[i]->run();
    }

    con.OLn("");
    for (vector<UnitTest*>::size_type i = 0; i < tests.size(); ++i)
    {
        if ( tests[i]->isPassed() )
        {
            ++nSucceededTests;
            con.SOn();
            if ( tests[i]->getName().empty() )
            {
                con.OLn("Test passed: %s(%d)!", tests[i]->getFile().c_str(), tests[i]->getSubTestCount());
            }
            else if ( tests[i]->getFile().empty() )
            {
                con.OLn("Test passed: %s(%d)!", tests[i]->getName().c_str(), tests[i]->getSubTestCount());
            }
            else
            {
                con.OLn("Test passed: %s(%d) in %s!", tests[i]->getName().c_str(), tests[i]->getSubTestCount(), tests[i]->getFile().c_str());
            }
            con.SOff();
        }
        else
        {
            con.EOn();
            if ( tests[i]->getName().empty() )
            {
                con.OLn("Test failed: %s", tests[i]->getFile().c_str());
            }
            else if ( tests[i]->getFile().empty() )
            {
                con.OLn("Test failed: %s", tests[i]->getName().c_str());
            }
            else
            {
                con.OLn("Test failed: %s in %s", tests[i]->getName().c_str(), tests[i]->getFile().c_str());
            }
            con.Indent();
            for (vector<string>::size_type j = 0; j < tests[i]->getMessages().size(); ++j)
            {
                con.OLn("%s", tests[i]->getMessages()[j].c_str());
            }
            con.Outdent();
            con.EOff();
        }
        nTotalSubTests += tests[i]->getSubTestCount();
        nTotalPassedSubTests += tests[i]->getPassedSubTestCount();
    }

    con.OLn("");
    con.OLn("========================================================");
    if ( nSucceededTests == tests.size() )
    {
        con.SOn();
    }
    else
    {
        con.EOn();
    }
    con.OLn("Passed tests: %d / %d (SubTests: %d / %d)", nSucceededTests, tests.size(), nTotalPassedSubTests, nTotalSubTests);
    con.NOn();
    con.OLn("========================================================");
    
    system("pause");

    return 0;

} // WinMain()