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


#include "../../../../PGEallHeaders.h"
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

    There are a lot of example implementations on the internet:
     - https://iq.opengenus.org/octree/
     - https://people.inf.elte.hu/stlraai/webfejl/articles/octree.html
     - https://www.flipcode.com/archives/Octree_Implementation.shtml
     - http://nomis80.org/code/octree.html
     - https://github.com/PointCloudLibrary/pcl/tree/master/octree

    Note this is not a Loose Octree implementation, but might become in the future.
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
    */
    enum NodeType
    {
        Parent,    /**< This non-leaf node does not have objects but 8 children nodes that might store objects. */
        Empty,     /**< This leaf node has neither children nodes nor objects stored inside. */
        Container  /**< This leaf node has at least 1 object stored inside. */
    };

    // ---------------------------------------------------------------------------

    PRREOctree();
    virtual ~PRREOctree();

    PRREOctree* insertObject(const PRREObject3D& obj);
    PRREOctree* removeObject(const PRREObject3D& obj);
    const PRREOctree* findObject(const PRREObject3D& obj) const;
    TPRREuint getDepthLevel() const;
    TPRREuint getMaxDepthLevel() const;
    NodeType getNodeType() const;


protected:

    // ---------------------------------------------------------------------------
       
    PRREOctree(const PRREOctree&);
    PRREOctree& operator=(const PRREOctree&);

private:

    PRREVector pos;
    PRREVector size;

}; // class PRREOctree 

