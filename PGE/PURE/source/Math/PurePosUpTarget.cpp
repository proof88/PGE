/*
    ###################################################################################
    PurePosUpTarget.cpp
    This file is part of PURE.
    Pure class for PUT (position, up and target) vectors.
    Made by PR00F88
    ###################################################################################
*/

#include "PureBaseIncludes.h"  // PCH
#include "../../include/external/Math/PurePosUpTarget.h"
#include "../../include/external/Math/PureTransformMatrix.h"
#include "../../include/internal/PurePragmas.h"


// ############################### PUBLIC ################################

/**
    Initializes the position vector to [0,0,0], the up vector to [0,1,0] and the target vector to [0,0,1].
*/
PurePosUpTarget::PurePosUpTarget() :
    vUp(0.0f, 1.0f, 0.0f),
    vTarget(0.0f, 0.0f, 1.0f)
{
}


PurePosUpTarget::PurePosUpTarget(const PurePosUpTarget& put) :
    vPos(put.vPos),
    vTarget(put.vTarget),
    vUp(put.vUp)
{}


/**
    Initializes the position, up and target vectors with the given vectors.
*/
PurePosUpTarget::PurePosUpTarget(const PureVector& pos, const PureVector& target, const PureVector& up) :
    vPos(pos),
    vTarget(target),
    vUp(up)
{
}

/**
    Gets Position vector.
    By default it is (0,0,0).
    We are looking from the Position vector to the Target vector.
    Changing the Position vector will result in changing the pitch and/or the yaw i.e. rotation along the X- and/or the Y-axis.
    If you don't want to change pitch and yaw, apply the same change to the Target vector as well: use the Move()/Strafe()/Elevate() functions.
 */
PureVector& PurePosUpTarget::getPosVec()
{
    return vPos;
}

/**
    Gets Position vector.
    By default it is (0,0,0).
    We are looking from the Position vector to the Target vector.
    Changing the Position vector will result in changing the pitch and/or the yaw i.e. rotation along the X- and/or the Y-axis.
    If you don't want to change pitch and yaw, apply the same change to the Target vector as well: use the Move()/Strafe()/Elevate() functions.
*/
const PureVector& PurePosUpTarget::getPosVec() const
{
    return vPos;
}

/**
    Gets Target vector.
    We are looking from the Position vector to the Target vector.
    By default it is (0,0,1) which means 0 degrees pitch and yaw together with the default (0,0,0) Position vector.
    Changing the Target vector will result in changing the pitch and/or the yaw i.e. rotation along the X- and/or the Y-axis.
    If you don't want to change pitch and yaw, apply the same change to the Position vector as well: use the Move()/Strafe()/Elevate() functions.
*/
PureVector& PurePosUpTarget::getTargetVec()
{
    return vTarget;
}

/**
    Gets Target vector.
    We are looking from the Position vector to the Target vector.
    By default it is (0,0,1) which means 0 degrees pitch and yaw together with the default (0,0,0) Position vector.
    Changing the Target vector will result in changing the pitch and/or the yaw i.e. rotation along the X- and/or the Y-axis.
    If you don't want to change pitch and yaw, apply the same change to the Position vector as well: use the Move()/Strafe()/Elevate() functions.
*/
const PureVector& PurePosUpTarget::getTargetVec() const
{
    return vTarget;
}

/**
    Gets Up vector.
    By default it is (0,1,0) meaning 0 degrees roll.
    Changing the Up vector results in rotation along the Z axis.
*/
PureVector& PurePosUpTarget::getUpVec()
{
    return vUp;
}

/**
    Gets Up vector.
    By default it is (0,1,0) meaning 0 degrees roll.
    Changing the Up vector results in rotation along the Z axis.
*/
const PureVector& PurePosUpTarget::getUpVec() const
{
    return vUp;
}

/**
    Moves forward or backward by the specified amount based on current direction.
*/
void PurePosUpTarget::Move(TPureFloat amount)
{
    PureVector tmpView(vTarget - vPos);
    tmpView.Normalize();
    vPos += amount * tmpView;
    vTarget += amount * tmpView;
}

/**
    Moves horizontally by the specified amount based on current direction.
*/
void PurePosUpTarget::Strafe(TPureFloat amount)
{
    PureVector tmpView(vTarget - vPos);
    tmpView.Normalize();
    PureVector tmpCross(vUp ^ tmpView);
    tmpCross.Normalize();
    vPos += amount * tmpCross;
    vTarget += amount * tmpCross;
    // todo: xz values only, not altering Y ?
}

/**
    Changes Y-position by the specified amount.
*/
void PurePosUpTarget::Elevate(TPureFloat amount)
{
    vPos.SetY( vPos.getY() + amount );
    vTarget.SetY( vTarget.getY() + amount );
}

/**
    Sets direction.
*/
void PurePosUpTarget::SetRotation(TPureFloat x, TPureFloat y, TPureFloat z)
{
    const PureVector vDefTarget(0, 0, 1);
    const PureVector vDefPos(0, 0, 0);
    // Note: we set absolute rotation here so our initial Target vec is the default in +Z direction and Pos vec is zero.
    // To implement Rotate() instead of SetRotation(), use the current vTarget and vPos vectors to calculate tmpView.
    PureVector tmpView(vDefTarget - vDefPos);  
    tmpView.Normalize();

    PureTransformMatrix matRotX, matRotY, matRotZ;
    matRotX.SetRotationX(x);
    matRotY.SetRotationY(y);
    matRotZ.SetRotationZ(z);
    // todo: 3 matrices, are u kidding me?

    tmpView *= matRotX * matRotY * matRotZ;
    tmpView.Normalize();
    vTarget = vPos + tmpView;
}


/**
    Equals to operator.
*/
TPureBool PurePosUpTarget::operator==(const PurePosUpTarget& put) const
{
    return put.vPos == vPos && put.vUp == vUp && put.vTarget == vTarget;
} // operator==()


/**
    Not equals to operator.
*/
TPureBool PurePosUpTarget::operator!=(const PurePosUpTarget& put) const
{
    return !( *this == put );
} // operator!=()


/**
    Assignment operator.
*/
PurePosUpTarget& PurePosUpTarget::operator=(const PurePosUpTarget& put)
{
    vPos = put.vPos;
    vTarget = put.vTarget;
    vUp = put.vUp;
    return *this;
} // operator=()


/**
    Write to stream operator.
*/
std::ostream& operator<< (std::ostream &s, const PurePosUpTarget& put)
{
    return ( s << "Pos" << put.getPosVec() << ", Up" << put.getUpVec() << ", Target" << put.getTargetVec() );
}


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################