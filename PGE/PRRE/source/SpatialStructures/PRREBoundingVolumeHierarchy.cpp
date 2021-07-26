/*
    ###################################################################################
    PRREBoundingVolumeHierarchy.cpp
    This file is part of PRRE.
    PRREBoundingVolumeHierarchy class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREbaseIncludes.h"  // PCH
#include "../../include/internal/SpatialStructures/PRREBoundingVolumeHierarchy.h"
#include "../../../../../PFL/PFL/PFL.h"

using namespace std;

/*
   PRREBoundingVolumeHierarchy
   ###########################################################################
*/


// ############################### PUBLIC ################################


/**
    Creates an octree-derived bounding volume hierarchy (BVH) node.
    Basically this is doing the same octree initialization as PRREOctree's public constructor.
    @param pos The world-space position of this node. For global use, you can specify just (0;0;0).
    @param size The length of the side of the cube represented by this octree-derived node. Recommend this to be big enough to contain any scene objects.
           Note that this is NOT the size of the bounding box used by the BVH, that will be calculated.
    @param maxDepthLevel The maximum node depth level supported by this BVH. 0 means there is no depth limit.
    @param currentDepthLevel The depth level of this specific node being created. For global use, just specify 0 so this will be the root node of the BVH.
*/
PRREBoundingVolumeHierarchy::PRREBoundingVolumeHierarchy(const PRREVector& pos, TPRREfloat size, TPRREuint maxDepthLevel, TPRREuint currentDepthLevel) :
    PRREOctree(pos, size, maxDepthLevel, currentDepthLevel)
{

} // PRREBoundingVolumeHierarchy(...)


PRREBoundingVolumeHierarchy::~PRREBoundingVolumeHierarchy()
{
    DeleteChildren();
} // ~PRREBoundingVolumeHierarchy()


/**
    Same behavior as PRREOctree::insertObject().

    @param obj The object to be inserted in the octree.
    @return The node where the object ends up after insertion. In case of error, PGENULL is returned.
            If the given object is outside the bounds of this Octree Node, it is considered as an error.
*/
PRREBoundingVolumeHierarchy* PRREBoundingVolumeHierarchy::insertObject(const PRREObject3D& obj)
{
    // Note 1:
    // most of the time dynamic_cast should be used for downcasting, however in this specific case,
    // we can guarantee that the returned Octree node is actually a derived PRREBoundingVolumeHierarchy node,
    // since we are in PRREBoundingVolumeHierarchy method. So we can use static_cast for faster cast.

    // Note 2: although we call ancestor's insertObject(), due to polymorphism it will still call our
    // derived Subdivide() method, so children will be PRREBoundingVolumeHierarchy instances!
    PRREBoundingVolumeHierarchy* node = static_cast<PRREBoundingVolumeHierarchy*>(PRREOctree::insertObject(obj));
    if ( PGENULL == node )
    {
        return PGENULL;
    }

    const PRREAxisAlignedBoundingBox obj_bbox(obj.getPosVec(), obj.getScaledSizeVec());
    node->aabb.ExtendBy(obj_bbox);

    PRREBoundingVolumeHierarchy* prevChild = node;
    PRREBoundingVolumeHierarchy* p = static_cast<PRREBoundingVolumeHierarchy*>(node->parent);
    while (p != PGENULL)
    {
        p->aabb.ExtendBy(prevChild->aabb);
        prevChild = p;
        p = static_cast<PRREBoundingVolumeHierarchy*>(p->parent);
    }

    return node;
} // insertObject()


/**
    Same behavior as PRREOctree::findObject().
    @param obj The object to be found.
    @return The BVH node containing the object. If the object is not found, PGENULL is returned.
*/
const PRREBoundingVolumeHierarchy* PRREBoundingVolumeHierarchy::findObject(const PRREObject3D& obj) const
{
    const PRREOctree* node = PRREOctree::findObject(obj);
    if ( node )
    {
        // most of the time dynamic_cast should be used for downcasting, however in this specific case,
        // we can guarantee that the found Octree node is actually a derived PRREBoundingVolumeHierarchy node,
        // since we are in PRREBoundingVolumeHierarchy method. So we can use static_cast for faster cast.
        return static_cast<const PRREBoundingVolumeHierarchy*>(node);
    }

    return PGENULL;
} // findObject()


/**
    Gets the AABB of this node.
    Note that the AABB has nothing to do with the position and size of the ancestor Octree node.
    Position and size of Octree node is just for determining which node should be the storage for an inserted object.
    However, the AABB is calculated purely by the position and size of the inserted objects, thus its position and size
    can easily differ from the position and size of the Octree node.

    @return The calculated AABB volume for this node.
*/
const PRREAxisAlignedBoundingBox& PRREBoundingVolumeHierarchy::getAABB() const
{
    return aabb;
}


// ############################## PROTECTED ##############################


PRREBoundingVolumeHierarchy::PRREBoundingVolumeHierarchy()
{

} // PRREBoundingVolumeHierarchy()


PRREBoundingVolumeHierarchy::PRREBoundingVolumeHierarchy(const PRREBoundingVolumeHierarchy&)
{
}


PRREBoundingVolumeHierarchy& PRREBoundingVolumeHierarchy::operator=(const PRREBoundingVolumeHierarchy&)
{
    return *this;
}


void PRREBoundingVolumeHierarchy::Subdivide()
{
    for (TPRREuint i = 0; i < 8; i++)
    {
        PRREBoundingVolumeHierarchy* const pChildNode = new PRREBoundingVolumeHierarchy(PRREVector(), getSize()/2.f, getMaxDepthLevel(), getDepthLevel() + 1);
        vChildren.push_back(pChildNode);
    }
}


// ############################### PRIVATE ###############################