/*
    ###################################################################################
    PRREVector.cpp
    This file is part of PRRE.
    PRRE 3D-vector class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/


#include "PRREbaseIncludes.h"  // PCH
#include "../../include/external/Math/PRREVector.h"
#include "../../include/internal/PRREpragmas.h"


// ############################### PUBLIC ################################


/**
    Creates a vector containing zeros.
    The 4th component of the vector (w) is initialized to 1.0f.
*/
PRREVector::PRREVector()
{
    pos.x = 0.0f;
    pos.y = 0.0f;
    pos.z = 0.0f;
    pos.w = 1.0f;
} // PRREVector()


PRREVector::PRREVector(const PRREVector& vec) :
    pos(vec.pos)
{}


/**
    Creates a nullvector.
    The 4th component of the vector (w) is initialized to 1.0f.
*/
PRREVector::PRREVector(const TPRREfloat& x, const TPRREfloat& y, const TPRREfloat& z)
{
    pos.x = x;
    pos.y = y;
    pos.z = z;
    pos.w = 1.0f;
} // PRREVector(...)


PRREVector::~PRREVector()
{

} // ~PRREVector()


/**
    Gets the X-coordinate.

    @return X-coordinate of the vector.
*/
TPRREfloat PRREVector::getX() const
{
    return pos.x;
} // getX()


/**
    Gets the Y-coordinate.

    @return Y-coordinate of the vector.
*/
TPRREfloat PRREVector::getY() const
{
    return pos.y;
} // getY()


/**
    Gets the Z-coordinate.

    @return Z-coordinate of the vector.
*/
TPRREfloat PRREVector::getZ() const
{
    return pos.z;
} // getZ()


/**
    Gets all the coordinates.

    @return XYZW-coordinates of the vector.
*/
const TPRRE_XYZW& PRREVector::getXYZW() const
{
    return pos;
}


/**
    Gets the value at the given index.
    @param index 0 means X, 1 means Y, 2 means Z, 3 means W coordinate of the vector.

    @return Coordinate at the given index. 0.0f when specifying wrong index.
*/
TPRREfloat PRREVector::get(TPRREbyte index) const
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
void PRREVector::SetX(TPRREfloat x)
{
    pos.x = x;
} // X()


/**
    Sets the Y-coordinate.

    @param y What Y-coordinate to be set.
*/
void PRREVector::SetY(TPRREfloat y)
{
    pos.y = y;
} // Y()


/**
    Sets the Z-coordinate.

    @param z What Z-coordinate to be set.
*/
void PRREVector::SetZ(TPRREfloat z)
{
    pos.z = z;
} // Z()


/**
    Sets the value at the given index.
    No effect when specifying invalid index.

    @param index 0 means X, 1 means Y, 2 means Z, 3 means W coordinate of the vector.
    @param value Coordinate value to be set.
*/
void PRREVector::Set(TPRREbyte index, TPRREfloat value)
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
void PRREVector::Set(TPRREfloat x, TPRREfloat y, TPRREfloat z)
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
TPRREbool PRREVector::isZero() const
{
    return ( abs(pos.x) < PFL::E ) && ( abs(pos.y) < PFL::E ) && ( abs(pos.z) < PFL::E );
} // isZero()


/**
    Sets the vector to be a nullvector.
    Nullvector is a vector having its XYZ coordinates all zero.
*/
void PRREVector::SetZero()
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
TPRREfloat PRREVector::getLength() const
{
    return sqrt( pos.x*pos.x + pos.y*pos.y + pos.z*pos.z );
} // getLength()


/**
    Normalizes the coordinates.
    Normalizing a vector means we keep the vector's direction but it will be a unit vector so its length/magnitude becomes 1.
    By normalizing 2 vectors looking at the exactly same direction but with different coordinates/length/magnitude, they become equal.
*/
void PRREVector::Normalize()
{
    // more explanation: https://stackoverflow.com/questions/10002918/what-is-the-need-for-normalizing-a-vector

    const TPRREfloat len = getLength();
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

    @param vec The other vector used for the calculation with this vector.

    @return Dot product of the 2 vectors.
            -1 if they look into exactly same direction, 0 when the 2 vectors are perpendicular to each other, -1 if they look into opposite direction.
*/
TPRREfloat PRREVector::getDotProduct(const PRREVector& vec) const
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
PRREVector PRREVector::getCrossProduct(const PRREVector& vec) const
{
    return PRREVector( pos.y*vec.pos.z - pos.z*vec.pos.y,
                       pos.z*vec.pos.x - pos.x*vec.pos.z,
                       pos.x*vec.pos.y - pos.y*vec.pos.x );
} // getCrossProduct()


/**
    Equals to operator.
*/
TPRREbool PRREVector::operator==(const PRREVector& vec) const
{
    const bool bX = (pos.x - vec.pos.x) < PFL::E;
    const bool bY = (pos.y - vec.pos.y) < PFL::E;
    const bool bZ = (pos.z - vec.pos.z) < PFL::E;

    return ( bX && bY && bZ );
} // operator==()


/**
    Not equals to operator.
*/
TPRREbool PRREVector::operator!=(const PRREVector& vec) const
{
    return !( *this == vec );
} // operator!=()


/**
    Assignment operator.
*/
PRREVector& PRREVector::operator=(const PRREVector& vec)
{
    pos = vec.pos;
    return *this;
} // operator=()


/**
    Addition operator.
*/
PRREVector PRREVector::operator+(const PRREVector& vec) const
{
    return PRREVector(pos.x + vec.pos.x, pos.y + vec.pos.y, pos.z + vec.pos.z);
} // operator+()


/**
    Addition assignment operator.
*/
PRREVector& PRREVector::operator+=(const PRREVector& vec)
{
    pos.x += vec.pos.x;
    pos.y += vec.pos.y;
    pos.z += vec.pos.z;
    return *this;
} // operator+=()


/**
    Subtraction operator.
*/
PRREVector PRREVector::operator-(const PRREVector& vec) const
{
    return PRREVector(pos.x - vec.pos.x, pos.y - vec.pos.y, pos.z - vec.pos.z);
} // operator-()


/**
    Subtraction assignment operator.
*/
PRREVector& PRREVector::operator-=(const PRREVector& vec)
{
    pos.x -= vec.pos.x;
    pos.y -= vec.pos.y;
    pos.z -= vec.pos.z;
    return *this;
} // operator-=()


/**
    Multiplication by scalar operator.
*/
PRREVector PRREVector::operator*(const TPRREfloat& scalar) const
{
    const PRREVector newvec(pos.x*scalar, pos.y*scalar, pos.z*scalar);
    return newvec;
} // operator*()


/**
    Multiplication assignmennt operator.
*/
PRREVector& PRREVector::operator*=(const TPRREfloat& scalar)
{
    pos.x *= scalar;
    pos.y *= scalar;
    pos.z *= scalar;
    return *this;
} // operator*=()


/**
    Dot product operator.
    See details at getDotProduct().
*/
TPRREfloat PRREVector::operator*(const PRREVector& vec) const
{
    return getDotProduct(vec);
} // operator*()


/**
    Cross product operator.
    See details at getCrossProduct().
*/
PRREVector PRREVector::operator^(const PRREVector& vec) const
{
    return getCrossProduct(vec);
} // operator^()


/**
    Cross product assignment operator.
*/
PRREVector& PRREVector::operator^=(const PRREVector& vec)
{
    TPRRE_XYZW newpos;
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
TPRREfloat& PRREVector::operator()(const TPRREbyte& index)
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
const TPRREfloat& PRREVector::operator()(const TPRREbyte& index) const
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
TPRREfloat& PRREVector::operator[](const TPRREbyte& index)
{
    return (*this)(index);    
} // operator[]()


/**
    Access element operator.

    @param index 0 means X, 1 means Y, 2 means Z, 3 means W coordinate of the vector.
*/
const TPRREfloat& PRREVector::operator[](const TPRREbyte& index) const
{
    return (*this)(index);    
} // operator[]()


/**
    Negative operator.
*/
PRREVector operator-(const PRREVector& vec)
{
    return PRREVector(-vec.getX(), -vec.getY(), -vec.getZ());
} // operator-()


/**
    Multiplication by scalar operator.
*/
PRREVector operator*(const TPRREfloat& scalar, const PRREVector& vec)
{
    return vec * scalar;
} // operator*()


/**
    Write to stream operator.
*/
std::ostream& operator<< (std::ostream &s, const PRREVector& vec)
{
    return ( s << "[" << vec.getX() << "," << vec.getY() << "," << vec.getZ() << "," << vec.get(3) << "]" );
}


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


bool PRREVector::correctIndex(int index)
{
    return index >= 0 && index < 4;
}

