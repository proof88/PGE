#pragma once

/*
    ###################################################################################
    PRREImageTest.h
    Unit test for PRREMesh3D.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../PRRE/PR00FsReducedRenderingEngine.h"
#include "../PRRE/Object3D/PRREMesh3DManager.h"
#include "../PRRE/gl/GL.h" // should not include but we do this for getVertexIndicesType()

#ifndef E
#define E 0.0001f
#endif // EPSILON

#ifndef PRREIMAGETEST_FILENAMES
#define PRREIMAGETEST_FILENAMES
#define BMP128x128x32op "_res/proba128x128x32_opaque.bmp"
#define BMP128x128x32tr "_res/proba128x128x32_transparent.bmp"
#define BMP128x128x24 "_res/proba128x128x24.bmp"
#define BMP128x128x8 "_res/proba128x128x8.bmp"
#define BMP128x128x4 "_res/proba128x128x4.bmp"
#define BMP128x128x1 "_res/proba128x128x1.bmp"
#endif

class PRREMesh3DTest :
    public UnitTest
{
public:

    PRREMesh3DTest() :
        UnitTest( __FILE__ )
    {
        engine = NULL;
        mesh = NULL;
        meshFromFile = NULL;
        meshPlane = NULL;
        meshBox = NULL;
        meshCube = NULL;
    }

    ~PRREMesh3DTest()
    {
        Finalize();
    }

protected:

    virtual void Initialize()
    {
        engine = &PR00FsReducedRenderingEngine::createAndGet();
        engine->initialize(PRRE_RENDERER_HW_FP, 800, 600, PRRE_WINDOWED, 0, 32, 24, 0, 0);  // pretty standard display mode, should work on most systems
        mm = &engine->getMesh3DManager();
        mesh = NULL;
        meshFromFile = NULL;
        meshPlane = mm->createPlane(1.0f, 2.0f);
        meshBox   = mm->createBox(1.0f, 2.0f, 3.0f);
        meshCube  = mm->createCube(1.0f);

        AddSubTest("testCtor", (PFNUNITSUBTEST) &PRREMesh3DTest::testCtor);
        AddSubTest("testDtor", (PFNUNITSUBTEST) &PRREMesh3DTest::testDtor);
        AddSubTest("testGetPrimitiveFormat", (PFNUNITSUBTEST) &PRREMesh3DTest::testGetPrimitiveFormat);
        AddSubTest("testGetVerticesCount", (PFNUNITSUBTEST) &PRREMesh3DTest::testGetVerticesCount);
        AddSubTest("testGetVertices", (PFNUNITSUBTEST) &PRREMesh3DTest::testGetVertices);
        AddSubTest("testGetVertexIndicesType", (PFNUNITSUBTEST) &PRREMesh3DTest::testGetVertexIndicesType);
        AddSubTest("testGetMinIndexValue", (PFNUNITSUBTEST) &PRREMesh3DTest::testGetMinIndexValue);
        AddSubTest("testGetMaxIndexValue", (PFNUNITSUBTEST) &PRREMesh3DTest::testGetMaxIndexValue);
        AddSubTest("testGetIndexFromArray", (PFNUNITSUBTEST) &PRREMesh3DTest::testGetIndexFromArray);
        AddSubTest("testGetNormals", (PFNUNITSUBTEST) &PRREMesh3DTest::testGetNormals);
        AddSubTest("testGetPosVec", (PFNUNITSUBTEST) &PRREMesh3DTest::testGetPosVec);
        AddSubTest("testGetSizeVec", (PFNUNITSUBTEST) &PRREMesh3DTest::testGetSizeVec);
        AddSubTest("testRecalculateSize", (PFNUNITSUBTEST) &PRREMesh3DTest::testRecalculateSize);
        AddSubTest("testGetMaterialNoTexture", (PFNUNITSUBTEST) &PRREMesh3DTest::testGetMaterialNoTexture);
        AddSubTest("testGetMaterialSingleLayered", (PFNUNITSUBTEST) &PRREMesh3DTest::testGetMaterialSingleLayered);
        AddSubTest("testGetMaterialMultiLayered", (PFNUNITSUBTEST) &PRREMesh3DTest::testGetMaterialMultiLayered);
        AddSubTest("testGetUsedSystemMemory", (PFNUNITSUBTEST) &PRREMesh3DTest::testGetUsedSystemMemory);

        // since Mesh3D became a Manager also, we should test these capabalities here as well for possible undocumented changed behavior
        AddSubTest("testGetCount", (PFNUNITSUBTEST) &PRREMesh3DTest::testGetCount);
        AddSubTest("testIsEmpty", (PFNUNITSUBTEST) &PRREMesh3DTest::testIsEmpty);
        AddSubTest("testGetSize", (PFNUNITSUBTEST) &PRREMesh3DTest::testGetSize);
        AddSubTest("testGetAttachedAt", (PFNUNITSUBTEST) &PRREMesh3DTest::testGetAttachedAt);
        AddSubTest("testPreAlloc", (PFNUNITSUBTEST) &PRREMesh3DTest::testPreAlloc);
        AddSubTest("testHasAttached2", (PFNUNITSUBTEST) &PRREMesh3DTest::testHasAttached2);
        AddSubTest("testAttach", (PFNUNITSUBTEST) &PRREMesh3DTest::testAttach);
        AddSubTest("testDetach", (PFNUNITSUBTEST) &PRREMesh3DTest::testDetach);
        AddSubTest("testDeleteAttachedInstance", (PFNUNITSUBTEST) &PRREMesh3DTest::testDeleteAttachedInstance);
        AddSubTest("testDeleteAll", (PFNUNITSUBTEST) &PRREMesh3DTest::testDeleteAll);
        AddSubTest("testWriteList", (PFNUNITSUBTEST) &PRREMesh3DTest::testWriteList);
    }

    virtual bool setUp()
    {
        mesh = mm->createBox(1.0f, 2.0f, 3.0f);
        meshFromFile = mm->createFromFile("_res/models/snail_proofps/snail.obj");
        return assertNotNull(mesh, "mesh = NULL" ) &
            assertNotNull(meshFromFile, "meshFromFile = NULL" );
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
    }

private:
    PR00FsReducedRenderingEngine* engine;
    PRREMesh3DManager* mm;
    PRREMesh3D* mesh;
    PRREMesh3D* meshFromFile;
    const PRREMesh3D* meshPlane,
                      *meshBox,
                      *meshCube;

    // ---------------------------------------------------------------------------

    PRREMesh3DTest(const PRREMesh3DTest&)
    {};         

    PRREMesh3DTest& operator=(const PRREMesh3DTest&)
    {
        return *this;
    };

    bool testCtor()
    {
        return true;
    }

    bool testDtor()
    {
        delete mesh;
        mesh = NULL;
        return assertEquals(4, mm->getCount());
    }

    bool testGetPrimitiveFormat()
    {
        return assertEquals(PRRE_PM_TRIANGLES, meshFromFile->getPrimitiveFormat(), "meshFromFile") &
            assertEquals(PRRE_PM_QUADS, mesh->getPrimitiveFormat(), "mesh") &
            assertEquals(PRRE_PM_QUADS, meshPlane->getPrimitiveFormat(), "plane") &
            assertEquals(PRRE_PM_QUADS, meshBox->getPrimitiveFormat(), "box") &
            assertEquals(PRRE_PM_QUADS, meshCube->getPrimitiveFormat(), "cube");
    }

    bool testGetVerticesCount()
    {
        return assertEquals((TPRREuint) 4, meshPlane->getVerticesCount(), "plane") &
            assertEquals((TPRREuint) 24, meshBox->getVerticesCount(), "box") &
            assertEquals((TPRREuint) 24, meshCube->getVerticesCount(), "cube") &
            assertEquals((TPRREuint) 0, meshFromFile->getVerticesCount(), "meshFromFile") &
            assertEquals(((PRREMesh3D*)meshPlane->getAttachedAt(0))->getVerticesCount(), meshPlane->getVerticesCount(), "plane 2") &
            assertEquals(((PRREMesh3D*)meshBox->getAttachedAt(0))->getVerticesCount(), meshBox->getVerticesCount(), "box 2") &
            assertEquals(((PRREMesh3D*)meshCube->getAttachedAt(0))->getVerticesCount(), meshCube->getVerticesCount(), "cube 2") &
            assertNotEquals(((PRREMesh3D*)meshFromFile->getAttachedAt(0))->getVerticesCount(), meshFromFile->getVerticesCount(), "meshFromFile 2") &
            assertEquals((TPRREuint)144, ((PRREMesh3D*)meshFromFile->getAttachedAt(0))->getVerticesCount(), "meshFromFile 3") &
            assertEquals((TPRREuint) 0, meshPlane->getVerticesCount(false), "plane noimplicit") &
            assertEquals((TPRREuint) 0, meshBox->getVerticesCount(false), "box noimplicit") &
            assertEquals((TPRREuint) 0, meshCube->getVerticesCount(false), "cube noimplicit") &
            assertEquals((TPRREuint) 0, meshFromFile->getVerticesCount(false), "meshFromFile noimplicit");
    }

    bool testGetVertices()
    {
        return assertNotNull(meshPlane->getVertices(), "plane") &
            assertNotNull(meshBox->getVertices(), "box") &
            assertNotNull(meshCube->getVertices(), "cube") &
            assertNull(meshFromFile->getVertices(), "meshFromFile") &
            assertEquals(((PRREMesh3D*)meshPlane->getAttachedAt(0))->getVertices(), meshPlane->getVertices(), "plane 2") &
            assertEquals(((PRREMesh3D*)meshBox->getAttachedAt(0))->getVertices(), meshBox->getVertices(), "box 2") &
            assertEquals(((PRREMesh3D*)meshCube->getAttachedAt(0))->getVertices(), meshCube->getVertices(), "cube 2") &
            assertNotEquals(((PRREMesh3D*)meshFromFile->getAttachedAt(0))->getVertices(), meshFromFile->getVertices(), "meshFromFile 2") &
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
            assertEquals(((PRREMesh3D*)meshPlane->getAttachedAt(0))->getVertexIndicesType(), meshPlane->getVertexIndicesType(false), "plane noimplicit") &
            assertEquals(((PRREMesh3D*)meshBox->getAttachedAt(0))->getVertexIndicesType(), meshBox->getVertexIndicesType(false), "box noimplicit") &
            assertEquals(((PRREMesh3D*)meshCube->getAttachedAt(0))->getVertexIndicesType(), meshCube->getVertexIndicesType(false), "cube noimplicit") &
            assertEquals((unsigned int)GL_UNSIGNED_BYTE, ((PRREMesh3D*)meshFromFile->getAttachedAt(0))->getVertexIndicesType(), "meshFromFile noimplicit");
    }

    bool testGetMinIndexValue()
    {
        return assertEquals((TPRREuint)0, meshPlane->getMinIndexValue(), "plane") &
            assertEquals((TPRREuint)0, meshBox->getMinIndexValue(), "box") &
            assertEquals((TPRREuint)0, meshCube->getMinIndexValue(), "cube") &
            assertEquals((TPRREuint)UINT_MAX, meshFromFile->getMinIndexValue(), "meshFromFile") &
            /* doesn't matter if implicit access or not, if there is vertex or not, 0 is the expected anyway */
            assertEquals(((PRREMesh3D*)meshPlane->getAttachedAt(0))->getMinIndexValue(), meshPlane->getMinIndexValue(false), "plane sub noimplicit") &
            assertEquals(((PRREMesh3D*)meshBox->getAttachedAt(0))->getMinIndexValue(), meshBox->getMinIndexValue(false), "box sub noimplicit") &
            assertEquals(((PRREMesh3D*)meshCube->getAttachedAt(0))->getMinIndexValue(), meshCube->getMinIndexValue(false), "cube sub noimplicit") &
            assertNotEquals(((PRREMesh3D*)meshFromFile->getAttachedAt(0))->getMinIndexValue(), meshFromFile->getMinIndexValue(false), "meshFromFile sub noimplicit") &
            assertEquals(((PRREMesh3D*)meshPlane->getAttachedAt(0))->getMinIndexValue(), meshPlane->getMinIndexValue(), "plane sub") &
            assertEquals(((PRREMesh3D*)meshBox->getAttachedAt(0))->getMinIndexValue(), meshBox->getMinIndexValue(), "box sub") &
            assertEquals(((PRREMesh3D*)meshCube->getAttachedAt(0))->getMinIndexValue(), meshCube->getMinIndexValue(), "cube sub") &
            assertEquals((TPRREuint) 0, ((PRREMesh3D*)meshFromFile->getAttachedAt(0))->getMinIndexValue(), "meshFromFile sub");
    }

    bool testGetMaxIndexValue()
    {
        return assertEquals((TPRREuint)3, meshPlane->getMaxIndexValue(), "plane") &
            assertEquals((TPRREuint)23, meshBox->getMaxIndexValue(), "box") &
            assertEquals((TPRREuint)23, meshCube->getMaxIndexValue(), "cube") &
            assertEquals((TPRREuint)0, meshFromFile->getMaxIndexValue(), "meshFromFile") &
            assertEquals((TPRREuint)0, meshPlane->getMaxIndexValue(false), "plane noimplicit") &
            assertEquals((TPRREuint)0, meshBox->getMaxIndexValue(false), "box noimplicit") &
            assertEquals((TPRREuint)0, meshCube->getMaxIndexValue(false), "cube noimplicit") &
            assertEquals((TPRREuint)0, meshFromFile->getMaxIndexValue(false), "meshFromFile noimplicit") &
            assertEquals((TPRREuint)3, ((PRREMesh3D*)meshPlane->getAttachedAt(0))->getMaxIndexValue(), "plane sub") &
            assertEquals((TPRREuint)23, ((PRREMesh3D*)meshBox->getAttachedAt(0))->getMaxIndexValue(), "box sub") &
            assertEquals((TPRREuint)23, ((PRREMesh3D*)meshCube->getAttachedAt(0))->getMaxIndexValue(), "cube sub") &
            assertEquals((TPRREuint)143, ((PRREMesh3D*)meshFromFile->getAttachedAt(0))->getMaxIndexValue(), "meshFromFile sub");
    }

    bool testGetIndexFromArray()
    {
        bool b = true;
        
        const PRREMesh3D& submeshPlane = * ((PRREMesh3D*) (meshPlane->getAttachedAt(0)));
        for (TPRREuint i = 0; i < submeshPlane.getVertexIndicesCount(); i++)
        {
            b &= assertEquals(i,
            submeshPlane.getIndexFromArray(submeshPlane.getVertexIndices(), i),
            (std::string("plane ") + std::to_string(i)).c_str());
        }

        const PRREMesh3D& submeshBox = * ((PRREMesh3D*) (meshBox->getAttachedAt(0)));
        for (TPRREuint i = 0; i < submeshBox.getVertexIndicesCount(); i++)
        {
            b &= assertEquals(i,
            submeshBox.getIndexFromArray(submeshBox.getVertexIndices(), i),
            (std::string("box ") + std::to_string(i)).c_str());
        }

        const PRREMesh3D& submeshCube = * ((PRREMesh3D*) (meshCube->getAttachedAt(0)));
        for (TPRREuint i = 0; i < submeshCube.getVertexIndicesCount(); i++)
        {
            b &= assertEquals(i,
            submeshCube.getIndexFromArray(submeshCube.getVertexIndices(), i),
            (std::string("cube ") + std::to_string(i)).c_str());
        }

        const PRREMesh3D& submeshFile = * ((PRREMesh3D*) (meshFromFile->getAttachedAt(0)));
        for (TPRREuint i = 0; i < submeshFile.getVertexIndicesCount(); i++)
        {
            b &= assertEquals(i,
            submeshFile.getIndexFromArray(submeshFile.getVertexIndices(), i),
            (std::string("meshFromFile ") + std::to_string(i)).c_str());
        }

        return b;
    }

    bool testGetNormals()
    {
        return assertNotNull(meshPlane->getNormals(), "plane") &
            assertNotNull(meshBox->getNormals(), "box") &
            assertNotNull(meshCube->getNormals(), "cube") &
            assertNull(meshFromFile->getNormals(), "meshFromFile") &
            assertEquals(((PRREMesh3D*)meshPlane->getAttachedAt(0))->getNormals(), meshPlane->getNormals(), "plane 2") &
            assertEquals(((PRREMesh3D*)meshBox->getAttachedAt(0))->getNormals(), meshBox->getNormals(), "box 2") &
            assertEquals(((PRREMesh3D*)meshCube->getAttachedAt(0))->getNormals(), meshCube->getNormals(), "cube 2") &
            assertNotEquals(((PRREMesh3D*)meshFromFile->getAttachedAt(0))->getNormals(), meshFromFile->getNormals(), "meshFromFile 2") &
            assertNull(meshPlane->getNormals(false), "plane noimplicit") &
            assertNull(meshBox->getNormals(false), "box noimplicit") &
            assertNull(meshCube->getNormals(false), "cube noimplicit") &
            assertNull(meshFromFile->getNormals(false), "meshFromFile noimplicit");
    }

    bool testGetPosVec()
    {
        // in our loaded object, there shouldn't be any subobject in pos(0,0,0)
        bool b1 = true;
        for (TPRREint i = 0; i < meshFromFile->getCount(); i++)
            b1 = b1 & assertFalse( ((PRREMesh3D*)meshFromFile->getAttachedAt(i))->getPosVec().isZero(), "pos zero mesh sub" );

        // in our other objects, all subobjects should be at pos(0,0,0)
        bool b2 = true;
        for (TPRREint i = 0; i < meshPlane->getCount(); i++)
            b2 = b2 & assertTrue( ((PRREMesh3D*)meshPlane->getAttachedAt(i))->getPosVec().isZero(), "pos zero plane sub" );

        bool b3 = true;
        for (TPRREint i = 0; i < meshBox->getCount(); i++)
            b3 = b3 & assertTrue( ((PRREMesh3D*)meshBox->getAttachedAt(i))->getPosVec().isZero(), "pos zero box sub" );

        bool b4 = true;
        for (TPRREint i = 0; i < meshCube->getCount(); i++)
            b4 = b4 & assertTrue( ((PRREMesh3D*)meshCube->getAttachedAt(i))->getPosVec().isZero(), "pos zero cube sub" );

        return assertEquals(0.0f, meshPlane->getPosVec().getX(), E, "plane x") &        
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

    bool testGetSizeVec()
    {
        // in our loaded object, there shouldn't be any subobject with size(0,0,0)
        bool b1 = true;
        for (TPRREint i = 0; i < meshFromFile->getCount(); i++)
            b1 = b1 & assertFalse( ((PRREMesh3D*)meshFromFile->getAttachedAt(i))->getSizeVec().isZero(), "size zero mesh sub" );

        // in our other objects, all subobjects size should be same as object size
        bool b2 = true;
        for (TPRREint i = 0; i < meshPlane->getCount(); i++)
            b2 = b2 & assertTrue( ((PRREMesh3D*)meshPlane->getAttachedAt(i))->getSizeVec() == meshPlane->getSizeVec(), "size plane sub" );

        bool b3 = true;
        for (TPRREint i = 0; i < meshBox->getCount(); i++)
            b3 = b3 & assertTrue( ((PRREMesh3D*)meshBox->getAttachedAt(i))->getSizeVec() == meshBox->getSizeVec(), "size box sub" );

        bool b4 = true;
        for (TPRREint i = 0; i < meshCube->getCount(); i++)
            b4 = b4 & assertTrue( ((PRREMesh3D*)meshCube->getAttachedAt(i))->getSizeVec() == meshCube->getSizeVec(), "size cube sub" );

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
        const PRREVector vOld = mesh->getSizeVec();
        mesh->RecalculateSize();

        return assertTrue(vOld == mesh->getSizeVec());
    }

    bool testGetMaterialNoTexture()
    {
        return assertEquals((TPRREubyte) 255, mesh->getMaterial().getTextureEnvColor().getRed(), "env mesh r") &
            assertEquals((TPRREubyte) 255, mesh->getMaterial().getTextureEnvColor().getGreen(), "env mesh g") &
            assertEquals((TPRREubyte) 255, mesh->getMaterial().getTextureEnvColor().getBlue(), "env mesh b") &
            assertEquals((TPRREubyte) 255, mesh->getMaterial().getTextureEnvColor().getAlpha(), "env mesh a") &
            assertEquals((TPRREubyte) 255, meshPlane->getMaterial().getTextureEnvColor().getRed(), "env plane r") &
            assertEquals((TPRREubyte) 255, meshPlane->getMaterial().getTextureEnvColor().getGreen(), "env plane g") &
            assertEquals((TPRREubyte) 255, meshPlane->getMaterial().getTextureEnvColor().getBlue(), "env plane b") &
            assertEquals((TPRREubyte) 255, meshPlane->getMaterial().getTextureEnvColor().getAlpha(), "env plane a") &
            assertEquals((TPRREubyte) 255, meshBox->getMaterial().getTextureEnvColor().getRed(), "env box r") &
            assertEquals((TPRREubyte) 255, meshBox->getMaterial().getTextureEnvColor().getGreen(), "env box g") &
            assertEquals((TPRREubyte) 255, meshBox->getMaterial().getTextureEnvColor().getBlue(), "env box b") &
            assertEquals((TPRREubyte) 255, meshBox->getMaterial().getTextureEnvColor().getAlpha(), "env box a") &
            assertEquals((TPRREubyte) 255, meshCube->getMaterial().getTextureEnvColor().getRed(), "env cube r") &
            assertEquals((TPRREubyte) 255, meshCube->getMaterial().getTextureEnvColor().getGreen(), "env cube g") &
            assertEquals((TPRREubyte) 255, meshCube->getMaterial().getTextureEnvColor().getBlue(), "env cube b") &
            assertEquals((TPRREubyte) 255, meshCube->getMaterial().getTextureEnvColor().getAlpha(), "env cube a") &
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
            assertEquals((TPRREuint) 4, meshPlane->getMaterial().getTexcoordsCount(), "texcoordcount plane") &
            assertEquals((TPRREuint) 24, meshBox->getMaterial().getTexcoordsCount(), "texcoordcount box") &
            assertEquals((TPRREuint) 24, meshCube->getMaterial().getTexcoordsCount(), "texcoordcount cube") &
            assertEquals(((PRREMesh3D*)meshPlane->getAttachedAt(0))->getMaterial().getTexcoordsCount(), meshPlane->getMaterial().getTexcoordsCount(), "texcoordcount plane 2") &
            assertEquals(((PRREMesh3D*)meshBox->getAttachedAt(0))->getMaterial().getTexcoordsCount(), meshBox->getMaterial().getTexcoordsCount(), "texcoordcount box 2") &
            assertEquals(((PRREMesh3D*)meshCube->getAttachedAt(0))->getMaterial().getTexcoordsCount(), meshCube->getMaterial().getTexcoordsCount(), "texcoordcount cube 2") &
            assertNotNull(meshPlane->getMaterial().getTexcoords(), "texcoords plane") &
            assertNotNull(meshBox->getMaterial().getTexcoords(), "texcoords box") &
            assertNotNull(meshCube->getMaterial().getTexcoords(), "texcoords cube") &
            assertEquals(((PRREMesh3D*)meshPlane->getAttachedAt(0))->getMaterial().getTexcoords(), meshPlane->getMaterial().getTexcoords(), "texcoords plane 2") &
            assertEquals(((PRREMesh3D*)meshBox->getAttachedAt(0))->getMaterial().getTexcoords(), meshBox->getMaterial().getTexcoords(), "texcoords box 2") &
            assertEquals(((PRREMesh3D*)meshCube->getAttachedAt(0))->getMaterial().getTexcoords(), meshCube->getMaterial().getTexcoords(), "texcoords cube 2") &
            assertEquals((TPRREuint) 4, meshPlane->getMaterial().getColorsCount(), "colorscount plane") &
            assertEquals((TPRREuint) 24, meshBox->getMaterial().getColorsCount(), "colorscount box") &
            assertEquals((TPRREuint) 24, meshCube->getMaterial().getColorsCount(), "colorscount cube") &
            assertEquals(((PRREMesh3D*)meshPlane->getAttachedAt(0))->getMaterial().getColorsCount(), meshPlane->getMaterial().getColorsCount(), "colorscount plane 2") &
            assertEquals(((PRREMesh3D*)meshBox->getAttachedAt(0))->getMaterial().getColorsCount(), meshBox->getMaterial().getColorsCount(), "colorscount box 2") &
            assertEquals(((PRREMesh3D*)meshCube->getAttachedAt(0))->getMaterial().getColorsCount(), meshCube->getMaterial().getColorsCount(), "colorscount cube 2") &
            assertEquals((TPRREuint) 4, meshPlane->getMaterial().getIndicesCount(), "indicescount plane") &
            assertEquals((TPRREuint) 24, meshBox->getMaterial().getIndicesCount(), "indicescount box") &
            assertEquals((TPRREuint) 24, meshCube->getMaterial().getIndicesCount(), "indicescount cube") &
            assertEquals(((PRREMesh3D*)meshPlane->getAttachedAt(0))->getMaterial().getIndicesCount(), meshPlane->getMaterial().getIndicesCount(), "indicescount plane 2") &
            assertEquals(((PRREMesh3D*)meshBox->getAttachedAt(0))->getMaterial().getIndicesCount(), meshBox->getMaterial().getIndicesCount(), "indicescount box 2") &
            assertEquals(((PRREMesh3D*)meshCube->getAttachedAt(0))->getMaterial().getIndicesCount(), meshCube->getMaterial().getIndicesCount(), "indicescount cube 2") &
            assertNotNull(meshPlane->getMaterial().getColors(), "colors plane") &
            assertNotNull(meshBox->getMaterial().getColors(), "colors box") &
            assertNotNull(meshCube->getMaterial().getColors(), "colors cube") &
            assertEquals(((PRREMesh3D*)meshPlane->getAttachedAt(0))->getMaterial().getColors(), meshPlane->getMaterial().getColors(), "colors plane 2") &
            assertEquals(((PRREMesh3D*)meshBox->getAttachedAt(0))->getMaterial().getColors(), meshBox->getMaterial().getColors(), "colors box 2") &
            assertEquals(((PRREMesh3D*)meshCube->getAttachedAt(0))->getMaterial().getColors(), meshCube->getMaterial().getColors(), "colors cube 2") &
            assertNotEquals(&(mesh->getMaterial(false)), &((PRREMesh3D*)meshPlane->getAttachedAt(0))->getMaterial(false), "material noimplicit mesh") &
            assertNotEquals(&(meshPlane->getMaterial(false)), &((PRREMesh3D*)meshPlane->getAttachedAt(0))->getMaterial(false), "material noimplicit plane") &
            assertNotEquals(&(meshBox->getMaterial(false)), &((PRREMesh3D*)meshPlane->getAttachedAt(0))->getMaterial(false), "material noimplicit box") &
            assertNotEquals(&(meshCube->getMaterial(false)), &((PRREMesh3D*)meshPlane->getAttachedAt(0))->getMaterial(false), "material noimplicit cube");
    }

    bool testGetMaterialSingleLayered()
    {
        PRRETextureManager& tm = engine->getTextureManager();
        PRRETexture* const tex24 = tm.createFromFile(BMP128x128x24);

        if ( !assertNotNull(tex24, "tex24 = NULL") )
            return false;

        // although textures are not auto-loaded for a Mesh, manually setting
        // textures for material of the mesh is not denied
        mesh->getMaterial().SetTexture(tex24);

        return assertNotNull(mesh->getMaterial().getTexture(), "tex") &
            assertTrue(mesh->getMaterial().isTextured(), "isTex") &
            assertTrue(mesh->getMaterial().isSingleTextured(), "isTexSingle") &
            assertFalse(mesh->getMaterial().isMultiTextured(), "isTexMulti");
    }

    bool testGetMaterialMultiLayered()
    {
        PRRETextureManager& tm = engine->getTextureManager();
        PRRETexture* const tex24 = tm.createFromFile(BMP128x128x24);

        if ( !assertNotNull(tex24, "tex24 = NULL") )
            return false;

        // although textures are not auto-loaded for a Mesh, manually setting
        // textures for material of the mesh is not denied
        mesh->getMaterial().SetTexture(tex24);
        mesh->getMaterial().SetTexture(tex24, 1);

        return assertNotNull(mesh->getMaterial().getTexture(), "tex") &
            assertTrue(mesh->getMaterial().isTextured(), "isTex") &
            assertFalse(mesh->getMaterial().isSingleTextured(), "isTexSingle") &
            assertTrue(mesh->getMaterial().isMultiTextured(), "isTexMulti");
    }

    bool testGetUsedSystemMemory()
    {
        return assertGreater(mesh->getUsedSystemMemory(),      sizeof(PRREMesh3D), "mesh") &
               assertGreater(meshPlane->getUsedSystemMemory(), sizeof(PRREMesh3D), "plane") &
               assertGreater(meshBox->getUsedSystemMemory(),   sizeof(PRREMesh3D), "box") &
               assertGreater(meshCube->getUsedSystemMemory(),  sizeof(PRREMesh3D), "cube");
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
        const TPRREint prevSize = mesh->getSize();
        mesh->PreAlloc(10);

        return assertEquals(prevSize + 10, mesh->getSize());
    }

    bool testHasAttached2()
    {
        PRREMesh3D* const mgd1 = mm->createCube(1);
        mgd1->DetachFrom();
        mesh->Attach(*mgd1);

        return assertFalse(mesh->hasAttached(*meshPlane), "meshPlane") &
               assertTrue(mesh->hasAttached(*mgd1), "mgd1");
    }

    bool testAttach()
    {
        PRREMesh3D& mgr1 = *(mm->createCube(1));
        PRREMesh3D& mgr2 = *(mm->createCube(1));
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
        PRREMesh3D& mgd = *((PRREMesh3D*) (mesh->getAttachedAt(0)));
        mesh->Detach(mgd);
        bool l = assertEquals(0, mesh->getCount());
        mesh->Attach(mgd);

        return l;
    }

    bool testDeleteAttachedInstance()
    {
        PRREMesh3D& mgd = *((PRREMesh3D*) (mesh->getAttachedAt(0)));
        mesh->DeleteAttachedInstance(mgd);

        return assertEquals(0, mesh->getCount());
    }

    bool testDeleteAll()
    {
        PRREMesh3D* const mgd1 = mm->createCube(1), * const mgd2 = mm->createCube(1);
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

}; // class PRREMesh3DTest