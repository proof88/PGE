/*
    ###################################################################################
    PureTextureManager.cpp
    This file is part of Pure.
    PureTextureManager class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PurebaseIncludes.h"  // PCH
#include "../../include/external/Material/PureTextureManager.h"
#include "../../include/internal/Material/PureTextureImpl.h"
#include "../../include/internal/Material/PureImageImpl.h"
#include "../../include/external/Hardware/PureHwInfo.h"
#include "../../include/internal/PureGLsafeFuncs.h"
#include "../../include/internal/PureGLsnippets.h"
#include "../../include/internal/Purepragmas.h"

using namespace std;


/*
   PureTextureManager::PureTextureManagerImpl
   ###########################################################################
*/


class PureTextureManager::PureTextureManagerImpl
{

public:

    // ---------------------------------------------------------------------------
           
    virtual ~PureTextureManagerImpl();

    TPurebool isInitialized() const; 
  
    TPureuint getUsedTextureMemory() const;                        
    TPurebool isHardwareMipMapGenerationEnabled() const;           
    void      SetHardwareMipMapGenerationEnabled(TPurebool state); 
    TPurebool isNativeDIBFormatSupportEnabled() const;             
    void      SetNativeDIBFormatSupportEnabled(TPurebool state);   
    TPurebool isLazyInstancingEnabled() const;                     
    void      SetLazyInstancingEnabled(TPurebool state);           
    TPurebool isPixelPreservingEnabled() const;                    
    void      SetPixelPreservingEnabled(TPurebool state);                 
    
    TPure_ISO_TEX_FILTERING getDefaultMinFilteringMode() const;          
    TPure_ISO_TEX_FILTERING getDefaultMagFilteringMode() const;          
    TPurebool setDefaultMinFilteringMode(TPure_ISO_TEX_FILTERING filtering);  
    TPurebool setDefaultMagFilteringMode(TPure_ISO_TEX_FILTERING filtering);  
    TPurebool setDefaultIsoFilteringMode(
        TPure_ISO_TEX_FILTERING minfilter,
        TPure_ISO_TEX_FILTERING magfilter);                              
    
    TPure_ANISO_TEX_FILTERING
        getDefaultAnisoFilteringMode() const;      
    TPurebool
        setDefaultAnisoFilteringMode(
            TPure_ANISO_TEX_FILTERING filtering);  
    TPure_ANISO_TEX_FILTERING getMaximumAnisoFiltering() const;

    TPure_TEX_WRAPPING
        getDefaultTextureWrappingModeS() const;
    TPure_TEX_WRAPPING
        getDefaultTextureWrappingModeT() const;
    TPurebool
        setDefaultTextureWrappingMode(
            TPure_TEX_WRAPPING tw_s,
            TPure_TEX_WRAPPING tw_t );
    
    TPure_TEX_COMPRESSION_MODE
        getDefaultCompressionMode() const;     
    TPurebool
        setDefaultCompressionMode(
            TPure_TEX_COMPRESSION_MODE mode); 

    TPurebool getDefaultBorder() const;
    TPurebool      setDefaultBorder(TPurebool state);

          PureColor& getDefaultBorderColor();
    const PureColor& getDefaultBorderColor() const; 

protected: 

    // ---------------------------------------------------------------------------

private:
    static PureHwInfo& pHWInfo;               /**< We need some info on HW. */

    // ---------------------------------------------------------------------------

    PureTextureManager* _pOwner;              /**< The owner public object who creates this pimpl object. */

    TPurebool bInited;                        /**< True if successfully inited, false if not functional. */
    TPureuint nRunningCounter;                /**< Always increased when creating a new Texture instance. */
    TPure_ISO_TEX_FILTERING   filtDefIsoMin,
                              filtDefIsoMag;  /**< Default isotropic filtering modes. */
    TPure_ANISO_TEX_FILTERING filtDefAniso;   /**< Default anisotropic filtering modes. */
    GLfloat   nAnisoFilterMax;                /**< Maximum level of anisotropic filtering. */
    TPurebool bAllowHWMIPmapGen;              /**< Allow generating of HW MIP maps? */
    TPurebool bAllowBGRAext;                  /**< Allow direct uploading of BGRA-format images? */
    TPurebool bAlwaysCreateNewFromFile;       /**< Always create new instances from the same file? */
    TPurebool bKeepPixelsInSysRAM;            /**< Keep pixels in system memory after creating texture? */
    TPure_TEX_COMPRESSION_MODE texComprDef;   /**< Default texture compression mode. */
    TPurebool bDefBorder;                     /**< Default texture border state. */
    PureColor clrDefBorder;                   /**< Default texture border color. By default it is black. */
    TPure_TEX_WRAPPING twDefS, twDefT;        /**< Default texture wrapping mode for S- and T- texture coordinates. */

    // ---------------------------------------------------------------------------

    PureTextureManagerImpl(PureTextureManager* owner);
    PureTextureManagerImpl(const PureTextureManagerImpl&);
    PureTextureManagerImpl& operator=(const PureTextureManagerImpl&);

    TPurebool generateAndUploadTexture(PureTexture& texture);

    friend class PureTextureManager;

};


// ############################### PUBLIC ################################


PureTextureManager::PureTextureManagerImpl::~PureTextureManagerImpl()
{
    _pOwner->getConsole().OLnOI("~PureTextureManager() ...");
    _pOwner->DeleteAll();
    _pOwner->getConsole().OO();    
    _pOwner = NULL;
}


TPurebool PureTextureManager::PureTextureManagerImpl::isInitialized() const
{
    return bInited;
} // isInitialized()


TPureuint PureTextureManager::PureTextureManagerImpl::getUsedTextureMemory() const
{
    TPureuint sum = 0;
    for (TPureint i = 0; i < _pOwner->getSize(); i++)
    {
        PureTexture* pTex = (PureTexture*) _pOwner->getAttachedAt(i);
        if ( pTex!= PGENULL )
            sum += pTex->getUsedTextureMemory();
    }
    return sum;
} // getUsedTextureMemory()


TPurebool PureTextureManager::PureTextureManagerImpl::isHardwareMipMapGenerationEnabled() const
{
    return bAllowHWMIPmapGen;
} // isHardwareMipMapGenerationEnabled()


void PureTextureManager::PureTextureManagerImpl::SetHardwareMipMapGenerationEnabled(TPurebool state)
{
    if ( !bInited )
    {
        return;
    }

    bAllowHWMIPmapGen = state && pHWInfo.getVideo().isHardwareMipMapGenerationSupported();
} // SetHardwareMipMapGenerationEnabled()


TPurebool PureTextureManager::PureTextureManagerImpl::isNativeDIBFormatSupportEnabled() const
{
    return bAllowBGRAext;
} // isNativeDIBFormatSupportEnabled()


void PureTextureManager::PureTextureManagerImpl::SetNativeDIBFormatSupportEnabled(TPurebool state)
{
    if ( !bInited )
    {
        return;
    }

    bAllowBGRAext = state && pHWInfo.getVideo().isNativeDIBFormatSupported();
} // SetNativeDIBFormatSupportEnabled()


TPurebool PureTextureManager::PureTextureManagerImpl::isLazyInstancingEnabled() const
{
    return !bAlwaysCreateNewFromFile;
} // isLazyInstancingEnabled()


void PureTextureManager::PureTextureManagerImpl::SetLazyInstancingEnabled(TPurebool state)
{
    bAlwaysCreateNewFromFile = !state;
} // SetLazyInstancingEnabled()


TPurebool PureTextureManager::PureTextureManagerImpl::isPixelPreservingEnabled() const
{
    return bKeepPixelsInSysRAM;
} // isPixelPreservingEnabled()


void PureTextureManager::PureTextureManagerImpl::SetPixelPreservingEnabled(TPurebool state)
{
    bKeepPixelsInSysRAM = state;
} // SetPixelPreservingEnabled()


TPure_ISO_TEX_FILTERING PureTextureManager::PureTextureManagerImpl::getDefaultMinFilteringMode() const
{
    return filtDefIsoMin;
} // getDefaultMinFilteringMode()


TPure_ISO_TEX_FILTERING PureTextureManager::PureTextureManagerImpl::getDefaultMagFilteringMode() const
{
    return filtDefIsoMag;
} // getDefaultMagFilteringMode()


TPurebool PureTextureManager::PureTextureManagerImpl::setDefaultMinFilteringMode(TPure_ISO_TEX_FILTERING filtering)
{
    filtDefIsoMin = filtering;
    return true;
} // setDefaultMinFilteringMode()


TPurebool PureTextureManager::PureTextureManagerImpl::setDefaultMagFilteringMode(TPure_ISO_TEX_FILTERING filtering)
{
    if ( (filtering == Pure_ISO_NEAREST) || (filtering == Pure_ISO_LINEAR) )
    {
        filtDefIsoMag = filtering;
        return true;
    }
    return false;
} // setDefaultMagFilteringMode()


TPurebool PureTextureManager::PureTextureManagerImpl::setDefaultIsoFilteringMode(TPure_ISO_TEX_FILTERING minfilter, TPure_ISO_TEX_FILTERING magfilter)
{
    return setDefaultMinFilteringMode( minfilter ) && setDefaultMagFilteringMode( magfilter );
} // setDefaultIsoFilteringMode()


TPure_ANISO_TEX_FILTERING PureTextureManager::PureTextureManagerImpl::getDefaultAnisoFilteringMode() const
{
    return filtDefAniso;
} // getDefaultAnisoFilteringMode()


TPurebool PureTextureManager::PureTextureManagerImpl::setDefaultAnisoFilteringMode(TPure_ANISO_TEX_FILTERING filtering)
{
    // TODO: should check for max aniso filter level same way as texture does!
    filtDefAniso = filtering;
    return true;
} // setDefaultAnisoFilteringMode()


TPure_ANISO_TEX_FILTERING PureTextureManager::PureTextureManagerImpl::getMaximumAnisoFiltering() const
{
    return PureTextureManager::getPureanisoTexFilteringNameFromFloat( nAnisoFilterMax );
} // getMaximumAnisoFiltering()


TPure_TEX_WRAPPING PureTextureManager::PureTextureManagerImpl::getDefaultTextureWrappingModeS() const
{
    return twDefS;
}


TPure_TEX_WRAPPING PureTextureManager::PureTextureManagerImpl::getDefaultTextureWrappingModeT() const
{
    return twDefT;
}


TPurebool PureTextureManager::PureTextureManagerImpl::setDefaultTextureWrappingMode(TPure_TEX_WRAPPING tw_s, TPure_TEX_WRAPPING tw_t )
{
    twDefS = tw_s;
    twDefT = tw_t;
    return true;
}


TPure_TEX_COMPRESSION_MODE PureTextureManager::PureTextureManagerImpl::getDefaultCompressionMode() const
{
    return texComprDef;
} // getDefaultCompressionMode()


TPurebool PureTextureManager::PureTextureManagerImpl::setDefaultCompressionMode(TPure_TEX_COMPRESSION_MODE mode)
{
    texComprDef = mode;
    return true;
} // setDefaultCompressionMode()


/**
    Gets the default border state.
*/
TPurebool PureTextureManager::PureTextureManagerImpl::getDefaultBorder() const
{
    return bDefBorder;
}


/**
    Sets the default border state.
*/
TPurebool PureTextureManager::PureTextureManagerImpl::setDefaultBorder(TPurebool state)
{
    bDefBorder = state;
    return true;
}


PureColor& PureTextureManager::PureTextureManagerImpl::getDefaultBorderColor()
{
    return clrDefBorder;
}


const PureColor& PureTextureManager::PureTextureManagerImpl::getDefaultBorderColor() const
{
    return clrDefBorder;
}


// ############################## PROTECTED ##############################


PureHwInfo& PureTextureManager::PureTextureManagerImpl::pHWInfo = PureHwInfo::get();


// ############################### PRIVATE ###############################


/**
    Sets default values.
    Requires a valid initialized PureHwInfo instance to be functional.

    @param owner The public class owning this pimpl object.
*/
PureTextureManager::PureTextureManagerImpl::PureTextureManagerImpl(PureTextureManager* owner)
{
    _pOwner = owner;
    _pOwner->getConsole().OLnOI("PureTextureManager() ...");
    bInited = false;
    nRunningCounter = 0;
    filtDefIsoMin = Pure_ISO_LINEAR;
    filtDefIsoMag = Pure_ISO_LINEAR;
    filtDefAniso  = Pure_ANISO_1X;
    bAlwaysCreateNewFromFile = false;
    bKeepPixelsInSysRAM = pHWInfo.isInitialized() ? ( pHWInfo.getVideo().isAcceleratorDetected() ? false : true ) : false;
    nAnisoFilterMax = pHWInfo.isInitialized() ? pHWInfo.getVideo().getMaximumAnisoFiltering() : 1.0f;
    bAllowHWMIPmapGen = pHWInfo.isInitialized() ? pHWInfo.getVideo().isHardwareMipMapGenerationSupported() : false;
    bAllowBGRAext = pHWInfo.isInitialized() ? pHWInfo.getVideo().isNativeDIBFormatSupported() : false;
    if ( pHWInfo.isInitialized() )
    {
        if ( pHWInfo.getVideo().isTextureCompressionSupported() )
        {
            pglHint(GL_TEXTURE_COMPRESSION_HINT_ARB, GL_NICEST);
        }
    }
    texComprDef = Pure_TC_NONE;
    bDefBorder = false;
    twDefS = twDefT = Pure_TW_REPEAT;

    if ( !pHWInfo.isInitialized() )
    {
        _pOwner->getConsole().EOLnOO("No HardwareInfo!");
        return;
    }

    bInited = true;
    _pOwner->getConsole().SOLnOO("Done!");
} 


PureTextureManager::PureTextureManagerImpl::PureTextureManagerImpl(const PureTextureManager::PureTextureManagerImpl&)
{

}


PureTextureManager::PureTextureManagerImpl& PureTextureManager::PureTextureManagerImpl::operator=(const PureTextureManager::PureTextureManagerImpl&)
{
    return *this;
}


TPurebool PureTextureManager::PureTextureManagerImpl::generateAndUploadTexture(PureTexture& texture)
{
    if ( (getDefaultMinFilteringMode() == Pure_ISO_NEAREST) || (getDefaultMinFilteringMode() == Pure_ISO_LINEAR) )
        texture.pImpl->nMIPmapCount = 1;
    else
        texture.pImpl->nMIPmapCount = PureTextureManager::getMIPmapCount( texture.getWidth(), texture.getHeight() );

    if ( pHWInfo.getVideo().isAcceleratorDetected() )
    {
        if ( !pglGenTextures(1, &(texture.pImpl->nInternalNum)) ) 
        {
            return false;
        }

        if ( !pglBindTexture(GL_TEXTURE_2D, texture.pImpl->nInternalNum) )
        {
            return false;
        }

        if ( !pglTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE) )
        {
            return false;
        }
    }

    bool b = true;
    b &= texture.setMagFilteringMode( getDefaultMagFilteringMode() );
    b &= texture.setMinFilteringMode( getDefaultMinFilteringMode() );
    b &= texture.setAnisoFilteringMode( getDefaultAnisoFilteringMode() );
    b &= texture.setTextureWrappingMode( getDefaultTextureWrappingModeS(), getDefaultTextureWrappingModeT() );
    b &= texture.setBorder( getDefaultBorder() );
    b &= texture.setBorderColor( getDefaultBorderColor() );
    if ( !b )
    {
        _pOwner->getConsole().EOLn("ERROR: failed to set an attribute to the texture!");
        return false;
    }

    if ( !texture.uploadPixels() )
    {
        _pOwner->getConsole().EOLn("ERROR: uploadPixels() failed!");
        return false;
    }

    return true;
}


/*
   PureTextureManager
   ###########################################################################
*/


// ############################### PUBLIC ################################


/**
    Calculates the number of MIP maps from the given texture size.
    @return Number of MIP maps based on the given texture size.
*/
TPureuint PureTextureManager::getMIPmapCount(TPureuint width, TPureuint height)
{
    if ( width == 0 || height == 0 )
        return 0;
                                                   
    TPureuint value = max(width,height);
    TPureuint count = 1;
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
TPureuint PureTextureManager::getWidthAtLevel(TPureuint level, TPureuint width, TPureuint height)
{
    if ( width == 0 || height == 0 )
        return 0;

    TPureuint value = max(width,height);
    TPureuint actlevel = 0;
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
TPureuint PureTextureManager::getHeightAtLevel(TPureuint level, TPureuint width, TPureuint height)
{
    if ( width == 0 || height == 0 )
        return 0;

    TPureuint value = max(width,height);
    TPureuint actlevel = 0;
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
TPureuint PureTextureManager::getMIPmappedTexSize(TPureuint basesize, TPureuint width, TPureuint height)
{
    if ( (basesize > 0) && (width > 0) && (height > 0) )
    {
        TPureuint value = max(width,height);
        TPureuint size = basesize;
        TPureuint size2 = size;
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
    Gets the Pure aniso texture filtering value that corresponds to the given floating point value.
    @return A Pure aniso texture filtering value corresponding to he given value.
            Pure_ANISO_1X if invalid value is given.
*/
TPure_ANISO_TEX_FILTERING PureTextureManager::getPureanisoTexFilteringNameFromFloat(TPurefloat value)
{
    switch ( (int)value )
    {
    case  0:
    case  1: return  Pure_ANISO_1X;
    case  2: return  Pure_ANISO_2X;
    case  4: return  Pure_ANISO_4X;
    case  8: return  Pure_ANISO_8X;
    case 16: return Pure_ANISO_16X;
    default: return Pure_ANISO_1X;
    }
} // getPureanisoTexFilteringNameFromFloat()


/**
    Gets the Pure aniso texture filtering value that corresponds to the given integer value.
    @return A Pure aniso texture filtering value corresponding to he given value.
            Pure_ANISO_1X if invalid value is given.
*/
TPure_ANISO_TEX_FILTERING PureTextureManager::getPureanisoTexFilteringNameFromUInt(TPureuint value)
{
    switch ( value )
    {
    case  0:
    case  1: return  Pure_ANISO_1X;
    case  2: return  Pure_ANISO_2X;
    case  4: return  Pure_ANISO_4X;
    case  8: return  Pure_ANISO_8X;
    case 16: return Pure_ANISO_16X;
    default: return Pure_ANISO_1X;
    }
} // getPureanisoTexFilteringNameFromUInt()


/**
    Gets the value that corresponds to the given Pure aniso texture filtering value.
    @return A floating point value corresponding to the given value.
            1.0 if invalid value is given.
*/
TPurefloat PureTextureManager::getFloatFromPureanisoTexFilteringName(TPure_ANISO_TEX_FILTERING value)
{
    switch ( value )
    {
    case  Pure_ANISO_1X:  return 1.0f;
    case  Pure_ANISO_2X:  return 2.0f;
    case  Pure_ANISO_4X:  return 4.0f;
    case  Pure_ANISO_8X:  return 8.0f;
    case Pure_ANISO_16X: return 16.0f;
    default: return 1.0f;
    }
} // getFloatFromPureanisoTexFilteringName()


/**
    Gets the value that corresponds to the given Pure aniso texture filtering value.
    @return An integer value corresponding to the given value.
            1 if invalid value is given.
*/
TPureuint PureTextureManager::getUIntFromPureanisoTexFilteringName(TPure_ANISO_TEX_FILTERING value)
{
    switch ( value )
    {
    case  Pure_ANISO_1X: return  1;
    case  Pure_ANISO_2X: return  2;
    case  Pure_ANISO_4X: return  4;
    case  Pure_ANISO_8X: return  8;
    case Pure_ANISO_16X: return 16;
    default: return 1;
    }
} // getUIntFromPureanisoTexFilteringName()


/**
    Sets default values.
    Requires a valid initialized PureHwInfo instance to be functional.

    @exception std::bad_alloc - This class or its ancestor dynamically allocates memory with operator new, in case of failure the exception is not handled but propagated to caller.
*/
PureTextureManager::PureTextureManager() :
    PureImageManager()
{
    pImpl = new PureTextureManagerImpl(this);
} // PureTextureManager(...)


PureTextureManager::~PureTextureManager()
{
    delete pImpl;
    pImpl = NULL;
} // ~PureTextureManager()


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& PureTextureManager::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* PureTextureManager::getLoggerModuleName()
{
    return "PureTextureManager";
} // getLoggerModuleName()


/**
    Tells whether the object is correctly initialized or not.
    @return True if TextureManager instance is initialized, false otherwise.
*/
TPurebool PureTextureManager::isInitialized() const
{
    return pImpl->isInitialized();
} // isInitialized()


/**
    Gets the total amount of allocated VRAM by all of the textures.
    @return Amount of allocated video memory for textures in Bytes.
*/
TPureuint PureTextureManager::getUsedTextureMemory() const
{
    return pImpl->getUsedTextureMemory();
} // getUsedTextureMemory()


/**
    Gets whether HW MIP map generation is enabled or not.
    Enabled by default.

    @return True if HW MIP map generation is enabled, false otherwise.
*/
TPurebool PureTextureManager::isHardwareMipMapGenerationEnabled() const
{
    return pImpl->isHardwareMipMapGenerationEnabled();
} // isHardwareMipMapGenerationEnabled()


/**
    Sets whether HW MIP map generation is enabled or not.
    Can be enabled only if the system supports it.
    Enabled by default, if supported.
*/
void PureTextureManager::SetHardwareMipMapGenerationEnabled(TPurebool state)
{
    pImpl->SetHardwareMipMapGenerationEnabled(state);
} // SetHardwareMipMapGenerationEnabled()


/**
    Gets whether direct upload of BGRA-format images is enabled or not.
    Enabled by default, if supported.

    @return True if direct upload of BGRA-format images is enabled, false otherwise.
*/
TPurebool PureTextureManager::isNativeDIBFormatSupportEnabled() const
{
    return pImpl->isNativeDIBFormatSupportEnabled();
} // isNativeDIBFormatSupportEnabled()


/**
    Sets whether direct upload of BGRA-format images is enabled or not.
    Can be enabled only if supported.
    Enabled by default, if supported.
*/
void PureTextureManager::SetNativeDIBFormatSupportEnabled(TPurebool state)
{
    pImpl->SetNativeDIBFormatSupportEnabled(state);
} // SetNativeDIBFormatSupportEnabled()


/**
    Lazy instancing means we don't create new texture instance for the same file that has been already loaded as a texture.
    On by default.
*/
TPurebool PureTextureManager::isLazyInstancingEnabled() const
{
    return pImpl->isLazyInstancingEnabled();
} // isLazyInstancingEnabled()


/**
    Lazy instancing means we don't create new texture instance for the same file that has been already loaded as a texture.
    If enabled, creating texture multiple times from the same image will result in creating only 1 PureTexture and keep referring to the same later.
    On by default.
*/
void PureTextureManager::SetLazyInstancingEnabled(TPurebool state)
{
    pImpl->SetLazyInstancingEnabled(state);
} // SetLazyInstancingEnabled()


/**
    Gets whether keeping pixels of loaded textures in system memory is enabled or not.
    Off by default.
    Keeping pixels in system memory means that after successfully creating the texture in
    video memory, the Image part of the Texture should still keep the pixels in system memory.
    This is useful when you want to update pixels after creating the texture but takes more memory.
*/
TPurebool PureTextureManager::isPixelPreservingEnabled() const
{
    return pImpl->isPixelPreservingEnabled();
} // isPixelPreservingEnabled()


/**
    Sets whether to keep pixels of loaded textures in system memory or delete them after texture uploading.
    Off by default.
    Keeping pixels in system memory means that after successfully creating the texture in
    video memory, the Image part of the Texture should still keep the pixels in system memory.
    This is useful when you want to update pixels after creating the texture but takes more memory.
*/
void PureTextureManager::SetPixelPreservingEnabled(TPurebool state)
{
    pImpl->SetPixelPreservingEnabled(state);
} // SetPixelPreservingEnabled()


/**
    Creates texture from the given image.
    This method may change the color component order of the given image temporarily but restores it after creating the texture.
    Based on lazy instancing setting, this function may return an already existing Texture instance if such exists with same file name.
    Lazy instancing is controlled by SetLazyInstancingEnabled().

    @return The constructed texture object on success, PGENULL otherwise.
*/
PureTexture* PureTextureManager::createTextureFromImage(const PureImage& img)
{
    getConsole().OLnOI("PureTextureManager::createTextureFromImage(...)");

    if ( !isInitialized() )
    {
        getConsole().OO();
        return PGENULL;
    }

    PureTexture* texture = isLazyInstancingEnabled() ? (PureTexture*) getByFilename( img.getFilename().c_str() ) : PGENULL;

    if ( texture != PGENULL )
    {   // we already have a texture with this filename
        getConsole().SOLn("> Found loaded texture, returning.");
        getConsole().OLnOO("");
        return texture;
    }

    getConsole().OLn("texture == PGENULL");
    try
    {
        texture = new PureTexture(img);
    }
    catch (const std::bad_alloc&)
    {
        getConsole().EOLn("ERROR: Failed to instantiate new PureTexture!");
        getConsole().OLnOO("");
        return PGENULL;
    }

    Attach(*texture);

    if ( !pImpl->generateAndUploadTexture(*texture) )
    {
        getConsole().EOLn("ERROR: generateAndUploadTexture() failed!");
        getConsole().OLnOO("");
        DeleteAttachedInstance(*texture);
        return PGENULL;
    }

    if ( !isPixelPreservingEnabled() )
        texture->FlushResources();

    texture->SetName("Texture " + std::to_string(pImpl->nRunningCounter++));

    getConsole().OLn("Texture nWidth and nHeight: %d %d", texture->getWidth(), texture->getHeight());
    getConsole().OLn("final nSize is %d Byte(s).", texture->pImpl->nSize);
    getConsole().OLn("internalnum = %d", texture->pImpl->nInternalNum);
    getConsole().OLn("");
    getConsole().SOLn("> Texture created, name: %s!", texture->getName().c_str());
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
PureTexture* PureTextureManager::createFromFile(const char* filename)
{
    if ( !isInitialized() )
    {
        return PGENULL;
    }

    getConsole().OLnOI("PureTextureManager::createFromFile(\"%s\")", filename);

    if ( filename == PGENULL )
    {
        getConsole().EOLnOO("ERROR: input was NULL, returning PGENULL");
        getConsole().OLn("");
        return PGENULL;
    }

    PureTexture* texture = isLazyInstancingEnabled() ? (PureTexture*) getByFilename( filename ) : PGENULL;

    if ( texture != PGENULL )
    {   // we already have a texture with this filename
        getConsole().SOLnOO("> Found loaded texture, returning: %s", texture->getName().c_str());
        getConsole().OLn("");
        return texture;
    }

    getConsole().OI();
    PureImage* tmpImg = PureImageManager::createFromFile(filename);
    getConsole().OO();
    if ( tmpImg == PGENULL )
    {
        getConsole().EOLnOO("ERROR: failed to create Image from file!");
        getConsole().OLn("");
        return PGENULL;
    }

    try
    {
        texture = new PureTexture(*tmpImg);
        PureImage* textureAsImage = (PureImage*)texture;
        if ( !textureAsImage->pImpl->cannibalize(*tmpImg) )
        {
            const std::string sErrMsg = "cannibalize() failed!";
            throw std::runtime_error(sErrMsg);
        }
    }
    catch (const std::exception& e)
    {
        getConsole().EOLn("ERROR: Failed to create or fill new PureTexture: %s!", e.what());
        DeleteAttachedInstance(*tmpImg);
        getConsole().OLnOO("");
        return PGENULL;
    }
    DeleteAttachedInstance(*tmpImg);

    Attach(*texture);

    if ( !pImpl->generateAndUploadTexture(*texture) )
    {
        getConsole().EOLn("ERROR: generateAndUploadTexture() failed!");
        getConsole().OLnOO("");
        DeleteAttachedInstance(*texture);
        return PGENULL;
    }

    if ( !isPixelPreservingEnabled() )
        texture->FlushResources();

    texture->SetName("Texture " + std::to_string(pImpl->nRunningCounter++));

    getConsole().OLn("Texture nWidth and nHeight: %d %d", texture->getWidth(), texture->getHeight());
    getConsole().OLn("final nSize is %d Byte(s).", texture->pImpl->nSize);
    getConsole().OLn("internalnum = %d", texture->pImpl->nInternalNum);
    getConsole().OLn("");
    getConsole().SOLn("> Texture created, name: %s!", texture->getName().c_str());
    getConsole().OOOLn("");
    return texture;
} // createFromFile()


/**
    Gets the default isotropic filtering mode when zooming out (when 1 texel < 1 pixel).
    The default mode is the mode that will be set at every texture creation.
    The default isotropic filtering mode when zooming out is Pure_ISO_LINEAR.

    @return Default isotropic filtering when zooming out.
*/
TPure_ISO_TEX_FILTERING PureTextureManager::getDefaultMinFilteringMode() const
{
    return pImpl->getDefaultMinFilteringMode();
} // getDefaultMinFilteringMode()


/**
    Gets the default isotropic filtering mode when zooming in (when 1 texel > 1 pixel).
    The default mode is the mode that will be set at every texture creation.
    The default isotropic filtering mode when zooming in is Pure_ISO_LINEAR.

    @return Default isotropic filtering when zooming in.
*/
TPure_ISO_TEX_FILTERING PureTextureManager::getDefaultMagFilteringMode() const
{
    return pImpl->getDefaultMagFilteringMode();
} // getDefaultMagFilteringMode()


/**
    Sets the default isotropic filtering mode when zooming out (when 1 texel < 1 pixel).
    The default mode is the mode that will be set at every texture creation.
    The default isotropic filtering mode when zooming out is Pure_ISO_LINEAR.
    @return True on success, false otherwise.
*/
TPurebool PureTextureManager::setDefaultMinFilteringMode(TPure_ISO_TEX_FILTERING filtering)
{
    return pImpl->setDefaultMinFilteringMode(filtering);
} // setDefaultMinFilteringMode()


/**
    Sets the default isotropic filtering mode when zooming in (when 1 texel > 1 pixel).
    The default mode is the mode that will be set at every texture creation.
    The default isotropic filtering mode when zooming in is Pure_ISO_LINEAR.
    Only the following 2 values are accepted: Pure_ISO_NEAREST and Pure_ISO_LINEAR.
    @return True on success, false otherwise.
*/
TPurebool PureTextureManager::setDefaultMagFilteringMode(TPure_ISO_TEX_FILTERING filtering)
{
    return pImpl->setDefaultMagFilteringMode(filtering);
} // setDefaultMagFilteringMode()


/**
    Sets the default isotropic filtering modes.
    Equivalent to calling setDefaultMinFilteringMode(minfilter) && setDefaultMagFilteringMode(magfilter).
    @return True on success, false otherwise.
*/ 
TPurebool PureTextureManager::setDefaultIsoFilteringMode(TPure_ISO_TEX_FILTERING minfilter, TPure_ISO_TEX_FILTERING magfilter)
{
    return pImpl->setDefaultIsoFilteringMode(minfilter, magfilter);
} // setDefaultIsoFilteringMode()


/**
    Gets the default anisotropic filtering level.
    The default mode is the mode that will be set at every texture creation.
    The default anisotropic filtering mode is Pure_ANISO_1X.

    @return Default anisotropic filtering.
*/
TPure_ANISO_TEX_FILTERING PureTextureManager::getDefaultAnisoFilteringMode() const
{
    return pImpl->getDefaultAnisoFilteringMode();
} // getDefaultAnisoFilteringMode()


/**
    Sets the default anisotropic filtering level.
    The default mode is the mode that will be set at every texture creation.
    The default anisotropic filtering mode is Pure_ANISO_1X, meaning it is off.
    Not every system supports anisotropic filtering.
    @return True on success, false otherwise.
*/
TPurebool PureTextureManager::setDefaultAnisoFilteringMode(TPure_ANISO_TEX_FILTERING filtering)
{
    return pImpl->setDefaultAnisoFilteringMode(filtering);
} // setDefaultAnisoFilteringMode()


/**
    Gets the maximum level of anisotropic filtering.
    Not every system supports anisotropic filtering.

    @return Maximum level of anisotropic filtering, Pure_ANISO_1X if not supported.
*/
TPure_ANISO_TEX_FILTERING PureTextureManager::getMaximumAnisoFiltering() const
{
    return pImpl->getMaximumAnisoFiltering();
} // getMaximumAnisoFiltering()


/**
    Gets the default texture wrapping mode for S coordinates.
    The default mode is the mode that will be set at every texture creation.
    The default texture wrapping mode is Pure_TW_REPEAT.
*/
TPure_TEX_WRAPPING PureTextureManager::getDefaultTextureWrappingModeS() const
{
    return pImpl->getDefaultTextureWrappingModeS();
}


/**
    Gets the default texture wrapping mode for T coordinates.
    The default mode is the mode that will be set at every texture creation.
    The default texture wrapping mode is Pure_TW_REPEAT.
*/
TPure_TEX_WRAPPING PureTextureManager::getDefaultTextureWrappingModeT() const
{
    return pImpl->getDefaultTextureWrappingModeT();
}


/**
    Sets the default texture wrapping mode.
    The default mode is the mode that will be set at every texture creation.
    The default texture wrapping mode is Pure_TW_REPEAT for S- and T- texture coordinates.
    @return True on success, false otherwise.
*/
TPurebool PureTextureManager::setDefaultTextureWrappingMode(TPure_TEX_WRAPPING tw_s, TPure_TEX_WRAPPING tw_t )
{
    return pImpl->setDefaultTextureWrappingMode(tw_s, tw_t);
}


/**
    Gets the default texture compression mode.
    The default mode is the mode that will be set at every texture creation.
    The default texture compression mode is Pure_TC_NONE.
    Not every system supports texture compression.

    @return Default texture compression mode. Default value is Pure_TC_NONE.
*/
TPure_TEX_COMPRESSION_MODE PureTextureManager::getDefaultCompressionMode() const
{
    return pImpl->getDefaultCompressionMode();
} // getDefaultCompressionMode()


/**
    Sets the default texture compression mode.
    The default mode is the mode that will be used at every texture creation.
    The default texture compression mode is Pure_TC_NONE.
    Not every system supports texture compression.

    You can select RGBA compression modes even if source image is in RGB. In that case, alpha values
    will be automatically set to 1.
    You can also select RGB compression mode even if source image is in RGBA. In that case, alpha values
    will be ignored.
    Please note that texture borders are not supported with compressed textures.
    @return True on success, false otherwise.
*/
TPurebool PureTextureManager::setDefaultCompressionMode(TPure_TEX_COMPRESSION_MODE mode)
{
    return pImpl->setDefaultCompressionMode(mode);
} // setDefaultCompressionMode()


/**
    Gets the default border state.
    The default mode is the mode that will be set at every texture creation.
    Please note that texture borders are not supported with compressed textures.
*/
TPurebool PureTextureManager::getDefaultBorder() const
{
    return pImpl->getDefaultBorder();
}


/**
    Sets the default border state.
    The default mode is the mode that will be set at every texture creation.
    Please note that texture borders are not supported with compressed textures.
    @return True on success, false otherwise.
*/
TPurebool PureTextureManager::setDefaultBorder(TPurebool state)
{
    return pImpl->setDefaultBorder(state);
}


/**
    Gets default color of the border.
    The default mode is the mode that will be set at every texture creation.
    Texture default border color is by default black.
    Please note that texture borders are not supported with compressed textures.
*/
PureColor& PureTextureManager::getDefaultBorderColor()
{
    return pImpl->getDefaultBorderColor();
}


/**
    Gets default color of the border.
    The default mode is the mode that will be set at every texture creation.
    Texture default border color is by default black.
    Please note that texture borders are not supported with compressed textures.
*/
const PureColor& PureTextureManager::getDefaultBorderColor() const
{
    return pImpl->getDefaultBorderColor();
}


/**
    From PureImageManager, adding total texture memory usage counting.
*/
void PureTextureManager::WriteList() const
{
    getConsole().OLn("PureTextureManager::WriteList()");
    getConsole().OLn("===============================");
    getConsole().OLnOI("");
    PureImageManager::WriteList();
    getConsole().OO();

    const TPureuint nVRAMtotal = getUsedTextureMemory();

    getConsole().OIOLnOO("> total used texture memory = %d Bytes <= %d kBytes <= %d MBytes", nVRAMtotal, (int)(ceil(nVRAMtotal/1024.0f)), (int)(ceil(nVRAMtotal/1024.0f/1024.0f)));
    getConsole().OLn("");
} // WriteList()


// ############################## PROTECTED ##############################


PureTextureManager::PureTextureManager(const PureTextureManager&)
{

}


PureTextureManager& PureTextureManager::operator=(const PureTextureManager&)
{
    return *this;
}


/**
    From PureImageManager, texture memory and internal number.
*/
void PureTextureManager::WriteListCallback(const PureManaged& mngd) const
{
    PureImageManager::WriteListCallback(mngd);
    PureTexture& tex = (PureTexture&) mngd;
    getConsole().OIOLnOO("id: %d, MIPmaps: %d, fmt: %d, tex mem: %d kB;",
        tex.pImpl->nInternalNum,
        tex.getMIPmapCount(),
        tex.getInternalFormat(),
        (int)ceil(tex.getUsedTextureMemory()/1024.0f));
} // WriteListCallback()


// ############################### PRIVATE ###############################



