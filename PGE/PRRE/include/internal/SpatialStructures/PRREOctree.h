#pragma once

/*
    ###################################################################################
    PRREOctree.h
    This file is part of PRRE.
    Internal header.
    PRREOctree class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/


#include <set>
#include <vector>

#include "../../../../PGEallHeaders.h"
#include "../../external/Math/PRREVector.h"
#include "../../external/Object3D/PRREObject3DManager.h"


/**
    Octree: a tree data structure in which each node has either 0 or exactly 8 children nodes which partition the parent node into equal size cubic volumes.
    This is a spatial hierarchy structure, to accelerate some of the spatial find operations, such as finding the closest object to each other in a scene.
    The root node is the spatially biggest node, with the size enough to cover the entire scene (e.g. map size).
    Scene objects are inserted into the octree one by one, by finding an empty node for them based on their position, starting from the root node.
    If a tree node suitable for the object being inserted doesn't have any object yet, we store the object in that node.
    If a tree node suitable for the object being inserted already has 1 object, and we have not yet reached the maximum tree depth, we further subdivide
    that node into 8 children nodes, and reinsert the object already inside the tree, and insert the new object.
    In case we already reached the maximum tree depth, we do not further subdivide but just insert the new object to the most suitable node, in this case
    a node can has multiple objects stored in it.

    This is just my own simply octree implementation.
    Note that although the octree divides the space perfectly, the objects being put inside this tree are allowed to overlap multiple tree nodes.
    Their position decides which tree node they will be put into, overlapping across different nodes is not considered.

    There are a lot of example implementations on the internet:
    A few simple:
     - https://iq.opengenus.org/octree/
     - https://people.inf.elte.hu/stlraai/webfejl/articles/octree.html
     - https://www.flipcode.com/archives/Octree_Implementation.shtml
    Some more advanced:
     - http://nomis80.org/code/octree.html
     - https://github.com/PointCloudLibrary/pcl/tree/master/octree

    Note this is not a Loose Octree implementation, but might become in the future.
    Info about loose octree:
     - https://sakura7.blog.hu/2010/07/05/loose_octree
     - http://www.tulrich.com/geekstuff/partitioning.html
*/
class PRREOctree
{
#ifdef PRRE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PRREOctree is included")
#endif

public:

    /**
        NodeType tells if the given node has children nodes or Objects.
        The default type is LeafEmpty. This changes to LeafContainer after adding 1 object to the node.
        LeafContainer can later change to Parent after adding further objects to the same node.
    */
    enum NodeType
    {
        Parent,        /**< This non-leaf node does not have objects but 8 children nodes that might store objects. */
        LeafEmpty,     /**< This leaf node has neither children nodes nor objects stored inside. */
        LeafContainer  /**< This leaf node has at least 1 object stored inside. */
    };


    /**
        ChildIndex indexes a child node within a vector of children nodes.
        It is basically a number between 0 and 7, but it is easier to read and write code if we
        set named bits depending on where the child node is placed on each axis compared to its parent.
        Front means facing to camera, back means looking away from camera view.
    */
    typedef TPRREuint ChildIndex;

    static const ChildIndex BIT_AXIS_Y;                             /**< Bit to set to TOP or BOTTOM. */
    static const ChildIndex BIT_AXIS_X;                             /**< Bit to set to LEFT or RIGHT. */
    static const ChildIndex BIT_AXIS_Z;                             /**< Bit to set to FRONT or BACK. */

    static const ChildIndex TOP;                                    /**< Child is above or in same as parent's position on Y-axis. */
    static const ChildIndex BOTTOM;                                 /**< Child is under parent's position on Y-axis. */
    static const ChildIndex LEFT;                                   /**< Child is left to parent's position on X-axis. */
    static const ChildIndex RIGHT;                                  /**< Child is right to or in same as parent's position on X-axis. */
    static const ChildIndex FRONT;                                  /**< Child is closer to camera than parent's position on Z-axis. */
    static const ChildIndex BACK;                                   /**< Child is farther or same distance from camera than parent's position on Z-axis. */

    // ---------------------------------------------------------------------------

    PRREOctree(const PRREVector& pos, TPRREfloat size, TPRREuint maxDepthLevel, TPRREuint currentDepthLevel);
    virtual ~PRREOctree();

    ChildIndex calculateIndex(const PRREVector& pos) const;         /**< Calculates child node index for the given position in the current node. */

    PRREOctree* insertObject(const PRREObject3D& obj);              /**< Inserts the given object in the octree. */
    // remove is not supported, since currently Octree is used for static objects, which are not being deleted,
    // only when the octree would need full rebuild anyway
    //TPRREbool removeObject(const PRREObject3D& obj);              /**< Removes the given object from the octree. */
    const PRREOctree* findObject(const PRREObject3D& obj) const;    /**< Finds the given object in the octree. */
    TPRREuint getDepthLevel() const;                                /**< Gets the current depth level of the octree node. */
    TPRREuint getMaxDepthLevel() const;                             /**< Gets the maximum depth level of the octree node as it was specified in the constructor of the octree. */
    NodeType getNodeType() const;                                   /**< Gets the type of the octree node which depends on if the node has any objects or children nodes. */
    const PRREVector& getPos() const;                               /**< Gets the world-space position of the node as specified in the constructor. */
    TPRREfloat getSize() const;                                     /**< Gets the length of the side of the cube represented by this node as it was specified in the constructor. */
    const std::vector<PRREOctree>& getChildren() const;             /**< Gets the children nodes of this node. */
    const std::set<const PRREObject3D*>& getObjects() const;        /**< Gets the stored objects of this node. */


protected:

    // ---------------------------------------------------------------------------
       
    PRREOctree();
    
    // defaults will be fine
    //PRREOctree(const PRREOctree&);
    //PRREOctree& operator=(const PRREOctree&);

private:

    PRREVector vPos;
    TPRREfloat fSize;
    TPRREuint nCurrentDepth;
    TPRREuint nMaxDepth;
    NodeType nodeType;
    std::vector<PRREOctree> vChildren;
    std::set<const PRREObject3D*> vObjects;

}; // class PRREOctree 

