#pragma once

/*
    ###################################################################################
    PureImageTest.h
    Unit test for PureMesh3D.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../Pure/include/external/PR00FsReducedRenderingEngine.h"
#include "../Pure/include/external/Object3D/PureMesh3DManager.h"
#include "../Pure/include/internal/gl/GL.h" // should not include but we do this for getVertexIndicesType()

#ifndef E
#define E 0.0001f
#endif // EPSILON

#ifndef PureIMAGETEST_FILENAMES
#define PureIMAGETEST_FILENAMES
#define BMP128x128x32op "_res/proba128x128x32_opaque.bmp"
#define BMP128x128x32tr "_res/proba128x128x32_transparent.bmp"
#define BMP128x128x24 "_res/proba128x128x24.bmp"
#define BMP128x128x8 "_res/proba128x128x8.bmp"
#define BMP128x128x4 "_res/proba128x128x4.bmp"
#define BMP128x128x1 "_res/proba128x128x1.bmp"
#endif

class PureMesh3DTest :
    public UnitTest
{
public:

    PureMesh3DTest() :
        UnitTest( __FILE__ )
    {
        engine = NULL;
        mesh = NULL;
        meshFromFile = NULL;
        meshPlane = NULL;
        meshBox = NULL;
        meshCube = NULL;
    }

    virtual ~PureMesh3DTest()
    {
        Finalize();
    }

protected:

    virtual void Initialize()
    {
        //CConsole::getConsoleInstance().SetLoggingState(PureMesh3DManager::getLoggerModuleName(), true);
        //CConsole::getConsoleInstance().SetLoggingState(PureMesh3D::getLoggerModuleName(), true);
        engine = &PR00FsReducedRenderingEngine::createAndGet();
        engine->initialize(PURE_RENDERER_HW_FP, 800, 600, PURE_WINDOWED, 0, 32, 24, 0, 0);  // pretty standard display mode, should work on most systems
        mm = &engine->getMesh3DManager();
        mesh = NULL;
        meshFromFile = NULL;
        meshPlane = mm->createPlane(1.0f, 2.0f);
        meshBox   = mm->createBox(1.0f, 2.0f, 3.0f);
        meshCube  = mm->createCube(1.0f);

        AddSubTest("testCtor", (PFNUNITSUBTEST) &PureMesh3DTest::testCtor);
        AddSubTest("testDtor", (PFNUNITSUBTEST) &PureMesh3DTest::testDtor);
        AddSubTest("testGetName", (PFNUNITSUBTEST) &PureMesh3DTest::testGetName);
        AddSubTest("testIsLevel1", (PFNUNITSUBTEST) &PureMesh3DTest::testIsLevel1);
        AddSubTest("testIsLevel2", (PFNUNITSUBTEST) &PureMesh3DTest::testIsLevel2);
        AddSubTest("testGetPrimitiveFormat", (PFNUNITSUBTEST) &PureMesh3DTest::testGetPrimitiveFormat);
        AddSubTest("testGetVerticesCount", (PFNUNITSUBTEST) &PureMesh3DTest::testGetVerticesCount);
        AddSubTest("testGetVertices", (PFNUNITSUBTEST) &PureMesh3DTest::testGetVertices);
        AddSubTest("testGetVertexIndicesType", (PFNUNITSUBTEST) &PureMesh3DTest::testGetVertexIndicesType);
        AddSubTest("testGetMinVertexIndex", (PFNUNITSUBTEST) &PureMesh3DTest::testGetMinVertexIndex);
        AddSubTest("testGetMaxVertexIndex", (PFNUNITSUBTEST) &PureMesh3DTest::testGetMaxVertexIndex);
        AddSubTest("testGetVertexIndicesCount", (PFNUNITSUBTEST) &PureMesh3DTest::testGetVertexIndicesCount);
        AddSubTest("testGetVertexIndex", (PFNUNITSUBTEST) &PureMesh3DTest::testGetVertexIndex);
        AddSubTest("testGetNormals", (PFNUNITSUBTEST) &PureMesh3DTest::testGetNormals);
        AddSubTest("testGetFaceCount", (PFNUNITSUBTEST) &PureMesh3DTest::testGetFaceCount);
        AddSubTest("testGetTriangleCount", (PFNUNITSUBTEST) &PureMesh3DTest::testGetTriangleCount);
        AddSubTest("testGetPosVec", (PFNUNITSUBTEST) &PureMesh3DTest::testGetPosVec);
        AddSubTest("testGetRelPosVec", (PFNUNITSUBTEST) &PureMesh3DTest::testGetRelPosVec);
        AddSubTest("testGetSizeVec", (PFNUNITSUBTEST) &PureMesh3DTest::testGetSizeVec);
        AddSubTest("testRecalculateSize", (PFNUNITSUBTEST) &PureMesh3DTest::testRecalculateSize);
        AddSubTest("testGetMaterialNoTexture", (PFNUNITSUBTEST) &PureMesh3DTest::testGetMaterialNoTexture);
        AddSubTest("testGetMaterialSingleLayered", (PFNUNITSUBTEST) &PureMesh3DTest::testGetMaterialSingleLayered);
        AddSubTest("testGetMaterialMultiLayered", (PFNUNITSUBTEST) &PureMesh3DTest::testGetMaterialMultiLayered);
        AddSubTest("testGetUsedSystemMemory", (PFNUNITSUBTEST) &PureMesh3DTest::testGetUsedSystemMemory);

        // since Mesh3D became a Manager also, we should test these capabalities here as well for possible undocumented changed behavior
        AddSubTest("testGetCount", (PFNUNITSUBTEST) &PureMesh3DTest::testGetCount);
        AddSubTest("testIsEmpty", (PFNUNITSUBTEST) &PureMesh3DTest::testIsEmpty);
        AddSubTest("testGetSize", (PFNUNITSUBTEST) &PureMesh3DTest::testGetSize);
        AddSubTest("testGetAttachedAt", (PFNUNITSUBTEST) &PureMesh3DTest::testGetAttachedAt);
        AddSubTest("testPreAlloc", (PFNUNITSUBTEST) &PureMesh3DTest::testPreAlloc);
        AddSubTest("testHasAttached2", (PFNUNITSUBTEST) &PureMesh3DTest::testHasAttached2);
        AddSubTest("testAttach", (PFNUNITSUBTEST) &PureMesh3DTest::testAttach);
        AddSubTest("testDetach", (PFNUNITSUBTEST) &PureMesh3DTest::testDetach);
        AddSubTest("testDeleteAttachedInstance", (PFNUNITSUBTEST) &PureMesh3DTest::testDeleteAttachedInstance);
        AddSubTest("testDeleteAll", (PFNUNITSUBTEST) &PureMesh3DTest::testDeleteAll);
        AddSubTest("testWriteList", (PFNUNITSUBTEST) &PureMesh3DTest::testWriteList);

        // Material of any Mesh3D must have its utiliser set to the Mesh3D instance
        AddSubTest("testMaterialGetUtiliser", (PFNUNITSUBTEST) &PureMesh3DTest::testMaterialGetUtiliser);
    }

    virtual bool setUp()
    {
        mesh = mm->createBox(1.0f, 2.0f, 3.0f);
        meshFromFile = mm->createFromFile("_res/models/snail_proofps/snail.obj");
        return assertNotNull(mesh, "mesh = NULL" ) &
            assertNotNull(meshFromFile, "meshFromFile = NULL" ) &
            assertNotNull(meshPlane, "meshPlane = NULL" ) &
            assertNotNull(meshBox, "meshBox = NULL" ) &
            assertNotNull(meshCube, "meshCube = NULL" );
    }

    virtual void TearDown()
    {
        if ( mesh != PGENULL )
        {
            delete mesh;
            mesh = NULL;
        }
        if ( meshFromFile != PGENULL )
        {
            delete meshFromFile;
            meshFromFile = NULL;
        }
    }

    virtual void Finalize()
    {
        mesh = NULL;
        meshFromFile = NULL;
        mm = NULL;

        if ( engine )
        {
            engine->shutdown();
            engine = NULL;
        }
        CConsole::getConsoleInstance().SetLoggingState(PureMesh3DManager::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(PureMesh3D::getLoggerModuleName(), false);
    }

private:
    PR00FsReducedRenderingEngine* engine;
    PureMesh3DManager* mm;
    PureMesh3D* mesh;
    PureMesh3D* meshFromFile;
    const PureMesh3D* meshPlane,
                      *meshBox,
                      *meshCube;

    // ---------------------------------------------------------------------------

    PureMesh3DTest(const PureMesh3DTest&)
    {};         

    PureMesh3DTest& operator=(const PureMesh3DTest&)
    {
        return *this;
    };

    bool testCtor()
    {
        return true;
    }

    bool testGetName()
    {
        return assertNotEquals(std::string::npos, meshFromFile->getName().find("Mesh3D "), "meshFromFile name") &
            assertNotEquals(std::string::npos, mesh->getName().find("Mesh3D "), "mesh name") &
            assertNotEquals(std::string::npos, meshPlane->getName().find("Mesh3D "), "meshPlane name") &
            assertNotEquals(std::string::npos, meshBox->getName().find("Mesh3D "), "meshBox name") &
            assertNotEquals(std::string::npos, meshCube->getName().find("Mesh3D "), "meshCube name") &
            assertNotEquals(std::string::npos, meshPlane->getName().find("Mesh3D "), "meshPlane name");
    }

    bool testDtor()
    {
        delete mesh;
        mesh = NULL;
        return assertEquals(4, mm->getCount());
    }

    bool testIsLevel1()
    {
        return assertTrue(meshFromFile->isLevel1(), "meshFromFile") &
            assertTrue(mesh->isLevel1(), "mesh") &
            assertTrue(meshPlane->isLevel1(), "plane") &
            assertTrue(meshBox->isLevel1(), "box") &
            assertTrue(meshCube->isLevel1(), "cube") &
            assertFalse(((PureMesh3D*)meshFromFile->getAttachedAt(0))->isLevel1(), "meshFromFile sub") &
            assertFalse(((PureMesh3D*)mesh->getAttachedAt(0))->isLevel1(), "mesh sub") &
            assertFalse(((PureMesh3D*)meshPlane->getAttachedAt(0))->isLevel1(), "plane sub") &
            assertFalse(((PureMesh3D*)meshBox->getAttachedAt(0))->isLevel1(), "box sub") &
            assertFalse(((PureMesh3D*)meshCube->getAttachedAt(0))->isLevel1(), "cube sub");
    }

    bool testIsLevel2()
    {
        return assertFalse(meshFromFile->isLevel2(), "meshFromFile") &
            assertFalse(mesh->isLevel2(), "mesh") &
            assertFalse(meshPlane->isLevel2(), "plane") &
            assertFalse(meshBox->isLevel2(), "box") &
            assertFalse(meshCube->isLevel2(), "cube") &
            assertTrue(((PureMesh3D*)meshFromFile->getAttachedAt(0))->isLevel2(), "meshFromFile sub") &
            assertTrue(((PureMesh3D*)mesh->getAttachedAt(0))->isLevel2(), "mesh sub") &
            assertTrue(((PureMesh3D*)meshPlane->getAttachedAt(0))->isLevel2(), "plane sub") &
            assertTrue(((PureMesh3D*)meshBox->getAttachedAt(0))->isLevel2(), "box sub") &
            assertTrue(((PureMesh3D*)meshCube->getAttachedAt(0))->isLevel2(), "cube sub");
    }

    bool testGetPrimitiveFormat()
    {
        return assertEquals(PURE_PM_TRIANGLES, meshFromFile->getPrimitiveFormat(), "meshFromFile") &
            assertEquals(PURE_PM_QUADS, mesh->getPrimitiveFormat(), "mesh") &
            assertEquals(PURE_PM_QUADS, meshPlane->getPrimitiveFormat(), "plane") &
            assertEquals(PURE_PM_QUADS, meshBox->getPrimitiveFormat(), "box") &
            assertEquals(PURE_PM_QUADS, meshCube->getPrimitiveFormat(), "cube");
    }

    bool testGetVerticesCount()
    {
        TPureUInt nVerticesCount = 0;
        for (TPureInt i = 0; i < meshFromFile->getCount(); i++)
        {
            nVerticesCount += ((PureMesh3D*)meshFromFile->getAttachedAt(i))->getVerticesCount();
        }

        return assertEquals((TPureUInt) 4, meshPlane->getVerticesCount(), "plane") &
            assertEquals((TPureUInt) 24, meshBox->getVerticesCount(), "box") &
            assertEquals((TPureUInt) 24, meshCube->getVerticesCount(), "cube") &
            assertEquals(nVerticesCount, meshFromFile->getVerticesCount(), "meshFromFile") &
            assertEquals(((PureMesh3D*)meshPlane->getAttachedAt(0))->getVerticesCount(), meshPlane->getVerticesCount(), "plane 2") &
            assertEquals(((PureMesh3D*)meshBox->getAttachedAt(0))->getVerticesCount(), meshBox->getVerticesCount(), "box 2") &
            assertEquals(((PureMesh3D*)meshCube->getAttachedAt(0))->getVerticesCount(), meshCube->getVerticesCount(), "cube 2") &
            assertEquals((TPureUInt)144, ((PureMesh3D*)meshFromFile->getAttachedAt(0))->getVerticesCount(), "meshFromFile 3");
    }

    bool testGetVertices()
    {
        return assertNotNull(meshPlane->getVertices(), "plane") &
            assertNotNull(meshBox->getVertices(), "box") &
            assertNotNull(meshCube->getVertices(), "cube") &
            assertNull(meshFromFile->getVertices(), "meshFromFile") &
            assertEquals(((PureMesh3D*)meshPlane->getAttachedAt(0))->getVertices(), meshPlane->getVertices(), "plane 2") &
            assertEquals(((PureMesh3D*)meshBox->getAttachedAt(0))->getVertices(), meshBox->getVertices(), "box 2") &
            assertEquals(((PureMesh3D*)meshCube->getAttachedAt(0))->getVertices(), meshCube->getVertices(), "cube 2") &
            assertNotEquals(((PureMesh3D*)meshFromFile->getAttachedAt(0))->getVertices(), meshFromFile->getVertices(), "meshFromFile 2") &
            assertNull(meshPlane->getVertices(false), "plane noimplicit") &
            assertNull(meshBox->getVertices(false), "box noimplicit") &
            assertNull(meshCube->getVertices(false), "cube noimplicit") &
            assertNull(meshFromFile->getVertices(false), "meshFromFile noimplicit");
    }

    bool testGetVertexIndicesType()
    {
        return assertEquals((unsigned int)GL_UNSIGNED_BYTE, meshPlane->getVertexIndicesType(), "plane") &
            assertEquals((unsigned int)GL_UNSIGNED_BYTE, meshBox->getVertexIndicesType(), "box") &
            assertEquals((unsigned int)GL_UNSIGNED_BYTE, meshCube->getVertexIndicesType(), "cube") &
            /* intentionally comparing level-1 parent vs level-2 submesh vertex index types since they must be equal anyway */
            assertEquals(((PureMesh3D*)meshPlane->getAttachedAt(0))->getVertexIndicesType(), meshPlane->getVertexIndicesType(false), "plane noimplicit") &
            assertEquals(((PureMesh3D*)meshBox->getAttachedAt(0))->getVertexIndicesType(), meshBox->getVertexIndicesType(false), "box noimplicit") &
            assertEquals(((PureMesh3D*)meshCube->getAttachedAt(0))->getVertexIndicesType(), meshCube->getVertexIndicesType(false), "cube noimplicit") &
            assertEquals((unsigned int)GL_UNSIGNED_BYTE, ((PureMesh3D*)meshFromFile->getAttachedAt(0))->getVertexIndicesType(), "meshFromFile noimplicit");
    }

    bool testGetMinVertexIndex()
    {
        return assertEquals((TPureUInt)0, meshPlane->getMinVertexIndex(), "plane") &
            assertEquals((TPureUInt)0, meshBox->getMinVertexIndex(), "box") &
            assertEquals((TPureUInt)0, meshCube->getMinVertexIndex(), "cube") &
            assertEquals((TPureUInt)UINT_MAX, meshFromFile->getMinVertexIndex(), "meshFromFile") &
            /* doesn't matter if implicit access or not, if there is vertex or not, 0 is the expected anyway */
            assertEquals(((PureMesh3D*)meshPlane->getAttachedAt(0))->getMinVertexIndex(), meshPlane->getMinVertexIndex(false), "plane sub noimplicit") &
            assertEquals(((PureMesh3D*)meshBox->getAttachedAt(0))->getMinVertexIndex(), meshBox->getMinVertexIndex(false), "box sub noimplicit") &
            assertEquals(((PureMesh3D*)meshCube->getAttachedAt(0))->getMinVertexIndex(), meshCube->getMinVertexIndex(false), "cube sub noimplicit") &
            assertNotEquals(((PureMesh3D*)meshFromFile->getAttachedAt(0))->getMinVertexIndex(), meshFromFile->getMinVertexIndex(false), "meshFromFile sub noimplicit") &
            assertEquals(((PureMesh3D*)meshPlane->getAttachedAt(0))->getMinVertexIndex(), meshPlane->getMinVertexIndex(), "plane sub") &
            assertEquals(((PureMesh3D*)meshBox->getAttachedAt(0))->getMinVertexIndex(), meshBox->getMinVertexIndex(), "box sub") &
            assertEquals(((PureMesh3D*)meshCube->getAttachedAt(0))->getMinVertexIndex(), meshCube->getMinVertexIndex(), "cube sub") &
            assertEquals((TPureUInt) 0, ((PureMesh3D*)meshFromFile->getAttachedAt(0))->getMinVertexIndex(), "meshFromFile sub");
    }

    bool testGetMaxVertexIndex()
    {
        return assertEquals((TPureUInt)3, meshPlane->getMaxVertexIndex(), "plane") &
            assertEquals((TPureUInt)23, meshBox->getMaxVertexIndex(), "box") &
            assertEquals((TPureUInt)23, meshCube->getMaxVertexIndex(), "cube") &
            assertEquals((TPureUInt)0, meshFromFile->getMaxVertexIndex(), "meshFromFile") &
            assertEquals((TPureUInt)0, meshPlane->getMaxVertexIndex(false), "plane noimplicit") &
            assertEquals((TPureUInt)0, meshBox->getMaxVertexIndex(false), "box noimplicit") &
            assertEquals((TPureUInt)0, meshCube->getMaxVertexIndex(false), "cube noimplicit") &
            assertEquals((TPureUInt)0, meshFromFile->getMaxVertexIndex(false), "meshFromFile noimplicit") &
            assertEquals((TPureUInt)3, ((PureMesh3D*)meshPlane->getAttachedAt(0))->getMaxVertexIndex(), "plane sub") &
            assertEquals((TPureUInt)23, ((PureMesh3D*)meshBox->getAttachedAt(0))->getMaxVertexIndex(), "box sub") &
            assertEquals((TPureUInt)23, ((PureMesh3D*)meshCube->getAttachedAt(0))->getMaxVertexIndex(), "cube sub") &
            assertEquals((TPureUInt)143, ((PureMesh3D*)meshFromFile->getAttachedAt(0))->getMaxVertexIndex(), "meshFromFile sub");
    }

     bool testGetVertexIndicesCount()
    {
        TPureUInt nVertexIndicesCount = 0;
        for (TPureInt i = 0; i < meshFromFile->getCount(); i++)
        {
            nVertexIndicesCount += ((PureMesh3D*)meshFromFile->getAttachedAt(i))->getVertexIndicesCount();
        }

        return assertEquals((TPureUInt) 4, meshPlane->getVertexIndicesCount(), "plane") &
            assertEquals((TPureUInt) 24, meshBox->getVertexIndicesCount(), "box") &
            assertEquals((TPureUInt) 24, meshCube->getVertexIndicesCount(), "cube") &
            assertEquals(nVertexIndicesCount, meshFromFile->getVertexIndicesCount(), "meshFromFile") &
            assertEquals(((PureMesh3D*)meshPlane->getAttachedAt(0))->getVertexIndicesCount(), meshPlane->getVertexIndicesCount(), "plane 2") &
            assertEquals(((PureMesh3D*)meshBox->getAttachedAt(0))->getVertexIndicesCount(), meshBox->getVertexIndicesCount(), "box 2") &
            assertEquals(((PureMesh3D*)meshCube->getAttachedAt(0))->getVertexIndicesCount(), meshCube->getVertexIndicesCount(), "cube 2") &
            assertEquals((TPureUInt)144, ((PureMesh3D*)meshFromFile->getAttachedAt(0))->getVertexIndicesCount(), "meshFromFile 3");
    }

    bool testGetVertexIndex()
    {
        bool b = true;
        
        const PureMesh3D& submeshPlane = * ((PureMesh3D*) (meshPlane->getAttachedAt(0)));
        for (TPureUInt i = 0; i < submeshPlane.getVertexIndicesCount(); i++)
        {
            b &= assertEquals(i, submeshPlane.getVertexIndex(i),
            (std::string("plane sub ") + std::to_string(i)).c_str());
            b &= assertEquals(i, meshPlane->getVertexIndex(i),
            (std::string("plane implicit ") + std::to_string(i)).c_str());
            // here explicit should return 0 since meshPlane is level-1 mesh without own geometry
            b &= assertEquals((TPureUInt)0, meshPlane->getVertexIndex(i, false),
            (std::string("plane explicit ") + std::to_string(i)).c_str());
        }

        const PureMesh3D& submeshBox = * ((PureMesh3D*) (meshBox->getAttachedAt(0)));
        for (TPureUInt i = 0; i < submeshBox.getVertexIndicesCount(); i++)
        {
            b &= assertEquals(i, submeshBox.getVertexIndex(i),
            (std::string("box sub ") + std::to_string(i)).c_str());
            b &= assertEquals(i, meshBox->getVertexIndex(i),
            (std::string("box implicit ") + std::to_string(i)).c_str());
            // here explicit should return 0 since meshBox is level-1 mesh without own geometry
            b &= assertEquals((TPureUInt)0, meshBox->getVertexIndex(i, false),
            (std::string("box explicit ") + std::to_string(i)).c_str());
        }

        const PureMesh3D& submeshCube = * ((PureMesh3D*) (meshCube->getAttachedAt(0)));
        for (TPureUInt i = 0; i < submeshCube.getVertexIndicesCount(); i++)
        {
            b &= assertEquals(i, submeshCube.getVertexIndex(i),
            (std::string("cube sub ") + std::to_string(i)).c_str());
            b &= assertEquals(i, meshCube->getVertexIndex(i),
            (std::string("cube implicit ") + std::to_string(i)).c_str());
            // here explicit should return 0 since meshCube is level-1 mesh without own geometry
            b &= assertEquals((TPureUInt)0, meshCube->getVertexIndex(i, false),
            (std::string("cube explicit ") + std::to_string(i)).c_str());
        }

        const PureMesh3D& submeshFile = * ((PureMesh3D*) (meshFromFile->getAttachedAt(0)));
        for (TPureUInt i = 0; i < submeshFile.getVertexIndicesCount(); i++)
        {
            b &= assertEquals(i, submeshFile.getVertexIndex(i),
            (std::string("meshFromFile sub ") + std::to_string(i)).c_str());
            // here implicit should return 0 since submesh is not selected implicitly for multi-submesh objects!
            b &= assertEquals((TPureUInt)0, meshFromFile->getVertexIndex(i),
            (std::string("meshFromFile implicit ") + std::to_string(i)).c_str());
            // here explicit should return 0 since meshFromFile is level-1 mesh without own geometry
            b &= assertEquals((TPureUInt)0, meshFromFile->getVertexIndex(i, false),
            (std::string("meshFromFile explicit ") + std::to_string(i)).c_str());
        }

        return b;
    }

    bool testGetNormals()
    {
        return assertNotNull(meshPlane->getNormals(), "plane") &
            assertNotNull(meshBox->getNormals(), "box") &
            assertNotNull(meshCube->getNormals(), "cube") &
            assertNull(meshFromFile->getNormals(), "meshFromFile") &
            assertEquals(((PureMesh3D*)meshPlane->getAttachedAt(0))->getNormals(), meshPlane->getNormals(), "plane 2") &
            assertEquals(((PureMesh3D*)meshBox->getAttachedAt(0))->getNormals(), meshBox->getNormals(), "box 2") &
            assertEquals(((PureMesh3D*)meshCube->getAttachedAt(0))->getNormals(), meshCube->getNormals(), "cube 2") &
            assertNotEquals(((PureMesh3D*)meshFromFile->getAttachedAt(0))->getNormals(), meshFromFile->getNormals(), "meshFromFile 2") &
            assertNull(meshPlane->getNormals(false), "plane noimplicit") &
            assertNull(meshBox->getNormals(false), "box noimplicit") &
            assertNull(meshCube->getNormals(false), "cube noimplicit") &
            assertNull(meshFromFile->getNormals(false), "meshFromFile noimplicit");
    }

    bool testGetFaceCount()
    {
        return assertEquals((TPureUInt)6, mesh->getFaceCount(), "mesh") &
            assertEquals((TPureUInt)728, meshFromFile->getFaceCount(), "meshFromFile");
    }

    bool testGetTriangleCount()
    {
        return assertEquals((TPureUInt)12, mesh->getTriangleCount(), "mesh") &
            assertEquals((TPureUInt)728, meshFromFile->getTriangleCount(), "meshFromFile");
    }

    bool testGetPosVec()
    {
        // in our loaded object, there shouldn't be any subobject in pos(0,0,0)
        bool b1 = true;
        for (TPureInt i = 0; i < meshFromFile->getCount(); i++)
            b1 = b1 & assertFalse( ((PureMesh3D*)meshFromFile->getAttachedAt(i))->getPosVec().isZero(), "pos zero mesh sub" );

        // in our other objects, all subobjects should be at pos(0,0,0)
        bool b2 = true;
        for (TPureInt i = 0; i < meshPlane->getCount(); i++)
            b2 = b2 & assertTrue( ((PureMesh3D*)meshPlane->getAttachedAt(i))->getPosVec().isZero(), "pos zero plane sub" );

        bool b3 = true;
        for (TPureInt i = 0; i < meshBox->getCount(); i++)
            b3 = b3 & assertTrue( ((PureMesh3D*)meshBox->getAttachedAt(i))->getPosVec().isZero(), "pos zero box sub" );

        bool b4 = true;
        for (TPureInt i = 0; i < meshCube->getCount(); i++)
            b4 = b4 & assertTrue( ((PureMesh3D*)meshCube->getAttachedAt(i))->getPosVec().isZero(), "pos zero cube sub" );

        return assertEquals(0.0f, meshFromFile->getPosVec().getX(), E, "meshFromFile x") &        
            assertEquals(0.0f, meshFromFile->getPosVec().getY(), E, "meshFromFile y") &
            assertEquals(0.0f, meshFromFile->getPosVec().getZ(), E, "meshFromFile z") &
            assertEquals(0.0f, meshPlane->getPosVec().getX(), E, "plane x") &        
            assertEquals(0.0f, meshPlane->getPosVec().getY(), E, "plane y") &
            assertEquals(0.0f, meshPlane->getPosVec().getZ(), E, "plane z") &
            assertEquals(0.0f, meshBox->getPosVec().getX(), E, "box x") &        
            assertEquals(0.0f, meshBox->getPosVec().getY(), E, "box y") &
            assertEquals(0.0f, meshBox->getPosVec().getZ(), E, "box z") &
            assertEquals(0.0f, meshCube->getPosVec().getX(), E, "cube x") &        
            assertEquals(0.0f, meshCube->getPosVec().getY(), E, "cube y") &
            assertEquals(0.0f, meshCube->getPosVec().getZ(), E, "cube z") &
            assertTrue(b1 & b2 & b3 & b4, "subobject position");
    }

    bool testGetRelPosVec()
    {
        // all subobjects should have their relpos as [0,0,0] since their position is calculated from their vertices' position and size
        bool b1 = true;
        for (TPureInt i = 0; i < meshFromFile->getCount(); i++)
            b1 = b1 & assertTrue( ((PureMesh3D*)meshFromFile->getAttachedAt(i))->getRelPosVec().isZero(), "relpos zero mesh sub" );

        // in other objects, this is also valid!
        bool b2 = true;
        for (TPureInt i = 0; i < meshPlane->getCount(); i++)
            b2 = b2 & assertTrue( ((PureMesh3D*)meshPlane->getAttachedAt(i))->getRelPosVec().isZero(), "relpos zero plane sub" );

        bool b3 = true;
        for (TPureInt i = 0; i < meshBox->getCount(); i++)
            b3 = b3 & assertTrue( ((PureMesh3D*)meshBox->getAttachedAt(i))->getRelPosVec().isZero(), "relpos zero box sub" );

        bool b4 = true;
        for (TPureInt i = 0; i < meshCube->getCount(); i++)
            b4 = b4 & assertTrue( ((PureMesh3D*)meshCube->getAttachedAt(i))->getRelPosVec().isZero(), "relpos zero cube sub" );

        return assertEquals(0.0f, meshFromFile->getRelPosVec().getX(), E, "meshFromFile x") &        
            assertEquals(0.0f, meshFromFile->getRelPosVec().getY(), E, "meshFromFile y") &
            assertEquals(0.0f, meshFromFile->getRelPosVec().getZ(), E, "meshFromFile z") &
            assertEquals(0.0f, meshPlane->getRelPosVec().getX(), E, "plane x") &        
            assertEquals(0.0f, meshPlane->getRelPosVec().getY(), E, "plane y") &
            assertEquals(0.0f, meshPlane->getRelPosVec().getZ(), E, "plane z") &
            assertEquals(0.0f, meshBox->getRelPosVec().getX(), E, "box x") &        
            assertEquals(0.0f, meshBox->getRelPosVec().getY(), E, "box y") &
            assertEquals(0.0f, meshBox->getRelPosVec().getZ(), E, "box z") &
            assertEquals(0.0f, meshCube->getRelPosVec().getX(), E, "cube x") &        
            assertEquals(0.0f, meshCube->getRelPosVec().getY(), E, "cube y") &
            assertEquals(0.0f, meshCube->getRelPosVec().getZ(), E, "cube z") &
            assertTrue(b1 & b2 & b3 & b4, "subobject relative position");
    }

    bool testGetSizeVec()
    {
        // in our loaded object, there shouldn't be any subobject with size(0,0,0)
        bool b1 = true;
        for (TPureInt i = 0; i < meshFromFile->getCount(); i++)
            b1 = b1 & assertFalse( ((PureMesh3D*)meshFromFile->getAttachedAt(i))->getSizeVec().isZero(), "size zero mesh sub" );

        // in our other objects, all subobjects size should be same as object size
        bool b2 = true;
        for (TPureInt i = 0; i < meshPlane->getCount(); i++)
            b2 = b2 & assertTrue( ((PureMesh3D*)meshPlane->getAttachedAt(i))->getSizeVec() == meshPlane->getSizeVec(), "size plane sub" );

        bool b3 = true;
        for (TPureInt i = 0; i < meshBox->getCount(); i++)
            b3 = b3 & assertTrue( ((PureMesh3D*)meshBox->getAttachedAt(i))->getSizeVec() == meshBox->getSizeVec(), "size box sub" );

        bool b4 = true;
        for (TPureInt i = 0; i < meshCube->getCount(); i++)
            b4 = b4 & assertTrue( ((PureMesh3D*)meshCube->getAttachedAt(i))->getSizeVec() == meshCube->getSizeVec(), "size cube sub" );

        return assertEquals(1.0f, meshPlane->getSizeVec().getX(), E, "plane x") &        
            assertEquals(2.0f, meshPlane->getSizeVec().getY(), E, "plane y") &
            assertEquals(0.0f, meshPlane->getSizeVec().getZ(), E, "plane z") &
            assertEquals(1.0f, meshBox->getSizeVec().getX(), E, "box x") &        
            assertEquals(2.0f, meshBox->getSizeVec().getY(), E, "box y") &
            assertEquals(3.0f, meshBox->getSizeVec().getZ(), E, "box z") &
            assertEquals(1.0f, meshCube->getSizeVec().getX(), E, "cube x") &        
            assertEquals(1.0f, meshCube->getSizeVec().getY(), E, "cube y") &
            assertEquals(1.0f, meshCube->getSizeVec().getZ(), E, "cube z") &
            assertTrue(b1 & b2 & b3 & b4, "subobject size");;
    }

    bool testRecalculateSize()
    {
        const PureVector vOld = mesh->getSizeVec();
        mesh->RecalculateSize();

        return assertTrue(vOld == mesh->getSizeVec());
    }

    bool testGetMaterialNoTexture()
    {
        return assertEquals((TPureUByte) 255, mesh->getMaterial().getTextureEnvColor().getRed(), "env mesh r") &
            assertEquals((TPureUByte) 255, mesh->getMaterial().getTextureEnvColor().getGreen(), "env mesh g") &
            assertEquals((TPureUByte) 255, mesh->getMaterial().getTextureEnvColor().getBlue(), "env mesh b") &
            assertEquals((TPureUByte) 255, mesh->getMaterial().getTextureEnvColor().getAlpha(), "env mesh a") &
            assertEquals((TPureUByte) 255, meshPlane->getMaterial().getTextureEnvColor().getRed(), "env plane r") &
            assertEquals((TPureUByte) 255, meshPlane->getMaterial().getTextureEnvColor().getGreen(), "env plane g") &
            assertEquals((TPureUByte) 255, meshPlane->getMaterial().getTextureEnvColor().getBlue(), "env plane b") &
            assertEquals((TPureUByte) 255, meshPlane->getMaterial().getTextureEnvColor().getAlpha(), "env plane a") &
            assertEquals((TPureUByte) 255, meshBox->getMaterial().getTextureEnvColor().getRed(), "env box r") &
            assertEquals((TPureUByte) 255, meshBox->getMaterial().getTextureEnvColor().getGreen(), "env box g") &
            assertEquals((TPureUByte) 255, meshBox->getMaterial().getTextureEnvColor().getBlue(), "env box b") &
            assertEquals((TPureUByte) 255, meshBox->getMaterial().getTextureEnvColor().getAlpha(), "env box a") &
            assertEquals((TPureUByte) 255, meshCube->getMaterial().getTextureEnvColor().getRed(), "env cube r") &
            assertEquals((TPureUByte) 255, meshCube->getMaterial().getTextureEnvColor().getGreen(), "env cube g") &
            assertEquals((TPureUByte) 255, meshCube->getMaterial().getTextureEnvColor().getBlue(), "env cube b") &
            assertEquals((TPureUByte) 255, meshCube->getMaterial().getTextureEnvColor().getAlpha(), "env cube a") &
            assertNull(mesh->getMaterial().getTexture(), "tex mesh") &
            assertNull(meshPlane->getMaterial().getTexture(), "tex plane") &
            assertNull(meshBox->getMaterial().getTexture(), "tex box") &
            assertNull(meshCube->getMaterial().getTexture(), "tex cube") &
            assertFalse(mesh->getMaterial().isTextured(), "isTex mesh") &
            assertFalse(meshPlane->getMaterial().isTextured(), "isTex plane") &
            assertFalse(meshBox->getMaterial().isTextured(), "isTex box") &
            assertFalse(meshCube->getMaterial().isTextured(), "isTex cube") &
            assertFalse(mesh->getMaterial().isSingleTextured(), "isTexSingle mesh") &
            assertFalse(meshPlane->getMaterial().isSingleTextured(), "isTexSingle plane") &
            assertFalse(meshBox->getMaterial().isSingleTextured(), "isTexSingle box") &
            assertFalse(meshCube->getMaterial().isSingleTextured(), "isTexSingle cube") &
            assertFalse(mesh->getMaterial().isMultiTextured(), "isTexMulti mesh") &
            assertFalse(meshPlane->getMaterial().isMultiTextured(), "isTexMulti plane") &
            assertFalse(meshBox->getMaterial().isMultiTextured(), "isTexMulti box") &
            assertFalse(meshCube->getMaterial().isMultiTextured(), "isTexMulti cube") &
            assertEquals((TPureUInt) 4, meshPlane->getMaterial().getTexcoordsCount(), "texcoordcount plane") &
            assertEquals((TPureUInt) 24, meshBox->getMaterial().getTexcoordsCount(), "texcoordcount box") &
            assertEquals((TPureUInt) 24, meshCube->getMaterial().getTexcoordsCount(), "texcoordcount cube") &
            assertEquals(((PureMesh3D*)meshPlane->getAttachedAt(0))->getMaterial().getTexcoordsCount(), meshPlane->getMaterial().getTexcoordsCount(), "texcoordcount plane 2") &
            assertEquals(((PureMesh3D*)meshBox->getAttachedAt(0))->getMaterial().getTexcoordsCount(), meshBox->getMaterial().getTexcoordsCount(), "texcoordcount box 2") &
            assertEquals(((PureMesh3D*)meshCube->getAttachedAt(0))->getMaterial().getTexcoordsCount(), meshCube->getMaterial().getTexcoordsCount(), "texcoordcount cube 2") &
            assertNotNull(meshPlane->getMaterial().getTexcoords(), "texcoords plane") &
            assertNotNull(meshBox->getMaterial().getTexcoords(), "texcoords box") &
            assertNotNull(meshCube->getMaterial().getTexcoords(), "texcoords cube") &
            assertEquals(((PureMesh3D*)meshPlane->getAttachedAt(0))->getMaterial().getTexcoords(), meshPlane->getMaterial().getTexcoords(), "texcoords plane 2") &
            assertEquals(((PureMesh3D*)meshBox->getAttachedAt(0))->getMaterial().getTexcoords(), meshBox->getMaterial().getTexcoords(), "texcoords box 2") &
            assertEquals(((PureMesh3D*)meshCube->getAttachedAt(0))->getMaterial().getTexcoords(), meshCube->getMaterial().getTexcoords(), "texcoords cube 2") &
            assertEquals((TPureUInt) 4, meshPlane->getMaterial().getColorsCount(), "colorscount plane") &
            assertEquals((TPureUInt) 24, meshBox->getMaterial().getColorsCount(), "colorscount box") &
            assertEquals((TPureUInt) 24, meshCube->getMaterial().getColorsCount(), "colorscount cube") &
            assertEquals(((PureMesh3D*)meshPlane->getAttachedAt(0))->getMaterial().getColorsCount(), meshPlane->getMaterial().getColorsCount(), "colorscount plane 2") &
            assertEquals(((PureMesh3D*)meshBox->getAttachedAt(0))->getMaterial().getColorsCount(), meshBox->getMaterial().getColorsCount(), "colorscount box 2") &
            assertEquals(((PureMesh3D*)meshCube->getAttachedAt(0))->getMaterial().getColorsCount(), meshCube->getMaterial().getColorsCount(), "colorscount cube 2") &
            assertEquals((TPureUInt) 4, meshPlane->getMaterial().getIndicesCount(), "indicescount plane") &
            assertEquals((TPureUInt) 24, meshBox->getMaterial().getIndicesCount(), "indicescount box") &
            assertEquals((TPureUInt) 24, meshCube->getMaterial().getIndicesCount(), "indicescount cube") &
            assertEquals(((PureMesh3D*)meshPlane->getAttachedAt(0))->getMaterial().getIndicesCount(), meshPlane->getMaterial().getIndicesCount(), "indicescount plane 2") &
            assertEquals(((PureMesh3D*)meshBox->getAttachedAt(0))->getMaterial().getIndicesCount(), meshBox->getMaterial().getIndicesCount(), "indicescount box 2") &
            assertEquals(((PureMesh3D*)meshCube->getAttachedAt(0))->getMaterial().getIndicesCount(), meshCube->getMaterial().getIndicesCount(), "indicescount cube 2") &
            assertNotNull(meshPlane->getMaterial().getColors(), "colors plane") &
            assertNotNull(meshBox->getMaterial().getColors(), "colors box") &
            assertNotNull(meshCube->getMaterial().getColors(), "colors cube") &
            assertEquals(((PureMesh3D*)meshPlane->getAttachedAt(0))->getMaterial().getColors(), meshPlane->getMaterial().getColors(), "colors plane 2") &
            assertEquals(((PureMesh3D*)meshBox->getAttachedAt(0))->getMaterial().getColors(), meshBox->getMaterial().getColors(), "colors box 2") &
            assertEquals(((PureMesh3D*)meshCube->getAttachedAt(0))->getMaterial().getColors(), meshCube->getMaterial().getColors(), "colors cube 2") &
            assertNotEquals(&(mesh->getMaterial(false)), &((PureMesh3D*)meshPlane->getAttachedAt(0))->getMaterial(false), "material noimplicit mesh") &
            assertNotEquals(&(meshPlane->getMaterial(false)), &((PureMesh3D*)meshPlane->getAttachedAt(0))->getMaterial(false), "material noimplicit plane") &
            assertNotEquals(&(meshBox->getMaterial(false)), &((PureMesh3D*)meshPlane->getAttachedAt(0))->getMaterial(false), "material noimplicit box") &
            assertNotEquals(&(meshCube->getMaterial(false)), &((PureMesh3D*)meshPlane->getAttachedAt(0))->getMaterial(false), "material noimplicit cube");
    }

    bool testGetMaterialSingleLayered()
    {
        PureTextureManager& tm = engine->getTextureManager();
        PureTexture* const tex24 = tm.createFromFile(BMP128x128x24);

        if ( !assertNotNull(tex24, "tex24 = NULL") )
            return false;

        // although textures are not auto-loaded for a Mesh, manually setting
        // textures for material of the mesh is not denied
        const bool b = assertTrue(mesh->getMaterial().setTexture(tex24), "setTexture tex24");

        return b &
            assertNotNull(mesh->getMaterial().getTexture(), "tex") &
            assertTrue(mesh->getMaterial().isTextured(), "isTex") &
            assertTrue(mesh->getMaterial().isSingleTextured(), "isTexSingle") &
            assertFalse(mesh->getMaterial().isMultiTextured(), "isTexMulti");
    }

    bool testGetMaterialMultiLayered()
    {
        PureTextureManager& tm = engine->getTextureManager();
        PureTexture* const tex24 = tm.createFromFile(BMP128x128x24);

        if ( !assertNotNull(tex24, "tex24 = NULL") )
            return false;

        // although textures are not auto-loaded for a Mesh, manually setting
        // textures for material of the mesh is not denied
        bool b = assertTrue(mesh->getMaterial().setTexture(tex24), "setTexture tex24");
        b &= assertTrue(mesh->getMaterial().setTexture(tex24, 1), "setTexture tex24, 1");

        return b &
            assertNotNull(mesh->getMaterial().getTexture(), "tex") &
            assertTrue(mesh->getMaterial().isTextured(), "isTex") &
            assertFalse(mesh->getMaterial().isSingleTextured(), "isTexSingle") &
            assertTrue(mesh->getMaterial().isMultiTextured(), "isTexMulti");
    }

    bool testGetUsedSystemMemory()
    {
        return assertGreater(mesh->getUsedSystemMemory(),       sizeof(PureMesh3D) + mesh->getVerticesCount() * 2 /* normals too */ * sizeof(TXYZ),         "mesh") &
            assertGreater(meshFromFile->getUsedSystemMemory(),  sizeof(PureMesh3D) + meshFromFile->getVerticesCount() * 2 /* normals too */ * sizeof(TXYZ), "meshFromFile") &
            assertGreater(meshPlane->getUsedSystemMemory(),     sizeof(PureMesh3D) + meshPlane->getVerticesCount() * 2 /* normals too */ * sizeof(TXYZ),    "plane") &
            assertGreater(meshBox->getUsedSystemMemory(),       sizeof(PureMesh3D) + meshBox->getVerticesCount() * 2 /* normals too */ * sizeof(TXYZ),      "box") &
            assertGreater(meshCube->getUsedSystemMemory(),      sizeof(PureMesh3D) + meshCube->getVerticesCount() * 2 /* normals too */ * sizeof(TXYZ),     "cube");
    }

    bool testGetCount()
    {
        return assertEquals(9, meshFromFile->getCount(), "meshFromFile") &
            assertEquals(1, meshPlane->getCount(), "plane") &
            assertEquals(1, meshBox->getCount(), "box") &
            assertEquals(1, meshCube->getCount(), "cube");
    }

    bool testIsEmpty()
    {
        return assertFalse(meshFromFile->isEmpty(), "meshFromFile") &
            assertFalse(meshPlane->isEmpty(), "plane") &
            assertFalse(meshBox->isEmpty(), "box") &
            assertFalse(meshCube->isEmpty(), "cube");
    }

    bool testGetSize()
    {
        return assertGequals(meshFromFile->getSize(),   meshFromFile->getCount(),      "meshFromFile") &
            assertGequals(meshPlane->getSize(), meshPlane->getCount(), "plane") &
            assertGequals(meshBox->getSize(),   meshBox->getCount(),   "box") &
            assertGequals(meshCube->getSize(),  meshCube->getCount(),  "cube");
    }

    bool testGetAttachedAt()
    {
        return assertNotNull(meshFromFile->getAttachedAt(0),   "meshFromFile") &
            assertNotNull(meshPlane->getAttachedAt(0), "plane") &
            assertNotNull(meshBox->getAttachedAt(0),   "box") &
            assertNotNull(meshCube->getAttachedAt(0),  "cube");
    }

    bool testPreAlloc()
    {
        const TPureInt prevSize = mesh->getSize();
        mesh->PreAlloc(10);

        return assertEquals(prevSize + 10, mesh->getSize());
    }

    bool testHasAttached2()
    {
        PureMesh3D* const mgd1 = mm->createCube(1);
        mgd1->DetachFrom();
        mesh->Attach(*mgd1);

        return assertFalse(mesh->hasAttached(*meshPlane), "meshPlane") &
               assertTrue(mesh->hasAttached(*mgd1), "mgd1");
    }

    bool testAttach()
    {
        PureMesh3D& mgr1 = *(mm->createCube(1));
        PureMesh3D& mgr2 = *(mm->createCube(1));
        mesh->DetachFrom();
        mgr1.Attach(*mesh);
        mgr2.Attach(*mesh);

        return assertTrue(mgr1.hasAttached(*mesh), "mgr1.hasAttached()") &
               assertEquals(2, mgr1.getCount(), "mgr1.getCount()") &
               assertFalse(mgr2.hasAttached(*mesh), "mgr2.hasAttached()") &
               assertEquals(1, mgr2.getCount(), "mgr2.getCount()");
    }

    bool testDetach()
    {
        PureMesh3D& mgd = *((PureMesh3D*) (mesh->getAttachedAt(0)));
        mesh->Detach(mgd);
        bool l = assertEquals(0, mesh->getCount());
        mesh->Attach(mgd);

        return l;
    }

    bool testDeleteAttachedInstance()
    {
        PureMesh3D& mgd = *((PureMesh3D*) (mesh->getAttachedAt(0)));
        mesh->DeleteAttachedInstance(mgd);

        return assertEquals(0, mesh->getCount());
    }

    bool testDeleteAll()
    {
        PureMesh3D* const mgd1 = mm->createCube(1), * const mgd2 = mm->createCube(1);
        mesh->DeleteAll();
        bool l = assertEquals(0, mesh->getCount(), "getCount 1");

        mesh->Attach(*mgd1);
        mesh->Attach(*mgd2);
        mesh->DeleteAll();
        l &= assertEquals(0, mesh->getCount(), "getCount 2");

        return l;
    }

    bool testWriteList()
    {
        mesh->WriteList();

        return true;
    }

    bool testMaterialGetUtiliser()
    {
        bool b = true;

        for (TPureInt i = 0; b && (i < mesh->getCount()); i++)
        {
            const PureMesh3D* const submesh = (PureMesh3D*) (mesh->getAttachedAt(i));
            const PureMaterial& submeshMat = submesh->getMaterial();
            b &= assertEquals(submesh, submeshMat.getUtiliser(), (std::string("mesh submeshes[") + std::to_string(i) + "]").c_str());
        }

        for (TPureInt i = 0; b && (i < meshFromFile->getCount()); i++)
        {
            const PureMesh3D* const submesh = (PureMesh3D*) (meshFromFile->getAttachedAt(i));
            const PureMaterial& submeshMat = submesh->getMaterial();
            b &= assertEquals(submesh, submeshMat.getUtiliser(), (std::string("meshFromFile submeshes[") + std::to_string(i) + "]").c_str());
        }

        for (TPureInt i = 0; b && (i < meshPlane->getCount()); i++)
        {
            const PureMesh3D* const submesh = (PureMesh3D*) (meshPlane->getAttachedAt(i));
            const PureMaterial& submeshMat = submesh->getMaterial();
            b &= assertEquals(submesh, submeshMat.getUtiliser(), (std::string("meshPlane submeshes[") + std::to_string(i) + "]").c_str());
        }

        for (TPureInt i = 0; b && (i < meshBox->getCount()); i++)
        {
            const PureMesh3D* const submesh = (PureMesh3D*) (meshBox->getAttachedAt(i));
            const PureMaterial& submeshMat = submesh->getMaterial();
            b &= assertEquals(submesh, submeshMat.getUtiliser(), (std::string("meshBox submeshes[") + std::to_string(i) + "]").c_str());
        }

        for (TPureInt i = 0; b && (i < meshCube->getCount()); i++)
        {
            const PureMesh3D* const submesh = (PureMesh3D*) (meshCube->getAttachedAt(i));
            const PureMaterial& submeshMat = submesh->getMaterial();
            b &= assertEquals(submesh, submeshMat.getUtiliser(), (std::string("meshCube submeshes[") + std::to_string(i) + "]").c_str());
        }

        return b & assertEquals(mesh, mesh->getMaterial(false).getUtiliser(), "mesh") &
            assertEquals(meshFromFile, meshFromFile->getMaterial(false).getUtiliser(), "meshFromFile") &
            assertEquals(meshPlane, meshPlane->getMaterial(false).getUtiliser(), "plane") &
            assertEquals(meshBox, meshBox->getMaterial(false).getUtiliser(), "box") &
            assertEquals(meshCube, meshCube->getMaterial(false).getUtiliser(), "cube");
    }

}; // class PureMesh3DTest