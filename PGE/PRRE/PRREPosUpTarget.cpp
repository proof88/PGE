/*
    ###################################################################################
    PRREPosUpTarget.cpp
    This file is part of PRRE.
    PRRE class for PUT (position, up and target) vectors.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREbaseIncludes.h"  // PCH
#include "PRREpragmas.h"
#include "PRREPosUpTarget.h"
#include "PRRETransformMatrix.h"


// ############################### PUBLIC ################################

/**
    Initializes the position vector to [0,0,0], the up vector to [0,1,0] and the target vector to [0,0,1].
*/
PRREPosUpTarget::PRREPosUpTarget() :
    vUp(0.0f, 1.0f, 0.0f),
    vTarget(0.0f, 0.0f, 1.0f)
{
}


PRREPosUpTarget::PRREPosUpTarget(const PRREPosUpTarget& put) :
    vPos(put.vPos),
    vTarget(put.vTarget),
    vUp(put.vUp)
{}


/**
    Initializes the position, up and target vectors with the given vectors.
*/
PRREPosUpTarget::PRREPosUpTarget(const PRREVector& pos, const PRREVector& target, const PRREVector& up) :
    vPos(pos),
    vTarget(target),
    vUp(up)
{
}

/**
    Gets Position vector.
 */
PRREVector& PRREPosUpTarget::getPosVec()
{
    return vPos;
}

/**
    Gets Position vector.
*/
const PRREVector& PRREPosUpTarget::getPosVec() const
{
    return vPos;
}

/**
    Gets Target vector.
*/
PRREVector& PRREPosUpTarget::getTargetVec()
{
    return vTarget;
}

/**
    Gets Target vector.
*/
const PRREVector& PRREPosUpTarget::getTargetVec() const
{
    return vTarget;
}

/**
    Gets Up vector.
*/
PRREVector& PRREPosUpTarget::getUpVec()
{
    return vUp;
}

/**
    Gets Up vector.
*/
const PRREVector& PRREPosUpTarget::getUpVec() const
{
    return vUp;
}

/**
    Moves forward or backward by the specified amount based on current direction.
*/
void PRREPosUpTarget::Move(TPRREfloat amount)
{
    PRREVector tmpView(vTarget - vPos);
    tmpView.Normalize();
    vPos += amount * tmpView;
    vTarget += amount * tmpView;
}

/**
    Moves horizontally by the specified amount based on current direction.
*/
void PRREPosUpTarget::Strafe(TPRREfloat amount)
{
    PRREVector tmpView(vTarget - vPos);
    tmpView.Normalize();
    PRREVector tmpCross(vUp ^ tmpView);
    tmpCross.Normalize();
    vPos += amount * tmpCross;
    vTarget += amount * tmpCross;
    // todo: xz values only, not altering Y ?
}

/**
    Changes Y-position by the specified amount.
*/
void PRREPosUpTarget::Elevate(TPRREfloat amount)
{
    vPos.SetY( vPos.getY() + amount );
    vTarget.SetY( vTarget.getY() + amount );
}

/**
    Sets direction.
*/
void PRREPosUpTarget::SetRotation(TPRREfloat x, TPRREfloat y, TPRREfloat z)
{
    const PRREVector vDefTarget(0, 0, 1);
    const PRREVector vDefPos(0, 0, 0);
    // Note: we set absolute rotation here so our initial Target vec is the default in +Z direction and Pos vec is zero.
    // To implement Rotate() instead of SetRotation(), use the current vTarget and vPos vectors to calculate tmpView.
    PRREVector tmpView(vDefTarget - vDefPos);  
    tmpView.Normalize();

    PRRETransformMatrix matRotX, matRotY, matRotZ;
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
TPRREbool PRREPosUpTarget::operator==(const PRREPosUpTarget& put) const
{
    return put.vPos == vPos && put.vUp == vUp && put.vTarget == vTarget;
} // operator==()


/**
    Not equals to operator.
*/
TPRREbool PRREPosUpTarget::operator!=(const PRREPosUpTarget& put) const
{
    return !( *this == put );
} // operator!=()


/**
    Assignment operator.
*/
PRREPosUpTarget& PRREPosUpTarget::operator=(const PRREPosUpTarget& put)
{
    vPos = put.vPos;
    vTarget = put.vTarget;
    vUp = put.vUp;
    return *this;
} // operator=()


/**
    Write to stream operator.
*/
std::ostream& operator<< (std::ostream &s, const PRREPosUpTarget& put)
{
    return ( s << "Pos" << put.getPosVec() << ", Up" << put.getUpVec() << ", Target" << put.getTargetVec() );
}


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################