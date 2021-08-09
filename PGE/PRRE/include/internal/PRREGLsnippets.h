#pragma once

/*
    ###################################################################################
    include/internal/PRREGLsnippets.h
    This file is part of PRRE.  
    Internal header.
    PRRE useful functions for OpenGL.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/


#include "../external/PRREallHeaders.h"
#include "../external/Material/PRREMaterialManager.h"

/**
    PRRE useful functions for OpenGL.
*/
class PRREGLsnippets
{
#ifdef PRRE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PRREGLsnippets is included")
#endif

public:

    static const char* getGLErrorTextFromEnum(GLenum err);  /**< Gets the string representation of the given GL error. */
    static TPRREbool   isGLErrorPresent();                  /**< Gets whether there is an error in OpenGL or not. */
    static GLenum      getLastSavedGLError();               /**< Gets the last saved GL error. */
    static void        ClearGLError();                      /**< Clears GL error state. */

    static TPRREuint getSizeofIndexType(GLenum iType);                                           /**< Returns the size in Bytes of the given OpenGL index type. */
    static TPRREuint getVertexIndex(const void* arr, TPRREuint index, GLenum iType);             /**< Returns arr[index] element from the given array of iType type elements. */
    static TPRREbool setVertexIndex(void* arr, TPRREuint index, TPRREuint value, GLenum iType);  /**< Sets arr[index] element of the given array of iType type elements to value. */

    static void      SetGLBoundingBoxRendering(TPRREbool state);                                 /**< Sets GL states up for bounding box rendering or resets them. */

    static TPRRE_BLENDFACTOR getPRREBlendFromGLBlend(GLenum glb);                               /**< Gets the appropriate PRRE blend factor for the given GL enum. */
    static GLenum             getGLBlendFromPRREBlend(TPRRE_BLENDFACTOR bf);                    /**< Gets the appropriate GL enum for the given PRRE blend factor. */
    
    static void      glLoadTextureIntoTMU(const PRRETexture* tex, TPRREuint iTMU, TPRREbool bSticked);  /**< Loads the given texture into the texture mapping unit. */
    static void      glLoadTexturesAndSetBlendState(const PRREMaterial* mat, TPRREbool bSticked);       /**< Loads all textures into texture mapping units and sets blending if needed for single-pass multitexturing. */

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

