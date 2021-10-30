#pragma once

/*
    ###################################################################################
    PRREPosUpTarget.h
    This file is part of PRRE.
    External header.
    PRRE class for PUT (position, up and target) vectors.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/


#include "../PRREallHeaders.h"
#include "../PRREtypes.h"
#include "PRREVector.h"
#include <sstream>


/**
    PRRE class for PUT (position, up and target) vectors.
    PRRE uses the left-handed Cartesian coordinate system.
    This means the XZ plane is horizontal, the XY and and YZ planes are vertical,
    a positive X value means right, a positive Y value means up, and a positive Z value means forward.
*/
class PRREPosUpTarget
{
#ifdef PRRE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PRREPosUpTarget is included")
#endif

public:

    // ---------------------------------------------------------------------------

    PRREPosUpTarget();                         /**< Initializes the position vector to [0,0,0], the up vector to [0,1,0] and the target vector to [0,0,1]. */

    PRREPosUpTarget(const PRREPosUpTarget& cm);

    PRREPosUpTarget(const PRREVector& pos, const PRREVector& target, const PRREVector& up);

    PRREVector& getPosVec();                    /**< Gets Position vector. */
    const PRREVector& getPosVec() const;        /**< Gets Position vector. */

    PRREVector& getTargetVec();                 /**< Gets Target vector. */
    const PRREVector& getTargetVec() const;     /**< Gets Target vector. */

    PRREVector& getUpVec();                     /**< Gets Up vector. */
    const PRREVector& getUpVec() const;         /**< Gets Up vector. */

    void Move(TPRREfloat amount);               /**< Moves forward or backward by the specified amount based on current direction. */
    void Strafe(TPRREfloat amount);             /**< Moves horizontally by the specified amount based on current direction. */
    void Elevate(TPRREfloat amount);            /**< Changes Y-position by the specified amount. */
    void SetRotation(
        TPRREfloat x,
        TPRREfloat y,
        TPRREfloat z);                          /**< Sets direction. */

    TPRREbool operator==(const PRREPosUpTarget& clr) const;  /**< Equals to operator. */
    TPRREbool operator!=(const PRREPosUpTarget& clr) const;  /**< Not equals to operator. */
    PRREPosUpTarget& operator=(const PRREPosUpTarget& vec);        /**< Assignment operator. */

private:

    // ---------------------------------------------------------------------------

    PRREVector vPos, vTarget, vUp;     /**< Position (eye), target (focus or view), up vectors. */

}; // class PRREPosUpTarget

std::ostream& operator<< (std::ostream &s, const PRREPosUpTarget& clr);  /**< Write to stream operator. */
