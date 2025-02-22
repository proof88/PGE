#pragma once

/*
    ###################################################################################
    PureBoundingVolumeHierarchy.h
    This file is part of PURE.
    Internal header.
    PureBoundingVolumeHierarchy class.
    Made by PR00F88
    ###################################################################################
*/


#include "../../../../PGEallHeaders.h"
#include "PureAxisAlignedBoundingBox.h"
#include "PureOctree.h"


/**
    BVH: Bounding Volume Hierarchy class.
    The bounding volumes of objects are put in a hierarchical tree.
    With the help of an octree, we can easily build this hierarchy:
    First insert the objects into the octree, traverse it from bottom to top: starting with the leaf nodes, we calculate the bounding boxes of the objects
    in the leaf nodes. Then we go 1 level above, and calculate the bounding volume for the node grouping all its children together.
    We recursively progressing up towards the root node, where the biggest bounding volume containing all previously calculated bounding volumes is created.

    This implementation is basically extending PureOctree nodes with bounding box calculation.

    Further material:
     - https://proof88.github.io/pure-doc/visibility.html
     - https://github.com/brandonpelfrey/Fast-BVH/tree/master/include
     - https://github.com/GrandPiaf/Raytracer/blob/master/Raytracer/AABB.h
     - https://github.com/taqu/BoundingVolumeHierarchy
*/
class PureBoundingVolumeHierarchy : public PureOctree
{
#ifdef PURE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureBoundingVolumeHierarchy is included")
#endif

public:

    // ---------------------------------------------------------------------------

    PureBoundingVolumeHierarchy(TPureUInt maxDepthLevel, TPureUInt currentDepthLevel);
    PureBoundingVolumeHierarchy(const PureVector& pos, TPureFloat size, TPureUInt maxDepthLevel, TPureUInt currentDepthLevel);
    virtual ~PureBoundingVolumeHierarchy();

    PureBoundingVolumeHierarchy(const PureBoundingVolumeHierarchy&) = delete;
    PureBoundingVolumeHierarchy& operator=(const PureBoundingVolumeHierarchy&) = delete;
    PureBoundingVolumeHierarchy(PureBoundingVolumeHierarchy&&) = delete;
    PureBoundingVolumeHierarchy& operator=(PureBoundingVolumeHierarchy&&) = delete;

    // yes, this is valid: overriding base method with a return type derived from the original return type: called as "covariant return type"
    virtual PureBoundingVolumeHierarchy* insertObject(const PureObject3D& obj) override;              /**< Inserts the given object in the octree. */
    virtual const PureBoundingVolumeHierarchy* findObject(const PureObject3D& obj) const override;    /**< Finds the given object in the octree. */
    virtual bool reset() override;                                                                    /**< Removes all children from this root node. */

    PureBoundingVolumeHierarchy* findObject(const PureObject3D& obj);                                 /**< Finds the given object in the octree. */

    const PureAxisAlignedBoundingBox& getAABB() const;              /**< Gets the AABB of this node. */

    PureBoundingVolumeHierarchy* findTightestFittingNode(const PureAxisAlignedBoundingBox& objAabb);
    PureBoundingVolumeHierarchy* findTightestFittingNode(const PureObject3D& obj);

    void updateAndEnableAabbDebugRendering(
        PureObject3DManager& objmgr,
        PureColor colorWireframe = PureColor(0, 255, 0, 255));      /**< Enables rendering wireframed boxes representing AABB of this node and its children. */
    void disableAabbDebugRendering();                               /**< Disables rendering wireframed boxes representing AABB of this node and its children. */
    
    PureObject3D* getDebugBox();     // RFR: shall not exist, but needed due to bad design around PureBoundingVolumeHierarchyRoot::markAabbDebugRendering()
    const PureObject3D* getDebugBox() const;

protected:

    PureObject3D* m_objDebugBox;

    // ---------------------------------------------------------------------------

    virtual TPureBool subdivide() override;

private:

    PureAxisAlignedBoundingBox m_aabb;

}; // class PureBoundingVolumeHierarchy


/**
    Root node for PureBoundingVolumeHierarchy.
    It is NOT mandatory to use it, however some functionality is implemented here only.
    These functionalities have effect on the overall tree, their helper variables are not needed in regular nodes.
*/
class PureBoundingVolumeHierarchyRoot : public PureBoundingVolumeHierarchy
{
#ifdef PURE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureBoundingVolumeHierarchyRoot is included")
#endif

public:

    // ---------------------------------------------------------------------------

    PureBoundingVolumeHierarchyRoot(TPureUInt maxDepthLevel, TPureUInt currentDepthLevel);
    PureBoundingVolumeHierarchyRoot(const PureVector& pos, TPureFloat size, TPureUInt maxDepthLevel, TPureUInt currentDepthLevel);
    virtual ~PureBoundingVolumeHierarchyRoot();

    virtual bool reset() override;                                  /**< Removes all children from this root node. */

    void markAabbDebugRendering(
        PureBoundingVolumeHierarchy* nodeToBeMarked,
        PureColor color = PureColor(255, 0, 0, 100));  /**< Marks the specified node in debug rendering i.e. its rendering will be different compared to other AABB debug boxes. */

private:

    PureColor m_colorPrevDebugBoxBeforeMarking;
    PureObject3D* m_objPrevMarkedDebugBox;

}; // class PureBoundingVolumeHierarchyRoot
