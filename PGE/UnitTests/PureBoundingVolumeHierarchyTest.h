#pragma once

/*
    ###################################################################################
    PureBoundingVolumeHierarchyTest.h
    Unit test for PureBoundingVolumeHierarchy.
    Made by PR00F88
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../Pure/include/external/SpatialStructures/PureBoundingVolumeHierarchy.h"

class PureBoundingVolumeHierarchyTest :
    public UnitTest
{
public:

    PureBoundingVolumeHierarchyTest() :
        UnitTest( __FILE__ )
    {
    } // PureBoundingVolumeHierarchyTest()

    ~PureBoundingVolumeHierarchyTest() = default;

    PureBoundingVolumeHierarchyTest(const PureBoundingVolumeHierarchyTest&) = delete;
    PureBoundingVolumeHierarchyTest& operator=(const PureBoundingVolumeHierarchyTest&) = delete;
    PureBoundingVolumeHierarchyTest(PureBoundingVolumeHierarchyTest&&) = delete;
    PureBoundingVolumeHierarchyTest& operator=(PureBoundingVolumeHierarchyTest&&) = delete;

protected:

    virtual void initialize() override
    {
        engine = NULL;
        om = NULL;
        addSubTest("testCtor1", (PFNUNITSUBTEST) &PureBoundingVolumeHierarchyTest::testCtor1);
        addSubTest("testCtor2", (PFNUNITSUBTEST) &PureBoundingVolumeHierarchyTest::testCtor2);
        addSubTest("testInsertObject", (PFNUNITSUBTEST) &PureBoundingVolumeHierarchyTest::testInsertObject);
        addSubTest("testFindTightestFittingNode", (PFNUNITSUBTEST)&PureBoundingVolumeHierarchyTest::testFindTightestFittingNode);
        addSubTest("testFindOneColliderObject", (PFNUNITSUBTEST)&PureBoundingVolumeHierarchyTest::testFindOneColliderObject);
        addSubTest("testFindAllColliderObjects", (PFNUNITSUBTEST)&PureBoundingVolumeHierarchyTest::testFindAllColliderObjects);
    }

    virtual bool setUp() override
    {
        bool ret = true;
        if ( engine == NULL )
        {
            PGEInputHandler& inputHandler = PGEInputHandler::createAndGet(cfgProfiles);
            engine = &PR00FsUltimateRenderingEngine::createAndGet(cfgProfiles, inputHandler);
            ret &= assertEquals((TPureUInt)0, engine->initialize(PURE_RENDERER_HW_FP, 800, 600, PURE_WINDOWED, 0, 32, 24, 0, 0), "engine");  // pretty standard display mode, should work on most systems
            om = &engine->getObject3DManager();
            ret &= assertNotNull(om, "om null");
        }
        return ret;
    }

    virtual void tearDown() override
    {
        finalize();
        om = NULL;
    }

    virtual void finalize() override
    {
        if ( engine )
        {
            engine->shutdown();
            engine = NULL;
        }
    }

private:
    PR00FsUltimateRenderingEngine* engine; // need the engine for creating Object3D instances
    PureObject3DManager* om;
    PGEcfgProfiles cfgProfiles;

    // ---------------------------------------------------------------------------

    /**
    * Helper function to build up the given empty tree.
    * Creates 5 new PureObject3D boxes with different positions and try to insert them into the tree.
    * Note that obj5 cannot be inserted into the tree (on purpose) hence there is node5 in the argument list.
    * So the expectation is that after calling this function, only 4 PureObject3D boxes will be inside the tree.
    * The given pointers will be updated.
    * 
    * @param tree       An empty input tree where we will try to insert 5 new boxes.
    * @param treeOrigin This will be set as the position of the tree.
    * @param obj1       A new box will be created and this pointer will be updated with the address of this new box.
    * @param obj2       Same purpose as obj1.
    * @param obj3       Same purpose as obj1.
    * @param obj4       Same purpose as obj1.
    * @param obj5       Same purpose as obj1.
    * @param node1      The tree node holding obj1.
    * @param node2      The tree node holding obj2.
    * @param node3      The tree node holding obj3.
    * @param node4      The tree node holding obj4.
    * 
    * @return True if BVH is successfully built with the 4 new boxes inserted, false otherwise.
    */
    bool buildBVH(
        PureBoundingVolumeHierarchy& tree,
        const PureVector& treeOrigin,
        PureObject3D*& obj1,
        PureObject3D*& obj2, 
        PureObject3D*& obj3, 
        PureObject3D*& obj4, 
        PureObject3D*& obj5,
        PureBoundingVolumeHierarchy*& node1,
        PureBoundingVolumeHierarchy*& node2, 
        PureBoundingVolumeHierarchy*& node3, 
        PureBoundingVolumeHierarchy*& node4)
    {
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

        obj1 = om->createBox(2.0f, 2.0f, 2.0f);
        obj2 = om->createBox(2.0f, 2.0f, 2.0f);
        obj3 = om->createBox(2.0f, 2.0f, 2.0f);
        obj4 = om->createBox(2.0f, 2.0f, 2.0f);
        obj5 = om->createBox(2.0f, 2.0f, 2.0f);
        if (!assertNotNull(obj1, "obj1 not null") || !assertNotNull(obj2, "obj2 not null") || !assertNotNull(obj3, "obj3 not null") || !assertNotNull(obj4, "obj4 not null") || !assertNotNull(obj5, "obj5 not null"))
        {
            return false;
        }

        obj1->getPosVec().Set(treeOrigin.getX() - 200.f, treeOrigin.getY() + 200.f, treeOrigin.getZ() + 200.f);
        obj2->getPosVec().Set(treeOrigin.getX() + 200.f, treeOrigin.getY() - 200.f, treeOrigin.getZ() - 200.f);
        obj3->getPosVec().Set(treeOrigin.getX() + 200.f, treeOrigin.getY() - 200.f, treeOrigin.getZ() - 200.f);
        obj4->getPosVec().Set(treeOrigin.getX(), treeOrigin.getY(), treeOrigin.getZ());
        obj5->getPosVec().Set(treeOrigin.getX() + 3000.f, treeOrigin.getY(), treeOrigin.getZ());

        node1 = tree.insertObject(*obj1);
        node2 = tree.insertObject(*obj2);
        node3 = tree.insertObject(*obj3);
        node4 = tree.insertObject(*obj4);
        const PureBoundingVolumeHierarchy* node5 = tree.insertObject(*obj5);  // out of tree bounds
        if (!assertNotNull(node1, "node 1 insert") ||
            !assertNotNull(node2, "node 2 insert") ||
            !assertNotNull(node3, "node 3 insert") ||
            !assertNotNull(node4, "node 4 insert") ||
            !assertNull(node5, "node 5 insert"))
        {
            return false;
        }

        // since nodes might be subdivided and objects might be moved down to another level when more objects are inserted, the above node ptrs need to be refreshed now!
        node1 = tree.findObject(*obj1);
        node2 = tree.findObject(*obj2);
        node3 = tree.findObject(*obj3);
        node4 = tree.findObject(*obj4);
        node5 = tree.findObject(*obj5);
        if (!assertNotNull(node1, "node 1 find") ||
            !assertNotNull(node2, "node 2 find") ||
            !assertNotNull(node3, "node 3 find") ||
            !assertNotNull(node4, "node 4 find") ||
            !assertNull(node5, "node 5 find") ||
            !assertTrue(node2 == node3, "obj2 and obj3 in same node") ||
            !assertTrue(node1 != node4, "obj1 and obj4 in different node") ||
            !assertTrue(node1 != node2, "obj1 and obj2,3 in different node") ||
            !assertTrue(node4 != node2, "obj4 and obj2,3 in different node") ||
            !assertEquals(1u, node1->getDepthLevel(), "node 1 depth") ||
            !assertEquals(1u, node4->getDepthLevel(), "node 4 depth") ||
            !assertEquals(2u, node2->getDepthLevel(), "node 2,3 depth"))
        {
            return false;
        }

        if (!assertEquals(PureOctree::NodeType::Parent, tree.getNodeType(), "tree nodeType"))
        {
            return false;
        }

        return true;
    }

    bool assertTreeIsReset(const PureBoundingVolumeHierarchy& tree)
    {
        return (assertEquals(PureOctree::NodeType::LeafEmpty, tree.getNodeType(), "nodeType") &
            assertEquals((std::size_t)0, tree.getChildren().size(), "children") &
            assertEquals((std::size_t)0, tree.getObjects().size(), "objects") &
            assertEquals(PureVector(), tree.getAABB().getPosVec(), "aabb pos") &
            assertEquals(PureVector(), tree.getAABB().getSizeVec(), "aabb size")) != 0;
    }

    bool testCtor1()
    {
        PureBoundingVolumeHierarchy tree(3, 0);

        return (assertEquals(0.0f, tree.getPos().getX(), "pos.x") &
            assertEquals(0.0f, tree.getPos().getY(), "pos.y") &
            assertEquals(0.0f, tree.getPos().getZ(), "pos.z") &
            assertEquals((TPureUInt)3, tree.getMaxDepthLevel(), "max depth level") &
            assertEquals((TPureUInt)0, tree.getDepthLevel(), "root node depth level") &
            assertEquals(0.f, tree.getSize(), "size") &
            assertTrue(assertTreeIsReset(tree), "reset")) != 0;
    }

    bool testCtor2()
    {
        PureBoundingVolumeHierarchy tree(PureVector(1, 2, 3), 1000.0f, 3, 0);
        
        return (assertEquals(1.0f, tree.getPos().getX(), "pos.x") &
        assertEquals(2.0f, tree.getPos().getY(), "pos.y") &
        assertEquals(3.0f, tree.getPos().getZ(), "pos.z") &
        assertEquals((TPureUInt)3, tree.getMaxDepthLevel(), "max depth level") &
        assertEquals((TPureUInt)0, tree.getDepthLevel(), "root node depth level") &
        assertEquals(1000.0f, tree.getSize(), "size") &
        assertTrue(assertTreeIsReset(tree), "reset")) != 0;
    }

    bool testInsertObject()
    {
        // basically this is the same test as found in PureOctreeTest.h, but here we are interested in the
        // BVH-related stuff only

        // used to test if the given world-space position other than (0,0,0) is really taken into calculations;
        // if there is any issue, change this to (0,0,0) to find out probable reason of tree not properly taking origin pos into account!
        const PureVector treeOrigin(100.f, 200.f, 300.f);

        PureObject3D* obj1 = nullptr;
        PureObject3D* obj2 = nullptr;
        PureObject3D* obj3 = nullptr;
        PureObject3D* obj4 = nullptr;
        PureObject3D* obj5 = nullptr;
        PureBoundingVolumeHierarchy* node1 = nullptr;
        PureBoundingVolumeHierarchy* node2 = nullptr;
        PureBoundingVolumeHierarchy* node3 = nullptr;
        PureBoundingVolumeHierarchy* node4 = nullptr;
        PureBoundingVolumeHierarchy tree(treeOrigin, 1000.0f, 2, 0);
        if (!assertTrue(buildBVH(tree, treeOrigin, obj1, obj2, obj3, obj4, obj5, node1, node2, node3, node4), "buildBVH"))
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

        bool b = true;
        b &= assertEquals(aabb_BRF_TLB_expected.getPosVec(), tree_BRF_TLB->getAABB().getPosVec(), "tree_BRF_TLB AABB pos") &
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

        // reset(): compared to the PureOctree test, we only test if AABB is also reset (see in assertTreeIsReset())!
        b &= assertTrue(tree.reset(), "reset level 0");
        b &= assertTrue(assertTreeIsReset(tree), "tree is reset 1");

        return b;
    }

    bool testFindTightestFittingNode()
    {
        // used to test if the given world-space position other than (0,0,0) is really taken into calculations;
        // if there is any issue, change this to (0,0,0) to find out probable reason of tree not properly taking origin pos into account!
        const PureVector treeOrigin(100.f, 200.f, 300.f);

        PureObject3D* obj1 = nullptr;
        PureObject3D* obj2 = nullptr;
        PureObject3D* obj3 = nullptr;
        PureObject3D* obj4 = nullptr;
        PureObject3D* obj5 = nullptr;
        PureBoundingVolumeHierarchy* node1 = nullptr;
        PureBoundingVolumeHierarchy* node2 = nullptr;
        PureBoundingVolumeHierarchy* node3 = nullptr;
        PureBoundingVolumeHierarchy* node4 = nullptr;
        PureBoundingVolumeHierarchy tree(treeOrigin, 1000.0f, 2, 0);
        if (!assertTrue(buildBVH(tree, treeOrigin, obj1, obj2, obj3, obj4, obj5, node1, node2, node3, node4), "buildBVH"))
        {
            return false;
        }

        bool b = assertTrue(node1 == tree.findTightestFittingNode(*obj1), "find 1 from root 1");
        b &= assertTrue(node2 == tree.findTightestFittingNode(*obj2), "find 2 from root 2");
        b &= assertTrue(node3 == tree.findTightestFittingNode(*obj3), "find 3 from root 3");
        b &= assertTrue(node4 == tree.findTightestFittingNode(*obj4), "find 4 from root 4");
        b &= assertTrue(nullptr == tree.findTightestFittingNode(*obj5), "find 5 from root 5");

        b &= assertTrue(node1 == node1->findTightestFittingNode(*obj1), "find 1 from node 1");
        b &= assertTrue(node2 == node2->findTightestFittingNode(*obj2), "find 2 from node 2");
        b &= assertTrue(node3 == node3->findTightestFittingNode(*obj3), "find 3 from node 3");
        b &= assertTrue(node4 == node4->findTightestFittingNode(*obj4), "find 4 from node 4");


        return b;
    }

    bool testFindOneColliderObject()
    {
        const PureVector treeOrigin(100.f, 200.f, 300.f);

        PureObject3D* obj1 = nullptr;
        PureObject3D* obj2 = nullptr;
        PureObject3D* obj3 = nullptr;
        PureObject3D* obj4 = nullptr;
        PureObject3D* obj5 = nullptr;
        PureBoundingVolumeHierarchy* node1 = nullptr;
        PureBoundingVolumeHierarchy* node2 = nullptr;
        PureBoundingVolumeHierarchy* node3 = nullptr;
        PureBoundingVolumeHierarchy* node4 = nullptr;
        PureBoundingVolumeHierarchy tree(treeOrigin, 1000.0f, 2, 0);
        if (!assertTrue(buildBVH(tree, treeOrigin, obj1, obj2, obj3, obj4, obj5, node1, node2, node3, node4), "buildBVH"))
        {
            return false;
        }

        bool b = true;

        // obj5 is outside of tree bounds, hence it cannot collide with anything within the tree
        b &= assertNull(tree.findOneColliderObject(*obj5), "obj5 collider, object version");
        
        const PureAxisAlignedBoundingBox obj5Aabb(obj5->getPosVec(), obj5->getScaledSizeVec());
        b &= assertNull(tree.findOneColliderObject(obj5Aabb, nullptr), "obj5 collider, aabb version");

        // obj1, being in the tree, is expected to be colliding with itself
        const PureObject3D* const pObj1CollidingWith = tree.findOneColliderObject(*obj1);
        b &= assertEquals(obj1, pObj1CollidingWith, "obj1 collider, object version");
        
        const PureAxisAlignedBoundingBox obj1Aabb(obj1->getPosVec(), obj1->getScaledSizeVec());
        b &= assertEquals(pObj1CollidingWith, tree.findOneColliderObject(obj1Aabb, nullptr), "obj1 collider, aabb version");

        // obj2, being in the tree, is expected to be colliding with itself or obj3 as they have same position and size!
        const PureObject3D* const pObj2CollidingWith = tree.findOneColliderObject(*obj2);
        b &= assertTrue((obj2 == pObj2CollidingWith) || (obj3 == pObj2CollidingWith), "obj2 collider, object version");
        
        const PureAxisAlignedBoundingBox obj2Aabb(obj2->getPosVec(), obj2->getScaledSizeVec());
        const PureObject3D* const pObj2CollidingWithAabb = tree.findOneColliderObject(obj2Aabb, nullptr);
        b &= assertTrue((obj2 == pObj2CollidingWithAabb) || (obj3 == pObj2CollidingWithAabb), "obj2 collider, aabb version");

        // obj3, same story
        const PureObject3D* const pObj3CollidingWith = tree.findOneColliderObject(*obj3);
        b &= assertTrue((obj2 == pObj3CollidingWith) || (obj3 == pObj3CollidingWith), "obj3 collider, object version");

        const PureAxisAlignedBoundingBox obj3Aabb(obj3->getPosVec(), obj3->getScaledSizeVec());
        const PureObject3D* const pObj3CollidingWithAabb = tree.findOneColliderObject(obj3Aabb, nullptr);
        b &= assertTrue((obj2 == pObj3CollidingWithAabb) || (obj3 == pObj3CollidingWithAabb), "obj3 collider, aabb version");

        // obj4, being in the tree, is expected to be colliding with itself
        const PureObject3D* const pObj4CollidingWith = tree.findOneColliderObject(*obj4);
        b &= assertEquals(obj4, pObj4CollidingWith, "obj4 collider, object version");

        const PureAxisAlignedBoundingBox obj4Aabb(obj4->getPosVec(), obj4->getScaledSizeVec());
        b &= assertEquals(pObj4CollidingWith, tree.findOneColliderObject(obj4Aabb, nullptr), "obj4 collider, aabb version");

        return b;
    }

    bool testFindAllColliderObjects()
    {
        const PureVector treeOrigin(100.f, 200.f, 300.f);

        PureObject3D* obj1 = nullptr;
        PureObject3D* obj2 = nullptr;
        PureObject3D* obj3 = nullptr;
        PureObject3D* obj4 = nullptr;
        PureObject3D* obj5 = nullptr;
        PureBoundingVolumeHierarchy* node1 = nullptr;
        PureBoundingVolumeHierarchy* node2 = nullptr;
        PureBoundingVolumeHierarchy* node3 = nullptr;
        PureBoundingVolumeHierarchy* node4 = nullptr;
        PureBoundingVolumeHierarchy tree(treeOrigin, 1000.0f, 2, 0);
        if (!assertTrue(buildBVH(tree, treeOrigin, obj1, obj2, obj3, obj4, obj5, node1, node2, node3, node4), "buildBVH"))
        {
            return false;
        }

        bool b = true;

        std::vector<const PureObject3D*> colliders;

        // obj5 is outside of tree bounds, hence it cannot collide with anything within the tree
        b &= assertFalse(tree.findAllColliderObjects(*obj5, colliders), "obj5 colliders 0, object version");
        b &= assertTrue(colliders.empty(), "obj5 colliders empty, object version");

        const PureAxisAlignedBoundingBox obj5Aabb(obj5->getPosVec(), obj5->getScaledSizeVec());
        b &= assertFalse(tree.findAllColliderObjects(obj5Aabb, nullptr, colliders), "obj5 colliders 0, aabb version");
        b &= assertTrue(colliders.empty(), "obj5 colliders empty, aabb version");

        // obj1, being in the tree, is expected to be colliding with itself
        colliders.clear();
        b &= assertTrue(tree.findAllColliderObjects(*obj1, colliders), "obj1 colliders 1, object version");
        b &= assertFalse(colliders.empty(), "obj1 colliders empty, object version");
        if (b)
        {
            b &= assertEquals(obj1, *colliders.begin(), "obj1 colliders exact check, object version");
        }

        colliders.clear();
        const PureAxisAlignedBoundingBox obj1Aabb(obj1->getPosVec(), obj1->getScaledSizeVec());
        b &= assertTrue(tree.findAllColliderObjects(obj1Aabb, nullptr, colliders), "obj1 colliders 1, aabb version");
        b &= assertFalse(colliders.empty(), "obj1 colliders empty, aabb version");
        if (b)
        {
            b &= assertEquals(obj1, *colliders.begin(), "obj1 colliders exact check, aabb version");
        }

        // obj2, being in the tree, is expected to be colliding with itself and obj3 as they have same position and size!
        colliders.clear();
        b &= assertTrue(tree.findAllColliderObjects(*obj2, colliders), "obj2 colliders 1, object version");
        b &= assertEquals(2u, colliders.size(), "obj2 colliders size, object version");
        if (b)
        {
            b &= assertTrue(std::find(colliders.begin(), colliders.end(), obj2) != colliders.end(), "obj2 colliders exact check 1, object version");
            b &= assertTrue(std::find(colliders.begin(), colliders.end(), obj3) != colliders.end(), "obj2 colliders exact check 2, object version");
        }

        colliders.clear();
        const PureAxisAlignedBoundingBox obj2Aabb(obj2->getPosVec(), obj2->getScaledSizeVec());
        b &= assertTrue(tree.findAllColliderObjects(obj2Aabb, nullptr, colliders), "obj2 colliders 1, aabb version");
        b &= assertEquals(2u, colliders.size(), "obj2 colliders size, aabb version");
        if (b)
        {
            b &= assertTrue(std::find(colliders.begin(), colliders.end(), obj2) != colliders.end(), "obj2 colliders exact check 1, aabb version");
            b &= assertTrue(std::find(colliders.begin(), colliders.end(), obj3) != colliders.end(), "obj2 colliders exact check 2, aabb version");
        }

        // same story for obj3
        colliders.clear();
        b &= assertTrue(tree.findAllColliderObjects(*obj3, colliders), "obj3 colliders 1, object version");
        b &= assertEquals(2u, colliders.size(), "obj3 colliders size, object version");
        if (b)
        {
            b &= assertTrue(std::find(colliders.begin(), colliders.end(), obj2) != colliders.end(), "obj3 colliders exact check 1, object version");
            b &= assertTrue(std::find(colliders.begin(), colliders.end(), obj3) != colliders.end(), "obj3 colliders exact check 2, object version");
        }

        colliders.clear();
        const PureAxisAlignedBoundingBox obj3Aabb(obj3->getPosVec(), obj3->getScaledSizeVec());
        b &= assertTrue(tree.findAllColliderObjects(obj3Aabb, nullptr, colliders), "obj3 colliders 1, aabb version");
        b &= assertEquals(2u, colliders.size(), "obj3 colliders size, aabb version");
        if (b)
        {
            b &= assertTrue(std::find(colliders.begin(), colliders.end(), obj2) != colliders.end(), "obj3 colliders exact check 1, aabb version");
            b &= assertTrue(std::find(colliders.begin(), colliders.end(), obj3) != colliders.end(), "obj3 colliders exact check 2, aabb version");
        }

        // obj4, being in the tree, is expected to be colliding with itself
        colliders.clear();
        b &= assertTrue(tree.findAllColliderObjects(*obj4, colliders), "obj4 colliders 1, object version");
        b &= assertFalse(colliders.empty(), "obj4 colliders empty, object version");
        if (b)
        {
            b &= assertEquals(obj4, *colliders.begin(), "obj4 colliders exact check, object version");
        }

        colliders.clear();
        const PureAxisAlignedBoundingBox obj4Aabb(obj4->getPosVec(), obj4->getScaledSizeVec());
        b &= assertTrue(tree.findAllColliderObjects(obj4Aabb, nullptr, colliders), "obj4 colliders 1, aabb version");
        b &= assertFalse(colliders.empty(), "obj4 colliders empty, aabb version");
        if (b)
        {
            b &= assertEquals(obj4, *colliders.begin(), "obj4 colliders exact check, aabb version");
        }

        // and now we create a very big object which shall be colliding with all objects in the tree
        PureObject3D* const objBig = om->createBox(1.0f, 1.0f, 1.0f);  // will scale it
        b &= assertNotNull(objBig, "objBig created");
        if (b)
        {
            objBig->getPosVec() = treeOrigin;
            objBig->SetScaling(tree.getSize());  // intentionally scaling to see if scaled size is used for collision calculation
            
            colliders.clear();
            b &= assertTrue(tree.findAllColliderObjects(*objBig, colliders), "objBig colliders 1, object version");
            b &= assertEquals(4u, colliders.size(), "objBig colliders size, object version");
            if (b)
            {
                b &= assertTrue(std::find(colliders.begin(), colliders.end(), obj1) != colliders.end(), "objBig colliders exact check 1, object version");
                b &= assertTrue(std::find(colliders.begin(), colliders.end(), obj2) != colliders.end(), "objBig colliders exact check 2, object version");
                b &= assertTrue(std::find(colliders.begin(), colliders.end(), obj3) != colliders.end(), "objBig colliders exact check 3, object version");
                b &= assertTrue(std::find(colliders.begin(), colliders.end(), obj4) != colliders.end(), "objBig colliders exact check 4, object version");
            }

            colliders.clear();
            const PureAxisAlignedBoundingBox objBigAabb(objBig->getPosVec(), objBig->getScaledSizeVec());
            b &= assertTrue(tree.findAllColliderObjects(objBigAabb, nullptr, colliders), "objBig colliders 1, aabb version");
            b &= assertEquals(4u, colliders.size(), "objBig colliders size, aabb version");
            if (b)
            {
                b &= assertTrue(std::find(colliders.begin(), colliders.end(), obj1) != colliders.end(), "objBig colliders exact check 1, object version");
                b &= assertTrue(std::find(colliders.begin(), colliders.end(), obj2) != colliders.end(), "objBig colliders exact check 2, object version");
                b &= assertTrue(std::find(colliders.begin(), colliders.end(), obj3) != colliders.end(), "objBig colliders exact check 3, object version");
                b &= assertTrue(std::find(colliders.begin(), colliders.end(), obj4) != colliders.end(), "objBig colliders exact check 4, object version");
            }
        }

        return b;
    }

}; // class PureBoundingVolumeHierarchyTest