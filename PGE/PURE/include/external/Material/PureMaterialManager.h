#pragma once

/*
    ###################################################################################
    PureMaterialManager.h
    This file is part of PURE.
    External header.
    PureMaterialManager and PureMaterial classes.
    Made by PR00F88
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

    PURE_CONSTANT_COLOR, PURE_ONE_MINUS_CONSTANT_COLOR, PURE_CONSTANT_ALPHA and PURE_ONE_MINUS_CONSTANT_ALPHA are available
    only if the GL version is 1.4 or greater or if ARB_imaging is supported by the driver.
    
    PURE_SRC_COLOR, PURE_ONE_MINUS_SRC_COLOR, PURE_DST_COLOR and PURE_ONE_MINUS_DST_COLOR are available only if GL version is 1.4 or greater.

    Example blending: when srcF = PURE_SRC_ALPHA, dstF = PURE_ONE_MINUS_SRC_ALPHA, is a recommended way to implement traditional transparency.
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

    Another example blending: when srcF = PURE_SRC_ALPHA, dstF = PURE_ONE, and srcA = 0.8, the result will be:
        destination (color buffer content) is brightened based on srcA value:
        srcF = 0.8/1.0 = 0.8,
        dstF = 1.0/1.0 = 1
        Red   = 0.8 * srcR + 1 * dstR = 0.8*srcR + dstR
        Green = 0.8 * srcG + 1 * dstG = 0.8*srcG + dstG
        Blue  = 0.8 * srcB + 1 * dstB = 0.8*srcB + dstB
        Alpha = 0.8 * srcA + 1 * dstA = 0.8*srcA + dstA

*/
enum TPURE_BLENDFACTOR
{
    PURE_ZERO,                     /**< Full zero (0,0,0,0). */
    PURE_ONE,                      /**< Full one (1,1,1,1). */
    PURE_SRC_COLOR,                /**< Source color components get divided by maximum color component values. */
    PURE_ONE_MINUS_SRC_COLOR,      /**< 1 minus value of PURE_SRC_COLOR. */
    PURE_DST_COLOR,                /**< Destination color components get divided by maximum color component values. */
    PURE_ONE_MINUS_DST_COLOR,      /**< 1 minus value of PURE_DST_COLOR. */
    PURE_SRC_ALPHA,                /**< Source alpha component get divided by maximum alpha component values. */
    PURE_ONE_MINUS_SRC_ALPHA,      /**< 1 minus value of PURE_SRC_ALPHA. */
    PURE_DST_ALPHA,                /**< Destination alpha component get divided by maximum alpha component values. */
    PURE_ONE_MINUS_DST_ALPHA,      /**< 1 minus value of PURE_DST_ALPHA. */
    PURE_CONSTANT_COLOR,           /**< Color components specified by glBlendColor(). */
    PURE_ONE_MINUS_CONSTANT_COLOR, /**< 1 minus value of PURE_CONSTANT_COLOR. */
    PURE_CONSTANT_ALPHA,           /**< Alpha component specified by glBlendColor(). */
    PURE_ONE_MINUS_CONSTANT_ALPHA, /**< 1 minus value of PURE_CONSTANT_ALPHA. */
    PURE_SRC_ALPHA_SATURATE        /**< (p,p,p,1) where p = min(srcA, maxA-dstA) / maxA. */
}; // TPURE_BLENDFACTOR

/**
    Preset blend modes.
*/
enum TPURE_BLENDMODE
{
    PURE_BM_NONE,                   /**< No blending. */
    PURE_BM_MANUAL,                 /**< Manual blending, using custom blend factors. */
    PURE_BM_STANDARD_TRANSPARENCY   /**< Standard transparency. Recommended only if we have real alpha values.
                                         Equivalent to setting (PURE_SRC_ALPHA, PURE_ONE_MINUS_SRC_ALPHA) src and dst blend factors
                                         by PureMaterial::setBlendFuncs(). */
}; // TPURE_BLENDMODE


/**
    Texture environment modes / functions.
    Each mode defines how produced fragment colors and alpha are calculated.
    The default mode is PURE_TEX_FUNC_MODULATE.

    Ctxl = color computed from the previous texture stage / incoming fragment color at stage 0.
    Csrc = texture source color.
    Cenv = texture environment color.

    Atxl = alpha computed from the previous texture stage / incoming alpha at stage 0.
    Asrc = alpha from texture source color.
    
*/  // TODO: ADD? COMBINE_EXT? etc?
    // TODO: get/set functions are not defined for this.
enum TPURE_TEX_FUNC
{
    PURE_TEX_FUNC_REPLACE,   /**< Produced fragment color = Csrc.
                                  Produced alpha value = Asrc for texture formats containing alpha, Atxl otherwise. */
    PURE_TEX_FUNC_DECAL,     /**< Produced fragment color = Ctxl * (1 - Asrc) + Csrc*Asrc for texture formats containing alpha, Csrc otherwise.
                                  Produced alpha value = Atxl. */
    PURE_TEX_FUNC_MODULATE,  /**< For every texture format, produced fragment color = Ctxl * Csrc.
                                  Produced alpha value = Atxl * Asrc for texture formats containing alpha, Atxl otherwise. */
    PURE_TEX_FUNC_BLEND,     /**< For every texture format, produced fragment color = Ctxl * (1 - Csrc) + Cenv*Csrc.
                                  Produced alpha value = Atxl * Asrc for texture formats containing alpha, Atxl otherwise. */ 
}; // TPURE_TEX_FUNC


/**
    Texture coordinate.
*/
struct TPURE_UVW
{
    float u, v, w;
}; // TPURE_UVW

typedef TPURE_UVW TUVW;



class PureMaterialManager;

/**
    Material class.
*/
class PureMaterial :
    public PureFiledManaged
{
#ifdef PURE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureMaterial is included")
#endif

public:
    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    static TPureBool isBlendFuncReallyBlending(TPURE_BLENDFACTOR sfactor, TPURE_BLENDFACTOR dfactor);  /**< Gets whether the given source and destination factors really mean blending or not. */

    // ---------------------------------------------------------------------------

    virtual              ~PureMaterial();

            CConsole&    getManagedConsole() const;                           /**< Returns access to console preset with logger module name as this class. */
    
            TPureBool    allocateArrays(                                      
                            TPureUInt nColorCount,
                            TPureUInt nTexcoordCount,
                            TPureUInt nIndexCount,
                            TPureUInt nIndexSize);                            /**< Allocate color-, texture coordinate-, and index arrays on all available levels. */

            TPureUInt    getIndicesCount() const;
            
            TPureUInt    getColorsCount(TPureUInt level = 0) const;           /**< Gets the number of colors on the specified level. */
            TRGBAFLOAT*  getColors(TPureUInt level = 0);                      /**< Gets the pointer to colors on the specified level. */
    const   TRGBAFLOAT*  getColors(TPureUInt level = 0) const;                /**< Gets the pointer to colors on the specified level. */
            
            TPureUInt    getTexcoordsCount(TPureUInt level = 0) const;        /**< Gets the number of texture coordinates on the specified level. */
            TUVW*        getTexcoords(TPureUInt level = 0);                   /**< Gets the pointer to texture coordinates on the specified level. */
    const   TUVW*        getTexcoords(TPureUInt level = 0) const;             /**< Gets the pointer to texture coordinates on the specified level. */

            PureTexture* getTexture(TPureUInt level = 0);                     /**< Gets the texture of the material on the specified level. */
    const   PureTexture* getTexture(TPureUInt level = 0) const;               /**< Gets the texture of the material on the specified level. */
            TPureBool    setTexture(PureTexture* tex, TPureUInt level = 0);   /**< Sets the texture of the material on the specified level. */

            TPureUInt    getTextureCount() const;                             /**< Gets the number of textures assigned to this material. */
            TPureBool    isTextured() const;                                  /**< Gets whether the material has at least 1 textured layer. */
            TPureBool    isSingleTextured() const;                            /**< Gets whether the material has only 1 textured layer. */
            TPureBool    isMultiTextured() const;                             /**< Gets whether the material has at least 2 textured layers. */

            PureColor&   getTextureEnvColor(TPureUInt level = 0);             /**< Gets the texture environment color of the material on the specified level. */
    const   PureColor&   getTextureEnvColor(TPureUInt level = 0) const;       /**< Gets the texture environment color of the material on the specified level. */

            TPURE_BLENDFACTOR getSourceBlendFunc(TPureUInt level = 0) const;       /**< Gets the source blend factor on the specified level. */
            TPureBool         setSourceBlendFunc(
                               TPURE_BLENDFACTOR value,
                               TPureUInt level = 0);                               /**< Sets the source blend factor on the specified level. */

            TPURE_BLENDFACTOR getDestinationBlendFunc(TPureUInt level = 0) const;  /**< Gets the destination blend factor on the specified level. */
            TPureBool         setDestinationBlendFunc(
                               TPURE_BLENDFACTOR value,
                               TPureUInt level = 0);                               /**< Sets the destination blend factor on the specified level. */

            TPureBool         setBlendFuncs(
                               TPURE_BLENDFACTOR src,
                               TPURE_BLENDFACTOR dst,
                               TPureUInt level = 0);                               /**< Sets the blend factors on the specified level. */

            TPURE_BLENDMODE   getBlendMode(TPureUInt level = 0) const;             /**< Gets the blend mode on the specified level. */
            TPureBool         setBlendMode(
                               TPURE_BLENDMODE mode,
                               TPureUInt level = 0);                               /**< Sets the blend mode on the specified level. */

            TPureBool    copyFromMaterial(
                            PureMaterial& srcMat,
                            TPureUInt dstLevel,
                            TPureUInt srcLevel);                              /**< Copies a layer of a material to a layer of this material. */

    virtual TPureUInt    getUsedSystemMemory() const;                         /**< Gets the amount of allocated system memory. */

    // ---------------------------------------------------------------------------

protected:

    // ---------------------------------------------------------------------------
    
    explicit PureMaterial(TPureUInt nLayers = 1);    /**< Only PureMaterialManager creates it. */ /* TODO: mark this as noexcept(false) when using newer compiler! */
    
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
#ifdef PURE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureMaterialManager is included")
#endif

public:

    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    PureMaterialManager();  /* TODO: mark this as noexcept(false) when using newer compiler! */
    virtual ~PureMaterialManager();

    CConsole&  getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */

    TPureBool isInitialized() const;  /**< Tells whether the object is correctly initialized or not. */
    
    TPureUInt getMaximumLayerCount() const;  /**< Tells maximum number of layers allowed per material. */

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
