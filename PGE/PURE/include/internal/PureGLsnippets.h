#pragma once

/*
    ###################################################################################
    include/internal/PureGLsnippets.h
    This file is part of PURE.  
    Internal header.
    Pure useful functions for OpenGL.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include <vector>

#include "../external/PureAllHeaders.h"
#include "../external/PureTypes.h"
#include "../external/Material/PureMaterialManager.h"

/**
    Pure useful functions for OpenGL.
*/
class PureGLsnippets
{
#ifdef Pure_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureGLsnippets is included")
#endif

public:

    static void        Init();                              /**< Initialize static class instance. */
    static const char* getGLErrorTextFromEnum(GLenum err);  /**< Gets the string representation of the given GL error. */
    static TPurebool   isGLErrorPresent();                  /**< Gets whether there is an error in OpenGL or not. */
    static GLenum      getLastSavedGLError();               /**< Gets the last saved GL error. */
    static void        ClearGLError();                      /**< Clears GL error state. */

    static TPureuint getSizeofIndexType(GLenum iType);                                           /**< Returns the size in Bytes of the given OpenGL index type. */
    static TPureuint getVertexIndex(const void* arr, TPureuint index, GLenum iType);             /**< Returns arr[index] element from the given array of iType type elements. */
    static TPurebool setVertexIndex(void* arr, TPureuint index, TPureuint value, GLenum iType);  /**< Sets arr[index] element of the given array of iType type elements to value. */

    static void      SetGLBoundingBoxRendering(TPurebool state);                                 /**< Sets GL states up for bounding box rendering or resets them. */
    static void      glPrepareBeforeDrawBoundingBox();
    static void      SetZPassRendering(TPurebool state);

    static TPure_BLENDFACTOR getPureBlendFromGLBlend(GLenum glb);                               /**< Gets the appropriate Pure blend factor for the given GL enum. */
    static GLenum            getGLBlendFromPureBlend(TPure_BLENDFACTOR bf);                     /**< Gets the appropriate GL enum for the given Pure blend factor. */
    
    static void      glLoadTextureIntoTMU(const PureTexture* tex, TPureuint iTMU, TPurebool bSticked);  /**< Loads the given texture into the texture mapping unit. */
    static void      glLoadTexturesAndSetBlendState(const PureMaterial* mat, TPurebool bObjLevel1Sticked, TPurebool bObjLevel1Blended);       /**< Loads all textures into texture mapping units and sets blending if needed for single-pass multitexturing. */

private:
    static const char* const GL_ERR_STR_NO_ERROR;
    static const char* const GL_ERR_STR_INVALID_ENUM;
    static const char* const GL_ERR_STR_INVALID_VALUE;
    static const char* const GL_ERR_STR_INVALID_OPERATION;
    static const char* const GL_ERR_STR_STACK_OVERFLOW;
    static const char* const GL_ERR_STR_STACK_UNDERFLOW;
    static const char* const GL_ERR_STR_OUT_OF_MEMORY;

    static GLenum errLast;

    static std::vector<GLuint> iLastTex;  /**< Last loaded texture in TMU, where vector elem 0 is for TMU0, elem 1 is for TMU1 and so on ... */

}; // class PureGLsnippets

