#pragma once

/*
    ###################################################################################
    include/external/include/external/include/external/include/external/include/external/PureTypes.h
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
typedef char TPurebyte;

/** Very small unsigned integer type (has same size as TPurebyte). */
typedef unsigned char TPureubyte;

/** Small signed integer type (2 Bytes on a standard desktop system). */
typedef short int TPureshort;

/** Small unsigned integer type (2 Bytes on a standard desktop system). */
typedef unsigned short int TPureushort;

/** Normal signed integer type (4 Bytes on a standard desktop system). */
typedef int TPureint;

/** Normal unsigned integer type (4 Bytes on a standard desktop system). */
typedef unsigned int TPureuint;

/** Long signed type (4 Bytes on a standard desktop system). */
typedef long TPurelong;

/** Long unsigned type (4 Bytes on a standard desktop system). */
typedef unsigned long TPureulong;

/** Normal signed floating point type (4 Bytes on a standard desktop system). */
typedef float TPurefloat;

/** Logical type (1 Byte on a standard desktop system). */
typedef bool TPurebool;


/**
    2D space coordinate.
*/
struct TPure_XY
{
    TPurefloat x, y;
};

typedef TPure_XY TXY;

struct TPure_SIZE_XY
{
    TPurefloat width, height;
};

typedef TPure_SIZE_XY TSXY;


/**
    3D space coordinate.
*/
struct TPure_XYZ
{
    TPurefloat x, y, z;
}; // TPure_XYZ

typedef TPure_XYZ TXYZ;


/**
    4D coordinate.
*/
struct TPure_XYZW
{
    TPurefloat x, y, z, w;
}; // TPure_XYZW

typedef TPure_XYZW TXYZW;


/**
    Simple RGB color [0.0-1.0].
*/
struct TPure_RGB_FLOAT
{
    TPurefloat red;
    TPurefloat green;
    TPurefloat blue;
}; // TPure_RGB_FLOAT

typedef TPure_RGB_FLOAT TRGBFLOAT;


/**
    Simple RGB color [0-255].
*/
struct TPure_RGB_UBYTE
{
    TPureubyte red;
    TPureubyte green;
    TPureubyte blue;
}; // TPure_RGB_UBYTE

typedef TPure_RGB_UBYTE TRGBUBYTE;


/**
    Simple RGBA color [0.0-1.0].
*/
struct TPure_RGBA_FLOAT
{
    TPurefloat red;
    TPurefloat green;
    TPurefloat blue;
    TPurefloat alpha;
}; // TPure_RGBA_FLOAT

typedef TPure_RGBA_FLOAT TRGBAFLOAT;


/**
    Simple RGBA color [0-255].
*/
struct TPure_RGBA_UBYTE
{
    TPureubyte red;
    TPureubyte green;
    TPureubyte blue;
    TPureubyte alpha;
}; // TPure_RGBA_UBYTE

typedef TPure_RGBA_UBYTE TRGBAUBYTE;


/**
    Simple rectangle.
*/
struct TPure_RECT_FLOAT
{
    TXY pos;
    TSXY size;
};

typedef TPure_RECT_FLOAT TRECT;


/**
    Display modes.
*/
enum TPure_DISPLAY_MODES
{
    Pure_FULLSCREEN,       /**< Fullscreen mode. */
    Pure_WINDOWED          /**< Windowed mode. */
}; // TPure_DISPLAY_MODES
