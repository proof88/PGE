/*
    ###################################################################################
    PureOctree.cpp
    This file is part of PURE.
    PureOctree class.
    Made by PR00F88
    ###################################################################################
*/

#include "PureBaseIncludes.h"  // PCH
#include "../../include/external/SpatialStructures/PureOctree.h"

#include <cassert>

#include "../../../../../PFL/PFL/PFL.h"

using namespace std;

/*
   PureOctree
   ###########################################################################
*/


// ############################### PUBLIC ################################


const PureOctree::ChildIndex PureOctree::BIT_AXIS_Y = 0;
const PureOctree::ChildIndex PureOctree::BIT_AXIS_X = 1;
const PureOctree::ChildIndex PureOctree::BIT_AXIS_Z = 2;

const PureOctree::ChildIndex PureOctree::TOP    = 0;
const PureOctree::ChildIndex PureOctree::BOTTOM = BIT(PureOctree::BIT_AXIS_Y);
const PureOctree::ChildIndex PureOctree::LEFT   = 0;
const PureOctree::ChildIndex PureOctree::RIGHT  = BIT(PureOctree::BIT_AXIS_X);
const PureOctree::ChildIndex PureOctree::FRONT  = 0;
const PureOctree::ChildIndex PureOctree::BACK   = BIT(PureOctree::BIT_AXIS_Z);


/**
    Creates an octree node.
    Initially the type of this node will be LeafEmpty.
    This ctor initializes size to 0 and position to [0,0,0].
    Size and position can be set later by setSize() and setPos() only if the tree is empty.

    @param maxDepthLevel The maximum node depth level supported by this octree. 0 means there is no depth limit.
    @param currentDepthLevel The depth level of this specific node being created. For global use, just specify 0 so this will be the root node of the octree.
*/
PureOctree::PureOctree(TPureUInt maxDepthLevel, TPureUInt currentDepthLevel) :
    fSize(0.f),
    nMaxDepth(maxDepthLevel),
    nCurrentDepth(currentDepthLevel),
    nodeType(NodeType::LeafEmpty),
    parent(PGENULL),
    objDebugBox(nullptr)
{
    
} // PureOctree(...)

/**
    Creates an octree node.
    Initially the type of this node will be LeafEmpty.

    @param pos The world-space position of this node. For global use, you can specify just [0,0,0].
    @param size The length of the side of the cube represented by this node. Recommend this to be big enough to contain any scene objects.
    @param maxDepthLevel The maximum node depth level supported by this octree. 0 means there is no depth limit.
    @param currentDepthLevel The depth level of this specific node being created. For global use, just specify 0 so this will be the root node of the octree.
*/
PureOctree::PureOctree(const PureVector& pos, TPureFloat size, TPureUInt maxDepthLevel, TPureUInt currentDepthLevel) :
    vPos(pos),
    fSize(size),
    nMaxDepth(maxDepthLevel),
    nCurrentDepth(currentDepthLevel),
    nodeType(NodeType::LeafEmpty),
    parent(PGENULL),
    objDebugBox(nullptr)
{

} // PureOctree(...)


PureOctree::~PureOctree()
{
    DeleteChildren();
    if (objDebugBox)
    {
        delete objDebugBox; // Object3D dtor triggers removing from ObjectManager too
    }
} // ~PureOctree()


/**
    Calculates child node index for the given world-space position in the current node.
    Bounds checking is NOT done, so giving a position which is outside the bounds of this Octree Node will not be detected.

    @param pos The world-space position for which child index should be selected in the current node.
    @return Calculated child index. If given position is same as Octree Node position, index will be TopRightBack.
*/
PureOctree::ChildIndex PureOctree::calculateIndex(const PureVector& pos) const
{
    ChildIndex iChild = 0;  // TopLeftFront
    
    if ( pos.getX() >= vPos.getX() )
    {
        iChild |= RIGHT;
    }
    if ( pos.getY() < vPos.getY() )
    {
        iChild |= BOTTOM;
    }
    if ( pos.getZ() >= vPos.getZ() )
    {
        iChild |= BACK;
    }

    return iChild;
} // calculateIndex()


/**
    Inserts the given object in the octree.
    The tree doesn't make copy of the objects, it just saves pointers to them.
    Note that if you insert the same object multiple times with same position, it will be still contained only once in the tree, within the same node.
    If you insert the same object multiple times with differens positions, it might be contained by multiple nodes, which will lead to invalid containments.
    However, since this Octree implementation is for inserting static objects once, it should not be a problem.

    @param obj The object to be inserted in the octree.
    @return The node where the object ends up after insertion. In case of error, PGENULL is returned.
            If the given object is outside the bounds of this Octree Node, it is considered as an error.
*/
PureOctree* PureOctree::insertObject(const PureObject3D& obj)
{
    if ( (obj.getPosVec().getX() < vPos.getX() - fSize/2.f) || (obj.getPosVec().getX() > vPos.getX() + fSize/2.f) )
        return PGENULL;

    if ( (obj.getPosVec().getY() < vPos.getY() - fSize/2.f) || (obj.getPosVec().getY() > vPos.getY() + fSize/2.f) )
        return PGENULL;

    if ( (obj.getPosVec().getZ() < vPos.getZ() - fSize/2.f) || (obj.getPosVec().getZ() > vPos.getZ() + fSize/2.f) )
        return PGENULL;

    if ( getNodeType() == LeafEmpty )
    {   // simply store the object at this level
        vObjects.insert(&obj);
        return this;
    }
    
    if ( getNodeType() == Parent )
    {   // pass the object to next depth level
        const ChildIndex iChild = calculateIndex(obj.getPosVec());
        return vChildren[iChild]->insertObject(obj);
    }
    
    // getNodeType() == LeafContainer
    if ( nCurrentDepth == nMaxDepth )
    {
        // cannot subdivide more, so just simply store the object at this last level
        vObjects.insert(&obj);
        return this;
    }
    else
    {
        // subdivide this node now, depth level increases
        if ( !subdivide() )
        {
            return PGENULL;
        }
        for (TPureUInt i = 0; i < vChildren.size(); i++)
        {
            vChildren[i]->parent = this;
        }
        vChildren[TOP    | LEFT  | FRONT]->vPos.Set(vPos.getX() - fSize/4.f, vPos.getY() + fSize/4.f, vPos.getZ() - fSize/4.f);
        vChildren[BOTTOM | LEFT  | FRONT]->vPos.Set(vPos.getX() - fSize/4.f, vPos.getY() - fSize/4.f, vPos.getZ() - fSize/4.f);
        vChildren[TOP    | RIGHT | FRONT]->vPos.Set(vPos.getX() + fSize/4.f, vPos.getY() + fSize/4.f, vPos.getZ() - fSize/4.f);
        vChildren[BOTTOM | RIGHT | FRONT]->vPos.Set(vPos.getX() + fSize/4.f, vPos.getY() - fSize/4.f, vPos.getZ() - fSize/4.f);
        vChildren[TOP    | LEFT  | BACK ]->vPos.Set(vPos.getX() - fSize/4.f, vPos.getY() + fSize/4.f, vPos.getZ() + fSize/4.f);
        vChildren[BOTTOM | LEFT  | BACK ]->vPos.Set(vPos.getX() - fSize/4.f, vPos.getY() - fSize/4.f, vPos.getZ() + fSize/4.f);
        vChildren[TOP    | RIGHT | BACK ]->vPos.Set(vPos.getX() + fSize/4.f, vPos.getY() + fSize/4.f, vPos.getZ() + fSize/4.f);
        vChildren[BOTTOM | RIGHT | BACK ]->vPos.Set(vPos.getX() + fSize/4.f, vPos.getY() - fSize/4.f, vPos.getZ() + fSize/4.f);
        
        // add the already inserted object to one of the new child nodes, remove it from this node
        const PureObject3D* objAlreadyInTree = *(vObjects.begin());
        vObjects.clear();
        const ChildIndex iChildForObjAlreadyInTree = calculateIndex(objAlreadyInTree->getPosVec());
        vChildren[iChildForObjAlreadyInTree]->insertObject(*objAlreadyInTree);

        // add the new object also to one of the new child nodes
        const ChildIndex iChild = calculateIndex(obj.getPosVec());
        return vChildren[iChild]->insertObject(obj);
    }
} // insertObject()


/**
    Finds the given object in the octree.
    Note that the search will NOT involve the whole tree. The current position of the given object will be used
    to select the nodes to be searched in the tree. This means that if the position of the given object has changed
    since it was put in the tree, the search might fail.

    @param obj The object to be found.
    @return The octree node containing the object. If the object is not found, PGENULL is returned.
*/
const PureOctree* PureOctree::findObject(const PureObject3D& obj) const
{
    if ( getNodeType() == LeafEmpty )
    {
        return PGENULL;
    }
    else if ( getNodeType() == Parent )
    {
        // request the suitable children on next depth level to try find the object
        const ChildIndex iChild = calculateIndex(obj.getPosVec());
        return vChildren[iChild]->findObject(obj);
    }

    // getNodeType() == LeafContainer
    if ( vObjects.find(&obj) != vObjects.end() )
    {
        return this;
    }
    else
    {
        return PGENULL;
    }
} // findObject()


/**
    Gets the current depth level of the octree node.
    @return Depth level of the octree node. 0 means this is the root of the octree.
*/
TPureUInt PureOctree::getDepthLevel() const
{
    return nCurrentDepth;
}


/**
    Gets the maximum depth level of the octree node as it was specified in the constructor of the octree.
    This value will be the same for all nodes within the same octree.
    @return Maximum depth level of the octree node. 0 means there is no depth limit.
*/
TPureUInt PureOctree::getMaxDepthLevel() const
{
    return nMaxDepth;
}


/**
    Gets the type of the octree node which depends on if the node has any objects or children nodes.
    @return The type of the octree node. See explanation of values at PureOctree::NodeType.
*/
PureOctree::NodeType PureOctree::getNodeType() const
{
    if ( vChildren.size() > 0 )
    {
        return NodeType::Parent;
    }
    else if ( vObjects.size() > 0 )
    {
        return NodeType::LeafContainer;
    }

    return NodeType::LeafEmpty;
} // getNodeType()


/**
    Gets the world-space position of the node as specified in the constructor.
    This value will be the same for all nodes at the same depth level of the same octree.
    Since children nodes partition the parent node into equal size cubic volumes, the biggest node is the root node.
    The deeper we go in the tree i.e. the higher the depth level, the smaller the nodes are.

    @return The position of the node in world-space.
*/
const PureVector& PureOctree::getPos() const
{
    return vPos;
}

/**
    Sets the world-space position of this root node. 
    
    Valid only for the root node (level 0), and only if it is still empty.

    @param pos The new world-space position of this root node.

    @return True in case of success, false if invoked on non-root node or non-empty node.
*/
bool PureOctree::setPos(const PureVector& pos)
{
    if ((parent != PGENULL) || (getNodeType() != NodeType::LeafEmpty))
    {
        return false;
    }

    vPos = pos;

    return true;
}


/**
    Gets the length of the side of the cube represented by this node as it was specified in the constructor.
    This value will be the same for all nodes at the same depth level of the same octree.
    Since children nodes partition the parent node into equal size cubic volumes, the biggest node is the root node.
    The deeper we go in the tree i.e. the higher the depth level, the smaller the nodes are.

    @return The length of the side of the cube represented by this node.
*/
TPureFloat PureOctree::getSize() const
{
    return fSize;
}

/**
    Sets the length of the side of the cube represented by this root node. 
    
    Valid only for the root node (level 0), and only if it is still empty.

    @param size The new length of the side of the cube represented by this root node.

    @return True in case of success, false if invoked on non-root node or non-empty node.

*/
bool PureOctree::setSize(TPureFloat size)
{
    if ((parent != PGENULL) || (getNodeType() != NodeType::LeafEmpty))
    {
        return false;
    }

    fSize = size;

    return true;
}


/**
    Gets the children nodes of this node.
    An octree node has either 0 or exactly 8 children nodes.

    @return The children nodes of this node.
*/
const std::vector<PureOctree*>& PureOctree::getChildren() const
{
    return vChildren;
}


/**
    Gets the parent node of this node.
    The root node does not have parent.

    @return The parent node of this node.
*/
const PureOctree* PureOctree::getParent() const
{
    return parent;
}


/**
    Gets the stored objects of this node.

    @return The stored objects of this node.
*/
const std::set<const PureObject3D*>& PureOctree::getObjects() const
{
    return vObjects;
}

/**
    Removes all children and objects from this node.
    Objects are not destroyed, just get forgot by this tree.

    Valid only for the root node (level 0).
    Useful before rebuilding the tree.

    @return True in case of success, false if invoked on non-root node.
*/
bool PureOctree::reset()
{
    if (parent != PGENULL)
    {
        return false;
    }

    // i.e. nCurrentDepth is 0 here cause we are the root node

    DeleteChildren();
    vObjects.clear(); // just in case
    nodeType = getNodeType(); // expecting NodeType::LeafEmpty

    // objDebugBox at this root level should be kept if it exists, no need to update since its pos and size are fixed in ctor / by setSize().
    // BUT, there is no other way to destroy ALL dynamic resources of the octree, and if we dont do this, the ptr would become invalid
    // when the application is exiting, since Object3DManager will destroy all objects including this, and THEN octree dtor is invoked which
    // would then operate on invalid ptr. Therefore, we make sure this ptr is handled here, and expect application to invoke reset() before exiting.
    // This could be solved by shared_ptr but engine does not support.
    if (objDebugBox)
    {
        delete objDebugBox; // Object3D dtor triggers removing from ObjectManager too
        objDebugBox = nullptr;
    }

    return true;
}

/**
    Enables rendering wireframed boxes representing this node and its children.

    Note that the debug boxes are not automatically updated after inserting further objects into the tree.
    Can be invoked again if further updates to the tree happened.

    Due to implementation of reset(), the root node is not rendered after a call to reset(), before inserting at least 1 object again into the tree.
*/
void PureOctree::updateAndEnableNodeDebugRendering(
    PureObject3DManager& objmgr,
    PureColor colorWireframe /* copy-elision probably */)
{
    if (getNodeType() == LeafEmpty)
    {
        return;
    }

    if (!objDebugBox)
    {
        objDebugBox = objmgr.createBox(1.f, 1.f, 1.f);
        objDebugBox->SetWireframed(true);
        //objDebugBox->SetWireframedCulled(false);
        objDebugBox->getMaterial(false).getTextureEnvColor(0) = colorWireframe;
    }
    objDebugBox->getPosVec() = getPos();
    objDebugBox->SetScaling(getSize());

    if (getNodeType() == Parent)
    {
        for (auto pNode : vChildren)
        {
            assert(pNode);
            pNode->updateAndEnableNodeDebugRendering(objmgr, colorWireframe);
        }
    }
}

/**
    Disables rendering wireframed boxes representing this node and its children.
*/
void PureOctree::disableNodeDebugRendering()
{
    if (getNodeType() == LeafEmpty)
    {
        return;
    }

    if (objDebugBox)
    {
        objDebugBox->Hide();
    }

    if (getNodeType() == Parent)
    {
        for (auto pNode : vChildren)
        {
            assert(pNode);
            pNode->disableNodeDebugRendering();
        }
    }
}


// ############################## PROTECTED ##############################


TPureBool PureOctree::subdivide()
{
    try
    {
      for (TPureUInt i = 0; i < 8; i++)
      {
          PureOctree* const pChildNode = new PureOctree(PureVector(), fSize/2.f, nMaxDepth, nCurrentDepth + 1);
          vChildren.push_back(pChildNode);
      }
    }
    catch (const std::exception&)
    {
        for (TPureUInt i = 0; i < 8; i++)
        {
            if ( vChildren[i] != PGENULL )
            {
                delete vChildren[i];
            }
            vChildren.resize(0);
        }
        return false;
    }
    return true;
}


void PureOctree::DeleteChildren()
{
    if ( NodeType::Parent == getNodeType() )
    {
        for (TPureUInt i = 0; i < 8; i++)
        {
            vChildren[i]->DeleteChildren();
            delete vChildren[i];
        }
        vChildren.resize(0);
        // do not update node type here because it might become leaf_empty or leaf_container, higher level logic shall call getNodeType() to decide!
    }
}


// ############################### PRIVATE ###############################

