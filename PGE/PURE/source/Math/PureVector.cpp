/*
    ###################################################################################
    PureVector.cpp
    This file is part of PURE.
    Pure 3D-vector class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/


#include "PureBaseIncludes.h"  // PCH
#include "../../include/external/Math/PureVector.h"
#include "../../include/internal/PurePragmas.h"


// ############################### PUBLIC ################################


/**
    Creates a vector containing zeros.
    The 4th component of the vector (w) is initialized to 1.0f.
*/
PureVector::PureVector()
{
    pos.x = 0.0f;
    pos.y = 0.0f;
    pos.z = 0.0f;
    pos.w = 1.0f;
} // PureVector()


PureVector::PureVector(const PureVector& vec) :
    pos(vec.pos)
{}


/**
    Creates a nullvector.
    The 4th component of the vector (w) is initialized to 1.0f.
*/
PureVector::PureVector(const TPureFloat& x, const TPureFloat& y, const TPureFloat& z)
{
    pos.x = x;
    pos.y = y;
    pos.z = z;
    pos.w = 1.0f;
} // PureVector(...)


PureVector::~PureVector()
{

} // ~PureVector()


/**
    Gets the X-coordinate.

    @return X-coordinate of the vector.
*/
TPureFloat PureVector::getX() const
{
    return pos.x;
} // getX()


/**
    Gets the Y-coordinate.

    @return Y-coordinate of the vector.
*/
TPureFloat PureVector::getY() const
{
    return pos.y;
} // getY()


/**
    Gets the Z-coordinate.

    @return Z-coordinate of the vector.
*/
TPureFloat PureVector::getZ() const
{
    return pos.z;
} // getZ()


/**
    Gets the W-coordinate.

    @return W-coordinate of the vector.
*/
TPureFloat PureVector::getW() const
{
    return pos.w;
} // getZ()


/**
    Gets all the coordinates.

    @return XYZW-coordinates of the vector.
*/
const TPURE_XYZW& PureVector::getXYZW() const
{
    return pos;
}


/**
    Gets the value at the given index.
    @param index 0 means X, 1 means Y, 2 means Z, 3 means W coordinate of the vector.

    @return Coordinate at the given index. 0.0f when specifying wrong index.
*/
TPureFloat PureVector::get(TPureByte index) const
{
    if ( correctIndex(index) )
    {
        switch ( index )
        {
        case 0: return pos.x; break;
        case 1: return pos.y; break;
        case 2: return pos.z; break;
        case 3: return pos.w; break;
        }
    }
    return 0.0f;
} // get()


/**
    Sets the X-coordinate.

    @param x What X-coordinate to be set.
*/
void PureVector::SetX(TPureFloat x)
{
    pos.x = x;
} // X()


/**
    Sets the Y-coordinate.

    @param y What Y-coordinate to be set.
*/
void PureVector::SetY(TPureFloat y)
{
    pos.y = y;
} // Y()


/**
    Sets the Z-coordinate.

    @param z What Z-coordinate to be set.
*/
void PureVector::SetZ(TPureFloat z)
{
    pos.z = z;
} // Z()


/**
    Sets the value at the given index.
    No effect when specifying invalid index.

    @param index 0 means X, 1 means Y, 2 means Z, 3 means W coordinate of the vector.
    @param value Coordinate value to be set.
*/
void PureVector::Set(TPureByte index, TPureFloat value)
{
    if ( correctIndex(index) )
    {
        switch (index)
        {
        case 0: pos.x = value; break;
        case 1: pos.y = value; break;
        case 2: pos.z = value; break;
        case 3: pos.w = value; break;
        }
    }
} // Set()


/**
    Sets the given values.

    @param x What X-coordinate to be set.
    @param y What Y-coordinate to be set.
    @param z What Z-coordinate to be set.
*/
void PureVector::Set(TPureFloat x, TPureFloat y, TPureFloat z)
{
    pos.x = x;
    pos.y = y;
    pos.z = z;
} // Set()


/**
    Gets if the vector is nullvector.
    Nullvector is a vector having its XYZ coordinates all zero.

    @return True if vector is nullvector, false otherwise.
*/
TPureBool PureVector::isZero() const
{
    return ( abs(pos.x) < PFL::E ) && ( abs(pos.y) < PFL::E ) && ( abs(pos.z) < PFL::E );
} // isZero()


/**
    Sets the vector to be a nullvector.
    Nullvector is a vector having its XYZ coordinates all zero.
*/
void PureVector::SetZero()
{
    pos.x = 0;
    pos.y = 0;
    pos.z = 0;
    pos.w = 1;
} // SetZero()


/**
    Gets the length.

    @return Length of the vector.
*/
TPureFloat PureVector::getLength() const
{
    return sqrt( pos.x*pos.x + pos.y*pos.y + pos.z*pos.z );
} // getLength()


/**
    Normalizes the coordinates.
    Normalizing a vector means we keep the vector's direction but it will be a unit vector so its length/magnitude becomes 1.
    By normalizing 2 vectors looking at the exactly same direction but with different coordinates/length/magnitude, they become equal.
*/
void PureVector::Normalize()
{
    // more explanation: https://stackoverflow.com/questions/10002918/what-is-the-need-for-normalizing-a-vector

    const TPureFloat len = getLength();
    if ( abs(len) >= PFL::E )
    {
        pos.x = pos.x / len;
        pos.y = pos.y / len;
        pos.z = pos.z / len;
    }
} // Normalize()


/**
    Dot product.
    The dot product between two unit vectors is the cosine of the angle between those two vectors.
    So it is useful to tell if 2 vectors are looking into same or similar direction, or opposite direction, etc.
    
    It is also useful when we need to sort objects in front-to-back or back-to-front order relative to camera position, because
    we can calculate the scalar projection of vector 'a' on vector 'b' if we have the dot product of vector 'a' and
    a unit vector in the direction of vector 'b':
     - vector 'a' can be calculated by subtracting camera position vector from object position vector;
     - vector 'b' can be calculated by subtracting camera position vector from camera target vector, its normalized form then
       will be a unit vector in the direction of where camera is looking at.
    Then the only task to do is to sort based on the calculated scalar projections.

    @param vec The other vector used for the calculation with this vector.

    @return Dot product of the 2 vectors.
            1 if they look into exactly same direction, 0 when the 2 vectors are perpendicular to each other, -1 if they look into opposite direction.
*/
TPureFloat PureVector::getDotProduct(const PureVector& vec) const
{
    // more explanation: https://stackoverflow.com/questions/10002918/what-is-the-need-for-normalizing-a-vector
    return pos.x*vec.pos.x + pos.y*vec.pos.y + pos.z*vec.pos.z;
} // getDotProduct()


/**
    Cross product.
    Cross product is a vector which is perpendicular to both input vectors. It means this is a normalvector to the plane containing both input vectors.

    @param vec The other vector used for the calculation with this vector.

    @return Cross product of the 2 vectors.
            Zero when the 2 vectors are parallel or antiparallel to each other, or one of the vectors is nullvector.
*/
PureVector PureVector::getCrossProduct(const PureVector& vec) const
{
    return PureVector( pos.y*vec.pos.z - pos.z*vec.pos.y,
                       pos.z*vec.pos.x - pos.x*vec.pos.z,
                       pos.x*vec.pos.y - pos.y*vec.pos.x );
} // getCrossProduct()


/**
    Equals to operator.
*/
TPureBool PureVector::operator==(const PureVector& vec) const
{
    const bool bX = abs(pos.x - vec.pos.x) < PFL::E;
    const bool bY = abs(pos.y - vec.pos.y) < PFL::E;
    const bool bZ = abs(pos.z - vec.pos.z) < PFL::E;

    return ( bX && bY && bZ );
} // operator==()


/**
    Not equals to operator.
*/
TPureBool PureVector::operator!=(const PureVector& vec) const
{
    return !( *this == vec );
} // operator!=()


/**
    Assignment operator.
*/
PureVector& PureVector::operator=(const PureVector& vec)
{
    pos = vec.pos;
    return *this;
} // operator=()


/**
    Addition operator.
*/
PureVector PureVector::operator+(const PureVector& vec) const
{
    return PureVector(pos.x + vec.pos.x, pos.y + vec.pos.y, pos.z + vec.pos.z);
} // operator+()


/**
    Addition assignment operator.
*/
PureVector& PureVector::operator+=(const PureVector& vec)
{
    pos.x += vec.pos.x;
    pos.y += vec.pos.y;
    pos.z += vec.pos.z;
    return *this;
} // operator+=()


/**
    Subtraction operator.
*/
PureVector PureVector::operator-(const PureVector& vec) const
{
    return PureVector(pos.x - vec.pos.x, pos.y - vec.pos.y, pos.z - vec.pos.z);
} // operator-()


/**
    Subtraction assignment operator.
*/
PureVector& PureVector::operator-=(const PureVector& vec)
{
    pos.x -= vec.pos.x;
    pos.y -= vec.pos.y;
    pos.z -= vec.pos.z;
    return *this;
} // operator-=()


/**
    Multiplication by scalar operator.
*/
PureVector PureVector::operator*(const TPureFloat& scalar) const
{
    const PureVector newvec(pos.x*scalar, pos.y*scalar, pos.z*scalar);
    return newvec;
} // operator*()


/**
    Multiplication assignment operator.
*/
PureVector& PureVector::operator*=(const TPureFloat& scalar)
{
    pos.x *= scalar;
    pos.y *= scalar;
    pos.z *= scalar;
    return *this;
} // operator*=()


/**
    Division by scalar operator.
*/
PureVector PureVector::operator/(const TPureFloat& scalar) const
{
    const PureVector newvec(pos.x / scalar, pos.y / scalar, pos.z / scalar);
    return newvec;
}


/**
    Division assignment operator.
*/
PureVector& PureVector::operator/=(const TPureFloat& scalar)
{
    pos.x /= scalar;
    pos.y /= scalar;
    pos.z /= scalar;
    return *this;
}


/**
    Dot product operator.
    See details at getDotProduct().
*/
TPureFloat PureVector::operator*(const PureVector& vec) const
{
    return getDotProduct(vec);
} // operator*()


/**
    Cross product operator.
    See details at getCrossProduct().
*/
PureVector PureVector::operator^(const PureVector& vec) const
{
    return getCrossProduct(vec);
} // operator^()


/**
    Cross product assignment operator.
*/
PureVector& PureVector::operator^=(const PureVector& vec)
{
    TPURE_XYZW newpos;
    newpos.x = pos.y*vec.pos.z - pos.z*vec.pos.y;
    newpos.y = pos.z*vec.pos.x - pos.x*vec.pos.z;
    newpos.z = pos.x*vec.pos.y - pos.y*vec.pos.x;
    pos = newpos;
    return *this;
} // operator^=()


/**
    Access element operator.

    @param index 0 means X, 1 means Y, 2 means Z, 3 means W coordinate of the vector.
*/
TPureFloat& PureVector::operator()(const TPureByte& index)
{
    if ( correctIndex(index) )
    {
        switch ( index )
        {
        case 0: return pos.x; break;
        case 1: return pos.y; break;
        case 2: return pos.z; break;
        case 3: return pos.w; break;
        }
    }
    return pos.x;    
} // operator()()


/**
    Access element operator.

    @param index 0 means X, 1 means Y, 2 means Z, 3 means W coordinate of the vector.
*/
const TPureFloat& PureVector::operator()(const TPureByte& index) const
{
    if ( correctIndex(index) )
    {
        switch ( index )
        {
        case 0: return pos.x; break;
        case 1: return pos.y; break;
        case 2: return pos.z; break;
        case 3: return pos.w; break;
        }
    }
    return pos.x;    
} // operator()()


/**
    Access element operator.

    @param index 0 means X, 1 means Y, 2 means Z, 3 means W coordinate of the vector.
*/
TPureFloat& PureVector::operator[](const TPureByte& index)
{
    return (*this)(index);    
} // operator[]()


/**
    Access element operator.

    @param index 0 means X, 1 means Y, 2 means Z, 3 means W coordinate of the vector.
*/
const TPureFloat& PureVector::operator[](const TPureByte& index) const
{
    return (*this)(index);    
} // operator[]()


/**
    Comparison operator "less than".
*/
TPureBool PureVector::operator<(const PureVector& r) const
{
    if ( getX() < r.getX() )
    {
        return true;
    }
    else if ( getX() == r.getX() )
    {
        if ( getY() < r.getY() )
        {
            return true;
        }
        else if ( getY() == r.getY() )
        {
            if ( getZ() < r.getZ() ) 
            {
                return true;
            }
        }
    }
    return false;
}


/**
    Comparison operator "greater than".
*/
TPureBool PureVector::operator>(const PureVector& r) const
{
    return r < *this;
}


/**
    Comparison operator "less than or equal".
*/
TPureBool PureVector::operator<=(const PureVector& r) const
{
    return !(*this > r);
}


/**
    Comparison operator "greater than or equal".
*/
TPureBool PureVector::operator>=(const PureVector& r) const
{
    return !(*this < r);
}


/**
    Negative operator.
*/
PureVector operator-(const PureVector& vec)
{
    return PureVector(-vec.getX(), -vec.getY(), -vec.getZ());
} // operator-()


/**
    Multiplication by scalar operator.
*/
PureVector operator*(const TPureFloat& scalar, const PureVector& vec)
{
    return vec * scalar;
} // operator*()


/**
    Write to stream operator.
*/
std::ostream& operator<< (std::ostream &s, const PureVector& vec)
{
    return ( s << "[" << vec.getX() << "," << vec.getY() << "," << vec.getZ() << "," << vec.get(3) << "]" );
}


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


bool PureVector::correctIndex(int index)
{
    return index >= 0 && index < 4;
}

