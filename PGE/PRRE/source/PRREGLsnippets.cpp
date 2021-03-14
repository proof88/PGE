/*
    ###################################################################################
    PRREGLsnippets.cpp
    This file is part of PRRE.
    PRRE useful GL functions.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREbaseIncludes.h"  // PCH
#include "../include/internal/PRREGLsnippets.h"
#include "../include/internal/PRREpragmas.h"


// ############################### PUBLIC ################################


/**
    Gets the string representation of the given GL error.

    @param err OpenGL error code. These are defined in GL.h.

    @return A string representing the given GL error.
*/
const char* PRREGLsnippets::getGLerrorTextFromEnum(GLenum err)
{
    switch (err)
    {
    case GL_NO_ERROR          : return GL_ERR_STR_NO_ERROR;
    case GL_INVALID_ENUM      : return GL_ERR_STR_INVALID_ENUM;
    case GL_INVALID_VALUE     : return GL_ERR_STR_INVALID_VALUE;
    case GL_INVALID_OPERATION : return GL_ERR_STR_INVALID_OPERATION;
    case GL_STACK_OVERFLOW    : return GL_ERR_STR_STACK_OVERFLOW;
    case GL_STACK_UNDERFLOW   : return GL_ERR_STR_STACK_UNDERFLOW;
    case GL_OUT_OF_MEMORY     : return GL_ERR_STR_OUT_OF_MEMORY;
    default                   : return "Unknown Error";
    }
} // getGLerrorTextFromEnum()


/**
    Gets whether there is an error in OpenGL or not.
    The error gets saved by this function and can be further evaluated later by calling getLastSavedGLerror().
    OpenGL error state is also cleared.

    @return True if there is an OpenGL error, false otherwise.
*/
TPRREbool PRREGLsnippets::isGLerrorPresent()
{
    errLast = glGetError();
    return ( errLast != GL_NO_ERROR );
} // isError()


/**
    Gets the last saved OpenGL error.
    The last saved OpenGL error code is GL_NO_ERROR by default so it is recommended to call isGLerrorPresent() first.

    @return The last OpenGL error code saved by isGLerrorPresent().
*/
GLenum PRREGLsnippets::getLastSavedGLerror()
{
    return errLast;
} // getLastSavedGLerror()


/**
    Clears OpenGL error state.
    Error code is not saved, so this function has no impact on return code of getLastSavedGLerror().
*/
void PRREGLsnippets::ClearGLerror()
{
    while ( glGetError() != GL_NO_ERROR ) ;
} // ClearGLerror()


/**
    Returns the size in Bytes of the given OpenGL index type.
    Useful when allocating index/element buffer/array for a specific index type.

    @return Size in Bytes of the given OpenGL index type.
*/
TPRREuint PRREGLsnippets::getSizeofIndexType(GLenum iType)
{
    switch (iType)
    {
    case GL_UNSIGNED_BYTE : return sizeof(TPRREubyte);
    case GL_UNSIGNED_SHORT: return sizeof(TPRREushort);
    default               : return sizeof(TPRREuint); // GL_UNSIGNED_INT
    }
}


/**
    Returns arr[index] element from the given array of iType type elements.
    Useful when the size of an index/element buffer/array is not known at compilation time, but we want to minimize its total size at runtime,
    thus we allocate its size based on the required element type selected at runtime.

    @return An iType type element from arr[index] position, casted to TPRREuint.
*/
TPRREuint PRREGLsnippets::getVertexIndex(const void* arr, TPRREuint index, GLenum iType)
{
    switch (iType)
    {
    case GL_UNSIGNED_BYTE : return ((TPRREubyte*)arr)[index];
    case GL_UNSIGNED_SHORT: return ((TPRREushort*)arr)[index];
    default               : return ((TPRREuint*)arr)[index]; // GL_UNSIGNED_INT
    }    
}


/**
    Sets arr[index] element of the given array of iType type elements to value.
    If the given value doesn't fit into given iType element type, an error message will be logged to console and no change to array will be done.

    @return True on success, false otherwise.
*/
TPRREbool PRREGLsnippets::setVertexIndex(void* arr, TPRREuint index, TPRREuint value, GLenum iType)
{
    switch (iType)
    {
    case GL_UNSIGNED_BYTE :
        if ( value > UCHAR_MAX )
        {
            CConsole::getConsoleInstance("PRREGLsnippets").EOLn("setVertexIndex() ERROR: storing %d as BYTE (MAX=%d)!", value, UCHAR_MAX);
            return false;
        }
        ((TPRREubyte*)arr)[index]  = (TPRREubyte) value;
        break;
    case GL_UNSIGNED_SHORT:
        if ( value > USHRT_MAX )
        {
            CConsole::getConsoleInstance("PRREGLsnippets").EOLn("setVertexIndex() ERROR: storing %d as SHORT (MAX=%d)!", value, USHRT_MAX);
            return false;
        }
        ((TPRREushort*)arr)[index] = (TPRREushort) value;
        break;
    default: // GL_UNSIGNED_INT
        ((TPRREuint*)arr)[index]   = value; 
        break;
    }
    return true;
}


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


const char* const PRREGLsnippets::GL_ERR_STR_NO_ERROR          = "No Error";
const char* const PRREGLsnippets::GL_ERR_STR_INVALID_ENUM      = "Invalid Enum";
const char* const PRREGLsnippets::GL_ERR_STR_INVALID_VALUE     = "Invalid Value";
const char* const PRREGLsnippets::GL_ERR_STR_INVALID_OPERATION = "Invalid Operation";
const char* const PRREGLsnippets::GL_ERR_STR_STACK_OVERFLOW    = "Stack Overflow";
const char* const PRREGLsnippets::GL_ERR_STR_STACK_UNDERFLOW   = "Stack Underflow";
const char* const PRREGLsnippets::GL_ERR_STR_OUT_OF_MEMORY     = "Out of Memory";

GLenum PRREGLsnippets::errLast = GL_NO_ERROR;