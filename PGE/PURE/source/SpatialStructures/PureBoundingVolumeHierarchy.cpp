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
#include "../../../../../455-355-7357-88/455-355-7357-88/Benchmarks.h"

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
    m_objDebugBox(nullptr)
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
    m_objDebugBox(nullptr)
{
    // TODO: AABB update with pos & size ??? Well, in 2023 I explicitly stated in the API comment for the size argument above that it is NOT the size of the AABB, 
    // so most probably I dont need to update AABB here.
} // PureBoundingVolumeHierarchy(...)


PureBoundingVolumeHierarchy::~PureBoundingVolumeHierarchy()
{
    DeleteChildren();
    if (m_objDebugBox)
    {
        delete m_objDebugBox; // Object3D dtor triggers removing from ObjectManager too
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
    node->m_aabb.ExtendBy(obj_bbox);

    PureBoundingVolumeHierarchy* prevChild = node;
    PureBoundingVolumeHierarchy* p = static_cast<PureBoundingVolumeHierarchy*>(node->m_parent);
    while (p != PGENULL)
    {
        p->m_aabb.ExtendBy(prevChild->m_aabb);
        prevChild = p;
        p = static_cast<PureBoundingVolumeHierarchy*>(p->m_parent);
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
        m_aabb = PureAxisAlignedBoundingBox();
        if (m_objDebugBox)
        {
            delete m_objDebugBox; // Object3D dtor triggers removing from ObjectManager too
            m_objDebugBox = nullptr;
        }
        return true;
    }

    return false;
}


/**
    Non-const version of findObject(), required in some cases because insertObject() also non-const due to these nodes
    shall be modifiable just for the sake of PureBoundingVolumeHierarchyRoot::markAabbDebugRendering().
    So the same reason as why we also have a non-const version of getDebugBox().

    @param obj The object to be found.
    @return The BVH node containing the object. If the object is not found, PGENULL is returned.
*/
PureBoundingVolumeHierarchy* PureBoundingVolumeHierarchy::findObject(const PureObject3D& obj)
{
    // simply invoke the const-version of findObject() above by const-casting:
    return const_cast<PureBoundingVolumeHierarchy*>(static_cast<const PureBoundingVolumeHierarchy*>(this)->findObject(obj));
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
    return m_aabb;
}

/**
    Finds the smallest node within the tree which still FULLY contains the given AABB.

    @param objAabb The AABB for which we are searching the smallest node that fully bounds this AABB.

    @return The smallest node within the tree which still FULLY contains the given AABB, or nullptr if there is no such node.
*/
const PureBoundingVolumeHierarchy* PureBoundingVolumeHierarchy::findTightestFittingNode(const PureAxisAlignedBoundingBox& objAabb) const
{
    ScopeBenchmarker<std::chrono::microseconds> bm(__func__);
    if (m_aabb.isInside(objAabb))
    {
        if (getNodeType() == Parent)
        {
            assert(!m_vChildren.empty());

            // the naive approach: iterate over children and try find one which is more tightly fitting.
            //for (auto pNode : m_vChildren)
            //{
            //    assert(pNode);
            //    PureBoundingVolumeHierarchy& bvhNode = static_cast<PureBoundingVolumeHierarchy&>(*pNode);
            //    PureBoundingVolumeHierarchy* const pBvhNodeTightestFittingChild = bvhNode.findTightestFittingNode(objAabb);
            //    if (pBvhNodeTightestFittingChild)
            //    {
            //        return pBvhNodeTightestFittingChild;
            //    }
            //}

            // the optimal approach:
            // since BVH nodes at the same level are NOT intersecting each other, therefore the closest child should be checked only,
            // since there CANNOT BE another child at the same level which fully fits. Only the closest could fit, if that does not fit then no other either!
            // This is another example of the power octree has!
            const ChildIndex iChild = calculateIndex(objAabb.getPosVec());
            PureBoundingVolumeHierarchy& bvhNode = static_cast<PureBoundingVolumeHierarchy&>(*m_vChildren[iChild]);
            PureBoundingVolumeHierarchy* const pBvhNodeTightestFittingChild = bvhNode.findTightestFittingNode(objAabb);
            if (pBvhNodeTightestFittingChild)
            {
                return pBvhNodeTightestFittingChild;
            }

            // TODO: OPT: but another optimal approach could be to simply go down in the hierarchy by only using calculateIndex() at each level,
            // and from there, start going up by invoking that m_aabb.isInside() that we are now doing from root towards down.
            // I'm also thinking that a parameter might control if we going like this or the other way that is currently implemented, because
            // this extra parameter specified by the app who could have more clue based on the size/kind of object, can better fine-tune the algorithm.
            // For example, if we are querying for a big building, the current implementation might be faster, but for small objects in a city, the other
            // approach might be faster due to much less call to m_aabb.isInside().
        }
        return this;
    }
    return nullptr;
}

/**
    Finds the smallest node within the tree which still FULLY contains the given Object3D.

    @param obj The Object3D for which we are searching the smallest node that fully bounds this Object3D.

    @return The smallest node within the tree which still FULLY contains the given Object3D, or nullptr if there is no such node.
*/
const PureBoundingVolumeHierarchy* PureBoundingVolumeHierarchy::findTightestFittingNode(const PureObject3D& obj) const
{
    PureAxisAlignedBoundingBox objAabb(obj.getPosVec(), obj.getScaledSizeVec());
    return findTightestFittingNode(objAabb);
}

/**
    Same as the const-version, however we need a non-const version as well for the same reason as why we have non-const version of
    other functions as well such as getDebugBox().
*/
PureBoundingVolumeHierarchy* PureBoundingVolumeHierarchy::findTightestFittingNode(const PureAxisAlignedBoundingBox& objAabb)
{
    // simply invoke the const-version by const-casting:
    return const_cast<PureBoundingVolumeHierarchy*>(static_cast<const PureBoundingVolumeHierarchy*>(this)->findTightestFittingNode(objAabb));
}

/**
    Same as the const-version, however we need a non-const version as well for the same reason as why we have non-const version of
    other functions as well such as getDebugBox().
*/
PureBoundingVolumeHierarchy* PureBoundingVolumeHierarchy::findTightestFittingNode(const PureObject3D& obj)
{
    // simply invoke the const-version by const-casting:
    return const_cast<PureBoundingVolumeHierarchy*>(static_cast<const PureBoundingVolumeHierarchy*>(this)->findTightestFittingNode(obj));
}

/**
    Finds an inserted object within the tree which is colliding (at least partially overlapping) with the given AABB.
    Basically same as findOneColliderObject(const PureObject3D&) but using the given object's AABB.

    @param objAabb    The AABB for which we are searching any colliding object within the tree.
    @param pStartNode From which BVH node collision detection will start.
                      For application, passing nullptr is recommended so the function will determine the proper start node.

    @return An object within the tree colliding with the specified object.
            Nullptr if no colliding object found.
*/
const PureObject3D* PureBoundingVolumeHierarchy::findOneColliderObject(const PureAxisAlignedBoundingBox& objAabb, const PureBoundingVolumeHierarchy* pStartNode) const
{
    if (!pStartNode)
    {
        ScopeBenchmarker<std::chrono::microseconds> bm(__func__);
        const auto pTightestFittingNode = findTightestFittingNode(objAabb);
        pStartNode = pTightestFittingNode ? pTightestFittingNode : this;
        return pStartNode->findOneColliderObject(objAabb, pStartNode);
    }

    if (getNodeType() == Parent)
    {
        //const ChildIndex iChild = calculateIndex(objAabb.getPosVec());
        //const PureBoundingVolumeHierarchy& bvhNode = static_cast<const PureBoundingVolumeHierarchy&>(*m_vChildren[iChild]);
        //return bvhNode.findOneColliderObject(objAabb, &bvhNode);

        const PureObject3D* pCollider = nullptr;
        for (size_t iChild = 0; (iChild < m_vChildren.size()) && !pCollider; iChild++)
        {
            const PureBoundingVolumeHierarchy& bvhNode = static_cast<const PureBoundingVolumeHierarchy&>(*m_vChildren[iChild]);
            pCollider = bvhNode.findOneColliderObject(objAabb, &bvhNode);
        }
        return pCollider;
    }

    // we are LeafEmpty or LeafContainer
    for (const auto& pStoredObj : getObjects())
    {
        assert(pStoredObj);
        if (colliding2(
            pStoredObj->getPosVec().getX(), pStoredObj->getPosVec().getY(), pStoredObj->getPosVec().getZ(),
            pStoredObj->getScaledSizeVec().getX(), pStoredObj->getScaledSizeVec().getY(), pStoredObj->getScaledSizeVec().getZ(),
            objAabb.getPosVec().getX(), objAabb.getPosVec().getY(), objAabb.getPosVec().getZ(),
            objAabb.getSizeVec().getX(), objAabb.getSizeVec().getY(), objAabb.getSizeVec().getZ()
        ))
        {
            return pStoredObj;
        }
    }

    return nullptr;
}

/**
    Finds an inserted object within the tree which is colliding (at least partially overlapping) with the given object.
    
    Only 1 object is returned, even if there are multiple colliding objects with the given object.
    The reason is that because at application physics level we place the moving objects initially to position where they don't collide with anything, and
    then on each axis we move them and check if there is ANY collision, if so then place them to their previous position on that current axis.
    For this implementation, it is enough to return only 1 collider.

    @param obj The Object3D for which we are searching any colliding object within the tree.

    @return An object within the tree colliding with the specified object.
            Nullptr if no colliding object found.
*/
const PureObject3D* PureBoundingVolumeHierarchy::findOneColliderObject(const PureObject3D& obj) const
{
    PureAxisAlignedBoundingBox objAabb(obj.getPosVec(), obj.getScaledSizeVec());
    return findOneColliderObject(objAabb, nullptr);
}

/**
    Finds all inserted objects within the tree which are colliding (at least partially overlapping) with the given AABB.
    Basically same as findAllColliderObjects(const PureObject3D&) but using the given object's AABB.

    @param objAabb    The AABB for which we are searching all colliding objects within the tree.
    @param pStartNode From which BVH node collision detection will start.
                      For application, passing nullptr is recommended so the function will determine the proper start node.
    @param colliders  Output container where all collider objects will be stored.

    @return True if at least 1 colliding object was found, false otherwise.
*/
bool PureBoundingVolumeHierarchy::findAllColliderObjects(
    const PureAxisAlignedBoundingBox& objAabb,
    const PureBoundingVolumeHierarchy* pStartNode,
    std::vector<const PureObject3D*>& colliders) const
{
    if (!pStartNode)
    {
        ScopeBenchmarker<std::chrono::microseconds> bm(__func__);
        colliders.clear();
        const auto pTightestFittingNode = findTightestFittingNode(objAabb);
        pStartNode = pTightestFittingNode ? pTightestFittingNode : this;
        return pStartNode->findAllColliderObjects(objAabb, pStartNode, colliders);
    }

    if (getNodeType() == Parent)
    {
        for (size_t iChild = 0; iChild < m_vChildren.size(); iChild++)
        {
            const PureBoundingVolumeHierarchy& bvhNode = static_cast<const PureBoundingVolumeHierarchy&>(*m_vChildren[iChild]);
            bvhNode.findAllColliderObjects(objAabb, &bvhNode, colliders);
        }
        return !colliders.empty();
    }

    // we are LeafEmpty or LeafContainer
    for (const auto& pStoredObj : getObjects())
    {
        assert(pStoredObj);
        if (colliding2(
            pStoredObj->getPosVec().getX(), pStoredObj->getPosVec().getY(), pStoredObj->getPosVec().getZ(),
            pStoredObj->getScaledSizeVec().getX(), pStoredObj->getScaledSizeVec().getY(), pStoredObj->getScaledSizeVec().getZ(),
            objAabb.getPosVec().getX(), objAabb.getPosVec().getY(), objAabb.getPosVec().getZ(),
            objAabb.getSizeVec().getX(), objAabb.getSizeVec().getY(), objAabb.getSizeVec().getZ()
        ))
        {
            colliders.push_back(pStoredObj);
        }
    }

    return !colliders.empty();
}

/**
    Finds all inserted objects within the tree which are colliding (at least partially overlapping) with the given object.

    @param obj       The Object3D for which we are searching any colliding object within the tree.
    @param colliders Output container where all collider objects will be stored.

    @return True if at least 1 colliding object was found, false otherwise.
*/
bool PureBoundingVolumeHierarchy::findAllColliderObjects(const PureObject3D& obj, std::vector<const PureObject3D*>& colliders) const
{
    PureAxisAlignedBoundingBox objAabb(obj.getPosVec(), obj.getScaledSizeVec());
    return findAllColliderObjects(objAabb, nullptr, colliders);
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
        // yes, empty leaf node also has aabb but that is the default-initialized [0,0,0]-sized :)
        return;
    }

    if (!m_objDebugBox)
    {
        m_objDebugBox = objmgr.createBox(1.f, 1.f, 1.f);
        m_objDebugBox->SetWireframed(true);
        //m_objDebugBox->SetWireframedCulled(false);
        m_objDebugBox->SetAffectingZBuffer(false);
        m_objDebugBox->SetTestingAgainstZBuffer(false);
        m_objDebugBox->getMaterial(false).getTextureEnvColor(0) = colorWireframe;
    }
    m_objDebugBox->getPosVec() = m_aabb.getPosVec();
    m_objDebugBox->SetScaling(m_aabb.getSizeVec());

    if (getNodeType() == Parent)
    {
        for (auto pNode : m_vChildren)
        {
            assert(pNode);
            PureBoundingVolumeHierarchy& bvhNode = static_cast<PureBoundingVolumeHierarchy&>(*pNode);
            bvhNode.updateAndEnableAabbDebugRendering(objmgr, colorWireframe);
        }
    }
}

PureObject3D* PureBoundingVolumeHierarchy::getDebugBox()
{
    return m_objDebugBox;
}

const PureObject3D* PureBoundingVolumeHierarchy::getDebugBox() const
{
    return m_objDebugBox;
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

    if (m_objDebugBox)
    {
        m_objDebugBox->Hide();
    }

    if (getNodeType() == Parent)
    {
        for (auto pNode : m_vChildren)
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
    assert(m_vChildren.size() == 0);
    try
    {
      for (TPureUInt i = 0; i < 8; i++)
      {
          PureBoundingVolumeHierarchy* const pChildNode = new PureBoundingVolumeHierarchy(PureVector(), getSize()/2.f, getMaxDepthLevel(), getDepthLevel() + 1);
          m_vChildren.push_back(pChildNode);
      }
    }
    catch (const std::exception&)
    {
        for (TPureUInt i = 0; i < 8; i++)
        {
            if (m_vChildren[i] != PGENULL )
            {
                delete m_vChildren[i];
            }
            m_vChildren.resize(0);
        }
        return false;
    }
    return true;
}


// ############################### PRIVATE ###############################


// private so I dont need to unit-test
bool PureBoundingVolumeHierarchy::colliding2(
    float o1px, float o1py, float o1pz, float o1sx, float o1sy, float o1sz,
    float o2px, float o2py, float o2pz, float o2sx, float o2sy, float o2sz)
{
    return (
        (
            (o1px - o1sx / 2 <= o2px + o2sx / 2)
            &&
            (o1px + o1sx / 2 >= o2px - o2sx / 2)
            )
        &&
        (
            (o1py - o1sy / 2 <= o2py + o2sy / 2)
            &&
            (o1py + o1sy / 2 >= o2py - o2sy / 2)
            )
        &&
        (
            (o1pz - o1sz / 2 <= o2pz + o2sz / 2)
            &&
            (o1pz + o1sz / 2 >= o2pz - o2sz / 2)
            )
        );
}


/*
   PureBoundingVolumeHierarchyRoot
   ###########################################################################
*/


// ############################### PUBLIC ################################


/**
    Same functionality as PureBoundingVolumeHierarchy() with the same arguments.
*/
PureBoundingVolumeHierarchyRoot::PureBoundingVolumeHierarchyRoot(TPureUInt maxDepthLevel, TPureUInt currentDepthLevel) :
    PureBoundingVolumeHierarchy(maxDepthLevel, currentDepthLevel),
    m_objPrevMarkedDebugBox(nullptr)
{
}

/**
    Same functionality as PureBoundingVolumeHierarchy() with the same arguments.
*/
PureBoundingVolumeHierarchyRoot::PureBoundingVolumeHierarchyRoot(const PureVector& pos, TPureFloat size, TPureUInt maxDepthLevel, TPureUInt currentDepthLevel) :
    PureBoundingVolumeHierarchy(pos, size, maxDepthLevel, currentDepthLevel),
    m_objPrevMarkedDebugBox(nullptr)
{
}


PureBoundingVolumeHierarchyRoot::~PureBoundingVolumeHierarchyRoot()
{
    m_objPrevMarkedDebugBox = nullptr;
} // ~PureBoundingVolumeHierarchyRoot()

/**
    Extending PureBoundingVolumeHierarchy::reset() behavior by clearing marked AABB debug box.

    @return True in case of success, false if invoked on non-root node.
*/
bool PureBoundingVolumeHierarchyRoot::reset()
{
    if (PureBoundingVolumeHierarchy::reset())
    {
        m_objPrevMarkedDebugBox = nullptr;
        return true;
    }
    return false;
}

/**
    Marks the specified node in debug rendering i.e. its rendering will be different compared to other AABB debug boxes.

    The previously marked node will be unmarked i.e. its debug rendering will be reset back to its original unmarked rendering.
    Valid only if updateAndEnableAabbDebugRendering() is invoked previously, otherwise no effect.

    @param nodeToBeMarked The node we want to mark.
                          If nullptr, no node will be marked, and the currently marked node (if there is any) will be unmarked.
    @param color          The color we want to use for marking the specified node.
*/
void PureBoundingVolumeHierarchyRoot::markAabbDebugRendering(
    PureBoundingVolumeHierarchy* nodeToBeMarked,
    PureColor color /* copy-elision probably */)
{
    if (m_objPrevMarkedDebugBox)
    {
        m_objPrevMarkedDebugBox->SetWireframed(true);
        m_objPrevMarkedDebugBox->getMaterial(false).getTextureEnvColor(0) = m_colorPrevDebugBoxBeforeMarking;
        m_objPrevMarkedDebugBox->getMaterial(false).setBlendMode(TPURE_BLENDMODE::PURE_BM_NONE);
    }

    if (!nodeToBeMarked || !nodeToBeMarked->getDebugBox())
    {
        return;
    }

    m_objPrevMarkedDebugBox = nodeToBeMarked->getDebugBox();
    m_colorPrevDebugBoxBeforeMarking = nodeToBeMarked->getDebugBox()->getMaterial(false).getTextureEnvColor(0);

    nodeToBeMarked->getDebugBox()->SetWireframed(false);
    nodeToBeMarked->getDebugBox()->getMaterial(false).getTextureEnvColor(0) = color;
    nodeToBeMarked->getDebugBox()->getMaterial(false).setBlendMode(TPURE_BLENDMODE::PURE_BM_STANDARD_TRANSPARENCY);
}


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################