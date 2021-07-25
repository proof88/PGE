#pragma once

/*
    ###################################################################################
    PRREBoundingVolumeHierarchyTest.h
    Unit test for PRREBoundingVolumeHierarchy.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../PRRE/include/internal/SpatialStructures/PRREBoundingVolumeHierarchy.h"

class PRREBoundingVolumeHierarchyTest :
    public UnitTest
{
public:

    PRREBoundingVolumeHierarchyTest() :
        UnitTest( __FILE__ )
    {
    } // PRREBoundingVolumeHierarchyTest()

protected:

    virtual void Initialize()
    {
        engine = NULL;
        om = NULL;
        AddSubTest("testCtor1", (PFNUNITSUBTEST) &PRREBoundingVolumeHierarchyTest::testCtor1);
        AddSubTest("testInsertObject", (PFNUNITSUBTEST) &PRREBoundingVolumeHierarchyTest::testInsertObject);
    }

    virtual bool setUp()
    {
        bool ret = true;
        if ( engine == NULL )
        {
            engine = &PR00FsReducedRenderingEngine::createAndGet();
            ret &= assertEquals((TPRREuint)0, engine->initialize(PRRE_RENDERER_HW_FP, 800, 600, PRRE_WINDOWED, 0, 32, 24, 0, 0), "engine");  // pretty standard display mode, should work on most systems
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
    PRREObject3DManager* om;

    // ---------------------------------------------------------------------------

    PRREBoundingVolumeHierarchyTest(const PRREBoundingVolumeHierarchyTest&)
    {}         

    PRREBoundingVolumeHierarchyTest& operator=(const PRREBoundingVolumeHierarchyTest&)
    {
        return *this;
    }

    bool testCtor1()
    {
        PRREBoundingVolumeHierarchy tree(PRREVector(1, 2, 3), 1000.0f, 3, 0);
        
        return assertEquals(1.0f, tree.getPos().getX(), "pos.x") &
        assertEquals(2.0f, tree.getPos().getY(), "pos.y") &
        assertEquals(3.0f, tree.getPos().getZ(), "pos.z") &
        assertEquals((TPRREuint)3, tree.getMaxDepthLevel(), "max depth level") &
        assertEquals((TPRREuint)0, tree.getDepthLevel(), "root node depth level") &
        assertEquals(1000.0f, tree.getSize(), "size") &
        assertEquals(PRREOctree::NodeType::LeafEmpty, tree.getNodeType(), "nodeType") &
        assertEquals((std::size_t)0, tree.getChildren().size(), "children") &
        assertEquals((std::size_t)0, tree.getObjects().size(), "objects");
    }

    bool testInsertObject()
    {
        // basically this is the same test as found in PRREOctreeTest.h, but here we are interested in the
        // BVH-related stuff only

        // used to test if the given world-space position other than (0,0,0) is really taken into calculations;
        // if there is any issue, change this to (0,0,0) to find out probable reason of tree not properly taking origin pos into account!
        const PRREVector treeOrigin(100.f, 200.f, 300.f);

        PRREBoundingVolumeHierarchy tree(treeOrigin, 1000.0f, 2, 0);
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
        
        PRREObject3D* const obj1 = om->createBox(2.0f, 2.0f, 2.0f);
        PRREObject3D* const obj2 = om->createBox(2.0f, 2.0f, 2.0f);
        PRREObject3D* const obj3 = om->createBox(2.0f, 2.0f, 2.0f);
        PRREObject3D* const obj4 = om->createBox(2.0f, 2.0f, 2.0f);
        PRREObject3D* const obj5 = om->createBox(2.0f, 2.0f, 2.0f);
        if ( !assertNotNull(obj1, "obj1 not null") || !assertNotNull(obj2, "obj2 not null") || !assertNotNull(obj3, "obj3 not null") || !assertNotNull(obj4, "obj4 not null") || !assertNotNull(obj5, "obj5 not null") )
        {
            return false;
        }

        obj1->getPosVec().Set(treeOrigin.getX() - 200.f, treeOrigin.getY() + 200.f, treeOrigin.getZ() + 200.f);
        obj2->getPosVec().Set(treeOrigin.getX() + 200.f, treeOrigin.getY() - 200.f, treeOrigin.getZ() - 200.f);
        obj3->getPosVec().Set(treeOrigin.getX() + 200.f, treeOrigin.getY() - 200.f, treeOrigin.getZ() - 200.f);
        obj4->getPosVec().Set(treeOrigin.getX(), treeOrigin.getY(), treeOrigin.getZ());
        obj5->getPosVec().Set(treeOrigin.getX() + 3000.f, treeOrigin.getY(), treeOrigin.getZ());

        const PRREOctree* const node1 = tree.insertObject(*obj1);
        const PRREOctree* const node2 = tree.insertObject(*obj2);
        const PRREOctree* const node3 = tree.insertObject(*obj3);
        const PRREOctree* const node4 = tree.insertObject(*obj4);
        const PRREOctree* const node5 = tree.insertObject(*obj5);  // out of tree bounds
        if ( !assertNotNull(node1, "node 1") || !assertNotNull(node2, "node 2") || !assertNotNull(node3, "node 3") || !assertNotNull(node4, "node 4") || !assertNull(node5, "node 5") )
        {
            return false;
        }

        // check level 0 alias root node
        bool b = assertEquals(&tree, node1, "tree root is node1") &
            assertEquals(treeOrigin.getX(), tree.getPos().getX(), "tree pos.x") &
            assertEquals(treeOrigin.getY(), tree.getPos().getY(), "tree pos.y") &
            assertEquals(treeOrigin.getZ(), tree.getPos().getZ(), "tree pos.z") &
            assertEquals((TPRREuint)2, tree.getMaxDepthLevel(), "tree max depth level") &
            assertEquals((TPRREuint)0, tree.getDepthLevel(), "tree depth level") &
            assertEquals(1000.0f, tree.getSize(), "tree size") &
            assertEquals(PRREOctree::NodeType::Parent, tree.getNodeType(), "tree nodeType") &
            assertEquals((std::size_t)8, tree.getChildren().size(), "tree children") &
            assertEquals((std::size_t)0, tree.getObjects().size(), "tree objects");

        // check level 1 nodes' common properties
        for (std::size_t i = 0; i < tree.getChildren().size(); i++)
        {
            b &= assertEquals(500.0f, tree.getChildren()[i]->getSize(), (std::string("node 1 children ") + std::to_string(i) + " size").c_str());
            b &= assertEquals((TPRREuint)2, tree.getChildren()[i]->getMaxDepthLevel(), (std::string("node 1 children ") + std::to_string(i) + " maxDepthLevel").c_str());
            b &= assertEquals((TPRREuint)1, tree.getChildren()[i]->getDepthLevel(), (std::string("node 1 children ") + std::to_string(i) + " depthLevel").c_str());
        }

        // check level 1 nodes' different properties too
        const PRREOctree& tree_TLF = *(tree.getChildren()[PRREOctree::TOP    | PRREOctree::LEFT  | PRREOctree::FRONT]);
        const PRREOctree& tree_BLF = *(tree.getChildren()[PRREOctree::BOTTOM | PRREOctree::LEFT  | PRREOctree::FRONT]);
        const PRREOctree& tree_TRF = *(tree.getChildren()[PRREOctree::TOP    | PRREOctree::RIGHT | PRREOctree::FRONT]);
        const PRREOctree& tree_BRF = *(tree.getChildren()[PRREOctree::BOTTOM | PRREOctree::RIGHT | PRREOctree::FRONT]);
        const PRREOctree& tree_TLB = *(tree.getChildren()[PRREOctree::TOP    | PRREOctree::LEFT  | PRREOctree::BACK ]);
        const PRREOctree& tree_BLB = *(tree.getChildren()[PRREOctree::BOTTOM | PRREOctree::LEFT  | PRREOctree::BACK ]);
        const PRREOctree& tree_TRB = *(tree.getChildren()[PRREOctree::TOP    | PRREOctree::RIGHT | PRREOctree::BACK ]);
        const PRREOctree& tree_BRB = *(tree.getChildren()[PRREOctree::BOTTOM | PRREOctree::RIGHT | PRREOctree::BACK ]);

        b &= assertEquals(PRREOctree::NodeType::LeafEmpty, tree_TLF.getNodeType(), "tree_TLF nodeType") &
            assertEquals((std::size_t)0, tree_TLF.getChildren().size(), "tree_TLF children") &
            assertEquals((std::size_t)0, tree_TLF.getObjects().size(), "tree_TLF objects") &
            assertEquals(treeOrigin.getX()-250.0f, tree_TLF.getPos().getX(), "tree_TLF pos.x") &
            assertEquals(treeOrigin.getY()+250.0f, tree_TLF.getPos().getY(), "tree_TLF pos.y") &
            assertEquals(treeOrigin.getZ()-250.0f, tree_TLF.getPos().getZ(), "tree_TLF pos.z");
        
        b &= assertEquals(PRREOctree::NodeType::LeafEmpty, tree_BLF.getNodeType(), "tree_BLF nodeType") &
            assertEquals((std::size_t)0, tree_BLF.getChildren().size(), "tree_BLF children") &
            assertEquals((std::size_t)0, tree_BLF.getObjects().size(), "tree_BLF objects") &
            assertEquals(treeOrigin.getX()-250.0f, tree_BLF.getPos().getX(), "tree_BLF pos.x") &
            assertEquals(treeOrigin.getY()-250.0f, tree_BLF.getPos().getY(), "tree_BLF pos.y") &
            assertEquals(treeOrigin.getZ()-250.0f, tree_BLF.getPos().getZ(), "tree_BLF pos.z");

        b &= assertEquals(PRREOctree::NodeType::LeafEmpty, tree_TRF.getNodeType(), "tree_TRF nodeType") &
            assertEquals((std::size_t)0, tree_TRF.getChildren().size(), "tree_TRF children") &
            assertEquals((std::size_t)0, tree_TRF.getObjects().size(), "tree_TRF objects") &
            assertEquals(treeOrigin.getX()+250.0f, tree_TRF.getPos().getX(), "tree_TRF pos.x") &
            assertEquals(treeOrigin.getY()+250.0f, tree_TRF.getPos().getY(), "tree_TRF pos.y") &
            assertEquals(treeOrigin.getZ()-250.0f, tree_TRF.getPos().getZ(), "tree_TRF pos.z");

        b &= assertEquals(PRREOctree::NodeType::Parent, tree_BRF.getNodeType(), "tree_BRF nodeType") &
            assertEquals((std::size_t)8, tree_BRF.getChildren().size(), "tree_BRF children") &
            assertEquals((std::size_t)0, tree_BRF.getObjects().size(), "tree_BRF objects") &
            assertEquals(treeOrigin.getX()+250.0f, tree_BRF.getPos().getX(), "tree_BRF pos.x") &
            assertEquals(treeOrigin.getY()-250.0f, tree_BRF.getPos().getY(), "tree_BRF pos.y") &
            assertEquals(treeOrigin.getZ()-250.0f, tree_BRF.getPos().getZ(), "tree_BRF pos.z");

        b &= assertEquals(PRREOctree::NodeType::LeafContainer, tree_TLB.getNodeType(), "tree_TLB nodeType") &
            assertEquals((std::size_t)0, tree_TLB.getChildren().size(), "tree_TLB children") &
            assertEquals((std::size_t)1, tree_TLB.getObjects().size(), "tree_TLB objects") &
            assertTrue(tree_TLB.getObjects().end() != tree_TLB.getObjects().find(obj1), "tree_TLB objects contains obj1") &
            assertEquals(treeOrigin.getX()-250.0f, tree_TLB.getPos().getX(), "tree_TLB pos.x") &
            assertEquals(treeOrigin.getY()+250.0f, tree_TLB.getPos().getY(), "tree_TLB pos.y") &
            assertEquals(treeOrigin.getZ()+250.0f, tree_TLB.getPos().getZ(), "tree_TLB pos.z");

        b &= assertEquals(PRREOctree::NodeType::LeafEmpty, tree_BLB.getNodeType(), "tree_BLB nodeType") &
            assertEquals((std::size_t)0, tree_BLB.getChildren().size(), "tree_BLB children") &
            assertEquals((std::size_t)0, tree_BLB.getObjects().size(), "tree_BLB objects") &
            assertEquals(treeOrigin.getX()-250.0f, tree_BLB.getPos().getX(), "tree_BLB pos.x") &
            assertEquals(treeOrigin.getY()-250.0f, tree_BLB.getPos().getY(), "tree_BLB pos.y") &
            assertEquals(treeOrigin.getZ()+250.0f, tree_BLB.getPos().getZ(), "tree_BLB pos.z");

        b &= assertEquals(PRREOctree::NodeType::LeafContainer, tree_TRB.getNodeType(), "tree_TRB nodeType") &
            assertEquals((std::size_t)0, tree_TRB.getChildren().size(), "tree_TRB children") &
            assertEquals((std::size_t)1, tree_TRB.getObjects().size(), "tree_TRB objects") &
            assertTrue(tree_TRB.getObjects().end() != tree_TRB.getObjects().find(obj4), "tree_TRB objects contains obj4") &
            assertEquals(treeOrigin.getX()+250.0f, tree_TRB.getPos().getX(), "tree_TRB pos.x") &
            assertEquals(treeOrigin.getY()+250.0f, tree_TRB.getPos().getY(), "tree_TRB pos.y") &
            assertEquals(treeOrigin.getZ()+250.0f, tree_TRB.getPos().getZ(), "tree_TRB pos.z");

        b &= assertEquals(PRREOctree::NodeType::LeafEmpty, tree_BRB.getNodeType(), "tree_BRB nodeType") &
            assertEquals((std::size_t)0, tree_BRB.getChildren().size(), "tree_BRB children") &
            assertEquals((std::size_t)0, tree_BRB.getObjects().size(), "tree_BRB objects") &
            assertEquals(treeOrigin.getX()+250.0f, tree_BRB.getPos().getX(), "tree_BRB pos.x") &
            assertEquals(treeOrigin.getY()-250.0f, tree_BRB.getPos().getY(), "tree_BRB pos.y") &
            assertEquals(treeOrigin.getZ()+250.0f, tree_BRB.getPos().getZ(), "tree_BRB pos.z");
        
        // check level 2 nodes' common properties (we have level 2 because tree_BRF node have children)
        for (std::size_t i = 0; i < tree_BRF.getChildren().size(); i++)
        {
            b &= assertEquals(250.0f, tree_BRF.getChildren()[i]->getSize(), (std::string("tree_BRF children ") + std::to_string(i) + " size").c_str());
            b &= assertEquals((TPRREuint)2, tree_BRF.getChildren()[i]->getMaxDepthLevel(), (std::string("tree_BRF children ") + std::to_string(i) + " maxDepthLevel").c_str());
            b &= assertEquals((TPRREuint)2, tree_BRF.getChildren()[i]->getDepthLevel(), (std::string("tree_BRF children ") + std::to_string(i) + " depthLevel").c_str());
        }

        // check level 2 nodes' different properties too
        const PRREOctree& tree_BRF_TLF = *(tree_BRF.getChildren()[PRREOctree::TOP    | PRREOctree::LEFT  | PRREOctree::FRONT]);
        const PRREOctree& tree_BRF_BLF = *(tree_BRF.getChildren()[PRREOctree::BOTTOM | PRREOctree::LEFT  | PRREOctree::FRONT]);
        const PRREOctree& tree_BRF_TRF = *(tree_BRF.getChildren()[PRREOctree::TOP    | PRREOctree::RIGHT | PRREOctree::FRONT]);
        const PRREOctree& tree_BRF_BRF = *(tree_BRF.getChildren()[PRREOctree::BOTTOM | PRREOctree::RIGHT | PRREOctree::FRONT]);
        const PRREOctree& tree_BRF_TLB = *(tree_BRF.getChildren()[PRREOctree::TOP    | PRREOctree::LEFT  | PRREOctree::BACK ]);
        const PRREOctree& tree_BRF_BLB = *(tree_BRF.getChildren()[PRREOctree::BOTTOM | PRREOctree::LEFT  | PRREOctree::BACK ]);
        const PRREOctree& tree_BRF_TRB = *(tree_BRF.getChildren()[PRREOctree::TOP    | PRREOctree::RIGHT | PRREOctree::BACK ]);
        const PRREOctree& tree_BRF_BRB = *(tree_BRF.getChildren()[PRREOctree::BOTTOM | PRREOctree::RIGHT | PRREOctree::BACK ]);

        b &= assertEquals(PRREOctree::NodeType::LeafEmpty, tree_BRF_TLF.getNodeType(), "tree_BRF_TLF nodeType") &
            assertEquals((std::size_t)0, tree_BRF_TLF.getChildren().size(), "tree_BRF_TLF children") &
            assertEquals((std::size_t)0, tree_BRF_TLF.getObjects().size(), "tree_BRF_TLF objects") &
            assertEquals(treeOrigin.getX()+125.0f, tree_BRF_TLF.getPos().getX(), "tree_BRF_TLF pos.x") &
            assertEquals(treeOrigin.getY()-125.0f, tree_BRF_TLF.getPos().getY(), "tree_BRF_TLF pos.y") &
            assertEquals(treeOrigin.getZ()-375.0f, tree_BRF_TLF.getPos().getZ(), "tree_BRF_TLF pos.z");
        
        b &= assertEquals(PRREOctree::NodeType::LeafEmpty, tree_BRF_BLF.getNodeType(), "tree_BRF_BLF nodeType") &
            assertEquals((std::size_t)0, tree_BRF_BLF.getChildren().size(), "tree_BRF_BLF children") &
            assertEquals((std::size_t)0, tree_BRF_BLF.getObjects().size(), "tree_BRF_BLF objects") &
            assertEquals(treeOrigin.getX()+125.0f, tree_BRF_BLF.getPos().getX(), "tree_BRF_BLF pos.x") &
            assertEquals(treeOrigin.getY()-375.0f, tree_BRF_BLF.getPos().getY(), "tree_BRF_BLF pos.y") &
            assertEquals(treeOrigin.getZ()-375.0f, tree_BRF_BLF.getPos().getZ(), "tree_BRF_BLF pos.z");

        b &= assertEquals(PRREOctree::NodeType::LeafEmpty, tree_BRF_TRF.getNodeType(), "tree_BRF_TRF nodeType") &
            assertEquals((std::size_t)0, tree_BRF_TRF.getChildren().size(), "tree_BRF_TRF children") &
            assertEquals((std::size_t)0, tree_BRF_TRF.getObjects().size(), "tree_BRF_TRF objects") &
            assertEquals(treeOrigin.getX()+375.0f, tree_BRF_TRF.getPos().getX(), "tree_BRF_TRF pos.x") &
            assertEquals(treeOrigin.getY()-125.0f, tree_BRF_TRF.getPos().getY(), "tree_BRF_TRF pos.y") &
            assertEquals(treeOrigin.getZ()-375.0f, tree_BRF_TRF.getPos().getZ(), "tree_BRF_TRF pos.z");

        b &= assertEquals(PRREOctree::NodeType::LeafEmpty, tree_BRF_BRF.getNodeType(), "tree_BRF_BRF nodeType") &
            assertEquals((std::size_t)0, tree_BRF_BRF.getChildren().size(), "tree_BRF_BRF children") &
            assertEquals((std::size_t)0, tree_BRF_BRF.getObjects().size(), "tree_BRF_BRF objects") &
            assertEquals(treeOrigin.getX()+375.0f, tree_BRF_BRF.getPos().getX(), "tree_BRF_BRF pos.x") &
            assertEquals(treeOrigin.getY()-375.0f, tree_BRF_BRF.getPos().getY(), "tree_BRF_BRF pos.y") &
            assertEquals(treeOrigin.getZ()-375.0f, tree_BRF_BRF.getPos().getZ(), "tree_BRF_BRF pos.z");

        b &= assertEquals(PRREOctree::NodeType::LeafContainer, tree_BRF_TLB.getNodeType(), "tree_BRF_TLB nodeType") &
            assertEquals((std::size_t)0, tree_BRF_TLB.getChildren().size(), "tree_BRF_TLB children") &
            assertEquals((std::size_t)2, tree_BRF_TLB.getObjects().size(), "tree_BRF_TLB objects") &
            assertTrue(tree_BRF_TLB.getObjects().end() != tree_BRF_TLB.getObjects().find(obj2), "tree_BRF_TLB objects contains obj2") &
            assertTrue(tree_BRF_TLB.getObjects().end() != tree_BRF_TLB.getObjects().find(obj3), "tree_BRF_TLB objects contains obj3") &
            assertEquals(treeOrigin.getX()+125.0f, tree_BRF_TLB.getPos().getX(), "tree_BRF_TLB pos.x") &
            assertEquals(treeOrigin.getY()-125.0f, tree_BRF_TLB.getPos().getY(), "tree_BRF_TLB pos.y") &
            assertEquals(treeOrigin.getZ()-125.0f, tree_BRF_TLB.getPos().getZ(), "tree_BRF_TLB pos.z");

        b &= assertEquals(PRREOctree::NodeType::LeafEmpty, tree_BRF_BLB.getNodeType(), "tree_BRF_BLB nodeType") &
            assertEquals((std::size_t)0, tree_BRF_BLB.getChildren().size(), "tree_BRF_BLB children") &
            assertEquals((std::size_t)0, tree_BRF_BLB.getObjects().size(), "tree_BRF_BLB objects") &
            assertEquals(treeOrigin.getX()+125.0f, tree_BRF_BLB.getPos().getX(), "tree_BRF_BLB pos.x") &
            assertEquals(treeOrigin.getY()-375.0f, tree_BRF_BLB.getPos().getY(), "tree_BRF_BLB pos.y") &
            assertEquals(treeOrigin.getZ()-125.0f, tree_BRF_BLB.getPos().getZ(), "tree_BRF_BLB pos.z");

        b &= assertEquals(PRREOctree::NodeType::LeafEmpty, tree_BRF_TRB.getNodeType(), "tree_BRF_TRB nodeType") &
            assertEquals((std::size_t)0, tree_BRF_TRB.getChildren().size(), "tree_BRF_TRB children") &
            assertEquals((std::size_t)0, tree_BRF_TRB.getObjects().size(), "tree_BRF_TRB objects") &
            assertEquals(treeOrigin.getX()+375.0f, tree_BRF_TRB.getPos().getX(), "tree_BRF_TRB pos.x") &
            assertEquals(treeOrigin.getY()-125.0f, tree_BRF_TRB.getPos().getY(), "tree_BRF_TRB pos.y") &
            assertEquals(treeOrigin.getZ()-125.0f, tree_BRF_TRB.getPos().getZ(), "tree_BRF_TRB pos.z");

        b &= assertEquals(PRREOctree::NodeType::LeafEmpty, tree_BRF_BRB.getNodeType(), "tree_BRF_BRB nodeType") &
            assertEquals((std::size_t)0, tree_BRF_BRB.getChildren().size(), "tree_BRF_BRB children") &
            assertEquals((std::size_t)0, tree_BRF_BRB.getObjects().size(), "tree_BRF_BRB objects") &
            assertEquals(treeOrigin.getX()+375.0f, tree_BRF_BRB.getPos().getX(), "tree_BRF_BRB pos.x") &
            assertEquals(treeOrigin.getY()-375.0f, tree_BRF_BRB.getPos().getY(), "tree_BRF_BRB pos.y") &
            assertEquals(treeOrigin.getZ()-125.0f, tree_BRF_BRB.getPos().getZ(), "tree_BRF_BRB pos.z");

        return false;
    }

}; // class PRREBoundingVolumeHierarchyTest