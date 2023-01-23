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
    static TPURE_ISO_TEX_FILTERING   getPureisoTexFilteringNameFromGLname(GLenum value);
    /** Gets the GL enum value that corresponds to the given Pure iso texture filtering value. */
    static GLenum                    getGLnameFromPureisoTexFilteringName(TPURE_ISO_TEX_FILTERING value);

    /** Gets the Pure texture wrapping mode value that corresponds to the given GL enum value. */
    static TPURE_TEX_WRAPPING getPuretexWrappingNameFromGLname(GLenum value);
    /** Gets the GL enum value that corresponds to the given Pure texture wrapping mode value. */
    static GLenum             getGLnameFromPuretexWrappingName(TPURE_TEX_WRAPPING value);

    // ---------------------------------------------------------------------------  

    virtual ~PureTextureImpl();
    
    TPURE_TEX_FORMAT getInternalFormat() const;   
    GLuint           getInternalNum() const;       
    
    TPURE_ISO_TEX_FILTERING getMinFilteringMode() const;  
    TPURE_ISO_TEX_FILTERING getMagFilteringMode() const;  
    TPureBool setMinFilteringMode(TPURE_ISO_TEX_FILTERING filtering);  
    TPureBool setMagFilteringMode(TPURE_ISO_TEX_FILTERING filtering);  
    TPureBool setIsoFilteringMode(TPURE_ISO_TEX_FILTERING minfilter,
        TPURE_ISO_TEX_FILTERING magfilter);                       

    TPureUInt getMIPmapCount() const;               

    TPURE_ANISO_TEX_FILTERING
        getAnisoFilteringMode() const;              
    TPureBool
        setAnisoFilteringMode(
            TPURE_ANISO_TEX_FILTERING filtering); 

    TPURE_TEX_WRAPPING
        getTextureWrappingModeS() const;
    TPURE_TEX_WRAPPING
        getTextureWrappingModeT() const;
    TPureBool
        setTextureWrappingMode(
            TPURE_TEX_WRAPPING tw_s,
            TPURE_TEX_WRAPPING tw_t );

    TPureBool        getBorder() const;
    TPureBool        setBorder(TPureBool state);
    const PureColor& getBorderColor() const;
    TPureBool        setBorderColor(const PureColor& clr);

    TPureBool uploadPixels();                             
    
    virtual TPureUInt getUsedSystemMemory() const;   
    TPureUInt         getUsedTextureMemory() const;  

protected:

    // ---------------------------------------------------------------------------  

private:

    PureTexture*  _pOwner;                 /**< The owner public object who creates this pimpl object. */

    TPureUInt nSize;                       /**< Amount of VRAM allocated (considering compression, MIP mapping, etc...). */
    TPURE_TEX_FORMAT texFormat;            /**< Texture format. */
    TPURE_ISO_TEX_FILTERING   filtIsoMin;  /**< Isotropic filtering mode when zooming out (1 texel < 1 pixel). */
    TPURE_ISO_TEX_FILTERING   filtIsoMag;  /**< Isotropic filtering mode when zooming in (1 texel > 1 pixel). */
    TPURE_ANISO_TEX_FILTERING filtAniso;   /**< Anisotropic filtering mode. */
    GLuint    nInternalNum;                /**< OpenGL texture ID. */
    TPureUInt nMIPmapCount;                /**< Number of MIP maps (>0). */
    TPURE_TEX_COMPRESSION_MODE texCompr;   /**< Texture compression mode. */
    TPureBool bBorder;                     /**< Texture has border or not. */
    PureColor clrBorder;                   /**< Texture border color. Black by default. */
    TPURE_TEX_WRAPPING twS, twT;           /**< Texture wrapping mode for S- and T- texture coordinates. */

    // ---------------------------------------------------------------------------

    PureTextureImpl(
        PureTexture* owner);    /**< Only PureTexture or PureTextureManager creates it. */

    PureTextureImpl(
        PureTexture* owner,
        const PureImage& img);             /**< Only PureTexture or PureTextureManager creates it this way. */
    
    PureTextureImpl(const PureTextureImpl&);
    PureTextureImpl& operator=(const PureTextureImpl&);

    GLenum getTransformedSourceFormat(
        TPURE_PIXEL_COMPONENT_ORDER oldco );    /**< Transforms to a proper pixel component order and returns the source format. */
    GLint  getTargetInternalFormat();           /**< Figures out the target internal format. */
    TPureBool actualUploadProc(
        GLint internalfmt, GLenum glTexFormat); /**< This contains the actual pixel upload code. */
    void   DescribeTexFormatAndSize(
        GLint internalfmt);                     /**< Updates texFormat and nSize members based on given internalfmt. */
   

    friend class PureTexture;
    friend class PureTextureManager;  

};
