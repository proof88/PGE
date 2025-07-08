#pragma once

/*
    ###################################################################################
    PureOctreeTest.h
    Unit test for PureOctree.
    Made by PR00F88
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../Pure/include/external/SpatialStructures/PureOctree.h"

class PureOctreeTest :
    public UnitTest
{
public:

    PureOctreeTest() :
        UnitTest( __FILE__ )
    {
    } // PureOctreeTest()

    ~PureOctreeTest() = default;

    PureOctreeTest(const PureOctreeTest&) = delete;
    PureOctreeTest& operator=(const PureOctreeTest&) = delete;
    PureOctreeTest(PureOctreeTest&&) = delete;
    PureOctreeTest& operator=(PureOctreeTest&&) = delete;

protected:

    virtual void initialize() override
    {
        engine = NULL;
        om = NULL;
        addSubTest("testCtor1", (PFNUNITSUBTEST)&PureOctreeTest::testCtor1);
        addSubTest("testCtor2", (PFNUNITSUBTEST) &PureOctreeTest::testCtor2);
        addSubTest("testCalculateChildIndex", (PFNUNITSUBTEST) &PureOctreeTest::testCalculateChildIndex);
        addSubTest("testInsertObject", (PFNUNITSUBTEST) &PureOctreeTest::testInsertObject);
        addSubTest("testFindObject", (PFNUNITSUBTEST) &PureOctreeTest::testFindObject);
        addSubTest("testSetSize", (PFNUNITSUBTEST)&PureOctreeTest::testSetSize);
        addSubTest("testSetPos", (PFNUNITSUBTEST)&PureOctreeTest::testSetPos);
        addSubTest("testSetMaxDepthLevel", (PFNUNITSUBTEST)&PureOctreeTest::testSetMaxDepthLevel);
        
        // reset(), getDepthLevel(), getMaxDepthLevel(), getNodeType(), getChildren() and getObjects() are tested within above functions
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
    * @param tree       An empty input tree where 5 new boxes will be inserted.
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
    * @return True if PureOctree is successfully built with the 4 new boxes inserted, false otherwise.
    */
    bool buildOctree(
        PureOctree& tree,
        const PureVector& treeOrigin,
        PureObject3D*& obj1,
        PureObject3D*& obj2,
        PureObject3D*& obj3,
        PureObject3D*& obj4,
        PureObject3D*& obj5,
        const PureOctree*& node1,
        const PureOctree*& node2,
        const PureOctree*& node3,
        const PureOctree*& node4)
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
        const PureOctree* node5 = tree.insertObject(*obj5);  // out of tree bounds
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

    bool assertTreeIsReset(const PureOctree& tree)
    {
        return (assertEquals(PureOctree::NodeType::LeafEmpty, tree.getNodeType(), "nodeType") &
            assertEquals((std::size_t)0, tree.getChildren().size(), "children") &
            assertEquals((std::size_t)0, tree.getObjects().size(), "objects")) != 0;
    }

    bool testCtor1()
    {
        PureOctree tree(3, 0);

        return (assertEquals(0.0f, tree.getPos().getX(), "pos.x") &
            assertEquals(0.0f, tree.getPos().getY(), "pos.y") &
            assertEquals(0.0f, tree.getPos().getZ(), "pos.z") &
            assertEquals((TPureUInt)3, tree.getMaxDepthLevel(), "max depth level") &
            assertEquals((TPureUInt)0, tree.getDepthLevel(), "root node depth level") &
            assertEquals(0.f, tree.getSize(), "size") &
            assertEquals((PureOctree*)PGENULL, tree.getParent(), "parent") &
            assertTrue(assertTreeIsReset(tree), "reset")) != 0;
    }
    
    bool testCtor2()
    {
        PureOctree tree(PureVector(1, 2, 3), 1000.0f, 3, 0);
        
        return (assertEquals(1.0f, tree.getPos().getX(), "pos.x") &
            assertEquals(2.0f, tree.getPos().getY(), "pos.y") &
            assertEquals(3.0f, tree.getPos().getZ(), "pos.z") &
            assertEquals((TPureUInt)3, tree.getMaxDepthLevel(), "max depth level") &
            assertEquals((TPureUInt)0, tree.getDepthLevel(), "root node depth level") &
            assertEquals(1000.0f, tree.getSize(), "size") &
            assertEquals((PureOctree*)PGENULL, tree.getParent(), "parent") &
            assertTrue(assertTreeIsReset(tree), "reset")) != 0;
    }

    bool testCalculateChildIndex()
    {
        PureOctree tree(PureVector(), 1000.0f, 2, 0);
        PureVector vec1(-200.f, 200.f, 200.f);
        PureVector vec2(200.f, -200.f, -200.f);
        PureVector vec3;

        return (assertEquals(PureOctree::ChildIndex(PureOctree::TOP | PureOctree::LEFT | PureOctree::BACK), tree.calculateChildIndex(vec1), "vec1") &
            assertEquals(PureOctree::ChildIndex(PureOctree::BOTTOM | PureOctree::RIGHT | PureOctree::FRONT), tree.calculateChildIndex(vec2), "vec2") &
            assertEquals(PureOctree::ChildIndex(PureOctree::TOP | PureOctree::RIGHT | PureOctree::BACK), tree.calculateChildIndex(vec3), "vec3")) != 0;
    }

    bool testInsertObject()
    {
        // used to test if the given world-space position other than (0,0,0) is really taken into calculations;
        // if there is any issue, change this to (0,0,0) to find out probable reason of tree not properly taking origin pos into account!
        const PureVector treeOrigin(100.f, 200.f, 300.f);

        PureObject3D* obj1 = nullptr;
        PureObject3D* obj2 = nullptr;
        PureObject3D* obj3 = nullptr;
        PureObject3D* obj4 = nullptr;
        PureObject3D* obj5 = nullptr;
        const PureOctree* node1 = nullptr;
        const PureOctree* node2 = nullptr;
        const PureOctree* node3 = nullptr;
        const PureOctree* node4 = nullptr;
        PureOctree tree(treeOrigin, 1000.0f, 2, 0);
        if (!assertTrue(buildOctree(tree, treeOrigin, obj1, obj2, obj3, obj4, obj5, node1, node2, node3, node4), "buildBVH"))
        {
            return false;
        }

        // check level 0 alias root node
        bool b = (
            assertEquals(treeOrigin.getX(), tree.getPos().getX(), "tree pos.x") &
            assertEquals(treeOrigin.getY(), tree.getPos().getY(), "tree pos.y") &
            assertEquals(treeOrigin.getZ(), tree.getPos().getZ(), "tree pos.z") &
            assertEquals((TPureUInt)2, tree.getMaxDepthLevel(), "tree max depth level") &
            assertEquals((TPureUInt)0, tree.getDepthLevel(), "tree depth level") &
            assertEquals(1000.0f, tree.getSize(), "tree size") &
            assertEquals(PureOctree::NodeType::Parent, tree.getNodeType(), "tree nodeType") &
            assertEquals((std::size_t)8, tree.getChildren().size(), "tree children") &
            assertEquals((PureOctree*)PGENULL, tree.getParent(), "parent") &
            assertEquals((std::size_t)0, tree.getObjects().size(), "tree objects")) != 0;

        // check level 1 nodes' common properties
        for (std::size_t i = 0; i < tree.getChildren().size(); i++)
        {
            b &= assertEquals(500.0f, tree.getChildren()[i]->getSize(), (std::string("node 1 children ") + std::to_string(i) + " size").c_str());
            b &= assertEquals(&tree, tree.getChildren()[i]->getParent(), (std::string("node 1 children ") + std::to_string(i) + " parent").c_str());
            b &= assertEquals((TPureUInt)2, tree.getChildren()[i]->getMaxDepthLevel(), (std::string("node 1 children ") + std::to_string(i) + " maxDepthLevel").c_str());
            b &= assertEquals((TPureUInt)1, tree.getChildren()[i]->getDepthLevel(), (std::string("node 1 children ") + std::to_string(i) + " depthLevel").c_str());
        }

        // check level 1 nodes' different properties too
        const PureOctree& tree_TLF = *(tree.getChildren()[PureOctree::TOP    | PureOctree::LEFT  | PureOctree::FRONT]);
        const PureOctree& tree_BLF = *(tree.getChildren()[PureOctree::BOTTOM | PureOctree::LEFT  | PureOctree::FRONT]);
        const PureOctree& tree_TRF = *(tree.getChildren()[PureOctree::TOP    | PureOctree::RIGHT | PureOctree::FRONT]);
        const PureOctree& tree_BRF = *(tree.getChildren()[PureOctree::BOTTOM | PureOctree::RIGHT | PureOctree::FRONT]);
        const PureOctree& tree_TLB = *(tree.getChildren()[PureOctree::TOP    | PureOctree::LEFT  | PureOctree::BACK ]);
        const PureOctree& tree_BLB = *(tree.getChildren()[PureOctree::BOTTOM | PureOctree::LEFT  | PureOctree::BACK ]);
        const PureOctree& tree_TRB = *(tree.getChildren()[PureOctree::TOP    | PureOctree::RIGHT | PureOctree::BACK ]);
        const PureOctree& tree_BRB = *(tree.getChildren()[PureOctree::BOTTOM | PureOctree::RIGHT | PureOctree::BACK ]);

        b &= assertEquals(PureOctree::NodeType::LeafEmpty, tree_TLF.getNodeType(), "tree_TLF nodeType") &
            assertEquals((std::size_t)0, tree_TLF.getChildren().size(), "tree_TLF children") &
            assertEquals((std::size_t)0, tree_TLF.getObjects().size(), "tree_TLF objects") &
            assertEquals(treeOrigin.getX()-250.0f, tree_TLF.getPos().getX(), "tree_TLF pos.x") &
            assertEquals(treeOrigin.getY()+250.0f, tree_TLF.getPos().getY(), "tree_TLF pos.y") &
            assertEquals(treeOrigin.getZ()-250.0f, tree_TLF.getPos().getZ(), "tree_TLF pos.z");
        
        b &= assertEquals(PureOctree::NodeType::LeafEmpty, tree_BLF.getNodeType(), "tree_BLF nodeType") &
            assertEquals((std::size_t)0, tree_BLF.getChildren().size(), "tree_BLF children") &
            assertEquals((std::size_t)0, tree_BLF.getObjects().size(), "tree_BLF objects") &
            assertEquals(treeOrigin.getX()-250.0f, tree_BLF.getPos().getX(), "tree_BLF pos.x") &
            assertEquals(treeOrigin.getY()-250.0f, tree_BLF.getPos().getY(), "tree_BLF pos.y") &
            assertEquals(treeOrigin.getZ()-250.0f, tree_BLF.getPos().getZ(), "tree_BLF pos.z");

        b &= assertEquals(PureOctree::NodeType::LeafEmpty, tree_TRF.getNodeType(), "tree_TRF nodeType") &
            assertEquals((std::size_t)0, tree_TRF.getChildren().size(), "tree_TRF children") &
            assertEquals((std::size_t)0, tree_TRF.getObjects().size(), "tree_TRF objects") &
            assertEquals(treeOrigin.getX()+250.0f, tree_TRF.getPos().getX(), "tree_TRF pos.x") &
            assertEquals(treeOrigin.getY()+250.0f, tree_TRF.getPos().getY(), "tree_TRF pos.y") &
            assertEquals(treeOrigin.getZ()-250.0f, tree_TRF.getPos().getZ(), "tree_TRF pos.z");

        b &= assertEquals(PureOctree::NodeType::Parent, tree_BRF.getNodeType(), "tree_BRF nodeType") &
            assertEquals((std::size_t)8, tree_BRF.getChildren().size(), "tree_BRF children") &
            assertEquals((std::size_t)0, tree_BRF.getObjects().size(), "tree_BRF objects") &
            assertEquals(treeOrigin.getX()+250.0f, tree_BRF.getPos().getX(), "tree_BRF pos.x") &
            assertEquals(treeOrigin.getY()-250.0f, tree_BRF.getPos().getY(), "tree_BRF pos.y") &
            assertEquals(treeOrigin.getZ()-250.0f, tree_BRF.getPos().getZ(), "tree_BRF pos.z");

        b &= assertEquals(PureOctree::NodeType::LeafContainer, tree_TLB.getNodeType(), "tree_TLB nodeType") &
            assertEquals((std::size_t)0, tree_TLB.getChildren().size(), "tree_TLB children") &
            assertEquals((std::size_t)1, tree_TLB.getObjects().size(), "tree_TLB objects") &
            assertTrue(tree_TLB.getObjects().end() != tree_TLB.getObjects().find(obj1), "tree_TLB objects contains obj1") &
            assertEquals(treeOrigin.getX()-250.0f, tree_TLB.getPos().getX(), "tree_TLB pos.x") &
            assertEquals(treeOrigin.getY()+250.0f, tree_TLB.getPos().getY(), "tree_TLB pos.y") &
            assertEquals(treeOrigin.getZ()+250.0f, tree_TLB.getPos().getZ(), "tree_TLB pos.z");

        b &= assertEquals(PureOctree::NodeType::LeafEmpty, tree_BLB.getNodeType(), "tree_BLB nodeType") &
            assertEquals((std::size_t)0, tree_BLB.getChildren().size(), "tree_BLB children") &
            assertEquals((std::size_t)0, tree_BLB.getObjects().size(), "tree_BLB objects") &
            assertEquals(treeOrigin.getX()-250.0f, tree_BLB.getPos().getX(), "tree_BLB pos.x") &
            assertEquals(treeOrigin.getY()-250.0f, tree_BLB.getPos().getY(), "tree_BLB pos.y") &
            assertEquals(treeOrigin.getZ()+250.0f, tree_BLB.getPos().getZ(), "tree_BLB pos.z");

        b &= assertEquals(PureOctree::NodeType::LeafContainer, tree_TRB.getNodeType(), "tree_TRB nodeType") &
            assertEquals((std::size_t)0, tree_TRB.getChildren().size(), "tree_TRB children") &
            assertEquals((std::size_t)1, tree_TRB.getObjects().size(), "tree_TRB objects") &
            assertTrue(tree_TRB.getObjects().end() != tree_TRB.getObjects().find(obj4), "tree_TRB objects contains obj4") &
            assertEquals(treeOrigin.getX()+250.0f, tree_TRB.getPos().getX(), "tree_TRB pos.x") &
            assertEquals(treeOrigin.getY()+250.0f, tree_TRB.getPos().getY(), "tree_TRB pos.y") &
            assertEquals(treeOrigin.getZ()+250.0f, tree_TRB.getPos().getZ(), "tree_TRB pos.z");

        b &= assertEquals(PureOctree::NodeType::LeafEmpty, tree_BRB.getNodeType(), "tree_BRB nodeType") &
            assertEquals((std::size_t)0, tree_BRB.getChildren().size(), "tree_BRB children") &
            assertEquals((std::size_t)0, tree_BRB.getObjects().size(), "tree_BRB objects") &
            assertEquals(treeOrigin.getX()+250.0f, tree_BRB.getPos().getX(), "tree_BRB pos.x") &
            assertEquals(treeOrigin.getY()-250.0f, tree_BRB.getPos().getY(), "tree_BRB pos.y") &
            assertEquals(treeOrigin.getZ()+250.0f, tree_BRB.getPos().getZ(), "tree_BRB pos.z");
        
        // check level 2 nodes' common properties (we have level 2 because tree_BRF node have children)
        for (std::size_t i = 0; i < tree_BRF.getChildren().size(); i++)
        {
            b &= assertEquals(250.0f, tree_BRF.getChildren()[i]->getSize(), (std::string("tree_BRF children ") + std::to_string(i) + " size").c_str());
            b &= assertEquals(&tree_BRF, tree_BRF.getChildren()[i]->getParent(), (std::string("node 1 children ") + std::to_string(i) + " parent").c_str());
            b &= assertEquals((TPureUInt)2, tree_BRF.getChildren()[i]->getMaxDepthLevel(), (std::string("tree_BRF children ") + std::to_string(i) + " maxDepthLevel").c_str());
            b &= assertEquals((TPureUInt)2, tree_BRF.getChildren()[i]->getDepthLevel(), (std::string("tree_BRF children ") + std::to_string(i) + " depthLevel").c_str());
        }

        // check level 2 nodes' different properties too
        const PureOctree& tree_BRF_TLF = *(tree_BRF.getChildren()[PureOctree::TOP    | PureOctree::LEFT  | PureOctree::FRONT]);
        const PureOctree& tree_BRF_BLF = *(tree_BRF.getChildren()[PureOctree::BOTTOM | PureOctree::LEFT  | PureOctree::FRONT]);
        const PureOctree& tree_BRF_TRF = *(tree_BRF.getChildren()[PureOctree::TOP    | PureOctree::RIGHT | PureOctree::FRONT]);
        const PureOctree& tree_BRF_BRF = *(tree_BRF.getChildren()[PureOctree::BOTTOM | PureOctree::RIGHT | PureOctree::FRONT]);
        const PureOctree& tree_BRF_TLB = *(tree_BRF.getChildren()[PureOctree::TOP    | PureOctree::LEFT  | PureOctree::BACK ]);
        const PureOctree& tree_BRF_BLB = *(tree_BRF.getChildren()[PureOctree::BOTTOM | PureOctree::LEFT  | PureOctree::BACK ]);
        const PureOctree& tree_BRF_TRB = *(tree_BRF.getChildren()[PureOctree::TOP    | PureOctree::RIGHT | PureOctree::BACK ]);
        const PureOctree& tree_BRF_BRB = *(tree_BRF.getChildren()[PureOctree::BOTTOM | PureOctree::RIGHT | PureOctree::BACK ]);

        b &= assertEquals(PureOctree::NodeType::LeafEmpty, tree_BRF_TLF.getNodeType(), "tree_BRF_TLF nodeType") &
            assertEquals((std::size_t)0, tree_BRF_TLF.getChildren().size(), "tree_BRF_TLF children") &
            assertEquals((std::size_t)0, tree_BRF_TLF.getObjects().size(), "tree_BRF_TLF objects") &
            assertEquals(treeOrigin.getX()+125.0f, tree_BRF_TLF.getPos().getX(), "tree_BRF_TLF pos.x") &
            assertEquals(treeOrigin.getY()-125.0f, tree_BRF_TLF.getPos().getY(), "tree_BRF_TLF pos.y") &
            assertEquals(treeOrigin.getZ()-375.0f, tree_BRF_TLF.getPos().getZ(), "tree_BRF_TLF pos.z");
        
        b &= assertEquals(PureOctree::NodeType::LeafEmpty, tree_BRF_BLF.getNodeType(), "tree_BRF_BLF nodeType") &
            assertEquals((std::size_t)0, tree_BRF_BLF.getChildren().size(), "tree_BRF_BLF children") &
            assertEquals((std::size_t)0, tree_BRF_BLF.getObjects().size(), "tree_BRF_BLF objects") &
            assertEquals(treeOrigin.getX()+125.0f, tree_BRF_BLF.getPos().getX(), "tree_BRF_BLF pos.x") &
            assertEquals(treeOrigin.getY()-375.0f, tree_BRF_BLF.getPos().getY(), "tree_BRF_BLF pos.y") &
            assertEquals(treeOrigin.getZ()-375.0f, tree_BRF_BLF.getPos().getZ(), "tree_BRF_BLF pos.z");

        b &= assertEquals(PureOctree::NodeType::LeafEmpty, tree_BRF_TRF.getNodeType(), "tree_BRF_TRF nodeType") &
            assertEquals((std::size_t)0, tree_BRF_TRF.getChildren().size(), "tree_BRF_TRF children") &
            assertEquals((std::size_t)0, tree_BRF_TRF.getObjects().size(), "tree_BRF_TRF objects") &
            assertEquals(treeOrigin.getX()+375.0f, tree_BRF_TRF.getPos().getX(), "tree_BRF_TRF pos.x") &
            assertEquals(treeOrigin.getY()-125.0f, tree_BRF_TRF.getPos().getY(), "tree_BRF_TRF pos.y") &
            assertEquals(treeOrigin.getZ()-375.0f, tree_BRF_TRF.getPos().getZ(), "tree_BRF_TRF pos.z");

        b &= assertEquals(PureOctree::NodeType::LeafEmpty, tree_BRF_BRF.getNodeType(), "tree_BRF_BRF nodeType") &
            assertEquals((std::size_t)0, tree_BRF_BRF.getChildren().size(), "tree_BRF_BRF children") &
            assertEquals((std::size_t)0, tree_BRF_BRF.getObjects().size(), "tree_BRF_BRF objects") &
            assertEquals(treeOrigin.getX()+375.0f, tree_BRF_BRF.getPos().getX(), "tree_BRF_BRF pos.x") &
            assertEquals(treeOrigin.getY()-375.0f, tree_BRF_BRF.getPos().getY(), "tree_BRF_BRF pos.y") &
            assertEquals(treeOrigin.getZ()-375.0f, tree_BRF_BRF.getPos().getZ(), "tree_BRF_BRF pos.z");

        b &= assertEquals(PureOctree::NodeType::LeafContainer, tree_BRF_TLB.getNodeType(), "tree_BRF_TLB nodeType") &
            assertEquals((std::size_t)0, tree_BRF_TLB.getChildren().size(), "tree_BRF_TLB children") &
            assertEquals((std::size_t)2, tree_BRF_TLB.getObjects().size(), "tree_BRF_TLB objects") &
            assertTrue(tree_BRF_TLB.getObjects().end() != tree_BRF_TLB.getObjects().find(obj2), "tree_BRF_TLB objects contains obj2") &
            assertTrue(tree_BRF_TLB.getObjects().end() != tree_BRF_TLB.getObjects().find(obj3), "tree_BRF_TLB objects contains obj3") &
            assertEquals(treeOrigin.getX()+125.0f, tree_BRF_TLB.getPos().getX(), "tree_BRF_TLB pos.x") &
            assertEquals(treeOrigin.getY()-125.0f, tree_BRF_TLB.getPos().getY(), "tree_BRF_TLB pos.y") &
            assertEquals(treeOrigin.getZ()-125.0f, tree_BRF_TLB.getPos().getZ(), "tree_BRF_TLB pos.z");

        b &= assertEquals(PureOctree::NodeType::LeafEmpty, tree_BRF_BLB.getNodeType(), "tree_BRF_BLB nodeType") &
            assertEquals((std::size_t)0, tree_BRF_BLB.getChildren().size(), "tree_BRF_BLB children") &
            assertEquals((std::size_t)0, tree_BRF_BLB.getObjects().size(), "tree_BRF_BLB objects") &
            assertEquals(treeOrigin.getX()+125.0f, tree_BRF_BLB.getPos().getX(), "tree_BRF_BLB pos.x") &
            assertEquals(treeOrigin.getY()-375.0f, tree_BRF_BLB.getPos().getY(), "tree_BRF_BLB pos.y") &
            assertEquals(treeOrigin.getZ()-125.0f, tree_BRF_BLB.getPos().getZ(), "tree_BRF_BLB pos.z");

        b &= assertEquals(PureOctree::NodeType::LeafEmpty, tree_BRF_TRB.getNodeType(), "tree_BRF_TRB nodeType") &
            assertEquals((std::size_t)0, tree_BRF_TRB.getChildren().size(), "tree_BRF_TRB children") &
            assertEquals((std::size_t)0, tree_BRF_TRB.getObjects().size(), "tree_BRF_TRB objects") &
            assertEquals(treeOrigin.getX()+375.0f, tree_BRF_TRB.getPos().getX(), "tree_BRF_TRB pos.x") &
            assertEquals(treeOrigin.getY()-125.0f, tree_BRF_TRB.getPos().getY(), "tree_BRF_TRB pos.y") &
            assertEquals(treeOrigin.getZ()-125.0f, tree_BRF_TRB.getPos().getZ(), "tree_BRF_TRB pos.z");

        b &= assertEquals(PureOctree::NodeType::LeafEmpty, tree_BRF_BRB.getNodeType(), "tree_BRF_BRB nodeType") &
            assertEquals((std::size_t)0, tree_BRF_BRB.getChildren().size(), "tree_BRF_BRB children") &
            assertEquals((std::size_t)0, tree_BRF_BRB.getObjects().size(), "tree_BRF_BRB objects") &
            assertEquals(treeOrigin.getX()+375.0f, tree_BRF_BRB.getPos().getX(), "tree_BRF_BRB pos.x") &
            assertEquals(treeOrigin.getY()-375.0f, tree_BRF_BRB.getPos().getY(), "tree_BRF_BRB pos.y") &
            assertEquals(treeOrigin.getZ()-125.0f, tree_BRF_BRB.getPos().getZ(), "tree_BRF_BRB pos.z");


        // now check reset() behavior

        // reset() negative test: try reset level 2 nodes (we have level 2 because tree_BRF node have children)
        for (std::size_t i = 0; i < tree_BRF.getChildren().size(); i++)
        {
            b &= assertFalse(tree_BRF.getChildren()[i]->reset(), (std::string("reset level 2 node ") + std::to_string(i)).c_str());
            b &= assertEquals(250.0f, tree_BRF.getChildren()[i]->getSize(), (std::string("tree_BRF children ") + std::to_string(i) + " size").c_str());
            b &= assertEquals(&tree_BRF, tree_BRF.getChildren()[i]->getParent(), (std::string("node 1 children ") + std::to_string(i) + " parent").c_str());
            b &= assertEquals((TPureUInt)2, tree_BRF.getChildren()[i]->getMaxDepthLevel(), (std::string("tree_BRF children ") + std::to_string(i) + " maxDepthLevel").c_str());
            b &= assertEquals((TPureUInt)2, tree_BRF.getChildren()[i]->getDepthLevel(), (std::string("tree_BRF children ") + std::to_string(i) + " depthLevel").c_str());
        }

        // reset() negative test: try reset level 1 nodes
        for (std::size_t i = 0; i < tree.getChildren().size(); i++)
        {
            b &= assertFalse(tree.getChildren()[i]->reset(), (std::string("reset level 1 node ") + std::to_string(i)).c_str());
            b &= assertEquals(500.0f, tree.getChildren()[i]->getSize(), (std::string("node 1 children ") + std::to_string(i) + " size").c_str());
            b &= assertEquals(&tree, tree.getChildren()[i]->getParent(), (std::string("node 1 children ") + std::to_string(i) + " parent").c_str());
            b &= assertEquals((TPureUInt)2, tree.getChildren()[i]->getMaxDepthLevel(), (std::string("node 1 children ") + std::to_string(i) + " maxDepthLevel").c_str());
            b &= assertEquals((TPureUInt)1, tree.getChildren()[i]->getDepthLevel(), (std::string("node 1 children ") + std::to_string(i) + " depthLevel").c_str());
        }
        // check level 2 nodes unaffected
        for (std::size_t i = 0; i < tree_BRF.getChildren().size(); i++)
        {
            b &= assertEquals(250.0f, tree_BRF.getChildren()[i]->getSize(), (std::string("tree_BRF children ") + std::to_string(i) + " size").c_str());
            b &= assertEquals(&tree_BRF, tree_BRF.getChildren()[i]->getParent(), (std::string("node 1 children ") + std::to_string(i) + " parent").c_str());
            b &= assertEquals((TPureUInt)2, tree_BRF.getChildren()[i]->getMaxDepthLevel(), (std::string("tree_BRF children ") + std::to_string(i) + " maxDepthLevel").c_str());
            b &= assertEquals((TPureUInt)2, tree_BRF.getChildren()[i]->getDepthLevel(), (std::string("tree_BRF children ") + std::to_string(i) + " depthLevel").c_str());
        }

        // reset() positive test when we have multi-level tree built above
        b &= assertTrue(tree.reset(), "reset level 0");
        b &= assertTrue(assertTreeIsReset(tree), "tree is reset 1");

        // reset() positive test when only 1 object is in root node
        const PureOctree* const node1_newtree = tree.insertObject(*obj1);
        if (!assertNotNull(node1_newtree, "node1_newtree 1"))
        {
            return false;
        }
        b &= assertEquals(&tree, node1_newtree, "tree root is node1_newtree");
        b &= assertTrue(tree.reset(), "reset level 0 again");
        b &= assertTrue(assertTreeIsReset(tree), "tree is reset 2");

        return b;
    }

    bool testFindObject()
    {
        // used to test if the given world-space position other than (0,0,0) is really taken into calculations;
        // if there is any issue, change this to (0,0,0) to find out probable reason of tree not properly taking origin pos into account!
        const PureVector treeOrigin(100.f, 200.f, 300.f);

        PureObject3D* obj1 = nullptr;
        PureObject3D* obj2 = nullptr;
        PureObject3D* obj3 = nullptr;
        PureObject3D* obj4 = nullptr;
        PureObject3D* obj5 = nullptr;
        const PureOctree* node1 = nullptr;
        const PureOctree* node2 = nullptr;
        const PureOctree* node3 = nullptr;
        const PureOctree* node4 = nullptr;
        PureOctree tree(treeOrigin, 1000.0f, 2, 0);
        if (!assertTrue(buildOctree(tree, treeOrigin, obj1, obj2, obj3, obj4, obj5, node1, node2, node3, node4), "buildBVH"))
        {
            return false;
        }

        // now check if actually the proper nodes have been found!
        if ( !assertEquals((std::size_t)8, tree.getChildren().size(), "tree children size") )
        {   // avoid crash
            return false;
        }

        const PureOctree& tree_TLB = *(tree.getChildren()[PureOctree::TOP    | PureOctree::LEFT  | PureOctree::BACK ]);
        const PureOctree& tree_BRF = *(tree.getChildren()[PureOctree::BOTTOM | PureOctree::RIGHT | PureOctree::FRONT]);
        const PureOctree& tree_TRB = *(tree.getChildren()[PureOctree::TOP    | PureOctree::RIGHT | PureOctree::BACK ]);

        bool b = assertEquals(&tree_TLB, node1, "node1 is tree_TLB");
        b &= assertEquals(&tree_TRB, node4, "node4 is tree_TRB");

        if ( !assertEquals((std::size_t)8, tree_BRF.getChildren().size(), "tree_BRF children size") )
        {   // avoid crash
            return false;
        }
        const PureOctree& tree_BRF_TLB = *(tree_BRF.getChildren()[PureOctree::TOP    | PureOctree::LEFT  | PureOctree::BACK ]);

        b &= assertEquals(&tree_BRF_TLB, node2, "found_node2 is tree_BRF_TLB");
        b &= assertEquals(&tree_BRF_TLB, node3, "found_node3 is tree_BRF_TLB");

        return b;
    }

    bool testSetSize()
    {
        PureOctree treeCtor1(2, 0);

        bool b = true;
        b &= assertTrue(treeCtor1.setSize(50.f), "size 0");

        PureObject3D* const obj1 = om->createBox(1.0f, 1.0f, 1.0f);
        PureObject3D* const obj2 = om->createBox(1.0f, 1.0f, 1.0f);
        if (!assertNotNull(obj1, "obj1 not null") || !assertNotNull(obj2, "obj2 not null"))
        {
            return false;
        }

        // 1 level
        const PureOctree* const node1 = treeCtor1.insertObject(*obj1);
        if (!assertNotNull(node1, "node 1"))
        {
            return false;
        }

        b &= assertFalse(treeCtor1.setSize(2.f), "size 1");
        b &= assertEquals(50.f, treeCtor1.getSize(), "size 2");

        // 2 levels
        PureOctree* const node2 = treeCtor1.insertObject(*obj2);
        if (!assertNotNull(node2, "node 2"))
        {
            return false;
        }

        const TPureFloat childNodeSize = node2->getSize();
        b &= assertFalse(node2->setSize(60.f), "size 3");
        b &= assertEquals(childNodeSize, node2->getSize(), "size 4");

        b &= assertFalse(treeCtor1.setSize(2.f), "size 5");
        b &= assertEquals(50.f, treeCtor1.getSize(), "size 6");

        // after reset()
        b &= assertTrue(treeCtor1.reset(), "reset 1");
        b &= assertTrue(treeCtor1.setSize(2.f), "size 7");
        b &= assertEquals(2.f, treeCtor1.getSize(), "size 8");
        
        // different ctor
        const PureVector treeOrigin(100.f, 200.f, 300.f);
        PureOctree treeCtor2(treeOrigin, 1000.0f, 2, 0);
        b &= assertTrue(treeCtor2.setSize(2.f), "size 9");
        b &= assertEquals(2.f, treeCtor1.getSize(), "size 10");
        
        return b;
    }

    bool testSetPos()
    {
        PureOctree treeCtor1(2, 0);

        bool b = true;
        b &= assertTrue(treeCtor1.setSize(5000.f), "size 0");

        const PureVector treeOrigin(100.f, 200.f, 300.f);
        b &= assertTrue(treeCtor1.setPos(treeOrigin), "pos 0");

        PureObject3D* const obj1 = om->createBox(1.0f, 1.0f, 1.0f);
        PureObject3D* const obj2 = om->createBox(1.0f, 1.0f, 1.0f);
        if (!assertNotNull(obj1, "obj1 not null") || !assertNotNull(obj2, "obj2 not null"))
        {
            return false;
        }

        // 1 level
        const PureOctree* const node1 = treeCtor1.insertObject(*obj1);
        if (!assertNotNull(node1, "node 1"))
        {
            return false;
        }

        b &= assertFalse(treeCtor1.setPos(PureVector(5, 6, 7)), "pos 1");
        b &= assertEquals(treeOrigin, treeCtor1.getPos(), "pos 2");

        // 2 levels
        PureOctree* const node2 = treeCtor1.insertObject(*obj2);
        if (!assertNotNull(node2, "node 2"))
        {
            return false;
        }

        const PureVector childNodePos = node2->getPos();
        b &= assertFalse(node2->setPos(PureVector(5, 6, 7)), "pos 3");
        b &= assertEquals(childNodePos, node2->getPos(), "pos 4");

        b &= assertFalse(treeCtor1.setPos(PureVector(5, 6, 7)), "pos 5");
        b &= assertEquals(treeOrigin, treeCtor1.getPos(), "pos 6");

        // after reset()
        b &= assertTrue(treeCtor1.reset(), "reset 1");
        b &= assertTrue(treeCtor1.setPos(PureVector(5, 6, 7)), "pos 7");
        b &= assertEquals(PureVector(5, 6, 7), treeCtor1.getPos(), "pos 8");
        
        // different ctor
        PureOctree treeCtor2(treeOrigin, 1000.0f, 2, 0);
        b &= assertTrue(treeCtor2.setPos(PureVector(5, 6, 7)), "pos 9");
        b &= assertEquals(PureVector(5, 6, 7), treeCtor2.getPos(), "pos 10");

        return b;
    }

    bool testSetMaxDepthLevel()
    {
        PureOctree treeCtor1(2, 0);

        bool b = true;
        b &= assertFalse(treeCtor1.setMaxDepthLevel(0), "size 0");
        b &= assertEquals(2u, treeCtor1.getMaxDepthLevel(), "size 1");
        b &= assertTrue(treeCtor1.setMaxDepthLevel(4), "size 2");
        b &= assertEquals(4u, treeCtor1.getMaxDepthLevel(), "size 3");

        PureObject3D* const obj1 = om->createBox(1.0f, 1.0f, 1.0f);
        PureObject3D* const obj2 = om->createBox(1.0f, 1.0f, 1.0f);
        if (!assertNotNull(obj1, "obj1 not null") || !assertNotNull(obj2, "obj2 not null"))
        {
            return false;
        }

        // 1 level
        const PureOctree* const node1 = treeCtor1.insertObject(*obj1);
        if (!assertNotNull(node1, "node 1"))
        {
            return false;
        }

        b &= assertFalse(treeCtor1.setMaxDepthLevel(2), "size 4");
        b &= assertEquals(4u, treeCtor1.getMaxDepthLevel(), "size 5");

        // 2 levels
        PureOctree* const node2 = treeCtor1.insertObject(*obj2);
        if (!assertNotNull(node2, "node 2"))
        {
            return false;
        }

        b &= assertFalse(node2->setMaxDepthLevel(2), "size 6");
        b &= assertEquals(4u, node2->getMaxDepthLevel(), "size 7");

        b &= assertFalse(treeCtor1.setMaxDepthLevel(2), "size 8");
        b &= assertEquals(4u, treeCtor1.getMaxDepthLevel(), "size 9");

        // after reset()
        b &= assertTrue(treeCtor1.reset(), "reset 1");
        b &= assertTrue(treeCtor1.setMaxDepthLevel(2), "size 10");
        b &= assertEquals(2u, treeCtor1.getMaxDepthLevel(), "size 11");

        // different ctor
        const PureVector treeOrigin(100.f, 200.f, 300.f);
        PureOctree treeCtor2(treeOrigin, 1000.0f, 2, 0);
        b &= assertTrue(treeCtor2.setMaxDepthLevel(4), "size 12");
        b &= assertEquals(4u, treeCtor2.getMaxDepthLevel(), "size 13");

        return b;
    }

}; // class PureOctreeTest