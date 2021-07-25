#pragma once

/*
    ###################################################################################
    PRREAxisAlignedBoundingBox.h
    This file is part of PRRE.
    Internal header.
    PRREAxisAlignedBoundingBox class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/


#include "../../../../PGEallHeaders.h"
#include "../../external/Math/PRREVector.h"


/**
    AABB: Axis-Aligned Bounding Box class.

    file:///D:/__PR00F__/___developing___/projects/PGE/PGE/Docs/PURE/html/visibility.html
    https://github.com/brandonpelfrey/Fast-BVH/tree/master/include
    https://github.com/GrandPiaf/Raytracer/blob/master/Raytracer/AABB.h
    https://github.com/taqu/BoundingVolumeHierarchy
*/
class PRREAxisAlignedBoundingBox
{
#ifdef PRRE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PRREAxisAlignedBoundingBox is included")
#endif

public:

    // ---------------------------------------------------------------------------

    PRREAxisAlignedBoundingBox();
    PRREAxisAlignedBoundingBox(const PRREVector& pos_, const PRREVector& size_);
    virtual ~PRREAxisAlignedBoundingBox();

    const PRREVector& getPosVec() const;                      /**< Gets the world-space position which is the center of the bounding box. */
    const PRREVector& getSizeVec() const;                     /**< Gets the base sizes. */

    TPRREbool isInside(const PRREVector& p) const;                     /**< Is the given point inside the bounding box? */
    TPRREbool isInside(const PRREAxisAlignedBoundingBox& aabb) const;  /**< Is the given box inside the bounding box? */
    void ExtendBy(const PRREVector& p);                                /**< Extends the size of the bounding box so the given point will be inside the box. */
    void ExtendBy(const PRREAxisAlignedBoundingBox& aabb);             /**< Extends the size of the bounding box so the given box will be inside the box. */

protected:

    // ---------------------------------------------------------------------------
    
    // allow the defaults to be generated
    /*PRREAxisAlignedBoundingBox(const PRREAxisAlignedBoundingBox&);
    PRREAxisAlignedBoundingBox& operator=(const PRREAxisAlignedBoundingBox&);*/

private:

    PRREVector pos;
    PRREVector size;

}; // class PRREAxisAlignedBoundingBox 

