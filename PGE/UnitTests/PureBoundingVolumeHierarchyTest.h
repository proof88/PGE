#pragma once

/*
    ###################################################################################
    PureBoundingVolumeHierarchyTest.h
    Unit test for PureBoundingVolumeHierarchy.
    Made by PR00F88
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../PGE.h"
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
        addSubTest("testSetPosVecAffectsAabb", (PFNUNITSUBTEST)&PureBoundingVolumeHierarchyTest::testSetPosVecAffectsAabb);
        addSubTest("testSetSizeAffectsAabb", (PFNUNITSUBTEST)&PureBoundingVolumeHierarchyTest::testSetSizeAffectsAabb);
        addSubTest("testInsertObject", (PFNUNITSUBTEST) &PureBoundingVolumeHierarchyTest::testInsertObject);
        addSubTest("testFindOneLowestLevelFittingNode_DownFromRootNode", (PFNUNITSUBTEST)&PureBoundingVolumeHierarchyTest::testFindOneLowestLevelFittingNode_DownFromRootNode);
        addSubTest("testFindOneLowestLevelFittingNode_UpFromLeafNode", (PFNUNITSUBTEST)&PureBoundingVolumeHierarchyTest::testFindOneLowestLevelFittingNode_UpFromLeafNode);
        addSubTest(
            "testFindOneColliderObject_1_startFromLowestLevelFittingNode",
            (PFNUNITSUBTEST)&PureBoundingVolumeHierarchyTest::testFindOneColliderObject_1_startFromLowestLevelFittingNode);
        addSubTest(
            "testFindAllColliderObjects_1_startFromLowestLevelFittingNode",
            (PFNUNITSUBTEST)&PureBoundingVolumeHierarchyTest::testFindAllColliderObjects_1_startFromLowestLevelFittingNode);
        addSubTest(
            "testFindOneColliderObject_2_startFromLowestLevelFittingNode",
            (PFNUNITSUBTEST)&PureBoundingVolumeHierarchyTest::testFindOneColliderObject_2_startFromLowestLevelFittingNode);
        addSubTest(
            "testFindAllColliderObjects_2_startFromLowestLevelFittingNode",
            (PFNUNITSUBTEST)&PureBoundingVolumeHierarchyTest::testFindAllColliderObjects_2_startFromLowestLevelFittingNode);
        addSubTest("testFindOneColliderObject_1_startFromFirstNode", (PFNUNITSUBTEST)&PureBoundingVolumeHierarchyTest::testFindOneColliderObject_1_startFromFirstNode);
        addSubTest("testFindAllColliderObjects_1_startFromFirstNode", (PFNUNITSUBTEST)&PureBoundingVolumeHierarchyTest::testFindAllColliderObjects_1_startFromFirstNode);
        addSubTest("testFindOneColliderObject_2_startFromFirstNode", (PFNUNITSUBTEST)&PureBoundingVolumeHierarchyTest::testFindOneColliderObject_2_startFromFirstNode);
        addSubTest("testFindAllColliderObjects_2_startFromFirstNode", (PFNUNITSUBTEST)&PureBoundingVolumeHierarchyTest::testFindAllColliderObjects_2_startFromFirstNode);
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
    bool buildBVH_1(
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

    /**
    * Helper function to build up the given empty tree.
    * This specialized version builds up a tree with size and objects in a way to mimic the
    * object of map_test_good_2_collision.txt from PRooFPS-dd which contains the minimal map layout
    * to reproduce the collision issue that was present in PRooFPS-dd v0.5 with BVH method.
    *
    * @param tree          An empty input tree where we will try to insert 5 new boxes.
    * @param treeOrigin    This will be set as the position of the tree.
    * @param obj1          A new box will be created and this pointer will be updated with the address of this new box.
    * @param obj2          Same purpose as obj1.
    * @param obj3_stair_1  Same purpose as obj1, but this is a smallstairstep piece (the bottom) of stair object.
    * @param obj3_stair_2  Same purpose as obj1, but this is a smallstairstep piece of stair object.
    * @param obj3_stair_3  Same purpose as obj1, but this is a smallstairstep piece of stair object.
    * @param obj3_stair_4  Same purpose as obj1, but this is a smallstairstep piece (the top) of stair object.
    * @param obj4          Same purpose as obj1.
    * @param node1         The tree node holding obj1.
    * @param node2         The tree node holding obj2.
    * @param node3_stair_1 The tree node holding obj3_stair_1.
    * @param node3_stair_2 The tree node holding obj3_stair_2.
    * @param node3_stair_3 The tree node holding obj3_stair_3.
    * @param node3_stair_4 The tree node holding obj3_stair_4.
    * @param node4         The tree node holding obj4.
    *
    * @return True if BVH is successfully built, false otherwise.
    */
    bool buildBVH_2(
        PureBoundingVolumeHierarchy& tree,
        const PureVector& /*treeOrigin*/,
        PureObject3D*& obj1,
        PureObject3D*& obj2,
        PureObject3D*& obj3_stair_1,
        PureObject3D*& obj3_stair_2,
        PureObject3D*& obj3_stair_3,
        PureObject3D*& obj3_stair_4,
        PureObject3D*& obj4,
        PureBoundingVolumeHierarchy*& node1,
        PureBoundingVolumeHierarchy*& node2,
        PureBoundingVolumeHierarchy*& node3_stair_1,
        PureBoundingVolumeHierarchy*& node3_stair_2,
        PureBoundingVolumeHierarchy*& node3_stair_3,
        PureBoundingVolumeHierarchy*& node3_stair_4,
        PureBoundingVolumeHierarchy*& node4)
    {
        /*

        This is a 85 x 85 x 85 size octree, in pos treeOrigin.
        Max tree depth is set to 3.
        Copied layout from map_test_good_2_collision.txt:

          e                                                                                   e
          e
          e
          e
          e
          e
          e
          e
          e
          e
          e
          e
          e
          e
          e
          e
          e
          e
          e
          e
          e                            F
          e
          e
          e
          e
          e                         S
          e
          e                      F /F
          e
          e
          e
          e

        As we can see there are only 3 regular unit-sized foreground blocks and 1 stairs block that is made up from 4 smaller foreground blocks.

        Inserting the objects into the tree happens like this step-by-step:
         - obj1 insert:
           - root node is LeafEmpty, obj1 is added to objects of the empty root node;
           - tree depth stays 0;

         - obj2 insert: 
           - root node is LeafContainer, we not yet reached depth limit 3, so subdivide root into 8 children nodes:
             - tree depth is now 1;
             - move the already inserted obj1 to the "left bottom front" (1) child node, and:
               parent nodes' AABB are extended recursively up to the root node with obj1's AABB;
           - due to its position, obj2 shall be also inserted into root's "left bottom front" (1) child node, so:
             - we not yet reached depth limit 3, so this child node now needs to be subdivided into 8 child nodes:
               - tree depth is now 2;
               - the already inserted obj1 goes to root's "left bottom front" (1) child node's "right top back" (6) child node, and:
                 parent nodes' AABB are extended recursively up to the root node with obj1's AABB;
           - obj2 node index also gets calculated as root's "left bottom front" (1) child node's "right top back" (6) child node, so:
             - we not yet reached depth limit 3, so this child node now needs to be subdivided into 8 child nodes:
               - tree depth is now 3;
               - the already inserted obj1 goes to root's "left bottom front" (1) child node's "right top back" (6) child node's "left top back" (4)
                 child node, and:
                 parent nodes' AABB are extended recursively up to the root node with obj1's AABB;
           - obj2 is finally inserted to root's "left bottom front" (1) child node's "right top back" (6) child node's "left bottom back" (5) child node, and:
             parent nodes' AABB are extended recursively up to the root node with obj1's AABB.

         - ascending stairs object (obj 3) insert:
           - smallstairstep1 (bottom piece) insert:
             - root node is LeafParent, we pass insertion to its "left bottom front" (1) child node;
             - we are at level 1, and "left bottom front" (1) child node is also LeafParent, we pass insertion to its "right top back" (6) child node;
             - we are at level 2, and this child node is also LeafParent, we pass insertion to its "left bottom back" (5) child node;
             - we are at level 3, and this child node is LeafContainer, and we have already reached max tree depth, therefore we cannot subdivide
               anymore, we are inserting this object into the objects container of this child node.
           - smallstairstep2 insert:
             - exactly same as previous stairstep insertion;
           - smallstairstep3 insert:
             - exactly same as previous stairstep insertion;
           - smallstairstep4 (top piece) insert:
             - root node is LeafParent, we pass insertion to its "left bottom front" (1) child node;
             - we are at level 1, and "left bottom front" (1) child node is also LeafParent, we pass insertion to its "right top back" (6) child node;
             - we are at level 2, and this child node is also LeafParent, we pass insertion to its "left top back" (4) child node;
             - we are at level 3, and this child node is LeafContainer, and we have already reached max tree depth, therefore we cannot subdivide
               anymore, we are inserting this object into the objects container of this child node.

         - obj4 insert:
           - root node is LeafParent, we pass insertion to its "left bottom front" (1) child node;
           - we are at level 1, and "left bottom front" (1) child node is also LeafParent, we pass insertion to its "right top back" (6) child node;
           - we are at level 2, and this child node is also LeafParent, we pass insertion to its "left bottom back" (5) child node;
           - we are at level 3, and this child node is LeafContainer, and we have already reached max tree depth, therefore we cannot subdivide
             anymore, we are inserting this object into the objects container of this child node.

        Summary after all these insertions:
         - root node's "left bottom front" (1) child's "right top back" (6) child's "left bottom back" (5) child contains the following objects:
           obj2, obj3_smallstairstep1, obj3_smallstairstep2, obj3_smallstairstep3, obj4.

         - root node's "left bottom front" (1) child node's "right top back" (6) child node's "left top back" (4) child contains the following objects:
           obj1, obj3_smallstairstep4.

        As we can see from above, AABB of sibling nodes CAN OVERLAP.
        The quadtree or octree idea originally assumes perfect spatial subdivision into non-overlapping children — but when used as a dynamic BVH
        with tight AABBs that grow, you lose the strict non-overlapping guarantee. However, this is true only for the node AABBs maintained by
        BVH logic, so the original node position and sizes at Octree level stay unchanged, providing perfect spatial subdivision always.

        To explain why:
        Since the objects are inserted into nodes based on their position, based on the Octree logic, but their containing node's AABB is adjusted
        based on the object's AABB by the BVH logic, it is not necessary but normal that after the insertion, the position and size of the node is modified, and
        these changes are being reflected upwards to all levels until reaching the root.
        During this expansion propagation upwards, obviously sibling nodes are not modified, not shrinked.

        This is the reason why 2 objects with similar position MIGHT end up in different nodes even if both would initially fall into the same node:
        because after the 1st object is inserted, its container node MIGHT HAVE changed its position too, leading the Octree logic choosing a
        different node for the 2nd object!

        What remains true is that parent nodes always maintain themselves to fully contain all their children nodes, even though children
        of the same parent can overlap with each other within their parent.

        This implies that parents can also overlap with their siblings on their level.

        The initially equal division of space into 8 children AABB nodes can easily become unequal, as after each insertion the position and size of
        the node AABBs might change.
        Similar to parents always fully bound their children nodes, all nodes always fully bound their contained objects using their AABB.

        The tree stays valid for broad-phase culling and hierarchical collision checks.
        This is because for collision check I always start from root node, and the only fact I'm relying on is if a node's AABB does not
        intersect the collision checked object's AABB then none of that node's children nodes are intersecting it either.

        Over time, as more objects are inserted and AABBs keep expanding, the hierarchy can get less tight and that can introduce more
        unnecessary checks, however this can be improved by rebuilding or rebalancing the tree - this is not supported at the moment.

        To conclude, because of sibling BVH nodes can overlap, it is NOT enough to check for collision with the objects within the tightest fitting node, but
        to check for collision with ALL siblings of the tightest fitting node.
        */

        // 2 normal foreground blocks in PRooFPS-dd terms
        obj1 = om->createBox(1.0f, 1.0f, 1.0f);
        obj1->getPosVec().Set(30.f, -20.f, -1.f);

        obj2 = om->createBox(1.0f, 1.0f, 1.0f);
        obj2->getPosVec().Set(24.f, -27.f, -1.f);

        // 1 ascending stairs block ('/') in PRooFPS-dd terms, consisting of 4 different size boxes.
        // starting with bottom piece.
        obj3_stair_1 = om->createBox(1.0f, 0.25f, 1.0f);
        obj3_stair_1->getPosVec().Set(26.f, -27.375f, -1.f);

        obj3_stair_2 = om->createBox(0.75f, 0.25f, 1.0f);
        obj3_stair_2->getPosVec().Set(26.125f, -27.125f, -1.f);

        obj3_stair_3 = om->createBox(0.5f, 0.25f, 1.0f);
        obj3_stair_3->getPosVec().Set(26.25f, -26.875f, -1.f);

        // ending with top piece
        obj3_stair_4 = om->createBox(0.25f, 0.25f, 1.0f);
        obj3_stair_4->getPosVec().Set(26.375f, -26.625f, -1.f);
        
        // 1 more normal foreground block in PRooFPS-dd terms
        obj4 = om->createBox(1.0f, 1.0f, 1.0f);
        obj4->getPosVec().Set(27.f, -27.f, -1.f);

        if (!assertNotNull(obj1, "obj1 not null") ||
            !assertNotNull(obj2, "obj2 not null") ||
            !assertNotNull(obj3_stair_1, "obj3_stair_1 not null") ||
            !assertNotNull(obj3_stair_2, "obj3_stair_2 not null") ||
            !assertNotNull(obj3_stair_3, "obj3_stair_3 not null") ||
            !assertNotNull(obj3_stair_4, "obj3_stair_4 not null") ||
            !assertNotNull(obj4, "obj4 not null"))
        {
            return false;
        }

        node1 = tree.insertObject(*obj1);
        node2 = tree.insertObject(*obj2);
        node3_stair_1 = tree.insertObject(*obj3_stair_1);
        node3_stair_2 = tree.insertObject(*obj3_stair_2);
        node3_stair_3 = tree.insertObject(*obj3_stair_3);
        node3_stair_4 = tree.insertObject(*obj3_stair_4);
        node4 = tree.insertObject(*obj4);
        if (!assertNotNull(obj1, "node1 insert") ||
            !assertNotNull(obj2, "node2 insert") ||
            !assertNotNull(obj3_stair_1, "node3_stair_1 insert") ||
            !assertNotNull(obj3_stair_2, "node3_stair_2 insert") ||
            !assertNotNull(obj3_stair_3, "node3_stair_3 insert") ||
            !assertNotNull(obj3_stair_4, "node3_stair_4 insert") ||
            !assertNotNull(obj4, "node4 insert"))
        {
            return false;
        }

        // since nodes might be subdivided and objects might be moved down to another level when more objects are inserted, the above node ptrs need to be refreshed now!
        node1 = tree.findObject(*obj1);
        node2 = tree.findObject(*obj2);
        node3_stair_1 = tree.findObject(*obj3_stair_1);
        node3_stair_2 = tree.findObject(*obj3_stair_2);
        node3_stair_3 = tree.findObject(*obj3_stair_3);
        node3_stair_4 = tree.findObject(*obj3_stair_4);
        node4 = tree.findObject(*obj4);
        if (!assertNotNull(obj1, "node1 find") ||
            !assertNotNull(obj2, "node2 find") ||
            !assertNotNull(obj3_stair_1, "node3_stair_1 find") ||
            !assertNotNull(obj3_stair_2, "node3_stair_2 find") ||
            !assertNotNull(obj3_stair_3, "node3_stair_3 find") ||
            !assertNotNull(obj3_stair_4, "node3_stair_4 find") ||
            !assertNotNull(obj4, "node4 find"))
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
            assertEquals(tree.getPos(), tree.getAABB().getPosVec(), "aabb pos") &
            assertEquals(PureVector(tree.getSize(), tree.getSize(), tree.getSize()), tree.getAABB().getSizeVec(), "aabb size")) != 0;
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

    bool testSetPosVecAffectsAabb()
    {
        const PureVector treeOrigin(100.f, 200.f, 300.f);
        const float treeSize = 1000.f;

        PureBoundingVolumeHierarchy treeShortCtor(3, 0);
        treeShortCtor.setSize(treeSize);

        bool b = true;
        b &= assertTrue(treeShortCtor.setPos(treeOrigin), "setpos shortctor 1");
        b &= assertEquals(treeOrigin, treeShortCtor.getAABB().getPosVec(), "aabb shortctor 1");

        const PureVector treeOrigin2(1.f, 2.f, 3.f);
        PureBoundingVolumeHierarchy treeLongCtor(treeOrigin2, treeSize, 3, 0);
        // these should also pass since tree is still empty
        b &= assertTrue(treeLongCtor.setPos(treeOrigin), "setpos treeLongCtor 1");
        b &= assertEquals(treeOrigin, treeLongCtor.getAABB().getPosVec(), "aabb treeLongCtor 1");

        PureObject3D* const obj1 = om->createBox(2.0f, 2.0f, 2.0f);
        if (!obj1)
        {
            return assertFalse(true, "obj1 null");
        }

        b &= assertNotNull(treeShortCtor.insertObject(*obj1), "insert short");
        b &= assertNotNull(treeLongCtor.insertObject(*obj1), "insert long");
        if (!b)
        {
            return false;
        }

        // now none of the trees is empty so operation shall fail
        b &= assertFalse(treeShortCtor.setPos(treeOrigin2), "setpos shortctor 2");
        b &= assertEquals(treeOrigin, treeShortCtor.getAABB().getPosVec(), "aabb shortctor 2");

        b &= assertFalse(treeLongCtor.setPos(treeOrigin2), "setpos treeLongCtor 2");
        b &= assertEquals(treeOrigin, treeLongCtor.getAABB().getPosVec(), "aabb treeLongCtor 2");

        return b;
    }

    bool testSetSizeAffectsAabb()
    {
        const PureVector treeOrigin(100.f, 200.f, 300.f);
        const float treeSize = 1000.f;

        PureBoundingVolumeHierarchy treeShortCtor(3, 0);
        treeShortCtor.setPos(treeOrigin);

        bool b = true;
        b &= assertTrue(treeShortCtor.setSize(treeSize), "setsize shortctor 1");
        b &= assertEquals(PureVector(treeSize, treeSize, treeSize), treeShortCtor.getAABB().getSizeVec(), "aabb shortctor 1");

        const float treeSize2 = 10.f;
        PureBoundingVolumeHierarchy treeLongCtor(treeOrigin, treeSize2, 3, 0);
        // these should also pass since tree is still empty
        b &= assertTrue(treeLongCtor.setSize(treeSize), "setsize treeLongCtor 1");
        b &= assertEquals(PureVector(treeSize, treeSize, treeSize), treeLongCtor.getAABB().getSizeVec(), "aabb treeLongCtor 1");

        PureObject3D* const obj1 = om->createBox(2.0f, 2.0f, 2.0f);
        if (!obj1)
        {
            return assertFalse(true, "obj1 null");
        }

        b &= assertNotNull(treeShortCtor.insertObject(*obj1), "insert short");
        b &= assertNotNull(treeLongCtor.insertObject(*obj1), "insert long");
        if (!b)
        {
            return false;
        }

        // now none of the trees is empty so operation shall fail
        b &= assertFalse(treeShortCtor.setSize(treeSize2), "setsize shortctor 2");
        b &= assertEquals(PureVector(treeSize, treeSize, treeSize), treeShortCtor.getAABB().getSizeVec(), "aabb shortctor 2");

        b &= assertFalse(treeShortCtor.setSize(treeSize2), "setsize treeLongCtor 2");
        b &= assertEquals(PureVector(treeSize, treeSize, treeSize), treeLongCtor.getAABB().getSizeVec(), "aabb treeLongCtor 2");

        return b;
    }

    bool testInsertObject_main(bool initTreeDimensionsInCtor)
    {
        // basically this is the same test as found in PureOctreeTest.h, but here we are interested in the
        // BVH-related stuff only

        // used to test if the given world-space position other than (0,0,0) is really taken into calculations;
        // if there is any issue, change this to (0,0,0) to find out probable reason of tree not properly taking origin pos into account!
        const PureVector treeOrigin(100.f, 200.f, 300.f);
        const float treeSize = 1000.f;

        PureObject3D* obj1 = nullptr;
        PureObject3D* obj2 = nullptr;
        PureObject3D* obj3 = nullptr;
        PureObject3D* obj4 = nullptr;
        PureObject3D* obj5 = nullptr;
        PureBoundingVolumeHierarchy* node1 = nullptr;
        PureBoundingVolumeHierarchy* node2 = nullptr;
        PureBoundingVolumeHierarchy* node3 = nullptr;
        PureBoundingVolumeHierarchy* node4 = nullptr;
        
        PureBoundingVolumeHierarchy tree_dimensionsInCtor(treeOrigin, treeSize, 2 /* maxDepth */, 0 /* currentDepth */);
        PureBoundingVolumeHierarchy tree_dimensionsAfterCtor(2 /* maxDepth */, 0 /* currentDepth */);
        PureBoundingVolumeHierarchy& tree = initTreeDimensionsInCtor ? tree_dimensionsInCtor : tree_dimensionsAfterCtor;
        if (!initTreeDimensionsInCtor)
        {
            bool bInit = true;
            bInit &= assertTrue(tree.setPos(treeOrigin), "tree setpos after ctor");
            bInit &= assertTrue(tree.setSize(treeSize), "tree setsize after ctor");
            if (!bInit)
            {
                return false;
            }
        }
        
        if (!assertTrue(buildBVH_1(tree, treeOrigin, obj1, obj2, obj3, obj4, obj5, node1, node2, node3, node4), "buildBVH_1"))
        {
            return false;
        }

        //const PureBoundingVolumeHierarchy* tree_TLF = static_cast<const PureBoundingVolumeHierarchy*>(tree.getChildren()[PureOctree::TOP    | PureOctree::LEFT  | PureOctree::FRONT]);
        //const PureBoundingVolumeHierarchy* tree_BLF = static_cast<const PureBoundingVolumeHierarchy*>(tree.getChildren()[PureOctree::BOTTOM | PureOctree::LEFT  | PureOctree::FRONT]);
        //const PureBoundingVolumeHierarchy* tree_TRF = static_cast<const PureBoundingVolumeHierarchy*>(tree.getChildren()[PureOctree::TOP    | PureOctree::RIGHT | PureOctree::FRONT]);
        const PureBoundingVolumeHierarchy* tree_BRF = static_cast<const PureBoundingVolumeHierarchy*>(tree.getChildren()[PureOctree::BOTTOM | PureOctree::RIGHT | PureOctree::FRONT]);
        const PureBoundingVolumeHierarchy* tree_TLB = static_cast<const PureBoundingVolumeHierarchy*>(tree.getChildren()[PureOctree::TOP    | PureOctree::LEFT  | PureOctree::BACK ]);
        //const PureBoundingVolumeHierarchy* tree_BLB = static_cast<const PureBoundingVolumeHierarchy*>(tree.getChildren()[PureOctree::BOTTOM | PureOctree::LEFT  | PureOctree::BACK ]);
        const PureBoundingVolumeHierarchy* tree_TRB = static_cast<const PureBoundingVolumeHierarchy*>(tree.getChildren()[PureOctree::TOP    | PureOctree::RIGHT | PureOctree::BACK ]);
        //const PureBoundingVolumeHierarchy* tree_BRB = static_cast<const PureBoundingVolumeHierarchy*>(tree.getChildren()[PureOctree::BOTTOM | PureOctree::RIGHT | PureOctree::BACK ]);

        if ( !assertEquals(PureOctree::NodeType::Parent, tree_BRF->getNodeType(), "tree_BRF nodeType") )
        {
            return false;
        }

        //const PureBoundingVolumeHierarchy* tree_BRF_TLF = static_cast<const PureBoundingVolumeHierarchy*>(tree_BRF->getChildren()[PureOctree::TOP    | PureOctree::LEFT  | PureOctree::FRONT]);
        //const PureBoundingVolumeHierarchy* tree_BRF_BLF = static_cast<const PureBoundingVolumeHierarchy*>(tree_BRF->getChildren()[PureOctree::BOTTOM | PureOctree::LEFT  | PureOctree::FRONT]);
        //const PureBoundingVolumeHierarchy* tree_BRF_TRF = static_cast<const PureBoundingVolumeHierarchy*>(tree_BRF->getChildren()[PureOctree::TOP    | PureOctree::RIGHT | PureOctree::FRONT]);
        //const PureBoundingVolumeHierarchy* tree_BRF_BRF = static_cast<const PureBoundingVolumeHierarchy*>(tree_BRF->getChildren()[PureOctree::BOTTOM | PureOctree::RIGHT | PureOctree::FRONT]);
        const PureBoundingVolumeHierarchy* tree_BRF_TLB = static_cast<const PureBoundingVolumeHierarchy*>(tree_BRF->getChildren()[PureOctree::TOP    | PureOctree::LEFT  | PureOctree::BACK ]);
        //const PureBoundingVolumeHierarchy* tree_BRF_BLB = static_cast<const PureBoundingVolumeHierarchy*>(tree_BRF->getChildren()[PureOctree::BOTTOM | PureOctree::LEFT  | PureOctree::BACK ]);
        //const PureBoundingVolumeHierarchy* tree_BRF_TRB = static_cast<const PureBoundingVolumeHierarchy*>(tree_BRF->getChildren()[PureOctree::TOP    | PureOctree::RIGHT | PureOctree::BACK ]);
        //const PureBoundingVolumeHierarchy* tree_BRF_BRB = static_cast<const PureBoundingVolumeHierarchy*>(tree_BRF->getChildren()[PureOctree::BOTTOM | PureOctree::RIGHT | PureOctree::BACK ]);

        // checking bounding boxes bottom-up

        // level-2 bounds
        const PureAxisAlignedBoundingBox aabb_obj2(obj2->getPosVec(), obj2->getScaledSizeVec());
        const PureAxisAlignedBoundingBox aabb_obj3(obj3->getPosVec(), obj3->getScaledSizeVec());
        PureAxisAlignedBoundingBox aabb_BRF_TLB_expected(
            tree_BRF_TLB->getPos(),
            PureVector(tree_BRF_TLB->getSize(), tree_BRF_TLB->getSize(), tree_BRF_TLB->getSize()));
        aabb_BRF_TLB_expected.ExtendBy(aabb_obj2);
        aabb_BRF_TLB_expected.ExtendBy(aabb_obj3);

        bool b = true;
        b &= assertEquals(aabb_BRF_TLB_expected.getPosVec(), tree_BRF_TLB->getAABB().getPosVec(), "tree_BRF_TLB AABB pos") &
            assertEquals(aabb_BRF_TLB_expected.getSizeVec(), tree_BRF_TLB->getAABB().getSizeVec(), "tree_BRF_TLB AABB size");

        b &= assertEquals(8u, tree.getChildren().size(), "tree children size");
        if (!b)
        {
            return false;
        }

        for (size_t j = 0; j < tree.getChildren().size(); j++)
        {
            const PureBoundingVolumeHierarchy* const bvhChildNode = static_cast<PureBoundingVolumeHierarchy*>(tree.getChildren()[j]);
            for (size_t i = 0; i < bvhChildNode->getChildren().size(); i++)
            {
                const PureBoundingVolumeHierarchy* const bvhChildChildNode = static_cast<PureBoundingVolumeHierarchy*>(bvhChildNode->getChildren()[i]);
                if (bvhChildChildNode != tree_BRF_TLB)
                {
                    b &= assertEquals(bvhChildChildNode->getPos(), bvhChildChildNode->getAABB().getPosVec(), "level 2 child AABB pos") &
                        assertEquals(
                            PureVector(bvhChildChildNode->getSize(), bvhChildChildNode->getSize(), bvhChildChildNode->getSize()),
                            bvhChildChildNode->getAABB().getSizeVec(),
                            "level 2 child AABB size");
                }
            }
        }

        // level-1 bounds
        const PureAxisAlignedBoundingBox aabb_obj1(obj1->getPosVec(), obj1->getScaledSizeVec());
        PureAxisAlignedBoundingBox aabb_TLB_expected(
            tree_TLB->getPos(),
            PureVector(tree_TLB->getSize(), tree_TLB->getSize(), tree_TLB->getSize()));
        aabb_TLB_expected.ExtendBy(aabb_obj1);

        b &= assertEquals(aabb_TLB_expected.getPosVec(), tree_TLB->getAABB().getPosVec(), "tree_TLB AABB pos") &
            assertEquals(aabb_TLB_expected.getSizeVec(), tree_TLB->getAABB().getSizeVec(), "tree_TLB AABB size");

        const PureAxisAlignedBoundingBox aabb_obj4(obj4->getPosVec(), obj4->getScaledSizeVec());
        PureAxisAlignedBoundingBox aabb_TRB_expected(
            tree_TRB->getPos(),
            PureVector(tree_TRB->getSize(), tree_TRB->getSize(), tree_TRB->getSize()));
        aabb_TRB_expected.ExtendBy(aabb_obj4);

        b &= assertEquals(aabb_TRB_expected.getPosVec(), tree_TRB->getAABB().getPosVec(),  "tree_TRB AABB pos") &
            assertEquals(aabb_TRB_expected.getSizeVec(), tree_TRB->getAABB().getSizeVec(), "tree_TRB AABB size");

        PureAxisAlignedBoundingBox aabb_BRF_expected(
            tree_BRF->getPos(),
            PureVector(tree_BRF->getSize(), tree_BRF->getSize(), tree_BRF->getSize()));
        aabb_BRF_expected.ExtendBy(aabb_obj2);
        aabb_BRF_expected.ExtendBy(aabb_obj3);

        b &= assertEquals(aabb_BRF_expected.getPosVec(), tree_BRF->getAABB().getPosVec(),  "tree_BRF AABB pos") &
            assertEquals(aabb_BRF_expected.getSizeVec(), tree_BRF->getAABB().getSizeVec(), "tree_BRF AABB size");

        for (size_t i = 0; i < tree.getChildren().size(); i++)
        {
            const PureBoundingVolumeHierarchy* const bvhNode = static_cast<PureBoundingVolumeHierarchy*>(tree.getChildren()[i]);
            if ((bvhNode != tree_BRF) && (bvhNode != tree_TLB) && (bvhNode != tree_TRB))
            {
                b &= assertEquals(bvhNode->getPos(), bvhNode->getAABB().getPosVec(), "level 1 child AABB pos") &
                    assertEquals(
                        PureVector(bvhNode->getSize(), bvhNode->getSize(), bvhNode->getSize()),
                        bvhNode->getAABB().getSizeVec(),
                        "level 1 child AABB size");
            }
        }

        // level-0 bounds
        PureAxisAlignedBoundingBox aabb_root_expected(
            tree.getPos(),
            PureVector(tree.getSize(), tree.getSize(), tree.getSize()));
        aabb_root_expected.ExtendBy(aabb_TLB_expected);
        aabb_root_expected.ExtendBy(aabb_TRB_expected);
        aabb_root_expected.ExtendBy(aabb_BRF_expected);

        b &= assertEquals(aabb_root_expected.getPosVec(), tree.getAABB().getPosVec(),  "root AABB pos") &
            assertEquals(aabb_root_expected.getSizeVec(), tree.getAABB().getSizeVec(), "root AABB size");

        // reset(): compared to the PureOctree test, we only test if AABB stays intact (see in assertTreeIsReset())!
        b &= assertTrue(tree.reset(), "reset level 0");
        b &= assertTrue(assertTreeIsReset(tree), "tree is reset 1");

        return b;
    }

    bool testInsertObject()
    {
        bool b = true;

        bool bInitTreeDimensionsInCtor = true;
        for (int i = 0; i < 2; i++)
        {
            b &= assertTrue(
                testInsertObject_main(bInitTreeDimensionsInCtor),
                (std::string(__func__) + ": bInitTreeDimensionsInCtor = " + std::to_string(bInitTreeDimensionsInCtor)).c_str());
            bInitTreeDimensionsInCtor = !bInitTreeDimensionsInCtor;
        }

        return b;
    }

    bool testFindOneLowestLevelFittingNode_DownFromRootNode_main(bool initTreeDimensionsInCtor)
    {
        // used to test if the given world-space position other than (0,0,0) is really taken into calculations;
        // if there is any issue, change this to (0,0,0) to find out probable reason of tree not properly taking origin pos into account!
        const PureVector treeOrigin(100.f, 200.f, 300.f);
        const float treeSize = 1000.f;

        PureObject3D* obj1 = nullptr;
        PureObject3D* obj2 = nullptr;
        PureObject3D* obj3 = nullptr;
        PureObject3D* obj4 = nullptr;
        PureObject3D* obj5 = nullptr;
        PureBoundingVolumeHierarchy* node1 = nullptr;
        PureBoundingVolumeHierarchy* node2 = nullptr;
        PureBoundingVolumeHierarchy* node3 = nullptr;
        PureBoundingVolumeHierarchy* node4 = nullptr;

        PureBoundingVolumeHierarchy tree_dimensionsInCtor(treeOrigin, treeSize, 2 /* maxDepth */, 0 /* currentDepth */);
        PureBoundingVolumeHierarchy tree_dimensionsAfterCtor(2 /* maxDepth */, 0 /* currentDepth */);
        PureBoundingVolumeHierarchy& tree = initTreeDimensionsInCtor ? tree_dimensionsInCtor : tree_dimensionsAfterCtor;
        if (!initTreeDimensionsInCtor)
        {
            bool bInit = true;
            bInit &= assertTrue(tree.setPos(treeOrigin), "tree setpos after ctor");
            bInit &= assertTrue(tree.setSize(treeSize), "tree setsize after ctor");
            if (!bInit)
            {
                return false;
            }
        }

        if (!assertTrue(buildBVH_1(tree, treeOrigin, obj1, obj2, obj3, obj4, obj5, node1, node2, node3, node4), "buildBVH_1"))
        {
            return false;
        }

        bool b = assertTrue(node1 == tree.findOneLowestLevelFittingNode(*obj1, BvhSearchDirection::DownFromRootNode), "find 1 from root 1");
        b &= assertTrue(node2 == tree.findOneLowestLevelFittingNode(*obj2, BvhSearchDirection::DownFromRootNode), "find 2 from root 2");
        b &= assertTrue(node3 == tree.findOneLowestLevelFittingNode(*obj3, BvhSearchDirection::DownFromRootNode), "find 3 from root 3");
        b &= assertTrue(node4 == tree.findOneLowestLevelFittingNode(*obj4, BvhSearchDirection::DownFromRootNode), "find 4 from root 4");
        b &= assertTrue(nullptr == tree.findOneLowestLevelFittingNode(*obj5, BvhSearchDirection::DownFromRootNode), "find 5 from root 5");

        b &= assertTrue(node1 == node1->findOneLowestLevelFittingNode(*obj1, BvhSearchDirection::DownFromRootNode), "find 1 from node 1");
        b &= assertTrue(node2 == node2->findOneLowestLevelFittingNode(*obj2, BvhSearchDirection::DownFromRootNode), "find 2 from node 2");
        b &= assertTrue(node3 == node3->findOneLowestLevelFittingNode(*obj3, BvhSearchDirection::DownFromRootNode), "find 3 from node 3");
        b &= assertTrue(node4 == node4->findOneLowestLevelFittingNode(*obj4, BvhSearchDirection::DownFromRootNode), "find 4 from node 4");


        return b;
    }

    bool testFindOneLowestLevelFittingNode_DownFromRootNode()
    {
        bool b = true;

        bool bInitTreeDimensionsInCtor = true;
        for (int i = 0; i < 2; i++)
        {
            b &= assertTrue(
                testFindOneLowestLevelFittingNode_DownFromRootNode_main(bInitTreeDimensionsInCtor),
                (std::string(__func__) + ": bInitTreeDimensionsInCtor = " + std::to_string(bInitTreeDimensionsInCtor)).c_str());
            bInitTreeDimensionsInCtor = !bInitTreeDimensionsInCtor;
        }

        return b;
    }

    bool testFindOneLowestLevelFittingNode_UpFromLeafNode_main(bool initTreeDimensionsInCtor)
    {
        // used to test if the given world-space position other than (0,0,0) is really taken into calculations;
        // if there is any issue, change this to (0,0,0) to find out probable reason of tree not properly taking origin pos into account!
        const PureVector treeOrigin(100.f, 200.f, 300.f);
        const float treeSize = 1000.f;

        PureObject3D* obj1 = nullptr;
        PureObject3D* obj2 = nullptr;
        PureObject3D* obj3 = nullptr;
        PureObject3D* obj4 = nullptr;
        PureObject3D* obj5 = nullptr;
        PureBoundingVolumeHierarchy* node1 = nullptr;
        PureBoundingVolumeHierarchy* node2 = nullptr;
        PureBoundingVolumeHierarchy* node3 = nullptr;
        PureBoundingVolumeHierarchy* node4 = nullptr;

        PureBoundingVolumeHierarchy tree_dimensionsInCtor(treeOrigin, treeSize, 2 /* maxDepth */, 0 /* currentDepth */);
        PureBoundingVolumeHierarchy tree_dimensionsAfterCtor(2 /* maxDepth */, 0 /* currentDepth */);
        PureBoundingVolumeHierarchy& tree = initTreeDimensionsInCtor ? tree_dimensionsInCtor : tree_dimensionsAfterCtor;
        if (!initTreeDimensionsInCtor)
        {
            bool bInit = true;
            bInit &= assertTrue(tree.setPos(treeOrigin), "tree setpos after ctor");
            bInit &= assertTrue(tree.setSize(treeSize), "tree setsize after ctor");
            if (!bInit)
            {
                return false;
            }
        }

        if (!assertTrue(buildBVH_1(tree, treeOrigin, obj1, obj2, obj3, obj4, obj5, node1, node2, node3, node4), "buildBVH_1"))
        {
            return false;
        }

        bool b = assertTrue(node1 == tree.findOneLowestLevelFittingNode(*obj1, BvhSearchDirection::UpFromLeafNode), "find 1 from root 1");
        b &= assertTrue(node2 == tree.findOneLowestLevelFittingNode(*obj2, BvhSearchDirection::UpFromLeafNode), "find 2 from root 2");
        b &= assertTrue(node3 == tree.findOneLowestLevelFittingNode(*obj3, BvhSearchDirection::UpFromLeafNode), "find 3 from root 3");
        b &= assertTrue(node4 == tree.findOneLowestLevelFittingNode(*obj4, BvhSearchDirection::UpFromLeafNode), "find 4 from root 4");
        b &= assertTrue(nullptr == tree.findOneLowestLevelFittingNode(*obj5, BvhSearchDirection::UpFromLeafNode), "find 5 from root 5");

        b &= assertTrue(node1 == node1->findOneLowestLevelFittingNode(*obj1, BvhSearchDirection::UpFromLeafNode), "find 1 from node 1");
        b &= assertTrue(node2 == node2->findOneLowestLevelFittingNode(*obj2, BvhSearchDirection::UpFromLeafNode), "find 2 from node 2");
        b &= assertTrue(node3 == node3->findOneLowestLevelFittingNode(*obj3, BvhSearchDirection::UpFromLeafNode), "find 3 from node 3");
        b &= assertTrue(node4 == node4->findOneLowestLevelFittingNode(*obj4, BvhSearchDirection::UpFromLeafNode), "find 4 from node 4");

        return b;
    }

    bool testFindOneLowestLevelFittingNode_UpFromLeafNode()
    {
        bool b = true;

        bool bInitTreeDimensionsInCtor = true;
        for (int i = 0; i < 2; i++)
        {
            b &= assertTrue(
                testFindOneLowestLevelFittingNode_UpFromLeafNode_main(bInitTreeDimensionsInCtor),
                (std::string(__func__) + ": bInitTreeDimensionsInCtor = " + std::to_string(bInitTreeDimensionsInCtor)).c_str());
            bInitTreeDimensionsInCtor = !bInitTreeDimensionsInCtor;
        }

        return b;
    }

    bool testFindOneColliderObject_1_startFromLowestLevelFittingNode_main(bool initTreeDimensionsInCtor, const BvhSearchDirection& searchDir)
    {
        const PureVector treeOrigin(100.f, 200.f, 300.f);
        const float treeSize = 1000.f;

        PureObject3D* obj1 = nullptr;
        PureObject3D* obj2 = nullptr;
        PureObject3D* obj3 = nullptr;
        PureObject3D* obj4 = nullptr;
        PureObject3D* obj5 = nullptr;
        PureBoundingVolumeHierarchy* node1 = nullptr;
        PureBoundingVolumeHierarchy* node2 = nullptr;
        PureBoundingVolumeHierarchy* node3 = nullptr;
        PureBoundingVolumeHierarchy* node4 = nullptr;

        PureBoundingVolumeHierarchy tree_dimensionsInCtor(treeOrigin, treeSize, 2 /* maxDepth */, 0 /* currentDepth */);
        PureBoundingVolumeHierarchy tree_dimensionsAfterCtor(2 /* maxDepth */, 0 /* currentDepth */);
        PureBoundingVolumeHierarchy& tree = initTreeDimensionsInCtor ? tree_dimensionsInCtor : tree_dimensionsAfterCtor;
        if (!initTreeDimensionsInCtor)
        {
            bool bInit = true;
            bInit &= assertTrue(tree.setPos(treeOrigin), "tree setpos after ctor");
            bInit &= assertTrue(tree.setSize(treeSize), "tree setsize after ctor");
            if (!bInit)
            {
                return false;
            }
        }

        if (!assertTrue(buildBVH_1(tree, treeOrigin, obj1, obj2, obj3, obj4, obj5, node1, node2, node3, node4), "buildBVH_1"))
        {
            return false;
        }

        bool b = true;

        // obj5 is outside of tree bounds, hence it cannot collide with anything within the tree
        b &= assertNull(tree.findOneColliderObject_startFromLowestLevelFittingNode(*obj5, searchDir), "obj5 collider, object version");
        
        const PureAxisAlignedBoundingBox obj5Aabb(obj5->getPosVec(), obj5->getScaledSizeVec());
        b &= assertNull(tree.findOneColliderObject_startFromLowestLevelFittingNode(obj5Aabb, nullptr, searchDir), "obj5 collider, aabb version");

        // obj1, being in the tree, is expected to be colliding with itself
        const PureObject3D* const pObj1CollidingWith = tree.findOneColliderObject_startFromLowestLevelFittingNode(*obj1, searchDir);
        b &= assertEquals(obj1, pObj1CollidingWith, "obj1 collider, object version");
        
        const PureAxisAlignedBoundingBox obj1Aabb(obj1->getPosVec(), obj1->getScaledSizeVec());
        b &= assertEquals(pObj1CollidingWith, tree.findOneColliderObject_startFromLowestLevelFittingNode(obj1Aabb, nullptr, searchDir), "obj1 collider, aabb version");

        // obj2, being in the tree, is expected to be colliding with itself or obj3 as they have same position and size!
        const PureObject3D* const pObj2CollidingWith = tree.findOneColliderObject_startFromLowestLevelFittingNode(*obj2, searchDir);
        b &= assertTrue((obj2 == pObj2CollidingWith) || (obj3 == pObj2CollidingWith), "obj2 collider, object version");
        
        const PureAxisAlignedBoundingBox obj2Aabb(obj2->getPosVec(), obj2->getScaledSizeVec());
        const PureObject3D* const pObj2CollidingWithAabb = tree.findOneColliderObject_startFromLowestLevelFittingNode(obj2Aabb, nullptr, searchDir);
        b &= assertTrue((obj2 == pObj2CollidingWithAabb) || (obj3 == pObj2CollidingWithAabb), "obj2 collider, aabb version");

        // obj3, same story
        const PureObject3D* const pObj3CollidingWith = tree.findOneColliderObject_startFromLowestLevelFittingNode(*obj3, searchDir);
        b &= assertTrue((obj2 == pObj3CollidingWith) || (obj3 == pObj3CollidingWith), "obj3 collider, object version");

        const PureAxisAlignedBoundingBox obj3Aabb(obj3->getPosVec(), obj3->getScaledSizeVec());
        const PureObject3D* const pObj3CollidingWithAabb = tree.findOneColliderObject_startFromLowestLevelFittingNode(obj3Aabb, nullptr, searchDir);
        b &= assertTrue((obj2 == pObj3CollidingWithAabb) || (obj3 == pObj3CollidingWithAabb), "obj3 collider, aabb version");

        // obj4, being in the tree, is expected to be colliding with itself
        const PureObject3D* const pObj4CollidingWith = tree.findOneColliderObject_startFromLowestLevelFittingNode(*obj4, searchDir);
        b &= assertEquals(obj4, pObj4CollidingWith, "obj4 collider, object version");

        const PureAxisAlignedBoundingBox obj4Aabb(obj4->getPosVec(), obj4->getScaledSizeVec());
        b &= assertEquals(pObj4CollidingWith, tree.findOneColliderObject_startFromLowestLevelFittingNode(obj4Aabb, nullptr, searchDir), "obj4 collider, aabb version");

        return b;
    }

    bool testFindOneColliderObject_1_startFromLowestLevelFittingNode()
    {
        bool b = true;

        for (int j = 0; j < 2; j++)
        {
            const BvhSearchDirection searchDir = ((j == 0) ? BvhSearchDirection::DownFromRootNode : BvhSearchDirection::UpFromLeafNode);
            bool bInitTreeDimensionsInCtor = true;
            for (int i = 0; i < 2; i++)
            {
                b &= assertTrue(
                    testFindOneColliderObject_1_startFromLowestLevelFittingNode_main(bInitTreeDimensionsInCtor, searchDir),
                    (std::string(__func__) +
                        ": bInitTreeDimensionsInCtor = " + std::to_string(bInitTreeDimensionsInCtor) +
                        ": searchDir = " + std::to_string(static_cast<int>(searchDir))
                    ).c_str());
                bInitTreeDimensionsInCtor = !bInitTreeDimensionsInCtor;
            }
        }

        return b;
    }

    bool testFindAllColliderObjects_1_startFromLowestLevelFittingNode_main(bool initTreeDimensionsInCtor, const BvhSearchDirection& searchDir)
    {
        const PureVector treeOrigin(100.f, 200.f, 300.f);
        const float treeSize = 1000.f;

        PureObject3D* obj1 = nullptr;
        PureObject3D* obj2 = nullptr;
        PureObject3D* obj3 = nullptr;
        PureObject3D* obj4 = nullptr;
        PureObject3D* obj5 = nullptr;
        PureBoundingVolumeHierarchy* node1 = nullptr;
        PureBoundingVolumeHierarchy* node2 = nullptr;
        PureBoundingVolumeHierarchy* node3 = nullptr;
        PureBoundingVolumeHierarchy* node4 = nullptr;

        PureBoundingVolumeHierarchy tree_dimensionsInCtor(treeOrigin, treeSize, 2 /* maxDepth */, 0 /* currentDepth */);
        PureBoundingVolumeHierarchy tree_dimensionsAfterCtor(2 /* maxDepth */, 0 /* currentDepth */);
        PureBoundingVolumeHierarchy& tree = initTreeDimensionsInCtor ? tree_dimensionsInCtor : tree_dimensionsAfterCtor;
        if (!initTreeDimensionsInCtor)
        {
            bool bInit = true;
            bInit &= assertTrue(tree.setPos(treeOrigin), "tree setpos after ctor");
            bInit &= assertTrue(tree.setSize(treeSize), "tree setsize after ctor");
            if (!bInit)
            {
                return false;
            }
        }

        if (!assertTrue(buildBVH_1(tree, treeOrigin, obj1, obj2, obj3, obj4, obj5, node1, node2, node3, node4), "buildBVH_1"))
        {
            return false;
        }

        bool b = true;

        std::vector<const PureObject3D*> colliders;

        // obj5 is outside of tree bounds, hence it cannot collide with anything within the tree
        b &= assertFalse(tree.findAllColliderObjects_startFromLowestLevelFittingNode(*obj5, colliders, searchDir), "obj5 colliders 0, object version");
        b &= assertTrue(colliders.empty(), "obj5 colliders empty, object version");

        const PureAxisAlignedBoundingBox obj5Aabb(obj5->getPosVec(), obj5->getScaledSizeVec());
        b &= assertFalse(tree.findAllColliderObjects_startFromLowestLevelFittingNode(obj5Aabb, nullptr, colliders, searchDir), "obj5 colliders 0, aabb version");
        b &= assertTrue(colliders.empty(), "obj5 colliders empty, aabb version");

        // obj1, being in the tree, is expected to be colliding with itself
        colliders.clear();
        b &= assertTrue(tree.findAllColliderObjects_startFromLowestLevelFittingNode(*obj1, colliders, searchDir), "obj1 colliders 1, object version");
        b &= assertFalse(colliders.empty(), "obj1 colliders empty, object version");
        if (b)
        {
            b &= assertEquals(obj1, *colliders.begin(), "obj1 colliders exact check, object version");
        }

        colliders.clear();
        const PureAxisAlignedBoundingBox obj1Aabb(obj1->getPosVec(), obj1->getScaledSizeVec());
        b &= assertTrue(tree.findAllColliderObjects_startFromLowestLevelFittingNode(obj1Aabb, nullptr, colliders, searchDir), "obj1 colliders 1, aabb version");
        b &= assertFalse(colliders.empty(), "obj1 colliders empty, aabb version");
        if (b)
        {
            b &= assertEquals(obj1, *colliders.begin(), "obj1 colliders exact check, aabb version");
        }

        // obj2, being in the tree, is expected to be colliding with itself and obj3 as they have same position and size!
        colliders.clear();
        b &= assertTrue(tree.findAllColliderObjects_startFromLowestLevelFittingNode(*obj2, colliders, searchDir), "obj2 colliders 1, object version");
        b &= assertEquals(2u, colliders.size(), "obj2 colliders size, object version");
        if (b)
        {
            b &= assertTrue(std::find(colliders.begin(), colliders.end(), obj2) != colliders.end(), "obj2 colliders exact check 1, object version");
            b &= assertTrue(std::find(colliders.begin(), colliders.end(), obj3) != colliders.end(), "obj2 colliders exact check 2, object version");
        }

        colliders.clear();
        const PureAxisAlignedBoundingBox obj2Aabb(obj2->getPosVec(), obj2->getScaledSizeVec());
        b &= assertTrue(tree.findAllColliderObjects_startFromLowestLevelFittingNode(obj2Aabb, nullptr, colliders, searchDir), "obj2 colliders 1, aabb version");
        b &= assertEquals(2u, colliders.size(), "obj2 colliders size, aabb version");
        if (b)
        {
            b &= assertTrue(std::find(colliders.begin(), colliders.end(), obj2) != colliders.end(), "obj2 colliders exact check 1, aabb version");
            b &= assertTrue(std::find(colliders.begin(), colliders.end(), obj3) != colliders.end(), "obj2 colliders exact check 2, aabb version");
        }

        // same story for obj3
        colliders.clear();
        b &= assertTrue(tree.findAllColliderObjects_startFromLowestLevelFittingNode(*obj3, colliders, searchDir), "obj3 colliders 1, object version");
        b &= assertEquals(2u, colliders.size(), "obj3 colliders size, object version");
        if (b)
        {
            b &= assertTrue(std::find(colliders.begin(), colliders.end(), obj2) != colliders.end(), "obj3 colliders exact check 1, object version");
            b &= assertTrue(std::find(colliders.begin(), colliders.end(), obj3) != colliders.end(), "obj3 colliders exact check 2, object version");
        }

        colliders.clear();
        const PureAxisAlignedBoundingBox obj3Aabb(obj3->getPosVec(), obj3->getScaledSizeVec());
        b &= assertTrue(tree.findAllColliderObjects_startFromLowestLevelFittingNode(obj3Aabb, nullptr, colliders, searchDir), "obj3 colliders 1, aabb version");
        b &= assertEquals(2u, colliders.size(), "obj3 colliders size, aabb version");
        if (b)
        {
            b &= assertTrue(std::find(colliders.begin(), colliders.end(), obj2) != colliders.end(), "obj3 colliders exact check 1, aabb version");
            b &= assertTrue(std::find(colliders.begin(), colliders.end(), obj3) != colliders.end(), "obj3 colliders exact check 2, aabb version");
        }

        // obj4, being in the tree, is expected to be colliding with itself
        colliders.clear();
        b &= assertTrue(tree.findAllColliderObjects_startFromLowestLevelFittingNode(*obj4, colliders, searchDir), "obj4 colliders 1, object version");
        b &= assertFalse(colliders.empty(), "obj4 colliders empty, object version");
        if (b)
        {
            b &= assertEquals(obj4, *colliders.begin(), "obj4 colliders exact check, object version");
        }

        colliders.clear();
        const PureAxisAlignedBoundingBox obj4Aabb(obj4->getPosVec(), obj4->getScaledSizeVec());
        b &= assertTrue(tree.findAllColliderObjects_startFromLowestLevelFittingNode(obj4Aabb, nullptr, colliders, searchDir), "obj4 colliders 1, aabb version");
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
            b &= assertTrue(tree.findAllColliderObjects_startFromLowestLevelFittingNode(*objBig, colliders, searchDir), "objBig colliders 1, object version");
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
            b &= assertTrue(tree.findAllColliderObjects_startFromLowestLevelFittingNode(objBigAabb, nullptr, colliders, searchDir), "objBig colliders 1, aabb version");
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

    bool testFindAllColliderObjects_1_startFromLowestLevelFittingNode()
    {
        bool b = true;

        for (int j = 0; j < 2; j++)
        {
            const BvhSearchDirection searchDir = ((j == 0) ? BvhSearchDirection::DownFromRootNode : BvhSearchDirection::UpFromLeafNode);
            bool bInitTreeDimensionsInCtor = true;
            for (int i = 0; i < 2; i++)
            {
                b &= assertTrue(
                    testFindAllColliderObjects_1_startFromLowestLevelFittingNode_main(bInitTreeDimensionsInCtor, searchDir),
                    (std::string(__func__) +
                        ": bInitTreeDimensionsInCtor = " + std::to_string(bInitTreeDimensionsInCtor) +
                        ": searchDir = " + std::to_string(static_cast<int>(searchDir))
                    ).c_str());
                bInitTreeDimensionsInCtor = !bInitTreeDimensionsInCtor;
            }
        }

        return b;
    }

    bool testFindOneColliderObject_2_startFromLowestLevelFittingNode_main(bool initTreeDimensionsInCtor, const BvhSearchDirection& searchDir)
    {
        const PureVector treeOrigin(42.5f, -16.f, 0.f);
        const float treeSize = 85.0f;  /* size as calculated during loading map_test_good_2_collision.txt */

        PureBoundingVolumeHierarchy tree_dimensionsInCtor(treeOrigin, treeSize, 3 /* maxDepth */, 0 /* currentDepth */);
        PureBoundingVolumeHierarchy tree_dimensionsAfterCtor(3 /* maxDepth */, 0 /* currentDepth */);
        PureBoundingVolumeHierarchy& tree = initTreeDimensionsInCtor ? tree_dimensionsInCtor : tree_dimensionsAfterCtor;
        if (!initTreeDimensionsInCtor)
        {
            bool bInit = true;
            bInit &= assertTrue(tree.setPos(treeOrigin), "tree setpos after ctor");
            bInit &= assertTrue(tree.setSize(treeSize), "tree setsize after ctor");
            if (!bInit)
            {
                return false;
            }
        }

        PureObject3D* obj1 = nullptr;
        PureObject3D* obj2 = nullptr;
        PureObject3D* obj3_stair_1 = nullptr;
        PureObject3D* obj3_stair_2 = nullptr;
        PureObject3D* obj3_stair_3 = nullptr;
        PureObject3D* obj3_stair_4 = nullptr;
        PureObject3D* obj4 = nullptr;
        PureBoundingVolumeHierarchy* node1 = nullptr;
        PureBoundingVolumeHierarchy* node2 = nullptr;
        PureBoundingVolumeHierarchy* node3_stair_1 = nullptr;
        PureBoundingVolumeHierarchy* node3_stair_2 = nullptr;
        PureBoundingVolumeHierarchy* node3_stair_3 = nullptr;
        PureBoundingVolumeHierarchy* node3_stair_4 = nullptr;
        PureBoundingVolumeHierarchy* node4 = nullptr;
        
        if (!assertTrue(
            buildBVH_2(
                tree, treeOrigin,
                obj1, obj2, obj3_stair_1, obj3_stair_2, obj3_stair_3, obj3_stair_4, obj4,
                node1, node2, node3_stair_1, node3_stair_2, node3_stair_3, node3_stair_4, node4), "buildBVH_2"))
        {
            return false;
        }

        bool b = true;

        // obj1, being in the tree, is expected to be colliding with itself
        const PureObject3D* const pObj1CollidingWith = tree.findOneColliderObject_startFromLowestLevelFittingNode(*obj1, searchDir);
        b &= assertEquals(obj1, pObj1CollidingWith, "obj1 collider, object version");

        const PureAxisAlignedBoundingBox obj1Aabb(obj1->getPosVec(), obj1->getScaledSizeVec());
        b &= assertEquals(pObj1CollidingWith, tree.findOneColliderObject_startFromLowestLevelFittingNode(obj1Aabb, nullptr, searchDir), "obj1 collider, aabb version");

        // obj2, being in the tree, is expected to be colliding with itself
        const PureObject3D* const pObj2CollidingWith = tree.findOneColliderObject_startFromLowestLevelFittingNode(*obj2, searchDir);
        b &= assertEquals(obj2, pObj2CollidingWith, "obj2 collider, object version");

        const PureAxisAlignedBoundingBox obj2Aabb(obj2->getPosVec(), obj2->getScaledSizeVec());
        b &= assertEquals(pObj2CollidingWith, tree.findOneColliderObject_startFromLowestLevelFittingNode(obj2Aabb, nullptr, searchDir), "obj2 collider, aabb version");

        // obj3_stair_1, being in the tree, is expected to be colliding with either itself or another block right next to it
        const PureObject3D* const pObj3_stair_1CollidingWith = tree.findOneColliderObject_startFromLowestLevelFittingNode(*obj3_stair_1, searchDir);
        b &= assertNotNull(pObj3_stair_1CollidingWith, "obj3_stair_1 collider, object version");

        const PureAxisAlignedBoundingBox obj3_stair_1Aabb(obj3_stair_1->getPosVec(), obj3_stair_1->getScaledSizeVec());
        b &= assertNotNull( tree.findOneColliderObject_startFromLowestLevelFittingNode(obj3_stair_1Aabb, nullptr, searchDir), "obj3_stair_1 collider, aabb version");

        // obj3_stair_2, being in the tree, is expected to be colliding with either itself or another block right next to it
        const PureObject3D* const pObj3_stair_2CollidingWith = tree.findOneColliderObject_startFromLowestLevelFittingNode(*obj3_stair_2, searchDir);
        b &= assertNotNull(pObj3_stair_2CollidingWith, "obj3_stair_2 collider, object version");

        const PureAxisAlignedBoundingBox obj3_stair_2Aabb(obj3_stair_2->getPosVec(), obj3_stair_2->getScaledSizeVec());
        b &= assertNotNull( tree.findOneColliderObject_startFromLowestLevelFittingNode(obj3_stair_2Aabb, nullptr, searchDir), "obj3_stair_2 collider, aabb version");

        // obj3_stair_3, being in the tree, is expected to be colliding with either itself or another block right next to it
        const PureObject3D* const pObj3_stair_3CollidingWith = tree.findOneColliderObject_startFromLowestLevelFittingNode(*obj3_stair_3, searchDir);
        b &= assertNotNull(pObj3_stair_3CollidingWith, "obj3_stair_3 collider, object version");

        const PureAxisAlignedBoundingBox obj3_stair_3Aabb(obj3_stair_3->getPosVec(), obj3_stair_3->getScaledSizeVec());
        b &= assertNotNull( tree.findOneColliderObject_startFromLowestLevelFittingNode(obj3_stair_3Aabb, nullptr, searchDir), "obj3_stair_3 collider, aabb version");

        // obj3_stair_4, being in the tree, is expected to be colliding with either itself or another block right next to it
        const PureObject3D* const pObj3_stair_4CollidingWith = tree.findOneColliderObject_startFromLowestLevelFittingNode(*obj3_stair_4, searchDir);
        b &= assertNotNull(pObj3_stair_4CollidingWith, "obj3_stair_4 collider, object version");

        const PureAxisAlignedBoundingBox obj3_stair_4Aabb(obj3_stair_4->getPosVec(), obj3_stair_4->getScaledSizeVec());
        b &= assertNotNull( tree.findOneColliderObject_startFromLowestLevelFittingNode(obj3_stair_4Aabb, nullptr, searchDir), "obj3_stair_4 collider, aabb version");

        // obj4, being in the tree, is expected to be colliding with either itself or another block right next to it
        const PureObject3D* const pObj4CollidingWith = tree.findOneColliderObject_startFromLowestLevelFittingNode(*obj4, searchDir);
        b &= assertNotNull(pObj4CollidingWith, "obj4 collider, object version");

        const PureAxisAlignedBoundingBox obj4Aabb(obj4->getPosVec(), obj4->getScaledSizeVec());
        b &= assertNotNull( tree.findOneColliderObject_startFromLowestLevelFittingNode(obj4Aabb, nullptr, searchDir), "obj4 collider, aabb version");

        // create a player-sized object
        PureObject3D* objPlayer = om->createPlane(0.95f, 1.88f);

        // initially it should not touch any foreground blocks and obviously not colliding with self since player is not added to the tree
        objPlayer->getPosVec().Set(
            obj4->getPosVec().getX(),
            obj4->getPosVec().getY() + obj4->getSizeVec().getY() / 2.f + objPlayer->getScaledSizeVec().getY() / 2.f + 0.1f /* shall not touch obj4 FG block */,
            -1.2f /*GAME_PLAYERS_POS_Z*/);
        
        b &= assertNull(tree.findOneColliderObject_startFromLowestLevelFittingNode(*objPlayer, searchDir), "objPlayer collider 1, object version");

        const PureAxisAlignedBoundingBox objPlayerAabb(objPlayer->getPosVec(), objPlayer->getScaledSizeVec());
        b &= assertNull(tree.findOneColliderObject_startFromLowestLevelFittingNode(objPlayerAabb, nullptr, searchDir), "objPlayer collider 1, aabb version");

        // but then, as gravity pulls player down, it shall be colliding with a block object too
        objPlayer->getPosVec().SetY(objPlayer->getPosVec().getY() - 0.2f);

        b &= assertEquals(obj4, tree.findOneColliderObject_startFromLowestLevelFittingNode(*objPlayer, searchDir), "objPlayer collider 2, object version");

        const PureAxisAlignedBoundingBox objPlayerAabb_2(objPlayer->getPosVec(), objPlayer->getScaledSizeVec());
        b &= assertEquals(obj4, tree.findOneColliderObject_startFromLowestLevelFittingNode(objPlayerAabb_2, nullptr, searchDir), "objPlayer collider 2, aabb version");

        return b;
    }

    bool testFindOneColliderObject_2_startFromLowestLevelFittingNode()
    {
        bool b = true;

        for (int j = 0; j < 2; j++)
        {
            const BvhSearchDirection searchDir = ((j == 0) ? BvhSearchDirection::DownFromRootNode : BvhSearchDirection::UpFromLeafNode);
            bool bInitTreeDimensionsInCtor = true;
            for (int i = 0; i < 2; i++)
            {
                b &= assertTrue(
                    testFindOneColliderObject_2_startFromLowestLevelFittingNode_main(bInitTreeDimensionsInCtor, searchDir),
                    (std::string(__func__) +
                        ": bInitTreeDimensionsInCtor = " + std::to_string(bInitTreeDimensionsInCtor) +
                        ": searchDir = " + std::to_string(static_cast<int>(searchDir))
                    ).c_str());
                bInitTreeDimensionsInCtor = !bInitTreeDimensionsInCtor;
            }
        }

        return b;
    }

    bool testFindAllColliderObjects_2_startFromLowestLevelFittingNode_main(bool initTreeDimensionsInCtor, const BvhSearchDirection& searchDir)
    {
        const PureVector treeOrigin(42.5f, -16.f, 0.f);
        const float treeSize = 85.f;  /* size as calculated during loading map_test_good_2_collision.txt */

        PureBoundingVolumeHierarchy tree_dimensionsInCtor(treeOrigin, treeSize, 3 /* maxDepth */, 0 /* currentDepth */);
        PureBoundingVolumeHierarchy tree_dimensionsAfterCtor(3 /* maxDepth */, 0 /* currentDepth */);
        PureBoundingVolumeHierarchy& tree = initTreeDimensionsInCtor ? tree_dimensionsInCtor : tree_dimensionsAfterCtor;
        if (!initTreeDimensionsInCtor)
        {
            bool bInit = true;
            bInit &= assertTrue(tree.setPos(treeOrigin), "tree setpos after ctor");
            bInit &= assertTrue(tree.setSize(treeSize), "tree setsize after ctor");
            if (!bInit)
            {
                return false;
            }
        }

        PureObject3D* obj1 = nullptr;
        PureObject3D* obj2 = nullptr;
        PureObject3D* obj3_stair_1 = nullptr;
        PureObject3D* obj3_stair_2 = nullptr;
        PureObject3D* obj3_stair_3 = nullptr;
        PureObject3D* obj3_stair_4 = nullptr;
        PureObject3D* obj4 = nullptr;
        PureBoundingVolumeHierarchy* node1 = nullptr;
        PureBoundingVolumeHierarchy* node2 = nullptr;
        PureBoundingVolumeHierarchy* node3_stair_1 = nullptr;
        PureBoundingVolumeHierarchy* node3_stair_2 = nullptr;
        PureBoundingVolumeHierarchy* node3_stair_3 = nullptr;
        PureBoundingVolumeHierarchy* node3_stair_4 = nullptr;
        PureBoundingVolumeHierarchy* node4 = nullptr;

        if (!assertTrue(
            buildBVH_2(
                tree, treeOrigin,
                obj1, obj2, obj3_stair_1, obj3_stair_2, obj3_stair_3, obj3_stair_4, obj4,
                node1, node2, node3_stair_1, node3_stair_2, node3_stair_3, node3_stair_4, node4), "buildBVH_2"))
        {
            return false;
        }

        bool b = true;

        std::vector<const PureObject3D*> colliders;


        // obj1, being in the tree, is expected to be colliding with itself
        colliders.clear();
        b &= assertTrue(tree.findAllColliderObjects_startFromLowestLevelFittingNode(*obj1, colliders, searchDir), "obj1 colliders 1, object version");
        b &= assertFalse(colliders.empty(), "obj1 colliders empty, object version");
        if (b)
        {
            b &= assertEquals(obj1, *colliders.begin(), "obj1 colliders exact check, object version");
        }

        colliders.clear();
        const PureAxisAlignedBoundingBox obj1Aabb(obj1->getPosVec(), obj1->getScaledSizeVec());
        b &= assertTrue(tree.findAllColliderObjects_startFromLowestLevelFittingNode(obj1Aabb, nullptr, colliders, searchDir), "obj1 colliders 1, aabb version");
        b &= assertFalse(colliders.empty(), "obj1 colliders empty, aabb version");
        if (b)
        {
            b &= assertEquals(obj1, *colliders.begin(), "obj1 colliders exact check, aabb version");
        }

        // obj2, being in the tree, is expected to be colliding with itself
        colliders.clear();
        b &= assertTrue(tree.findAllColliderObjects_startFromLowestLevelFittingNode(*obj2, colliders, searchDir), "obj2 colliders 1, object version");
        b &= assertFalse(colliders.empty(), "obj2 colliders empty, object version");
        if (b)
        {
            b &= assertEquals(obj2, *colliders.begin(), "obj2 colliders exact check, object version");
        }

        colliders.clear();
        const PureAxisAlignedBoundingBox obj2Aabb(obj2->getPosVec(), obj2->getScaledSizeVec());
        b &= assertTrue(tree.findAllColliderObjects_startFromLowestLevelFittingNode(obj2Aabb, nullptr, colliders, searchDir), "obj2 colliders 1, aabb version");
        b &= assertFalse(colliders.empty(), "obj2 colliders empty, aabb version");
        if (b)
        {
            b &= assertEquals(obj2, *colliders.begin(), "obj2 colliders exact check, aabb version");
        }

        // obj3_stair_1, being in the tree, is expected to be colliding with either itself or another block right next to it
        colliders.clear();
        b &= assertTrue(tree.findAllColliderObjects_startFromLowestLevelFittingNode(*obj3_stair_1, colliders, searchDir), "obj3_stair_1 colliders 1, object version");
        b &= assertFalse(colliders.empty(), "obj3_stair_1 colliders empty, object version");
        if (b)
        {
            b &= assertTrue(std::find(colliders.begin(), colliders.end(), obj3_stair_1) != colliders.end(), "obj3_stair_1 colliders exact check 1, object version");
        }

        colliders.clear();
        const PureAxisAlignedBoundingBox obj3_stair_1Aabb(obj3_stair_1->getPosVec(), obj3_stair_1->getScaledSizeVec());
        b &= assertTrue(tree.findAllColliderObjects_startFromLowestLevelFittingNode(obj3_stair_1Aabb, nullptr, colliders, searchDir), "obj3_stair_1 colliders 1, aabb version");
        b &= assertFalse(colliders.empty(), "obj3_stair_1 colliders empty, aabb version");
        if (b)
        {
            b &= assertTrue(std::find(colliders.begin(), colliders.end(), obj3_stair_1) != colliders.end(), "obj3_stair_1 colliders exact check 1, aabb version");
        }

        // obj3_stair_2, being in the tree, is expected to be colliding with either itself or another block right next to it
        colliders.clear();
        b &= assertTrue(tree.findAllColliderObjects_startFromLowestLevelFittingNode(*obj3_stair_2, colliders, searchDir), "obj3_stair_2 colliders 1, object version");
        b &= assertFalse(colliders.empty(), "obj3_stair_2 colliders empty, object version");
        if (b)
        {
            b &= assertTrue(std::find(colliders.begin(), colliders.end(), obj3_stair_2) != colliders.end(), "obj3_stair_2 colliders exact check 1, object version");
        }

        colliders.clear();
        const PureAxisAlignedBoundingBox obj3_stair_2Aabb(obj3_stair_2->getPosVec(), obj3_stair_2->getScaledSizeVec());
        b &= assertTrue(tree.findAllColliderObjects_startFromLowestLevelFittingNode(obj3_stair_2Aabb, nullptr, colliders, searchDir), "obj3_stair_2 colliders 1, aabb version");
        b &= assertFalse(colliders.empty(), "obj3_stair_2 colliders empty, aabb version");
        if (b)
        {
            b &= assertTrue(std::find(colliders.begin(), colliders.end(), obj3_stair_2) != colliders.end(), "obj3_stair_2 colliders exact check 1, aabb version");
        }

        // obj3_stair_3, being in the tree, is expected to be colliding with either itself or another block right next to it
        colliders.clear();
        b &= assertTrue(tree.findAllColliderObjects_startFromLowestLevelFittingNode(*obj3_stair_3, colliders, searchDir), "obj3_stair_3 colliders 1, object version");
        b &= assertFalse(colliders.empty(), "obj3_stair_3 colliders empty, object version");
        if (b)
        {
            b &= assertTrue(std::find(colliders.begin(), colliders.end(), obj3_stair_3) != colliders.end(), "obj3_stair_3 colliders exact check 1, object version");
        }

        colliders.clear();
        const PureAxisAlignedBoundingBox obj3_stair_3Aabb(obj3_stair_3->getPosVec(), obj3_stair_3->getScaledSizeVec());
        b &= assertTrue(tree.findAllColliderObjects_startFromLowestLevelFittingNode(obj3_stair_3Aabb, nullptr, colliders, searchDir), "obj3_stair_3 colliders 1, aabb version");
        b &= assertFalse(colliders.empty(), "obj3_stair_3 colliders empty, aabb version");
        if (b)
        {
            b &= assertTrue(std::find(colliders.begin(), colliders.end(), obj3_stair_3) != colliders.end(), "obj3_stair_3 colliders exact check 1, aabb version");
        }

        // obj3_stair_4, being in the tree, is expected to be colliding with either itself or another block right next to it
        colliders.clear();
        b &= assertTrue(tree.findAllColliderObjects_startFromLowestLevelFittingNode(*obj3_stair_4, colliders, searchDir), "obj3_stair_4 colliders 1, object version");
        b &= assertFalse(colliders.empty(), "obj3_stair_4 colliders empty, object version");
        if (b)
        {
            b &= assertTrue(std::find(colliders.begin(), colliders.end(), obj3_stair_4) != colliders.end(), "obj3_stair_4 colliders exact check 1, object version");
        }

        colliders.clear();
        const PureAxisAlignedBoundingBox obj3_stair_4Aabb(obj3_stair_4->getPosVec(), obj3_stair_4->getScaledSizeVec());
        b &= assertTrue(tree.findAllColliderObjects_startFromLowestLevelFittingNode(obj3_stair_4Aabb, nullptr, colliders, searchDir), "obj3_stair_4 colliders 1, aabb version");
        b &= assertFalse(colliders.empty(), "obj3_stair_4 colliders empty, aabb version");
        if (b)
        {
            b &= assertTrue(std::find(colliders.begin(), colliders.end(), obj3_stair_4) != colliders.end(), "obj3_stair_4 colliders exact check 1, aabb version");
        }

        // obj4, being in the tree, is expected to be colliding with either itself or another block right next to it
        colliders.clear();
        b &= assertTrue(tree.findAllColliderObjects_startFromLowestLevelFittingNode(*obj4, colliders, searchDir), "obj4 colliders 1, object version");
        b &= assertFalse(colliders.empty(), "obj4 colliders empty, object version");
        if (b)
        {
            b &= assertTrue(std::find(colliders.begin(), colliders.end(), obj4) != colliders.end(), "obj4 colliders exact check 1, object version");
        }

        colliders.clear();
        const PureAxisAlignedBoundingBox obj4Aabb(obj4->getPosVec(), obj4->getScaledSizeVec());
        b &= assertTrue(tree.findAllColliderObjects_startFromLowestLevelFittingNode(obj4Aabb, nullptr, colliders, searchDir), "obj4 colliders 1, aabb version");
        b &= assertFalse(colliders.empty(), "obj4 colliders empty, aabb version");
        if (b)
        {
            b &= assertTrue(std::find(colliders.begin(), colliders.end(), obj4) != colliders.end(), "obj4 colliders exact check 1, aabb version");
        }

        // create a player-sized object
        PureObject3D* objPlayer = om->createPlane(0.95f, 1.88f);

        // initially it should not touch any foreground blocks and obviously not colliding with self since player is not added to the tree
        objPlayer->getPosVec().Set(
            obj4->getPosVec().getX(),
            obj4->getPosVec().getY() + obj4->getSizeVec().getY() / 2.f + objPlayer->getScaledSizeVec().getY() / 2.f + 0.1f /* shall not touch obj4 FG block */,
            -1.2f /*GAME_PLAYERS_POS_Z*/);

        colliders.clear();
        b &= assertFalse(tree.findAllColliderObjects_startFromLowestLevelFittingNode(*objPlayer, colliders, searchDir), "objPlayer colliders 1, object version");
        b &= assertTrue(colliders.empty(), "objPlayer colliders empty, object version 1");

        colliders.clear();
        const PureAxisAlignedBoundingBox objPlayerAabb(objPlayer->getPosVec(), objPlayer->getScaledSizeVec());
        b &= assertFalse(tree.findAllColliderObjects_startFromLowestLevelFittingNode(objPlayerAabb, nullptr, colliders, searchDir), "objPlayer colliders 1, aabb version");
        b &= assertTrue(colliders.empty(), "objPlayer colliders empty, aabb version 1");

        // but then, as gravity pulls player down, it shall be colliding with a block object too
        objPlayer->getPosVec().SetY(objPlayer->getPosVec().getY() - 0.2f);
        
        colliders.clear();
        b &= assertTrue(tree.findAllColliderObjects_startFromLowestLevelFittingNode(*objPlayer, colliders, searchDir), "objPlayer colliders 2, object version");
        b &= assertFalse(colliders.empty(), "objPlayer colliders empty, object version 2");
        if (b)
        {
            b &= assertEquals(obj4, *colliders.begin(), "objPlayer colliders exact check 2, object version");
        }

        colliders.clear();
        const PureAxisAlignedBoundingBox objPlayerAabb_2(objPlayer->getPosVec(), objPlayer->getScaledSizeVec());
        b &= assertTrue(tree.findAllColliderObjects_startFromLowestLevelFittingNode(objPlayerAabb_2, nullptr, colliders, searchDir), "objPlayer colliders 2, aabb version");
        b &= assertFalse(colliders.empty(), "objPlayer colliders empty, aabb version 2");
        if (b)
        {
            b &= assertEquals(obj4, *colliders.begin(), "objPlayer colliders exact check 2, aabb version");
        }

        return b;
    }

    bool testFindAllColliderObjects_2_startFromLowestLevelFittingNode()
    {
        bool b = true;

        for (int j = 0; j < 2; j++)
        {
            const BvhSearchDirection searchDir = ((j == 0) ? BvhSearchDirection::DownFromRootNode : BvhSearchDirection::UpFromLeafNode);
            bool bInitTreeDimensionsInCtor = true;
            for (int i = 0; i < 2; i++)
            {
                b &= assertTrue(
                    testFindAllColliderObjects_2_startFromLowestLevelFittingNode_main(bInitTreeDimensionsInCtor, searchDir),
                    (std::string(__func__) +
                        ": bInitTreeDimensionsInCtor = " + std::to_string(bInitTreeDimensionsInCtor) +
                        ": searchDir = " + std::to_string(static_cast<int>(searchDir))
                    ).c_str());
                bInitTreeDimensionsInCtor = !bInitTreeDimensionsInCtor;
            }
        }

        return b;
    }

    bool testFindOneColliderObject_1_startFromFirstNode_main(bool initTreeDimensionsInCtor)
    {
        const PureVector treeOrigin(100.f, 200.f, 300.f);
        const float treeSize = 1000.f;

        PureObject3D* obj1 = nullptr;
        PureObject3D* obj2 = nullptr;
        PureObject3D* obj3 = nullptr;
        PureObject3D* obj4 = nullptr;
        PureObject3D* obj5 = nullptr;
        PureBoundingVolumeHierarchy* node1 = nullptr;
        PureBoundingVolumeHierarchy* node2 = nullptr;
        PureBoundingVolumeHierarchy* node3 = nullptr;
        PureBoundingVolumeHierarchy* node4 = nullptr;

        PureBoundingVolumeHierarchy tree_dimensionsInCtor(treeOrigin, treeSize, 2 /* maxDepth */, 0 /* currentDepth */);
        PureBoundingVolumeHierarchy tree_dimensionsAfterCtor(2 /* maxDepth */, 0 /* currentDepth */);
        PureBoundingVolumeHierarchy& tree = initTreeDimensionsInCtor ? tree_dimensionsInCtor : tree_dimensionsAfterCtor;
        if (!initTreeDimensionsInCtor)
        {
            bool bInit = true;
            bInit &= assertTrue(tree.setPos(treeOrigin), "tree setpos after ctor");
            bInit &= assertTrue(tree.setSize(treeSize), "tree setsize after ctor");
            if (!bInit)
            {
                return false;
            }
        }

        if (!assertTrue(buildBVH_1(tree, treeOrigin, obj1, obj2, obj3, obj4, obj5, node1, node2, node3, node4), "buildBVH_1"))
        {
            return false;
        }

        bool b = true;

        // obj5 is outside of tree bounds, hence it cannot collide with anything within the tree
        b &= assertNull(tree.findOneColliderObject_startFromFirstNode(*obj5), "obj5 collider, object version");

        const PureAxisAlignedBoundingBox obj5Aabb(obj5->getPosVec(), obj5->getScaledSizeVec());
        b &= assertNull(tree.findOneColliderObject_startFromFirstNode(obj5Aabb, nullptr), "obj5 collider, aabb version");

        // obj1, being in the tree, is expected to be colliding with itself
        const PureObject3D* const pObj1CollidingWith = tree.findOneColliderObject_startFromFirstNode(*obj1);
        b &= assertEquals(obj1, pObj1CollidingWith, "obj1 collider, object version");

        const PureAxisAlignedBoundingBox obj1Aabb(obj1->getPosVec(), obj1->getScaledSizeVec());
        b &= assertEquals(pObj1CollidingWith, tree.findOneColliderObject_startFromFirstNode(obj1Aabb, nullptr), "obj1 collider, aabb version");

        // obj2, being in the tree, is expected to be colliding with itself or obj3 as they have same position and size!
        const PureObject3D* const pObj2CollidingWith = tree.findOneColliderObject_startFromFirstNode(*obj2);
        b &= assertTrue((obj2 == pObj2CollidingWith) || (obj3 == pObj2CollidingWith), "obj2 collider, object version");

        const PureAxisAlignedBoundingBox obj2Aabb(obj2->getPosVec(), obj2->getScaledSizeVec());
        const PureObject3D* const pObj2CollidingWithAabb = tree.findOneColliderObject_startFromFirstNode(obj2Aabb, nullptr);
        b &= assertTrue((obj2 == pObj2CollidingWithAabb) || (obj3 == pObj2CollidingWithAabb), "obj2 collider, aabb version");

        // obj3, same story
        const PureObject3D* const pObj3CollidingWith = tree.findOneColliderObject_startFromFirstNode(*obj3);
        b &= assertTrue((obj2 == pObj3CollidingWith) || (obj3 == pObj3CollidingWith), "obj3 collider, object version");

        const PureAxisAlignedBoundingBox obj3Aabb(obj3->getPosVec(), obj3->getScaledSizeVec());
        const PureObject3D* const pObj3CollidingWithAabb = tree.findOneColliderObject_startFromFirstNode(obj3Aabb, nullptr);
        b &= assertTrue((obj2 == pObj3CollidingWithAabb) || (obj3 == pObj3CollidingWithAabb), "obj3 collider, aabb version");

        // obj4, being in the tree, is expected to be colliding with itself
        const PureObject3D* const pObj4CollidingWith = tree.findOneColliderObject_startFromFirstNode(*obj4);
        b &= assertEquals(obj4, pObj4CollidingWith, "obj4 collider, object version");

        const PureAxisAlignedBoundingBox obj4Aabb(obj4->getPosVec(), obj4->getScaledSizeVec());
        b &= assertEquals(pObj4CollidingWith, tree.findOneColliderObject_startFromFirstNode(obj4Aabb, nullptr), "obj4 collider, aabb version");

        return b;
    }

    bool testFindOneColliderObject_1_startFromFirstNode()
    {
        bool b = true;

        bool bInitTreeDimensionsInCtor = true;
        for (int i = 0; i < 2; i++)
        {
            b &= assertTrue(
                testFindOneColliderObject_1_startFromFirstNode_main(bInitTreeDimensionsInCtor),
                (std::string(__func__) + ": bInitTreeDimensionsInCtor = " + std::to_string(bInitTreeDimensionsInCtor)).c_str());
            bInitTreeDimensionsInCtor = !bInitTreeDimensionsInCtor;
        }

        return b;
    }

    bool testFindAllColliderObjects_1_startFromFirstNode_main(bool initTreeDimensionsInCtor)
    {
        const PureVector treeOrigin(100.f, 200.f, 300.f);
        const float treeSize = 1000.f;

        PureObject3D* obj1 = nullptr;
        PureObject3D* obj2 = nullptr;
        PureObject3D* obj3 = nullptr;
        PureObject3D* obj4 = nullptr;
        PureObject3D* obj5 = nullptr;
        PureBoundingVolumeHierarchy* node1 = nullptr;
        PureBoundingVolumeHierarchy* node2 = nullptr;
        PureBoundingVolumeHierarchy* node3 = nullptr;
        PureBoundingVolumeHierarchy* node4 = nullptr;

        PureBoundingVolumeHierarchy tree_dimensionsInCtor(treeOrigin, treeSize, 2 /* maxDepth */, 0 /* currentDepth */);
        PureBoundingVolumeHierarchy tree_dimensionsAfterCtor(2 /* maxDepth */, 0 /* currentDepth */);
        PureBoundingVolumeHierarchy& tree = initTreeDimensionsInCtor ? tree_dimensionsInCtor : tree_dimensionsAfterCtor;
        if (!initTreeDimensionsInCtor)
        {
            bool bInit = true;
            bInit &= assertTrue(tree.setPos(treeOrigin), "tree setpos after ctor");
            bInit &= assertTrue(tree.setSize(treeSize), "tree setsize after ctor");
            if (!bInit)
            {
                return false;
            }
        }

        if (!assertTrue(buildBVH_1(tree, treeOrigin, obj1, obj2, obj3, obj4, obj5, node1, node2, node3, node4), "buildBVH_1"))
        {
            return false;
        }

        bool b = true;

        std::vector<const PureObject3D*> colliders;

        // obj5 is outside of tree bounds, hence it cannot collide with anything within the tree
        b &= assertFalse(tree.findAllColliderObjects_startFromFirstNode(*obj5, colliders), "obj5 colliders 0, object version");
        b &= assertTrue(colliders.empty(), "obj5 colliders empty, object version");

        const PureAxisAlignedBoundingBox obj5Aabb(obj5->getPosVec(), obj5->getScaledSizeVec());
        b &= assertFalse(tree.findAllColliderObjects_startFromFirstNode(obj5Aabb, nullptr, colliders), "obj5 colliders 0, aabb version");
        b &= assertTrue(colliders.empty(), "obj5 colliders empty, aabb version");

        // obj1, being in the tree, is expected to be colliding with itself
        colliders.clear();
        b &= assertTrue(tree.findAllColliderObjects_startFromFirstNode(*obj1, colliders), "obj1 colliders 1, object version");
        b &= assertFalse(colliders.empty(), "obj1 colliders empty, object version");
        if (b)
        {
            b &= assertEquals(obj1, *colliders.begin(), "obj1 colliders exact check, object version");
        }

        colliders.clear();
        const PureAxisAlignedBoundingBox obj1Aabb(obj1->getPosVec(), obj1->getScaledSizeVec());
        b &= assertTrue(tree.findAllColliderObjects_startFromFirstNode(obj1Aabb, nullptr, colliders), "obj1 colliders 1, aabb version");
        b &= assertFalse(colliders.empty(), "obj1 colliders empty, aabb version");
        if (b)
        {
            b &= assertEquals(obj1, *colliders.begin(), "obj1 colliders exact check, aabb version");
        }

        // obj2, being in the tree, is expected to be colliding with itself and obj3 as they have same position and size!
        colliders.clear();
        b &= assertTrue(tree.findAllColliderObjects_startFromFirstNode(*obj2, colliders), "obj2 colliders 1, object version");
        b &= assertEquals(2u, colliders.size(), "obj2 colliders size, object version");
        if (b)
        {
            b &= assertTrue(std::find(colliders.begin(), colliders.end(), obj2) != colliders.end(), "obj2 colliders exact check 1, object version");
            b &= assertTrue(std::find(colliders.begin(), colliders.end(), obj3) != colliders.end(), "obj2 colliders exact check 2, object version");
        }

        colliders.clear();
        const PureAxisAlignedBoundingBox obj2Aabb(obj2->getPosVec(), obj2->getScaledSizeVec());
        b &= assertTrue(tree.findAllColliderObjects_startFromFirstNode(obj2Aabb, nullptr, colliders), "obj2 colliders 1, aabb version");
        b &= assertEquals(2u, colliders.size(), "obj2 colliders size, aabb version");
        if (b)
        {
            b &= assertTrue(std::find(colliders.begin(), colliders.end(), obj2) != colliders.end(), "obj2 colliders exact check 1, aabb version");
            b &= assertTrue(std::find(colliders.begin(), colliders.end(), obj3) != colliders.end(), "obj2 colliders exact check 2, aabb version");
        }

        // same story for obj3
        colliders.clear();
        b &= assertTrue(tree.findAllColliderObjects_startFromFirstNode(*obj3, colliders), "obj3 colliders 1, object version");
        b &= assertEquals(2u, colliders.size(), "obj3 colliders size, object version");
        if (b)
        {
            b &= assertTrue(std::find(colliders.begin(), colliders.end(), obj2) != colliders.end(), "obj3 colliders exact check 1, object version");
            b &= assertTrue(std::find(colliders.begin(), colliders.end(), obj3) != colliders.end(), "obj3 colliders exact check 2, object version");
        }

        colliders.clear();
        const PureAxisAlignedBoundingBox obj3Aabb(obj3->getPosVec(), obj3->getScaledSizeVec());
        b &= assertTrue(tree.findAllColliderObjects_startFromFirstNode(obj3Aabb, nullptr, colliders), "obj3 colliders 1, aabb version");
        b &= assertEquals(2u, colliders.size(), "obj3 colliders size, aabb version");
        if (b)
        {
            b &= assertTrue(std::find(colliders.begin(), colliders.end(), obj2) != colliders.end(), "obj3 colliders exact check 1, aabb version");
            b &= assertTrue(std::find(colliders.begin(), colliders.end(), obj3) != colliders.end(), "obj3 colliders exact check 2, aabb version");
        }

        // obj4, being in the tree, is expected to be colliding with itself
        colliders.clear();
        b &= assertTrue(tree.findAllColliderObjects_startFromFirstNode(*obj4, colliders), "obj4 colliders 1, object version");
        b &= assertFalse(colliders.empty(), "obj4 colliders empty, object version");
        if (b)
        {
            b &= assertEquals(obj4, *colliders.begin(), "obj4 colliders exact check, object version");
        }

        colliders.clear();
        const PureAxisAlignedBoundingBox obj4Aabb(obj4->getPosVec(), obj4->getScaledSizeVec());
        b &= assertTrue(tree.findAllColliderObjects_startFromFirstNode(obj4Aabb, nullptr, colliders), "obj4 colliders 1, aabb version");
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
            b &= assertTrue(tree.findAllColliderObjects_startFromFirstNode(*objBig, colliders), "objBig colliders 1, object version");
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
            b &= assertTrue(tree.findAllColliderObjects_startFromFirstNode(objBigAabb, nullptr, colliders), "objBig colliders 1, aabb version");
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

    bool testFindAllColliderObjects_1_startFromFirstNode()
    {
        bool b = true;

        bool bInitTreeDimensionsInCtor = true;
        for (int i = 0; i < 2; i++)
        {
            b &= assertTrue(
                testFindAllColliderObjects_1_startFromFirstNode_main(bInitTreeDimensionsInCtor),
                (std::string(__func__) + ": bInitTreeDimensionsInCtor = " + std::to_string(bInitTreeDimensionsInCtor)).c_str());
            bInitTreeDimensionsInCtor = !bInitTreeDimensionsInCtor;
        }

        return b;
    }

    bool testFindOneColliderObject_2_startFromFirstNode_main(bool initTreeDimensionsInCtor)
    {
        const PureVector treeOrigin(42.5f, -16.f, 0.f);
        const float treeSize = 85.f;  /* size as calculated during loading map_test_good_2_collision.txt */

        PureBoundingVolumeHierarchy tree_dimensionsInCtor(treeOrigin, treeSize, 3 /* maxDepth */, 0 /* currentDepth */);
        PureBoundingVolumeHierarchy tree_dimensionsAfterCtor(3 /* maxDepth */, 0 /* currentDepth */);
        PureBoundingVolumeHierarchy& tree = initTreeDimensionsInCtor ? tree_dimensionsInCtor : tree_dimensionsAfterCtor;
        if (!initTreeDimensionsInCtor)
        {
            bool bInit = true;
            bInit &= assertTrue(tree.setPos(treeOrigin), "tree setpos after ctor");
            bInit &= assertTrue(tree.setSize(treeSize), "tree setsize after ctor");
            if (!bInit)
            {
                return false;
            }
        }

        PureObject3D* obj1 = nullptr;
        PureObject3D* obj2 = nullptr;
        PureObject3D* obj3_stair_1 = nullptr;
        PureObject3D* obj3_stair_2 = nullptr;
        PureObject3D* obj3_stair_3 = nullptr;
        PureObject3D* obj3_stair_4 = nullptr;
        PureObject3D* obj4 = nullptr;
        PureBoundingVolumeHierarchy* node1 = nullptr;
        PureBoundingVolumeHierarchy* node2 = nullptr;
        PureBoundingVolumeHierarchy* node3_stair_1 = nullptr;
        PureBoundingVolumeHierarchy* node3_stair_2 = nullptr;
        PureBoundingVolumeHierarchy* node3_stair_3 = nullptr;
        PureBoundingVolumeHierarchy* node3_stair_4 = nullptr;
        PureBoundingVolumeHierarchy* node4 = nullptr;

        if (!assertTrue(
            buildBVH_2(
                tree, treeOrigin,
                obj1, obj2, obj3_stair_1, obj3_stair_2, obj3_stair_3, obj3_stair_4, obj4,
                node1, node2, node3_stair_1, node3_stair_2, node3_stair_3, node3_stair_4, node4), "buildBVH_2"))
        {
            return false;
        }

        bool b = true;

        // obj1, being in the tree, is expected to be colliding with itself
        const PureObject3D* const pObj1CollidingWith = tree.findOneColliderObject_startFromFirstNode(*obj1);
        b &= assertEquals(obj1, pObj1CollidingWith, "obj1 collider, object version");

        const PureAxisAlignedBoundingBox obj1Aabb(obj1->getPosVec(), obj1->getScaledSizeVec());
        b &= assertEquals(pObj1CollidingWith, tree.findOneColliderObject_startFromFirstNode(obj1Aabb, nullptr), "obj1 collider, aabb version");

        // obj2, being in the tree, is expected to be colliding with itself
        const PureObject3D* const pObj2CollidingWith = tree.findOneColliderObject_startFromFirstNode(*obj2);
        b &= assertEquals(obj2, pObj2CollidingWith, "obj2 collider, object version");

        const PureAxisAlignedBoundingBox obj2Aabb(obj2->getPosVec(), obj2->getScaledSizeVec());
        b &= assertEquals(pObj2CollidingWith, tree.findOneColliderObject_startFromFirstNode(obj2Aabb, nullptr), "obj2 collider, aabb version");

        // obj3_stair_1, being in the tree, is expected to be colliding with either itself or another block right next to it
        const PureObject3D* const pObj3_stair_1CollidingWith = tree.findOneColliderObject_startFromFirstNode(*obj3_stair_1);
        b &= assertNotNull(pObj3_stair_1CollidingWith, "obj3_stair_1 collider, object version");

        const PureAxisAlignedBoundingBox obj3_stair_1Aabb(obj3_stair_1->getPosVec(), obj3_stair_1->getScaledSizeVec());
        b &= assertNotNull(tree.findOneColliderObject_startFromFirstNode(obj3_stair_1Aabb, nullptr), "obj3_stair_1 collider, aabb version");

        // obj3_stair_2, being in the tree, is expected to be colliding with either itself or another block right next to it
        const PureObject3D* const pObj3_stair_2CollidingWith = tree.findOneColliderObject_startFromFirstNode(*obj3_stair_2);
        b &= assertNotNull(pObj3_stair_2CollidingWith, "obj3_stair_2 collider, object version");

        const PureAxisAlignedBoundingBox obj3_stair_2Aabb(obj3_stair_2->getPosVec(), obj3_stair_2->getScaledSizeVec());
        b &= assertNotNull(tree.findOneColliderObject_startFromFirstNode(obj3_stair_2Aabb, nullptr), "obj3_stair_2 collider, aabb version");

        // obj3_stair_3, being in the tree, is expected to be colliding with either itself or another block right next to it
        const PureObject3D* const pObj3_stair_3CollidingWith = tree.findOneColliderObject_startFromFirstNode(*obj3_stair_3);
        b &= assertNotNull(pObj3_stair_3CollidingWith, "obj3_stair_3 collider, object version");

        const PureAxisAlignedBoundingBox obj3_stair_3Aabb(obj3_stair_3->getPosVec(), obj3_stair_3->getScaledSizeVec());
        b &= assertNotNull(tree.findOneColliderObject_startFromFirstNode(obj3_stair_3Aabb, nullptr), "obj3_stair_3 collider, aabb version");

        // obj3_stair_4, being in the tree, is expected to be colliding with either itself or another block right next to it
        const PureObject3D* const pObj3_stair_4CollidingWith = tree.findOneColliderObject_startFromFirstNode(*obj3_stair_4);
        b &= assertNotNull(pObj3_stair_4CollidingWith, "obj3_stair_4 collider, object version");

        const PureAxisAlignedBoundingBox obj3_stair_4Aabb(obj3_stair_4->getPosVec(), obj3_stair_4->getScaledSizeVec());
        b &= assertNotNull(tree.findOneColliderObject_startFromFirstNode(obj3_stair_4Aabb, nullptr), "obj3_stair_4 collider, aabb version");

        // obj4, being in the tree, is expected to be colliding with either itself or another block right next to it
        const PureObject3D* const pObj4CollidingWith = tree.findOneColliderObject_startFromFirstNode(*obj4);
        b &= assertNotNull(pObj4CollidingWith, "obj4 collider, object version");

        const PureAxisAlignedBoundingBox obj4Aabb(obj4->getPosVec(), obj4->getScaledSizeVec());
        b &= assertNotNull(tree.findOneColliderObject_startFromFirstNode(obj4Aabb, nullptr), "obj4 collider, aabb version");

        // create a player-sized object
        PureObject3D* objPlayer = om->createPlane(0.95f, 1.88f);

        // initially it should not touch any foreground blocks and obviously not colliding with self since player is not added to the tree
        objPlayer->getPosVec().Set(
            obj4->getPosVec().getX(),
            obj4->getPosVec().getY() + obj4->getSizeVec().getY() / 2.f + objPlayer->getScaledSizeVec().getY() / 2.f + 0.1f /* shall not touch obj4 FG block */,
            -1.2f /*GAME_PLAYERS_POS_Z*/);
        
        b &= assertNull(tree.findOneColliderObject_startFromFirstNode(*objPlayer), "objPlayer collider 1, object version");

        const PureAxisAlignedBoundingBox objPlayerAabb(objPlayer->getPosVec(), objPlayer->getScaledSizeVec());
        b &= assertNull(tree.findOneColliderObject_startFromFirstNode(objPlayerAabb, nullptr), "objPlayer collider 1, aabb version");

        // but then, as gravity pulls player down, it shall be colliding with a block object too
        objPlayer->getPosVec().SetY(objPlayer->getPosVec().getY() - 0.2f);

        b &= assertEquals(obj4, tree.findOneColliderObject_startFromFirstNode(*objPlayer), "objPlayer collider 2, object version");

        const PureAxisAlignedBoundingBox objPlayerAabb_2(objPlayer->getPosVec(), objPlayer->getScaledSizeVec());
        b &= assertEquals(obj4, tree.findOneColliderObject_startFromFirstNode(objPlayerAabb_2, nullptr), "objPlayer collider 2, aabb version");

        return b;
    }

    bool testFindOneColliderObject_2_startFromFirstNode()
    {
        bool b = true;

        bool bInitTreeDimensionsInCtor = true;
        for (int i = 0; i < 2; i++)
        {
            b &= assertTrue(
                testFindOneColliderObject_2_startFromFirstNode_main(bInitTreeDimensionsInCtor),
                (std::string(__func__) + ": bInitTreeDimensionsInCtor = " + std::to_string(bInitTreeDimensionsInCtor)).c_str());
            bInitTreeDimensionsInCtor = !bInitTreeDimensionsInCtor;
        }

        return b;
    }

    bool testFindAllColliderObjects_2_startFromFirstNode_main(bool initTreeDimensionsInCtor)
    {
        const PureVector treeOrigin(42.5f, -16.f, 0.f);
        const float treeSize = 85.f;  /* size as calculated during loading map_test_good_2_collision.txt */

        PureBoundingVolumeHierarchy tree_dimensionsInCtor(treeOrigin, treeSize, 3 /* maxDepth */, 0 /* currentDepth */);
        PureBoundingVolumeHierarchy tree_dimensionsAfterCtor(3 /* maxDepth */, 0 /* currentDepth */);
        PureBoundingVolumeHierarchy& tree = initTreeDimensionsInCtor ? tree_dimensionsInCtor : tree_dimensionsAfterCtor;
        if (!initTreeDimensionsInCtor)
        {
            bool bInit = true;
            bInit &= assertTrue(tree.setPos(treeOrigin), "tree setpos after ctor");
            bInit &= assertTrue(tree.setSize(treeSize), "tree setsize after ctor");
            if (!bInit)
            {
                return false;
            }
        }

        PureObject3D* obj1 = nullptr;
        PureObject3D* obj2 = nullptr;
        PureObject3D* obj3_stair_1 = nullptr;
        PureObject3D* obj3_stair_2 = nullptr;
        PureObject3D* obj3_stair_3 = nullptr;
        PureObject3D* obj3_stair_4 = nullptr;
        PureObject3D* obj4 = nullptr;
        PureBoundingVolumeHierarchy* node1 = nullptr;
        PureBoundingVolumeHierarchy* node2 = nullptr;
        PureBoundingVolumeHierarchy* node3_stair_1 = nullptr;
        PureBoundingVolumeHierarchy* node3_stair_2 = nullptr;
        PureBoundingVolumeHierarchy* node3_stair_3 = nullptr;
        PureBoundingVolumeHierarchy* node3_stair_4 = nullptr;
        PureBoundingVolumeHierarchy* node4 = nullptr;

        if (!assertTrue(
            buildBVH_2(
                tree, treeOrigin,
                obj1, obj2, obj3_stair_1, obj3_stair_2, obj3_stair_3, obj3_stair_4, obj4,
                node1, node2, node3_stair_1, node3_stair_2, node3_stair_3, node3_stair_4, node4), "buildBVH_2"))
        {
            return false;
        }

        bool b = true;

        std::vector<const PureObject3D*> colliders;


        // obj1, being in the tree, is expected to be colliding with itself
        colliders.clear();
        b &= assertTrue(tree.findAllColliderObjects_startFromFirstNode(*obj1, colliders), "obj1 colliders 1, object version");
        b &= assertFalse(colliders.empty(), "obj1 colliders empty, object version");
        if (b)
        {
            b &= assertEquals(obj1, *colliders.begin(), "obj1 colliders exact check, object version");
        }

        colliders.clear();
        const PureAxisAlignedBoundingBox obj1Aabb(obj1->getPosVec(), obj1->getScaledSizeVec());
        b &= assertTrue(tree.findAllColliderObjects_startFromFirstNode(obj1Aabb, nullptr, colliders), "obj1 colliders 1, aabb version");
        b &= assertFalse(colliders.empty(), "obj1 colliders empty, aabb version");
        if (b)
        {
            b &= assertEquals(obj1, *colliders.begin(), "obj1 colliders exact check, aabb version");
        }

        // obj2, being in the tree, is expected to be colliding with itself
        colliders.clear();
        b &= assertTrue(tree.findAllColliderObjects_startFromFirstNode(*obj2, colliders), "obj2 colliders 1, object version");
        b &= assertFalse(colliders.empty(), "obj2 colliders empty, object version");
        if (b)
        {
            b &= assertEquals(obj2, *colliders.begin(), "obj2 colliders exact check, object version");
        }

        colliders.clear();
        const PureAxisAlignedBoundingBox obj2Aabb(obj2->getPosVec(), obj2->getScaledSizeVec());
        b &= assertTrue(tree.findAllColliderObjects_startFromFirstNode(obj2Aabb, nullptr, colliders), "obj2 colliders 1, aabb version");
        b &= assertFalse(colliders.empty(), "obj2 colliders empty, aabb version");
        if (b)
        {
            b &= assertEquals(obj2, *colliders.begin(), "obj2 colliders exact check, aabb version");
        }

        // obj3_stair_1, being in the tree, is expected to be colliding with either itself or another block right next to it
        colliders.clear();
        b &= assertTrue(tree.findAllColliderObjects_startFromFirstNode(*obj3_stair_1, colliders), "obj3_stair_1 colliders 1, object version");
        b &= assertFalse(colliders.empty(), "obj3_stair_1 colliders empty, object version");
        if (b)
        {
            b &= assertTrue(std::find(colliders.begin(), colliders.end(), obj3_stair_1) != colliders.end(), "obj3_stair_1 colliders exact check 1, object version");
        }

        colliders.clear();
        const PureAxisAlignedBoundingBox obj3_stair_1Aabb(obj3_stair_1->getPosVec(), obj3_stair_1->getScaledSizeVec());
        b &= assertTrue(tree.findAllColliderObjects_startFromFirstNode(obj3_stair_1Aabb, nullptr, colliders), "obj3_stair_1 colliders 1, aabb version");
        b &= assertFalse(colliders.empty(), "obj3_stair_1 colliders empty, aabb version");
        if (b)
        {
            b &= assertTrue(std::find(colliders.begin(), colliders.end(), obj3_stair_1) != colliders.end(), "obj3_stair_1 colliders exact check 1, aabb version");
        }

        // obj3_stair_2, being in the tree, is expected to be colliding with either itself or another block right next to it
        colliders.clear();
        b &= assertTrue(tree.findAllColliderObjects_startFromFirstNode(*obj3_stair_2, colliders), "obj3_stair_2 colliders 1, object version");
        b &= assertFalse(colliders.empty(), "obj3_stair_2 colliders empty, object version");
        if (b)
        {
            b &= assertTrue(std::find(colliders.begin(), colliders.end(), obj3_stair_2) != colliders.end(), "obj3_stair_2 colliders exact check 1, object version");
        }

        colliders.clear();
        const PureAxisAlignedBoundingBox obj3_stair_2Aabb(obj3_stair_2->getPosVec(), obj3_stair_2->getScaledSizeVec());
        b &= assertTrue(tree.findAllColliderObjects_startFromFirstNode(obj3_stair_2Aabb, nullptr, colliders), "obj3_stair_2 colliders 1, aabb version");
        b &= assertFalse(colliders.empty(), "obj3_stair_2 colliders empty, aabb version");
        if (b)
        {
            b &= assertTrue(std::find(colliders.begin(), colliders.end(), obj3_stair_2) != colliders.end(), "obj3_stair_2 colliders exact check 1, aabb version");
        }

        // obj3_stair_3, being in the tree, is expected to be colliding with either itself or another block right next to it
        colliders.clear();
        b &= assertTrue(tree.findAllColliderObjects_startFromFirstNode(*obj3_stair_3, colliders), "obj3_stair_3 colliders 1, object version");
        b &= assertFalse(colliders.empty(), "obj3_stair_3 colliders empty, object version");
        if (b)
        {
            b &= assertTrue(std::find(colliders.begin(), colliders.end(), obj3_stair_3) != colliders.end(), "obj3_stair_3 colliders exact check 1, object version");
        }

        colliders.clear();
        const PureAxisAlignedBoundingBox obj3_stair_3Aabb(obj3_stair_3->getPosVec(), obj3_stair_3->getScaledSizeVec());
        b &= assertTrue(tree.findAllColliderObjects_startFromFirstNode(obj3_stair_3Aabb, nullptr, colliders), "obj3_stair_3 colliders 1, aabb version");
        b &= assertFalse(colliders.empty(), "obj3_stair_3 colliders empty, aabb version");
        if (b)
        {
            b &= assertTrue(std::find(colliders.begin(), colliders.end(), obj3_stair_3) != colliders.end(), "obj3_stair_3 colliders exact check 1, aabb version");
        }

        // obj3_stair_4, being in the tree, is expected to be colliding with either itself or another block right next to it
        colliders.clear();
        b &= assertTrue(tree.findAllColliderObjects_startFromFirstNode(*obj3_stair_4, colliders), "obj3_stair_4 colliders 1, object version");
        b &= assertFalse(colliders.empty(), "obj3_stair_4 colliders empty, object version");
        if (b)
        {
            b &= assertTrue(std::find(colliders.begin(), colliders.end(), obj3_stair_4) != colliders.end(), "obj3_stair_4 colliders exact check 1, object version");
        }

        colliders.clear();
        const PureAxisAlignedBoundingBox obj3_stair_4Aabb(obj3_stair_4->getPosVec(), obj3_stair_4->getScaledSizeVec());
        b &= assertTrue(tree.findAllColliderObjects_startFromFirstNode(obj3_stair_4Aabb, nullptr, colliders), "obj3_stair_4 colliders 1, aabb version");
        b &= assertFalse(colliders.empty(), "obj3_stair_4 colliders empty, aabb version");
        if (b)
        {
            b &= assertTrue(std::find(colliders.begin(), colliders.end(), obj3_stair_4) != colliders.end(), "obj3_stair_4 colliders exact check 1, aabb version");
        }

        // obj4, being in the tree, is expected to be colliding with either itself or another block right next to it
        colliders.clear();
        b &= assertTrue(tree.findAllColliderObjects_startFromFirstNode(*obj4, colliders), "obj4 colliders 1, object version");
        b &= assertFalse(colliders.empty(), "obj4 colliders empty, object version");
        if (b)
        {
            b &= assertTrue(std::find(colliders.begin(), colliders.end(), obj4) != colliders.end(), "obj4 colliders exact check 1, object version");
        }

        colliders.clear();
        const PureAxisAlignedBoundingBox obj4Aabb(obj4->getPosVec(), obj4->getScaledSizeVec());
        b &= assertTrue(tree.findAllColliderObjects_startFromFirstNode(obj4Aabb, nullptr, colliders), "obj4 colliders 1, aabb version");
        b &= assertFalse(colliders.empty(), "obj4 colliders empty, aabb version");
        if (b)
        {
            b &= assertTrue(std::find(colliders.begin(), colliders.end(), obj4) != colliders.end(), "obj4 colliders exact check 1, aabb version");
        }

        // create a player-sized object
        PureObject3D* objPlayer = om->createPlane(0.95f, 1.88f);

        // initially it should not touch any foreground blocks and obviously not colliding with self since player is not added to the tree
        objPlayer->getPosVec().Set(
            obj4->getPosVec().getX(),
            obj4->getPosVec().getY() + obj4->getSizeVec().getY() / 2.f + objPlayer->getScaledSizeVec().getY() / 2.f + 0.1f /* shall not touch obj4 FG block */,
            -1.2f /*GAME_PLAYERS_POS_Z*/);

        colliders.clear();
        b &= assertFalse(tree.findAllColliderObjects_startFromFirstNode(*objPlayer, colliders), "objPlayer colliders 1, object version");
        b &= assertTrue(colliders.empty(), "objPlayer colliders empty, object version 1");

        colliders.clear();
        const PureAxisAlignedBoundingBox objPlayerAabb(objPlayer->getPosVec(), objPlayer->getScaledSizeVec());
        b &= assertFalse(tree.findAllColliderObjects_startFromFirstNode(objPlayerAabb, nullptr, colliders), "objPlayer colliders 1, aabb version");
        b &= assertTrue(colliders.empty(), "objPlayer colliders empty, aabb version 1");

        // but then, as gravity pulls player down, it shall be colliding with a block object
        objPlayer->getPosVec().SetY(objPlayer->getPosVec().getY() - 0.2f);

        colliders.clear();
        b &= assertTrue(tree.findAllColliderObjects_startFromFirstNode(*objPlayer, colliders), "objPlayer colliders 2, object version");
        b &= assertFalse(colliders.empty(), "objPlayer colliders empty, object version 2");
        if (b)
        {
            b &= assertEquals(obj4, *colliders.begin(), "objPlayer colliders exact check 2, object version");
        }

        colliders.clear();
        const PureAxisAlignedBoundingBox objPlayerAabb_2(objPlayer->getPosVec(), objPlayer->getScaledSizeVec());
        b &= assertTrue(tree.findAllColliderObjects_startFromFirstNode(objPlayerAabb_2, nullptr, colliders), "objPlayer colliders 2, aabb version");
        b &= assertFalse(colliders.empty(), "objPlayer colliders empty, aabb version 2");
        if (b)
        {
            b &= assertEquals(obj4, *colliders.begin(), "objPlayer colliders exact check 2, aabb version");
        }

        return b;
    }

    bool testFindAllColliderObjects_2_startFromFirstNode()
    {
        bool b = true;

        bool bInitTreeDimensionsInCtor = true;
        for (int i = 0; i < 2; i++)
        {
            b &= assertTrue(
                testFindAllColliderObjects_2_startFromFirstNode_main(bInitTreeDimensionsInCtor),
                (std::string(__func__) + ": bInitTreeDimensionsInCtor = " + std::to_string(bInitTreeDimensionsInCtor)).c_str());
            bInitTreeDimensionsInCtor = !bInitTreeDimensionsInCtor;
        }

        return b;
    }

}; // class PureBoundingVolumeHierarchyTest