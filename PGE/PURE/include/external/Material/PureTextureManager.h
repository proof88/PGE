#pragma once

/*
    ###################################################################################
    PureTextureManager.h
    This file is part of PURE.
    External header.
    PureTextureManager and PureTexture classes.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../PureAllHeaders.h"
#include "PureImageManager.h"


/**
    Texture filtering modes.
*/
enum TPure_ISO_TEX_FILTERING
{
    Pure_ISO_NEAREST,                /**< Both minification and magnification: nearest neighbor filtering (pixel from 1 texel). */
    Pure_ISO_LINEAR,                 /**< Both minification and magnification: bilinear filtering (pixel from 4 texels). */
    Pure_ISO_NEAREST_MIPMAP_NEAREST, /**< Minification only: nearest MIP map level selection and nearest-neighbor filtering from that level. */
    Pure_ISO_NEAREST_MIPMAP_LINEAR,  /**< Minification only: nearest 2 MIP map levels selection, nearest-neighbor filtering from each levels and
                                                             finally weighted-avging the 2 values. */
    Pure_ISO_LINEAR_MIPMAP_NEAREST,  /**< Minification only: nearest MIP map level selection and bilinear filtering from that level (pixel from 4 texels). */
    Pure_ISO_LINEAR_MIPMAP_LINEAR    /**< Minification only: nearest 2 MIP map levels selection, bilinear filtering from each levels and finally weighted-avging the 2 values. */
}; // TPure_ISO_TEX_FILTERING


/**
    Anisotropic filtering modes.
*/
enum TPure_ANISO_TEX_FILTERING
{
    Pure_ANISO_1X  =   1,  /**< No aniso filtering. */
    Pure_ANISO_2X  =   2,  /**< Aniso filtering level 2. */
    Pure_ANISO_4X  =   4,  /**< Aniso filtering level 4. */
    Pure_ANISO_8X  =   8,  /**< Aniso filtering level 8. */
    Pure_ANISO_16X =  16,  /**< Aniso filtering level 16. */    
    Pure_ANISO_MAX = 100   /**< Maximum available aniso filtering level. */
}; // TPure_ANISO_TEX_FILTERING


/**
    Texture compression modes.
*/
enum TPure_TEX_COMPRESSION_MODE
{
    Pure_TC_NONE,              /**< None. */
    Pure_TC_S3TC_RGB_DXT1,     /**< S3TC RGB DXT1. */
    Pure_TC_S3TC_RGBA_DXT1,    /**< S3TC RGBA DXT1. */
    Pure_TC_S3TC_RGBA_DXT3,    /**< S3TC RGBA DXT3. */
    Pure_TC_S3TC_RGBA_DXT5,    /**< S3TC RGBA DXT5. */
    Pure_TC_3DFX_RGB_FXT1,     /**< S3TC RGB FXT1. */
    Pure_TC_3DFX_RGBA_FXT1,    /**< S3TC RGBA FXT1. */
    Pure_TC_AUTO               /**< Engine chooses automatically. */
}; // TPure_TEX_COMPRESSION_MODE


/**
    Texture formats.
*/
enum TPure_TEX_FORMAT
{
    Pure_TF_DXT1,   /**< DXT1. */
    Pure_TF_DXT1A,  /**< DXT1A. */
    Pure_TF_DXT3A,  /**< DXT3A. */
    Pure_TF_DXT5A,  /**< DXT5A. */
    Pure_TF_FXT1,   /**< FXT1. */
    Pure_TF_FXT1A,  /**< FXT1A. */
    Pure_TF_RGBA,   /**< RGBA. */
    Pure_TF_UNSURE  /**< Unsure which. */
}; // TPure_TEX_FORMAT


/**
    Texture wrapping modes.
    Controls how the texture should be sampled when a texture coordinate outside the range of [0,1] is given.
*/
enum TPure_TEX_WRAPPING
{
    Pure_TW_REPEAT,              /**< Texture is mapped repeated onto the object,
                                      as the texture coordinates' integer part is ignored, only the fractional part is used. */

    Pure_TW_MIRRORED_REPEAT,     /**< Texture is mapped repeated onto the object,
                                      as the texture coordinates' fractional part is used depending on the integer part's parity:
                                       - if integer part is even, fractional part is used;
                                       - if integer part is odd, 1 minus fractional part is used. 
                                      Not available on all systems. */

    Pure_TW_CLAMP,               /**< Texture is mapped clamped onto the object,
                                      by texture coordinates being clamped to the range [0,1].
                                      Border color is used for coordinates outside the range if texture has border set. If not then border texels' color is used. */

    Pure_TW_CLAMP_TO_BORDER,     /**< Texture is mapped clamped onto the object,
                                      by texture coordinates being clamped to the range [-1/2N, 1+1/2N] where N is the size of the texture in the direction of clamping.
                                      Border color is used where texture coordinates are outside of range [0,1].
                                      Not available on all systems. */

    Pure_TW_CLAMP_TO_EDGE        /**< Texture is mapped clamped onto the object,
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
#ifdef Pure_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureTexture is included")
#endif

public:
    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    virtual ~PureTexture();

    CConsole&   getManagedConsole() const;            /**< Returns access to console preset with logger module name as this class. */

   
    TPure_TEX_FORMAT getInternalFormat() const;    /**< Gets the internal texture format, including compression mode. */
    TPureuint        getInternalNum() const;       /**< Gets the internal texture ID. */
    
    TPure_ISO_TEX_FILTERING getMinFilteringMode() const;  /**< Gets the isotropic filtering mode when zooming out (1 texel < 1 pixel). */
    TPure_ISO_TEX_FILTERING getMagFilteringMode() const;  /**< Gets the isotropic filtering mode when zooming in (1 texel > 1 pixel). */
    TPurebool setMinFilteringMode(TPure_ISO_TEX_FILTERING filtering);  /**< Sets the isotropic filtering mode when zooming out (1 texel < 1 pixel). */
    TPurebool setMagFilteringMode(TPure_ISO_TEX_FILTERING filtering);  /**< Sets the isotropic filtering mode when zooming in (1 texel > 1 pixel). */
    TPurebool setIsoFilteringMode(
        TPure_ISO_TEX_FILTERING minfilter,
        TPure_ISO_TEX_FILTERING magfilter);                            /**< Sets the isotropic filtering modes. */

    TPureuint getMIPmapCount() const;               /**< Gets the number of MIP maps. */

    TPure_ANISO_TEX_FILTERING
        getAnisoFilteringMode() const;              /**< Gets the anisotropic filtering mode. */
    TPurebool
        setAnisoFilteringMode(
            TPure_ANISO_TEX_FILTERING filtering);   /**< Sets the anisotropic filtering mode. */

    TPure_TEX_WRAPPING
        getTextureWrappingModeS() const;            /**< Gets the texture wrapping mode for S coordinates. */
    TPure_TEX_WRAPPING
        getTextureWrappingModeT() const;            /**< Gets the texture wrapping mode for T coordinates. */
    TPurebool
        setTextureWrappingMode(
            TPure_TEX_WRAPPING tw_s,
            TPure_TEX_WRAPPING tw_t );              /**< Sets the texture wrapping mode. */

    TPurebool        getBorder() const;             /**< Gets the border state. */
    TPurebool        setBorder(TPurebool state);    /**< Sets the border state. */
    const PureColor& getBorderColor() const;                  /**< Gets color of the border. */
    TPurebool        setBorderColor(const PureColor& clr);    /**< Sets color of the border. */

    TPurebool uploadPixels();                             /**< Uploads pixels in system memory to video memory. */
    
    virtual TPureuint getUsedSystemMemory() const;   /**< Gets the amount of allocated system memory for the texture (VRAM not included). */
    TPureuint         getUsedTextureMemory() const;  /**< Gets the amount of VRAM allocated (considering compression, MIP mapping, etc...). */

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
#ifdef Pure_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureTextureManager is included")
#endif

public:
    static TPureuint getMIPmapCount(
        TPureuint width, TPureuint height);  /**< Calculates the number of MIP maps from the given texture size. */
    static TPureuint getWidthAtLevel(
        TPureuint level,
        TPureuint width, TPureuint height);  /**< Calculates the texture width at the given MIP map level. */
    static TPureuint getHeightAtLevel(
        TPureuint level,
        TPureuint width, TPureuint height);  /**< Calculates the texture height at the given MIP map level. */
    static TPureuint getMIPmappedTexSize(
        TPureuint basesize,                                 
        TPureuint width, TPureuint height);  /**< Calculates the total size of the MIP mapped texture in memory based on its basesize and size. */

    /** Gets the Pure aniso texture filtering value that corresponds to the given floating point value. */
    static TPure_ANISO_TEX_FILTERING getPureanisoTexFilteringNameFromFloat(TPurefloat value);
    /** Gets the Pure aniso texture filtering value that corresponds to the given integer value. */
    static TPure_ANISO_TEX_FILTERING getPureanisoTexFilteringNameFromUInt(TPureuint value);
    /** Gets the value that corresponds to the given Pure aniso texture filtering value. */
    static TPurefloat                getFloatFromPureanisoTexFilteringName(TPure_ANISO_TEX_FILTERING value);
    /** Gets the value that corresponds to the given Pure aniso texture filtering value. */
    static TPureuint                 getUIntFromPureanisoTexFilteringName(TPure_ANISO_TEX_FILTERING value);  

    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    PureTextureManager();             /**< Sets default values. */ /* TODO: mark this as noexcept(false) when using newer compiler! */
    virtual ~PureTextureManager();

    CConsole&  getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */

    TPurebool isInitialized() const;  /**< Tells whether the object is correctly initialized or not. */
  
    TPureuint getUsedTextureMemory() const;                         /**< Gets the total amount of allocated VRAM by all of the textures. */
    TPurebool isHardwareMipMapGenerationEnabled() const;            /**< Gets whether HW MIP map generation is enabled or not. */
    void      SetHardwareMipMapGenerationEnabled(TPurebool state);  /**< Sets whether HW MIP map generation is enabled or not. */
    TPurebool isNativeDIBFormatSupportEnabled() const;              /**< Gets whether direct upload of BGRA-format images is enabled or not. */
    void      SetNativeDIBFormatSupportEnabled(TPurebool state);    /**< Sets whether direct upload of BGRA-format images is enabled or not. */
    TPurebool isLazyInstancingEnabled() const;                      /**< Gets whether always creating new texture instances from the same file is enabled or not. */
    void      SetLazyInstancingEnabled(TPurebool state);            /**< Sets whether always create new texture instances from the same file or not. */
    TPurebool isPixelPreservingEnabled() const;                     /**< Gets whether keeping pixels of loaded textures in system memory is enabled or not. */
    void      SetPixelPreservingEnabled(TPurebool state);           /**< Sets whether to keep pixels of loaded textures in system memory or delete them after texture uploading. */

    PureTexture*         createTextureFromImage(const PureImage& img);  /**< Creates texture from the given image. */
    virtual PureTexture* createFromFile(const char* filename);          /**< Creates texture from the given file. */
    
    TPure_ISO_TEX_FILTERING getDefaultMinFilteringMode() const;           /**< Gets the default isotropic filtering mode when zooming out. */
    TPure_ISO_TEX_FILTERING getDefaultMagFilteringMode() const;           /**< Gets the default isotropic filtering mode when zooming in. */
    TPurebool setDefaultMinFilteringMode(TPure_ISO_TEX_FILTERING filtering);   /**< Sets the default isotropic filtering mode when zooming out (when 1 texel < 1 pixel). */
    TPurebool setDefaultMagFilteringMode(TPure_ISO_TEX_FILTERING filtering);   /**< Sets the default isotropic filtering mode when zooming in (when 1 texel > 1 pixel). */
    TPurebool setDefaultIsoFilteringMode(
        TPure_ISO_TEX_FILTERING minfilter,
        TPure_ISO_TEX_FILTERING magfilter);                                    /**< Sets the default isotropic filtering modes. */ 
    
    TPure_ANISO_TEX_FILTERING
        getDefaultAnisoFilteringMode() const;      /**< Gets the default anisotropic filtering level. */
    TPurebool
        setDefaultAnisoFilteringMode(
            TPure_ANISO_TEX_FILTERING filtering);  /**< Sets the default anisotropic filtering level. */
    TPure_ANISO_TEX_FILTERING getMaximumAnisoFiltering() const;       /**< Gets the maximum level of anisotropic filtering. */

    TPure_TEX_WRAPPING
        getDefaultTextureWrappingModeS() const;     /**< Gets the default texture wrapping mode for S coordinates. */
    TPure_TEX_WRAPPING
        getDefaultTextureWrappingModeT() const;     /**< Gets the default texture wrapping mode for T coordinates. */
    TPurebool
        setDefaultTextureWrappingMode(
            TPure_TEX_WRAPPING tw_s,
            TPure_TEX_WRAPPING tw_t );              /**< Sets the default texture wrapping mode. */
    
    TPure_TEX_COMPRESSION_MODE
        getDefaultCompressionMode() const;     /**< Gets the default texture compression mode. */
    TPurebool
        setDefaultCompressionMode(
            TPure_TEX_COMPRESSION_MODE mode);  /**< Sets the default texture compression mode. */

    TPurebool getDefaultBorder() const;             /**< Gets the default border state. */
    TPurebool setDefaultBorder(TPurebool state);    /**< Sets the default border state. */

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
