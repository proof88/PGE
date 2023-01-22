#pragma once

/*
    ###################################################################################
    PureTextureImpl.h
    This file is part of PURE.
    Internal header.
    PureTextureImpl class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../../external/PureAllHeaders.h"
#include "../../external/Material/PureTextureManager.h"
#include "gl/gl.h"  // for GLenum and similar, which should be removed from here soon ...

class PureTexture::PureTextureImpl
{

public:
    /** Gets the Pure iso texture filtering value that corresponds to the given GL enum value. */
    static TPure_ISO_TEX_FILTERING   getPureisoTexFilteringNameFromGLname(GLenum value);
    /** Gets the GL enum value that corresponds to the given Pure iso texture filtering value. */
    static GLenum                    getGLnameFromPureisoTexFilteringName(TPure_ISO_TEX_FILTERING value);

    /** Gets the Pure texture wrapping mode value that corresponds to the given GL enum value. */
    static TPure_TEX_WRAPPING getPuretexWrappingNameFromGLname(GLenum value);
    /** Gets the GL enum value that corresponds to the given Pure texture wrapping mode value. */
    static GLenum             getGLnameFromPuretexWrappingName(TPure_TEX_WRAPPING value);

    // ---------------------------------------------------------------------------  

    virtual ~PureTextureImpl();
    
    TPure_TEX_FORMAT getInternalFormat() const;   
    GLuint           getInternalNum() const;       
    
    TPure_ISO_TEX_FILTERING getMinFilteringMode() const;  
    TPure_ISO_TEX_FILTERING getMagFilteringMode() const;  
    TPurebool setMinFilteringMode(TPure_ISO_TEX_FILTERING filtering);  
    TPurebool setMagFilteringMode(TPure_ISO_TEX_FILTERING filtering);  
    TPurebool setIsoFilteringMode(TPure_ISO_TEX_FILTERING minfilter,
        TPure_ISO_TEX_FILTERING magfilter);                       

    TPureuint getMIPmapCount() const;               

    TPure_ANISO_TEX_FILTERING
        getAnisoFilteringMode() const;              
    TPurebool
        setAnisoFilteringMode(
            TPure_ANISO_TEX_FILTERING filtering); 

    TPure_TEX_WRAPPING
        getTextureWrappingModeS() const;
    TPure_TEX_WRAPPING
        getTextureWrappingModeT() const;
    TPurebool
        setTextureWrappingMode(
            TPure_TEX_WRAPPING tw_s,
            TPure_TEX_WRAPPING tw_t );

    TPurebool        getBorder() const;
    TPurebool        setBorder(TPurebool state);
    const PureColor& getBorderColor() const;
    TPurebool        setBorderColor(const PureColor& clr);

    TPurebool uploadPixels();                             
    
    virtual TPureuint getUsedSystemMemory() const;   
    TPureuint         getUsedTextureMemory() const;  

protected:

    // ---------------------------------------------------------------------------  

private:

    PureTexture*  _pOwner;                 /**< The owner public object who creates this pimpl object. */

    TPureuint nSize;                       /**< Amount of VRAM allocated (considering compression, MIP mapping, etc...). */
    TPure_TEX_FORMAT texFormat;            /**< Texture format. */
    TPure_ISO_TEX_FILTERING   filtIsoMin;  /**< Isotropic filtering mode when zooming out (1 texel < 1 pixel). */
    TPure_ISO_TEX_FILTERING   filtIsoMag;  /**< Isotropic filtering mode when zooming in (1 texel > 1 pixel). */
    TPure_ANISO_TEX_FILTERING filtAniso;   /**< Anisotropic filtering mode. */
    GLuint    nInternalNum;                /**< OpenGL texture ID. */
    TPureuint nMIPmapCount;                /**< Number of MIP maps (>0). */
    TPure_TEX_COMPRESSION_MODE texCompr;   /**< Texture compression mode. */
    TPurebool bBorder;                     /**< Texture has border or not. */
    PureColor clrBorder;                   /**< Texture border color. Black by default. */
    TPure_TEX_WRAPPING twS, twT;           /**< Texture wrapping mode for S- and T- texture coordinates. */

    // ---------------------------------------------------------------------------

    PureTextureImpl(
        PureTexture* owner);    /**< Only PureTexture or PureTextureManager creates it. */

    PureTextureImpl(
        PureTexture* owner,
        const PureImage& img);             /**< Only PureTexture or PureTextureManager creates it this way. */
    
    PureTextureImpl(const PureTextureImpl&);
    PureTextureImpl& operator=(const PureTextureImpl&);

    GLenum getTransformedSourceFormat(
        TPure_PIXEL_COMPONENT_ORDER oldco );    /**< Transforms to a proper pixel component order and returns the source format. */
    GLint  getTargetInternalFormat();           /**< Figures out the target internal format. */
    TPurebool actualUploadProc(
        GLint internalfmt, GLenum glTexFormat); /**< This contains the actual pixel upload code. */
    void   DescribeTexFormatAndSize(
        GLint internalfmt);                     /**< Updates texFormat and nSize members based on given internalfmt. */
   

    friend class PureTexture;
    friend class PureTextureManager;  

};
