#pragma once

/*
    ###################################################################################
    PurePosUpTarget.h
    This file is part of PURE.
    External header.
    Pure class for PUT (position, up and target) vectors.
    Made by PR00F88
    ###################################################################################
*/


#include "../PureAllHeaders.h"
#include "../PureTypes.h"
#include "PureVector.h"
#include <sstream>


/**
    PURE class for PUT (position, up and target) vectors.
    PURE uses the left-handed Cartesian coordinate system.
    This means the XZ plane is horizontal, the XY and and YZ planes are vertical,
    a positive X value means right, a positive Y value means up, and a positive Z value means forward.
*/
class PurePosUpTarget
{
#ifdef PURE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PurePosUpTarget is included")
#endif

public:

    // ---------------------------------------------------------------------------

    PurePosUpTarget();                         /**< Initializes the position vector to [0,0,0], the up vector to [0,1,0] and the target vector to [0,0,1]. */

    PurePosUpTarget(const PurePosUpTarget& cm);

    PurePosUpTarget(const PureVector& pos, const PureVector& target, const PureVector& up);

    PureVector& getPosVec();                    /**< Gets Position vector. */
    const PureVector& getPosVec() const;        /**< Gets Position vector. */

    PureVector& getTargetVec();                 /**< Gets Target vector. */
    const PureVector& getTargetVec() const;     /**< Gets Target vector. */

    PureVector& getUpVec();                     /**< Gets Up vector. */
    const PureVector& getUpVec() const;         /**< Gets Up vector. */

    void Move(TPureFloat amount);               /**< Moves forward or backward by the specified amount based on current direction. */
    void Strafe(TPureFloat amount);             /**< Moves horizontally by the specified amount based on current direction. */
    void Elevate(TPureFloat amount);            /**< Changes Y-position by the specified amount. */
    void SetRotation(
        TPureFloat x,
        TPureFloat y,
        TPureFloat z);                          /**< Sets direction. */

    TPureBool operator==(const PurePosUpTarget& clr) const;  /**< Equals to operator. */
    TPureBool operator!=(const PurePosUpTarget& clr) const;  /**< Not equals to operator. */
    PurePosUpTarget& operator=(const PurePosUpTarget& vec);  /**< Assignment operator. */

private:

    // ---------------------------------------------------------------------------

    PureVector vPos, vTarget, vUp;     /**< Position (eye), target (focus or view), up vectors. */

}; // class PurePosUpTarget

std::ostream& operator<< (std::ostream &s, const PurePosUpTarget& clr);  /**< Write to stream operator. */
