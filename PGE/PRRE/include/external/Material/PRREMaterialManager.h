#pragma once

/*
    ###################################################################################
    PRREMaterialManager.h
    This file is part of PRRE.
    External header.
    PRREMaterialManager and PRREMaterial classes.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../PRREallHeaders.h"
#include "../PRREtypes.h"
#include "../PRREFiledManager.h"
#include "PRRETextureManager.h"


/**
    Blend factors.

    Rendering a blended object means that the affected pixels in the color buffer won't be completely overwritten, but the color of the
    affected pixels will be taken into account with the source color and blending factors.
    Blending factors are floating point values between 0 and 1.
    We call the colors of the affected pixels already in the color buffer as destination colors.
    
    The written values will be:
    Red   = srcF * srcR + dstF * dstR
    Green = srcF * srcG + dstF * dstG
    Blue  = srcF * srcB + dstF * dstB
    Alpha = srcF * srcA + dstF * dstA
    where
    srcF is source blend factor, dstF is destination blend factor,
    srcR, srcG, srcB, srcA are source color components (red-green-blue-alpha),
    dstR, dstG, dstB, dstA are destination color components (red-green-blue-alpha).
    
    Maximum color component values (maxR, maxG, maxB, maxA) are the maximum values for each color component depending on
    the number of red, green, blue and alpha bitplanes.
    maxR = 2^redBitPlanes - 1, maxG, maxB and maxA defined similarly.

    Blended color components won't exceed maximum color component values.

    PRRE_CONSTANT_COLOR, PRRE_ONE_MINUS_CONSTANT_COLOR, PRRE_CONSTANT_ALPHA and PRRE_ONE_MINUS_CONSTANT_ALPHA are available
    only if the GL version is 1.4 or greater or if ARB_imaging is supported by the driver.
    
    PRRE_SRC_COLOR, PRRE_ONE_MINUS_SRC_COLOR, PRRE_DST_COLOR and PRRE_ONE_MINUS_DST_COLOR are available only if GL version is 1.4 or greater.

    Example blending: when srcF = PRRE_SRC_ALPHA, dstF = PRRE_ONE_MINUS_SRC_ALPHA, is a recommended way to implement traditional transparency.
        When srcA = 1.0, the result will be simple replacements which is expected since source/incoming alpha is 1.0 meaning that object is not transparent at all:
            srcF = 1.0/1.0 = 1,
            dstF = 1 - 1.0/1.0 = 0
            Red   = 1 * srcR + 0 * dstR = srcR
            Green = 1 * srcG + 0 * dstG = srcG
            Blue  = 1 * srcB + 0 * dstB = srcB
            Alpha = 1 * srcA + 0 * dstA = srcA
        But when srcA = 0.4, the result will be:
            srcF = 0.4/1.0 = 0.4,
            dstF = 1 - 0.4/1.0 = 0.6
            Red   = 0.4 * srcR + 0.6 * dstR
            Green = 0.4 * srcG + 0.6 * dstG
            Blue  = 0.4 * srcB + 0.6 * dstB
            Alpha = 0.4 * srcA + 0.6 * dstA

    Another example blending: when srcF = PRRE_SRC_ALPHA, dstF = PRRE_ONE, and srcA = 0.8, the result will be:
        destination (color buffer content) is brightened based on srcA value:
        srcF = 0.8/1.0 = 0.8,
        dstF = 1.0/1.0 = 1
        Red   = 0.8 * srcR + 1 * dstR = 0.8*srcR + dstR
        Green = 0.8 * srcG + 1 * dstG = 0.8*srcG + dstG
        Blue  = 0.8 * srcB + 1 * dstB = 0.8*srcB + dstB
        Alpha = 0.8 * srcA + 1 * dstA = 0.8*srcA + dstA

*/
enum TPRRE_BLENDFACTORS
{
    PRRE_ZERO,                     /**< Full zero (0,0,0,0). */
    PRRE_ONE,                      /**< Full one (1,1,1,1). */
    PRRE_SRC_COLOR,                /**< Source color components get divided by maximum color component values. */
    PRRE_ONE_MINUS_SRC_COLOR,      /**< 1 minus value of PRRE_SRC_COLOR. */
    PRRE_DST_COLOR,                /**< Destination color components get divided by maximum color component values. */
    PRRE_ONE_MINUS_DST_COLOR,      /**< 1 minus value of PRRE_DST_COLOR. */
    PRRE_SRC_ALPHA,                /**< Source alpha component get divided by maximum alpha component values. */
    PRRE_ONE_MINUS_SRC_ALPHA,      /**< 1 minus value of PRRE_SRC_ALPHA. */
    PRRE_DST_ALPHA,                /**< Destination alpha component get divided by maximum alpha component values. */
    PRRE_ONE_MINUS_DST_ALPHA,      /**< 1 minus value of PRRE_DST_ALPHA. */
    PRRE_CONSTANT_COLOR,           /**< Color components specified by glBlendColor(). */
    PRRE_ONE_MINUS_CONSTANT_COLOR, /**< 1 minus value of PRRE_CONSTANT_COLOR. */
    PRRE_CONSTANT_ALPHA,           /**< Alpha component specified by glBlendColor(). */
    PRRE_ONE_MINUS_CONSTANT_ALPHA, /**< 1 minus value of PRRE_CONSTANT_ALPHA. */
    PRRE_SRC_ALPHA_SATURATE        /**< (p,p,p,1) where p = min(srcA, maxA-dstA) / maxA. */
}; // TPRRE_BLENDFACTORS

/**
    Preset blend modes.
*/
enum TPRRE_BLENDMODES
{
    PRRE_BM_NONE,                   /**< No blending. */
    PRRE_BM_MANUAL,                 /**< Manual blending, using custom blend factors. */
    PRRE_BM_STANDARD_TRANSPARENCY   /**< Transparency. */
}; // TPRRE_BLENDMODES


/**
    Texture environment modes / functions.
    Each mode defines how produced fragment colors and alpha are calculated.
    The default mode is PRRE_TEX_FUNC_MODULATE.

    Ctxl = color computed from the previous texture stage / incoming fragment color at stage 0.
    Csrc = texture source color.
    Cenv = texture environment color.

    Atxl = alpha computed from the previous texture stage / incoming alpha at stage 0.
    Asrc = alpha from texture source color.
    
*/  // TODO: ADD? COMBINE_EXT? etc?
    // TODO: get/set functions are not defined for this.
enum TPRRE_TEX_FUNC
{
    PRRE_TEX_FUNC_REPLACE,   /**< Produced fragment color = Csrc.
                                  Produced alpha value = Asrc for texture formats containing alpha, Atxl otherwise. */
    PRRE_TEX_FUNC_DECAL,     /**< Produced fragment color = Ctxl * (1 - Asrc) + Csrc*Asrc for texture formats containing alpha, Csrc otherwise.
                                  Produced alpha value = Atxl. */
    PRRE_TEX_FUNC_MODULATE,  /**< For every texture format, produced fragment color = Ctxl * Csrc.
                                  Produced alpha value = Atxl * Asrc for texture formats containing alpha, Atxl otherwise. */
    PRRE_TEX_FUNC_BLEND,     /**< For every texture format, produced fragment color = Ctxl * (1 - Csrc) + Cenv*Csrc.
                                  Produced alpha value = Atxl * Asrc for texture formats containing alpha, Atxl otherwise. */ 
}; // TPRRE_TEX_FUNC


/**
    Texture coordinate.
*/
struct TPRRE_UVW
{
    float u, v, w;
}; // TPRRE_UVW

typedef TPRRE_UVW TUVW;



class PRREMaterialManager;

/**
    Material class.

    TODO: This class directly uses OpenGL?
*/
class PRREMaterial :
    public PRREFiledManaged
{
#ifdef PRRE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PRREMaterial is included")
#endif

public:
    virtual              ~PRREMaterial();

            TPRREbool    allocateArrays(                                      
                            TPRREuint nColorCount,
                            TPRREuint nTexcoordCount,
                            TPRREuint nIndexCount,
                            TPRREuint nIndexSize);                            /**< Allocate color-, texture coordinate-, and index arrays on all available levels. */

            TPRREuint    getIndicesCount() const;
            
            TPRREuint    getColorsCount(TPRREuint level = 0) const;           /**< Gets the number of colors on the specified level. */
            TRGBAFLOAT*  getColors(TPRREuint level = 0);                      /**< Gets the pointer to colors on the specified level. */
    const   TRGBAFLOAT*  getColors(TPRREuint level = 0) const;                /**< Gets the pointer to colors on the specified level. */
            
            TPRREuint    getTexcoordsCount(TPRREuint level = 0) const;        /**< Gets the number of texture coordinates on the specified level. */
            TUVW*        getTexcoords(TPRREuint level = 0);                   /**< Gets the pointer to texture coordinates on the specified level. */
    const   TUVW*        getTexcoords(TPRREuint level = 0) const;             /**< Gets the pointer to texture coordinates on the specified level. */

            PRRETexture* getTexture(TPRREuint level = 0);                     /**< Gets the texture of the material on the specified level. */
    const   PRRETexture* getTexture(TPRREuint level = 0) const;               /**< Gets the texture of the material on the specified level. */
            TPRREbool    setTexture(PRRETexture* tex, TPRREuint level = 0);   /**< Sets the texture of the material on the specified level. */

            TPRREuint    getTextureCount() const;                             /**< Gets the number of textures assigned to this material. */
            TPRREbool    isTextured() const;                                  /**< Gets whether the material has at least 1 textured layer. */
            TPRREbool    isSingleTextured() const;                            /**< Gets whether the material has only 1 textured layer. */
            TPRREbool    isMultiTextured() const;                             /**< Gets whether the material has at least 2 textured layers. */

            PRREColor&   getTextureEnvColor(TPRREuint level = 0);             /**< Gets the texture environment color of the material on the specified level. */
    const   PRREColor&   getTextureEnvColor(TPRREuint level = 0) const;       /**< Gets the texture environment color of the material on the specified level. */

            TPRRE_BLENDFACTORS getSourceBlendFunc(TPRREuint level = 0) const;       /**< Gets the source blend factor on the specified level. */
            TPRREbool          setSourceBlendFunc(
                                TPRRE_BLENDFACTORS value,
                                TPRREuint level = 0);                               /**< Sets the source blend factor on the specified level. */

            TPRRE_BLENDFACTORS getDestinationBlendFunc(TPRREuint level = 0) const;  /**< Gets the destination blend factor on the specified level. */
            TPRREbool          setDestinationBlendFunc(
                                TPRRE_BLENDFACTORS value,
                                TPRREuint level = 0);                               /**< Sets the destination blend factor on the specified level. */

            TPRREbool          setBlendFuncs(
                                TPRRE_BLENDFACTORS src,
                                TPRRE_BLENDFACTORS dst,
                                TPRREuint level = 0);                               /**< Sets the blend factors on the specified level. */

            TPRRE_BLENDMODES   getBlendMode(TPRREuint level = 0) const;             /**< Gets the blend mode on the specified level. */
            TPRREbool          setBlendMode(
                                TPRRE_BLENDMODES mode,
                                TPRREuint level = 0);                               /**< Sets the blend mode on the specified level. */

            TPRREbool    copyFromMaterial(
                            PRREMaterial& srcMat,
                            TPRREuint dstLevel,
                            TPRREuint srcLevel);                              /**< Copies a layer of a material to a layer of this material. */

    virtual TPRREuint    getUsedSystemMemory() const;                         /**< Gets the amount of allocated system memory. */

    // ---------------------------------------------------------------------------

protected:

    // ---------------------------------------------------------------------------
    
    explicit PRREMaterial(TPRREuint nLayers = 1);    /**< Only PRREMaterialManager creates it. */ /* TODO: mark this as noexcept(false) when using newer compiler! */
    
    PRREMaterial(const PRREMaterial&);
    PRREMaterial& operator=(const PRREMaterial&);

private:
    class PRREMaterialImpl;
    PRREMaterialImpl* pImpl;

    friend class PRREMaterialManager;  

}; // class PRREMaterial


/**
    Material manager class.

    TODO: This class doesn't use any API directly, just constant values from OpenGL?
*/
class PRREMaterialManager :
    public PRREFiledManager
{
#ifdef PRRE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PRREMaterialManager is included")
#endif

public:

    // ---------------------------------------------------------------------------

    PRREMaterialManager();  /* TODO: mark this as noexcept(false) when using newer compiler! */
    virtual ~PRREMaterialManager();

    TPRREbool isInitialized() const;  /**< Tells whether the object is correctly initialized or not. */
    
    TPRREuint getMaximumLayerCount() const;  /**< Tells maximum number of layers allowed per material. */

    PRREMaterial* createMaterial();   /**< Creates a new material. */
    
    /*PRREObject3D* createFromFile(const char* filename);*/   

protected:

    PRREMaterialManager(const PRREMaterialManager&);
    PRREMaterialManager& operator=(const PRREMaterialManager&);

private:
    class PRREMaterialManagerImpl;
    PRREMaterialManagerImpl* pImpl;  

}; // class PRREMaterialManager
