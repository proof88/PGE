/*
    ###################################################################################
    PureAxisAlignedBoundingBox.cpp
    This file is part of PURE.
    PureAxisAlignedBoundingBox class.
    Made by PR00F88
    ###################################################################################
*/

#include "PureBaseIncludes.h"  // PCH
#include "../../include/external/SpatialStructures/PureAxisAlignedBoundingBox.h"

using namespace std;

/*
   PureAxisAlignedBoundingBox
   ###########################################################################
*/


// ############################### PUBLIC ################################


PureAxisAlignedBoundingBox::PureAxisAlignedBoundingBox(const PureVector& pos_, const PureVector& size_) :
    pos(pos_),
    size(size_)
{

} // PureAxisAlignedBoundingBox()

/**
* @return True if all sides of this box is non-zero, false otherwise.
*/
const bool PureAxisAlignedBoundingBox::isInitialized() const
{
    return ((size.getX() != 0.f) && (size.getY() != 0.f) && (size.getZ() != 0.f));
}

/**
    Gets the world-space position which is the center of the bounding box.
    @return The center of the bounding box in world-space.
*/
const PureVector& PureAxisAlignedBoundingBox::getPosVec() const
{
    return pos;
} 


/**
    Gets the base sizes.
    @return Size of the bounding box.
*/
const PureVector& PureAxisAlignedBoundingBox::getSizeVec() const
{
    return size;
}                     


/**
    Is the given point inside the bounding box?
    @param p The point to be tested. Coordinates are expected in world-space.
    @return True if the given point is inside the bounding box, false otherwise.
*/
TPureBool PureAxisAlignedBoundingBox::isInside(const PureVector& p) const
{
    if (!isInitialized())
    {
        return false;
    }

    return ( (p.getX() >= (pos.getX() - size.getX()/2.f)) && (p.getX() <= (pos.getX() + size.getX()/2.f)) ) &&
        ( (p.getY() >= (pos.getY() - size.getY()/2.f)) && (p.getY() <= (pos.getY() + size.getY()/2.f)) ) &&
        ( (p.getZ() >= (pos.getZ() - size.getZ()/2.f)) && (p.getZ() <= (pos.getZ() + size.getZ()/2.f)) );
}

/**
    Is the given box fully inside the bounding box?

    @param aabb The box to be tested.
    @return True if the given box is fully inside the bounding box, false otherwise. Partial contain also results false.
*/
TPureBool PureAxisAlignedBoundingBox::isInside(const PureAxisAlignedBoundingBox& aabb) const
{
    const PureVector& vecAabbPos = aabb.getPosVec();
    const PureVector& vecAabbSize = aabb.getSizeVec();
    const PureVector aabb_min(
        vecAabbPos.getX() - vecAabbSize.getX() / 2.f,
        vecAabbPos.getY() - vecAabbSize.getY() / 2.f,
        vecAabbPos.getZ() - vecAabbSize.getZ() / 2.f);
    const PureVector aabb_max(
        vecAabbPos.getX() + vecAabbSize.getX() / 2.f,
        vecAabbPos.getY() + vecAabbSize.getY() / 2.f,
        vecAabbPos.getZ() + vecAabbSize.getZ() / 2.f);


    return isInside(aabb_min) && isInside(aabb_max);
}


/**
    Extends the size of the bounding box so the given point will be inside the box.
    The position of the bounding box is also expected to change after the extend.
    If this is a 0-sized (uninitialized) box, nothing will happen.

    @param p The point to be included. Coordinates are expected in world-space.
*/
void PureAxisAlignedBoundingBox::ExtendBy(const PureVector& p)
{
    if (!isInitialized())
    {
        return;
    }

    const PureVector oldPos = pos;

    if ( p.getX() < pos.getX() - size.getX()/2.f ) {
        pos.SetX( (p.getX() + pos.getX()+size.getX()/2.f) / 2.f );
        size.SetX( ( oldPos.getX() + size.getX()/2.f ) - p.getX());
    } else if ( p.getX() > pos.getX() + size.getX()/2.f ) {
        pos.SetX( (p.getX() + pos.getX()-size.getX()/2.f) / 2.f );
        size.SetX( p.getX() - ( oldPos.getX() - size.getX()/2.f ));
    }

    if ( p.getY() < pos.getY() - size.getY()/2.f ) {
        pos.SetY( (p.getY() + pos.getY()+size.getY()/2.f) / 2.f );
        size.SetY( ( oldPos.getY() + size.getY()/2.f ) - p.getY());
    } else if ( p.getY() > pos.getY() + size.getY()/2.f ) {
        pos.SetY( (p.getY() + pos.getY()-size.getY()/2.f) / 2.f );
        size.SetY( p.getY() - ( oldPos.getY() - size.getY()/2.f ));
    }

    if ( p.getZ() < pos.getZ() - size.getZ()/2.f ) {
        pos.SetZ( (p.getZ() + pos.getZ()+size.getZ()/2.f) / 2.f );
        size.SetZ( ( oldPos.getZ() + size.getZ()/2.f ) - p.getZ());
    } else if ( p.getZ() > pos.getZ() + size.getZ()/2.f ) {
        pos.SetZ( (p.getZ() + pos.getZ()-size.getZ()/2.f) / 2.f );
        size.SetZ( p.getZ() - ( oldPos.getZ() - size.getZ()/2.f ));
    }
}       


/**
    Extends the size of the bounding box so the given box will be fully inside the box.
    The position of the bounding box is also expected to change after the extend.
    If this is a 0-sized (uninitialized) box, its position and size will be set to the position and size of the given box (copy).

    @param aabb The box to be included.
*/
void PureAxisAlignedBoundingBox::ExtendBy(const PureAxisAlignedBoundingBox& aabb)
{
    if (!isInitialized())
    {
        pos = aabb.pos;
        size = aabb.size;
        return;
    }

    const PureVector aabb_min(
        aabb.getPosVec().getX() - aabb.getSizeVec().getX()/2.f,
        aabb.getPosVec().getY() - aabb.getSizeVec().getY()/2.f,
        aabb.getPosVec().getZ() - aabb.getSizeVec().getZ()/2.f);
    const PureVector aabb_max(
        aabb.getPosVec().getX() + aabb.getSizeVec().getX()/2.f,
        aabb.getPosVec().getY() + aabb.getSizeVec().getY()/2.f,
        aabb.getPosVec().getZ() + aabb.getSizeVec().getZ()/2.f);

    ExtendBy(aabb_min);
    ExtendBy(aabb_max);
}    


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################

