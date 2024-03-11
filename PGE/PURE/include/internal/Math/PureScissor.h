#pragma once

/*
    ###################################################################################
    PUREScissor.h
    This file is part of PURE.
    Internal header.
    Class implementing scissoring operations.
    Made by PR00F88
    ###################################################################################
*/

#include "../../external/PureAllHeaders.h"


/**
    Scissor operations class.
    Currently this class implements Cohen-Sutherland line clipping.

    Not following the Pimpl idiom here as there are no hidden members in this class for the outside world.
*/
class PUREScissor
{

#ifdef PURE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PUREScissor is included")
#endif

public:
    typedef int CS_RGNCODE;

    static const CS_RGNCODE CS_INSIDE;
    static const CS_RGNCODE CS_LEFT;
    static const CS_RGNCODE CS_RIGHT;
    static const CS_RGNCODE CS_BOTTOM;
    static const CS_RGNCODE CS_TOP;

    // ---------------------------------------------------------------------------
    
    static bool DoScissor(int& x0, int& y0, int& x1, int& y1, int xmin, int xmax, int ymin, int ymax);  /**< Runs the scissoring algorithm for the given user line. */
    

    // ---------------------------------------------------------------------------

protected:

private:

    PUREScissor::PUREScissor() {}

    PUREScissor::PUREScissor(const PUREScissor& other) { other; }

    PUREScissor& PUREScissor::operator= (const PUREScissor& other)
    {
        other;
        return *this;
    }

    virtual PUREScissor::~PUREScissor() {}

    static CS_RGNCODE ComputeCode(int x, int y, int xmin, int xmax, int ymin, int ymax);  /**< Determines the region code for the given (x,y) point. */

    static void IntersectLeft(int& x0, int& y0, int& x1, int& y1, int xmin);    /**< Calculates intersection point of left clipping line and user line. */
    static void IntersectRight(int& x0, int& y0, int& x1, int& y1, int xmax);   /**< Calculates intersection point of right clipping line and user line. */
    static void IntersectTop(int& x0, int& y0, int& x1, int& y1, int ymax);     /**< Calculates intersection point of top clipping line and user line. */
    static void IntersectBottom(int& x0, int& y0, int& x1, int& y1, int ymin);  /**< Calculates intersection point of bottom clipping line and user line. */

    static void Swap(int& a, int& b);
    static bool canTrivialReject(PUREScissor::CS_RGNCODE cp0, PUREScissor::CS_RGNCODE cp1);   /**< Based on the given region codes of 2 points, tells if the line defined by the 2 points can be trivially rejected or not. */
    static bool canTrivialAccept(PUREScissor::CS_RGNCODE cp0, PUREScissor::CS_RGNCODE cp1);   /**< Based on the given region codes of 2 points, tells if the line defined by the 2 points can be trivially accepted or not. */

}; // class PUREScissor
