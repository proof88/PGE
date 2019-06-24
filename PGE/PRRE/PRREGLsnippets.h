#pragma once

/*
    ###################################################################################
    PRREGLsnippets.h
    This file is part of PRRE.  
    Internal header.
    PRRE useful functions for OpenGL.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/


#include "PRREallHeaders.h"

/**
    PRRE useful functions for OpenGL.

    This class directly uses OpenGL.
*/
class PRREGLsnippets
{
#ifdef PRRE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PRREGLsnippets is included")
#endif

public:

    static const char* getGLerrorTextFromEnum(GLenum err);  /**< Gets the string representation of the given GL error. */
    static TPRREbool   isGLerrorPresent();                  /**< Gets whether there is an error in OpenGL or not. */
    static GLenum      getLastSavedGLerror();               /**< Gets the last saved GL error. */
    static void        ClearGLerror();                      /**< Clears GL error state. */

    static TPRREuint getSizeofIndexType(GLenum iType);                                            /**< Returns the size in Bytes of the given OpenGL index type. */
    static TPRREuint getIndexFromArray(const void* arr, TPRREuint index, GLenum iType);           /**< Returns arr[index] element from the given array of iType type elements. */
    static void      SetIndexInArray(void* arr, TPRREuint index, TPRREuint value, GLenum iType);  /**< Sets arr[index] element of the given array of iType type elements to value. */

private:
    static const char* const GL_ERR_STR_NO_ERROR;
    static const char* const GL_ERR_STR_INVALID_ENUM;
    static const char* const GL_ERR_STR_INVALID_VALUE;
    static const char* const GL_ERR_STR_INVALID_OPERATION;
    static const char* const GL_ERR_STR_STACK_OVERFLOW;
    static const char* const GL_ERR_STR_STACK_UNDERFLOW;
    static const char* const GL_ERR_STR_OUT_OF_MEMORY;

    static GLenum errLast;

}; // class PRREGLsnippets

