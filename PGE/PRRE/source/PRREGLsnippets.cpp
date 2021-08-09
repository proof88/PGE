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

#include <cassert>

#include "../include/internal/PRREGLextensionFuncs.h"
#include "../include/internal/PRREpragmas.h"
#include "../include/external/Hardware/PRREhwInfo.h"

static const char* const sLoggerModuleName = "PRREGLsnippets";


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
} // getSizeofIndexType()


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
} // getVertexIndex()


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
            CConsole::getConsoleInstance(sLoggerModuleName).EOLn("setVertexIndex() ERROR: storing %d as BYTE (MAX=%d)!", value, UCHAR_MAX);
            return false;
        }
        ((TPRREubyte*)arr)[index]  = (TPRREubyte) value;
        break;
    case GL_UNSIGNED_SHORT:
        if ( value > USHRT_MAX )
        {
            CConsole::getConsoleInstance(sLoggerModuleName).EOLn("setVertexIndex() ERROR: storing %d as SHORT (MAX=%d)!", value, USHRT_MAX);
            return false;
        }
        ((TPRREushort*)arr)[index] = (TPRREushort) value;
        break;
    default: // GL_UNSIGNED_INT
        ((TPRREuint*)arr)[index]   = value; 
        break;
    }
    return true;
} // setVertexIndex()


/**
    Sets GL states up for bounding box rendering or reset them.
    @param state If true, GL states are set for bounding box rendering. If false, some states are reset for normal rendering (rest of states are controlled by Object3D in such case).
*/
void PRREGLsnippets::glBoundingBoxRendering(TPRREbool state)
{
    if ( state )
    {
        // obj->Draw() must not set these states when the given renderPass is PRRE_RPASS_SYNC_OCCLUSION_START
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        glDepthMask(GL_FALSE);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glDisable(GL_LIGHTING);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDisable(GL_BLEND);
        glDisable(GL_ALPHA_TEST);
        glLoadTexturesAndSetBlendState(PGENULL, false);
    }
    else
    {
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glDepthMask(GL_TRUE);
        glFlush(); // this to make sure any initiated GL query is actually starts to be processed by GPU so later we dont have to wait that much for results
    }
} // glBoundingBoxRendering()


/**
    Gets the appropriate PRRE blend factor for the given GL enum.

    @return The appropriate PRRE blend factor for the given GL enum.
            PRRE_ZERO for invalid GL enum.
*/
TPRRE_BLENDFACTORS PRREGLsnippets::getPRREblendFromGLblend(GLenum glb)
{
    switch( glb )
    {
    case GL_ZERO                : return PRRE_ZERO;
    case GL_ONE                 : return PRRE_ONE;
    case GL_SRC_COLOR           : return PRRE_SRC_COLOR;
    case GL_ONE_MINUS_SRC_COLOR : return PRRE_ONE_MINUS_SRC_COLOR;
    case GL_DST_COLOR           : return PRRE_DST_COLOR;
    case GL_ONE_MINUS_DST_COLOR : return PRRE_ONE_MINUS_DST_COLOR;
    case GL_SRC_ALPHA           : return PRRE_SRC_ALPHA;
    case GL_ONE_MINUS_SRC_ALPHA : return PRRE_ONE_MINUS_SRC_ALPHA;
    case GL_DST_ALPHA           : return PRRE_DST_ALPHA;         
    case GL_ONE_MINUS_DST_ALPHA : return PRRE_ONE_MINUS_DST_ALPHA;
    case GL_SRC_ALPHA_SATURATE  : return PRRE_SRC_ALPHA_SATURATE;
    default                     : return PRRE_ZERO;
    }
} // getPRREblendFromGLblend()


/**
    Gets the appropriate GL enum for the given PRRE blend factor.

    @return The appropriate GL enum for the given PRRE blend factor.
            GL_ZERO for invalid PRRE blend factor.
*/
GLenum PRREGLsnippets::getGLblendFromPRREblend(TPRRE_BLENDFACTORS bf)
{
    switch( bf )
    {
    case PRRE_ZERO                : return GL_ZERO;
    case PRRE_ONE                 : return GL_ONE;
    case PRRE_SRC_COLOR           : return GL_SRC_COLOR;
    case PRRE_ONE_MINUS_SRC_COLOR : return GL_ONE_MINUS_SRC_COLOR;
    case PRRE_DST_COLOR           : return GL_DST_COLOR;
    case PRRE_ONE_MINUS_DST_COLOR : return GL_ONE_MINUS_DST_COLOR;
    case PRRE_SRC_ALPHA           : return GL_SRC_ALPHA;
    case PRRE_ONE_MINUS_SRC_ALPHA : return GL_ONE_MINUS_SRC_ALPHA;
    case PRRE_DST_ALPHA           : return GL_DST_ALPHA;         
    case PRRE_ONE_MINUS_DST_ALPHA : return GL_ONE_MINUS_DST_ALPHA;
    case PRRE_SRC_ALPHA_SATURATE  : return GL_SRC_ALPHA_SATURATE;
    default                       : return GL_ZERO;
    }
} // getGLblendFromPRREblend()


/**
    Loads the given texture into the texture mapping unit.

    @param tex      Texture to be loaded. If NULL, texturing will be disabled on the specified texture mapping unit.
    @param iTMU     Into which texture mapping unit we want to load the texture. Currently it must be either 0 or 1.
    @param bSticked True if object is sticked to screen, false otherwise.
*/
void PRREGLsnippets::glLoadTextureIntoTMU(const PRRETexture* tex, TPRREuint iTMU, TPRREbool bSticked)
{
    /* Make sure we don't call GL functions when no accelerator is present */
    if ( !PRREhwInfo::get().getVideo().isAcceleratorDetected() )
        return;

    // TODO: the PR00FPS-style lasttex method is not present here currently ... later it should be implemented.
    // A general TMU manager should record which TMU holds which texture, so that can avoid unnecessary glBindTexture() calls.

    // currently we support only 2 textured layers ...
    if ( iTMU > 1 )
    {
        CConsole::getConsoleInstance(sLoggerModuleName).EOLn("ERROR: glLoadTextureIntoTMU(tex, %d)", iTMU);
        assert( false );
    }

    if ( PRREhwInfo::get().getVideo().isMultiTexturingSupported() )
    {
        glActiveTextureARB(GL_TEXTURE0_ARB + iTMU);
        if ( tex != PGENULL )
        {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, tex->getInternalNum());
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        }
        else
            glDisable(GL_TEXTURE_2D);
    }
    else
    {
        if ( tex != PGENULL )
        {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, tex->getInternalNum());
            if ( bSticked )
                glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
            else
                glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        }
        else
            glDisable(GL_TEXTURE_2D);
    }
} // glLoadTextureIntoTMU()


/**
    Loads all textures into texture mapping units and sets blending if needed for single-pass multitexturing.
    @param mat      Material from where textures should be loaded. If it is PGENULL, texturing will be disabled.
    @param bSticked True if object is sticked to screen, false otherwise.
*/
void PRREGLsnippets::glLoadTexturesAndSetBlendState(const PRREMaterial* mat, TPRREbool bSticked)
{
    if ( PGENULL == mat )
    {
        glDisable(GL_BLEND);
        glLoadTextureIntoTMU( PGENULL, 0, bSticked );
        if ( PRREhwInfo::get().getVideo().isMultiTexturingSupported() )
        {
            glLoadTextureIntoTMU( PGENULL, 1, bSticked );
        }
        return;
    }

    if ( PRREhwInfo::get().getVideo().isMultiTexturingSupported() )
    {
        if ( mat->isMultiTextured() )
        {
            // enable blending of 2nd layer
            glEnable(GL_BLEND);
	        glBlendFunc(getGLblendFromPRREblend(mat->getSourceBlendFunc(1)), getGLblendFromPRREblend(mat->getDestinationBlendFunc(1)));
            glLoadTextureIntoTMU( mat->getTexture(0), 0, bSticked );
            glLoadTextureIntoTMU( mat->getTexture(1), 1, bSticked );
        }
        else
        {
            // disable blending only if base layer is not blended ...
            if ( ! PRREMaterial::isBlendFuncBlends(mat->getSourceBlendFunc(), mat->getDestinationBlendFunc()) )
            {
                glDisable(GL_BLEND);
            }
            glLoadTextureIntoTMU( mat->getTexture(0), 0, bSticked );
            glLoadTextureIntoTMU( PGENULL, 1, bSticked );
        }
    }
    else
    {
        // disable blending only if base layer is not blended ...
        if ( ! PRREMaterial::isBlendFuncBlends(mat->getSourceBlendFunc(), mat->getDestinationBlendFunc()) )
        {
            glDisable(GL_BLEND);
        }
        glLoadTextureIntoTMU( mat->getTexture(), 0, bSticked );
    }
} // glLoadTexturesAndSetBlendState()


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