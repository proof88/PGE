/*
    ###################################################################################
    PRRETextureManager.cpp
    This file is part of PRRE.
    PRRETextureManager class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREbaseIncludes.h"  // PCH
#include "PRREpragmas.h"
#include "PRRETextureManager.h"
#include "PRRETextureImpl.h"
#include "Hardware/PRREhwInfo.h"
#include "PRREGLsnippets.h"

using namespace std;


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


