#pragma once

/*
    ###################################################################################
    PureTextureManager.h
    This file is part of PURE.
    External header.
    PureTextureManager and PureTexture classes.
    Made by PR00F88
    ###################################################################################
*/

#include "../PureAllHeaders.h"
#include "PureImageManager.h"


/**
    Texture filtering modes.
*/
enum TPURE_ISO_TEX_FILTERING
{
    PURE_ISO_NEAREST,                /**< Both minification and magnification: nearest neighbor filtering (pixel from 1 texel). */
    PURE_ISO_LINEAR,                 /**< Both minification and magnification: bilinear filtering (pixel from 4 texels). */
    PURE_ISO_NEAREST_MIPMAP_NEAREST, /**< Minification only: nearest MIP map level selection and nearest-neighbor filtering from that level. */
    PURE_ISO_NEAREST_MIPMAP_LINEAR,  /**< Minification only: nearest 2 MIP map levels selection, nearest-neighbor filtering from each levels and
                                                             finally weighted-avging the 2 values. */
    PURE_ISO_LINEAR_MIPMAP_NEAREST,  /**< Minification only: nearest MIP map level selection and bilinear filtering from that level (pixel from 4 texels). */
    PURE_ISO_LINEAR_MIPMAP_LINEAR    /**< Minification only: nearest 2 MIP map levels selection, bilinear filtering from each levels and finally weighted-avging the 2 values. */
}; // TPURE_ISO_TEX_FILTERING


/**
    Anisotropic filtering modes.
*/
enum TPURE_ANISO_TEX_FILTERING
{
    PURE_ANISO_1X  =   1,  /**< No aniso filtering. */
    PURE_ANISO_2X  =   2,  /**< Aniso filtering level 2. */
    PURE_ANISO_4X  =   4,  /**< Aniso filtering level 4. */
    PURE_ANISO_8X  =   8,  /**< Aniso filtering level 8. */
    PURE_ANISO_16X =  16,  /**< Aniso filtering level 16. */    
    PURE_ANISO_MAX = 100   /**< Maximum available aniso filtering level. */
}; // TPURE_ANISO_TEX_FILTERING


/**
    Texture compression modes.
*/
enum TPURE_TEX_COMPRESSION_MODE
{
    PURE_TC_NONE,              /**< None. */
    PURE_TC_S3TC_RGB_DXT1,     /**< S3TC RGB DXT1. */
    PURE_TC_S3TC_RGBA_DXT1,    /**< S3TC RGBA DXT1. */
    PURE_TC_S3TC_RGBA_DXT3,    /**< S3TC RGBA DXT3. */
    PURE_TC_S3TC_RGBA_DXT5,    /**< S3TC RGBA DXT5. */
    PURE_TC_3DFX_RGB_FXT1,     /**< S3TC RGB FXT1. */
    PURE_TC_3DFX_RGBA_FXT1,    /**< S3TC RGBA FXT1. */
    PURE_TC_AUTO               /**< Engine chooses automatically. */
}; // TPURE_TEX_COMPRESSION_MODE


/**
    Texture formats.
*/
enum TPURE_TEX_FORMAT
{
    PURE_TF_DXT1,   /**< DXT1. */
    PURE_TF_DXT1A,  /**< DXT1A. */
    PURE_TF_DXT3A,  /**< DXT3A. */
    PURE_TF_DXT5A,  /**< DXT5A. */
    PURE_TF_FXT1,   /**< FXT1. */
    PURE_TF_FXT1A,  /**< FXT1A. */
    PURE_TF_RGBA,   /**< RGBA. */
    PURE_TF_UNSURE  /**< Unsure which. */
}; // TPURE_TEX_FORMAT


/**
    Texture wrapping modes.
    Controls how the texture should be sampled when a texture coordinate outside the range of [0,1] is given.
*/
enum TPURE_TEX_WRAPPING
{
    PURE_TW_REPEAT,              /**< Texture is mapped repeated onto the object,
                                      as the texture coordinates' integer part is ignored, only the fractional part is used. */

    PURE_TW_MIRRORED_REPEAT,     /**< Texture is mapped repeated onto the object,
                                      as the texture coordinates' fractional part is used depending on the integer part's parity:
                                       - if integer part is even, fractional part is used;
                                       - if integer part is odd, 1 minus fractional part is used. 
                                      Not available on all systems. */

    PURE_TW_CLAMP,               /**< Texture is mapped clamped onto the object,
                                      by texture coordinates being clamped to the range [0,1].
                                      Border color is used for coordinates outside the range if texture has border set. If not then border texels' color is used. */

    PURE_TW_CLAMP_TO_BORDER,     /**< Texture is mapped clamped onto the object,
                                      by texture coordinates being clamped to the range [-1/2N, 1+1/2N] where N is the size of the texture in the direction of clamping.
                                      Border color is used where texture coordinates are outside of range [0,1].
                                      Not available on all systems. */

    PURE_TW_CLAMP_TO_EDGE        /**< Texture is mapped clamped onto the object,
                                      by texture coordinates being clamped to the range [1/2N, 1-1/2N] where N is the size of the texture in the direction of clamping.
                                      Border color is always ignored. */
};


class PureTextureManager;

/**
    Texture-representing class.
*/
class PureTexture :
    public PureImage
{
#ifdef PURE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureTexture is included")
#endif

public:
    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    virtual ~PureTexture();

    CConsole&   getManagedConsole() const;            /**< Returns access to console preset with logger module name as this class. */

   
    TPURE_TEX_FORMAT getInternalFormat() const;    /**< Gets the internal texture format, including compression mode. */
    TPureUInt        getInternalNum() const;       /**< Gets the internal texture ID. */
    
    TPURE_ISO_TEX_FILTERING getMinFilteringMode() const;  /**< Gets the isotropic filtering mode when zooming out (1 texel < 1 pixel). */
    TPURE_ISO_TEX_FILTERING getMagFilteringMode() const;  /**< Gets the isotropic filtering mode when zooming in (1 texel > 1 pixel). */
    TPureBool setMinFilteringMode(TPURE_ISO_TEX_FILTERING filtering);  /**< Sets the isotropic filtering mode when zooming out (1 texel < 1 pixel). */
    TPureBool setMagFilteringMode(TPURE_ISO_TEX_FILTERING filtering);  /**< Sets the isotropic filtering mode when zooming in (1 texel > 1 pixel). */
    TPureBool setIsoFilteringMode(
        TPURE_ISO_TEX_FILTERING minfilter,
        TPURE_ISO_TEX_FILTERING magfilter);                            /**< Sets the isotropic filtering modes. */

    TPureUInt getMIPmapCount() const;               /**< Gets the number of MIP maps. */

    TPURE_ANISO_TEX_FILTERING
        getAnisoFilteringMode() const;              /**< Gets the anisotropic filtering mode. */
    TPureBool
        setAnisoFilteringMode(
            TPURE_ANISO_TEX_FILTERING filtering);   /**< Sets the anisotropic filtering mode. */

    TPURE_TEX_WRAPPING
        getTextureWrappingModeS() const;            /**< Gets the texture wrapping mode for S coordinates. */
    TPURE_TEX_WRAPPING
        getTextureWrappingModeT() const;            /**< Gets the texture wrapping mode for T coordinates. */
    TPureBool
        setTextureWrappingMode(
            TPURE_TEX_WRAPPING tw_s,
            TPURE_TEX_WRAPPING tw_t );              /**< Sets the texture wrapping mode. */

    TPureBool        getBorder() const;             /**< Gets the border state. */
    TPureBool        setBorder(TPureBool state);    /**< Sets the border state. */
    const PureColor& getBorderColor() const;                  /**< Gets color of the border. */
    TPureBool        setBorderColor(const PureColor& clr);    /**< Sets color of the border. */

    TPureBool uploadPixels();                             /**< Uploads pixels in system memory to video memory. */
    
    virtual TPureUInt getUsedSystemMemory() const;   /**< Gets the amount of allocated system memory for the texture (VRAM not included). */
    TPureUInt         getUsedTextureMemory() const;  /**< Gets the amount of VRAM allocated (considering compression, MIP mapping, etc...). */

protected:

    // ---------------------------------------------------------------------------

    PureTexture();                                 /**< Only PureTextureManager creates it. */ /* TODO: mark this as noexcept(false) when using newer compiler! */
    explicit PureTexture(const PureImage& img);    /**< Only PureTextureManager creates it this way. */ /* TODO: mark this as noexcept(false) when using newer compiler! */
    
    PureTexture(const PureTexture&);
    PureTexture& operator=(const PureTexture&);
   

private:

    class PureTextureImpl;
    PureTextureImpl* pImpl;

    friend class PureTextureManager;  

}; // class PureTexture


/**
    Texture manager class.
*/
class PureTextureManager :
    public PureImageManager
{
#ifdef PURE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureTextureManager is included")
#endif

public:
    static TPureUInt getMIPmapCount(
        TPureUInt width, TPureUInt height);  /**< Calculates the number of MIP maps from the given texture size. */
    static TPureUInt getWidthAtLevel(
        TPureUInt level,
        TPureUInt width, TPureUInt height);  /**< Calculates the texture width at the given MIP map level. */
    static TPureUInt getHeightAtLevel(
        TPureUInt level,
        TPureUInt width, TPureUInt height);  /**< Calculates the texture height at the given MIP map level. */
    static TPureUInt getMIPmappedTexSize(
        TPureUInt basesize,                                 
        TPureUInt width, TPureUInt height);  /**< Calculates the total size of the MIP mapped texture in memory based on its basesize and size. */

    /** Gets the Pure aniso texture filtering value that corresponds to the given floating point value. */
    static TPURE_ANISO_TEX_FILTERING getPureanisoTexFilteringNameFromFloat(TPureFloat value);
    /** Gets the Pure aniso texture filtering value that corresponds to the given integer value. */
    static TPURE_ANISO_TEX_FILTERING getPureanisoTexFilteringNameFromUInt(TPureUInt value);
    /** Gets the value that corresponds to the given Pure aniso texture filtering value. */
    static TPureFloat                getFloatFromPureanisoTexFilteringName(TPURE_ANISO_TEX_FILTERING value);
    /** Gets the value that corresponds to the given Pure aniso texture filtering value. */
    static TPureUInt                 getUIntFromPureanisoTexFilteringName(TPURE_ANISO_TEX_FILTERING value);  

    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    PureTextureManager();             /**< Sets default values. */ /* TODO: mark this as noexcept(false) when using newer compiler! */
    virtual ~PureTextureManager();

    CConsole&  getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */

    TPureBool isInitialized() const;  /**< Tells whether the object is correctly initialized or not. */
  
    TPureUInt getUsedTextureMemory() const;                         /**< Gets the total amount of allocated VRAM by all of the textures. */
    TPureBool isHardwareMipMapGenerationEnabled() const;            /**< Gets whether HW MIP map generation is enabled or not. */
    void      SetHardwareMipMapGenerationEnabled(TPureBool state);  /**< Sets whether HW MIP map generation is enabled or not. */
    TPureBool isNativeDIBFormatSupportEnabled() const;              /**< Gets whether direct upload of BGRA-format images is enabled or not. */
    void      SetNativeDIBFormatSupportEnabled(TPureBool state);    /**< Sets whether direct upload of BGRA-format images is enabled or not. */
    TPureBool isLazyInstancingEnabled() const;                      /**< Gets whether always creating new texture instances from the same file is enabled or not. */
    void      SetLazyInstancingEnabled(TPureBool state);            /**< Sets whether always create new texture instances from the same file or not. */
    TPureBool isPixelPreservingEnabled() const;                     /**< Gets whether keeping pixels of loaded textures in system memory is enabled or not. */
    void      SetPixelPreservingEnabled(TPureBool state);           /**< Sets whether to keep pixels of loaded textures in system memory or delete them after texture uploading. */

    PureTexture*         createTextureFromImage(const PureImage& img);  /**< Creates texture from the given image. */
    virtual PureTexture* createFromFile(const char* filename);          /**< Creates texture from the given file. */
    
    TPURE_ISO_TEX_FILTERING getDefaultMinFilteringMode() const;           /**< Gets the default isotropic filtering mode when zooming out. */
    TPURE_ISO_TEX_FILTERING getDefaultMagFilteringMode() const;           /**< Gets the default isotropic filtering mode when zooming in. */
    TPureBool setDefaultMinFilteringMode(TPURE_ISO_TEX_FILTERING filtering);   /**< Sets the default isotropic filtering mode when zooming out (when 1 texel < 1 pixel). */
    TPureBool setDefaultMagFilteringMode(TPURE_ISO_TEX_FILTERING filtering);   /**< Sets the default isotropic filtering mode when zooming in (when 1 texel > 1 pixel). */
    TPureBool setDefaultIsoFilteringMode(
        TPURE_ISO_TEX_FILTERING minfilter,
        TPURE_ISO_TEX_FILTERING magfilter);                                    /**< Sets the default isotropic filtering modes. */ 
    
    TPURE_ANISO_TEX_FILTERING
        getDefaultAnisoFilteringMode() const;      /**< Gets the default anisotropic filtering level. */
    TPureBool
        setDefaultAnisoFilteringMode(
            TPURE_ANISO_TEX_FILTERING filtering);  /**< Sets the default anisotropic filtering level. */
    TPURE_ANISO_TEX_FILTERING getMaximumAnisoFiltering() const;       /**< Gets the maximum level of anisotropic filtering. */

    TPURE_TEX_WRAPPING
        getDefaultTextureWrappingModeS() const;     /**< Gets the default texture wrapping mode for S coordinates. */
    TPURE_TEX_WRAPPING
        getDefaultTextureWrappingModeT() const;     /**< Gets the default texture wrapping mode for T coordinates. */
    TPureBool
        setDefaultTextureWrappingMode(
            TPURE_TEX_WRAPPING tw_s,
            TPURE_TEX_WRAPPING tw_t );              /**< Sets the default texture wrapping mode. */
    
    TPURE_TEX_COMPRESSION_MODE
        getDefaultCompressionMode() const;     /**< Gets the default texture compression mode. */
    TPureBool
        setDefaultCompressionMode(
            TPURE_TEX_COMPRESSION_MODE mode);  /**< Sets the default texture compression mode. */

    TPureBool getDefaultBorder() const;             /**< Gets the default border state. */
    TPureBool setDefaultBorder(TPureBool state);    /**< Sets the default border state. */

          PureColor& getDefaultBorderColor();         /**< Gets default color of the border. */
    const PureColor& getDefaultBorderColor() const;   /**< Gets default color of the border. */

    virtual void WriteList() const;            /**< From PureImageManager, adding total texture memory usage counting. */

protected: 

    // ---------------------------------------------------------------------------

    PureTextureManager(const PureTextureManager&);
    PureTextureManager& operator=(const PureTextureManager&);

    virtual void WriteListCallback(const PureManaged& mngd) const;  /**< From PureImageManager, texture memory and internal number. */

private:

    class PureTextureManagerImpl;
    PureTextureManagerImpl* pImpl;

}; // class PureTextureManager
