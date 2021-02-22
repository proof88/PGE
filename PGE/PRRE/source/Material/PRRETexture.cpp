/*
    ###################################################################################
    PRRETexture.cpp
    This file is part of PRRE.
    PRRETexture class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREbaseIncludes.h"  // PCH
#include "../../include/external/Material/PRRETextureManager.h"
#include "../../include/internal/Material/PRRETextureImpl.h"
#include "../../include/internal/PRREGLsafeFuncs.h"
#include "../../include/internal/PRREGLsnippets.h"
#include "../../include/internal/PRREpragmas.h"
#include "../../include/external/Hardware/PRREhwInfo.h"

using namespace std;

// TODO: following few DD structs may be needed IN THE FUTURE when we add DDS format support, so keeping them here ...
// DOC: for now I'm not documenting them.

/*
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


/*
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


/*
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
    {
        glDeleteTextures(1, &nInternalNum);
    }
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


TPRREbool PRRETexture::PRRETextureImpl::setMinFilteringMode(TPRRE_ISO_TEX_FILTERING filtering)
{
    if ( (filtering != PRRE_ISO_NEAREST) && (filtering != PRRE_ISO_LINEAR) && (nMIPmapCount == 1) )
        return false;

    if ( PRREhwInfo::get().getVideo().isAcceleratorDetected() )
    {
        if ( ( nInternalNum == 0 ) ||
             ( !pglBindTexture(GL_TEXTURE_2D, nInternalNum) ) ||
             ( !pglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, getGLnameFromPRREisoTexFilteringName(filtering)) ) )
        {
            return false;
        }
    }
    filtIsoMin = filtering;
    return true;
} // setMinFilteringMode()


TPRREbool PRRETexture::PRRETextureImpl::setMagFilteringMode(TPRRE_ISO_TEX_FILTERING filtering)
{
    if ( (filtering == PRRE_ISO_NEAREST) || (filtering == PRRE_ISO_LINEAR) )
    {
        
        if ( PRREhwInfo::get().getVideo().isAcceleratorDetected() )
        {
            if ( ( nInternalNum == 0 ) ||
                 ( !pglBindTexture(GL_TEXTURE_2D, nInternalNum) ) ||
                 ( !pglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, getGLnameFromPRREisoTexFilteringName(filtering)) ) )
            {
                return false;
            }
        }
        filtIsoMag = filtering;
        return true;
    }
    return false;
} // setMagFilteringMode()


TPRREbool PRRETexture::PRRETextureImpl::setIsoFilteringMode(TPRRE_ISO_TEX_FILTERING minfilter, TPRRE_ISO_TEX_FILTERING magfilter)
{
    return setMinFilteringMode( minfilter ) && setMagFilteringMode( magfilter );
} // setIsoFilteringMode()


TPRREuint PRRETexture::PRRETextureImpl::getMIPmapCount() const 
{
    return nMIPmapCount;
} // getMIPmapCount()


TPRRE_ANISO_TEX_FILTERING PRRETexture::PRRETextureImpl::getAnisoFilteringMode() const
{
    return filtAniso;
} // getAnisoFilteringMode()


TPRREbool PRRETexture::PRRETextureImpl::setAnisoFilteringMode(TPRRE_ANISO_TEX_FILTERING filtering)
{
    _pOwner->getConsole().OIOLn("PRRETexture::setAnisoFilteringMode(%fX)", PRRETextureManager::getFloatFromPRREanisoTexFilteringName(filtering));
    
    if ( _pOwner->getManager() == PGENULL )
        return false;
    
    if ( !PRREhwInfo::get().getVideo().isAcceleratorDetected() )
    {
        _pOwner->getConsole().EOLnOO("WARNING: anisotropic filtering is not supported without acceleration!");
        return false;
    }

    bool result = false;
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
        if ( (nInternalNum == 0) || (!pglBindTexture(GL_TEXTURE_2D, nInternalNum)) )
        {
            _pOwner->getConsole().EOLnOO("ERROR: nInternalNum is 0 or pglBindTexture() failed!");
            return false;
        }
        if ( availMaxFilteringF >= newFilteringF )
        {
            if ( pglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, (GLfloat) newFilteringF) )
            {
                filtAniso = filtering;
                result = true;
            }
        }
        else
        {
            _pOwner->getConsole().EOLn("WARNING: selected mode not supported, setting max supported: %fX!", availMaxFilteringF);
            if ( pglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, (GLfloat) availMaxFilteringF) )
            {
                filtAniso = PRRETextureManager::getPRREanisoTexFilteringNameFromFloat(availMaxFilteringF);
                result = true;
            }
        }
    }
    else
    {
        _pOwner->getConsole().EOLn("ERROR: anisotropic filtering is not supported");
    }
    _pOwner->getConsole().OO();
    return result;
} // setAnisoFilteringMode()


TPRRE_TEX_WRAPPING PRRETexture::PRRETextureImpl::getTextureWrappingModeS() const
{
    return twS;
}


TPRRE_TEX_WRAPPING PRRETexture::PRRETextureImpl::getTextureWrappingModeT() const
{
    return twT;
}


TPRREbool PRRETexture::PRRETextureImpl::setTextureWrappingMode(TPRRE_TEX_WRAPPING tw_s, TPRRE_TEX_WRAPPING tw_t )
{
    // TODO: probably we should check for availability of given mode on current HW, especially with PRRE_TW_MIRRORED_REPEAT and PRRE_TW_CLAMP_TO_BORDER  
    if ( PRREhwInfo::get().getVideo().isAcceleratorDetected() )
    {
        if ( (nInternalNum == 0) ||
             (!pglBindTexture(GL_TEXTURE_2D, nInternalNum)) ||
             (!pglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, getGLnameFromPRREtexWrappingName(tw_s)) ) ||
             (!pglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, getGLnameFromPRREtexWrappingName(tw_t)) ) )
        {
            return false;
        }
    }
    twS = tw_s;
    twT = tw_t;
    return true;
}


TPRREbool PRRETexture::PRRETextureImpl::getBorder() const
{
    return bBorder;
}


TPRREbool PRRETexture::PRRETextureImpl::setBorder(TPRREbool state)
{
    bBorder = state;
    return true;
}


const PRREColor& PRRETexture::PRRETextureImpl::getBorderColor() const
{
    return clrBorder;
}


TPRREbool PRRETexture::PRRETextureImpl::setBorderColor(const PRREColor& clr)
{
    if ( PRREhwInfo::get().getVideo().isAcceleratorDetected() )
    {
        if ( nInternalNum == 0 )
        {
            return false;
        }
        GLfloat color[4];
        color[0] = clr.getRedAsFloat();
        color[1] = clr.getGreenAsFloat();
        color[2] = clr.getBlueAsFloat();
        color[3] = clr.getAlphaAsFloat();
        
        if ( (!pglBindTexture(GL_TEXTURE_2D, nInternalNum)) ||
             (!pglTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color)) )
        {
            return false;
        }
    }
    clrBorder = clr;
    return true;
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
        if (!pglBindTexture(GL_TEXTURE_2D, nInternalNum))
        {
            return false;
        }
    }

    const TPRRE_PIXEL_COMPONENT_ORDER oldco = _pOwner->getPixelComponentOrder();
    const GLenum glTexFormat = getTransformedSourceFormat(oldco); // might invoke setPixelComponentOrder()!
    if ( glTexFormat == GL_INVALID_ENUM )
    {
        _pOwner->getConsole().EOLnOO("ERROR: getTransformedSourceFormat() failed!");
        // try reset original pixel component order
        if ( !_pOwner->setPixelComponentOrder( oldco ) )
        {
            _pOwner->getConsole().EOLnOO("ERROR: recovery setPixelComponentOrder(oldco) failed!");
        }
        return false;
    }

    const GLint internalfmt = getTargetInternalFormat();

    if ( !actualUploadProc(internalfmt, glTexFormat) )
    {
        _pOwner->getConsole().EOLnOO("ERROR: actualUploadProc() failed!");
        return false;
    }

    DescribeTexFormatAndSize(internalfmt);

    if ( _pOwner->getPixelComponentOrder() != oldco )
    {
        if ( !_pOwner->setPixelComponentOrder( oldco ) )
        {
            _pOwner->getConsole().EOLnOO("ERROR: finished creating new texture, but setPixelComponentOrder(oldco) failed!");
            // intentionally let function still return true
        }
    }

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

    @return OpenGL source format enum. GL_INVALID_ENUM in case of error.
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
            if ( !_pOwner->setPixelComponentOrder(PRRE_RGB) )
                return GL_INVALID_ENUM;
            return GL_RGB;
        }
    }
    else if ( oldco == PRRE_BGRA )
    {
        if ( hwInfo.getVideo().isNativeDIBFormatSupported() && parent.isNativeDIBFormatSupportEnabled() )
            return GL_BGRA_EXT;
        else
        {
            if ( !_pOwner->setPixelComponentOrder(PRRE_RGBA) )
                return GL_INVALID_ENUM;
            return GL_RGBA;
        }
    }
    else if ( _pOwner->getPixelComponentOrder() % 2 == 0 )
    {
        if ( oldco != PRRE_RGB )
        {
            if ( !_pOwner->setPixelComponentOrder(PRRE_RGB) )
                return GL_INVALID_ENUM;
        }
        return GL_RGB;
    }
    else
    {
        if ( oldco != PRRE_RGBA )
        {
            if ( !_pOwner->setPixelComponentOrder(PRRE_RGBA) )
                return GL_INVALID_ENUM;
        }
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

    @return True on success, false on error.
*/
TPRREbool PRRETexture::PRRETextureImpl::actualUploadProc(GLint internalfmt, GLenum glTexFormat)
{
    const PRRETextureManager& parent = *((PRRETextureManager*) _pOwner->getManager());
    const PRREhwInfo& hwInfo = PRREhwInfo::get();

    if ( !hwInfo.getVideo().isAcceleratorDetected() )
    {
        getConsole().OLn("no need to upload texture, non-accelerated rendering is active");
        return true;
    }

    if ( nMIPmapCount > 1 )
    {
        getConsole().OLn("mipmapping on");
        getConsole().O("auto-generating mipmaps with ");
        if ( hwInfo.getVideo().isHardwareMipMapGenerationSupported() && parent.isHardwareMipMapGenerationEnabled() )
        {        
            getConsole().OLn("HW-support.");
            if (!pglTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE) )
            {
                return false;
            }
            
            // WARNING: up to OpenGL 2.x, border is allowed to be 0 or 1. However from GL 3.0 it must be 0.
            // WARNING: if GL version does not support NPOT sizes, width must be 2n+2(border) for some integer n.
            // WARNING: if GL version does not support NPOT sizes, height must be 2m+2(border) for some integer m.
            // TODO: So in the future when I add support for NPOT textures, I need to consider border value when specifying width and height.
            return pglTexImage2D(GL_TEXTURE_2D, 0, internalfmt, _pOwner->getWidth(), _pOwner->getHeight(), bBorder ? (GLint) 1 : (GLint) 0, glTexFormat, GL_UNSIGNED_BYTE, _pOwner->getPixels());
        }
        else
        {
            getConsole().OLn("SW-only.");
            const GLint errCode = gluBuild2DMipmaps(GL_TEXTURE_2D, internalfmt, _pOwner->getWidth(), _pOwner->getHeight(), glTexFormat, GL_UNSIGNED_BYTE, _pOwner->getPixels());
            if ( errCode != GL_NO_ERROR )
            {
                getConsole().EOLn("PRRETextureImpl::actualUploadProc() ERROR: %d, %s", errCode, gluErrorString(errCode));
                return false;
            }
            return true;
        }
    }
    else
    {
        getConsole().OLn("mipmapping off");

        // WARNING: up to OpenGL 2.x, border is allowed to be 0 or 1. However from GL 3.0 it must be 0.
        // WARNING: if GL version does not support NPOT sizes, width must be 2n+2(border) for some integer n.
        // WARNING: if GL version does not support NPOT sizes, height must be 2m+2(border) for some integer m.
        // TODO: So in the future when I add support for NPOT textures, I need to consider border value when specifying width and height.
        return pglTexImage2D(GL_TEXTURE_2D, 0, internalfmt, _pOwner->getWidth(), _pOwner->getHeight(), bBorder ? (GLint) 1 : (GLint) 0, glTexFormat, GL_UNSIGNED_BYTE, _pOwner->getPixels());
    }
} // actualUploadProc()


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
    delete pImpl;
    pImpl = NULL;
} // ~PRRETexture()


/**
    Gets the internal texture format.    
    @return The internal texture format.
*/
TPRRE_TEX_FORMAT PRRETexture::getInternalFormat() const
{
    return pImpl->getInternalFormat();
} // getInternalFormat()


/**
    Gets the internal texture ID.
    @return The texture ID.
*/
TPRREuint PRRETexture::getInternalNum() const
{
    return (TPRREuint) pImpl->getInternalNum();
    // todo: later this should be fixed: the method should be protected and GLuint can be used explicitly as return value type
} // getInternalNum()


/**
    Gets the isotropic filtering mode when zooming out (1 texel < 1 pixel).
    @return The isotropic filtering mode when zooming out.
*/
TPRRE_ISO_TEX_FILTERING PRRETexture::getMinFilteringMode() const
{
    return pImpl->getMinFilteringMode();
} // getMinFilteringMode()


/**
    Gets the isotropic filtering mode when zooming in (1 texel > 1 pixel).
    @return The isotropic filtering mode when zooming in.
*/
TPRRE_ISO_TEX_FILTERING PRRETexture::getMagFilteringMode() const
{
    return pImpl->getMagFilteringMode();
} // getMagFilteringMode()


/**
    Sets the isotropic filtering mode when zooming out (1 texel < 1 pixel).
    If the texture has been created without MIP maps, only PRRE_ISO_NEAREST and PRRE_ISO_LINEAR are acceptable.
    @return True on success, false otherwise.
*/
TPRREbool PRRETexture::setMinFilteringMode(TPRRE_ISO_TEX_FILTERING filtering)
{
    return pImpl->setMinFilteringMode(filtering);
} // setMinFilteringMode()


/**
    Sets the isotropic filtering mode when zooming in (1 texel > 1 pixel).
    Only the following 2 values are accepted: PRRE_ISO_NEAREST and PRRE_ISO_LINEAR.
    @return True on success, false otherwise.
*/
TPRREbool PRRETexture::setMagFilteringMode(TPRRE_ISO_TEX_FILTERING filtering)
{
    return pImpl->setMagFilteringMode(filtering);
} // setMagFilteringMode()


/**
    Sets the isotropic filtering modes.
    Equivalent to calling setMinFilteringMode(minfilter) && setMagFilteringMode(magfilter).
    @return True on success, false otherwise.
*/
TPRREbool PRRETexture::setIsoFilteringMode(TPRRE_ISO_TEX_FILTERING minfilter, TPRRE_ISO_TEX_FILTERING magfilter)
{
    return pImpl->setIsoFilteringMode(minfilter, magfilter);
} // setIsoFilteringMode()


/**
    Gets the number of MIP maps.
    @return Number of MIP maps.
*/
TPRREuint PRRETexture::getMIPmapCount() const 
{
    return pImpl->getMIPmapCount();
} // getMIPmapCount()


/**
    Gets the anisotropic filtering mode.
    @return The anisotropic filtering mode.
*/
TPRRE_ANISO_TEX_FILTERING PRRETexture::getAnisoFilteringMode() const
{
    return pImpl->getAnisoFilteringMode();
} // getAnisoFilteringMode()


/**
    Sets the anisotropic filtering mode.
    Note that if anisotropic filtering is supported but on a lower filtering mode, that will be set and considered as success.
    Obviously if anisotropic filtering is not supported, this function will fail.
    @return True on success, false otherwise.
*/
TPRREbool PRRETexture::setAnisoFilteringMode(TPRRE_ANISO_TEX_FILTERING filtering)
{
    return pImpl->setAnisoFilteringMode(filtering);
} // setAnisoFilteringMode()


/**
    Gets the texture wrapping mode for S coordinates.
    @return Texture wrapping mode for S coordinates.
*/
TPRRE_TEX_WRAPPING PRRETexture::getTextureWrappingModeS() const
{
    return pImpl->getTextureWrappingModeS();
}


/**
    Gets the texture wrapping mode for T coordinates.
    @return Texture wrapping mode for T coordinates.
*/
TPRRE_TEX_WRAPPING PRRETexture::getTextureWrappingModeT() const
{
    return pImpl->getTextureWrappingModeT();
}


/**
    Sets the texture wrapping mode.
    @param tw_s Specifies wrapping mode for S texture coordinates.
    @param tw_t Specifies wrapping mode for T texture coordinates.
    @return True on success, false otherwise.
*/
TPRREbool PRRETexture::setTextureWrappingMode(TPRRE_TEX_WRAPPING tw_s, TPRRE_TEX_WRAPPING tw_t )
{
    return pImpl->setTextureWrappingMode(tw_s, tw_t);
}


/**
    Gets the border state.
    @return Texture border state: true if has border, false otherwise.
*/
TPRREbool PRRETexture::getBorder() const
{
    return pImpl->getBorder();
}


/**
    Sets the border state.
    No effect until uploadPixels() is called.
    Please note that texture borders are not supported with compressed textures.
    @return True on success, false otherwise.
*/
TPRREbool PRRETexture::setBorder(TPRREbool state)
{
    return pImpl->setBorder(state);
}


/**
    Gets color of the border.
    Texture border color is by default black.
    @return Texture border color.
*/
const PRREColor& PRRETexture::getBorderColor() const
{
    return pImpl->getBorderColor();
}


/**
    Sets color of the border.
    Texture border color is by default black.
    No effect until uploadPixels() is called.
    Please note that texture borders are not supported with compressed textures.
    @return True on success, false otherwise.
*/
TPRREbool PRRETexture::setBorderColor(const PRREColor& clr)
{
    return pImpl->setBorderColor(clr);
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
    return pImpl->uploadPixels();
} // uploadPixels()


/**
    Gets the amount of allocated system memory for the texture (VRAM not included).
    VRAM usage can be retrieved by getUsedTextureMemory().
    @return Amount of allocated system memory in Bytes.
*/
TPRREuint PRRETexture::getUsedSystemMemory() const
{
    return PRREImage::getUsedSystemMemory() + sizeof(*this) + pImpl->getUsedSystemMemory();
} // getUsedSystemMemory()


/**
    Gets the amount of VRAM allocated (considering compression, MIP mapping, etc...).
    This does not include used system memory by the object itself, including possible cached pixel data.
    Used system memory can be retrieved by getUsedSystemMemory().
    @return Amount of allocated video memory in Bytes.
*/
TPRREuint PRRETexture::getUsedTextureMemory() const
{
    return pImpl->getUsedTextureMemory();
} // getUsedTextureMemory()


// ############################## PROTECTED ##############################


/**
    Only PRRETextureManager creates it.

    @exception std::bad_alloc - This class or its ancestor dynamically allocates memory with operator new, in case of failure the exception is not handled but propagated to caller.
*/
PRRETexture::PRRETexture()
{
    getConsole().OLnOI("PRRETexture() ...");
    pImpl = new PRRETextureImpl(this);
    getConsole().SOLnOO("Done!");
} // PRRETexture()


/**
    Only PRRETextureManager creates it this way.

    @exception std::bad_alloc - This class or its ancestor dynamically allocates memory with operator new, in case of failure the exception is not handled but propagated to caller.
*/
PRRETexture::PRRETexture(const PRREImage& img) : 
   PRREImage(img)
{
    getConsole().OLnOI("PRRETexture(img) ...");
    pImpl = new PRRETextureImpl(this, img);
    getConsole().SOLnOO("Done!");
} // PRRETexture(...)


PRRETexture::PRRETexture(const PRRETexture&)
{
    // UNUSED
}


PRRETexture& PRRETexture::operator=(const PRRETexture&)
{
    // UNUSED
    return *this;
}


// ############################### PRIVATE ###############################

