#pragma once

/*
    ###################################################################################
    PRRETextureManager.h
    This file is part of PRRE.
    External header.
    PRRETextureManager and PRRETexture classes.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../PRREallHeaders.h"
#include "PRREImageManager.h"


/**
    Texture filtering modes.
*/
enum TPRRE_ISO_TEX_FILTERING
{
    PRRE_ISO_NEAREST,                /**< Both minification and magnification: nearest neighbor filtering (pixel from 1 texel). */
    PRRE_ISO_LINEAR,                 /**< Both minification and magnification: bilinear filtering (pixel from 4 texels). */
    PRRE_ISO_NEAREST_MIPMAP_NEAREST, /**< Minification only: nearest MIP map level selection and nearest-neighbor filtering from that level. */
    PRRE_ISO_NEAREST_MIPMAP_LINEAR,  /**< Minification only: nearest 2 MIP map levels selection, nearest-neighbor filtering from each levels and
                                                             finally weighted-avging the 2 values. */
    PRRE_ISO_LINEAR_MIPMAP_NEAREST,  /**< Minification only: nearest MIP map level selection and bilinear filtering from that level (pixel from 4 texels). */
    PRRE_ISO_LINEAR_MIPMAP_LINEAR    /**< Minification only: nearest 2 MIP map levels selection, bilinear filtering from each levels and finally weighted-avging the 2 values. */
}; // TPRRE_ISO_TEX_FILTERING


/**
    Anisotropic filtering modes.
*/
enum TPRRE_ANISO_TEX_FILTERING
{
    PRRE_ANISO_1X  =   1,  /**< No aniso filtering. */
    PRRE_ANISO_2X  =   2,  /**< Aniso filtering level 2. */
    PRRE_ANISO_4X  =   4,  /**< Aniso filtering level 4. */
    PRRE_ANISO_8X  =   8,  /**< Aniso filtering level 8. */
    PRRE_ANISO_16X =  16,  /**< Aniso filtering level 16. */    
    PRRE_ANISO_MAX = 100   /**< Maximum available aniso filtering level. */
}; // TPRRE_ANISO_TEX_FILTERING


/**
    Texture compression modes.
*/
enum TPRRE_TEX_COMPRESSION_MODE
{
    PRRE_TC_NONE,              /**< None. */
    PRRE_TC_S3TC_RGB_DXT1,     /**< S3TC RGB DXT1. */
    PRRE_TC_S3TC_RGBA_DXT1,    /**< S3TC RGBA DXT1. */
    PRRE_TC_S3TC_RGBA_DXT3,    /**< S3TC RGBA DXT3. */
    PRRE_TC_S3TC_RGBA_DXT5,    /**< S3TC RGBA DXT5. */
    PRRE_TC_3DFX_RGB_FXT1,     /**< S3TC RGB FXT1. */
    PRRE_TC_3DFX_RGBA_FXT1,    /**< S3TC RGBA FXT1. */
    PRRE_TC_AUTO               /**< Engine chooses automatically. */
}; // TPRRE_TEX_COMPRESSION_MODE


/**
    Texture formats.
*/
enum TPRRE_TEX_FORMAT
{
    PRRE_TF_DXT1,   /**< DXT1. */
    PRRE_TF_DXT1A,  /**< DXT1A. */
    PRRE_TF_DXT3A,  /**< DXT3A. */
    PRRE_TF_DXT5A,  /**< DXT5A. */
    PRRE_TF_FXT1,   /**< FXT1. */
    PRRE_TF_FXT1A,  /**< FXT1A. */
    PRRE_TF_RGBA,   /**< RGBA. */
    PRRE_TF_UNSURE  /**< Unsure which. */
}; // TPRRE_TEX_FORMAT


/**
    Texture wrapping modes.
    Controls how the texture should be sampled when a texture coordinate outside the range of [0,1] is given.
*/
enum TPRRE_TEX_WRAPPING
{
    PRRE_TW_REPEAT,              /**< Texture is mapped repeated onto the object,
                                      as the texture coordinates' integer part is ignored, only the fractional part is used. */

    PRRE_TW_MIRRORED_REPEAT,     /**< Texture is mapped repeated onto the object,
                                      as the texture coordinates' fractional part is used depending on the integer part's parity:
                                       - if integer part is even, fractional part is used;
                                       - if integer part is odd, 1 minus fractional part is used. 
                                      Not available on all systems. */

    PRRE_TW_CLAMP,               /**< Texture is mapped clamped onto the object,
                                      by texture coordinates being clamped to the range [0,1].
                                      Border color is used for coordinates outside the range if texture has border set. If not then border texels' color is used. */

    PRRE_TW_CLAMP_TO_BORDER,     /**< Texture is mapped clamped onto the object,
                                      by texture coordinates being clamped to the range [-1/2N, 1+1/2N] where N is the size of the texture in the direction of clamping.
                                      Border color is used where texture coordinates are outside of range [0,1].
                                      Not available on all systems. */

    PRRE_TW_CLAMP_TO_EDGE        /**< Texture is mapped clamped onto the object,
                                      by texture coordinates being clamped to the range [1/2N, 1-1/2N] where N is the size of the texture in the direction of clamping.
                                      Border color is always ignored. */
};


class PRRETextureManager;

/**
    Texture-representing class.
*/
class PRRETexture :
    public PRREImage
{
#ifdef PRRE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PRRETexture is included")
#endif

public:
    virtual ~PRRETexture();

    CConsole&   getManagedConsole() const;            /**< Returns access to console preset with logger module name as this class. */
    const char* getLoggerModuleName() const;          /**< Returns the logger module name of this class. */

   
    TPRRE_TEX_FORMAT getInternalFormat() const;    /**< Gets the internal texture format, including compression mode. */
    TPRREuint        getInternalNum() const;       /**< Gets the internal texture ID. */
    
    TPRRE_ISO_TEX_FILTERING getMinFilteringMode() const;  /**< Gets the isotropic filtering mode when zooming out (1 texel < 1 pixel). */
    TPRRE_ISO_TEX_FILTERING getMagFilteringMode() const;  /**< Gets the isotropic filtering mode when zooming in (1 texel > 1 pixel). */
    TPRREbool setMinFilteringMode(TPRRE_ISO_TEX_FILTERING filtering);  /**< Sets the isotropic filtering mode when zooming out (1 texel < 1 pixel). */
    TPRREbool setMagFilteringMode(TPRRE_ISO_TEX_FILTERING filtering);  /**< Sets the isotropic filtering mode when zooming in (1 texel > 1 pixel). */
    TPRREbool setIsoFilteringMode(
        TPRRE_ISO_TEX_FILTERING minfilter,
        TPRRE_ISO_TEX_FILTERING magfilter);                            /**< Sets the isotropic filtering modes. */

    TPRREuint getMIPmapCount() const;               /**< Gets the number of MIP maps. */

    TPRRE_ANISO_TEX_FILTERING
        getAnisoFilteringMode() const;              /**< Gets the anisotropic filtering mode. */
    TPRREbool
        setAnisoFilteringMode(
            TPRRE_ANISO_TEX_FILTERING filtering);   /**< Sets the anisotropic filtering mode. */

    TPRRE_TEX_WRAPPING
        getTextureWrappingModeS() const;            /**< Gets the texture wrapping mode for S coordinates. */
    TPRRE_TEX_WRAPPING
        getTextureWrappingModeT() const;            /**< Gets the texture wrapping mode for T coordinates. */
    TPRREbool
        setTextureWrappingMode(
            TPRRE_TEX_WRAPPING tw_s,
            TPRRE_TEX_WRAPPING tw_t );              /**< Sets the texture wrapping mode. */

    TPRREbool        getBorder() const;             /**< Gets the border state. */
    TPRREbool        setBorder(TPRREbool state);    /**< Sets the border state. */
    const PRREColor& getBorderColor() const;                  /**< Gets color of the border. */
    TPRREbool        setBorderColor(const PRREColor& clr);    /**< Sets color of the border. */

    TPRREbool uploadPixels();                             /**< Uploads pixels in system memory to video memory. */
    
    virtual TPRREuint getUsedSystemMemory() const;   /**< Gets the amount of allocated system memory for the texture (VRAM not included). */
    TPRREuint         getUsedTextureMemory() const;  /**< Gets the amount of VRAM allocated (considering compression, MIP mapping, etc...). */

protected:

    // ---------------------------------------------------------------------------

    PRRETexture();                                 /**< Only PRRETextureManager creates it. */ /* TODO: mark this as noexcept(false) when using newer compiler! */
    explicit PRRETexture(const PRREImage& img);    /**< Only PRRETextureManager creates it this way. */ /* TODO: mark this as noexcept(false) when using newer compiler! */
    
    PRRETexture(const PRRETexture&);
    PRRETexture& operator=(const PRRETexture&);
   

private:

    class PRRETextureImpl;
    PRRETextureImpl* pImpl;

    friend class PRRETextureManager;  

}; // class PRRETexture


/**
    Texture manager class.
*/
class PRRETextureManager :
    public PRREImageManager
{
#ifdef PRRE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PRRETextureManager is included")
#endif

public:
    static TPRREuint getMIPmapCount(
        TPRREuint width, TPRREuint height);  /**< Calculates the number of MIP maps from the given texture size. */
    static TPRREuint getWidthAtLevel(
        TPRREuint level,
        TPRREuint width, TPRREuint height);  /**< Calculates the texture width at the given MIP map level. */
    static TPRREuint getHeightAtLevel(
        TPRREuint level,
        TPRREuint width, TPRREuint height);  /**< Calculates the texture height at the given MIP map level. */
    static TPRREuint getMIPmappedTexSize(
        TPRREuint basesize,                                 
        TPRREuint width, TPRREuint height);  /**< Calculates the total size of the MIP mapped texture in memory based on its basesize and size. */

    /** Gets the PRRE aniso texture filtering value that corresponds to the given floating point value. */
    static TPRRE_ANISO_TEX_FILTERING getPRREanisoTexFilteringNameFromFloat(TPRREfloat value);
    /** Gets the PRRE aniso texture filtering value that corresponds to the given integer value. */
    static TPRRE_ANISO_TEX_FILTERING getPRREanisoTexFilteringNameFromUInt(TPRREuint value);
    /** Gets the value that corresponds to the given PRRE aniso texture filtering value. */
    static TPRREfloat                getFloatFromPRREanisoTexFilteringName(TPRRE_ANISO_TEX_FILTERING value);
    /** Gets the value that corresponds to the given PRRE aniso texture filtering value. */
    static TPRREuint                 getUIntFromPRREanisoTexFilteringName(TPRRE_ANISO_TEX_FILTERING value);   

    // ---------------------------------------------------------------------------

    PRRETextureManager();             /**< Sets default values. */ /* TODO: mark this as noexcept(false) when using newer compiler! */
    virtual ~PRRETextureManager();

    CConsole&  getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */
    const char* getLoggerModuleName() const;          /**< Returns the logger module name of this class. */

    TPRREbool isInitialized() const;  /**< Tells whether the object is correctly initialized or not. */
  
    TPRREuint getUsedTextureMemory() const;                         /**< Gets the total amount of allocated VRAM by all of the textures. */
    TPRREbool isHardwareMipMapGenerationEnabled() const;            /**< Gets whether HW MIP map generation is enabled or not. */
    void      SetHardwareMipMapGenerationEnabled(TPRREbool state);  /**< Sets whether HW MIP map generation is enabled or not. */
    TPRREbool isNativeDIBFormatSupportEnabled() const;              /**< Gets whether direct upload of BGRA-format images is enabled or not. */
    void      SetNativeDIBFormatSupportEnabled(TPRREbool state);    /**< Sets whether direct upload of BGRA-format images is enabled or not. */
    TPRREbool isLazyInstancingEnabled() const;                      /**< Gets whether always creating new texture instances from the same file is enabled or not. */
    void      SetLazyInstancingEnabled(TPRREbool state);            /**< Sets whether always create new texture instances from the same file or not. */
    TPRREbool isPixelPreservingEnabled() const;                     /**< Gets whether keeping pixels of loaded textures in system memory is enabled or not. */
    void      SetPixelPreservingEnabled(TPRREbool state);           /**< Sets whether to keep pixels of loaded textures in system memory or delete them after texture uploading. */

    PRRETexture*         createTextureFromImage(const PRREImage& img);  /**< Creates texture from the given image. */
    virtual PRRETexture* createFromFile(const char* filename);          /**< Creates texture from the given file. */
    
    TPRRE_ISO_TEX_FILTERING getDefaultMinFilteringMode() const;           /**< Gets the default isotropic filtering mode when zooming out. */
    TPRRE_ISO_TEX_FILTERING getDefaultMagFilteringMode() const;           /**< Gets the default isotropic filtering mode when zooming in. */
    TPRREbool setDefaultMinFilteringMode(TPRRE_ISO_TEX_FILTERING filtering);   /**< Sets the default isotropic filtering mode when zooming out (when 1 texel < 1 pixel). */
    TPRREbool setDefaultMagFilteringMode(TPRRE_ISO_TEX_FILTERING filtering);   /**< Sets the default isotropic filtering mode when zooming in (when 1 texel > 1 pixel). */
    TPRREbool setDefaultIsoFilteringMode(
        TPRRE_ISO_TEX_FILTERING minfilter,
        TPRRE_ISO_TEX_FILTERING magfilter);                                    /**< Sets the default isotropic filtering modes. */ 
    
    TPRRE_ANISO_TEX_FILTERING
        getDefaultAnisoFilteringMode() const;      /**< Gets the default anisotropic filtering level. */
    TPRREbool
        setDefaultAnisoFilteringMode(
            TPRRE_ANISO_TEX_FILTERING filtering);  /**< Sets the default anisotropic filtering level. */
    TPRRE_ANISO_TEX_FILTERING getMaximumAnisoFiltering() const;       /**< Gets the maximum level of anisotropic filtering. */

    TPRRE_TEX_WRAPPING
        getDefaultTextureWrappingModeS() const;     /**< Gets the default texture wrapping mode for S coordinates. */
    TPRRE_TEX_WRAPPING
        getDefaultTextureWrappingModeT() const;     /**< Gets the default texture wrapping mode for T coordinates. */
    TPRREbool
        setDefaultTextureWrappingMode(
            TPRRE_TEX_WRAPPING tw_s,
            TPRRE_TEX_WRAPPING tw_t );              /**< Sets the default texture wrapping mode. */
    
    TPRRE_TEX_COMPRESSION_MODE
        getDefaultCompressionMode() const;     /**< Gets the default texture compression mode. */
    TPRREbool
        setDefaultCompressionMode(
            TPRRE_TEX_COMPRESSION_MODE mode);  /**< Sets the default texture compression mode. */

    TPRREbool getDefaultBorder() const;             /**< Gets the default border state. */
    TPRREbool setDefaultBorder(TPRREbool state);    /**< Sets the default border state. */

          PRREColor& getDefaultBorderColor();         /**< Gets default color of the border. */
    const PRREColor& getDefaultBorderColor() const;   /**< Gets default color of the border. */

    virtual void WriteList() const;            /**< From PRREImageManager, adding total texture memory usage counting. */

protected: 

    // ---------------------------------------------------------------------------

    PRRETextureManager(const PRRETextureManager&);
    PRRETextureManager& operator=(const PRRETextureManager&);

    virtual void WriteListCallback(const PRREManaged& mngd) const;  /**< From PRREImageManager, texture memory and internal number. */

private:

    class PRRETextureManagerImpl;
    PRRETextureManagerImpl* pImpl;

}; // class PRRETextureManager
