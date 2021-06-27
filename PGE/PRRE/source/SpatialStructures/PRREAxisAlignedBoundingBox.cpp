/*
    ###################################################################################
    PRREAxisAlignedBoundingBox.cpp
    This file is part of PRRE.
    PRREAxisAlignedBoundingBox class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREbaseIncludes.h"  // PCH
#include "../../include/internal/SpatialStructures/PRREAxisAlignedBoundingBox.h"

using namespace std;

/*
   PRREAxisAlignedBoundingBox
   ###########################################################################
*/


// ############################### PUBLIC ################################


PRREAxisAlignedBoundingBox::PRREAxisAlignedBoundingBox(const PRREVector& pos_, const PRREVector& size_) :
    pos(pos_),
    size(size_)
{

} // PRREAxisAlignedBoundingBox()


PRREAxisAlignedBoundingBox::~PRREAxisAlignedBoundingBox()
{

} // ~PRREAxisAlignedBoundingBox()


/**
    Gets the world-space position which is the center of the bounding box.
    @return The center of the bounding box in world-space.
*/
const PRREVector& PRREAxisAlignedBoundingBox::getPosVec() const
{
    return pos;
} 


/**
    Gets the base sizes.
    @return Size of the bounding box.
*/
const PRREVector& PRREAxisAlignedBoundingBox::getSizeVec() const
{
    return size;
}                     


/**
    Is the given point inside the bounding box?
    @param p The point to be tested. Coordinates are expected in world-space.
    @return True if the given point is inside the bounding box, false otherwise.
*/
TPRREbool PRREAxisAlignedBoundingBox::isInside(const PRREVector& p) const
{
    return ( (p.getX() >= (pos.getX() - size.getX()/2.f)) && (p.getX() <= (pos.getX() + size.getX()/2.f)) ) &&
        ( (p.getY() >= (pos.getY() - size.getY()/2.f)) && (p.getY() <= (pos.getY() + size.getY()/2.f)) ) &&
        ( (p.getZ() >= (pos.getZ() - size.getZ()/2.f)) && (p.getZ() <= (pos.getZ() + size.getZ()/2.f)) );
}

/**
    Is the given box inside the bounding box?
    @param aabb The box to be tested.
    @return True if the given box is fully inside the bounding box, false otherwise. Partial contain also results false.
*/
TPRREbool PRREAxisAlignedBoundingBox::isInside(const PRREAxisAlignedBoundingBox& aabb) const
{
    const PRREVector aabb_min(
        aabb.getPosVec().getX() - aabb.getSizeVec().getX()/2.f,
        aabb.getPosVec().getY() - aabb.getSizeVec().getY()/2.f,
        aabb.getPosVec().getZ() - aabb.getSizeVec().getZ()/2.f);
    const PRREVector aabb_max(
        aabb.getPosVec().getX() + aabb.getSizeVec().getX()/2.f,
        aabb.getPosVec().getY() + aabb.getSizeVec().getY()/2.f,
        aabb.getPosVec().getZ() + aabb.getSizeVec().getZ()/2.f);


    return isInside(aabb_min) && isInside(aabb_max);
}


/**
    Extends the size of the bounding box so the given point will be inside the box.
    The position of the bounding box is also expected to change after the extend.
    @param p The point to be included. Coordinates are expected in world-space.
*/
void PRREAxisAlignedBoundingBox::ExtendBy(const PRREVector& p)
{
    const PRREVector oldPos = pos;

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
    @param aabb The box to be included.
*/
void PRREAxisAlignedBoundingBox::ExtendBy(const PRREAxisAlignedBoundingBox& aabb)
{
    const PRREVector aabb_min(
        aabb.getPosVec().getX() - aabb.getSizeVec().getX()/2.f,
        aabb.getPosVec().getY() - aabb.getSizeVec().getY()/2.f,
        aabb.getPosVec().getZ() - aabb.getSizeVec().getZ()/2.f);
    const PRREVector aabb_max(
        aabb.getPosVec().getX() + aabb.getSizeVec().getX()/2.f,
        aabb.getPosVec().getY() + aabb.getSizeVec().getY()/2.f,
        aabb.getPosVec().getZ() + aabb.getSizeVec().getZ()/2.f);

    ExtendBy(aabb_min);
    ExtendBy(aabb_max);
}    


// ############################## PROTECTED ##############################


PRREAxisAlignedBoundingBox::PRREAxisAlignedBoundingBox()
{

}


/*
PRREAxisAlignedBoundingBox::PRREAxisAlignedBoundingBox(const PRREAxisAlignedBoundingBox& other)
{

}


PRREAxisAlignedBoundingBox& PRREAxisAlignedBoundingBox::operator=(const PRREAxisAlignedBoundingBox& other)
{
    return *this;
}
*/


// ############################### PRIVATE ###############################

