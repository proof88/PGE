#pragma once

/*
    ###################################################################################
    PureMaterialManager.h
    This file is part of PURE.
    External header.
    PureMaterialManager and PureMaterial classes.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../PureAllHeaders.h"
#include "../PureTypes.h"
#include "../PureFiledManager.h"
#include "PureTextureManager.h"


/**
    Blend factor.

    Blend factors are floating point values between 0 and 1.
    Rendering a blended object means that the affected pixels in the color buffer won't be completely overwritten, but
    will be taken into account with the source color and blending factors.
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

    Pure_CONSTANT_COLOR, Pure_ONE_MINUS_CONSTANT_COLOR, Pure_CONSTANT_ALPHA and Pure_ONE_MINUS_CONSTANT_ALPHA are available
    only if the GL version is 1.4 or greater or if ARB_imaging is supported by the driver.
    
    Pure_SRC_COLOR, Pure_ONE_MINUS_SRC_COLOR, Pure_DST_COLOR and Pure_ONE_MINUS_DST_COLOR are available only if GL version is 1.4 or greater.

    Example blending: when srcF = Pure_SRC_ALPHA, dstF = Pure_ONE_MINUS_SRC_ALPHA, is a recommended way to implement traditional transparency.
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

    Another example blending: when srcF = Pure_SRC_ALPHA, dstF = Pure_ONE, and srcA = 0.8, the result will be:
        destination (color buffer content) is brightened based on srcA value:
        srcF = 0.8/1.0 = 0.8,
        dstF = 1.0/1.0 = 1
        Red   = 0.8 * srcR + 1 * dstR = 0.8*srcR + dstR
        Green = 0.8 * srcG + 1 * dstG = 0.8*srcG + dstG
        Blue  = 0.8 * srcB + 1 * dstB = 0.8*srcB + dstB
        Alpha = 0.8 * srcA + 1 * dstA = 0.8*srcA + dstA

*/
enum TPure_BLENDFACTOR
{
    Pure_ZERO,                     /**< Full zero (0,0,0,0). */
    Pure_ONE,                      /**< Full one (1,1,1,1). */
    Pure_SRC_COLOR,                /**< Source color components get divided by maximum color component values. */
    Pure_ONE_MINUS_SRC_COLOR,      /**< 1 minus value of Pure_SRC_COLOR. */
    Pure_DST_COLOR,                /**< Destination color components get divided by maximum color component values. */
    Pure_ONE_MINUS_DST_COLOR,      /**< 1 minus value of Pure_DST_COLOR. */
    Pure_SRC_ALPHA,                /**< Source alpha component get divided by maximum alpha component values. */
    Pure_ONE_MINUS_SRC_ALPHA,      /**< 1 minus value of Pure_SRC_ALPHA. */
    Pure_DST_ALPHA,                /**< Destination alpha component get divided by maximum alpha component values. */
    Pure_ONE_MINUS_DST_ALPHA,      /**< 1 minus value of Pure_DST_ALPHA. */
    Pure_CONSTANT_COLOR,           /**< Color components specified by glBlendColor(). */
    Pure_ONE_MINUS_CONSTANT_COLOR, /**< 1 minus value of Pure_CONSTANT_COLOR. */
    Pure_CONSTANT_ALPHA,           /**< Alpha component specified by glBlendColor(). */
    Pure_ONE_MINUS_CONSTANT_ALPHA, /**< 1 minus value of Pure_CONSTANT_ALPHA. */
    Pure_SRC_ALPHA_SATURATE        /**< (p,p,p,1) where p = min(srcA, maxA-dstA) / maxA. */
}; // TPure_BLENDFACTOR

/**
    Preset blend modes.
*/
enum TPure_BLENDMODE
{
    Pure_BM_NONE,                   /**< No blending. */
    Pure_BM_MANUAL,                 /**< Manual blending, using custom blend factors. */
    Pure_BM_STANDARD_TRANSPARENCY   /**< Standard transparency. Recommended only if we have real alpha values.
                                         Equivalent to setting (Pure_SRC_ALPHA, Pure_ONE_MINUS_SRC_ALPHA) src and dst blend factors
                                         by PureMaterial::setBlendFuncs(). */
}; // TPure_BLENDMODE


/**
    Texture environment modes / functions.
    Each mode defines how produced fragment colors and alpha are calculated.
    The default mode is Pure_TEX_FUNC_MODULATE.

    Ctxl = color computed from the previous texture stage / incoming fragment color at stage 0.
    Csrc = texture source color.
    Cenv = texture environment color.

    Atxl = alpha computed from the previous texture stage / incoming alpha at stage 0.
    Asrc = alpha from texture source color.
    
*/  // TODO: ADD? COMBINE_EXT? etc?
    // TODO: get/set functions are not defined for this.
enum TPure_TEX_FUNC
{
    Pure_TEX_FUNC_REPLACE,   /**< Produced fragment color = Csrc.
                                  Produced alpha value = Asrc for texture formats containing alpha, Atxl otherwise. */
    Pure_TEX_FUNC_DECAL,     /**< Produced fragment color = Ctxl * (1 - Asrc) + Csrc*Asrc for texture formats containing alpha, Csrc otherwise.
                                  Produced alpha value = Atxl. */
    Pure_TEX_FUNC_MODULATE,  /**< For every texture format, produced fragment color = Ctxl * Csrc.
                                  Produced alpha value = Atxl * Asrc for texture formats containing alpha, Atxl otherwise. */
    Pure_TEX_FUNC_BLEND,     /**< For every texture format, produced fragment color = Ctxl * (1 - Csrc) + Cenv*Csrc.
                                  Produced alpha value = Atxl * Asrc for texture formats containing alpha, Atxl otherwise. */ 
}; // TPure_TEX_FUNC


/**
    Texture coordinate.
*/
struct TPure_UVW
{
    float u, v, w;
}; // TPure_UVW

typedef TPure_UVW TUVW;



class PureMaterialManager;

/**
    Material class.
*/
class PureMaterial :
    public PureFiledManaged
{
#ifdef Pure_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureMaterial is included")
#endif

public:
    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    static TPurebool isBlendFuncReallyBlending(TPure_BLENDFACTOR sfactor, TPure_BLENDFACTOR dfactor);  /**< Gets whether the given source and destination factors really mean blending or not. */

    // ---------------------------------------------------------------------------

    virtual              ~PureMaterial();

            CConsole&    getManagedConsole() const;                           /**< Returns access to console preset with logger module name as this class. */
    
            TPurebool    allocateArrays(                                      
                            TPureuint nColorCount,
                            TPureuint nTexcoordCount,
                            TPureuint nIndexCount,
                            TPureuint nIndexSize);                            /**< Allocate color-, texture coordinate-, and index arrays on all available levels. */

            TPureuint    getIndicesCount() const;
            
            TPureuint    getColorsCount(TPureuint level = 0) const;           /**< Gets the number of colors on the specified level. */
            TRGBAFLOAT*  getColors(TPureuint level = 0);                      /**< Gets the pointer to colors on the specified level. */
    const   TRGBAFLOAT*  getColors(TPureuint level = 0) const;                /**< Gets the pointer to colors on the specified level. */
            
            TPureuint    getTexcoordsCount(TPureuint level = 0) const;        /**< Gets the number of texture coordinates on the specified level. */
            TUVW*        getTexcoords(TPureuint level = 0);                   /**< Gets the pointer to texture coordinates on the specified level. */
    const   TUVW*        getTexcoords(TPureuint level = 0) const;             /**< Gets the pointer to texture coordinates on the specified level. */

            PureTexture* getTexture(TPureuint level = 0);                     /**< Gets the texture of the material on the specified level. */
    const   PureTexture* getTexture(TPureuint level = 0) const;               /**< Gets the texture of the material on the specified level. */
            TPurebool    setTexture(PureTexture* tex, TPureuint level = 0);   /**< Sets the texture of the material on the specified level. */

            TPureuint    getTextureCount() const;                             /**< Gets the number of textures assigned to this material. */
            TPurebool    isTextured() const;                                  /**< Gets whether the material has at least 1 textured layer. */
            TPurebool    isSingleTextured() const;                            /**< Gets whether the material has only 1 textured layer. */
            TPurebool    isMultiTextured() const;                             /**< Gets whether the material has at least 2 textured layers. */

            PureColor&   getTextureEnvColor(TPureuint level = 0);             /**< Gets the texture environment color of the material on the specified level. */
    const   PureColor&   getTextureEnvColor(TPureuint level = 0) const;       /**< Gets the texture environment color of the material on the specified level. */

            TPure_BLENDFACTOR getSourceBlendFunc(TPureuint level = 0) const;       /**< Gets the source blend factor on the specified level. */
            TPurebool         setSourceBlendFunc(
                               TPure_BLENDFACTOR value,
                               TPureuint level = 0);                               /**< Sets the source blend factor on the specified level. */

            TPure_BLENDFACTOR getDestinationBlendFunc(TPureuint level = 0) const;  /**< Gets the destination blend factor on the specified level. */
            TPurebool         setDestinationBlendFunc(
                               TPure_BLENDFACTOR value,
                               TPureuint level = 0);                               /**< Sets the destination blend factor on the specified level. */

            TPurebool         setBlendFuncs(
                               TPure_BLENDFACTOR src,
                               TPure_BLENDFACTOR dst,
                               TPureuint level = 0);                               /**< Sets the blend factors on the specified level. */

            TPure_BLENDMODE   getBlendMode(TPureuint level = 0) const;             /**< Gets the blend mode on the specified level. */
            TPurebool         setBlendMode(
                               TPure_BLENDMODE mode,
                               TPureuint level = 0);                               /**< Sets the blend mode on the specified level. */

            TPurebool    copyFromMaterial(
                            PureMaterial& srcMat,
                            TPureuint dstLevel,
                            TPureuint srcLevel);                              /**< Copies a layer of a material to a layer of this material. */

    virtual TPureuint    getUsedSystemMemory() const;                         /**< Gets the amount of allocated system memory. */

    // ---------------------------------------------------------------------------

protected:

    // ---------------------------------------------------------------------------
    
    explicit PureMaterial(TPureuint nLayers = 1);    /**< Only PureMaterialManager creates it. */ /* TODO: mark this as noexcept(false) when using newer compiler! */
    
    PureMaterial(const PureMaterial&);
    PureMaterial& operator=(const PureMaterial&);

private:
    class PureMaterialImpl;
    PureMaterialImpl* pImpl;

    friend class PureMaterialManager;  

}; // class PureMaterial


/**
    Material manager class.
*/
class PureMaterialManager :
    public PureFiledManager
{
#ifdef Pure_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureMaterialManager is included")
#endif

public:

    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    PureMaterialManager();  /* TODO: mark this as noexcept(false) when using newer compiler! */
    virtual ~PureMaterialManager();

    CConsole&  getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */

    TPurebool isInitialized() const;  /**< Tells whether the object is correctly initialized or not. */
    
    TPureuint getMaximumLayerCount() const;  /**< Tells maximum number of layers allowed per material. */

    PureMaterial* createMaterial();   /**< Creates a new material. */
    
    /*PureObject3D* createFromFile(const char* filename);*/   

    virtual void WriteList() const;   /**< From PureFiledManager, adding logging material data. */

protected:

    PureMaterialManager(const PureMaterialManager&);
    PureMaterialManager& operator=(const PureMaterialManager&);

    virtual void WriteListCallback(const PureManaged& mngd) const;

private:
    class PureMaterialManagerImpl;
    PureMaterialManagerImpl* pImpl;  

}; // class PureMaterialManager
