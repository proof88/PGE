/*
    ###################################################################################
    PureAxisAlignedBoundingBox.cpp
    This file is part of Pure.
    PureAxisAlignedBoundingBox class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PurebaseIncludes.h"  // PCH
#include "../../include/internal/SpatialStructures/PureAxisAlignedBoundingBox.h"

using namespace std;

/*
   PureAxisAlignedBoundingBox
   ###########################################################################
*/


// ############################### PUBLIC ################################


PureAxisAlignedBoundingBox::PureAxisAlignedBoundingBox()
{

} // PureAxisAlignedBoundingBox


PureAxisAlignedBoundingBox::PureAxisAlignedBoundingBox(const PureVector& pos_, const PureVector& size_) :
    pos(pos_),
    size(size_)
{

} // PureAxisAlignedBoundingBox()


PureAxisAlignedBoundingBox::~PureAxisAlignedBoundingBox()
{

} // ~PureAxisAlignedBoundingBox()


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
TPurebool PureAxisAlignedBoundingBox::isInside(const PureVector& p) const
{
    if ( (size.getX() == 0.f) || (size.getY() == 0.f) || (size.getZ() == 0.f) )
        return false;

    return ( (p.getX() >= (pos.getX() - size.getX()/2.f)) && (p.getX() <= (pos.getX() + size.getX()/2.f)) ) &&
        ( (p.getY() >= (pos.getY() - size.getY()/2.f)) && (p.getY() <= (pos.getY() + size.getY()/2.f)) ) &&
        ( (p.getZ() >= (pos.getZ() - size.getZ()/2.f)) && (p.getZ() <= (pos.getZ() + size.getZ()/2.f)) );
}

/**
    Is the given box inside the bounding box?
    @param aabb The box to be tested.
    @return True if the given box is fully inside the bounding box, false otherwise. Partial contain also results false.
*/
TPurebool PureAxisAlignedBoundingBox::isInside(const PureAxisAlignedBoundingBox& aabb) const
{
    const PureVector aabb_min(
        aabb.getPosVec().getX() - aabb.getSizeVec().getX()/2.f,
        aabb.getPosVec().getY() - aabb.getSizeVec().getY()/2.f,
        aabb.getPosVec().getZ() - aabb.getSizeVec().getZ()/2.f);
    const PureVector aabb_max(
        aabb.getPosVec().getX() + aabb.getSizeVec().getX()/2.f,
        aabb.getPosVec().getY() + aabb.getSizeVec().getY()/2.f,
        aabb.getPosVec().getZ() + aabb.getSizeVec().getZ()/2.f);


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
    if ( (size.getX() == 0.f) || (size.getY() == 0.f) || (size.getZ() == 0.f) )
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
    Extends the size of the bounding box so the given box will be inside the box.
    The position of the bounding box is also expected to change after the extend.
    If this is a 0-sized (uninitialized) box, its position and size will be set to the position and size of the given box (copy).

    @param aabb The box to be included.
*/
void PureAxisAlignedBoundingBox::ExtendBy(const PureAxisAlignedBoundingBox& aabb)
{
    if ( (size.getX() == 0.f) || (size.getY() == 0.f) || (size.getZ() == 0.f) )
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


/*
PureAxisAlignedBoundingBox::PureAxisAlignedBoundingBox(const PureAxisAlignedBoundingBox& other)
{

}


PureAxisAlignedBoundingBox& PureAxisAlignedBoundingBox::operator=(const PureAxisAlignedBoundingBox& other)
{
    return *this;
}
*/


// ############################### PRIVATE ###############################

