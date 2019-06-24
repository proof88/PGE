#pragma once

/*
    ###################################################################################
    PRREtypes.h
    This file is part of PRRE.
    External header.
    PRRE typedefs, enums, struct defs.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREallHeaders.h"


#ifndef PGENULL
#define PGENULL NULL
#endif


/**
    Number of bits in a Byte depends on the target architecture.
    Usually it is 8 bits, but to avoid false calculations, use CHAR_BIT value in climits header.
*/

/** Very small signed integer type (always 1 Byte, but remember this doesn't necessarily 8 bits). */
typedef char TPRREbyte;

/** Very small unsigned integer type (has same size as TPRREbyte). */
typedef unsigned char TPRREubyte;

/** Small signed integer type (2 Bytes on a standard desktop system). */
typedef short int TPRREshort;

/** Small unsigned integer type (2 Bytes on a standard desktop system). */
typedef unsigned short int TPRREushort;

/** Normal signed integer type (4 Bytes on a standard desktop system). */
typedef int TPRREint;

/** Normal unsigned integer type (4 Bytes on a standard desktop system). */
typedef unsigned int TPRREuint;

/** Long signed type (4 Bytes on a standard desktop system). */
typedef long TPRRElong;

/** Long unsigned type (4 Bytes on a standard desktop system). */
typedef unsigned long TPRREulong;

/** Normal signed floating point type (4 Bytes on a standard desktop system). */
typedef float TPRREfloat;

/** Logical type (1 Byte on a standard desktop system). */
typedef bool TPRREbool;


/**
    2D space coordinate.
*/
struct TPRRE_XY
{
    TPRREfloat x, y;
};

typedef TPRRE_XY TXY;

struct TPRRE_SIZE_XY
{
    TPRREfloat width, height;
};

typedef TPRRE_SIZE_XY TSXY;


/**
    3D space coordinate.
*/
struct TPRRE_XYZ
{
    TPRREfloat x, y, z;
}; // TPRRE_XYZ

typedef TPRRE_XYZ TXYZ;


/**
    4D coordinate.
*/
struct TPRRE_XYZW
{
    TPRREfloat x, y, z, w;
}; // TPRRE_XYZW

typedef TPRRE_XYZW TXYZW;


/**
    Simple RGB color [0.0-1.0].
*/
struct TPRRE_RGB_FLOAT
{
    TPRREfloat red;
    TPRREfloat green;
    TPRREfloat blue;
}; // TPRRE_RGB_FLOAT

typedef TPRRE_RGB_FLOAT TRGBFLOAT;


/**
    Simple RGB color [0-255].
*/
struct TPRRE_RGB_UBYTE
{
    TPRREubyte red;
    TPRREubyte green;
    TPRREubyte blue;
}; // TPRRE_RGB_UBYTE

typedef TPRRE_RGB_UBYTE TRGBUBYTE;


/**
    Simple RGBA color [0.0-1.0].
*/
struct TPRRE_RGBA_FLOAT
{
    TPRREfloat red;
    TPRREfloat green;
    TPRREfloat blue;
    TPRREfloat alpha;
}; // TPRRE_RGBA_FLOAT

typedef TPRRE_RGBA_FLOAT TRGBAFLOAT;


/**
    Simple RGBA color [0-255].
*/
struct TPRRE_RGBA_UBYTE
{
    TPRREubyte red;
    TPRREubyte green;
    TPRREubyte blue;
    TPRREubyte alpha;
}; // TPRRE_RGBA_UBYTE

typedef TPRRE_RGBA_UBYTE TRGBAUBYTE;


/**
    Simple rectangle.
*/
struct TPRRE_RECT_FLOAT
{
    TXY pos;
    TSXY size;
};

typedef TPRRE_RECT_FLOAT TRECT;


/**
    Display modes.
*/
enum TPRRE_DISPLAY_MODES
{
    PRRE_FULLSCREEN,       /**< Fullscreen mode. */
    PRRE_WINDOWED          /**< Windowed mode. */
}; // TPRRE_DISPLAY_MODES
