/*
    ###################################################################################
    PUREScissor.cpp
    This file is part of PURE.
    Class implementing scissoring operations.
    Made by PR00F88
    ###################################################################################
*/

#include "PureBaseIncludes.h"  // PCH
#include "../../include/internal/Math/PUREScissor.h"
#include "../../include/internal/PurePragmas.h"


// ############################### PUBLIC ################################


/*
    Region codes for Cohen-Sutherland line clipping algorithm.
          |      |
     1001 | 1000 | 1010
     _____|______|_____
          |      |
     0001 | 0000 | 0010
     _____|______|_____
          |      |
     0101 | 0100 | 0110
          |      |

     The sequence for reading the codes' bits is TBRL (Top, Bottom, Right, Left).
     Example: 1010 means Top Right region.
*/

const PUREScissor::CS_RGNCODE PUREScissor::CS_INSIDE = 0;  // 0000
const PUREScissor::CS_RGNCODE PUREScissor::CS_LEFT   = 1;  // 0001
const PUREScissor::CS_RGNCODE PUREScissor::CS_RIGHT  = 2;  // 0010
const PUREScissor::CS_RGNCODE PUREScissor::CS_BOTTOM = 4;  // 0100
const PUREScissor::CS_RGNCODE PUREScissor::CS_TOP    = 8;  // 1000


/**
    Runs the scissoring algorithm for the given user line.
    User line is defined by P0(x0,y0) and P1(x1,y1) points.
    P0 and P1 points might be updated by this function.
    A point can be updated after intersection calculation and it may be also swapped with the other point.
    This means that input P0 and P1 might be updated as:
    - P0' and P1,
    - P1' and P0 (swapped order compared to input),
    where P0' and P1' are calculated points of user line intersecting with a clipping line.

    @param x0   X-position of P0 point of user line.
    @param y0   Y-position of P0 point of user line.
    @param x1   X-position of P1 point of user line.
    @param y1   Y-position of P1 point of user line.
    @param xmin X-position of left vertical clipping line.
    @param ymin Y-position of bottom horizontal clipping line.
    @param xmax X-position of right vertical clipping line.
    @param ymax Y-position of top horizontal clipping line.   

    @return True if user line or part of it needs draw i.e. falls within the inside region (0000),
            false otherwise. If true, P0 and P1 points might be updated so they fit into the inside
            clipping region.
*/
bool PUREScissor::DoScissor(int& x0, int& y0, int& x1, int& y1, int xmin, int xmax, int ymin, int ymax)
{

    // Cohen-Sutherland.
    // Might be studied against parametric line clipping ( Cyrus & Beck (1978), Liang & Barsky (1984) ) from performance perspective.

    /*
    // unsure why this swap is present in my old code, theoretically this is not needed at all
    if ( x0 > x1 )
    {
        Swap(x0, x1);
        Swap(y0, y1);
    } 
    */

    CS_RGNCODE c0 = ComputeCode(x0, y0, xmin, xmax, ymin, ymax);
    CS_RGNCODE c1 = ComputeCode(x1, y1, xmin, xmax, ymin, ymax);

    while ( !( canTrivialAccept(c0, c1) || canTrivialReject(c0, c1) ) )
    {
        // either P0 or P1 is outside
        if ( c0 == 0 )
        {
            Swap(x0, x1);
            Swap(y0, y1);
            Swap(c0, c1);
        }

        // clip the line against a clip region
        if ( (c0 & CS_LEFT) == CS_LEFT )          IntersectLeft(x0, y0, x1, y1, xmin);
        else if ( (c0 & CS_RIGHT) == CS_RIGHT )   IntersectRight(x0, y0, x1, y1, xmax);
        else if ( (c0 & CS_BOTTOM) == CS_BOTTOM ) IntersectBottom(x0, y0, x1, y1, ymin);
        else if ( (c0 & CS_TOP) == CS_TOP )       IntersectTop(x0, y0, x1, y1, ymax);

        c0 = ComputeCode(x0, y0, xmin, xmax, ymin, ymax);
    }

    return canTrivialAccept(c0, c1);
} // DoScissor()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


/**
    Determines the region code for the given (x,y) point.
    @param x    X-coordinate of the given point.
    @param y    Y-coordinate of the given point.
    @param xmin X-coordinate of the left edge of the clip window.
    @param xmax X-coordinate of the right edge of the clip window.
    @param ymin Y-coordinate of the bottom edge of the clip window.
    @param ymax Y-coordinate of the top edge of the clip window.

    @return Region code for the given (x,y) point.
*/
PUREScissor::CS_RGNCODE PUREScissor::ComputeCode(int x, int y, int xmin, int xmax, int ymin, int ymax)
{
    PUREScissor::CS_RGNCODE code = CS_INSIDE;

    if (x < xmin)           // to the left of clip window
        code |= CS_LEFT;
    else if (x > xmax)      // to the right of clip window
        code |= CS_RIGHT;

    if (y < ymin)           // below the clip window
        code |= CS_BOTTOM;
    else if (y > ymax)      // above the clip window
        code |= CS_TOP;

    return code;
}

/**
    Calculates intersection point of left clipping line and user line.
    User line is defined by P0(x0,y0) and P1(x1,y1) points.
    Intersection point will be updated in (x0,y0).

    @param x0   X-position of P0 point of user line.
    @param y0   Y-position of P0 point of user line.
    @param x1   X-position of P1 point of user line.
    @param y1   Y-position of P1 point of user line.
    @param xmin X-position of left clipping line.
*/
void PUREScissor::IntersectLeft(int& x0, int& y0, int& x1, int& y1, int xmin)
{
    float m = (y1-y0) / (float)(x1-x0);
    y0 = (int) (m * (xmin-x0) + y0);
    x0 = xmin;
}

/**
    Calculates intersection point of right clipping line and user line.
    User line is defined by P0(x0,y0) and P1(x1,y1) points.
    Intersection point will be updated in (x0,y0).

    @param x0   X-position of P0 point of user line.
    @param y0   Y-position of P0 point of user line.
    @param x1   X-position of P1 point of user line.
    @param y1   Y-position of P1 point of user line.
    @param xmax X-position of right clipping line.
*/
void PUREScissor::IntersectRight(int& x0, int& y0, int& x1, int& y1, int xmax)
{
    IntersectLeft(x0, y0, x1, y1, xmax);
}

/**
    Calculates intersection point of top clipping line and user line.
    User line is defined by P0(x0,y0) and P1(x1,y1) points.
    Intersection point will be updated in (x0,y0).

    @param x0   X-position of P0 point of user line.
    @param y0   Y-position of P0 point of user line.
    @param x1   X-position of P1 point of user line.
    @param y1   Y-position of P1 point of user line.
    @param ymax Y-position of top clipping line.
*/
void PUREScissor::IntersectTop(int& x0, int& y0, int& x1, int& y1, int ymax)
{
    float m = (x1-x0) / (float)(y1-y0);
    x0 = (int) (m * (ymax-y0) + x0);
    y0 = ymax;
}

/**
    Calculates intersection point of bottom clipping line and user line.
    User line is defined by P0(x0,y0) and P1(x1,y1) points.
    Intersection point will be updated in (x0,y0).

    @param x0   X-position of P0 point of user line.
    @param y0   Y-position of P0 point of user line.
    @param x1   X-position of P1 point of user line.
    @param y1   Y-position of P1 point of user line.
    @param ymin Y-position of bottom clipping line.
*/
void PUREScissor::IntersectBottom(int& x0, int& y0, int& x1, int& y1, int ymin)
{
    IntersectTop(x0, y0, x1, y1, ymin);
}


void PUREScissor::Swap(int& a, int& b)
{
    int tmp = a;
    a = b;
    b = tmp;
}

/**
    Based on the given region codes of 2 points, tells if the line defined by the 2 points can be trivially rejected or not.

    @return True if the line can be trivially rejected, otherwise false.
*/
bool PUREScissor::canTrivialReject(PUREScissor::CS_RGNCODE cp0, PUREScissor::CS_RGNCODE cp1)
{
    return (cp0 & cp1) != 0;
}

/**
    Based on the given region codes of 2 points, tells if the line defined by the 2 points can be trivially accepted or not.

    @return True if the line can be trivially accepted, otherwise false.
*/
bool PUREScissor::canTrivialAccept(PUREScissor::CS_RGNCODE cp0, PUREScissor::CS_RGNCODE cp1)
{
    return (cp0 | cp1) == 0;
}
