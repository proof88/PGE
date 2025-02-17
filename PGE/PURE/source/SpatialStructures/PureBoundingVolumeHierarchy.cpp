#include "PureBoundingVolumeHierarchy.h"
/*
    ###################################################################################
    PureBoundingVolumeHierarchy.cpp
    This file is part of PURE.
    PureBoundingVolumeHierarchy class.
    Made by PR00F88
    ###################################################################################
*/

#include "PureBaseIncludes.h"  // PCH
#include "../../include/external/SpatialStructures/PureBoundingVolumeHierarchy.h"

#include <cassert>

#include "../../../../../PFL/PFL/PFL.h"

using namespace std;

/*
   PureBoundingVolumeHierarchy
   ###########################################################################
*/


// ############################### PUBLIC ################################


/**
    Creates an octree-derived bounding volume hierarchy (BVH) node.
    Basically this is doing the same octree initialization as PureOctree's public constructor with the same arguments.

    @param maxDepthLevel     The maximum node depth level supported by this BVH. 0 means there is no depth limit.
    @param currentDepthLevel The depth level of this specific node being created. For global use, just specify 0 so this will be the root node of the BVH.
*/
PureBoundingVolumeHierarchy::PureBoundingVolumeHierarchy(TPureUInt maxDepthLevel, TPureUInt currentDepthLevel) :
    PureOctree(maxDepthLevel, currentDepthLevel),
    objDebugBox(nullptr)
{
}

/**
    Creates an octree-derived bounding volume hierarchy (BVH) node.
    Basically this is doing the same octree initialization as PureOctree's public constructor with the same arguments.

    @param pos  The world-space position of this node. For global use, you can specify just (0;0;0).
    @param size The length of the side of the cube represented by this octree-derived node. Recommend this to be big enough to contain any scene objects.
                Note that this is NOT the size of the bounding box used by the BVH, that will be calculated.
    @param maxDepthLevel     The maximum node depth level supported by this BVH. 0 means there is no depth limit.
    @param currentDepthLevel The depth level of this specific node being created. For global use, just specify 0 so this will be the root node of the BVH.
*/
PureBoundingVolumeHierarchy::PureBoundingVolumeHierarchy(const PureVector& pos, TPureFloat size, TPureUInt maxDepthLevel, TPureUInt currentDepthLevel) :
    PureOctree(pos, size, maxDepthLevel, currentDepthLevel),
    objDebugBox(nullptr)
{
    // TODO: AABB update with pos & size ??? Well, in 2023 I explicitly stated in the API comment for the size argument above that it is NOT the size of the AABB, 
    // so most probably I dont need to update AABB here.
} // PureBoundingVolumeHierarchy(...)


PureBoundingVolumeHierarchy::~PureBoundingVolumeHierarchy()
{
    DeleteChildren();
    if (objDebugBox)
    {
        delete objDebugBox; // Object3D dtor triggers removing from ObjectManager too
    }
} // ~PureBoundingVolumeHierarchy()


/**
    Same behavior as PureOctree::insertObject().

    @param obj The object to be inserted in the octree.
    @return The node where the object ends up after insertion. In case of error, PGENULL is returned.
            If the given object is outside the bounds of this Octree Node, it is considered as an error.
*/
PureBoundingVolumeHierarchy* PureBoundingVolumeHierarchy::insertObject(const PureObject3D& obj)
{
    // Note 1:
    // most of the time dynamic_cast should be used for downcasting, however in this specific case,
    // we can guarantee that the returned Octree node is actually a derived PureBoundingVolumeHierarchy node,
    // since we are in PureBoundingVolumeHierarchy method. So we can use static_cast for faster cast.

    // Note 2: although we call ancestor's insertObject(), due to polymorphism it will still call our
    // derived Subdivide() method, so children will be PureBoundingVolumeHierarchy instances!
    PureBoundingVolumeHierarchy* node = static_cast<PureBoundingVolumeHierarchy*>(PureOctree::insertObject(obj));
    if ( PGENULL == node )
    {
        return PGENULL;
    }

    const PureAxisAlignedBoundingBox obj_bbox(obj.getPosVec(), obj.getScaledSizeVec());
    node->aabb.ExtendBy(obj_bbox);

    PureBoundingVolumeHierarchy* prevChild = node;
    PureBoundingVolumeHierarchy* p = static_cast<PureBoundingVolumeHierarchy*>(node->parent);
    while (p != PGENULL)
    {
        p->aabb.ExtendBy(prevChild->aabb);
        prevChild = p;
        p = static_cast<PureBoundingVolumeHierarchy*>(p->parent);
    }

    return node;
} // insertObject()


/**
    Same behavior as PureOctree::findObject().
    @param obj The object to be found.
    @return The BVH node containing the object. If the object is not found, PGENULL is returned.
*/
const PureBoundingVolumeHierarchy* PureBoundingVolumeHierarchy::findObject(const PureObject3D& obj) const
{
    const PureOctree* node = PureOctree::findObject(obj);
    if ( node )
    {
        // most of the time dynamic_cast should be used for downcasting, however in this specific case,
        // we can guarantee that the found Octree node is actually a derived PureBoundingVolumeHierarchy node,
        // since we are in PureBoundingVolumeHierarchy method. So we can use static_cast for faster cast.
        return static_cast<const PureBoundingVolumeHierarchy*>(node);
    }

    return PGENULL;
} // findObject()


/**
    Extending PureOctree::reset() behavior by resetting the AABB also.

    @return True in case of success, false if invoked on non-root node.
*/
bool PureBoundingVolumeHierarchy::reset()
{
    if (PureOctree::reset())
    {
        aabb = PureAxisAlignedBoundingBox();
        if (objDebugBox)
        {
            delete objDebugBox; // Object3D dtor triggers removing from ObjectManager too
            objDebugBox = nullptr;
        }
        return true;
    }

    return false;
}


/**
    Gets the AABB of this node.
    Note that the AABB has nothing to do with the position and size of the ancestor Octree node.
    Position and size of Octree node is just for determining which node should be the storage for an inserted object.
    However, the AABB is calculated purely by the position and size of the inserted objects, thus its position and size
    can easily differ from the position and size of the Octree node.

    @return The calculated AABB volume for this node.
*/
const PureAxisAlignedBoundingBox& PureBoundingVolumeHierarchy::getAABB() const
{
    return aabb;
}

/**
    Enables rendering wireframed boxes representing AABB of this node.
    Note that the debug boxes are not automatically updated after inserting further objects into the tree.
    Can be invoked again if further updates to the tree happened.
*/
void PureBoundingVolumeHierarchy::updateAndEnableAabbDebugRendering(
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
    objDebugBox->getPosVec() = aabb.getPosVec();
    objDebugBox->SetScaling(aabb.getSizeVec());

    if (getNodeType() == Parent)
    {
        for (auto pNode : vChildren)
        {
            assert(pNode);
            PureBoundingVolumeHierarchy& bvhNode = static_cast<PureBoundingVolumeHierarchy&>(*pNode);
            bvhNode.updateAndEnableAabbDebugRendering(objmgr, colorWireframe);
        }
    }
}

/**
    Disables rendering wireframed boxes representing AABB of this node.
*/
void PureBoundingVolumeHierarchy::disableAabbDebugRendering()
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
            PureBoundingVolumeHierarchy& bvhNode = static_cast<PureBoundingVolumeHierarchy&>(*pNode);
            bvhNode.disableAabbDebugRendering();
        }
    }
}


// ############################## PROTECTED ##############################


TPureBool PureBoundingVolumeHierarchy::subdivide()
{
    assert(vChildren.size() == 0);
    try
    {
      for (TPureUInt i = 0; i < 8; i++)
      {
          PureBoundingVolumeHierarchy* const pChildNode = new PureBoundingVolumeHierarchy(PureVector(), getSize()/2.f, getMaxDepthLevel(), getDepthLevel() + 1);
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


// ############################### PRIVATE ###############################