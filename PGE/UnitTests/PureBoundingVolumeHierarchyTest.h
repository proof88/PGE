#pragma once

/*
    ###################################################################################
    PureBoundingVolumeHierarchyTest.h
    Unit test for PureBoundingVolumeHierarchy.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../Pure/include/internal/SpatialStructures/PureBoundingVolumeHierarchy.h"

class PureBoundingVolumeHierarchyTest :
    public UnitTest
{
public:

    PureBoundingVolumeHierarchyTest() :
        UnitTest( __FILE__ )
    {
    } // PureBoundingVolumeHierarchyTest()

protected:

    virtual void Initialize()
    {
        engine = NULL;
        om = NULL;
        AddSubTest("testCtor1", (PFNUNITSUBTEST) &PureBoundingVolumeHierarchyTest::testCtor1);
        AddSubTest("testInsertObject", (PFNUNITSUBTEST) &PureBoundingVolumeHierarchyTest::testInsertObject);
    }

    virtual bool setUp()
    {
        bool ret = true;
        if ( engine == NULL )
        {
            engine = &PR00FsReducedRenderingEngine::createAndGet();
            ret &= assertEquals((TPureUInt)0, engine->initialize(PURE_RENDERER_HW_FP, 800, 600, PURE_WINDOWED, 0, 32, 24, 0, 0), "engine");  // pretty standard display mode, should work on most systems
            om = &engine->getObject3DManager();
            ret &= assertNotNull(om, "om null");
        }
        return ret;
    }

    virtual void TearDown()
    {
        Finalize();
        om = NULL;
    }

    virtual void Finalize()
    {
        if ( engine )
        {
            engine->shutdown();
            engine = NULL;
        }
    }

private:
    PR00FsReducedRenderingEngine* engine; // need the engine for creating Object3D instances
    PureObject3DManager* om;

    // ---------------------------------------------------------------------------

    PureBoundingVolumeHierarchyTest(const PureBoundingVolumeHierarchyTest&)
    {}         

    PureBoundingVolumeHierarchyTest& operator=(const PureBoundingVolumeHierarchyTest&)
    {
        return *this;
    }

    bool testCtor1()
    {
        PureBoundingVolumeHierarchy tree(PureVector(1, 2, 3), 1000.0f, 3, 0);
        
        return assertEquals(1.0f, tree.getPos().getX(), "pos.x") &
        assertEquals(2.0f, tree.getPos().getY(), "pos.y") &
        assertEquals(3.0f, tree.getPos().getZ(), "pos.z") &
        assertEquals((TPureUInt)3, tree.getMaxDepthLevel(), "max depth level") &
        assertEquals((TPureUInt)0, tree.getDepthLevel(), "root node depth level") &
        assertEquals(1000.0f, tree.getSize(), "size") &
        assertEquals(PureOctree::NodeType::LeafEmpty, tree.getNodeType(), "nodeType") &
        assertEquals((std::size_t)0, tree.getChildren().size(), "children") &
        assertEquals((std::size_t)0, tree.getObjects().size(), "objects");
    }

    bool testInsertObject()
    {
        // basically this is the same test as found in PureOctreeTest.h, but here we are interested in the
        // BVH-related stuff only

        // used to test if the given world-space position other than (0,0,0) is really taken into calculations;
        // if there is any issue, change this to (0,0,0) to find out probable reason of tree not properly taking origin pos into account!
        const PureVector treeOrigin(100.f, 200.f, 300.f);

        PureBoundingVolumeHierarchy tree(treeOrigin, 1000.0f, 2, 0);
        /*

        This is a 1000 x 1000 x 1000 size octree, in pos treeOrigin.
        Max tree depth is set to 2.
        In the left picture the tree depth is only showed as 1, meaning only root node and its 8 children are visible.
        Picture is showing obj1 and obj2 inserted.
        Other objects are not yet inserted in the picture, as it would require further subdivision to be displayed.
        The full tree is visualized in the right picture.

                                                                    |
                             <----- 1000 ----->                     |
                      ^      _________________         ^            |         level 0                   root
              1000  /  TLB /| obj1   /       /| TRB    |            |                        ____________(.)____________
                  /      /- |- - - /-| - - /- |        |            |                       /   /   /   /   \   \   \   \
                /      /_|_______/_______/ |  |        |            |         level 1     (x) (x) ( ) ( )   ( ) (.) ( ) ( )
              ¡       |  |  |_ _| _ _| _| _|_ |        |  1000      |                     TLB TRB    ___________BRF___________
                      |  | /|   |    |  |  | /| BRB    |            |                  (obj1) (obj4)/   /   /   / \   \   \   \
                 TLF  |  /  |   |  / |  |  /  |        |            |                              /   /   /   /   \   \   \   \
                      |/_|__|___|/___|__|/_|__|        |            |         level 2            (x) ( ) ( ) ( )   ( ) ( ) ( ) ( )
                      |  |  |_ _|_ _ |_ |_ | _|        |            |                            TLB
                      |  | /    |    |  |  | /         ¡            |                           (obj2,
                      |  /- - - |- /- - |- /                        |                            obj3)
                      |/_________/_obj2_|/                          |
                          BLF      BRF                              |

        In this test we put 4 objects inside the tree:
         - at the beginning there are no children of root node, tree depth is 0;
         - obj1 is inserted, so root node will have 8 children, slicing root into 500x500x500 cubes, tree depth will be 1;
         - obj1 is placed at (-200, 200, 200), so it should be in root.children[TopLeftBack];
         - obj2 is placed at (200, -200, -200), so it should be in root.children[BottomRightFront], tree depth doesn't change;
         - left picture above shows the tree status at this point;
         - obj3 is also placed at (200, -200, -200), so root.children[BottomRightFront] will also have 8 children, slicing it into 250x250x250 cubes,
           tree depth reaches max tree depth 2,
           and obj2 will be in root.children[BottomRightFront].children[TopLeftBack],
           and since we already reached max tree depth 2, no further subdivision will happen, so obj3 will be also in same node as obj2;
         - obj4 is placed at treeOrigin, so it should be root.children[TopRightBack] as defined;
         - obj5 is placed at (3000, 0, 0), intentionally outside tree root node volume, so it should not be inserted in tree!

        */
        
        PureObject3D* const obj1 = om->createBox(2.0f, 2.0f, 2.0f);
        PureObject3D* const obj2 = om->createBox(2.0f, 2.0f, 2.0f);
        PureObject3D* const obj3 = om->createBox(2.0f, 2.0f, 2.0f);
        PureObject3D* const obj4 = om->createBox(2.0f, 2.0f, 2.0f);
        PureObject3D* const obj5 = om->createBox(2.0f, 2.0f, 2.0f);
        if ( !assertNotNull(obj1, "obj1 not null") || !assertNotNull(obj2, "obj2 not null") || !assertNotNull(obj3, "obj3 not null") || !assertNotNull(obj4, "obj4 not null") || !assertNotNull(obj5, "obj5 not null") )
        {
            return false;
        }

        obj1->getPosVec().Set(treeOrigin.getX() - 200.f, treeOrigin.getY() + 200.f, treeOrigin.getZ() + 200.f);
        obj2->getPosVec().Set(treeOrigin.getX() + 200.f, treeOrigin.getY() - 200.f, treeOrigin.getZ() - 200.f);
        obj3->getPosVec().Set(treeOrigin.getX() + 200.f, treeOrigin.getY() - 200.f, treeOrigin.getZ() - 200.f);
        obj4->getPosVec().Set(treeOrigin.getX(), treeOrigin.getY(), treeOrigin.getZ());
        obj5->getPosVec().Set(treeOrigin.getX() + 3000.f, treeOrigin.getY(), treeOrigin.getZ());

        const PureBoundingVolumeHierarchy* const node1 = tree.insertObject(*obj1);
        const PureBoundingVolumeHierarchy* const node2 = tree.insertObject(*obj2);
        const PureBoundingVolumeHierarchy* const node3 = tree.insertObject(*obj3);
        const PureBoundingVolumeHierarchy* const node4 = tree.insertObject(*obj4);
        const PureBoundingVolumeHierarchy* const node5 = tree.insertObject(*obj5);  // out of tree bounds
        if ( !assertNotNull(node1, "node 1") || !assertNotNull(node2, "node 2") || !assertNotNull(node3, "node 3") || !assertNotNull(node4, "node 4") || !assertNull(node5, "node 5") )
        {
            return false;
        }

        if ( !assertEquals(PureOctree::NodeType::Parent, tree.getNodeType(), "tree nodeType") )
        {
            return false;
        }

        const PureBoundingVolumeHierarchy* tree_TLF = static_cast<const PureBoundingVolumeHierarchy*>(tree.getChildren()[PureOctree::TOP    | PureOctree::LEFT  | PureOctree::FRONT]);
        const PureBoundingVolumeHierarchy* tree_BLF = static_cast<const PureBoundingVolumeHierarchy*>(tree.getChildren()[PureOctree::BOTTOM | PureOctree::LEFT  | PureOctree::FRONT]);
        const PureBoundingVolumeHierarchy* tree_TRF = static_cast<const PureBoundingVolumeHierarchy*>(tree.getChildren()[PureOctree::TOP    | PureOctree::RIGHT | PureOctree::FRONT]);
        const PureBoundingVolumeHierarchy* tree_BRF = static_cast<const PureBoundingVolumeHierarchy*>(tree.getChildren()[PureOctree::BOTTOM | PureOctree::RIGHT | PureOctree::FRONT]);
        const PureBoundingVolumeHierarchy* tree_TLB = static_cast<const PureBoundingVolumeHierarchy*>(tree.getChildren()[PureOctree::TOP    | PureOctree::LEFT  | PureOctree::BACK ]);
        const PureBoundingVolumeHierarchy* tree_BLB = static_cast<const PureBoundingVolumeHierarchy*>(tree.getChildren()[PureOctree::BOTTOM | PureOctree::LEFT  | PureOctree::BACK ]);
        const PureBoundingVolumeHierarchy* tree_TRB = static_cast<const PureBoundingVolumeHierarchy*>(tree.getChildren()[PureOctree::TOP    | PureOctree::RIGHT | PureOctree::BACK ]);
        const PureBoundingVolumeHierarchy* tree_BRB = static_cast<const PureBoundingVolumeHierarchy*>(tree.getChildren()[PureOctree::BOTTOM | PureOctree::RIGHT | PureOctree::BACK ]);

        if ( !assertEquals(PureOctree::NodeType::Parent, tree_BRF->getNodeType(), "tree_BRF nodeType") )
        {
            return false;
        }

        const PureBoundingVolumeHierarchy* tree_BRF_TLF = static_cast<const PureBoundingVolumeHierarchy*>(tree_BRF->getChildren()[PureOctree::TOP    | PureOctree::LEFT  | PureOctree::FRONT]);
        const PureBoundingVolumeHierarchy* tree_BRF_BLF = static_cast<const PureBoundingVolumeHierarchy*>(tree_BRF->getChildren()[PureOctree::BOTTOM | PureOctree::LEFT  | PureOctree::FRONT]);
        const PureBoundingVolumeHierarchy* tree_BRF_TRF = static_cast<const PureBoundingVolumeHierarchy*>(tree_BRF->getChildren()[PureOctree::TOP    | PureOctree::RIGHT | PureOctree::FRONT]);
        const PureBoundingVolumeHierarchy* tree_BRF_BRF = static_cast<const PureBoundingVolumeHierarchy*>(tree_BRF->getChildren()[PureOctree::BOTTOM | PureOctree::RIGHT | PureOctree::FRONT]);
        const PureBoundingVolumeHierarchy* tree_BRF_TLB = static_cast<const PureBoundingVolumeHierarchy*>(tree_BRF->getChildren()[PureOctree::TOP    | PureOctree::LEFT  | PureOctree::BACK ]);
        const PureBoundingVolumeHierarchy* tree_BRF_BLB = static_cast<const PureBoundingVolumeHierarchy*>(tree_BRF->getChildren()[PureOctree::BOTTOM | PureOctree::LEFT  | PureOctree::BACK ]);
        const PureBoundingVolumeHierarchy* tree_BRF_TRB = static_cast<const PureBoundingVolumeHierarchy*>(tree_BRF->getChildren()[PureOctree::TOP    | PureOctree::RIGHT | PureOctree::BACK ]);
        const PureBoundingVolumeHierarchy* tree_BRF_BRB = static_cast<const PureBoundingVolumeHierarchy*>(tree_BRF->getChildren()[PureOctree::BOTTOM | PureOctree::RIGHT | PureOctree::BACK ]);

        // checking bounding boxes bottom-up

        // level-2 bounds
        const PureAxisAlignedBoundingBox aabb_obj2(obj2->getPosVec(), obj2->getScaledSizeVec());
        const PureAxisAlignedBoundingBox aabb_obj3(obj3->getPosVec(), obj3->getScaledSizeVec());
        PureAxisAlignedBoundingBox aabb_BRF_TLB_expected;
        aabb_BRF_TLB_expected.ExtendBy(aabb_obj2);
        aabb_BRF_TLB_expected.ExtendBy(aabb_obj3);

        bool b = assertEquals(aabb_BRF_TLB_expected.getPosVec(), tree_BRF_TLB->getAABB().getPosVec(), "tree_BRF_TLB AABB pos") &
            assertEquals(aabb_BRF_TLB_expected.getSizeVec(), tree_BRF_TLB->getAABB().getSizeVec(), "tree_BRF_TLB AABB size");

        b &= assertEquals(PureVector(), tree_BRF_TLF->getAABB().getPosVec(), "tree_BRF_TLF AABB pos") &
            assertEquals(PureVector(), tree_BRF_TLF->getAABB().getSizeVec(), "tree_BRF_TLF AABB size") &
            assertEquals(PureVector(), tree_BRF_BLF->getAABB().getPosVec(),  "tree_BRF_BLF AABB pos") &
            assertEquals(PureVector(), tree_BRF_BLF->getAABB().getSizeVec(), "tree_BRF_BLF AABB size") &
            assertEquals(PureVector(), tree_BRF_TRF->getAABB().getPosVec(),  "tree_BRF_TRF AABB pos") &
            assertEquals(PureVector(), tree_BRF_TRF->getAABB().getSizeVec(), "tree_BRF_TRF AABB size") &
            assertEquals(PureVector(), tree_BRF_BRF->getAABB().getPosVec(),  "tree_BRF_BRF AABB pos") &
            assertEquals(PureVector(), tree_BRF_BRF->getAABB().getSizeVec(), "tree_BRF_BRF AABB size") &
            assertEquals(PureVector(), tree_BRF_BLB->getAABB().getPosVec(),  "tree_BRF_BLB AABB pos") &
            assertEquals(PureVector(), tree_BRF_BLB->getAABB().getSizeVec(), "tree_BRF_BLB AABB size") &
            assertEquals(PureVector(), tree_BRF_TRB->getAABB().getPosVec(),  "tree_BRF_TRB AABB pos") &
            assertEquals(PureVector(), tree_BRF_TRB->getAABB().getSizeVec(), "tree_BRF_TRB AABB size") &
            assertEquals(PureVector(), tree_BRF_BRB->getAABB().getPosVec(),  "tree_BRF_BRB AABB pos") &
            assertEquals(PureVector(), tree_BRF_BRB->getAABB().getSizeVec(), "tree_BRF_BRB AABB size");

        // level-1 bounds
        const PureAxisAlignedBoundingBox aabb_TLB_expected(obj1->getPosVec(), obj1->getScaledSizeVec());

        b &= assertEquals(aabb_TLB_expected.getPosVec(), tree_TLB->getAABB().getPosVec(), "tree_TLB AABB pos") &
            assertEquals(aabb_TLB_expected.getSizeVec(), tree_TLB->getAABB().getSizeVec(), "tree_TLB AABB size");

        const PureAxisAlignedBoundingBox aabb_TRB_expected(obj4->getPosVec(), obj4->getScaledSizeVec());

        b &= assertEquals(aabb_TRB_expected.getPosVec(), tree_TRB->getAABB().getPosVec(),  "tree_TRB AABB pos") &
            assertEquals(aabb_TRB_expected.getSizeVec(), tree_TRB->getAABB().getSizeVec(), "tree_TRB AABB size");

        PureAxisAlignedBoundingBox aabb_BRF_expected;
        aabb_BRF_expected.ExtendBy(aabb_obj2);
        aabb_BRF_expected.ExtendBy(aabb_obj3);

        b &= assertEquals(aabb_BRF_expected.getPosVec(), tree_BRF->getAABB().getPosVec(),  "tree_BRF AABB pos") &
            assertEquals(aabb_BRF_expected.getSizeVec(), tree_BRF->getAABB().getSizeVec(), "tree_BRF AABB size");

        b &= assertEquals(PureVector(), tree_TLF->getAABB().getPosVec(), "tree_TLF AABB pos") &
            assertEquals(PureVector(), tree_TLF->getAABB().getSizeVec(), "tree_TLF AABB size") &
            assertEquals(PureVector(), tree_BLF->getAABB().getPosVec(),  "tree_BLF AABB pos") &
            assertEquals(PureVector(), tree_BLF->getAABB().getSizeVec(), "tree_BLF AABB size") &
            assertEquals(PureVector(), tree_TRF->getAABB().getPosVec(),  "tree_TRF AABB pos") &
            assertEquals(PureVector(), tree_TRF->getAABB().getSizeVec(), "tree_TRF AABB size") &
            assertEquals(PureVector(), tree_BLB->getAABB().getPosVec(),  "tree_BLB AABB pos") &
            assertEquals(PureVector(), tree_BLB->getAABB().getSizeVec(), "tree_BLB AABB size") &
            assertEquals(PureVector(), tree_BRB->getAABB().getPosVec(),  "tree_BRB AABB pos") &
            assertEquals(PureVector(), tree_BRB->getAABB().getSizeVec(), "tree_BRB AABB size");

        // level-0 bounds
        PureAxisAlignedBoundingBox aabb_root_expected(aabb_TLB_expected);
        aabb_root_expected.ExtendBy(aabb_TRB_expected);
        aabb_root_expected.ExtendBy(aabb_BRF_expected);

        b &= assertEquals(aabb_root_expected.getPosVec(), tree.getAABB().getPosVec(),  "root AABB pos") &
            assertEquals(aabb_root_expected.getSizeVec(), tree.getAABB().getSizeVec(), "root AABB size");

        return b;
    }

}; // class PureBoundingVolumeHierarchyTest