#pragma once

/*
    ###################################################################################
    PureBoundingVolumeHierarchy.h
    This file is part of PURE.
    Internal header.
    PureBoundingVolumeHierarchy class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/


#include "../../../../PGEallHeaders.h"
#include "../../internal/SpatialStructures/PureAxisAlignedBoundingBox.h"
#include "../../internal/SpatialStructures/PureOctree.h"


/**
    BVH: Bounding Volume Hierarchy class.
    The bounding volumes of objects are put in a hierarchical tree.
    With the help of an octree, we can easily build this hierarchy:
    First insert the objects into the octree, traverse it from bottom to top: starting with the leaf nodes, we calculate the bounding boxes of the objects
    in the leaf nodes. Then we go 1 level above, and calculate the bounding volume for the node grouping all its children together.
    We recursively progressing up towards the root node, where the biggest bounding volume containing all previously calculated bounding volumes is created.

    This implementation is basically extending PureOctree nodes with bounding box calculation.
*/
class PureBoundingVolumeHierarchy : public PureOctree
{
#ifdef Pure_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureBoundingVolumeHierarchy is included")
#endif

public:

    // ---------------------------------------------------------------------------

    PureBoundingVolumeHierarchy(const PureVector& pos, TPurefloat size, TPureuint maxDepthLevel, TPureuint currentDepthLevel);
    virtual ~PureBoundingVolumeHierarchy();

    // yes, this is valid: overriding base method with a return type derived from the original return type: called as "covariant return type"
    virtual PureBoundingVolumeHierarchy* insertObject(const PureObject3D& obj);              /**< Inserts the given object in the octree. */
    virtual const PureBoundingVolumeHierarchy* findObject(const PureObject3D& obj) const;    /**< Finds the given object in the octree. */

    const PureAxisAlignedBoundingBox& getAABB() const;                                       /**< Gets the AABB of this node. */

protected:

    // ---------------------------------------------------------------------------
       
    PureBoundingVolumeHierarchy();
    
    PureBoundingVolumeHierarchy(const PureBoundingVolumeHierarchy&);
    PureBoundingVolumeHierarchy& operator=(const PureBoundingVolumeHierarchy&);

    virtual TPurebool subdivide(); // override

private:

    PureAxisAlignedBoundingBox aabb;


}; // class PureBoundingVolumeHierarchy 

