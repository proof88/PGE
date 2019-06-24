/*
    ###################################################################################
    PRRETextureManager.cpp
    This file is part of PRRE.
    PRRETextureManager class.
    PRRETexture class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREbaseIncludes.h"  // PCH
#include "PRREpragmas.h"
#include "PRRETextureManager.h"
#include "PRREhwInfo.h"
#include "PRREGLsnippets.h"

using namespace std;

// TODO: following few DD structs may be needed IN THE FUTURE when we add DDS format support, so keeping them here ...

/**
    DirectDraw pixel format.
    http://msdn.microsoft.com/en-us/library/aa918568.aspx
*/
struct DDPIXELFORMAT
{
    DWORD dwSize;
    DWORD dwFlags;
    DWORD dwFourCC;
    DWORD dwRGBBitCount;
    DWORD dwRBitMask;
    DWORD dwGBitMask;
    DWORD dwBBitMask;
    DWORD dwRGBAlphaBitMask;
}; // DDPIXELFORMAT


/**
    Capabilities of a DirectDrawSurface object.

    http://msdn.microsoft.com/en-us/library/ms907797.aspx
*/
struct DDSCAPS2
{
    DWORD dwCaps1;
    DWORD dwCaps2;
    DWORD dwCaps3;
    DWORD dwCaps4;
}; // DDSCAPS2


/**
    DirectDrawSurface descriptor.

    http://msdn.microsoft.com/en-us/library/windows/desktop/gg426114%28v=vs.85%29.aspx
*/
struct DDSURFACEDESC2
{
    DWORD dwSize;
    DWORD dwFlags;
    DWORD dwHeight;
    DWORD dwWidth;
    DWORD dwPitchOrLinearSize;
    DWORD dwDepth;
    DWORD dwMipMapCount;
    DWORD dwReserved1[11];
    DDPIXELFORMAT ddpfPixelFormat;
    DDSCAPS2 ddsCaps;
    DWORD dwReserved2;
}; // DDSURFACEDESC2


/*
   PRRETexture::PRRETextureImpl
   ###########################################################################
*/


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


// ############################### PUBLIC ################################


/**
    Gets the PRRE iso texture filtering value that corresponds to the given GL enum value.
    @return A GL texture filtering enum value corresponding to the given value.
            PRRE_ISO_LINEAR if invalid value is given.
*/
TPRRE_ISO_TEX_FILTERING PRRETexture::PRRETextureImpl::getPRREisoTexFilteringNameFromGLname(GLenum value)
{
    switch ( value )
    {
    case GL_NEAREST: return PRRE_ISO_NEAREST;
    case GL_LINEAR:  return PRRE_ISO_LINEAR;
    case GL_NEAREST_MIPMAP_NEAREST: return PRRE_ISO_NEAREST_MIPMAP_NEAREST;
    case GL_NEAREST_MIPMAP_LINEAR:  return PRRE_ISO_NEAREST_MIPMAP_LINEAR;
    case GL_LINEAR_MIPMAP_NEAREST:  return PRRE_ISO_LINEAR_MIPMAP_NEAREST;
    case GL_LINEAR_MIPMAP_LINEAR:   return PRRE_ISO_LINEAR_MIPMAP_LINEAR;
    default: return PRRE_ISO_LINEAR;
    }    
} // getPRREisoTexFilteringNameFromGLname()


/**
    Gets the GL enum value that corresponds to the given PRRE iso texture filtering value.
    @return A GL texture filtering enum value corresponding to the given value.
            GL_LINEAR if invalid value is given.
*/
GLenum PRRETexture::PRRETextureImpl::getGLnameFromPRREisoTexFilteringName(TPRRE_ISO_TEX_FILTERING value)
{
    switch ( value )
    {
    case PRRE_ISO_NEAREST: return GL_NEAREST;
    case PRRE_ISO_LINEAR:  return GL_LINEAR;
    case PRRE_ISO_NEAREST_MIPMAP_NEAREST: return GL_NEAREST_MIPMAP_NEAREST;
    case PRRE_ISO_NEAREST_MIPMAP_LINEAR:  return GL_NEAREST_MIPMAP_LINEAR;
    case PRRE_ISO_LINEAR_MIPMAP_NEAREST:  return GL_LINEAR_MIPMAP_NEAREST;
    case PRRE_ISO_LINEAR_MIPMAP_LINEAR:   return GL_LINEAR_MIPMAP_LINEAR;
    default: return GL_LINEAR;
    }
} // getGLnameFromPRREisoTexFilteringName()


/**
    Gets the PRRE texture wrapping mode value that corresponds to the given GL enum value.
    @return A PRRE texture wrapping mode value corresponding to the given GL value.
            PRRE_TW_REPEAT if invalid value is given.
*/
TPRRE_TEX_WRAPPING PRRETexture::PRRETextureImpl::getPRREtexWrappingNameFromGLname(GLenum value)
{
    switch (value)
    {
    case GL_CLAMP           : return PRRE_TW_CLAMP;
    case GL_REPEAT          : return  PRRE_TW_REPEAT;
    case GL_MIRRORED_REPEAT : return PRRE_TW_MIRRORED_REPEAT;
    case GL_CLAMP_TO_BORDER : return PRRE_TW_CLAMP_TO_BORDER;
    case GL_CLAMP_TO_EDGE   : return PRRE_TW_CLAMP_TO_EDGE;
    default: return PRRE_TW_REPEAT;
    }    
}


/**
    Gets the GL enum value that corresponds to the given PRRE texture wrapping mode value.
    @return A GL texture wrapping enum value corresponding to the given value.
            GL_REPEAT if invalid value is given.
*/
GLenum  PRRETexture::PRRETextureImpl::getGLnameFromPRREtexWrappingName(TPRRE_TEX_WRAPPING value)
{
    switch (value)
    {
    case PRRE_TW_CLAMP           : return GL_CLAMP;
    case PRRE_TW_REPEAT          : return GL_REPEAT;
    case PRRE_TW_MIRRORED_REPEAT : return GL_MIRRORED_REPEAT;
    case PRRE_TW_CLAMP_TO_BORDER : return GL_CLAMP_TO_BORDER;
    case PRRE_TW_CLAMP_TO_EDGE   : return GL_CLAMP_TO_EDGE;
    default: return GL_REPEAT;
    }
}


PRRETexture::PRRETextureImpl::~PRRETextureImpl()
{
    _pOwner->getConsole().OLnOI("~PRRETexture() ...");
    if ( nInternalNum > 0 )
        glDeleteTextures(1, &nInternalNum);
    _pOwner->getConsole().SOLnOO("Done!");
    _pOwner = NULL;
} // ~PRRETexture()


TPRRE_TEX_FORMAT PRRETexture::PRRETextureImpl::getInternalFormat() const
{
    return texFormat;
} // getInternalFormat()


GLuint PRRETexture::PRRETextureImpl::getInternalNum() const
{
    return nInternalNum;
} // getInternalNum()


TPRRE_ISO_TEX_FILTERING PRRETexture::PRRETextureImpl::getMinFilteringMode() const
{
    return filtIsoMin;
} // getMinFilteringMode()


TPRRE_ISO_TEX_FILTERING PRRETexture::PRRETextureImpl::getMagFilteringMode() const
{
    return filtIsoMag;
} // getMagFilteringMode()


void PRRETexture::PRRETextureImpl::SetMinFilteringMode(TPRRE_ISO_TEX_FILTERING filtering)
{
    if ( (filtering != PRRE_ISO_NEAREST) && (filtering != PRRE_ISO_LINEAR) && (nMIPmapCount == 1) )
        return;

    filtIsoMin = filtering;
    if ( PRREhwInfo::get().getVideo().isAcceleratorDetected() )
    {
        glBindTexture(GL_TEXTURE_2D, nInternalNum);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, getGLnameFromPRREisoTexFilteringName(filtIsoMin));
    }
} // SetMinFilteringMode()


void PRRETexture::PRRETextureImpl::SetMagFilteringMode(TPRRE_ISO_TEX_FILTERING filtering)
{
    if ( (filtering == PRRE_ISO_NEAREST) || (filtering == PRRE_ISO_LINEAR) )
    {
        filtIsoMag = filtering;
        if ( PRREhwInfo::get().getVideo().isAcceleratorDetected() )
        {
            glBindTexture(GL_TEXTURE_2D, nInternalNum);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, getGLnameFromPRREisoTexFilteringName(filtIsoMag));
        }
    }
} // SetMagFilteringMode()


void PRRETexture::PRRETextureImpl::SetIsoFilteringMode(TPRRE_ISO_TEX_FILTERING minfilter, TPRRE_ISO_TEX_FILTERING magfilter)
{
    SetMinFilteringMode( minfilter );
    SetMagFilteringMode( magfilter );
} // SetIsoFilteringMode()


TPRREuint PRRETexture::PRRETextureImpl::getMIPmapCount() const 
{
    return nMIPmapCount;
} // getMIPmapCount()


TPRRE_ANISO_TEX_FILTERING PRRETexture::PRRETextureImpl::getAnisoFilteringMode() const
{
    return filtAniso;
} // getAnisoFilteringMode()


void PRRETexture::PRRETextureImpl::SetAnisoFilteringMode(TPRRE_ANISO_TEX_FILTERING filtering)
{
    _pOwner->getConsole().OIOLn("PRRETexture::SetAnisoFilteringMode(%fX)", PRRETextureManager::getFloatFromPRREanisoTexFilteringName(filtering));
    
    if ( _pOwner->getManager() == PGENULL )
        return;
    
    if ( !PRREhwInfo::get().getVideo().isAcceleratorDetected() )
    {
        _pOwner->getConsole().EOLnOO("WARNING: anisotropic filtering is not supported without acceleration!");
        return;
    }

    const TPRREfloat newFilteringF = ((PRRETextureManager*)_pOwner->getManager())->getFloatFromPRREanisoTexFilteringName(filtering);
    const TPRREfloat availMaxFilteringF = ((PRRETextureManager*)_pOwner->getManager())->getFloatFromPRREanisoTexFilteringName(
        ((PRRETextureManager*)_pOwner->getManager())->getMaximumAnisoFiltering() );
    // in Release mode, although getMaximumAnisoFiltering() returns ANISO_16X, availMaxFilteringF becomes 0.
    // This can be fixed by adding a console printout to getFloatFromPRREanisoTexFilteringName(), so probably this is some
    // compiler optimization bug which doesnt happen in Debug mode. Would be good to play with the Release mode compiler options
    // to see exactly which option causes this. This is weird because getFloatFromPRREanisoTexFilteringName() can never ever be 0.
    // _pOwner->getConsole().OLn( "availMaxFilteringF: %f",  availMaxFilteringF);
    if ( availMaxFilteringF > 1.0f )
    {
        glBindTexture(GL_TEXTURE_2D, nInternalNum);
        if ( availMaxFilteringF >= newFilteringF )
        {
            filtAniso = filtering;
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, (GLfloat) newFilteringF);
        }
        else
        {
            _pOwner->getConsole().EOLn("WARNING: selected mode not supported, setting max supported: %fX!", availMaxFilteringF);
            filtAniso = PRRETextureManager::getPRREanisoTexFilteringNameFromFloat(availMaxFilteringF);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, (GLfloat) availMaxFilteringF);
        }
    }
    else
    {
        _pOwner->getConsole().EOLn("ERROR: anisotropic filtering is not supported");
    }
    _pOwner->getConsole().OO();
} // SetAnisoFilteringMode()


TPRRE_TEX_WRAPPING PRRETexture::PRRETextureImpl::getTextureWrappingModeS() const
{
    return twS;
}


TPRRE_TEX_WRAPPING PRRETexture::PRRETextureImpl::getTextureWrappingModeT() const
{
    return twT;
}


void PRRETexture::PRRETextureImpl::SetTextureWrappingMode(TPRRE_TEX_WRAPPING tw_s, TPRRE_TEX_WRAPPING tw_t )
{
    // TODO: probably we should check for availability of given mode on current HW, especially with PRRE_TW_MIRRORED_REPEAT and PRRE_TW_CLAMP_TO_BORDER
    twS = tw_s;
    twT = tw_t;
    if ( PRREhwInfo::get().getVideo().isAcceleratorDetected() )
    {
        glBindTexture(GL_TEXTURE_2D, nInternalNum);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, getGLnameFromPRREtexWrappingName(twS));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, getGLnameFromPRREtexWrappingName(twT));
    }
    
}


TPRREbool PRRETexture::PRRETextureImpl::getBorder() const
{
    return bBorder;
}


void PRRETexture::PRRETextureImpl::SetBorder(TPRREbool state)
{
    bBorder = state;
}


const PRREColor& PRRETexture::PRRETextureImpl::getBorderColor() const
{
    return clrBorder;
}


void PRRETexture::PRRETextureImpl::SetBorderColor(const PRREColor& clr)
{
    clrBorder = clr;

    if ( PRREhwInfo::get().getVideo().isAcceleratorDetected() )
    {
        if ( nInternalNum > 0 )
        {
            GLfloat color[4];
            color[0] = clrBorder.getRedAsFloat();
            color[1] = clrBorder.getGreenAsFloat();
            color[2] = clrBorder.getBlueAsFloat();
            color[3] = clrBorder.getAlphaAsFloat();
        
            glBindTexture(GL_TEXTURE_2D, nInternalNum);
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
        }
    }
}


TPRREbool PRRETexture::PRRETextureImpl::uploadPixels()
{
    _pOwner->getConsole().OIOLn("PRRETexture::uploadPixels()");

    if ( _pOwner->getPixels() == PGENULL )
    {
        _pOwner->getConsole().EOLnOO("ERROR: getPixels() returned NULL!");
        return false;
    }

    if ( _pOwner->getManager() == PGENULL )
    {
        _pOwner->getConsole().EOLnOO("ERROR: getManager() returned NULL!");
        return false;
    }

    if ( strcmp(typeid(*_pOwner->getManager()).raw_name(), typeid(PRRETextureManager&).raw_name()) )
    {
        _pOwner->getConsole().EOLnOO("ERROR: getManager() type is not PRRETextureManager!");
        return false;
    }

    if ( PRREhwInfo::get().getVideo().isAcceleratorDetected() )
    {
        glBindTexture(GL_TEXTURE_2D, nInternalNum);
    }

    const TPRRE_PIXEL_COMPONENT_ORDER oldco = _pOwner->getPixelComponentOrder();
    const GLenum glTexFormat = getTransformedSourceFormat(oldco);
    const GLint  internalfmt = getTargetInternalFormat();

    ActualUploadProc(internalfmt, glTexFormat);    
    DescribeTexFormatAndSize(internalfmt);

    if ( _pOwner->getPixelComponentOrder() != oldco )
        _pOwner->SetPixelComponentOrder( oldco );

    _pOwner->getConsole().OO();

    return true;
} // uploadPixels()


TPRREuint PRRETexture::PRRETextureImpl::getUsedSystemMemory() const
{
    // no need to calculate with this->_pOwner->getPixelsSize() + here as PRRETexture::getUsedSystemMemory() already calls PRREImage::getUsedSystemMemory() too
    return sizeof(*this);
} // getUsedSystemMemory()


TPRREuint PRRETexture::PRRETextureImpl::getUsedTextureMemory() const
{
    return nSize;
} // getUsedTextureMemory()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


/**
    Only PRRETextureManager creates it.

    @param owner The public class owning this pimpl object.
*/
PRRETexture::PRRETextureImpl::PRRETextureImpl(PRRETexture* owner)
{
    _pOwner = owner;
    _pOwner->getConsole().OLnOI("PRRETexture() ...");
    _pOwner->getConsole().SOLnOO("Done!");
} // PRRETexture()


/**
    Only PRRETextureManager creates it this way.
*/
PRRETexture::PRRETextureImpl::PRRETextureImpl(PRRETexture* owner, const PRREImage& img)
{
    _pOwner = owner;
    texFormat  = PRRE_TF_RGBA;
    filtIsoMin = filtIsoMag = PRRE_ISO_LINEAR;
    filtAniso  = PRRE_ANISO_1X;
    nInternalNum = 0;
    nMIPmapCount = 1;
    texCompr = PRRE_TC_NONE;
    bBorder = false;
    twS = PRRE_TW_REPEAT;
    twT = PRRE_TW_REPEAT;
} // PRRETexture(...)


PRRETexture::PRRETextureImpl::PRRETextureImpl(const PRRETexture::PRRETextureImpl&)
{

}


PRRETexture::PRRETextureImpl& PRRETexture::PRRETextureImpl::operator=(const PRRETexture::PRRETextureImpl&)
{
    return *this;
}


/**
    Hack to be able to use CConsole singleton instance instead of owner's protected console instance.
    TODO: maybe the whole getConsole() stuff should be removed from everywhere because CConsole is a singleton class after all.
          No reason to think like different classes have different console instances, I don't see usecase for that.
*/
CConsole& PRRETexture::PRRETextureImpl::getConsole() const
{
    return CConsole::getConsoleInstance();
}


/**
    Transforms to a proper pixel component order and returns the proper OpenGL source format.
    For example, if the image's color component order is PRRE_BGR and OpenGL driver supports that format natively, no transform is needed, respective OpenGL source format will be returned quickly.
    However if driver doesn't have support for that, then color component order first gets transformed to PRRE_RGB, and then respective OpenGL source format will be returned.

    @return OpenGL source format enum.
*/
GLenum PRRETexture::PRRETextureImpl::getTransformedSourceFormat(TPRRE_PIXEL_COMPONENT_ORDER oldco)
{
    const PRRETextureManager& parent = *((PRRETextureManager*) _pOwner->getManager());
    const PRREhwInfo& hwInfo = PRREhwInfo::get();

    if ( oldco == PRRE_BGR )
    {
        if ( hwInfo.getVideo().isNativeDIBFormatSupported() && parent.isNativeDIBFormatSupportEnabled() )
            return GL_BGR_EXT;
        else
        {
            _pOwner->SetPixelComponentOrder(PRRE_RGB);
            return GL_RGB;
        }
    }
    else if ( oldco == PRRE_BGRA )
    {
        if ( hwInfo.getVideo().isNativeDIBFormatSupported() && parent.isNativeDIBFormatSupportEnabled() )
            return GL_BGRA_EXT;
        else
        {
            _pOwner->SetPixelComponentOrder(PRRE_RGBA);
            return GL_RGBA;
        }
    }
    else if ( _pOwner->getPixelComponentOrder() % 2 == 0 )
    {
        if ( oldco != PRRE_RGB )
            _pOwner->SetPixelComponentOrder(PRRE_RGB);
        return GL_RGB;
    }
    else
    {
        if ( oldco != PRRE_RGBA )
            _pOwner->SetPixelComponentOrder(PRRE_RGBA);
        return GL_RGBA;
    }
} // getTransformedSourceFormat()


/**
    Figures out the target internal format.
    Used when uploading pixels as a texture to the VGA.

    @return OpenGL target internal format for storing texture in VRAM.
*/
GLint PRRETexture::PRRETextureImpl::getTargetInternalFormat()
{
    const PRRETextureManager& parent = *((PRRETextureManager*) _pOwner->getManager());
    const PRREhwInfo& hwInfo = PRREhwInfo::get();

    if ( hwInfo.getVideo().isTextureCompressionSupported() )
    {
        switch ( parent.getDefaultCompressionMode() )
        {
        case PRRE_TC_NONE: return _pOwner->getPixelComponentOrder() % 2 == 0 ? 3 : 4;
        case PRRE_TC_S3TC_RGB_DXT1 : return GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
        case PRRE_TC_S3TC_RGBA_DXT1: return GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
        case PRRE_TC_S3TC_RGBA_DXT3: return GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
        case PRRE_TC_S3TC_RGBA_DXT5: return GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
        case PRRE_TC_3DFX_RGB_FXT1 : return GL_COMPRESSED_RGB_FXT1_3DFX;
        case PRRE_TC_3DFX_RGBA_FXT1: return GL_COMPRESSED_RGBA_FXT1_3DFX;
        /* PRRE_TC_AUTO causes the driver to select any of the above */
        default: /* for PRRE_TC_AUTO */ return _pOwner->getPixelComponentOrder() % 2 == 0 ? GL_COMPRESSED_RGB_ARB : GL_COMPRESSED_RGBA_ARB;
        }
    }
    else
        return _pOwner->getPixelComponentOrder() % 2 == 0 ? 3 : 4;
} // getTargetInternalFormat()


/**
    This contains the actual pixel upload code.

    The following properties of the given texture object should be set properly before calling this method:
     - nMIPmapCount: should be a positive value; if 1, no MIP maps will be generated, otherwise MIP maps will be generated.
     - nWidth, nHeight: although not used by this method (because the method uses sizes of the given Image), it is recommended to be already defined.

    @param internalfmt OpenGL target internal format for storing texture in VRAM. Suitable can be selected by getTargetInternalFormat().
    @param glTexFormat OpenGL source format, how to interpret input. Suitable can be selected by getTransformedSourceFormat().
*/
void PRRETexture::PRRETextureImpl::ActualUploadProc(GLint internalfmt, GLenum glTexFormat)
{
    const PRRETextureManager& parent = *((PRRETextureManager*) _pOwner->getManager());
    const PRREhwInfo& hwInfo = PRREhwInfo::get();

    if ( !hwInfo.getVideo().isAcceleratorDetected() )
    {
        getConsole().OLn("no need to upload texture, non-accelerated rendering is active");
        return;
    }

    if ( nMIPmapCount > 1 )
    {
        getConsole().OLn("mipmapping on");
        getConsole().O("auto-generating mipmaps with ");
        if ( hwInfo.getVideo().isHardwareMipMapGenerationSupported() && parent.isHardwareMipMapGenerationEnabled() )
        {        
            getConsole().OLn("HW-support.");
            glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
            
            // WARNING: up to OpenGL 2.x, border is allowed to be 0 or 1. However from GL 3.0 it must be 0.
            // WARNING: if GL version does not support NPOT sizes, width must be 2n+2(border) for some integer n.
            // WARNING: if GL version does not support NPOT sizes, height must be 2m+2(border) for some integer m.
            // TODO: So in the future when I add support for NPOT textures, I need to consider border value when specifying width and height.
            glTexImage2D(GL_TEXTURE_2D, 0, internalfmt, _pOwner->getWidth(), _pOwner->getHeight(), bBorder ? (GLint) 1 : (GLint) 0, glTexFormat, GL_UNSIGNED_BYTE, _pOwner->getPixels());
        }
        else
        {
            getConsole().OLn("SW-only.");
            gluBuild2DMipmaps(GL_TEXTURE_2D, internalfmt, _pOwner->getWidth(), _pOwner->getHeight(), glTexFormat, GL_UNSIGNED_BYTE, _pOwner->getPixels());
        }
        getConsole().OLn("done!");
    }
    else
    {
        getConsole().OLn("mipmapping off");

        // WARNING: up to OpenGL 2.x, border is allowed to be 0 or 1. However from GL 3.0 it must be 0.
        // WARNING: if GL version does not support NPOT sizes, width must be 2n+2(border) for some integer n.
        // WARNING: if GL version does not support NPOT sizes, height must be 2m+2(border) for some integer m.
        // TODO: So in the future when I add support for NPOT textures, I need to consider border value when specifying width and height.
        glTexImage2D(GL_TEXTURE_2D, 0, internalfmt, _pOwner->getWidth(), _pOwner->getHeight(), bBorder ? (GLint) 1 : (GLint) 0, glTexFormat, GL_UNSIGNED_BYTE, _pOwner->getPixels());
    }
} // ActualUploadProc()


/**
    Internal format (texFormat) and used texture memory (nSize) will be updated based on given internalfmt.
    This is needed so that real size and compression is queried from OpenGL after the texture has been uploaded.
    Not doing much when software rendering is selected.
*/
void PRRETexture::PRRETextureImpl::DescribeTexFormatAndSize(GLint internalfmt)
{
    nSize = _pOwner->getWidth() * _pOwner->getHeight() * 4;
    texFormat = PRRE_TF_RGBA;

    if ( !PRREhwInfo::get().getVideo().isAcceleratorDetected() )
    {
        return;
    }

    if ( (internalfmt != 3) && (internalfmt != 4) )
    {
        GLint tmpret = 0;
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED, &tmpret);
        if ( tmpret == GL_TRUE )
        {
            glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &tmpret);
            getConsole().O("final internal texFormat is ");
            switch ( tmpret )
            {
            case GL_RGBA:
                getConsole().OLn("GL_RGBA");
                break;
            case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
                texFormat = PRRE_TF_DXT1;
                getConsole().OLn("GL_COMPRESSED_RGB_S3TC_DXT1_EXT");
                break;
            case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
                texFormat = PRRE_TF_DXT1A;
                getConsole().OLn("GL_COMPRESSED_RGBA_S3TC_DXT1_EXT");
                break;
            case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
                texFormat = PRRE_TF_DXT3A;
                getConsole().OLn("GL_COMPRESSED_RGBA_S3TC_DXT3_EXT");
                break;
            case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
                texFormat = PRRE_TF_DXT5A;
                getConsole().OLn("GL_COMPRESSED_RGBA_S3TC_DXT5_EXT");
                break;
            case GL_COMPRESSED_RGB_FXT1_3DFX:
                texFormat = PRRE_TF_FXT1;
                getConsole().OLn("GL_COMPRESSED_RGB_FXT1_3DFX");
                break;
            case GL_COMPRESSED_RGBA_FXT1_3DFX:
                texFormat = PRRE_TF_FXT1A;
                getConsole().OLn("GL_COMPRESSED_RGBA_FXT1_3DFX");
                break;
            /* In 14.12 AMD Catalyst Omega Software, requesting auto-compression at glTexImage2D() returns general compression here instead of any if the specific values above. */
            case GL_COMPRESSED_RGB_ARB:
                texFormat = PRRE_TF_UNSURE;
                getConsole().OLn("GL_COMPRESSED_RGB_ARB");
                break;
            case GL_COMPRESSED_RGBA_ARB:
                texFormat = PRRE_TF_UNSURE;
                getConsole().OLn("GL_COMPRESSED_RGBA_ARB");
                break;
            default:
                getConsole().EOLn("ERROR: glGetTexLevelParameteriv() returned %d for GL_TEXTURE_INTERNAL_FORMAT", tmpret);
                break;
            }
            glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &tmpret);
            nSize = tmpret;
        }
    }
    if ( nMIPmapCount > 1 )
    {
        nSize = PRRETextureManager::getMIPmappedTexSize(nSize, _pOwner->getWidth(), _pOwner->getHeight());
    }
} // DescribeTexFormatAndSize()


/*
   PRRETexture
   ###########################################################################
*/


// ############################### PUBLIC ################################


PRRETexture::~PRRETexture()
{
    delete p;
    p = NULL;
} // ~PRRETexture()


/**
    Gets the internal texture format.    
    @return The internal texture format.
*/
TPRRE_TEX_FORMAT PRRETexture::getInternalFormat() const
{
    return p->getInternalFormat();
} // getInternalFormat()


/**
    Gets the internal texture ID.
    @return The texture ID.
*/
TPRREuint PRRETexture::getInternalNum() const
{
    return (TPRREuint) p->getInternalNum();
    // todo: later this should be fixed: the method should be protected and GLuint can be used explicitly as return value type
} // getInternalNum()


/**
    Gets the isotropic filtering mode when zooming out (1 texel < 1 pixel).
    @return The isotropic filtering mode when zooming out.
*/
TPRRE_ISO_TEX_FILTERING PRRETexture::getMinFilteringMode() const
{
    return p->getMinFilteringMode();
} // getMinFilteringMode()


/**
    Gets the isotropic filtering mode when zooming in (1 texel > 1 pixel).
    @return The isotropic filtering mode when zooming in.
*/
TPRRE_ISO_TEX_FILTERING PRRETexture::getMagFilteringMode() const
{
    return p->getMagFilteringMode();
} // getMagFilteringMode()


/**
    Sets the isotropic filtering mode when zooming out (1 texel < 1 pixel).
    If the texture has been created without MIP maps, only PRRE_ISO_NEAREST and PRRE_ISO_LINEAR are acceptable.
*/
void PRRETexture::SetMinFilteringMode(TPRRE_ISO_TEX_FILTERING filtering)
{
    p->SetMinFilteringMode(filtering);
} // SetMinFilteringMode()


/**
    Sets the isotropic filtering mode when zooming in (1 texel > 1 pixel).
    Only the following 2 values are accepted: PRRE_ISO_NEAREST and PRRE_ISO_LINEAR.
*/
void PRRETexture::SetMagFilteringMode(TPRRE_ISO_TEX_FILTERING filtering)
{
    p->SetMagFilteringMode(filtering);
} // SetMagFilteringMode()


/**
    Sets the isotropic filtering modes.
    Equivalent to calling SetMinFilteringMode() and SetMagFilteringMode().
*/
void PRRETexture::SetIsoFilteringMode(TPRRE_ISO_TEX_FILTERING minfilter, TPRRE_ISO_TEX_FILTERING magfilter)
{
    p->SetIsoFilteringMode(minfilter, magfilter);
} // SetIsoFilteringMode()


/**
    Gets the number of MIP maps.
    @return Number of MIP maps.
*/
TPRREuint PRRETexture::getMIPmapCount() const 
{
    return p->getMIPmapCount();
} // getMIPmapCount()


/**
    Gets the anisotropic filtering mode.
    @return The anisotropic filtering mode.
*/
TPRRE_ANISO_TEX_FILTERING PRRETexture::getAnisoFilteringMode() const
{
    return p->getAnisoFilteringMode();
} // getAnisoFilteringMode()


/**
    Sets the anisotropic filtering mode.
*/
void PRRETexture::SetAnisoFilteringMode(TPRRE_ANISO_TEX_FILTERING filtering)
{
    p->SetAnisoFilteringMode(filtering);
} // SetAnisoFilteringMode()


/**
    Gets the texture wrapping mode for S coordinates.
*/
TPRRE_TEX_WRAPPING PRRETexture::getTextureWrappingModeS() const
{
    return p->getTextureWrappingModeS();
}


/**
    Gets the texture wrapping mode for S coordinates.
*/
TPRRE_TEX_WRAPPING PRRETexture::getTextureWrappingModeT() const
{
    return p->getTextureWrappingModeT();
}


/**
    Sets the texture wrapping mode.
    @param tw_s Specifies wrapping mode for S texture coordinates.
    @param tw_t Specifies wrapping mode for T texture coordinates.
*/
void PRRETexture::SetTextureWrappingMode(TPRRE_TEX_WRAPPING tw_s, TPRRE_TEX_WRAPPING tw_t )
{
    p->SetTextureWrappingMode(tw_s, tw_t);
}


/**
    Gets the border state.
*/
TPRREbool PRRETexture::getBorder() const
{
    return p->getBorder();
}


/**
    Sets the border state.
    No effect until uploadPixels() is called.
    Please note that texture borders are not supported with compressed textures.
*/
void PRRETexture::SetBorder(TPRREbool state)
{
    p->SetBorder(state);
}


/**
    Gets color of the border.
    Texture border color is by default black.
*/
const PRREColor& PRRETexture::getBorderColor() const
{
    return p->getBorderColor();
}


/**
    Sets color of the border.
    Texture border color is by default black.
    No effect until uploadPixels() is called.
    Please note that texture borders are not supported with compressed textures.
*/
void PRRETexture::SetBorderColor(const PRREColor& clr)
{
    return p->SetBorderColor(clr);
}


/**
    Uploads pixels in system memory to video memory.
    Should be used after directly modifying pixels or changing some of the properties like border.
    Has effect only if the Texture object is owned by a TextureManager and the Image part of the Texture object still has the pixels in system memory.

    Internal format and used texture memory will be updated by this method so those should be queried again in case of interest.

    The upload mechanism depends on the CURRENT settings of the owner TextureManager: for example, if the texture compression was set to off by default
    when the texture was created, the texture may still become compressed if the compression setting has been altered already when calling this method.

    This method may change the color component order of the given image temporarily but restores it after creating the texture.
    
    @return False if the texture object has no manager, or its manager is not a TextureManager, or pixels are not in system memory anymore. True otherwise.
*/
TPRREbool PRRETexture::uploadPixels()
{
    return p->uploadPixels();
} // uploadPixels()


/**
    Gets the amount of allocated system memory for the texture (VRAM not included).
    @return Amount of allocated system memory in Bytes.
*/
TPRREuint PRRETexture::getUsedSystemMemory() const
{
    return PRREImage::getUsedSystemMemory() + sizeof(*this) + p->getUsedSystemMemory();
} // getUsedSystemMemory()


/**
    Gets the amount of VRAM allocated (considering compression, MIP mapping, etc...).
    @return Amount of allocated video memory in Bytes.
*/
TPRREuint PRRETexture::getUsedTextureMemory() const
{
    return p->getUsedTextureMemory();
} // getUsedTextureMemory()


// ############################## PROTECTED ##############################


/**
    Only PRRETextureManager creates it.
*/
PRRETexture::PRRETexture()
{
    p = new PRRETextureImpl(this);
} // PRRETexture()


/**
    Only PRRETextureManager creates it this way.
*/
PRRETexture::PRRETexture(const PRREImage& img) : 
   PRREImage(img)
{
    getConsole().OLnOI("PRRETexture(img) ...");
    p = new PRRETextureImpl(this, img);
    getConsole().SOLnOO("Done!");
} // PRRETexture(...)


PRRETexture::PRRETexture(const PRRETexture&)
{

}


PRRETexture& PRRETexture::operator=(const PRRETexture&)
{
    return *this;
}


// ############################### PRIVATE ###############################


/*
   PRRETextureManager::PRRETextureManagerImpl
   ###########################################################################
*/



class PRRETextureManager::PRRETextureManagerImpl
{

public:

    // ---------------------------------------------------------------------------
           
    virtual ~PRRETextureManagerImpl();

    TPRREbool isInitialized() const; 
  
    TPRREuint getUsedTextureMemory() const;                        
    TPRREbool isHardwareMipMapGenerationEnabled() const;           
    void      SetHardwareMipMapGenerationEnabled(TPRREbool state); 
    TPRREbool isNativeDIBFormatSupportEnabled() const;             
    void      SetNativeDIBFormatSupportEnabled(TPRREbool state);   
    TPRREbool isLazyInstancingEnabled() const;                     
    void      SetLazyInstancingEnabled(TPRREbool state);           
    TPRREbool isPixelPreservingEnabled() const;                    
    void      SetPixelPreservingEnabled(TPRREbool state);                 
    
    TPRRE_ISO_TEX_FILTERING getDefaultMinFilteringMode() const;          
    TPRRE_ISO_TEX_FILTERING getDefaultMagFilteringMode() const;          
    void SetDefaultMinFilteringMode(TPRRE_ISO_TEX_FILTERING filtering);  
    void SetDefaultMagFilteringMode(TPRRE_ISO_TEX_FILTERING filtering);  
    void SetDefaultIsoFilteringMode(
        TPRRE_ISO_TEX_FILTERING minfilter,
        TPRRE_ISO_TEX_FILTERING magfilter);                              
    
    TPRRE_ANISO_TEX_FILTERING
        getDefaultAnisoFilteringMode() const;      
    void
        SetDefaultAnisoFilteringMode(
            TPRRE_ANISO_TEX_FILTERING filtering);  
    TPRRE_ANISO_TEX_FILTERING getMaximumAnisoFiltering() const;

    TPRRE_TEX_WRAPPING
        getDefaultTextureWrappingModeS() const;
    TPRRE_TEX_WRAPPING
        getDefaultTextureWrappingModeT() const;
    void
        SetDefaultTextureWrappingMode(
            TPRRE_TEX_WRAPPING tw_s,
            TPRRE_TEX_WRAPPING tw_t );
    
    TPRRE_TEX_COMPRESSION_MODE
        getDefaultCompressionMode() const;     
    void
        SetDefaultCompressionMode(
            TPRRE_TEX_COMPRESSION_MODE mode); 

    TPRREbool getDefaultBorder() const;
    void      SetDefaultBorder(TPRREbool state);

          PRREColor& getDefaultBorderColor();
    const PRREColor& getDefaultBorderColor() const; 

protected: 

    // ---------------------------------------------------------------------------

private:

    static PRREhwInfo& pHWInfo;                               /**< We need some info on HW. */

    // ---------------------------------------------------------------------------

    PRRETextureManager* _pOwner;     /**< The owner public object who creates this pimpl object. */

    TPRREbool bInited;             /**< True if successfully inited, false if not functional. */
    TPRREuint nVRAMtotal;          /**< Used by WriteList() and WriteListCallback(). */
    TPRRE_ISO_TEX_FILTERING   filtDefIsoMin,
                              filtDefIsoMag;  /**< Default isotropic filtering modes. */
    TPRRE_ANISO_TEX_FILTERING filtDefAniso;   /**< Default anisotropic filtering modes. */
    GLfloat   nAnisoFilterMax;                /**< Maximum level of anisotropic filtering. */
    TPRREbool bAllowHWMIPmapGen;              /**< Allow generating of HW MIP maps? */
    TPRREbool bAllowBGRAext;                  /**< Allow direct uploading of BGRA-format images? */
    TPRREbool bAlwaysCreateNewFromFile;       /**< Always create new instances from the same file? */
    TPRREbool bKeepPixelsInSysRAM;            /**< Keep pixels in system memory after creating texture? */
    TPRRE_TEX_COMPRESSION_MODE texComprDef;   /**< Default texture compression mode. */
    TPRREbool bDefBorder;                     /**< Default texture border state. */
    PRREColor clrDefBorder;                   /**< Default texture border color. By default it is black. */
    TPRRE_TEX_WRAPPING twDefS, twDefT;        /**< Default texture wrapping mode for S- and T- texture coordinates. */

    // ---------------------------------------------------------------------------

    PRRETextureManagerImpl(PRRETextureManager* owner);
    PRRETextureManagerImpl(const PRRETextureManagerImpl&);
    PRRETextureManagerImpl& operator=(const PRRETextureManagerImpl&);

    friend class PRRETextureManager;

};


// ############################### PUBLIC ################################


PRRETextureManager::PRRETextureManagerImpl::~PRRETextureManagerImpl()
{
    _pOwner->getConsole().OLnOI("~PRRETextureManager() ...");
    _pOwner->DeleteAll();
    _pOwner->getConsole().OO();    
    _pOwner = NULL;
}


TPRREbool PRRETextureManager::PRRETextureManagerImpl::isInitialized() const
{
    return bInited;
} // isInitialized()


TPRREuint PRRETextureManager::PRRETextureManagerImpl::getUsedTextureMemory() const
{
    TPRREuint sum = 0;
    for (TPRREint i = 0; i < _pOwner->getSize(); i++)
    {
        PRRETexture* pTex = (PRRETexture*) _pOwner->getAttachedAt(i);
        if ( pTex!= PGENULL )
            sum += pTex->getUsedTextureMemory();
    }
    return sum;
} // getUsedTextureMemory()


TPRREbool PRRETextureManager::PRRETextureManagerImpl::isHardwareMipMapGenerationEnabled() const
{
    return bAllowHWMIPmapGen;
} // isHardwareMipMapGenerationEnabled()


void PRRETextureManager::PRRETextureManagerImpl::SetHardwareMipMapGenerationEnabled(TPRREbool state)
{
    if ( !bInited )
    {
        return;
    }

    bAllowHWMIPmapGen = state && pHWInfo.getVideo().isHardwareMipMapGenerationSupported();
} // SetHardwareMipMapGenerationEnabled()


TPRREbool PRRETextureManager::PRRETextureManagerImpl::isNativeDIBFormatSupportEnabled() const
{
    return bAllowBGRAext;
} // isNativeDIBFormatSupportEnabled()


void PRRETextureManager::PRRETextureManagerImpl::SetNativeDIBFormatSupportEnabled(TPRREbool state)
{
    if ( !bInited )
    {
        return;
    }

    bAllowBGRAext = state && pHWInfo.getVideo().isNativeDIBFormatSupported();
} // SetNativeDIBFormatSupportEnabled()


TPRREbool PRRETextureManager::PRRETextureManagerImpl::isLazyInstancingEnabled() const
{
    return bAlwaysCreateNewFromFile;
} // isLazyInstancingEnabled()


void PRRETextureManager::PRRETextureManagerImpl::SetLazyInstancingEnabled(TPRREbool state)
{
    bAlwaysCreateNewFromFile = state;
} // SetLazyInstancingEnabled()


TPRREbool PRRETextureManager::PRRETextureManagerImpl::isPixelPreservingEnabled() const
{
    return bKeepPixelsInSysRAM;
} // isPixelPreservingEnabled()


void PRRETextureManager::PRRETextureManagerImpl::SetPixelPreservingEnabled(TPRREbool state)
{
    bKeepPixelsInSysRAM = state;
} // SetPixelPreservingEnabled()


TPRRE_ISO_TEX_FILTERING PRRETextureManager::PRRETextureManagerImpl::getDefaultMinFilteringMode() const
{
    return filtDefIsoMin;
} // getDefaultMinFilteringMode()


TPRRE_ISO_TEX_FILTERING PRRETextureManager::PRRETextureManagerImpl::getDefaultMagFilteringMode() const
{
    return filtDefIsoMag;
} // getDefaultMagFilteringMode()


void PRRETextureManager::PRRETextureManagerImpl::SetDefaultMinFilteringMode(TPRRE_ISO_TEX_FILTERING filtering)
{
    filtDefIsoMin = filtering;
} // SetDefaultMinFilteringMode()


void PRRETextureManager::PRRETextureManagerImpl::SetDefaultMagFilteringMode(TPRRE_ISO_TEX_FILTERING filtering)
{
    if ( (filtering == PRRE_ISO_NEAREST) || (filtering == PRRE_ISO_LINEAR) )
        filtDefIsoMag = filtering;
} // SetDefaultMagFilteringMode()


void PRRETextureManager::PRRETextureManagerImpl::SetDefaultIsoFilteringMode(TPRRE_ISO_TEX_FILTERING minfilter, TPRRE_ISO_TEX_FILTERING magfilter)
{
    SetDefaultMinFilteringMode( minfilter );
    SetDefaultMagFilteringMode( magfilter );
} // SetDefaultIsoFilteringMode()


TPRRE_ANISO_TEX_FILTERING PRRETextureManager::PRRETextureManagerImpl::getDefaultAnisoFilteringMode() const
{
    return filtDefAniso;
} // getDefaultAnisoFilteringMode()


void PRRETextureManager::PRRETextureManagerImpl::SetDefaultAnisoFilteringMode(TPRRE_ANISO_TEX_FILTERING filtering)
{
    filtDefAniso = filtering;
} // SetDefaultAnisoFilteringMode()


TPRRE_ANISO_TEX_FILTERING PRRETextureManager::PRRETextureManagerImpl::getMaximumAnisoFiltering() const
{
    return PRRETextureManager::getPRREanisoTexFilteringNameFromFloat( nAnisoFilterMax );
} // getMaximumAnisoFiltering()


TPRRE_TEX_WRAPPING PRRETextureManager::PRRETextureManagerImpl::getDefaultTextureWrappingModeS() const
{
    return twDefS;
}


TPRRE_TEX_WRAPPING PRRETextureManager::PRRETextureManagerImpl::getDefaultTextureWrappingModeT() const
{
    return twDefT;
}


void PRRETextureManager::PRRETextureManagerImpl::SetDefaultTextureWrappingMode(TPRRE_TEX_WRAPPING tw_s, TPRRE_TEX_WRAPPING tw_t )
{
    twDefS = tw_s;
    twDefT = tw_t;
}


TPRRE_TEX_COMPRESSION_MODE PRRETextureManager::PRRETextureManagerImpl::getDefaultCompressionMode() const
{
    return texComprDef;
} // getDefaultCompressionMode()


void PRRETextureManager::PRRETextureManagerImpl::SetDefaultCompressionMode(TPRRE_TEX_COMPRESSION_MODE mode)
{
    texComprDef = mode;
} // SetDefaultCompressionMode()


/**
    Gets the default border state.
*/
TPRREbool PRRETextureManager::PRRETextureManagerImpl::getDefaultBorder() const
{
    return bDefBorder;
}


/**
    Sets the default border state.
*/
void PRRETextureManager::PRRETextureManagerImpl::SetDefaultBorder(TPRREbool state)
{
    bDefBorder = state;
}


PRREColor& PRRETextureManager::PRRETextureManagerImpl::getDefaultBorderColor()
{
    return clrDefBorder;
}


const PRREColor& PRRETextureManager::PRRETextureManagerImpl::getDefaultBorderColor() const
{
    return clrDefBorder;
}


// ############################## PROTECTED ##############################


PRREhwInfo& PRRETextureManager::PRRETextureManagerImpl::pHWInfo = PRREhwInfo::get();


// ############################### PRIVATE ###############################


/**
    Sets default values.
    Requires a valid initialized PRREhwInfo instance to be functional.

    @param owner The public class owning this pimpl object.
*/
PRRETextureManager::PRRETextureManagerImpl::PRRETextureManagerImpl(PRRETextureManager* owner)
{
    _pOwner = owner;
    _pOwner->getConsole().OLnOI("PRRETextureManager() ...");
    bInited = false;
    nVRAMtotal = 0;
    filtDefIsoMin = PRRE_ISO_LINEAR;
    filtDefIsoMag = PRRE_ISO_LINEAR;
    filtDefAniso  = PRRE_ANISO_1X;
    bAlwaysCreateNewFromFile = false;
    bKeepPixelsInSysRAM = pHWInfo.isInitialized() ? ( pHWInfo.getVideo().isAcceleratorDetected() ? false : true ) : false;
    nAnisoFilterMax = pHWInfo.isInitialized() ? pHWInfo.getVideo().getMaximumAnisoFiltering() : 1.0f;
    bAllowHWMIPmapGen = pHWInfo.isInitialized() ? pHWInfo.getVideo().isHardwareMipMapGenerationSupported() : false;
    bAllowBGRAext = pHWInfo.isInitialized() ? pHWInfo.getVideo().isNativeDIBFormatSupported() : false;
    if ( pHWInfo.isInitialized() )
    {
        if ( pHWInfo.getVideo().isTextureCompressionSupported() )
        {
            glHint(GL_TEXTURE_COMPRESSION_HINT_ARB, GL_NICEST);
            PRREGLsnippets::ClearGLerror();
        }
    }
    texComprDef = PRRE_TC_NONE;
    bDefBorder = false;
    twDefS = twDefT = PRRE_TW_REPEAT;

    if ( !pHWInfo.isInitialized() )
    {
        _pOwner->getConsole().EOLnOO("No HardwareInfo!");
        return;
    }

    bInited = true;
    _pOwner->getConsole().SOLnOO("Done!");
} 


PRRETextureManager::PRRETextureManagerImpl::PRRETextureManagerImpl(const PRRETextureManager::PRRETextureManagerImpl&)
{

}


PRRETextureManager::PRRETextureManagerImpl& PRRETextureManager::PRRETextureManagerImpl::operator=(const PRRETextureManager::PRRETextureManagerImpl&)
{
    return *this;
}


/*
   PRRETextureManager
   ###########################################################################
*/


// ############################### PUBLIC ################################


/**
    Calculates the number of MIP maps from the given texture size.
    @return Number of MIP maps based on the given texture size.
*/
TPRREuint PRRETextureManager::getMIPmapCount(TPRREuint width, TPRREuint height)
{
    if ( width == 0 || height == 0 )
        return 0;
                                                   
    TPRREuint value = max(width,height);
    TPRREuint count = 1;
    while ( value > 1 )
    {
        value = value / 2;
        count++;
    }
    return count;
} // getMipmapCount()


/**
    Calculates the texture width at the given MIP map level.
    @return Texture width in pixels at the given MIP map level.
*/
TPRREuint PRRETextureManager::getWidthAtLevel(TPRREuint level, TPRREuint width, TPRREuint height)
{
    if ( width == 0 || height == 0 )
        return 0;

    TPRREuint value = max(width,height);
    TPRREuint actlevel = 0;
    while ( (value > 1) && (actlevel != level) )
    {
        value = value / 2;
        width = width / 2;
        actlevel++;
    }
    return width == 0 ? 1 : width;
} // getWidthAtLevel()


/**
    Calculates the texture height at the given MIP map level.
    @return Texture height in pixels at the given MIP map level.
*/
TPRREuint PRRETextureManager::getHeightAtLevel(TPRREuint level, TPRREuint width, TPRREuint height)
{
    if ( width == 0 || height == 0 )
        return 0;

    TPRREuint value = max(width,height);
    TPRREuint actlevel = 0;
    while ( (value > 1) && (actlevel != level) )
    {
        value = value / 2;
        height = height / 2;
        actlevel++;
    }
    return height == 0 ? 1 : height;
} // getHeightAtLevel()


/**
    Calculates the total size of the MIP mapped texture in memory based on its basesize and size.
    @return The total size of the MIP mapped texture in memory in Bytes.
*/
TPRREuint PRRETextureManager::getMIPmappedTexSize(TPRREuint basesize, TPRREuint width, TPRREuint height)
{
    if ( (basesize > 0) && (width > 0) && (height > 0) )
    {
        TPRREuint value = max(width,height);
        TPRREuint size = basesize;
        TPRREuint size2 = size;
        while ( value > 1 )
        {
            value = value / 2;
            size /= 4;
            size2 += size;
        }
        return size2;
    }
    else
        return 0;
} // getMIPmappedTexSize()


/**
    Gets the PRRE aniso texture filtering value that corresponds to the given floating point value.
    @return A PRRE aniso texture filtering value corresponding to he given value.
            PRRE_ANISO_1X if invalid value is given.
*/
TPRRE_ANISO_TEX_FILTERING PRRETextureManager::getPRREanisoTexFilteringNameFromFloat(TPRREfloat value)
{
    switch ( (int)value )
    {
    case  0:
    case  1: return  PRRE_ANISO_1X;
    case  2: return  PRRE_ANISO_2X;
    case  4: return  PRRE_ANISO_4X;
    case  8: return  PRRE_ANISO_8X;
    case 16: return PRRE_ANISO_16X;
    default: return PRRE_ANISO_1X;
    }
} // getPRREanisoTexFilteringNameFromFloat()


/**
    Gets the PRRE aniso texture filtering value that corresponds to the given integer value.
    @return A PRRE aniso texture filtering value corresponding to he given value.
            PRRE_ANISO_1X if invalid value is given.
*/
TPRRE_ANISO_TEX_FILTERING PRRETextureManager::getPRREanisoTexFilteringNameFromUInt(TPRREuint value)
{
    switch ( value )
    {
    case  0:
    case  1: return  PRRE_ANISO_1X;
    case  2: return  PRRE_ANISO_2X;
    case  4: return  PRRE_ANISO_4X;
    case  8: return  PRRE_ANISO_8X;
    case 16: return PRRE_ANISO_16X;
    default: return PRRE_ANISO_1X;
    }
} // getPRREanisoTexFilteringNameFromUInt()


/**
    Gets the value that corresponds to the given PRRE aniso texture filtering value.
    @return A floating point value corresponding to the given value.
            1.0 if invalid value is given.
*/
TPRREfloat PRRETextureManager::getFloatFromPRREanisoTexFilteringName(TPRRE_ANISO_TEX_FILTERING value)
{
    switch ( value )
    {
    case  PRRE_ANISO_1X:  return 1.0f;
    case  PRRE_ANISO_2X:  return 2.0f;
    case  PRRE_ANISO_4X:  return 4.0f;
    case  PRRE_ANISO_8X:  return 8.0f;
    case PRRE_ANISO_16X: return 16.0f;
    default: return 1.0f;
    }
} // getFloatFromPRREanisoTexFilteringName()


/**
    Gets the value that corresponds to the given PRRE aniso texture filtering value.
    @return An integer value corresponding to the given value.
            1 if invalid value is given.
*/
TPRREuint PRRETextureManager::getUIntFromPRREanisoTexFilteringName(TPRRE_ANISO_TEX_FILTERING value)
{
    switch ( value )
    {
    case  PRRE_ANISO_1X: return  1;
    case  PRRE_ANISO_2X: return  2;
    case  PRRE_ANISO_4X: return  4;
    case  PRRE_ANISO_8X: return  8;
    case PRRE_ANISO_16X: return 16;
    default: return 1;
    }
} // getUIntFromPRREanisoTexFilteringName()


/**
    Sets default values.
    Requires a valid initialized PRREhwInfo instance to be functional.
*/
PRRETextureManager::PRRETextureManager() :
    PRREImageManager()
{
    p = new PRRETextureManagerImpl(this);
} // PRRETextureManager(...)


PRRETextureManager::~PRRETextureManager()
{
    delete p;
    p = NULL;
} // ~PRRETextureManager()


/**
    Tells whether the object is correctly initialized or not.
    @return True if TextureManager instance is initialized, false otherwise.
*/
TPRREbool PRRETextureManager::isInitialized() const
{
    return p->isInitialized();
} // isInitialized()


/**
    Gets the total amount of allocated VRAM by all of the textures.
    @return Amount of allocated video memory for textures in Bytes.
*/
TPRREuint PRRETextureManager::getUsedTextureMemory() const
{
    return p->getUsedTextureMemory();
} // getUsedTextureMemory()


/**
    Gets whether HW MIP map generation is enabled or not.
    Enabled by default.

    @return True if HW MIP map generation is enabled, false otherwise.
*/
TPRREbool PRRETextureManager::isHardwareMipMapGenerationEnabled() const
{
    return p->isHardwareMipMapGenerationEnabled();
} // isHardwareMipMapGenerationEnabled()


/**
    Sets whether HW MIP map generation is enabled or not.
    Can be enabled only if the system supports it.
    Enabled by default, if supported.
*/
void PRRETextureManager::SetHardwareMipMapGenerationEnabled(TPRREbool state)
{
    p->SetHardwareMipMapGenerationEnabled(state);
} // SetHardwareMipMapGenerationEnabled()


/**
    Gets whether direct upload of BGRA-format images is enabled or not.
    Enabled by default, if supported.

    @return True if direct upload of BGRA-format images is enabled, false otherwise.
*/
TPRREbool PRRETextureManager::isNativeDIBFormatSupportEnabled() const
{
    return p->isNativeDIBFormatSupportEnabled();
} // isNativeDIBFormatSupportEnabled()


/**
    Sets whether direct upload of BGRA-format images is enabled or not.
    Can be enabled only if supported.
    Enabled by default, if supported.
*/
void PRRETextureManager::SetNativeDIBFormatSupportEnabled(TPRREbool state)
{
    p->SetNativeDIBFormatSupportEnabled(state);
} // SetNativeDIBFormatSupportEnabled()


/**
    Gets whether always creating new texture instances from the same file is enabled or not.
    Off by default.
*/
TPRREbool PRRETextureManager::isLazyInstancingEnabled() const
{
    return p->isLazyInstancingEnabled();
} // isLazyInstancingEnabled()


/**
    Sets whether always create new texture instances from the same file or not.
    If enabled, creating texture multiple times from the same image will result in creating only 1 PRRETexture and keep referring to the same later.
    Off by default.
*/
void PRRETextureManager::SetLazyInstancingEnabled(TPRREbool state)
{
    p->SetLazyInstancingEnabled(state);
} // SetLazyInstancingEnabled()


/**
    Gets whether keeping pixels of loaded textures in system memory is enabled or not.
    Off by default.
    Keeping pixels in system memory means that after successfully creating the texture in
    video memory, the Image part of the Texture should still keep the pixels in system memory.
    This is useful when you want to update pixels after creating the texture but takes more memory.
*/
TPRREbool PRRETextureManager::isPixelPreservingEnabled() const
{
    return p->isPixelPreservingEnabled();
} // isPixelPreservingEnabled()


/**
    Sets whether to keep pixels of loaded textures in system memory or delete them after texture uploading.
    Off by default.
    Keeping pixels in system memory means that after successfully creating the texture in
    video memory, the Image part of the Texture should still keep the pixels in system memory.
    This is useful when you want to update pixels after creating the texture but takes more memory.
*/
void PRRETextureManager::SetPixelPreservingEnabled(TPRREbool state)
{
    p->SetPixelPreservingEnabled(state);
} // SetPixelPreservingEnabled()


/**
    Creates texture from the given image.
    This method may change the color component order of the given image temporarily but restores it after creating the texture.
    Based on lazy instancing setting, this function may return an already existing Texture instance if such exists with same image data.
    Lazy instancing is controlled by SetLazyInstancingEnabled().

    @return The constructed texture object on success, PGENULL otherwise.
*/
PRRETexture* PRRETextureManager::createTextureFromImage(const PRREImage& img)
{
    getConsole().OLnOI("PRRETextureManager::createTextureFromImage(...)");

    if ( !isInitialized() )
    {
        getConsole().OO();
        return PGENULL;
    }

    PRRETexture* texture = isLazyInstancingEnabled() ? (PRRETexture*) getByFilename( img.getFilename().c_str() ) : PGENULL;

    if ( texture != PGENULL )
    {   // we already have a texture with this filename
        getConsole().SOLn("> Found loaded texture, returning.");
        getConsole().OLnOO("");
        return texture;
    }

    getConsole().OLn("texture == PGENULL");
    texture = new PRRETexture(img);
    Attach(*texture);

    if ( (getDefaultMinFilteringMode() == PRRE_ISO_NEAREST) || (getDefaultMinFilteringMode() == PRRE_ISO_LINEAR) )
        texture->p->nMIPmapCount = 1;
    else
        texture->p->nMIPmapCount = PRRETextureManager::getMIPmapCount( texture->getWidth(), texture->getHeight() );

    if ( p->pHWInfo.getVideo().isAcceleratorDetected() )
    {
        glGenTextures(1, &(texture->p->nInternalNum));
        glBindTexture(GL_TEXTURE_2D, texture->p->nInternalNum);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    }
    texture->SetMagFilteringMode( getDefaultMagFilteringMode() );
    texture->SetMinFilteringMode( getDefaultMinFilteringMode() );
    texture->SetAnisoFilteringMode( getDefaultAnisoFilteringMode() );
    texture->SetTextureWrappingMode( getDefaultTextureWrappingModeS(), getDefaultTextureWrappingModeT() );
    texture->SetBorder( getDefaultBorder() );
    texture->SetBorderColor( getDefaultBorderColor() );
    if ( !texture->uploadPixels() )
    {
        getConsole().EOLn("ERROR: uploadPixels() failed, will return defective texture object!");
        // TODO: probably here we should delete the texture object and return NULL.
    }

    if ( !isPixelPreservingEnabled() )
        texture->FlushResources();

    getConsole().OLn("Texture nWidth and nHeight: %d %d", texture->getWidth(), texture->getHeight());
    getConsole().OLn("final nSize is %d Byte(s).", texture->p->nSize);
    getConsole().OLn("internalnum = %d", texture->p->nInternalNum);
    getConsole().OLn("");
    getConsole().SOLn("> Texture created!");
    getConsole().OOOLn("");
    return texture;
} // createTextureFromImage()


/**
    Creates texture from the given file.   
    Only the file formats accepted in ImageManager can be used here.
    Based on lazy instancing setting, this function may return an already existing Texture instance if such exists with same image data.
    Lazy instancing is controlled by SetLazyInstancingEnabled().

    @return The constructed texture object on success, PGENULL otherwise.
*/
PRRETexture* PRRETextureManager::createFromFile(const char* filename)
{
    if ( !isInitialized() )
    {
        return PGENULL;
    }

    getConsole().OLnOI("PRRETextureManager::createFromFile(\"%s\")", filename);

    if ( filename == PGENULL )
    {
        getConsole().EOLnOO("ERROR: input was NULL, returning PGENULL");
        getConsole().OLn("");
        return PGENULL;
    }

    PRRETexture* texture = isLazyInstancingEnabled() ? (PRRETexture*) getByFilename( filename ) : PGENULL;

    if ( texture != PGENULL )
    {   // we already have a texture with this filename
        getConsole().SOLnOO("> Found loaded texture, returning.");
        getConsole().OLn("");
        return texture;
    }

    string sFileExt = PFL::getExtension(filename);
    if ( sFileExt == "" )
    {                                              
        getConsole().EOLnOO("ERROR: no file extension!");
        getConsole().OLn("");
        return PGENULL;    
    }
    if ( !PFL::fileExists(filename) )
    {
        getConsole().EOLnOO("ERROR: file doesn't exist!");
        getConsole().OLn("");
        return PGENULL;
    }

    transform(sFileExt.begin(), sFileExt.end(), sFileExt.begin(), ::toupper);
    getConsole().OLn("ext: .%s", sFileExt.c_str());
    const PRREImage* img = PGENULL;
    if ( sFileExt == "BMP" )
    {
        getConsole().OI();
        img = loadBMP(filename);
        getConsole().OO();
    }    
    else
    {
        getConsole().EOLnOO("ERROR: unsupported extension!");
        getConsole().OLn("");
        return PGENULL;
    }
    getConsole().OI();
    texture = createTextureFromImage(*img);
    getConsole().OO();
    getConsole().SOLnOO("> Texture loaded successfully!");
    getConsole().OLn("");
    return texture;
} // createFromFile()


/**
    Gets the default isotropic filtering mode when zooming out (when 1 texel < 1 pixel).
    The default mode is the mode that will be set at every texture creation.
    The default isotropic filtering mode when zooming out is PRRE_ISO_LINEAR.

    @return Default isotropic filtering when zooming out.
*/
TPRRE_ISO_TEX_FILTERING PRRETextureManager::getDefaultMinFilteringMode() const
{
    return p->getDefaultMinFilteringMode();
} // getDefaultMinFilteringMode()


/**
    Gets the default isotropic filtering mode when zooming in (when 1 texel > 1 pixel).
    The default mode is the mode that will be set at every texture creation.
    The default isotropic filtering mode when zooming in is PRRE_ISO_LINEAR.

    @return Default isotropic filtering when zooming in.
*/
TPRRE_ISO_TEX_FILTERING PRRETextureManager::getDefaultMagFilteringMode() const
{
    return p->getDefaultMagFilteringMode();
} // getDefaultMagFilteringMode()


/**
    Sets the default isotropic filtering mode when zooming out (when 1 texel < 1 pixel).
    The default mode is the mode that will be set at every texture creation.
    The default isotropic filtering mode when zooming out is PRRE_ISO_LINEAR.
*/
void PRRETextureManager::SetDefaultMinFilteringMode(TPRRE_ISO_TEX_FILTERING filtering)
{
    p->SetDefaultMinFilteringMode(filtering);
} // SetDefaultMinFilteringMode()


/**
    Sets the default isotropic filtering mode when zooming in (when 1 texel > 1 pixel).
    The default mode is the mode that will be set at every texture creation.
    The default isotropic filtering mode when zooming in is PRRE_ISO_LINEAR.
    Only the following 2 values are accepted: PRRE_ISO_NEAREST and PRRE_ISO_LINEAR.
*/
void PRRETextureManager::SetDefaultMagFilteringMode(TPRRE_ISO_TEX_FILTERING filtering)
{
    p->SetDefaultMagFilteringMode(filtering);
} // SetDefaultMagFilteringMode()


/**
    Sets the default isotropic filtering modes.
    Equivalent to calling SetDefaultMinFilteringMode() and SetDefaultMagFilteringMode().
*/ 
void PRRETextureManager::SetDefaultIsoFilteringMode(TPRRE_ISO_TEX_FILTERING minfilter, TPRRE_ISO_TEX_FILTERING magfilter)
{
    p->SetDefaultIsoFilteringMode(minfilter, magfilter);
} // SetDefaultIsoFilteringMode()


/**
    Gets the default anisotropic filtering level.
    The default mode is the mode that will be set at every texture creation.
    The default anisotropic filtering mode is PRRE_ANISO_1X.

    @return Default anisotropic filtering.
*/
TPRRE_ANISO_TEX_FILTERING PRRETextureManager::getDefaultAnisoFilteringMode() const
{
    return p->getDefaultAnisoFilteringMode();
} // getDefaultAnisoFilteringMode()


/**
    Sets the default anisotropic filtering level.
    The default mode is the mode that will be set at every texture creation.
    The default anisotropic filtering mode is PRRE_ANISO_1X, meaning it is off.
    Not every system supports anisotropic filtering.
*/
void PRRETextureManager::SetDefaultAnisoFilteringMode(TPRRE_ANISO_TEX_FILTERING filtering)
{
    p->SetDefaultAnisoFilteringMode(filtering);
} // SetDefaultAnisoFilteringMode()


/**
    Gets the maximum level of anisotropic filtering.
    Not every system supports anisotropic filtering.

    @return Maximum level of anisotropic filtering, PRRE_ANISO_1X if not supported.
*/
TPRRE_ANISO_TEX_FILTERING PRRETextureManager::getMaximumAnisoFiltering() const
{
    return p->getMaximumAnisoFiltering();
} // getMaximumAnisoFiltering()


/**
    Gets the default texture wrapping mode for S coordinates.
    The default mode is the mode that will be set at every texture creation.
    The default texture wrapping mode is PRRE_TW_REPEAT.
*/
TPRRE_TEX_WRAPPING PRRETextureManager::getDefaultTextureWrappingModeS() const
{
    return p->getDefaultTextureWrappingModeS();
}


/**
    Gets the default texture wrapping mode for T coordinates.
    The default mode is the mode that will be set at every texture creation.
    The default texture wrapping mode is PRRE_TW_REPEAT.
*/
TPRRE_TEX_WRAPPING PRRETextureManager::getDefaultTextureWrappingModeT() const
{
    return p->getDefaultTextureWrappingModeT();
}


/**
    Sets the default texture wrapping mode.
    The default mode is the mode that will be set at every texture creation.
    The default texture wrapping mode is PRRE_TW_REPEAT for S- and T- texture coordinates.
*/
void PRRETextureManager::SetDefaultTextureWrappingMode(TPRRE_TEX_WRAPPING tw_s, TPRRE_TEX_WRAPPING tw_t )
{
    p->SetDefaultTextureWrappingMode(tw_s, tw_t);
}


/**
    Gets the default texture compression mode.
    The default mode is the mode that will be set at every texture creation.
    The default texture compression mode is PRRE_TC_NONE.
    Not every system supports texture compression.

    @return Default texture compression mode. Default value is PRRE_TC_NONE.
*/
TPRRE_TEX_COMPRESSION_MODE PRRETextureManager::getDefaultCompressionMode() const
{
    return p->getDefaultCompressionMode();
} // getDefaultCompressionMode()


/**
    Sets the default texture compression mode.
    The default mode is the mode that will be used at every texture creation.
    The default texture compression mode is PRRE_TC_NONE.
    Not every system supports texture compression.

    You can select RGBA compression modes even if source image is in RGB. In that case, alpha values
    will be automatically set to 1.
    You can also select RGB compression mode even if source image is in RGBA. In that case, alpha values
    will be ignored.
    Please note that texture borders are not supported with compressed textures.
*/
void PRRETextureManager::SetDefaultCompressionMode(TPRRE_TEX_COMPRESSION_MODE mode)
{
    p->SetDefaultCompressionMode(mode);
} // SetDefaultCompressionMode()


/**
    Gets the default border state.
    The default mode is the mode that will be set at every texture creation.
    Please note that texture borders are not supported with compressed textures.
*/
TPRREbool PRRETextureManager::getDefaultBorder() const
{
    return p->getDefaultBorder();
}


/**
    Sets the default border state.
    The default mode is the mode that will be set at every texture creation.
    Please note that texture borders are not supported with compressed textures.
*/
void PRRETextureManager::SetDefaultBorder(TPRREbool state)
{
    p->SetDefaultBorder(state);
}


/**
    Gets default color of the border.
    The default mode is the mode that will be set at every texture creation.
    Texture default border color is by default black.
    Please note that texture borders are not supported with compressed textures.
*/
PRREColor& PRRETextureManager::getDefaultBorderColor()
{
    return p->getDefaultBorderColor();
}


/**
    Gets default color of the border.
    The default mode is the mode that will be set at every texture creation.
    Texture default border color is by default black.
    Please note that texture borders are not supported with compressed textures.
*/
const PRREColor& PRRETextureManager::getDefaultBorderColor() const
{
    return p->getDefaultBorderColor();
}


/**
    From PRREImageManager, adding total texture memory usage counting.
*/
void PRRETextureManager::WriteList() const
{
    PRREImageManager::WriteList();

    TPRREuint nVRAMtotal = 0;
    for (TPRREint i = 0; i < getSize(); i++)
    {
        const PRRETexture* const pMngd = (PRRETexture*) getAttachedAt(i);
        if ( pMngd != PGENULL )
            nVRAMtotal += pMngd->getUsedTextureMemory();
    }

    getConsole().OIOLnOO("> total used texture memory = %d Bytes = %d kBytes = %f MBytes", nVRAMtotal, nVRAMtotal/1024, nVRAMtotal/1024.0f/1024.0f);
    getConsole().OLn("");
} // WriteList()


// ############################## PROTECTED ##############################


PRRETextureManager::PRRETextureManager(const PRRETextureManager&)
{

}


PRRETextureManager& PRRETextureManager::operator=(const PRRETextureManager&)
{
    return *this;
}


/**
    From PRREImageManager, texture memory and internal number.
*/
void PRRETextureManager::WriteListCallback(const PRREManaged& mngd) const
{
    PRREImageManager::WriteListCallback(mngd);
    PRRETexture& tex = (PRRETexture&) mngd;
    getConsole().OIOLnOO("texture memory: %d kB; internal num.: %d", tex.getUsedTextureMemory()/1024, tex.p->nInternalNum);
} // WriteListCallback()


// ############################### PRIVATE ###############################



