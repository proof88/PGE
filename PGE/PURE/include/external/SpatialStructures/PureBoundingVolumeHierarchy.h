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

    With this implementation, only the AABB of the BVH nodes are being updated over time with each insertion, i.e. their sizes might change to be able to
    fully bound all their children and contained objects, and this implies that their position also changes with each size updates.
    BUT, size and position of their ancestor Octree nodes DO NOT change over time at all.

    Note that there are also other ways to build up a BVH, for example here binary tree is used:
    https://www.haroldserrano.com/blog/visualizing-the-boundary-volume-hierarchy-collision-algorithm

    Also, there are way more ways to optimize, for example, exploiting spatial coherence: most moving objects will be at the same OR similar position in the
    next frame as in the current frame.

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
    virtual bool reset() override;

    PureBoundingVolumeHierarchy* findObject(const PureObject3D& obj);                                 /**< Finds the given object in the octree. */

    const PureAxisAlignedBoundingBox& getAABB() const;              /**< Gets the AABB of this node. */

    const PureBoundingVolumeHierarchy* findLowestLevelFittingNode(const PureAxisAlignedBoundingBox& objAabb) const;
    const PureBoundingVolumeHierarchy* findLowestLevelFittingNode(const PureObject3D& obj) const;

    PureBoundingVolumeHierarchy* findLowestLevelFittingNode(const PureAxisAlignedBoundingBox& objAabb);
    PureBoundingVolumeHierarchy* findLowestLevelFittingNode(const PureObject3D& obj);

    const PureObject3D* findOneColliderObject_startFromLowestLevelFittingNode(
        const PureAxisAlignedBoundingBox& objAabb,
        const PureBoundingVolumeHierarchy* pStartNode) const;
    const PureObject3D* findOneColliderObject_startFromLowestLevelFittingNode(const PureObject3D& obj) const;

    const PureObject3D* findOneColliderObject_startFromFirstNode(
        const PureAxisAlignedBoundingBox& objAabb,
        const PureBoundingVolumeHierarchy* pStartNode) const;
    const PureObject3D* findOneColliderObject_startFromFirstNode(const PureObject3D& obj) const;

    bool findAllColliderObjects_startFromLowestLevelFittingNode(
        const PureAxisAlignedBoundingBox& objAabb,
        const PureBoundingVolumeHierarchy* pStartNode,
        std::vector<const PureObject3D*>& colliders) const;
    bool findAllColliderObjects_startFromLowestLevelFittingNode(
        const PureObject3D& obj,
        std::vector<const PureObject3D*>& colliders) const;

    bool findAllColliderObjects_startFromFirstNode(
        const PureAxisAlignedBoundingBox& objAabb,
        const PureBoundingVolumeHierarchy* pStartNode,
        std::vector<const PureObject3D*>& colliders) const;
    bool findAllColliderObjects_startFromFirstNode(
        const PureObject3D& obj,
        std::vector<const PureObject3D*>& colliders) const;

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
    virtual TPureBool postSubdivideDone() override;

private:

    PureAxisAlignedBoundingBox m_aabb;

    static bool colliding2(
        float o1px, float o1py, float o1pz, float o1sx, float o1sy, float o1sz,
        float o2px, float o2py, float o2pz, float o2sx, float o2sy, float o2sz);

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
