#pragma once

/*
    ###################################################################################
    PureTypes.h
    This file is part of PURE.
    External header.
    Pure typedefs, enums, struct defs.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PureAllHeaders.h"


#ifndef PGENULL
#define PGENULL NULL
#endif


/**
    Number of bits in a Byte depends on the target architecture.
    Usually it is 8 bits, but to avoid false calculations, use CHAR_BIT value in climits header.
*/

/** Very small signed integer type (always 1 Byte, but remember this doesn't necessarily 8 bits). */
typedef char TPureByte;

/** Very small unsigned integer type (has same size as TPureByte). */
typedef unsigned char TPureUByte;

/** Small signed integer type (2 Bytes on a standard desktop system). */
typedef short int TPureShort;

/** Small unsigned integer type (2 Bytes on a standard desktop system). */
typedef unsigned short int TPureUShort;

/** Normal signed integer type (4 Bytes on a standard desktop system). */
typedef int TPureInt;

/** Normal unsigned integer type (4 Bytes on a standard desktop system). */
typedef unsigned int TPureUInt;

/** Long signed type (4 Bytes on a standard desktop system). */
typedef long TPureLong;

/** Long unsigned type (4 Bytes on a standard desktop system). */
typedef unsigned long TPureULong;

/** Normal signed floating point type (4 Bytes on a standard desktop system). */
typedef float TPureFloat;

/** Logical type (1 Byte on a standard desktop system). */
typedef bool TPureBool;


/**
    2D space coordinate.
*/
struct TPURE_XY
{
    TPureFloat x, y;
};

typedef TPURE_XY TXY;

struct TPURE_SIZE_XY
{
    TPureFloat width, height;
};

typedef TPURE_SIZE_XY TSXY;


/**
    3D space coordinate.
*/
struct TPURE_XYZ
{
    TPureFloat x, y, z;
}; // TPURE_XYZ

typedef TPURE_XYZ TXYZ;


/**
    4D coordinate.
*/
struct TPURE_XYZW
{
    TPureFloat x, y, z, w;
}; // TPURE_XYZW

typedef TPURE_XYZW TXYZW;


/**
    Simple RGB color [0.0-1.0].
*/
struct TPURE_RGB_FLOAT
{
    TPureFloat red;
    TPureFloat green;
    TPureFloat blue;
}; // TPURE_RGB_FLOAT

typedef TPURE_RGB_FLOAT TRGBFLOAT;


/**
    Simple RGB color [0-255].
*/
struct TPURE_RGB_UBYTE
{
    TPureUByte red;
    TPureUByte green;
    TPureUByte blue;
}; // TPURE_RGB_UBYTE

typedef TPURE_RGB_UBYTE TRGBUBYTE;


/**
    Simple RGBA color [0.0-1.0].
*/
struct TPURE_RGBA_FLOAT
{
    TPureFloat red;
    TPureFloat green;
    TPureFloat blue;
    TPureFloat alpha;
}; // TPURE_RGBA_FLOAT

typedef TPURE_RGBA_FLOAT TRGBAFLOAT;


/**
    Simple RGBA color [0-255].
*/
struct TPURE_RGBA_UBYTE
{
    TPureUByte red;
    TPureUByte green;
    TPureUByte blue;
    TPureUByte alpha;
}; // TPURE_RGBA_UBYTE

typedef TPURE_RGBA_UBYTE TRGBAUBYTE;


/**
    Simple rectangle.
*/
struct TPURE_RECT_FLOAT
{
    TXY pos;
    TSXY size;
};

typedef TPURE_RECT_FLOAT TRECT;


/**
    Display modes.
*/
enum TPURE_DISPLAY_MODES
{
    PURE_FULLSCREEN,       /**< Fullscreen mode. */
    PURE_WINDOWED          /**< Windowed mode. */
}; // TPURE_DISPLAY_MODES
