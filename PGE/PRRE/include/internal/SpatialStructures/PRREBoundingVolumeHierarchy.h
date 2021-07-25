#pragma once

/*
    ###################################################################################
    PRREBoundingVolumeHierarchy.h
    This file is part of PRRE.
    Internal header.
    PRREBoundingVolumeHierarchy class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/


#include "../../../../PGEallHeaders.h"
#include "../../internal/SpatialStructures/PRREAxisAlignedBoundingBox.h"
#include "../../internal/SpatialStructures/PRREOctree.h"


/**
    BVH: Bounding Volume Hierarchy class.
    The bounding volumes of objects are put in a hierarchical tree.
    With the help of an octree, we can easily build this hierarchy:
    First insert the objects into the octree, traverse it from bottom to top: starting with the leaf nodes, we calculate the bounding boxes of the objects
    in the leaf nodes. Then we go 1 level above, and calculate the bounding volume for the node grouping all its children together.
    We recursively progressing up towards the root node, where the biggest bounding volume containing all previously calculated bounding volumes is created.

    This implementation is basically extending PRREOctree nodes with bounding box calculation.
*/
class PRREBoundingVolumeHierarchy : public PRREOctree
{
#ifdef PRRE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PRREBoundingVolumeHierarchy is included")
#endif

public:

    // ---------------------------------------------------------------------------

    PRREBoundingVolumeHierarchy(const PRREVector& pos, TPRREfloat size, TPRREuint maxDepthLevel, TPRREuint currentDepthLevel);
    virtual ~PRREBoundingVolumeHierarchy();

    // yes, this is valid: overriding base method with a return type derived from the original return type: called as "covariant return type"
    virtual PRREBoundingVolumeHierarchy* insertObject(const PRREObject3D& obj);              /**< Inserts the given object in the octree. */
    virtual const PRREBoundingVolumeHierarchy* findObject(const PRREObject3D& obj) const;    /**< Finds the given object in the octree. */

    const PRREAxisAlignedBoundingBox& getAABB() const;                                       /**< Gets the AABB of this node. */

protected:

    // ---------------------------------------------------------------------------
       
    PRREBoundingVolumeHierarchy();
    
    // defaults will be fine
    //PRREBoundingVolumeHierarchy(const PRREBoundingVolumeHierarchy&);
    //PRREBoundingVolumeHierarchy& operator=(const PRREBoundingVolumeHierarchy&);

    virtual void Subdivide(); // override

private:

    PRREAxisAlignedBoundingBox aabb;


}; // class PRREBoundingVolumeHierarchy 

