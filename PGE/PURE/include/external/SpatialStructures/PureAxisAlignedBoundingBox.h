#pragma once

/*
    ###################################################################################
    PureAxisAlignedBoundingBox.h
    This file is part of PURE.
    Internal header.
    PureAxisAlignedBoundingBox class.
    Made by PR00F88
    ###################################################################################
*/


#include "../../../../PGEallHeaders.h"
#include "../Math/PureVector.h"


/**
    AABB: Axis-Aligned Bounding Box class.

    file:///D:/__PR00F__/___developing___/projects/PGE/PGE/Docs/PURE/html/visibility.html
    https://github.com/brandonpelfrey/Fast-BVH/tree/master/include
    https://github.com/GrandPiaf/Raytracer/blob/master/Raytracer/AABB.h
    https://github.com/taqu/BoundingVolumeHierarchy
*/
class PureAxisAlignedBoundingBox
{
#ifdef PURE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureAxisAlignedBoundingBox is included")
#endif

public:

    // ---------------------------------------------------------------------------

    PureAxisAlignedBoundingBox();
    PureAxisAlignedBoundingBox(const PureVector& pos_, const PureVector& size_);
    virtual ~PureAxisAlignedBoundingBox();

    const PureVector& getPosVec() const;                      /**< Gets the world-space position which is the center of the bounding box. */
    const PureVector& getSizeVec() const;                     /**< Gets the base sizes. */

    TPureBool isInside(const PureVector& p) const;                     /**< Is the given point inside the bounding box? */
    TPureBool isInside(const PureAxisAlignedBoundingBox& aabb) const;  /**< Is the given box inside the bounding box? */
    void ExtendBy(const PureVector& p);                                /**< Extends the size of the bounding box so the given point will be inside the box. */
    void ExtendBy(const PureAxisAlignedBoundingBox& aabb);             /**< Extends the size of the bounding box so the given box will be inside the box. */

protected:

    // ---------------------------------------------------------------------------
    
    // allow the defaults to be generated
    /*PureAxisAlignedBoundingBox(const PureAxisAlignedBoundingBox&);
    PureAxisAlignedBoundingBox& operator=(const PureAxisAlignedBoundingBox&);*/

private:

    PureVector pos;
    PureVector size;

}; // class PureAxisAlignedBoundingBox 

