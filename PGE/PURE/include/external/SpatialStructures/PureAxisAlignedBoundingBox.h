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
*/
class PureAxisAlignedBoundingBox
{
#ifdef PURE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureAxisAlignedBoundingBox is included")
#endif

public:

    // ---------------------------------------------------------------------------

    PureAxisAlignedBoundingBox() = default;
    PureAxisAlignedBoundingBox(const PureVector& pos_, const PureVector& size_);

    ~PureAxisAlignedBoundingBox() = default;

    PureAxisAlignedBoundingBox(const PureAxisAlignedBoundingBox&) = default;
    PureAxisAlignedBoundingBox& operator=(const PureAxisAlignedBoundingBox&) = default;
    PureAxisAlignedBoundingBox(PureAxisAlignedBoundingBox&&) = default;
    PureAxisAlignedBoundingBox& operator=(PureAxisAlignedBoundingBox&&) = default;

    const bool isInitialized() const;

    const PureVector& getPosVec() const;                      /**< Gets the world-space position which is the center of the bounding box. */
    const PureVector& getSizeVec() const;                     /**< Gets the base sizes. */

    TPureBool isInside(const PureVector& p) const;                       /**< Is the given point inside the bounding box? */
    TPureBool isInside(const PureAxisAlignedBoundingBox& aabb) const;    /**< Is the given box fully inside the bounding box? */
    TPureBool intersects(const PureAxisAlignedBoundingBox& aabb) const;  /**< Is the given box intersecting (partially inside) the bounding box? */
    void ExtendBy(const PureVector& p);                                  /**< Extends the size of the bounding box so the given point will be inside the box. */
    void ExtendBy(const PureAxisAlignedBoundingBox& aabb);               /**< Extends the size of the bounding box so the given box will be fully inside the box. */

private:

    PureVector pos;
    PureVector size;

}; // class PureAxisAlignedBoundingBox 

