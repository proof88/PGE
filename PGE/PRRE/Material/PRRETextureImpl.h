#pragma once

/*
    ###################################################################################
    PRRETextureImpl.h
    This file is part of PRRE.
    Internal header.
    PRRETextureImpl class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../PRREallHeaders.h"
#include "PRRETextureManager.h"
#include "gl/gl.h"  // for GLenum and similar, which should be removed from here soon ...

class PRRETexture::PRRETextureImpl
{

public:
    /** Gets the PRRE iso texture filtering value that corresponds to the given GL enum value. */
    static TPRRE_ISO_TEX_FILTERING   getPRREisoTexFilteringNameFromGLname(GLenum value);
    /** Gets the GL enum value that corresponds to the given PRRE iso texture filtering value. */
    static GLenum                    getGLnameFromPRREisoTexFilteringName(TPRRE_ISO_TEX_FILTERING value);

    /** Gets the PRRE texture wrapping mode value that corresponds to the given GL enum value. */
    static TPRRE_TEX_WRAPPING getPRREtexWrappingNameFromGLname(GLenum value);
    /** Gets the GL enum value that corresponds to the given PRRE texture wrapping mode value. */
    static GLenum             getGLnameFromPRREtexWrappingName(TPRRE_TEX_WRAPPING value);

    // ---------------------------------------------------------------------------  

    virtual ~PRRETextureImpl();
    
    TPRRE_TEX_FORMAT getInternalFormat() const;   
    GLuint           getInternalNum() const;       
    
    TPRRE_ISO_TEX_FILTERING getMinFilteringMode() const;  
    TPRRE_ISO_TEX_FILTERING getMagFilteringMode() const;  
    void SetMinFilteringMode(TPRRE_ISO_TEX_FILTERING filtering);  
    void SetMagFilteringMode(TPRRE_ISO_TEX_FILTERING filtering);  
    void SetIsoFilteringMode(TPRRE_ISO_TEX_FILTERING minfilter,
        TPRRE_ISO_TEX_FILTERING magfilter);                       

    TPRREuint getMIPmapCount() const;               

    TPRRE_ANISO_TEX_FILTERING
        getAnisoFilteringMode() const;              
    void
        SetAnisoFilteringMode(
            TPRRE_ANISO_TEX_FILTERING filtering); 

    TPRRE_TEX_WRAPPING
        getTextureWrappingModeS() const;
    TPRRE_TEX_WRAPPING
        getTextureWrappingModeT() const;
    void
        SetTextureWrappingMode(
            TPRRE_TEX_WRAPPING tw_s,
            TPRRE_TEX_WRAPPING tw_t );

          TPRREbool  getBorder() const;
    void             SetBorder(TPRREbool state);
    const PRREColor& getBorderColor() const;
    void             SetBorderColor(const PRREColor& clr);

    TPRREbool uploadPixels();                             
    
    virtual TPRREuint getUsedSystemMemory() const;   
    TPRREuint         getUsedTextureMemory() const;  

protected:

    // ---------------------------------------------------------------------------  

private:

    PRRETexture*  _pOwner;                 /**< The owner public object who creates this pimpl object. */

    TPRREuint nSize;                       /**< Amount of VRAM allocated (considering compression, MIP mapping, etc...). */
    TPRRE_TEX_FORMAT texFormat;            /**< Texture format. */
    TPRRE_ISO_TEX_FILTERING   filtIsoMin;  /**< Isotropic filtering mode when zooming out (1 texel < 1 pixel). */
    TPRRE_ISO_TEX_FILTERING   filtIsoMag;  /**< Isotropic filtering mode when zooming in (1 texel > 1 pixel). */
    TPRRE_ANISO_TEX_FILTERING filtAniso;   /**< Anisotropic filtering mode. */
    GLuint    nInternalNum;                /**< OpenGL texture ID. */
    TPRREuint nMIPmapCount;                /**< Number of MIP maps (>0). */
    TPRRE_TEX_COMPRESSION_MODE texCompr;   /**< Texture compression mode. */
    TPRREbool bBorder;                     /**< Texture has border or not. */
    PRREColor clrBorder;                   /**< Texture border color. Black by default. */
    TPRRE_TEX_WRAPPING twS, twT;           /**< Texture wrapping mode for S- and T- texture coordinates. */

    // ---------------------------------------------------------------------------

    PRRETextureImpl(
        PRRETexture* owner);    /**< Only PRRETexture or PRRETextureManager creates it. */

    PRRETextureImpl(
        PRRETexture* owner,
        const PRREImage& img);             /**< Only PRRETexture or PRRETextureManager creates it this way. */
    
    PRRETextureImpl(const PRRETextureImpl&);
    PRRETextureImpl& operator=(const PRRETextureImpl&);

    CConsole& getConsole() const;               /**< Hack to be able to use CConsole singleton instance instead of owner's protected console instance. */

    GLenum getTransformedSourceFormat(
        TPRRE_PIXEL_COMPONENT_ORDER oldco );    /**< Transforms to a proper pixel component order and returns the source format. */
    GLint  getTargetInternalFormat();           /**< Figures out the target internal format. */
    void   ActualUploadProc(
        GLint internalfmt, GLenum glTexFormat); /**< This contains the actual pixel upload code. */
    void   DescribeTexFormatAndSize(
        GLint internalfmt);                     /**< Updates texFormat and nSize members based on given internalfmt. */
   

    friend class PRRETexture;
    friend class PRRETextureManager;  

};
