/*
    ###################################################################################
    PRREOctree.cpp
    This file is part of PRRE.
    PRREOctree class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREbaseIncludes.h"  // PCH
#include "../../include/internal/SpatialStructures/PRREOctree.h"
#include "../../../../../PFL/PFL/PFL.h"

using namespace std;

/*
   PRREOctree
   ###########################################################################
*/


// ############################### PUBLIC ################################


const PRREOctree::ChildIndex PRREOctree::BIT_AXIS_Y = 0;
const PRREOctree::ChildIndex PRREOctree::BIT_AXIS_X = 1;
const PRREOctree::ChildIndex PRREOctree::BIT_AXIS_Z = 2;

const PRREOctree::ChildIndex PRREOctree::TOP    = 0;
const PRREOctree::ChildIndex PRREOctree::BOTTOM = BIT(PRREOctree::BIT_AXIS_Y);
const PRREOctree::ChildIndex PRREOctree::LEFT   = 0;
const PRREOctree::ChildIndex PRREOctree::RIGHT  = BIT(PRREOctree::BIT_AXIS_X);
const PRREOctree::ChildIndex PRREOctree::FRONT  = 0;
const PRREOctree::ChildIndex PRREOctree::BACK   = BIT(PRREOctree::BIT_AXIS_Z);


/**
    Creates an octree node.
    Initially the type of this node will be LeafEmpty.
    @param pos The world-space position of this node. For global use, you can specify just (0;0;0).
    @param size The length of the side of the cube represented by this node. Recommend this to be big enough to contain any scene objects.
    @param maxDepthLevel The maximum node depth level supported by this octree. 0 means there is no depth limit.
    @param currentDepthLevel The depth level of this specific node being created. For global use, just specify 0 so this will be the root node of the octree.
*/
PRREOctree::PRREOctree(const PRREVector& pos, TPRREfloat size, TPRREuint maxDepthLevel, TPRREuint currentDepthLevel) :
    vPos(pos),
    fSize(size),
    nMaxDepth(maxDepthLevel),
    nCurrentDepth(currentDepthLevel),
    nodeType(NodeType::LeafEmpty)
{

} // PRREOctree(...)


PRREOctree::~PRREOctree()
{

} // ~PRREOctree()


/**
    Calculates child node index for the given world-space position in the current node.
    Bounds checking is NOT done, so giving a position which is outside the bounds of this Octree Node will not be detected.

    @param pos The world-space position for which child index should be selected in the current node.
    @return Calculated child index. If given position is same as Octree Node position, index will be TopRightBack.
*/
PRREOctree::ChildIndex PRREOctree::calculateIndex(const PRREVector& pos) const
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
}


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
PRREOctree* PRREOctree::insertObject(const PRREObject3D& obj)
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
    else if ( getNodeType() == Parent )
    {   // pass the object to next depth level
        const ChildIndex iChild = calculateIndex(obj.getPosVec());
        return vChildren[iChild].insertObject(obj);
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
        PRREOctree referenceChildNode(PRREVector(), fSize/2.f, nMaxDepth, nCurrentDepth + 1);
        vChildren.resize(8, referenceChildNode);
        vChildren[TOP    | LEFT  | FRONT].vPos.Set(vPos.getX() - fSize/4.f, vPos.getY() + fSize/4.f, vPos.getZ() - fSize/4.f);
        vChildren[BOTTOM | LEFT  | FRONT].vPos.Set(vPos.getX() - fSize/4.f, vPos.getY() - fSize/4.f, vPos.getZ() - fSize/4.f);
        vChildren[TOP    | RIGHT | FRONT].vPos.Set(vPos.getX() + fSize/4.f, vPos.getY() + fSize/4.f, vPos.getZ() - fSize/4.f);
        vChildren[BOTTOM | RIGHT | FRONT].vPos.Set(vPos.getX() + fSize/4.f, vPos.getY() - fSize/4.f, vPos.getZ() - fSize/4.f);
        vChildren[TOP    | LEFT  | BACK ].vPos.Set(vPos.getX() - fSize/4.f, vPos.getY() + fSize/4.f, vPos.getZ() + fSize/4.f);
        vChildren[BOTTOM | LEFT  | BACK ].vPos.Set(vPos.getX() - fSize/4.f, vPos.getY() - fSize/4.f, vPos.getZ() + fSize/4.f);
        vChildren[TOP    | RIGHT | BACK ].vPos.Set(vPos.getX() + fSize/4.f, vPos.getY() + fSize/4.f, vPos.getZ() + fSize/4.f);
        vChildren[BOTTOM | RIGHT | BACK ].vPos.Set(vPos.getX() + fSize/4.f, vPos.getY() - fSize/4.f, vPos.getZ() + fSize/4.f);
        
        // add the already inserted object to one of the new child nodes, remove it from this node
        const PRREObject3D* objAlreadyInTree = *(vObjects.begin());
        vObjects.clear();
        const ChildIndex iChildForObjAlreadyInTree = calculateIndex(objAlreadyInTree->getPosVec());
        vChildren[iChildForObjAlreadyInTree].insertObject(*objAlreadyInTree);

        // add the new object also to one of the new child nodes
        const ChildIndex iChild = calculateIndex(obj.getPosVec());
        return vChildren[iChild].insertObject(obj);
    }
}


/**
    Finds the given object in the octree.
    @param obj The object to be found.
    @return The octree node containing the object. If the object is not found, PGENULL is returned.
*/
const PRREOctree* PRREOctree::findObject(const PRREObject3D& obj) const
{
    if ( getNodeType() == LeafEmpty )
    {
        return PGENULL;
    }
    else if ( getNodeType() == Parent )
    {
        // request the suitable children on next depth level to try find the object
        const ChildIndex iChild = calculateIndex(obj.getPosVec());
        return vChildren[iChild].findObject(obj);
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
}


/**
    Gets the current depth level of the octree node.
    @return Depth level of the octree node. 0 means this is the root of the octree.
*/
TPRREuint PRREOctree::getDepthLevel() const
{
    return nCurrentDepth;
}


/**
    Gets the maximum depth level of the octree node as it was specified in the constructor of the octree.
    This value will be the same for all nodes within the same octree.
    @return Maximum depth level of the octree node. 0 means there is no depth limit.
*/
TPRREuint PRREOctree::getMaxDepthLevel() const
{
    return nMaxDepth;
}


/**
    Gets the type of the octree node which depends on if the node has any objects or children nodes.
    @return The type of the octree node. See explanation of values at PRREOctree::NodeType.
*/
PRREOctree::NodeType PRREOctree::getNodeType() const
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
}


/**
    Gets the world-space position of the node as specified in the constructor.
    This value will be the same for all nodes at the same depth level of the same octree.
    Since children nodes partition the parent node into equal size cubic volumes, the biggest node is the root node.
    The deeper we go in the tree i.e. the higher the depth level, the smaller the nodes are.

    @return The position of the node in world-space.
*/
const PRREVector& PRREOctree::getPos() const
{
    return vPos;
}


/**
    Gets the length of the side of the cube represented by this node as it was specified in the constructor.
    This value will be the same for all nodes at the same depth level of the same octree.
    Since children nodes partition the parent node into equal size cubic volumes, the biggest node is the root node.
    The deeper we go in the tree i.e. the higher the depth level, the smaller the nodes are.

    @return The length of the side of the cube represented by this node.
*/
TPRREfloat PRREOctree::getSize() const
{
    return fSize;
}


/**
    Gets the children nodes of this node.
    An octree node has either 0 or exactly 8 children nodes.

    @return The children nodes of this node.
*/
const std::vector<PRREOctree>& PRREOctree::getChildren() const
{
    return vChildren;
}


/**
    Gets the stored objects of this node.

    @return The stored objects of this node.
*/
const std::set<const PRREObject3D*>& PRREOctree::getObjects() const
{
    return vObjects;
}


// ############################## PROTECTED ##############################


PRREOctree::PRREOctree()
{

} // PRREOctree()


// ############################### PRIVATE ###############################

