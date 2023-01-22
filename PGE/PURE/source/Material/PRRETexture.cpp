/*
    ###################################################################################
    PureTexture.cpp
    This file is part of Pure.
    PureTexture class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PurebaseIncludes.h"  // PCH
#include "../../include/external/Material/PureTextureManager.h"
#include "../../include/internal/Material/PureTextureImpl.h"
#include "../../include/internal/PureGLsafeFuncs.h"
#include "../../include/internal/PureGLsnippets.h"
#include "../../include/internal/Purepragmas.h"
#include "../../include/external/Hardware/PureHwInfo.h"

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
   PureTexture::PureTextureImpl
   ###########################################################################
*/

// ############################### PUBLIC ################################


/**
    Gets the Pure iso texture filtering value that corresponds to the given GL enum value.
    @return A GL texture filtering enum value corresponding to the given value.
            Pure_ISO_LINEAR if invalid value is given.
*/
TPure_ISO_TEX_FILTERING PureTexture::PureTextureImpl::getPureisoTexFilteringNameFromGLname(GLenum value)
{
    switch ( value )
    {
    case GL_NEAREST: return Pure_ISO_NEAREST;
    case GL_LINEAR:  return Pure_ISO_LINEAR;
    case GL_NEAREST_MIPMAP_NEAREST: return Pure_ISO_NEAREST_MIPMAP_NEAREST;
    case GL_NEAREST_MIPMAP_LINEAR:  return Pure_ISO_NEAREST_MIPMAP_LINEAR;
    case GL_LINEAR_MIPMAP_NEAREST:  return Pure_ISO_LINEAR_MIPMAP_NEAREST;
    case GL_LINEAR_MIPMAP_LINEAR:   return Pure_ISO_LINEAR_MIPMAP_LINEAR;
    default: return Pure_ISO_LINEAR;
    }    
} // getPureisoTexFilteringNameFromGLname()


/**
    Gets the GL enum value that corresponds to the given Pure iso texture filtering value.
    @return A GL texture filtering enum value corresponding to the given value.
            GL_LINEAR if invalid value is given.
*/
GLenum PureTexture::PureTextureImpl::getGLnameFromPureisoTexFilteringName(TPure_ISO_TEX_FILTERING value)
{
    switch ( value )
    {
    case Pure_ISO_NEAREST: return GL_NEAREST;
    case Pure_ISO_LINEAR:  return GL_LINEAR;
    case Pure_ISO_NEAREST_MIPMAP_NEAREST: return GL_NEAREST_MIPMAP_NEAREST;
    case Pure_ISO_NEAREST_MIPMAP_LINEAR:  return GL_NEAREST_MIPMAP_LINEAR;
    case Pure_ISO_LINEAR_MIPMAP_NEAREST:  return GL_LINEAR_MIPMAP_NEAREST;
    case Pure_ISO_LINEAR_MIPMAP_LINEAR:   return GL_LINEAR_MIPMAP_LINEAR;
    default: return GL_LINEAR;
    }
} // getGLnameFromPureisoTexFilteringName()


/**
    Gets the Pure texture wrapping mode value that corresponds to the given GL enum value.
    @return A Pure texture wrapping mode value corresponding to the given GL value.
            Pure_TW_REPEAT if invalid value is given.
*/
TPure_TEX_WRAPPING PureTexture::PureTextureImpl::getPuretexWrappingNameFromGLname(GLenum value)
{
    switch (value)
    {
    case GL_CLAMP           : return Pure_TW_CLAMP;
    case GL_REPEAT          : return  Pure_TW_REPEAT;
    case GL_MIRRORED_REPEAT : return Pure_TW_MIRRORED_REPEAT;
    case GL_CLAMP_TO_BORDER : return Pure_TW_CLAMP_TO_BORDER;
    case GL_CLAMP_TO_EDGE   : return Pure_TW_CLAMP_TO_EDGE;
    default: return Pure_TW_REPEAT;
    }    
}


/**
    Gets the GL enum value that corresponds to the given Pure texture wrapping mode value.
    @return A GL texture wrapping enum value corresponding to the given value.
            GL_REPEAT if invalid value is given.
*/
GLenum  PureTexture::PureTextureImpl::getGLnameFromPuretexWrappingName(TPure_TEX_WRAPPING value)
{
    switch (value)
    {
    case Pure_TW_CLAMP           : return GL_CLAMP;
    case Pure_TW_REPEAT          : return GL_REPEAT;
    case Pure_TW_MIRRORED_REPEAT : return GL_MIRRORED_REPEAT;
    case Pure_TW_CLAMP_TO_BORDER : return GL_CLAMP_TO_BORDER;
    case Pure_TW_CLAMP_TO_EDGE   : return GL_CLAMP_TO_EDGE;
    default: return GL_REPEAT;
    }
}


PureTexture::PureTextureImpl::~PureTextureImpl()
{
    _pOwner->getManagedConsole().OLnOI("~PureTexture() ...");
    if ( nInternalNum > 0 )
    {
        glDeleteTextures(1, &nInternalNum);
    }
    _pOwner->getManagedConsole().SOLnOO("Done!");
    _pOwner = NULL;
} // ~PureTexture()


TPure_TEX_FORMAT PureTexture::PureTextureImpl::getInternalFormat() const
{
    return texFormat;
} // getInternalFormat()


GLuint PureTexture::PureTextureImpl::getInternalNum() const
{
    return nInternalNum;
} // getInternalNum()


TPure_ISO_TEX_FILTERING PureTexture::PureTextureImpl::getMinFilteringMode() const
{
    return filtIsoMin;
} // getMinFilteringMode()


TPure_ISO_TEX_FILTERING PureTexture::PureTextureImpl::getMagFilteringMode() const
{
    return filtIsoMag;
} // getMagFilteringMode()


TPurebool PureTexture::PureTextureImpl::setMinFilteringMode(TPure_ISO_TEX_FILTERING filtering)
{
    if ( (filtering != Pure_ISO_NEAREST) && (filtering != Pure_ISO_LINEAR) && (nMIPmapCount == 1) )
        return false;

    if ( PureHwInfo::get().getVideo().isAcceleratorDetected() )
    {
        if ( ( nInternalNum == 0 ) ||
             ( !pglBindTexture(GL_TEXTURE_2D, nInternalNum) ) ||
             ( !pglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, getGLnameFromPureisoTexFilteringName(filtering)) ) )
        {
            return false;
        }
    }
    filtIsoMin = filtering;
    return true;
} // setMinFilteringMode()


TPurebool PureTexture::PureTextureImpl::setMagFilteringMode(TPure_ISO_TEX_FILTERING filtering)
{
    if ( (filtering == Pure_ISO_NEAREST) || (filtering == Pure_ISO_LINEAR) )
    {
        
        if ( PureHwInfo::get().getVideo().isAcceleratorDetected() )
        {
            if ( ( nInternalNum == 0 ) ||
                 ( !pglBindTexture(GL_TEXTURE_2D, nInternalNum) ) ||
                 ( !pglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, getGLnameFromPureisoTexFilteringName(filtering)) ) )
            {
                return false;
            }
        }
        filtIsoMag = filtering;
        return true;
    }
    return false;
} // setMagFilteringMode()


TPurebool PureTexture::PureTextureImpl::setIsoFilteringMode(TPure_ISO_TEX_FILTERING minfilter, TPure_ISO_TEX_FILTERING magfilter)
{
    return setMinFilteringMode( minfilter ) && setMagFilteringMode( magfilter );
} // setIsoFilteringMode()


TPureuint PureTexture::PureTextureImpl::getMIPmapCount() const 
{
    return nMIPmapCount;
} // getMIPmapCount()


TPure_ANISO_TEX_FILTERING PureTexture::PureTextureImpl::getAnisoFilteringMode() const
{
    return filtAniso;
} // getAnisoFilteringMode()


TPurebool PureTexture::PureTextureImpl::setAnisoFilteringMode(TPure_ANISO_TEX_FILTERING filtering)
{
    _pOwner->getManagedConsole().OIOLn("PureTexture::setAnisoFilteringMode(%fX)", PureTextureManager::getFloatFromPureanisoTexFilteringName(filtering));
    
    if ( _pOwner->getManager() == PGENULL )
        return false;
    
    if ( !PureHwInfo::get().getVideo().isAcceleratorDetected() )
    {
        _pOwner->getManagedConsole().EOLnOO("WARNING: anisotropic filtering is not supported without acceleration!");
        return false;
    }

    bool result = false;
    const TPurefloat newFilteringF = ((PureTextureManager*)_pOwner->getManager())->getFloatFromPureanisoTexFilteringName(filtering);
    const TPurefloat availMaxFilteringF = ((PureTextureManager*)_pOwner->getManager())->getFloatFromPureanisoTexFilteringName(
        ((PureTextureManager*)_pOwner->getManager())->getMaximumAnisoFiltering() );
    // in Release mode, although getMaximumAnisoFiltering() returns ANISO_16X, availMaxFilteringF becomes 0.
    // This can be fixed by adding a console printout to getFloatFromPureanisoTexFilteringName(), so probably this is some
    // compiler optimization bug which doesnt happen in Debug mode. Would be good to play with the Release mode compiler options
    // to see exactly which option causes this. This is weird because getFloatFromPureanisoTexFilteringName() can never ever be 0.
    // _pOwner->getManagedConsole().OLn( "availMaxFilteringF: %f",  availMaxFilteringF);
    if ( availMaxFilteringF > 1.0f )
    {
        if ( (nInternalNum == 0) || (!pglBindTexture(GL_TEXTURE_2D, nInternalNum)) )
        {
            _pOwner->getManagedConsole().EOLnOO("ERROR: nInternalNum is 0 or pglBindTexture() failed!");
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
            _pOwner->getManagedConsole().EOLn("WARNING: selected mode not supported, setting max supported: %fX!", availMaxFilteringF);
            if ( pglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, (GLfloat) availMaxFilteringF) )
            {
                filtAniso = PureTextureManager::getPureanisoTexFilteringNameFromFloat(availMaxFilteringF);
                result = true;
            }
        }
    }
    else
    {
        _pOwner->getManagedConsole().EOLn("ERROR: anisotropic filtering is not supported");
    }
    _pOwner->getManagedConsole().OO();
    return result;
} // setAnisoFilteringMode()


TPure_TEX_WRAPPING PureTexture::PureTextureImpl::getTextureWrappingModeS() const
{
    return twS;
}


TPure_TEX_WRAPPING PureTexture::PureTextureImpl::getTextureWrappingModeT() const
{
    return twT;
}


TPurebool PureTexture::PureTextureImpl::setTextureWrappingMode(TPure_TEX_WRAPPING tw_s, TPure_TEX_WRAPPING tw_t )
{
    // TODO: probably we should check for availability of given mode on current HW, especially with Pure_TW_MIRRORED_REPEAT and Pure_TW_CLAMP_TO_BORDER  
    if ( PureHwInfo::get().getVideo().isAcceleratorDetected() )
    {
        if ( (nInternalNum == 0) ||
             (!pglBindTexture(GL_TEXTURE_2D, nInternalNum)) ||
             (!pglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, getGLnameFromPuretexWrappingName(tw_s)) ) ||
             (!pglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, getGLnameFromPuretexWrappingName(tw_t)) ) )
        {
            return false;
        }
    }
    twS = tw_s;
    twT = tw_t;
    return true;
}


TPurebool PureTexture::PureTextureImpl::getBorder() const
{
    return bBorder;
}


TPurebool PureTexture::PureTextureImpl::setBorder(TPurebool state)
{
    bBorder = state;
    return true;
}


const PureColor& PureTexture::PureTextureImpl::getBorderColor() const
{
    return clrBorder;
}


TPurebool PureTexture::PureTextureImpl::setBorderColor(const PureColor& clr)
{
    if ( PureHwInfo::get().getVideo().isAcceleratorDetected() )
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


TPurebool PureTexture::PureTextureImpl::uploadPixels()
{
    _pOwner->getManagedConsole().OIOLn("PureTexture::uploadPixels()");

    if ( _pOwner->getPixels() == PGENULL )
    {
        _pOwner->getManagedConsole().EOLnOO("ERROR: getPixels() returned NULL!");
        return false;
    }

    if ( _pOwner->getManager() == PGENULL )
    {
        _pOwner->getManagedConsole().EOLnOO("ERROR: getManager() returned NULL!");
        return false;
    }

    if ( strcmp(typeid(*_pOwner->getManager()).raw_name(), typeid(PureTextureManager&).raw_name()) )
    {
        _pOwner->getManagedConsole().EOLnOO("ERROR: getManager() type is not PureTextureManager!");
        return false;
    }

    if ( PureHwInfo::get().getVideo().isAcceleratorDetected() )
    {
        if (!pglBindTexture(GL_TEXTURE_2D, nInternalNum))
        {
            return false;
        }
    }

    const TPure_PIXEL_COMPONENT_ORDER oldco = _pOwner->getPixelComponentOrder();
    const GLenum glTexFormat = getTransformedSourceFormat(oldco); // might invoke setPixelComponentOrder()!
    if ( glTexFormat == GL_INVALID_ENUM )
    {
        _pOwner->getManagedConsole().EOLnOO("ERROR: getTransformedSourceFormat() failed!");
        // try reset original pixel component order
        if ( !_pOwner->setPixelComponentOrder( oldco ) )
        {
            _pOwner->getManagedConsole().EOLnOO("ERROR: recovery setPixelComponentOrder(oldco) failed!");
        }
        return false;
    }

    const GLint internalfmt = getTargetInternalFormat();

    if ( !actualUploadProc(internalfmt, glTexFormat) )
    {
        _pOwner->getManagedConsole().EOLnOO("ERROR: actualUploadProc() failed!");
        return false;
    }

    DescribeTexFormatAndSize(internalfmt);

    if ( _pOwner->getPixelComponentOrder() != oldco )
    {
        if ( !_pOwner->setPixelComponentOrder( oldco ) )
        {
            _pOwner->getManagedConsole().EOLnOO("ERROR: finished creating new texture, but setPixelComponentOrder(oldco) failed!");
            // intentionally let function still return true
        }
    }

    _pOwner->getManagedConsole().OO();

    return true;
} // uploadPixels()


TPureuint PureTexture::PureTextureImpl::getUsedSystemMemory() const
{
    // no need to calculate with this->_pOwner->getPixelsSize() + here as PureTexture::getUsedSystemMemory() already calls PureImage::getUsedSystemMemory() too
    return sizeof(*this);
} // getUsedSystemMemory()


TPureuint PureTexture::PureTextureImpl::getUsedTextureMemory() const
{
    return nSize;
} // getUsedTextureMemory()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


/**
    Only PureTextureManager creates it.

    @param owner The public class owning this pimpl object.
*/
PureTexture::PureTextureImpl::PureTextureImpl(PureTexture* owner)
{
    _pOwner = owner;
} // PureTexture()


/**
    Only PureTextureManager creates it this way.
*/
PureTexture::PureTextureImpl::PureTextureImpl(PureTexture* owner, const PureImage& img)
{
    _pOwner = owner;
    texFormat  = Pure_TF_RGBA;
    filtIsoMin = filtIsoMag = Pure_ISO_LINEAR;
    filtAniso  = Pure_ANISO_1X;
    nInternalNum = 0;
    nMIPmapCount = 1;
    bBorder = false;
    twS = Pure_TW_REPEAT;
    twT = Pure_TW_REPEAT;
} // PureTexture(...)


PureTexture::PureTextureImpl::PureTextureImpl(const PureTexture::PureTextureImpl&)
{

}


PureTexture::PureTextureImpl& PureTexture::PureTextureImpl::operator=(const PureTexture::PureTextureImpl&)
{
    return *this;
}


/**
    Transforms to a proper pixel component order and returns the proper OpenGL source format.
    For example, if the image's color component order is Pure_BGR and OpenGL driver supports that format natively, no transform is needed, respective OpenGL source format will be returned quickly.
    However if driver doesn't have support for that, then color component order first gets transformed to Pure_RGB, and then respective OpenGL source format will be returned.

    @return OpenGL source format enum. GL_INVALID_ENUM in case of error.
*/
GLenum PureTexture::PureTextureImpl::getTransformedSourceFormat(TPure_PIXEL_COMPONENT_ORDER oldco)
{
    const PureTextureManager& parent = *((PureTextureManager*) _pOwner->getManager());
    const PureHwInfo& hwInfo = PureHwInfo::get();

    if ( oldco == Pure_BGR )
    {
        if ( hwInfo.getVideo().isNativeDIBFormatSupported() && parent.isNativeDIBFormatSupportEnabled() )
            return GL_BGR_EXT;
        else
        {
            if ( !_pOwner->setPixelComponentOrder(Pure_RGB) )
                return GL_INVALID_ENUM;
            return GL_RGB;
        }
    }
    else if ( oldco == Pure_BGRA )
    {
        if ( hwInfo.getVideo().isNativeDIBFormatSupported() && parent.isNativeDIBFormatSupportEnabled() )
            return GL_BGRA_EXT;
        else
        {
            if ( !_pOwner->setPixelComponentOrder(Pure_RGBA) )
                return GL_INVALID_ENUM;
            return GL_RGBA;
        }
    }
    else if ( _pOwner->getPixelComponentOrder() % 2 == 0 )
    {
        if ( oldco != Pure_RGB )
        {
            if ( !_pOwner->setPixelComponentOrder(Pure_RGB) )
                return GL_INVALID_ENUM;
        }
        return GL_RGB;
    }
    else
    {
        if ( oldco != Pure_RGBA )
        {
            if ( !_pOwner->setPixelComponentOrder(Pure_RGBA) )
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
GLint PureTexture::PureTextureImpl::getTargetInternalFormat()
{
    const PureTextureManager& parent = *((PureTextureManager*) _pOwner->getManager());
    const PureHwInfo& hwInfo = PureHwInfo::get();

    if ( hwInfo.getVideo().isTextureCompressionSupported() )
    {
        switch ( parent.getDefaultCompressionMode() )
        {
        case Pure_TC_NONE: return _pOwner->getPixelComponentOrder() % 2 == 0 ? 3 : 4;
        case Pure_TC_S3TC_RGB_DXT1 : return GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
        case Pure_TC_S3TC_RGBA_DXT1: return GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
        case Pure_TC_S3TC_RGBA_DXT3: return GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
        case Pure_TC_S3TC_RGBA_DXT5: return GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
        case Pure_TC_3DFX_RGB_FXT1 : return GL_COMPRESSED_RGB_FXT1_3DFX;
        case Pure_TC_3DFX_RGBA_FXT1: return GL_COMPRESSED_RGBA_FXT1_3DFX;
        /* Pure_TC_AUTO causes the driver to select any of the above */
        default: /* for Pure_TC_AUTO */ return _pOwner->getPixelComponentOrder() % 2 == 0 ? GL_COMPRESSED_RGB_ARB : GL_COMPRESSED_RGBA_ARB;
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
TPurebool PureTexture::PureTextureImpl::actualUploadProc(GLint internalfmt, GLenum glTexFormat)
{
    const PureTextureManager& parent = *((PureTextureManager*) _pOwner->getManager());
    const PureHwInfo& hwInfo = PureHwInfo::get();

    if ( !hwInfo.getVideo().isAcceleratorDetected() )
    {
        _pOwner->getManagedConsole().OLn("no need to upload texture, non-accelerated rendering is active");
        return true;
    }

    if ( nMIPmapCount > 1 )
    {
        _pOwner->getManagedConsole().OLn("mipmapping on");
        if ( hwInfo.getVideo().isHardwareMipMapGenerationSupported() && parent.isHardwareMipMapGenerationEnabled() )
        {        
            _pOwner->getManagedConsole().OLn("auto-generating mipmaps with HW-support.");
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
            _pOwner->getManagedConsole().OLn("auto-generating mipmaps with SW-only.");
            const GLint errCode = gluBuild2DMipmaps(GL_TEXTURE_2D, internalfmt, _pOwner->getWidth(), _pOwner->getHeight(), glTexFormat, GL_UNSIGNED_BYTE, _pOwner->getPixels());
            if ( errCode != GL_NO_ERROR )
            {
                _pOwner->getManagedConsole().EOLn("PureTextureImpl::actualUploadProc() ERROR: %d, %s", errCode, gluErrorString(errCode));
                return false;
            }
            return true;
        }
    }
    else
    {
        _pOwner->getManagedConsole().OLn("mipmapping off");

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
void PureTexture::PureTextureImpl::DescribeTexFormatAndSize(GLint internalfmt)
{
    nSize = _pOwner->getWidth() * _pOwner->getHeight() * 4;
    texFormat = Pure_TF_RGBA;

    if ( !PureHwInfo::get().getVideo().isAcceleratorDetected() )
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
            switch ( tmpret )
            {
            case GL_RGBA:
                _pOwner->getManagedConsole().OLn("final internal texFormat is GL_RGBA");
                break;
            case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
                texFormat = Pure_TF_DXT1;
                _pOwner->getManagedConsole().OLn("final internal texFormat is GL_COMPRESSED_RGB_S3TC_DXT1_EXT");
                break;
            case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
                texFormat = Pure_TF_DXT1A;
                _pOwner->getManagedConsole().OLn("final internal texFormat is GL_COMPRESSED_RGBA_S3TC_DXT1_EXT");
                break;
            case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
                texFormat = Pure_TF_DXT3A;
                _pOwner->getManagedConsole().OLn("final internal texFormat is GL_COMPRESSED_RGBA_S3TC_DXT3_EXT");
                break;
            case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
                texFormat = Pure_TF_DXT5A;
                _pOwner->getManagedConsole().OLn("final internal texFormat is GL_COMPRESSED_RGBA_S3TC_DXT5_EXT");
                break;
            case GL_COMPRESSED_RGB_FXT1_3DFX:
                texFormat = Pure_TF_FXT1;
                _pOwner->getManagedConsole().OLn("final internal texFormat is GL_COMPRESSED_RGB_FXT1_3DFX");
                break;
            case GL_COMPRESSED_RGBA_FXT1_3DFX:
                texFormat = Pure_TF_FXT1A;
                _pOwner->getManagedConsole().OLn("final internal texFormat is GL_COMPRESSED_RGBA_FXT1_3DFX");
                break;
            /* In 14.12 AMD Catalyst Omega Software, requesting auto-compression at glTexImage2D() returns general compression here instead of any if the specific values above. */
            case GL_COMPRESSED_RGB_ARB:
                texFormat = Pure_TF_UNSURE;
                _pOwner->getManagedConsole().OLn("final internal texFormat is GL_COMPRESSED_RGB_ARB");
                break;
            case GL_COMPRESSED_RGBA_ARB:
                texFormat = Pure_TF_UNSURE;
                _pOwner->getManagedConsole().OLn("final internal texFormat is GL_COMPRESSED_RGBA_ARB");
                break;
            default:
                _pOwner->getManagedConsole().EOLn("final internal texFormat ERROR: glGetTexLevelParameteriv() returned %d for GL_TEXTURE_INTERNAL_FORMAT", tmpret);
                break;
            }
            glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &tmpret);
            nSize = tmpret;
        }
    }
    if ( nMIPmapCount > 1 )
    {
        nSize = PureTextureManager::getMIPmappedTexSize(nSize, _pOwner->getWidth(), _pOwner->getHeight());
    }
} // DescribeTexFormatAndSize()


/*
   PureTexture
   ###########################################################################
*/


// ############################### PUBLIC ################################


PureTexture::~PureTexture()
{
    delete pImpl;
    pImpl = NULL;
} // ~PureTexture()


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& PureTexture::getManagedConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* PureTexture::getLoggerModuleName()
{
    return "PureTexture";
} // getLoggerModuleName()


/**
    Gets the internal texture format.    
    @return The internal texture format.
*/
TPure_TEX_FORMAT PureTexture::getInternalFormat() const
{
    return pImpl->getInternalFormat();
} // getInternalFormat()


/**
    Gets the internal texture ID.
    @return The texture ID.
*/
TPureuint PureTexture::getInternalNum() const
{
    return (TPureuint) pImpl->getInternalNum();
    // todo: later this should be fixed: the method should be protected and GLuint can be used explicitly as return value type
} // getInternalNum()


/**
    Gets the isotropic filtering mode when zooming out (1 texel < 1 pixel).
    @return The isotropic filtering mode when zooming out.
*/
TPure_ISO_TEX_FILTERING PureTexture::getMinFilteringMode() const
{
    return pImpl->getMinFilteringMode();
} // getMinFilteringMode()


/**
    Gets the isotropic filtering mode when zooming in (1 texel > 1 pixel).
    @return The isotropic filtering mode when zooming in.
*/
TPure_ISO_TEX_FILTERING PureTexture::getMagFilteringMode() const
{
    return pImpl->getMagFilteringMode();
} // getMagFilteringMode()


/**
    Sets the isotropic filtering mode when zooming out (1 texel < 1 pixel).
    If the texture has been created without MIP maps, only Pure_ISO_NEAREST and Pure_ISO_LINEAR are acceptable.
    @return True on success, false otherwise.
*/
TPurebool PureTexture::setMinFilteringMode(TPure_ISO_TEX_FILTERING filtering)
{
    return pImpl->setMinFilteringMode(filtering);
} // setMinFilteringMode()


/**
    Sets the isotropic filtering mode when zooming in (1 texel > 1 pixel).
    Only the following 2 values are accepted: Pure_ISO_NEAREST and Pure_ISO_LINEAR.
    @return True on success, false otherwise.
*/
TPurebool PureTexture::setMagFilteringMode(TPure_ISO_TEX_FILTERING filtering)
{
    return pImpl->setMagFilteringMode(filtering);
} // setMagFilteringMode()


/**
    Sets the isotropic filtering modes.
    Equivalent to calling setMinFilteringMode(minfilter) && setMagFilteringMode(magfilter).
    @return True on success, false otherwise.
*/
TPurebool PureTexture::setIsoFilteringMode(TPure_ISO_TEX_FILTERING minfilter, TPure_ISO_TEX_FILTERING magfilter)
{
    return pImpl->setIsoFilteringMode(minfilter, magfilter);
} // setIsoFilteringMode()


/**
    Gets the number of MIP maps.
    @return Number of MIP maps.
*/
TPureuint PureTexture::getMIPmapCount() const 
{
    return pImpl->getMIPmapCount();
} // getMIPmapCount()


/**
    Gets the anisotropic filtering mode.
    @return The anisotropic filtering mode.
*/
TPure_ANISO_TEX_FILTERING PureTexture::getAnisoFilteringMode() const
{
    return pImpl->getAnisoFilteringMode();
} // getAnisoFilteringMode()


/**
    Sets the anisotropic filtering mode.
    Note that if anisotropic filtering is supported but on a lower filtering mode, that will be set and considered as success.
    Obviously if anisotropic filtering is not supported, this function will fail.
    @return True on success, false otherwise.
*/
TPurebool PureTexture::setAnisoFilteringMode(TPure_ANISO_TEX_FILTERING filtering)
{
    return pImpl->setAnisoFilteringMode(filtering);
} // setAnisoFilteringMode()


/**
    Gets the texture wrapping mode for S coordinates.
    @return Texture wrapping mode for S coordinates.
*/
TPure_TEX_WRAPPING PureTexture::getTextureWrappingModeS() const
{
    return pImpl->getTextureWrappingModeS();
}


/**
    Gets the texture wrapping mode for T coordinates.
    @return Texture wrapping mode for T coordinates.
*/
TPure_TEX_WRAPPING PureTexture::getTextureWrappingModeT() const
{
    return pImpl->getTextureWrappingModeT();
}


/**
    Sets the texture wrapping mode.
    @param tw_s Specifies wrapping mode for S texture coordinates.
    @param tw_t Specifies wrapping mode for T texture coordinates.
    @return True on success, false otherwise.
*/
TPurebool PureTexture::setTextureWrappingMode(TPure_TEX_WRAPPING tw_s, TPure_TEX_WRAPPING tw_t )
{
    return pImpl->setTextureWrappingMode(tw_s, tw_t);
}


/**
    Gets the border state.
    @return Texture border state: true if has border, false otherwise.
*/
TPurebool PureTexture::getBorder() const
{
    return pImpl->getBorder();
}


/**
    Sets the border state.
    No effect until uploadPixels() is called.
    Please note that texture borders are not supported with compressed textures.
    @return True on success, false otherwise.
*/
TPurebool PureTexture::setBorder(TPurebool state)
{
    return pImpl->setBorder(state);
}


/**
    Gets color of the border.
    Texture border color is by default black.
    @return Texture border color.
*/
const PureColor& PureTexture::getBorderColor() const
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
TPurebool PureTexture::setBorderColor(const PureColor& clr)
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
TPurebool PureTexture::uploadPixels()
{
    return pImpl->uploadPixels();
} // uploadPixels()


/**
    Gets the amount of allocated system memory for the texture (VRAM not included).
    VRAM usage can be retrieved by getUsedTextureMemory().
    @return Amount of allocated system memory in Bytes.
*/
TPureuint PureTexture::getUsedSystemMemory() const
{
    return PureImage::getUsedSystemMemory() + sizeof(*this) + pImpl->getUsedSystemMemory();
} // getUsedSystemMemory()


/**
    Gets the amount of VRAM allocated (considering compression, MIP mapping, etc...).
    This does not include used system memory by the object itself, including possible cached pixel data.
    Used system memory can be retrieved by getUsedSystemMemory().
    @return Amount of allocated video memory in Bytes.
*/
TPureuint PureTexture::getUsedTextureMemory() const
{
    return pImpl->getUsedTextureMemory();
} // getUsedTextureMemory()


// ############################## PROTECTED ##############################


/**
    Only PureTextureManager creates it.

    @exception std::bad_alloc - This class or its ancestor dynamically allocates memory with operator new, in case of failure the exception is not handled but propagated to caller.
*/
PureTexture::PureTexture()
{
    getManagedConsole().OLnOI("PureTexture() ...");
    pImpl = new PureTextureImpl(this);
    getManagedConsole().SOLnOO("Done!");
} // PureTexture()


/**
    Only PureTextureManager creates it this way.

    @exception std::bad_alloc - This class or its ancestor dynamically allocates memory with operator new, in case of failure the exception is not handled but propagated to caller.
*/
PureTexture::PureTexture(const PureImage& img) : 
   PureImage(img)
{
    getManagedConsole().OLnOI("PureTexture(img) ...");
    pImpl = new PureTextureImpl(this, img);
    getManagedConsole().SOLnOO("Done!");
} // PureTexture(...)


PureTexture::PureTexture(const PureTexture&)
{
    // UNUSED
}


PureTexture& PureTexture::operator=(const PureTexture&)
{
    // UNUSED
    return *this;
}


// ############################### PRIVATE ###############################

