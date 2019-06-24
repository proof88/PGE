/*
    ###################################################################################
    PRREhwVideoImpl.cpp
    This file is part of PRRE.
    PRRE video capabilities class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/


#include "PRREbaseIncludes.h"  // PCH
#include "PRREpragmas.h"
#include "PRREhwVideo.h"
#include "PRREGLsnippets.h"
#include "PRREGLextensionFuncs.h"
#include "PRRESharedSettings.h"

using namespace std;

class PRREhwVideoImpl :
    public PRREhwVideo
{

public:
    static PRREhwVideoImpl& get();

    // ---------------------------------------------------------------------------

    void Initialize(
        HGLRC rc, HDC wnd_dc,
        TPRREuint nResX, TPRREuint nResY,
        TPRREint nColorBits, TPRREint nDepthBits,
        TPRREint nStencilBits, TPRREint nFSAAlevel );

    TPRREbool  isAcceleratorDetected() const;
    TPRREuint  getUsedVideoMemory() const;       
    TPRREuint  getUsedTextureMemory() const;  
    TPRREuint  getColorBufferPixelCount() const; 
    TPRREuint  getColorBufferSize() const;      
    TPRREuint  getDepthBufferSize() const;      
    TPRREuint  getStencilBufferSize() const;   
    TPRREuint  getSampleBufferSize() const;       
    TPRREuint  getUsedFrameBufferMemory() const; 
    TPRREbool  isVSyncSupported() const;          
    TPRREbool  isMultiTexturingSupported() const; 
    TPRREint   getTextureUnitsCount() const;    
    TPRREbool  isHardwareMipMapGenerationSupported() const;  
    TPRREbool  isNativeDIBFormatSupported() const;       
    TPRREbool  isAnisoFilteringSupported() const;      
    TPRREfloat getMaximumAnisoFiltering() const;       
    TPRREbool  isTextureCompressionSupported() const;   
    TPRREbool  isVTCSupported() const;                  
    TPRREbool  is3DcSupported() const;                  
    TPRREbool  isRGTCSupported() const;                
    TPRREbool  isLATCSupported() const;                
    TPRREbool  isAdvancedPixelFormatSetSupported() const;
    TPRREbool  isFullSceneAntiAliasingSupported() const;  
    TPRREint   getMaxSamplesCount() const;               
    TPRREbool  isSamplesSupported(TPRREint numsamples) const;                   
    TPRREbool  isMultiDrawArraysSupported() const;     
    TPRREbool  isDrawRangeElementsSupported() const;   
    TPRREbool  isCompiledVertexArraySupported() const;  
    TPRREbool  isNVVertexArrayRangeSupported() const;    
    TPRREbool  isNVElementArraySupported() const;       
    TPRREbool  isATIVertexArrayObjectSupported() const; 
    TPRREbool  isATIMapObjectBufferSupported() const;    
    TPRREbool  isATIElementArraySupported() const;      
    TPRREbool  isVertexBufferObjectSupported() const;    
    TPRREbool  isATITruFormSupported() const;           
    const std::string& getVideocardOSName() const;          
    const std::string& getVideocardOGLName() const;        
    const std::string& getVideocardOGLVersion() const;
    const std::string& getVideocardGLSLVersion() const;
    const std::string& getVideocardOGLVendor() const;
    const std::string& getVideocardOGLFeatureList() const;
    const std::string& getVideocardWGLFeatureList() const;

    void WriteStats(); 

protected:
    // ---------------------------------------------------------------------------

private:

    static const GLint   GL_STANDARD_MIN_CLIPPING_PLANES;
    static const GLint   GL_STANDARD_MIN_VIEWPORTS;
    static const GLint   GL_STANDARD_MIN_VIEWPORT_BOUNDS_RANGE[];
    static const GLint   GL_STANDARD_MIN_MATRIX_STACK_DEPTH_PROJECTION;
    static const GLint   GL_STANDARD_MIN_MATRIX_STACK_DEPTH_MODELVIEW;
    static const GLint   GL_STANDARD_MIN_MATRIX_STACK_DEPTH_COLOR;
    static const GLint   GL_STANDARD_MIN_MATRIX_STACK_DEPTH_TEXTURE;
    static const GLint   GL_STANDARD_MIN_STACK_DEPTH_ATTRIB;
    static const GLint   GL_STANDARD_MIN_STACK_DEPTH_CLIENT_ATTRIB;
    static const GLint   GL_STANDARD_MIN_DISPLAY_LIST_NESTING_LEVEL;
    static const GLfloat GL_STANDARD_MIN_TEXTURE_LOD_BIAS;
    static const GLint   GL_STANDARD_MIN_TEXTURE_SIZE;
    static const GLint   GL_STANDARD_MIN_RECTANGLE_TEXTURE_SIZE;
    static const GLint   GL_STANDARD_MIN_3D_TEXTURE_SIZE;
    static const GLint   GL_STANDARD_MIN_CUBE_MAP_TEXTURE_SIZE;
    static const GLint   GL_STANDARD_MIN_COMPRESSED_TEXTURE_FORMATS;

    static const TPRREint MSAA_SUPP_LEVELS_ARRAY_SIZE = 33;  /**< Size of array which holds the support of MSAA-levels. */

    static PRREhwVideoImpl hwVideoInstance;
    static HGLRC hGLRC;                          /**< OpenGL rendering context handle. */
    static HDC   hWndDC;                         /**< Our main window's device context. */
    static TPRREuint nResX, nResY;               /**< Display resolution. */      
    static TPRREint  nColorBits, nDepthBits;     /**< Display color depth, ZBuffer-depth. */
    static TPRREint  nStencilBits, nFSAAlevel;   /**< Stencil Buffer depth, FSAA-level. */
    static std::map<GLint, std::string> vTexComprFmts;  /**< List of compressed texture formats with names. */

    static bool      bAlreadyInitializedOnceOGL; /**< True if OGL-initialization has been done once already.
                                                      This is to speed up consecutive start-stop initializations with same OGL renderer. */
    static TPRREbool bSuppAnisoFiltering;        /**< Is anisotropic filtering available? */
    static GLfloat   nAnisoFilterMax;            /**< Maximum level of anisotropic filtering. */
    static TPRREbool bSuppBGRAtex;               /**< Is uploading textures in BGRA-format available? */
    static TPRREbool bSuppHWmipmapGen;           /**< Is HW MIP map generation available? */
    static TPRREbool bSuppTexCompression;        /**< Is texture compression available? */
    static TPRREbool bSuppVTC;                   /**< Is Volume Texture Compression available? */
    static TPRREbool bSupp3Dc;                   /**< Is 3Dc/ATI2/DXN compression supported? */
    static TPRREbool bSuppRGTC;                  /**< Is Red-Green compression supported? */
    static TPRREbool bSuppLATC;                  /**< Is Luminance-Alpha compression supported? */
    static TPRREbool bSuppMultiTexturing;        /**< Is multitexturing available (fixed func. pipeline)? */
    static TPRREbool bSuppMultiDrawArrays;       /**< Is MultiDrawArrays supported? */
    static TPRREbool bSuppDrawRangeElements;     /**< Is DrawRangeElements supported? */
    static TPRREbool bSuppCompiledVertexArrays;  /**< Is CompiledVertexArrays supported? */
    static TPRREbool bSuppNVVertexArrayRange;    /**< Is the nVidia-specific Vertex Array Range supported? */
    static TPRREbool bSuppNVVertexArrayRange2;   /**< Is the nVidia-specific Vertex Array Range 2 supported? */
    static TPRREbool bSuppNVElementArray;        /**< Is the nVidia-specific Element Array supported? */
    static TPRREbool bSuppATIVertexArrayObject;  /**< Is the ATI-specific Vertex Array Object supported? */
    static TPRREbool bSuppATIMapObjectBuffer;    /**< Is the ATI-specific MapObjectBuffer (for Vertex Array Object) supported? */
    static TPRREbool bSuppATIElementArray;       /**< Is the ATI-specific Element Array supported? */
    static TPRREbool bSuppVertexBuffers;         /**< Is Vertex Buffer Object supported? */
    static TPRREbool bSuppATITruForm;            /**< Is the ATI-specific TruForm supported? */
    static TPRREint  nTMUcount;                  /**< Number of texture units. */
    static TPRREbool bMSAAlevels[MSAA_SUPP_LEVELS_ARRAY_SIZE];  /**< Is the specified MSAA-level available? */
    static TPRREint  nMSAAmaxLevel;              /**< Maximum level of MSAA. */

    static std::string sVidNameWin;              /**< VGA name based on Windows. */
    static std::string sVidNameOGL;              /**< VGA name based on OpenGL. */
    static std::string sVidVendorOGL;            /**< VGA vendor based on OpenGL. */
    static std::string sVidFeaturesOGL;          /**< VGA features based on OGL. */
    static std::string sVidFeaturesWGL;          /**< VGA features based on WGL. */
    static std::string sVidVersionOGL;           /**< OpenGL version. */
    static std::string sVidVersionGLSL;          /**< GLSL version. */
    static TPRREuint   nVidVersionGLSLmaj;       /**< GLSL major version. */
    static TPRREuint   nVidVersionGLSLmin;       /**< GLSL minor version. */

    // ---------------------------------------------------------------------------

    PRRESharedSettings& sharedSettings;  /**< Pointer to shared settings. */

    GLint   tmpRet[6];                   /**< Hold for glGetInteger...() results. */
    GLfloat tmpRetf[2];                  /**< Hold for glGetFloat...() results. */

    DISPLAY_DEVICEA dispdev;             /**< To query the Windows name of the VGA. */

    // ---------------------------------------------------------------------------

    PRREhwVideoImpl();                  /**< Sets members to default values. */

    PRREhwVideoImpl(const PRREhwVideoImpl&);
    PRREhwVideoImpl& operator=(const PRREhwVideoImpl&);

    virtual ~PRREhwVideoImpl();

    /** Preinitializes members. */
    void      PreInitialize(
        HGLRC rc, HDC dc,
        TPRREuint resx, TPRREuint resy,
        TPRREint cbits, TPRREint dbits,
        TPRREint sbits, TPRREint fsaa);
    void      PreInitialize();        /**< Preinitializes members. */
    TPRREbool initializeBase();       /**< Does the real initializations. */
    void      DeinitializeBase();     /**< Does the real deinitialization. */
    TPRREbool isExtensionSupported(
        const char* extname) const;   /**< Is the given extension supported? */

    void printExtensionList(
        const std::string& features,
         const char* endstring);         /**< Writes the given extensions list to console. */           
    void printOGLextensions();           /**< Writes OGL extension list to console. */
    void printWGLextensions();           /**< Writes WGL extension list to console. */

    void CheckAgainstStandardMinValue(
        GLint value, GLint stdValue );      /**< Checks given value against given standard value. */
    void CheckAgainstStandardMinValuef(
        GLfloat value, GLfloat stdValue );  /**< Checks given value against given standard value. */
    void CheckAgainstStandardRange(
        GLint val1, GLint val2,
        GLint stdVal1, GLint stdVal2 );     /**< Checks given range against given standard range. */

    void DiscoverViewportAvailability();
    void DiscoverMatrixStacksAvailability();
    void DiscoverFramebufferAndClipping();
    void DiscoverMultiDrawArraysAvailability();
    void DiscoverDrawRangeElementsAvailability();
    void DiscoverCompiledVertexArraysAvailability();
    void DiscoverNVVertexArrayRangeAvailability();
    void DiscoverNVElementArrayAvailability();
    void DiscoverATIVertexArrayObjectAvailability();
    void DiscoverATIMapObjectBufferAvailability();
    void DiscoverATIElementArrayAvailability();
    void DiscoverVertexBuffersAvailability();
    void DiscoverATITruFormAvailability();
    void DiscoverVertexRenderingAvailability();
    void DiscoverMultiTexturingAvailability();
    void DiscoverAnisoFilteringAvailability();
    void DiscoverTextureCompressionAvailability();
    void DiscoverTexturingAvailability();
    void DiscoverFSAAavailability(HDC dc);
    bool printExtensionSupported(const char* title, const char* name, std::vector<void*>& functions);
    bool discoverOpenGL_1_1_availability();
    bool discoverOpenGL_1_2_availability();
    bool discoverOpenGL_1_3_availability();
    bool discoverOpenGL_1_4_availability();
    bool discoverOpenGL_1_5_availability();
    bool discoverOpenGL_2_0_availability();
    bool discoverOpenGL_2_1_availability();
    bool discoverOpenGL_3_0_availability();
    bool discoverOpenGL_3_1_availability();
    bool discoverOpenGL_3_2_availability();
    bool discoverOpenGL_3_3_availability();
    bool discoverOpenGL_4_0_availability();
    bool discoverOpenGL_4_1_availability();
    bool discoverOpenGL_4_2_availability();
    bool discoverOpenGL_4_3_availability();
    void DiscoverOpenGL_X_Y_availability(const char* text, bool (PRREhwVideoImpl::*f)());
    void DiscoverOpenGLavailability();      

};



// ############################### PUBLIC ################################


/** 
    Gets the singleton instance.
*/
PRREhwVideoImpl& PRREhwVideoImpl::get()
{
    return hwVideoInstance;
} // get()


/**
    Sets members to real values within the singleton instance.
    Does nothing if rc is NULL.
*/
void PRREhwVideoImpl::Initialize(
    HGLRC rc, HDC wnd_dc,
    TPRREuint nResX, TPRREuint nResY,
    TPRREint nColorBits, TPRREint nDepthBits,
    TPRREint nStencilBits, TPRREint nFSAAlevel )
{
    getConsole().OIOLn("PRREhwVideo::Initialize() ...");
    getConsole().OI();

    if ( isInitialized() )
    {
        getConsole().OLnOO("Already initialized!");
        getConsole().OO();
        return;
    }

    if ( rc == NULL )
    {
        getConsole().OLn("Warning: rc is NULL!");
    }

    PreInitialize(rc, wnd_dc, nResX, nResY, nColorBits, nDepthBits, nStencilBits, nFSAAlevel);
    PRREhwBase::Initialize();  // here we trigger initializeBase()
    
} // Initialize(...)


/**
    Gets whether video accelerator is detected or not.
    @return True if non-software renderer is initialized, false otherwise.
*/
TPRREbool PRREhwVideoImpl::isAcceleratorDetected() const
{
    return isInitialized() && ( hGLRC != NULL );
}


/**
    Gets the amount of used VRAM.
    @return The amount of used VRAM in Bytes.
*/
TPRREuint PRREhwVideoImpl::getUsedVideoMemory() const
{
    return ( getUsedTextureMemory() + getUsedFrameBufferMemory() );
} // getUsedVideoMemory()


/**
    Gets the amount of used texture memory.
    @return The amount of used texture memory in Bytes.
*/
TPRREuint PRREhwVideoImpl::getUsedTextureMemory() const
{
    // todo: go thru all textures in all texturemgrs and sum texture memory usage
    return 0;
} // getUsedTextureMemory()


/**
    Gets the number of pixels in the Frame Buffer (width x height).
    @return The number of pixels in the Frame Buffer (width x height).
*/
TPRREuint PRREhwVideoImpl::getColorBufferPixelCount() const
{
    return ( nResX * nResY );
} // getColorBufferPixelCount()


/** 
    Gets the amount of VRAM used by the Color Buffer in Bytes.
    @return The amount of VRAM used by the Color Buffer in Bytes.
*/
TPRREuint PRREhwVideoImpl::getColorBufferSize() const
{
    return ( getColorBufferPixelCount()*2 * nColorBits/8 );
} // getColorBufferSize()


/** 
    Gets the amount of VRAM used by the Depth Buffer in Bytes.
    @return The amount of VRAM used by the Depth Buffer in Bytes.
*/
TPRREuint PRREhwVideoImpl::getDepthBufferSize() const
{
    return ( nDepthBits/8 * getColorBufferPixelCount() );
} // getDepthBufferSize()


/**
    Gets the amount of VRAM used by the Stencil Buffer in Bytes.
    @return The amount of VRAM used by the Stencil Buffer in Bytes.
*/
TPRREuint PRREhwVideoImpl::getStencilBufferSize() const
{
    return ( nStencilBits/8 * getColorBufferPixelCount() );
} // getStencilBufferSize()


/**
    Gets the amount of VRAM used by the Sample Buffer in Bytes.
    @return The amount of VRAM used by the Sample Buffer in Bytes.
*/
TPRREuint PRREhwVideoImpl::getSampleBufferSize() const
{
    return ( nFSAAlevel * (getColorBufferSize()/2 + getDepthBufferSize() + getStencilBufferSize()) );
} // getSampleBufferSize()


/**
    Gets the amount of VRAM used by the Frame Buffer in Bytes.
    @return The amount of VRAM used by the Frame Buffer in Bytes.
*/
TPRREuint PRREhwVideoImpl::getUsedFrameBufferMemory() const
{
    return ( getColorBufferSize() + getDepthBufferSize() + getStencilBufferSize() + getSampleBufferSize() );
} // getUsedFrameBufferMemory()
 

/**
    Gets whether VSync is supported or not.
    @return True if VSync is supported, false otherwise.
*/
TPRREbool PRREhwVideoImpl::isVSyncSupported() const
{
    return sharedSettings.get(PRRE_SSET_VSYNC);
} // isVSyncSupported()


/**
    Gets whether multitexturing is supported or not.
    @return True if multitexturing is supported, false otherwise.
*/
TPRREbool PRREhwVideoImpl::isMultiTexturingSupported() const
{
    return isInitialized() && bSuppMultiTexturing;
} // isMultiTexturingSupported()


/**
    Gets the number of texture units.
    @return The number of texture units.
*/
TPRREint PRREhwVideoImpl::getTextureUnitsCount() const
{
    return isInitialized() ? nTMUcount : 0;
} // getTextureUnitsCount()


/**
    Gets whether HW MIP map generation is supported or not.
    @return True if HW MIP map generation is supported, otherwise false.
*/
TPRREbool PRREhwVideoImpl::isHardwareMipMapGenerationSupported() const
{
    return isInitialized() && bSuppHWmipmapGen;
} // isHardwareMipMapGenerationSupported()


/**
    Gets whether uploading of native DIBs (BGRA) is supported or not.
    @return True if uploading of native DIBs (BGRA) is supported, otherwise false.
*/
TPRREbool PRREhwVideoImpl::isNativeDIBFormatSupported() const
{
    return isInitialized() && bSuppBGRAtex;
} // isNativeDIBFormatSupported()


/**
    Gets whether anisotropic filtering is supported or not.
    @return True if anisotropic filtering is supported, false otherwise.
*/
TPRREbool PRREhwVideoImpl::isAnisoFilteringSupported() const
{
    return isInitialized() && bSuppAnisoFiltering;
} // isAnisoFilteringSupported()


/**
    Gets the maximum available level of anisotropy.
    @return The maximum available level of anisotropy.
*/
TPRREfloat PRREhwVideoImpl::getMaximumAnisoFiltering() const
{
    return isInitialized() ? (TPRREfloat) nAnisoFilterMax : (TPRREfloat) 0.0f;
} // getMaximumAnisoFiltering()


/**
    Gets whether texture compression is supported or not.
    @return True if texture compression is supported, otherwise false.
*/
TPRREbool PRREhwVideoImpl::isTextureCompressionSupported() const
{
    return isInitialized() && bSuppTexCompression;
} // isTextureCompressionSupported()


/**
    Gets whether Volume Texture Compression is supported or not.
    @return True if Volume Texture Compression is supported, otherwise false.
*/
TPRREbool PRREhwVideoImpl::isVTCSupported() const
{
    return isInitialized() && bSuppVTC;
} // isVTCSupported()


/**
    Gets whether 3Dc/ATI2/DXN compression is supported or not.
    @return True if 3Dc/ATI2/DXN compression is supported, otherwise false.
*/
TPRREbool PRREhwVideoImpl::is3DcSupported() const
{
    return isInitialized() && bSupp3Dc;
} // is3DcSupported()


/**
    Gets whether Red-Green compression is supported or not.
    @return True if Red-Green compression is supported, otherwise false.
*/
TPRREbool PRREhwVideoImpl::isRGTCSupported() const
{
    return isInitialized() && bSuppRGTC;
} // isRGTCSupported()


/**
    Gets whether Luminance-Alpha compression is supported or not.
    @return True if Luminance-Alpha compression is supported, otherwise false.
*/
TPRREbool PRREhwVideoImpl::isLATCSupported() const
{
    return isInitialized() && bSuppLATC;
} // isLATCsupported()


/**
    Gets whether advanced pixel formats supported or not.
    Advanced pixel formats are required for various extra features like MSAA.
*/
TPRREbool PRREhwVideoImpl::isAdvancedPixelFormatSetSupported() const
{
    return isInitialized() && isExtensionSupported("WGL_ARB_pixel_format");
} // isAdvancedPixelFormatSetSupported()


/** 
    Gets whether FSAA is supported or not.
    @return True if FSAA is supported, otherwise false.
*/
TPRREbool PRREhwVideoImpl::isFullSceneAntiAliasingSupported() const
{
    return isInitialized() && ( nMSAAmaxLevel > 1 );
} // isFullSceneAntiAliasingSupported()


/**
    Gets the maximum FSAA level.
    @return The maximum available FSAA-level.
*/
TPRREint PRREhwVideoImpl::getMaxSamplesCount() const
{
    return isInitialized() ? nMSAAmaxLevel : 0;
} // getMaxSamplesCount()


/**
    Gets whether the given MSAA-level is supported or not.
    @return True if the given MSAA-level is supported, otherwise false.
*/
TPRREbool PRREhwVideoImpl::isSamplesSupported(TPRREint numsamples) const
{
    return isInitialized() && (numsamples > 0) && ((numsamples < MSAA_SUPP_LEVELS_ARRAY_SIZE) ? bMSAAlevels[numsamples] : false);
} // isSamplesSupported()


/**
    Gets whether MultiDrawArrays is supported or not.
*/
TPRREbool PRREhwVideoImpl::isMultiDrawArraysSupported() const
{
    return isInitialized() && bSuppMultiDrawArrays;
} // isMultiDrawArraysSupported()


/**
    Gets whether DrawRangeElements is supported or not.
*/
TPRREbool PRREhwVideoImpl::isDrawRangeElementsSupported() const
{
    return isInitialized() && bSuppDrawRangeElements;
} // isDrawRangeElementsSupported()


/**
    Gets whether Compiled Vertex Array is supported or not.
*/
TPRREbool PRREhwVideoImpl::isCompiledVertexArraySupported() const 
{
    return isInitialized() && bSuppCompiledVertexArrays;
} // isCompiledVertexArraySupported()


/**
    Gets whether the nVidia-specific Vertex Array Range is supported.
*/
TPRREbool PRREhwVideoImpl::isNVVertexArrayRangeSupported() const
{
    return isInitialized() && bSuppNVVertexArrayRange;
} // isNVVertexArrayRangeSupported() 


/**
    Gets whether the nVidia-specific Element Array is supported.
*/
TPRREbool PRREhwVideoImpl::isNVElementArraySupported() const
{
    return isInitialized() && bSuppNVElementArray;
} // isNVElementArraySupported()


/**
    Gets whether the ATI-specific Vertex Array Object is supported.
*/
TPRREbool PRREhwVideoImpl::isATIVertexArrayObjectSupported() const
{
    return isInitialized() && bSuppATIVertexArrayObject;
} // isATIVertexArrayObjectSupported()  


/**
    Gets whether the ATI-specific MapObjectBuffer (for Vertex Array Object) is supported.
*/
TPRREbool PRREhwVideoImpl::isATIMapObjectBufferSupported() const
{
    return isInitialized() && bSuppATIMapObjectBuffer;
} // isATIMapObjectBufferSupported()


/**
    Gets whether the ATI-specific Element Array is supported.
*/
TPRREbool PRREhwVideoImpl::isATIElementArraySupported() const
{
    return isInitialized() && bSuppATIElementArray;
} // isATIElementArraySupported()


/**
    Gets whether Vertex Buffer Object is supported or not.
*/
TPRREbool PRREhwVideoImpl::isVertexBufferObjectSupported() const
{
    return isInitialized() && bSuppVertexBuffers;
} // isVertexBufferObjectSupported()


/**
    Gets whether the ATI-specific TruForm is supported.
*/
TPRREbool PRREhwVideoImpl::isATITruFormSupported() const
{
    return isInitialized() && bSuppATITruForm;
} // isATITruFormSupported()


/**
    Gets the Windows-based VGA name.
    @return The Windows-based VGA name.
*/
const string& PRREhwVideoImpl::getVideocardOSName() const 
{
    static const std::string sEmpty = "";
    return isInitialized() ? sVidNameWin : sEmpty;
} // getVideocardOSName()


/**
    Gets the OpenGL-based VGA name.
    @return The OpenGL-based VGA name.
*/
const string& PRREhwVideoImpl::getVideocardOGLName() const 
{
    static const std::string sEmpty = "";
    return isInitialized() ? sVidNameOGL : sEmpty;
} // getVideocardOGLName()


/**
    Gets the OpenGL version supported by the driver.
    @return The OpenGL version.
*/
const string& PRREhwVideoImpl::getVideocardOGLVersion() const 
{
    static const std::string sEmpty = "";
    return isInitialized() ? sVidVersionOGL : sEmpty;
} // getVideocardOGLVersion()


/**
    Gets the GLSL version supported by the driver.
*/
const string& PRREhwVideoImpl::getVideocardGLSLVersion() const
{
    static const std::string sEmpty = "";
    return isInitialized() ? sVidVersionGLSL : sEmpty;
} // getVideocardGLSLVersion()


/**
    Gets the OpenGL-based VGA vendor.
    @return The VGA vendor based on OpenGL.
*/
const string& PRREhwVideoImpl::getVideocardOGLVendor() const 
{
    static const std::string sEmpty = "";
    return isInitialized() ? sVidVendorOGL : sEmpty;
} // getVideocardOGLVendor()


/**
    Gets the OpenGL features supported by the driver.
    @return The OpenGL features supported by the driver.
*/
const string& PRREhwVideoImpl::getVideocardOGLFeatureList() const 
{
    static const std::string sEmpty = "";
    return isInitialized() ?sVidFeaturesOGL : sEmpty;
} // getVideocardOGLFeatureList()


/**
    Gets the WGL features supported by the driver.
    @return The WGL features supported by the driver.
*/
const string& PRREhwVideoImpl::getVideocardWGLFeatureList() const 
{
    static const std::string sEmpty = "";
    return isInitialized() ?sVidFeaturesWGL : sEmpty;
} // getVideocardWGLFeatureList()


/**
    Writes statistics to the console.
*/
void PRREhwVideoImpl::WriteStats()
{
    getConsole().OLn("PRREhwVideo::WriteStats()");
    getConsole().L();
    getConsole().OLn("Estimated amount of VRAM in use:");
    getConsole().OLn("  Frame buffer ~ %f kB == %f MB", this->getUsedFrameBufferMemory()/1024.0f, this->getUsedFrameBufferMemory()/1024.0f/1024.0f);
    getConsole().OLn("   - Back buffer + Front buffer ~ %f kB == %f MB", this->getColorBufferSize()/1024.0f, this->getColorBufferSize()/1024.0f/1024.0f);
    getConsole().OLn("   - Depth Buffer ~ %f kB == %f MB", this->getDepthBufferSize()/1024.0f, this->getDepthBufferSize()/1024.0f/1024.0f);
    getConsole().OLn("   - Stencil Buffer ~ %f kB == %f MB", this->getStencilBufferSize()/1024.0f, this->getStencilBufferSize()/1024.0f/1024.0f);
    getConsole().OLn("  Texture buffer ~ %f kB == %f MB", this->getUsedTextureMemory()/1024.0f, this->getUsedTextureMemory()/1024.0f/1024.0f);
    getConsole().OLn("-------------------------");
    getConsole().OLn("Total: ~ %f kB == %f MB", this->getUsedVideoMemory()/1024.0f, this->getUsedVideoMemory()/1024.0f/1024.0f);
    getConsole().L();
} // WriteStats()


// ############################## PROTECTED ##############################


/**
    Preinitializes members.
*/
void PRREhwVideoImpl::PreInitialize(HGLRC rc, HDC dc, TPRREuint resx, TPRREuint resy, TPRREint cbits, TPRREint dbits, TPRREint sbits, TPRREint fsaa)
{
    hGLRC = rc;
    hWndDC = dc;
    nResX = resx;
    nResY = resy;
    nColorBits = cbits;
    nDepthBits = dbits;
    nStencilBits = sbits;
    nFSAAlevel   = fsaa;

    memset(&dispdev, 0, sizeof(dispdev));
    dispdev.cb = sizeof(dispdev);

} // PreInitialize()


/**
    Preinitializes members.
*/
void PRREhwVideoImpl::PreInitialize()
{
    PreInitialize(hGLRC, hWndDC, nResX, nResY, nColorBits, nDepthBits, nStencilBits, nFSAAlevel);
    sharedSettings.Set(PRRE_SSET_VSYNC, false);
} // deinitializeBase()


/**
    Does the real initialization.
*/
TPRREbool PRREhwVideoImpl::initializeBase()
{
    EnumDisplayDevicesA(NULL, 0, &dispdev, NULL);
    sVidNameWin = dispdev.DeviceString;

    if ( hGLRC == NULL )
    {
        // app may be using software renderer
        getConsole().OLn("GFX card: %s", sVidNameWin.c_str());
        sharedSettings.Set(PRRE_SSET_VSYNC, false);
        getConsole().OO();
        getConsole().OO();

        return true;
    }

    if ( vTexComprFmts.empty () )
    {
        vTexComprFmts[ (GLint) GL_COMPRESSED_RGB                              ] = "RGB";
        vTexComprFmts[ (GLint) GL_COMPRESSED_RGBA                             ] = "RGBA";
        vTexComprFmts[ (GLint) GL_COMPRESSED_ALPHA                            ] = "ALPHA";
        vTexComprFmts[ (GLint) GL_COMPRESSED_LUMINANCE                        ] = "LUMINANCE";
        vTexComprFmts[ (GLint) GL_COMPRESSED_LUMINANCE_ALPHA                  ] = "LUMINANCE_ALPHA";
        vTexComprFmts[ (GLint) GL_COMPRESSED_INTENSITY                        ] = "INTENSITY";
        vTexComprFmts[ (GLint) GL_COMPRESSED_SRGB                             ] = "SRGB";
        vTexComprFmts[ (GLint) GL_COMPRESSED_SRGB_ALPHA                       ] = "SRGB_ALPHA";
        vTexComprFmts[ (GLint) GL_COMPRESSED_SLUMINANCE                       ] = "SLUMINANCE";
        vTexComprFmts[ (GLint) GL_COMPRESSED_SLUMINANCE_ALPHA                 ] = "SLUMINANCE_ALPHA";
        vTexComprFmts[ (GLint) GL_COMPRESSED_RGB_S3TC_DXT1_EXT                ] = "RGB_S3TC_DXT1_EXT";
        vTexComprFmts[ (GLint) GL_COMPRESSED_RGBA_S3TC_DXT1_EXT               ] = "RGBA_S3TC_DXT1_EXT";
        vTexComprFmts[ (GLint) GL_COMPRESSED_RGBA_S3TC_DXT3_EXT               ] = "RGBA_S3TC_DXT3_EXT";
        vTexComprFmts[ (GLint) GL_COMPRESSED_RGBA_S3TC_DXT5_EXT               ] = "RGBA_S3TC_DXT5_EXT";
        vTexComprFmts[ (GLint) GL_COMPRESSED_RGB_FXT1_3DFX                    ] = "RGB_FXT1_3DFX";
        vTexComprFmts[ (GLint) GL_COMPRESSED_RGBA_FXT1_3DFX                   ] = "RGBA_FXT1_3DFX";
        vTexComprFmts[ (GLint) GL_COMPRESSED_SRGB_EXT                         ] = "SRGB_EXT";
        vTexComprFmts[ (GLint) GL_COMPRESSED_SRGB_S3TC_DXT1_EXT               ] = "SRGB_S3TC_DXT1_EXT";
        vTexComprFmts[ (GLint) GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT         ] = "SRGB_ALPHA_S3TC_DXT1_EXT";
        vTexComprFmts[ (GLint) GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT         ] = "SRGB_ALPHA_S3TC_DXT3_EXT";
        vTexComprFmts[ (GLint) GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT         ] = "SRGB_ALPHA_S3TC_DXT5_EXT";
        vTexComprFmts[ (GLint) GL_COMPRESSED_LUMINANCE_LATC1_EXT              ] = "LUMINANCE_LATC1_EXT";
        vTexComprFmts[ (GLint) GL_COMPRESSED_SIGNED_LUMINANCE_LATC1_EXT       ] = "SIGNED_LUMINANCE_LATC1_EXT";
        vTexComprFmts[ (GLint) GL_COMPRESSED_LUMINANCE_ALPHA_LATC2_EXT        ] = "LUMINANCE_ALPHA_LATC2_EXT";
        vTexComprFmts[ (GLint) GL_COMPRESSED_SIGNED_LUMINANCE_ALPHA_LATC2_EXT ] = "SIGNED_LUMINANCE_ALPHA_LATC2_EXT";
        vTexComprFmts[ (GLint) GL_COMPRESSED_RED_GREEN_RGTC2_EXT              ] = "RED_GREEN_RGTC2_EXT";
        vTexComprFmts[ (GLint) GL_COMPRESSED_SIGNED_RED_GREEN_RGTC2_EXT       ] = "SIGNED_RED_GREEN_RGTC2_EXT";
        vTexComprFmts[ (GLint) GL_COMPRESSED_RED                              ] = "RED";
        vTexComprFmts[ (GLint) GL_COMPRESSED_RG                               ] = "RG";
        vTexComprFmts[ (GLint) GL_COMPRESSED_RED_RGTC1                        ] = "RED_RGTC1";
        vTexComprFmts[ (GLint) GL_COMPRESSED_SIGNED_RED_RGTC1                 ] = "SIGNED_RED_RGTC1";
        vTexComprFmts[ (GLint) GL_COMPRESSED_RG_RGTC2                         ] = "RG_RGTC2";
        vTexComprFmts[ (GLint) GL_COMPRESSED_SIGNED_RG_RGTC2                  ] = "SIGNED_RG_RGTC2";
        vTexComprFmts[ (GLint) GL_COMPRESSED_RGBA_BPTC_UNORM_ARB              ] = "RGBA_BPTC_UNORM_ARB";
        vTexComprFmts[ (GLint) GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM_ARB        ] = "SRGB_ALPHA_BPTC_UNORM_ARB";
        vTexComprFmts[ (GLint) GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB        ] = "RGB_BPTC_SIGNED_FLOAT_ARB";
        vTexComprFmts[ (GLint) GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB      ] = "RGB_BPTC_UNSIGNED_FLOAT_ARB";
        vTexComprFmts[ (GLint) GL_COMPRESSED_RGBA_ASTC_4x4_KHR                ] = "RGBA_ASTC_4x4_KHR";
        vTexComprFmts[ (GLint) GL_COMPRESSED_RGBA_ASTC_5x4_KHR                ] = "RGBA_ASTC_5x4_KHR";
        vTexComprFmts[ (GLint) GL_COMPRESSED_RGBA_ASTC_5x5_KHR                ] = "RGBA_ASTC_5x5_KHR";
        vTexComprFmts[ (GLint) GL_COMPRESSED_RGBA_ASTC_6x5_KHR                ] = "RGBA_ASTC_6x5_KHR";
        vTexComprFmts[ (GLint) GL_COMPRESSED_RGBA_ASTC_6x6_KHR                ] = "RGBA_ASTC_6x6_KHR";
        vTexComprFmts[ (GLint) GL_COMPRESSED_RGBA_ASTC_8x5_KHR                ] = "RGBA_ASTC_8x5_KHR";
        vTexComprFmts[ (GLint) GL_COMPRESSED_RGBA_ASTC_8x6_KHR                ] = "RGBA_ASTC_8x6_KHR";
        vTexComprFmts[ (GLint) GL_COMPRESSED_RGBA_ASTC_8x8_KHR                ] = "RGBA_ASTC_8x8_KHR";
        vTexComprFmts[ (GLint) GL_COMPRESSED_RGBA_ASTC_10x5_KHR               ] = "RGBA_ASTC_10x5_KHR";
        vTexComprFmts[ (GLint) GL_COMPRESSED_RGBA_ASTC_10x6_KHR               ] = "RGBA_ASTC_10x6_KHR";
        vTexComprFmts[ (GLint) GL_COMPRESSED_RGBA_ASTC_10x8_KHR               ] = "RGBA_ASTC_10x8_KHR";
        vTexComprFmts[ (GLint) GL_COMPRESSED_RGBA_ASTC_10x10_KHR              ] = "RGBA_ASTC_10x10_KHR";
        vTexComprFmts[ (GLint) GL_COMPRESSED_RGBA_ASTC_12x10_KHR              ] = "RGBA_ASTC_12x10_KHR";
        vTexComprFmts[ (GLint) GL_COMPRESSED_RGBA_ASTC_12x12_KHR              ] = "RGBA_ASTC_12x12_KHR";
        vTexComprFmts[ (GLint) GL_COMPRESSED_SRGB8_ALPHA8_ASTC_4x4_KHR        ] = "SRGB8_ALPHA8_ASTC_4x4_KHR";
        vTexComprFmts[ (GLint) GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x4_KHR        ] = "SRGB8_ALPHA8_ASTC_5x4_KHR";
        vTexComprFmts[ (GLint) GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x5_KHR        ] = "SRGB8_ALPHA8_ASTC_5x5_KHR";
        vTexComprFmts[ (GLint) GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x5_KHR        ] = "SRGB8_ALPHA8_ASTC_6x5_KHR";
        vTexComprFmts[ (GLint) GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x6_KHR        ] = "SRGB8_ALPHA8_ASTC_6x6_KHR";
        vTexComprFmts[ (GLint) GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x5_KHR        ] = "SRGB8_ALPHA8_ASTC_8x5_KHR";
        vTexComprFmts[ (GLint) GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x6_KHR        ] = "SRGB8_ALPHA8_ASTC_8x6_KHR";
        vTexComprFmts[ (GLint) GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x8_KHR        ] = "SRGB8_ALPHA8_ASTC_8x8_KHR";
        vTexComprFmts[ (GLint) GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x5_KHR       ] = "SRGB8_ALPHA8_ASTC_10x5_KHR";
        vTexComprFmts[ (GLint) GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x6_KHR       ] = "SRGB8_ALPHA8_ASTC_10x6_KHR";
        vTexComprFmts[ (GLint) GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x8_KHR       ] = "SRGB8_ALPHA8_ASTC_10x8_KHR";
        vTexComprFmts[ (GLint) GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x10_KHR      ] = "SRGB8_ALPHA8_ASTC_10x10_KHR";
        vTexComprFmts[ (GLint) GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x10_KHR      ] = "SRGB8_ALPHA8_ASTC_12x10_KHR";
        vTexComprFmts[ (GLint) GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x12_KHR      ] = "SRGB8_ALPHA8_ASTC_12x12_KHR";
        vTexComprFmts[ (GLint) GL_COMPRESSED_RGB8_ETC2                        ] = "RGB8_ETC2";
        vTexComprFmts[ (GLint) GL_COMPRESSED_SRGB8_ETC2                       ] = "SRGB8_ETC2";
        vTexComprFmts[ (GLint) GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2    ] = "RGB8_PUNCHTHROUGH_ALPHA1_ETC2";
        vTexComprFmts[ (GLint) GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2   ] = "SRGB8_PUNCHTHROUGH_ALPHA1_ETC2";
        vTexComprFmts[ (GLint) GL_COMPRESSED_RGBA8_ETC2_EAC                   ] = "RGBA8_ETC2_EAC";
        vTexComprFmts[ (GLint) GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC            ] = "SRGB8_ALPHA8_ETC2_EAC";
        vTexComprFmts[ (GLint) GL_COMPRESSED_R11_EAC                          ] = "R11_EAC";
        vTexComprFmts[ (GLint) GL_COMPRESSED_SIGNED_R11_EAC                   ] = "SIGNED_R11_EAC";
        vTexComprFmts[ (GLint) GL_COMPRESSED_RG11_EAC                         ] = "RG11_EAC";
        vTexComprFmts[ (GLint) GL_COMPRESSED_SIGNED_RG11_EAC                  ] = "SIGNED_RG11_EAC";
        vTexComprFmts[ (GLint) GL_PALETTE4_RGB8_OES                           ] = "PALETTE4_RGB8_OES";
        vTexComprFmts[ (GLint) GL_PALETTE4_RGBA8_OES                          ] = "PALETTE4_RGBA8_OES";
        vTexComprFmts[ (GLint) GL_PALETTE4_R5_G6_B5_OES                       ] = "PALETTE4_R5_G6_B5_OES";
        vTexComprFmts[ (GLint) GL_PALETTE4_RGBA4_OES                          ] = "PALETTE4_RGBA4_OES";
        vTexComprFmts[ (GLint) GL_PALETTE4_RGB5_A1_OES                        ] = "PALETTE4_RGB5_A1_OES";
        vTexComprFmts[ (GLint) GL_PALETTE8_RGB8_OES                           ] = "PALETTE8_RGB8_OES";
        vTexComprFmts[ (GLint) GL_PALETTE8_RGBA8_OES                          ] = "PALETTE8_RGBA8_OES";
        vTexComprFmts[ (GLint) GL_PALETTE8_R5_G6_B5_OES                       ] = "PALETTE8_R5_G6_B5_OESXXX";
        vTexComprFmts[ (GLint) GL_PALETTE8_RGBA4_OES                          ] = "PALETTE8_RGBA4_OES";
        vTexComprFmts[ (GLint) GL_PALETTE8_RGB5_A1_OES                        ] = "PALETTE8_RGB5_A1_OES";
    }

    if ( !bAlreadyInitializedOnceOGL )
    {
        getConsole().O("Getting non-1.1 func ptrs ... ");
        PRREGLgetFunctionPointers();
        getConsole().OLn("done!");

        getConsole().OLn("");
        getConsole().OLn("GFX card: %s", sVidNameWin.c_str());
    
        const GLubyte* ptmp = glGetString(GL_RENDERER);
        if ( ptmp )
        {
            sVidNameOGL = (char*) ptmp;
            getConsole().OLn("glRenderer = %s", sVidNameOGL.c_str());
        }
        else
        {
            getConsole().EOLn("ERROR: failed to query GL_RENDERER!");
            return false;
        }

        if ( ptmp = glGetString(GL_VENDOR) )
        {
            sVidVendorOGL = (char*) ptmp;
            getConsole().OLn("glVendor = %s", sVidVendorOGL.c_str());
        }
        else
        {
            getConsole().EOLn("ERROR: failed to query GL_VENDOR!");
            return false;
        }

        if ( ptmp = glGetString(GL_VERSION) )
        {
            sVidVersionOGL = (char*) ptmp;
            getConsole().OLn("glVersion = %s", sVidVersionOGL.c_str());
        }
        else
        {
            getConsole().EOLn("ERROR: failed to query GL_VERSION!");
            return false;
        }

        if ( ptmp = glGetString(GL_SHADING_LANGUAGE_VERSION) )
        {
            sVidVersionGLSL = (char*) ptmp;
            sscanf((const char*) ptmp, "%d.%d", &nVidVersionGLSLmaj, &nVidVersionGLSLmin);
            getConsole().OLn("glslVersion = %s (%d.%d)", sVidVersionGLSL.c_str(), nVidVersionGLSLmaj, nVidVersionGLSLmin);
        }
        else
        {
            getConsole().EOLn("WARNING: failed to query GL_SHADING_LANGUAGE_VERSION!");
        }

        // OGL error may be generated due to querying GL_SHADING_LANGUAGE_VERSION which may not be supported by all implementations
        PRREGLsnippets::ClearGLerror();
        
        if ( ptmp = glGetString(GL_EXTENSIONS) )
        {
            sVidFeaturesOGL = (char*) ptmp;
        }
        else
        {
            getConsole().EOLn("ERROR: failed to query GL_EXTENSIONS!");
            return false;
        }    
        
        if ( (wglGetExtensionsStringARB != NULL) && (ptmp = (const GLubyte*) wglGetExtensionsStringARB( wglGetCurrentDC() )) )
            sVidFeaturesWGL = (char*) ptmp;
        else
            getConsole().EOLn("WARNING: failed to query WGL_EXTENSIONS!");

        // OGL error may be generated due to querying WGL_EXTENSIONS which may not be supported by all implementations
        PRREGLsnippets::ClearGLerror();

        getConsole().OLn("Extensions:");
        getConsole().OLn("");
        printOGLextensions();
        printWGLextensions();
    } // !bAlreadyInitializedOnceOGL

    sharedSettings.Set(
        PRRE_SSET_VSYNC,
        isExtensionSupported("WGL_EXT_swap_control") && (wglSwapIntervalEXT != PGENULL) );

    if ( !bAlreadyInitializedOnceOGL )
        DiscoverOpenGLavailability();

    bAlreadyInitializedOnceOGL = true;

    getConsole().OOSOLn("done!");
    getConsole().OO();

    return true;
} // initializeBase()


/**
    Does the real deinitialization.
*/
void PRREhwVideoImpl::DeinitializeBase()
{
    hGLRC = NULL;
    hWndDC = NULL;
    nResX = 0;
    nResY = 0;
    nColorBits = 0;
    nDepthBits = 0;
    nStencilBits = 0;
    nFSAAlevel   = 0;
    vTexComprFmts.clear();
} // deinitializeBase()


/**
    Is the given extension supported?
    @return True if the given extension is supported (exported by the driver), false otherwise.
*/
TPRREbool PRREhwVideoImpl::isExtensionSupported(const char* extname) const
{
    return ( (strstr(sVidFeaturesOGL.c_str(),extname) != NULL) || (strstr(sVidFeaturesWGL.c_str(),extname) != NULL) );
} // isExtensionSupported()


// ############################### PRIVATE ###############################


PRREhwVideoImpl PRREhwVideoImpl::hwVideoInstance;


const GLint   PRREhwVideoImpl::GL_STANDARD_MIN_CLIPPING_PLANES = 6;
const GLint   PRREhwVideoImpl::GL_STANDARD_MIN_VIEWPORTS = 16;
const GLint   PRREhwVideoImpl::GL_STANDARD_MIN_VIEWPORT_BOUNDS_RANGE[] = {-32768, 32767};
const GLint   PRREhwVideoImpl::GL_STANDARD_MIN_MATRIX_STACK_DEPTH_PROJECTION = 2;
const GLint   PRREhwVideoImpl::GL_STANDARD_MIN_MATRIX_STACK_DEPTH_MODELVIEW = 32;
const GLint   PRREhwVideoImpl::GL_STANDARD_MIN_MATRIX_STACK_DEPTH_COLOR = 2;
const GLint   PRREhwVideoImpl::GL_STANDARD_MIN_MATRIX_STACK_DEPTH_TEXTURE = 2;
const GLint   PRREhwVideoImpl::GL_STANDARD_MIN_STACK_DEPTH_ATTRIB = 16;
const GLint   PRREhwVideoImpl::GL_STANDARD_MIN_STACK_DEPTH_CLIENT_ATTRIB = 16;
const GLint   PRREhwVideoImpl::GL_STANDARD_MIN_DISPLAY_LIST_NESTING_LEVEL = 64;
const GLfloat PRREhwVideoImpl::GL_STANDARD_MIN_TEXTURE_LOD_BIAS = 4.0f;
const GLint   PRREhwVideoImpl::GL_STANDARD_MIN_TEXTURE_SIZE = 64;
const GLint   PRREhwVideoImpl::GL_STANDARD_MIN_RECTANGLE_TEXTURE_SIZE = 1024;
const GLint   PRREhwVideoImpl::GL_STANDARD_MIN_3D_TEXTURE_SIZE = 64;
const GLint   PRREhwVideoImpl::GL_STANDARD_MIN_CUBE_MAP_TEXTURE_SIZE = 1024;
const GLint   PRREhwVideoImpl::GL_STANDARD_MIN_COMPRESSED_TEXTURE_FORMATS = 4;

HGLRC     PRREhwVideoImpl::hGLRC = NULL;
HDC       PRREhwVideoImpl::hWndDC = NULL;
TPRREuint PRREhwVideoImpl::nResX = 0;
TPRREuint PRREhwVideoImpl::nResY = 0;
TPRREint  PRREhwVideoImpl::nColorBits = 0;
TPRREint  PRREhwVideoImpl::nDepthBits = 0;
TPRREint  PRREhwVideoImpl::nStencilBits = 0;
TPRREint  PRREhwVideoImpl::nFSAAlevel = 0;

bool      PRREhwVideoImpl::bAlreadyInitializedOnceOGL = false; 
bool      PRREhwVideoImpl::bSuppAnisoFiltering       = false;
GLfloat   PRREhwVideoImpl::nAnisoFilterMax           = 0;
bool      PRREhwVideoImpl::bSuppBGRAtex              = false;
bool      PRREhwVideoImpl::bSuppHWmipmapGen          = false;
bool      PRREhwVideoImpl::bSuppTexCompression       = false;
bool      PRREhwVideoImpl::bSuppVTC                  = false;
bool      PRREhwVideoImpl::bSupp3Dc                  = false;
bool      PRREhwVideoImpl::bSuppRGTC                 = false;
bool      PRREhwVideoImpl::bSuppLATC                 = false;
bool      PRREhwVideoImpl::bSuppMultiTexturing       = false;
bool      PRREhwVideoImpl::bSuppMultiDrawArrays      = false;
bool      PRREhwVideoImpl::bSuppDrawRangeElements    = false;
bool      PRREhwVideoImpl::bSuppCompiledVertexArrays = false;
bool      PRREhwVideoImpl::bSuppNVVertexArrayRange   = false;
bool      PRREhwVideoImpl::bSuppNVVertexArrayRange2  = false;
bool      PRREhwVideoImpl::bSuppNVElementArray       = false;
bool      PRREhwVideoImpl::bSuppATIVertexArrayObject = false;
bool      PRREhwVideoImpl::bSuppATIMapObjectBuffer   = false;
bool      PRREhwVideoImpl::bSuppATIElementArray      = false;
bool      PRREhwVideoImpl::bSuppVertexBuffers        = false;
bool      PRREhwVideoImpl::bSuppATITruForm           = false;
TPRREint  PRREhwVideoImpl::nTMUcount                 = 0;
TPRREint  PRREhwVideoImpl::nMSAAmaxLevel             = 0;
bool      PRREhwVideoImpl::bMSAAlevels[MSAA_SUPP_LEVELS_ARRAY_SIZE] = { false };

std::string     PRREhwVideoImpl::sVidNameWin;       
std::string     PRREhwVideoImpl::sVidNameOGL;       
std::string     PRREhwVideoImpl::sVidVendorOGL;     
std::string     PRREhwVideoImpl::sVidFeaturesOGL;   
std::string     PRREhwVideoImpl::sVidFeaturesWGL;   
std::string     PRREhwVideoImpl::sVidVersionOGL;    
std::string     PRREhwVideoImpl::sVidVersionGLSL;   
TPRREuint       PRREhwVideoImpl::nVidVersionGLSLmaj = 0;
TPRREuint       PRREhwVideoImpl::nVidVersionGLSLmin = 0;

// Would be better to use C++11 uniform initialization style to fill the map ...
std::map<GLint, std::string> PRREhwVideoImpl::vTexComprFmts;


/** 
    Sets members to default values.
*/
PRREhwVideoImpl::PRREhwVideoImpl() :
    sharedSettings( PRRESharedSettings::createAndGet() )
{
    PreInitialize(NULL, NULL, 0, 0, 0, 0, 0, 0);
} // PRREhwVideoImpl()


PRREhwVideoImpl::PRREhwVideoImpl(const PRREhwVideoImpl&) :
    sharedSettings( PRRESharedSettings::createAndGet() )
{

}


PRREhwVideoImpl& PRREhwVideoImpl::operator=(const PRREhwVideoImpl&)
{
    return *this;
}


PRREhwVideoImpl::~PRREhwVideoImpl()
{
    Deinitialize();
} // ~PRREhwVideoImpl


/**
    Writes the given extension list to console.
*/
void PRREhwVideoImpl::printExtensionList(const std::string& features, const char* endstring)
{
#ifdef PGE_CCONSOLE_IS_ENABLED
    stringstream ssFeatures(features);
    string tmpSpacesString;
    int i = 0;
    int numspaces = 0;

    const int nIndentOriginal = getConsole().getIndent();
    const int nLineLeftMargin =  2;  // each line starts at this position
    const int nLineMax        = 80;  // assuming we have this number of chars available per line
    const int nLineEndMargin  =  2;  // need to leave this number of characters empty at end of line
    const int nLeftColMax     = nLineMax/2 - nLineLeftMargin - nLineEndMargin;  // left column have this width

    // temporarily reset console indentation level so we have enough space
    getConsole().SetIndent(nLineLeftMargin);

    // we print extensions in 2 columns
    do
    {
        string sub;
        ssFeatures >> sub;
        
        // sub is empty when no more extension left in ssFeatures
        if ( sub.empty() )
            continue;

        if ( i % 2 == 0 )
        {
            // left column in table
            getConsole().O("%s", sub.c_str());
            numspaces = nLeftColMax - sub.length();
        }
        else
        {
            // right column in table
            if ( (sub.length() <= (nLeftColMax+2)) && (numspaces > 0) )
            {
                tmpSpacesString = "";
                for (int j = 0; j < numspaces; j++)
                    tmpSpacesString += ' ';
                getConsole().O(tmpSpacesString.c_str());
            }
            else
            {
                // too long extension needs to be written to next line (left column) to avoid line wrap
                // we fell here due to one of 2 reasons:
                // - sub.length() > 40 means our current string originally targeted to right column is too long
                // - numspaces <= 0 means the previous string already printed to left column is too long, did not leave enough space for us
                // either reason we need to jump to next line
                getConsole().OLn("");
            }
            getConsole().OLn("%s", sub.c_str());
        }
        i++;
    } while (ssFeatures);

    getConsole().SetIndent(nIndentOriginal);

    if ( i % 2 == 1 )
        getConsole().OLn("");
    getConsole().OLn("");
    getConsole().OLn(endstring, i);
    getConsole().OLn("");
#endif
} // printExtensionList()


/**
    Writes OGL extension list to console.
*/
void PRREhwVideoImpl::printOGLextensions()
{
    printExtensionList(sVidFeaturesOGL, "parsed %d GL extensions");
} // printOGLextensions()


/**
    Writes WGL extension list to console.
*/
void PRREhwVideoImpl::printWGLextensions()
{
    printExtensionList(sVidFeaturesWGL, "parsed %d WGL extensions");
} // printWGLextensions()


/**
    Checks given value against given standard value.
*/
void PRREhwVideoImpl::CheckAgainstStandardMinValue(GLint value, GLint stdValue)
{
    if ( value < stdValue )
        getConsole().EOLn("%d is less than the standard minimum of %d!", value, stdValue);
} // CheckAgainstStandardMinValue()


/**
    Checks given value against given standard value.
*/
void PRREhwVideoImpl::CheckAgainstStandardMinValuef(GLfloat value, GLfloat stdValue)
{
    if ( value < stdValue )
        getConsole().EOLn("%f is less than the standard minimum of %f!", value, stdValue);
} // CheckAgainstStandardMinValuef()


/**
    Checks given range against given standard range.
*/
void PRREhwVideoImpl::CheckAgainstStandardRange(GLint val1, GLint val2, GLint stdVal1, GLint stdVal2)
{
    if ( ( val1 > stdVal1 ) || ( val2 < stdVal2 ) )
        getConsole().EOLn("Range problem: [%d, %d] vs standard [%d, %d]!",
                           val1, val2, stdVal1, stdVal2);
} // CheckAgainstStandardRange()


/**
    Query viewport details.
    Note that it is highly recommended to call all DiscoverOpenGL_X_Y_availability() functions before calling any DiscoverXYZ() function.
*/
void PRREhwVideoImpl::DiscoverViewportAvailability()
{
    getConsole().OLnOI("Viewport:");

    PRREGLsnippets::ClearGLerror();

    glGetIntegerv(GL_MAX_VIEWPORT_DIMS, tmpRet);
    if ( glGetError() == GL_NO_ERROR )
        getConsole().OLn("Maximum viewport size: %d x %d", tmpRet[0], tmpRet[1]);
    else getConsole().EOLn("Maximum viewport size: ? x ?");

    glGetIntegerv(GL_VIEWPORT_BOUNDS_RANGE, tmpRet);
    if ( glGetError() == GL_NO_ERROR )
    {
        getConsole().OLn("Viewport bounds range: [%d, %d]", tmpRet[0], tmpRet[1]);
        CheckAgainstStandardRange(tmpRet[0], tmpRet[1],
                                  GL_STANDARD_MIN_VIEWPORT_BOUNDS_RANGE[0],
                                  GL_STANDARD_MIN_VIEWPORT_BOUNDS_RANGE[1]);
    }
    else getConsole().EOLn("Viewport bounds range: [?, ?]");

    glGetIntegerv(GL_MAX_VIEWPORTS, tmpRet);
    if ( glGetError() == GL_NO_ERROR )
    {
        getConsole().OLn("Max viewports: %d", tmpRet[0]);
        CheckAgainstStandardMinValue(tmpRet[0], GL_STANDARD_MIN_VIEWPORTS);
    }
    else getConsole().EOLn("Max viewports: ?");

    getConsole().OLnOO("");
} // DiscoverViewportAvailability()


/**
    Query matrix stacks details.
    Note that it is highly recommended to call all DiscoverOpenGL_X_Y_availability() functions before calling any DiscoverXYZ() function.
*/
void PRREhwVideoImpl::DiscoverMatrixStacksAvailability()
{
    getConsole().OLnOI("Matrix stacks:");

    glGetIntegerv(GL_MAX_PROJECTION_STACK_DEPTH, tmpRet);
    getConsole().OLn("Max projection matrix stack depth: %d", tmpRet[0]);
    CheckAgainstStandardMinValue(tmpRet[0], GL_STANDARD_MIN_MATRIX_STACK_DEPTH_PROJECTION);

    glGetIntegerv(GL_MAX_MODELVIEW_STACK_DEPTH, tmpRet);
    getConsole().OLn("Max modelview matrix stack depth: %d", tmpRet[0]);
    CheckAgainstStandardMinValue(tmpRet[0], GL_STANDARD_MIN_MATRIX_STACK_DEPTH_MODELVIEW);

    glGetIntegerv(GL_MAX_COLOR_MATRIX_STACK_DEPTH, tmpRet);
    getConsole().OLn("Max color matrix stack depth: %d", tmpRet[0]);
    CheckAgainstStandardMinValue(tmpRet[0], GL_STANDARD_MIN_MATRIX_STACK_DEPTH_COLOR);

    glGetIntegerv(GL_MAX_TEXTURE_STACK_DEPTH, tmpRet);
    getConsole().OLn("Max texture stack depth: %d", tmpRet[0]);
    CheckAgainstStandardMinValue(tmpRet[0], GL_STANDARD_MIN_MATRIX_STACK_DEPTH_TEXTURE);

    glGetIntegerv(GL_MAX_ATTRIB_STACK_DEPTH, tmpRet);
    getConsole().OLn("Max server attribute stack depth: %d", tmpRet[0]);
    CheckAgainstStandardMinValue(tmpRet[0], GL_STANDARD_MIN_STACK_DEPTH_ATTRIB);

    glGetIntegerv(GL_MAX_CLIENT_ATTRIB_STACK_DEPTH, tmpRet);
    getConsole().OLn("Max client attribute stack depth: %d", tmpRet[0]);
    CheckAgainstStandardMinValue(tmpRet[0], GL_STANDARD_MIN_STACK_DEPTH_CLIENT_ATTRIB);

    getConsole().OLnOO("");
} // DiscoverMatrixStacksAvailability()


/**
    Query framebuffer and clipping details.
    Note that it is highly recommended to call all DiscoverOpenGL_X_Y_availability() functions before calling any DiscoverXYZ() function.
*/
void PRREhwVideoImpl::DiscoverFramebufferAndClipping()
{
    getConsole().OLnOI("Framebuffer and Clipping:");

    glGetIntegerv(GL_RED_BITS, &(tmpRet[0]));
    glGetIntegerv(GL_GREEN_BITS, &(tmpRet[1]));
    glGetIntegerv(GL_BLUE_BITS, &(tmpRet[2]));
    glGetIntegerv(GL_ALPHA_BITS, &(tmpRet[3]));
    glGetIntegerv(GL_DEPTH_BITS, &(tmpRet[4]));
    glGetIntegerv(GL_STENCIL_BITS, &(tmpRet[5]));

    getConsole().OLn("OpenGL framebuffer format: R%dG%dB%dA%d Z%d S%d",
                      tmpRet[0], tmpRet[1], tmpRet[2], tmpRet[3], tmpRet[4], tmpRet[5]);

    glGetIntegerv(GL_MAX_CLIP_PLANES, tmpRet);
    getConsole().OLn("Max clipping planes: %d", tmpRet[0]);
    CheckAgainstStandardMinValue(tmpRet[0], GL_STANDARD_MIN_CLIPPING_PLANES);

    getConsole().OLnOO("");
} // DiscoverFrambufferAndClipping()


/**
    Query multi draw arrays availability.
    Note that it is highly recommended to call all DiscoverOpenGL_X_Y_availability() functions before calling any DiscoverXYZ() function.
*/
void PRREhwVideoImpl::DiscoverMultiDrawArraysAvailability()
{
    // bSuppMultiDrawArrays is already set by DiscoverOpenGL_X_Y_availability()
    if ( bSuppMultiDrawArrays )
    {
        getConsole().SOLn("MultiDrawArrays supported!");
    }
    else
        getConsole().EOLn("MultiDrawArrays not supported!");
} // DiscoverMultiDrawArraysAvailability()


/**
    Query draw range elements availability.
    Note that it is highly recommended to call all DiscoverOpenGL_X_Y_availability() functions before calling any DiscoverXYZ() function.
*/
void PRREhwVideoImpl::DiscoverDrawRangeElementsAvailability()
{
    // bSuppMultiDrawArrays is already set by DiscoverOpenGL_X_Y_availability()
    if ( bSuppDrawRangeElements )
    {
        PRREGLsnippets::ClearGLerror();

        glGetIntegerv(GL_MAX_ELEMENTS_VERTICES_EXT, &(tmpRet[0]));
        if ( (glGetError() != GL_NO_ERROR) || (tmpRet[0] < 1) )
        {
            getConsole().EOLn("DrawRangeElements ext. is exported, but MRVAV is %d.", tmpRet[0]);
            getConsole().EOLn("DrawRangeElements is disabled, upgrade graphics card driver!");
            bSuppDrawRangeElements = false;
            PRREGLsnippets::ClearGLerror();
            return;
        }

        glGetIntegerv(GL_MAX_ELEMENTS_INDICES_EXT, &(tmpRet[1]));
        if ( (glGetError() != GL_NO_ERROR) || (tmpRet[1] < 1) )
        {
            getConsole().EOLn("DrawRangeElements ext. is exported, but MRVAI is %d.", tmpRet[1]);
            getConsole().EOLn("DrawRangeElements is disabled, upgrade graphics card driver!");
            bSuppDrawRangeElements = false;
            PRREGLsnippets::ClearGLerror();
            return;
        }
        
        getConsole().SOLn("DrawRangeElements supported!");
        getConsole().OLn("  Max recommended vertex array vertices: %d", tmpRet[0]);
        getConsole().OLn("  Max recommended vertex array indices: %d", tmpRet[1]);
    }
    else
        getConsole().EOLn("DrawRangeElements not supported!");
} // DiscoverDrawRangeElementsAvailability()


/**
    Query compiled vertex arrays availability.
    Note that it is highly recommended to call all DiscoverOpenGL_X_Y_availability() functions before calling any DiscoverXYZ() function.
*/
void PRREhwVideoImpl::DiscoverCompiledVertexArraysAvailability()
{
    // bSuppCompiledVertexArrays is not set before, we need to check extension and funcpointers here
    if ( bSuppCompiledVertexArrays = isExtensionSupported("GL_EXT_compiled_vertex_array") )
    {
        if ( bSuppCompiledVertexArrays = (glLockArraysEXT && glUnlockArraysEXT) )
            getConsole().SOLn("CompiledVertexArrays supported!");
        else
            getConsole().EOLn("CompiledVertexArrays not supported (func)!");
    }
    else
        getConsole().EOLn("CompiledVertexArrays not supported (ext)!");
} // DiscoverCompiledVertexArraysAvailability()


/**
    Query NV vertex array range availability.
    Note that it is highly recommended to call all DiscoverOpenGL_X_Y_availability() functions before calling any DiscoverXYZ() function.
*/
void PRREhwVideoImpl::DiscoverNVVertexArrayRangeAvailability()
{
    // bSuppNVVertexArrayRange is not set before, we need to check extension and funcpointers here
    if ( bSuppNVVertexArrayRange = isExtensionSupported("GL_NV_vertex_array_range") )
    {
        if ( bSuppNVVertexArrayRange = (glFlushVertexArrayRangeNV && glVertexArrayRangeNV) )
        {
            getConsole().SOLn("nVidia Vertex Array Range supported!");

            if ( bSuppNVVertexArrayRange2 = isExtensionSupported("GL_NV_vertex_array_range2") )
                getConsole().SOLn("nVidia Vertex Array Range 2 supported!");
            else
                getConsole().EOLn("nVidia Vertex Array Range 2 not supported (ext)!");
        }
        else
            getConsole().EOLn("nVidia Vertex Array Range not supported (func)!");
    }
    else
        getConsole().EOLn("nVidia Vertex Array Range not supported (ext)!");
} // DiscoverNVVertexArrayRangeAvailability()


/**
    Query NV element array availability.
    Note that it is highly recommended to call all DiscoverOpenGL_X_Y_availability() functions before calling any DiscoverXYZ() function.
*/
void PRREhwVideoImpl::DiscoverNVElementArrayAvailability()
{
    if ( bSuppNVElementArray = isExtensionSupported("GL_NV_element_array") )
    {
        // I would check for func pointers but this extension is not in my glext.h for some reason, maybe they removed it ...
        // Will dig deeper when I want to use this actually ...
        getConsole().SOLn("NV Element Array supported!");
    }
    else
        getConsole().EOLn("NV Element Array not supported (ext)!");
} // DiscoverNVElementArrayAvailability()


/**
    Query ATI vertex array object availability.
    Note that it is highly recommended to call all DiscoverOpenGL_X_Y_availability() functions before calling any DiscoverXYZ() function.
*/
void PRREhwVideoImpl::DiscoverATIVertexArrayObjectAvailability()
{
    // bSuppATIVertexArrayObject is not set before by DiscoverOpenGL_X_Y_availability(), we need to check extension and funcpointers here
    if ( bSuppATIVertexArrayObject = isExtensionSupported("GL_ATI_vertex_array_object") )
    {
        if ( bSuppATIVertexArrayObject = (glNewObjectBufferATI && glIsObjectBufferATI &&
             glUpdateObjectBufferATI && glGetObjectBufferfvATI && glGetObjectBufferivATI &&
             glFreeObjectBufferATI && glArrayObjectATI && glGetArrayObjectfvATI &&
             glGetArrayObjectivATI && glVariantArrayObjectATI && glGetVariantArrayObjectfvATI &&
             glGetVariantArrayObjectivATI)
           )
            getConsole().SOLn("ATI Vertex Array Object supported!");
        else
            getConsole().EOLn("ATI Vertex Array Object not supported (func)!");
    }
    else
        getConsole().EOLn("ATI Vertex Array Object not supported (ext)!");
} // DiscoverATIVertexArrayObjectAvailability()


/**
    Query ATI map object buffer availability.
    Note that it is highly recommended to call all DiscoverOpenGL_X_Y_availability() functions before calling any DiscoverXYZ() function.
*/
void PRREhwVideoImpl::DiscoverATIMapObjectBufferAvailability()
{
    // bSuppATIMapObjectBuffer is not set before by DiscoverOpenGL_X_Y_availability(), we need to check extension and funcpointers here
    if ( bSuppATIMapObjectBuffer = isExtensionSupported("GL_ATI_map_object_buffer") )
    {
        if ( bSuppATIMapObjectBuffer = (glMapObjectBufferATI && glUnmapObjectBufferATI) )
            getConsole().SOLn("ATI Map Object Buffer supported!");
        else
            getConsole().EOLn("ATI Map Object Buffer not supported (func)!");
    }
    else
        getConsole().EOLn("ATI Map Object Buffer not supported (ext)!");
} // DiscoverATIMapObjectBufferAvailability()


/**
    Query ATI element array availability.
    Note that it is highly recommended to call all DiscoverOpenGL_X_Y_availability() functions before calling any DiscoverXYZ() function.
*/
void PRREhwVideoImpl::DiscoverATIElementArrayAvailability()
{
    // bSuppATIElementArray is not set before by DiscoverOpenGL_X_Y_availability(), we need to check extension and funcpointers here
    if ( bSuppATIElementArray = isExtensionSupported("GL_ATI_element_array") )
    {
        if ( bSuppATIMapObjectBuffer = (glElementPointerATI && glDrawElementArrayATI && glDrawRangeElementArrayATI) )
            getConsole().SOLn("ATI Element Array supported!");
        else
            getConsole().EOLn("ATI Element Array not supported (func)!");
    }
    else
        getConsole().EOLn("ATI Element Array not supported (ext)!");
} // DiscoverATIElementArrayAvailability()


/**
    Query vertex buffers availability.
    Note that it is highly recommended to call all DiscoverOpenGL_X_Y_availability() functions before calling any DiscoverXYZ() function.
*/
void PRREhwVideoImpl::DiscoverVertexBuffersAvailability()
{
    // bSuppVertexBuffers is already set by DiscoverOpenGL_X_Y_availability()
    if ( bSuppVertexBuffers )
        getConsole().SOLn("Vertex Buffer Objects supported!");
    else
        getConsole().EOLn("Vertex Buffer Objects not supported!");
} // DiscoverVertexBuffersAvailability()


/**
    Query ATI TruForm availability.
    Note that it is highly recommended to call all DiscoverOpenGL_X_Y_availability() functions before calling any DiscoverXYZ() function.
*/
void PRREhwVideoImpl::DiscoverATITruFormAvailability()
{
    // bSuppATITruForm is not set before by DiscoverOpenGL_X_Y_availability(), we need to check extension and funcpointers here
    if ( bSuppATITruForm = isExtensionSupported("GL_ATI_pn_triangles") )
    {
        if ( bSuppATIMapObjectBuffer = (glPNTrianglesiATI && glPNTrianglesfATI) )
            getConsole().SOLn("ATI TruForm supported!");
        else
            getConsole().EOLn("ATI TruForm not supported (func)!");
    }
    else
        getConsole().EOLn("ATI TruForm not supported (ext)!");
} // DiscoverATITruFormAvailability()


/**
    Query vertex rendering specific features availability.
    Note that it is highly recommended to call all DiscoverOpenGL_X_Y_availability() functions before calling any DiscoverXYZ() function.
*/
void PRREhwVideoImpl::DiscoverVertexRenderingAvailability()
{
    getConsole().OLnOI("Vertex Rendering Caps:");

    glGetIntegerv(GL_MAX_LIST_NESTING, tmpRet);
    getConsole().OLn("Max display list nesting level: %d", tmpRet[0]);
    CheckAgainstStandardMinValue(tmpRet[0], GL_STANDARD_MIN_DISPLAY_LIST_NESTING_LEVEL);

    DiscoverMultiDrawArraysAvailability();
    DiscoverDrawRangeElementsAvailability();
    DiscoverCompiledVertexArraysAvailability();
    DiscoverNVVertexArrayRangeAvailability();
    DiscoverATIVertexArrayObjectAvailability();
    DiscoverATIMapObjectBufferAvailability();
    DiscoverVertexBuffersAvailability();
    DiscoverATITruFormAvailability();

    getConsole().OLnOO("");
} // DiscoverVertexRenderingAvailability()


/**
    Query multitexturing availability.
    Note that it is highly recommended to call all DiscoverOpenGL_X_Y_availability() functions before calling any DiscoverXYZ() function.
*/
void PRREhwVideoImpl::DiscoverMultiTexturingAvailability()
{
    // bSuppMultiTexturing is already set by DiscoverOpenGL_X_Y_availability()
    if ( bSuppMultiTexturing )
    {
        glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB, (GLint*) &nTMUcount);
        if ( nTMUcount > 1 )
            getConsole().SOLn("Multitexturing supported, TMU count is %d.", nTMUcount);
        else
        {
            getConsole().EOLn("Multitexturing ext. is exported, but TMU count is %d.", nTMUcount);
            getConsole().EOLn("Multitexturing is disabled, upgrade graphics card driver!");
            nTMUcount = 1;
            bSuppMultiTexturing = false;
            PRREGLsnippets::ClearGLerror();
        }
    }
    else
    {
        nTMUcount = 1;
        getConsole().EOLn("Multitexturing not supported.");
    }
} // DiscoverMultiTexturingAvailability()


/**
    Query anisotropic filtering availability.
    Note that it is highly recommended to call all DiscoverOpenGL_X_Y_availability() functions before calling any DiscoverXYZ() function.
*/
void PRREhwVideoImpl::DiscoverAnisoFilteringAvailability()
{
    // bSuppAnisoFiltering is not set before by DiscoverOpenGL_X_Y_availability(), we need to check extension here
    if ( bSuppAnisoFiltering = isExtensionSupported("GL_EXT_texture_filter_anisotropic") )
    {
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &nAnisoFilterMax);
        if ( nAnisoFilterMax > 1.0 )
            getConsole().SOLn("Anisotropic filtering supported @ %fX", nAnisoFilterMax);
        else
        {
            getConsole().EOLn("Anisotropic filtering ext. is exported, but nAnisoFilterMax is %f.", nAnisoFilterMax);
            getConsole().EOLn("Anisotropic filtering is disabled, upgrade graphics card driver!");
            nAnisoFilterMax = 0.0;
            bSuppAnisoFiltering = false;
            PRREGLsnippets::ClearGLerror();
        }
    }
    else
        getConsole().EOLn("Anisotropic filtering not supported.");
} // DiscoverAnisoFilteringAvailability()


/**
    Query texture compression availability.
    Note that it is highly recommended to call all DiscoverOpenGL_X_Y_availability() functions before calling any DiscoverXYZ() function.
*/
void PRREhwVideoImpl::DiscoverTextureCompressionAvailability()
{
    // bSuppTexCompression is already set by DiscoverOpenGL_X_Y_availability()
    if ( bSuppTexCompression )
    {
        getConsole().SOLn("Texture compression supported!");
        glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS_ARB, tmpRet);
        if ( glGetError() == GL_NO_ERROR )
        {
            getConsole().OLn("  %d compressed texture formats available:", tmpRet[0]);
            CheckAgainstStandardMinValue(tmpRet[0], GL_STANDARD_MIN_COMPRESSED_TEXTURE_FORMATS);
            GLint* compressedFormats = (GLint*) malloc(tmpRet[0] * sizeof(GLint));
            glGetIntegerv(GL_COMPRESSED_TEXTURE_FORMATS_ARB, compressedFormats);
            if ( glGetError() == GL_NO_ERROR )
            {
                getConsole().OI();
                for (GLint i = 0; i < tmpRet[0]; i++)
                    if ( vTexComprFmts.find( compressedFormats[i] ) == vTexComprFmts.end() )
                        getConsole().OLn("  Unknown: %d", compressedFormats[i]);
                    else
                        getConsole().OLn("  %s", vTexComprFmts[ compressedFormats[i] ].c_str());   
                getConsole().OO();
            }

            free(compressedFormats);
        }
        else
            getConsole().EOLn("  Compressed texture formats available: ?");

        if ( bSuppVTC = isExtensionSupported("GL_NV_texture_compression_vtc") )
            getConsole().SOLn("  VTC also supported.");
        if ( bSupp3Dc = isExtensionSupported("GL_ATI_texture_compression_3dc") )
            getConsole().SOLn("  3Dc also supported.");
        if ( bSuppRGTC )
            getConsole().SOLn("  RGTC also supported.");
        if ( bSuppLATC = isExtensionSupported("GL_EXT_texture_compression_latc") || isExtensionSupported("GL_NV_texture_compression_latc") )
            getConsole().SOLn("  LATC also supported.");
    }
    else
        getConsole().EOLn("Texture compression not supported.");
} // DiscoverTextureCompressionAvailability()


/**
    Query texturing capabilities.
    Note that it is highly recommended to call all DiscoverOpenGL_X_Y_availability() functions before calling any DiscoverXYZ() function.
*/
void PRREhwVideoImpl::DiscoverTexturingAvailability()
{
    getConsole().OLnOI("Texturing Caps:");

    PRREGLsnippets::ClearGLerror();

    glGetFloatv(GL_MAX_TEXTURE_LOD_BIAS_EXT, tmpRetf);
    if ( glGetError() == GL_NO_ERROR )
    {
        getConsole().OLn("Max texture LOD bias: %f", tmpRetf[0]);
        CheckAgainstStandardMinValuef(tmpRetf[0], GL_STANDARD_MIN_TEXTURE_LOD_BIAS);
    }
    else getConsole().EOLn("Max texture LOD bias: ?");

    glGetIntegerv(GL_MAX_TEXTURE_SIZE, tmpRet);
    if ( glGetError() == GL_NO_ERROR )
    {
        getConsole().OLn("Max texture size: %d x %d", tmpRet[0], tmpRet[0]);
        CheckAgainstStandardMinValue(tmpRet[0], GL_STANDARD_MIN_TEXTURE_SIZE);
    }
    else getConsole().EOLn("Max texture size: ? x ?");
    
    glGetIntegerv(GL_MAX_RECTANGLE_TEXTURE_SIZE_ARB, tmpRet);
    if ( glGetError() == GL_NO_ERROR )
    {
        getConsole().OLn("Max rectangle texture size: %d x %d", tmpRet[0], tmpRet[0]);
        CheckAgainstStandardMinValue(tmpRet[0], GL_STANDARD_MIN_RECTANGLE_TEXTURE_SIZE);
    }
    else getConsole().EOLn("Max rectangle texture size: ? x ?");

    glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE_EXT, tmpRet);
    if ( glGetError() == GL_NO_ERROR )
    {
        getConsole().OLn("Max 3D texture size: %d x %d x %d", tmpRet[0], tmpRet[0], tmpRet[0]);
        CheckAgainstStandardMinValue(tmpRet[0], GL_STANDARD_MIN_3D_TEXTURE_SIZE);
    }
    else getConsole().EOLn("Max 3D texture size: ? x ? x ?");
    
    glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE_EXT, tmpRet);
    if ( glGetError() == GL_NO_ERROR )
    {
        getConsole().OLn("Max cube map texture size: %d x %d", tmpRet[0], tmpRet[0]);
        CheckAgainstStandardMinValue(tmpRet[0], GL_STANDARD_MIN_CUBE_MAP_TEXTURE_SIZE);
    }
    else getConsole().EOLn("Max cube map texture size: ? x ?");

    DiscoverMultiTexturingAvailability();
    DiscoverAnisoFilteringAvailability();  

    if ( bSuppHWmipmapGen = isExtensionSupported("GL_SGIS_generate_mipmap") )
        getConsole().SOLn("Hardware mipmap generation supported.");
    else
        getConsole().EOLn("Hardware mipmap generation not supported, using software.");

    // bSuppBGRAtex is already set by DiscoverOpenGL_X_Y_availability()
    if ( bSuppBGRAtex )
        getConsole().SOLn("Native support for BGR/BGRA formats (DIBs).");
    else
        getConsole().EOLn("Native support for BGR/BGRA formats (DIBs) not found, using software.");
    
    DiscoverTextureCompressionAvailability();

    /*
        TODO: NPOT texture check, texture rectangle check
        http://www.opengl.org/registry/specs/ARB/texture_non_power_of_two.txt
        http://www.opengl.org/registry/specs/ARB/texture_rectangle.txt 
        http://www.opengl.org/registry/specs/NV/texture_rectangle.txt
    */

    getConsole().OLnOO("");

} // DiscoverTexturingAvailability()


/**
    Query FSAA availability.
    Note that it is highly recommended to call all DiscoverOpenGL_X_Y_availability() functions before calling any DiscoverXYZ() function.
*/
void PRREhwVideoImpl::DiscoverFSAAavailability(HDC dc)
{
    getConsole().OLnOI("FSAA:");

    nMSAAmaxLevel = 0;
    memset(bMSAAlevels, 0, MSAA_SUPP_LEVELS_ARRAY_SIZE);
    if ( wglChoosePixelFormatARB == PGENULL )
    {
        getConsole().EOLn("WARNING: wglChoosePixelFormatARB is PGENULL!");
        return;
    }

    if ( !isExtensionSupported("WGL_ARB_multisample") )
    {
        getConsole().EOLn("WARNING: WGL_ARB_multisample not supported!");
        return;
    }
    
    TPRREint samples = MSAA_SUPP_LEVELS_ARRAY_SIZE;
    int iAttributes[] =
    {
        WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
        WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
        WGL_COLOR_BITS_ARB,     nColorBits,
        WGL_ALPHA_BITS_ARB,     8,
        WGL_DEPTH_BITS_ARB,     nDepthBits,
        WGL_STENCIL_BITS_ARB,   nStencilBits,
        WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
        WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
        WGL_SAMPLES_ARB,        samples,
        0,                      0
    };
    float fAttributes[] = {0, 0};

    while ( samples > 0 )
    {
        samples--;
        iAttributes[19] = samples;
        UINT numformats;
        int pixelformat;
        int valid = wglChoosePixelFormatARB(dc, iAttributes, fAttributes, (UINT)1, &pixelformat, &numformats);
        if ( (valid > 0) && (numformats > 0) )
        {
            if ( nMSAAmaxLevel == 0 )
                nMSAAmaxLevel = samples;
            bMSAAlevels[samples] = true;
        }
    }
    getConsole().OLn("Supported MSAA levels:");
    for (TPRREint i = 0; i < MSAA_SUPP_LEVELS_ARRAY_SIZE; i++)
        if ( bMSAAlevels[i] )
            getConsole().O(" %d", i);
    getConsole().OLn("");

    getConsole().OLnOO("");
} // DiscoverFSAAavailability()

/**
    Checks if given extension is supported and prints the result.
    Not only extension name string but extension-specific function pointers can be also checked.

    @param title     A human-readable short form title of the extension/feature.
    @param name      The extension name we are checking for.
    @param functions Extension-specific function pointers to be checked. This vector becomes empty after the call.

    @return True if given extension name string is exported by driver and optionally specified function pointers
            are also valid, otherwise false.
*/
bool PRREhwVideoImpl::printExtensionSupported(const char* title, const char* name, std::vector<void*>& functions)
{
    bool result;

    getConsole().O(title);
    if ( result = isExtensionSupported(name) )
    {
        for (std::size_t i = 0; (i < functions.size()) && result; i++)
            result &= (functions[i] != PGENULL);

        if ( result )
            getConsole().SOLn("OK!");
        else
            getConsole().EOLn("NOK (func)!");
    }
    else
        getConsole().EOLn("NOK (ext)!");

    functions.clear();
    return result;
}

bool PRREhwVideoImpl::discoverOpenGL_1_1_availability()
{
    return true;
} // discoverOpenGL_1_1_availability()

bool PRREhwVideoImpl::discoverOpenGL_1_2_availability()
{
    bool result = true;
    std::vector<void*> functions;

    getConsole().OI();

    functions.push_back( glTexImage3DEXT );
    functions.push_back( glTexSubImage3DEXT );
    result &= printExtensionSupported("3D tex is ", "GL_EXT_texture3D", functions);

    bSuppBGRAtex = printExtensionSupported("BGRA px formats are ", "GL_EXT_bgra", functions);
    result &= bSuppBGRAtex;

    result &= printExtensionSupported("Packed px formats are ", "GL_EXT_packed_pixels", functions);

    result &= printExtensionSupported("Normal rescaling is ", "GL_EXT_rescale_normal", functions);

    result &= printExtensionSupported("Separate spec clr is ", "GL_EXT_separate_specular_color", functions);

    bool bTexEdgeClamp = printExtensionSupported("Texcoord edge clamping is ", "GL_SGIS_texture_edge_clamp", functions);

    bTexEdgeClamp |= printExtensionSupported("Texcoord edge clamping is ", "GL_EXT_texture_edge_clamp", functions);
    
    result &= bTexEdgeClamp;

    result &= printExtensionSupported("Tex LOD control is ", "GL_SGIS_texture_lod", functions);

    functions.push_back( glDrawRangeElementsEXT );
    bSuppDrawRangeElements = printExtensionSupported("VA draw element rng is ", "GL_EXT_draw_range_elements", functions);

    result &= bSuppDrawRangeElements;

    getConsole().OO();

    return result;
} // discoverOpenGL_1_2_availability()

bool PRREhwVideoImpl::discoverOpenGL_1_3_availability()
{
    bool result = true;
    std::vector<void*> functions;

    getConsole().OI();

    // 3D textures were introduced in OGL 1.2 so we check for compressed 3D textures availability as well here
    functions.push_back( glCompressedTexImage3DARB );
    functions.push_back( glCompressedTexImage2DARB );
    functions.push_back( glCompressedTexImage1DARB );
    functions.push_back( glCompressedTexSubImage3DARB );
    functions.push_back( glCompressedTexSubImage2DARB );
    functions.push_back( glCompressedTexSubImage1DARB );
    functions.push_back( glGetCompressedTexImageARB );
    bSuppTexCompression = printExtensionSupported("Compr textures (ARB) are ", "GL_ARB_texture_compression", functions);

    /* although GL_EXT_texture_compression_s3tc and GL_3DFX_texture_compression_FXT1 depend on GL_ARB_texture_compression, we check them
       with "or" because there could be a driver supporting a specific TC without exporting GL_ARB_texture_compression. */
    if ( !bSuppTexCompression )
    {
        // let's try again with other extension (experimental code reserved for old HW)
        functions.push_back( glCompressedTexImage3DARB );
        functions.push_back( glCompressedTexImage2DARB );
        functions.push_back( glCompressedTexImage1DARB );
        functions.push_back( glCompressedTexSubImage3DARB );
        functions.push_back( glCompressedTexSubImage2DARB );
        functions.push_back( glCompressedTexSubImage1DARB );
        functions.push_back( glGetCompressedTexImageARB );
        bSuppTexCompression = printExtensionSupported("Compr textures (EXT) are ", "GL_EXT_texture_compression_s3tc", functions);
    }

    if ( !bSuppTexCompression )
    {
        // let's try again with other extension (experimental code reserved for old HW)
        functions.push_back( glCompressedTexImage3DARB );
        functions.push_back( glCompressedTexImage2DARB );
        functions.push_back( glCompressedTexImage1DARB );
        functions.push_back( glCompressedTexSubImage3DARB );
        functions.push_back( glCompressedTexSubImage2DARB );
        functions.push_back( glCompressedTexSubImage1DARB );
        functions.push_back( glGetCompressedTexImageARB );
        bSuppTexCompression = printExtensionSupported("Compr textures (FXT1) are ", "GL_3DFX_texture_compression_FXT1", functions);
    }

    result &= bSuppTexCompression;

    result &= printExtensionSupported("Cube map textures are ", "GL_ARB_texture_cube_map", functions);

    functions.push_back(glSampleCoverageARB);
    result &= printExtensionSupported("Multisampling is ", "GL_ARB_multisample", functions);

    functions.push_back( glActiveTextureARB );
    functions.push_back( glClientActiveTextureARB );
    functions.push_back( glMultiTexCoord1dARB );
    functions.push_back( glMultiTexCoord1dvARB );
    functions.push_back( glMultiTexCoord1fARB );
    functions.push_back( glMultiTexCoord1fvARB );
    functions.push_back( glMultiTexCoord1iARB );
    functions.push_back( glMultiTexCoord1ivARB );
    functions.push_back( glMultiTexCoord1sARB );
    functions.push_back( glMultiTexCoord1svARB );
    functions.push_back( glMultiTexCoord2dARB );
    functions.push_back( glMultiTexCoord2dvARB );
    functions.push_back( glMultiTexCoord2fARB );
    functions.push_back( glMultiTexCoord2fvARB );
    functions.push_back( glMultiTexCoord2iARB );
    functions.push_back( glMultiTexCoord2ivARB );
    functions.push_back( glMultiTexCoord2sARB );
    functions.push_back( glMultiTexCoord2svARB );
    functions.push_back( glMultiTexCoord3dARB );
    functions.push_back( glMultiTexCoord3dvARB );
    functions.push_back( glMultiTexCoord3fARB );
    functions.push_back( glMultiTexCoord3fvARB );
    functions.push_back( glMultiTexCoord3iARB );
    functions.push_back( glMultiTexCoord3ivARB );
    functions.push_back( glMultiTexCoord3sARB );
    functions.push_back( glMultiTexCoord3svARB );
    functions.push_back( glMultiTexCoord4dARB );
    functions.push_back( glMultiTexCoord4dvARB );
    functions.push_back( glMultiTexCoord4fARB );
    functions.push_back( glMultiTexCoord4fvARB );
    functions.push_back( glMultiTexCoord4iARB );
    functions.push_back( glMultiTexCoord4ivARB );
    functions.push_back( glMultiTexCoord4sARB );
    functions.push_back( glMultiTexCoord4svARB );
    bSuppMultiTexturing = printExtensionSupported("Multitexturing is ", "GL_ARB_multitexture", functions);
    result &= bSuppMultiTexturing;

    result &= printExtensionSupported("Tex add env mode is ", "GL_ARB_texture_env_add", functions);

    result &= printExtensionSupported("Tex comb env mode is ", "GL_ARB_texture_env_combine", functions);

    result &= printExtensionSupported("Tex dot3 env mode is ", "GL_ARB_texture_env_dot3", functions);

    result &= printExtensionSupported("Tex border clamp is ", "GL_ARB_texture_border_clamp", functions);

    result &= printExtensionSupported("Transpose matrix is ", "GL_ARB_transpose_matrix", functions);

    getConsole().OO();

    return result;
} // discoverOpenGL_1_3_availability()
    
bool PRREhwVideoImpl::discoverOpenGL_1_4_availability()
{
    bool result = true;
    std::vector<void*> functions;

    getConsole().OI();

    result &= printExtensionSupported("Auto MIP map gen is ", "GL_SGIS_generate_mipmap", functions);

    result &= printExtensionSupported("Blend squaring is ", "GL_NV_blend_square", functions);

    result &= printExtensionSupported("Depth textures are ", "GL_ARB_depth_texture", functions);

    result &= printExtensionSupported("Depth shadows are ", "GL_ARB_shadow", functions);

    result &= printExtensionSupported("Fog coordinates are ", "GL_EXT_fog_coord", functions);

    functions.push_back( glMultiDrawArraysEXT );
    functions.push_back( glMultiDrawElementsEXT );
    bSuppMultiDrawArrays = printExtensionSupported("Multiple draw arrays EXT is ", "GL_EXT_multi_draw_arrays", functions);

    functions.push_back( glMultiDrawArraysEXT );
    functions.push_back( glMultiDrawElementsEXT );
    bSuppMultiDrawArrays |= printExtensionSupported("Multiple draw arrays SUN is ", "GL_SUN_multi_draw_arrays", functions);
    
    result &= bSuppMultiDrawArrays;

    functions.push_back( glPointParameterfARB );
    functions.push_back( glPointParameterfvARB );
    result &= printExtensionSupported("Point params are ", "GL_ARB_point_parameters", functions);

    functions.push_back( glSecondaryColor3bEXT );
    functions.push_back( glSecondaryColor3bvEXT );
    functions.push_back( glSecondaryColor3dEXT );
    functions.push_back( glSecondaryColor3dvEXT );
    functions.push_back( glSecondaryColor3fEXT );
    functions.push_back( glSecondaryColor3fvEXT );
    functions.push_back( glSecondaryColor3iEXT );
    functions.push_back( glSecondaryColor3ivEXT );
    functions.push_back( glSecondaryColor3sEXT );
    functions.push_back( glSecondaryColor3svEXT );
    functions.push_back( glSecondaryColor3ubEXT );
    functions.push_back( glSecondaryColor3ubvEXT );
    functions.push_back( glSecondaryColor3uiEXT );
    functions.push_back( glSecondaryColor3uivEXT );
    functions.push_back( glSecondaryColor3usEXT );
    functions.push_back( glSecondaryColor3usvEXT );
    functions.push_back( glSecondaryColorPointerEXT );
    result &= printExtensionSupported("Sec color is ", "GL_EXT_secondary_color", functions);

    functions.push_back( glBlendFuncSeparateEXT );
    result &= printExtensionSupported("Sep blend funcs are ", "GL_EXT_blend_func_separate", functions);

    result &= printExtensionSupported("Stencil wrap is ", "GL_EXT_stencil_wrap", functions);

    result &= printExtensionSupported("Tex xbar env mode is ", "GL_ARB_texture_env_crossbar", functions);

    result &= printExtensionSupported("Tex LOD bias is ", "GL_EXT_texture_lod_bias", functions);

    result &= printExtensionSupported("Tex ARB mirr rpt is ", "GL_ARB_texture_mirrored_repeat", functions);

    result &= printExtensionSupported("Tex IBM mirr rpt is ", "GL_IBM_texture_mirrored_repeat", functions);

    functions.push_back( glSecondaryColor3bEXT );
    functions.push_back( glSecondaryColor3bvEXT );
    functions.push_back( glSecondaryColor3dEXT );
    functions.push_back( glSecondaryColor3dvEXT );
    functions.push_back( glSecondaryColor3fEXT );
    functions.push_back( glSecondaryColor3fvEXT );
    functions.push_back( glSecondaryColor3iEXT );
    functions.push_back( glSecondaryColor3ivEXT );
    functions.push_back( glSecondaryColor3sEXT );
    functions.push_back( glSecondaryColor3svEXT );
    functions.push_back( glSecondaryColor3ubEXT );
    functions.push_back( glSecondaryColor3ubvEXT );
    functions.push_back( glSecondaryColor3uiEXT );
    functions.push_back( glSecondaryColor3uivEXT );
    functions.push_back( glSecondaryColor3usEXT );
    functions.push_back( glSecondaryColor3usvEXT );
    functions.push_back( glSecondaryColorPointerEXT );
    result &= printExtensionSupported("Wnd raster pos is ", "GL_ARB_window_pos", functions);

    getConsole().OO();

    return result;
} // discoverOpenGL_1_4_availability()


bool PRREhwVideoImpl::discoverOpenGL_1_5_availability()
{
    bool result = true;
    std::vector<void*> functions;

    getConsole().OI();

    functions.push_back( glGenBuffersARB );
    functions.push_back( glBindBufferARB );
    functions.push_back( glBufferDataARB );
    functions.push_back( glBufferSubDataARB );
    functions.push_back( glGetBufferSubDataARB );
    functions.push_back( glDeleteBuffersARB );
    functions.push_back( glIsBufferARB );
    functions.push_back( glMapBufferARB );
    functions.push_back( glUnmapBufferARB );
    functions.push_back( glGetBufferParameterivARB );
    functions.push_back( glGetBufferPointervARB );
    bSuppVertexBuffers = printExtensionSupported("Buff objects are ", "GL_ARB_vertex_buffer_object", functions);
    result &= bSuppVertexBuffers;

    functions.push_back( glGenQueriesARB );
    functions.push_back( glDeleteQueriesARB );
    functions.push_back( glIsQueryARB );
    functions.push_back( glBeginQueryARB );
    functions.push_back( glEndQueryARB );
    functions.push_back( glGetQueryivARB );
    functions.push_back( glGetQueryObjectivARB );
    functions.push_back( glGetQueryObjectuivARB );
    result &= printExtensionSupported("Occlusion query is ", "GL_ARB_occlusion_query", functions);

    result &= printExtensionSupported("Shadow funcs are ", "GL_EXT_shadow_funcs", functions);

    getConsole().OO();

    return result;
} // discoverOpenGL_1_5_availability()


bool PRREhwVideoImpl::discoverOpenGL_2_0_availability()
{
    bool result = true;
    std::vector<void*> functions;

    getConsole().OI();

    functions.push_back( glDeleteObjectARB );
    functions.push_back( glGetHandleARB );
    functions.push_back( glDetachObjectARB );
    functions.push_back( glCreateShaderObjectARB );
    functions.push_back( glShaderSourceARB );
    functions.push_back( glCompileShaderARB );
    functions.push_back( glCreateProgramObjectARB );
    functions.push_back( glAttachObjectARB );
    functions.push_back( glLinkProgramARB );
    functions.push_back( glUseProgramObjectARB );
    functions.push_back( glValidateProgramARB );
    functions.push_back( glUniform1fARB );
    functions.push_back( glUniform2fARB );
    functions.push_back( glUniform3fARB );
    functions.push_back( glUniform4fARB );
    functions.push_back( glUniform1iARB );
    functions.push_back( glUniform2iARB );
    functions.push_back( glUniform3iARB );
    functions.push_back( glUniform4iARB );
    functions.push_back( glUniform1fvARB );
    functions.push_back( glUniform2fvARB );
    functions.push_back( glUniform3fvARB );
    functions.push_back( glUniform4fvARB );
    functions.push_back( glUniform1ivARB );
    functions.push_back( glUniform2ivARB );
    functions.push_back( glUniform3ivARB );
    functions.push_back( glUniform4ivARB );
    functions.push_back( glUniformMatrix2fvARB );
    functions.push_back( glUniformMatrix3fvARB );
    functions.push_back( glUniformMatrix4fvARB );
    functions.push_back( glGetObjectParameterfvARB );
    functions.push_back( glGetObjectParameterivARB );
    functions.push_back( glGetInfoLogARB );
    functions.push_back( glGetAttachedObjectsARB );
    functions.push_back( glGetUniformLocationARB );
    functions.push_back( glGetActiveUniformARB );
    functions.push_back( glGetUniformfvARB );
    functions.push_back( glGetUniformivARB );
    functions.push_back( glGetShaderSourceARB );
    result &= printExtensionSupported("Shader objects are ", "GL_ARB_shader_objects", functions);

    functions.push_back( glBindAttribLocationARB );
    functions.push_back( glGetActiveAttribARB );
    functions.push_back( glGetAttribLocationARB );
    result &= printExtensionSupported("VS is ", "GL_ARB_vertex_shader", functions);

    result &= printExtensionSupported("FS is ", "GL_ARB_fragment_shader", functions);

    result &= printExtensionSupported("SL 1.1 is ", "GL_ARB_shading_language_100", functions);

    functions.push_back( glDrawBuffersARB );
    result &= printExtensionSupported("Multi render targets are ", "GL_ARB_draw_buffers", functions);

    result &= printExtensionSupported("NPOT tex is ", "GL_ARB_texture_non_power_of_two", functions);

    result &= printExtensionSupported("Point sprites are ", "GL_ARB_point_sprite", functions);

    // assuming GL_ATI_separate_stencil and GL_EXT_stencil_two_side are exchangeable
    functions.push_back( glStencilOpSeparateATI );
    functions.push_back( glStencilFuncSeparateATI );
    bool b2sideStencil = printExtensionSupported("ATI sep stenc is ", "GL_ATI_separate_stencil", functions);

    functions.push_back( glActiveStencilFaceEXT );
    b2sideStencil |= printExtensionSupported("Two side stenc is ", "GL_EXT_stencil_two_side", functions);

    result &= b2sideStencil;

    getConsole().OO();

    return result;
} // discoverOpenGL_2_0_availability()


bool PRREhwVideoImpl::discoverOpenGL_2_1_availability()
{
    bool result = true;
    std::vector<void*> functions;

    getConsole().OI();

    result &= printExtensionSupported("Px buff objects are ", "GL_ARB_pixel_buffer_object", functions);

    result &= printExtensionSupported("sRGB textures are ", "GL_EXT_texture_sRGB", functions);

    getConsole().O("Expected GLSL version is ");
    if ( (nVidVersionGLSLmaj > 2) || ( (nVidVersionGLSLmaj == 2) && (nVidVersionGLSLmin >= 1) ) )
    {
        getConsole().SOLn("supported!");
    }
    else
    {
        result &= false;
        getConsole().EOLn("NOT supported!");
    }

    getConsole().OO();

    return result;
} // discoverOpenGL_2_1_availability()


bool PRREhwVideoImpl::discoverOpenGL_3_0_availability()
{
    bool result = true;
    std::vector<void*> functions;

    getConsole().OI();

    functions.push_back(glIsRenderbuffer);
    functions.push_back(glBindRenderbuffer);
    functions.push_back(glDeleteRenderbuffers);
    functions.push_back(glGenRenderbuffers);
    functions.push_back(glRenderbufferStorage);
    functions.push_back(glGetRenderbufferParameteriv);
    functions.push_back(glIsFramebuffer);
    functions.push_back(glBindFramebuffer);
    functions.push_back(glDeleteFramebuffers);
    functions.push_back(glGenFramebuffers);
    functions.push_back(glCheckFramebufferStatus);
    functions.push_back(glFramebufferTexture1D);
    functions.push_back(glFramebufferTexture2D);
    functions.push_back(glFramebufferTexture3D);
    functions.push_back(glFramebufferRenderbuffer);
    functions.push_back(glGetFramebufferAttachmentParameteriv);
    functions.push_back(glGenerateMipmap);
    functions.push_back(glBlitFramebuffer);
    functions.push_back(glRenderbufferStorageMultisample);
    functions.push_back(glFramebufferTextureLayer);
    result &= printExtensionSupported("FB objects are ", "GL_ARB_framebuffer_object", functions);

    functions.push_back(glBindVertexArray);
    functions.push_back(glDeleteVertexArrays);
    functions.push_back(glGenVertexArrays);
    functions.push_back(glIsVertexArray);
    result &= printExtensionSupported("VA objects are ", "GL_ARB_vertex_array_object", functions);

    functions.push_back(glBeginConditionalRenderNV);
    functions.push_back(glEndConditionalRenderNV);
    result &= printExtensionSupported("Cond render is ", "GL_NV_conditional_render", functions);

    functions.push_back(glClampColorARB);
    result &= printExtensionSupported("FP internal formats are ", "GL_ARB_color_buffer_float", functions);

    /*
        GL_NV_depth_buffer_float is from 2006 by nVidia to support floating-point values in textures
        by adding some new internal formats.
        GL_ARB_depth_buffer_float is for the same purpose, it was approved by ARB in 2008.
        The NV extension needs 3 special functions because the core functions clamp input values by
        default to [0,1] range.
        However the ARB extensions doesn't need special functions since core functionality got changed
        then also so input values are not clamped anymore to [0,1] range.
        Summary: both can be used, but NV's needs the special functions. Hence we "OR" to "result" here.
    */

    functions.push_back(glDepthRangedNV);
    functions.push_back(glClearDepthdNV);
    functions.push_back(glDepthBoundsdNV);
    bool bDepthBuffFloat = printExtensionSupported("NV depth buff float is ", "GL_NV_depth_buffer_float", functions);

    bDepthBuffFloat |= printExtensionSupported("ARB depth buff float is ", "GL_ARB_depth_buffer_float", functions);

    result &= bDepthBuffFloat;

    /* Following 3 extensions are not interchangeable, need to check separately and "AND" to "result". */

    result &= printExtensionSupported("FP internal formats are ", "GL_ARB_texture_float", functions);

    result &= printExtensionSupported("FP internal formats are ", "GL_EXT_packed_float", functions);

    result &= printExtensionSupported("FP internal formats are ", "GL_EXT_texture_shared_exponent", functions);

    /* GL_NV_half_float and GL_ARB_half_float_vertex are not interchangeable.
       GL_ARB_half_float_vertex adds some extra to GL_NV_half_float. The latter was introduced earlier. */

    functions.push_back(glVertex2hNV);
    functions.push_back(glVertex2hvNV);
    functions.push_back(glVertex3hNV);
    functions.push_back(glVertex3hvNV);
    functions.push_back(glVertex4hNV);
    functions.push_back(glVertex4hvNV);
    functions.push_back(glNormal3hNV);
    functions.push_back(glNormal3hvNV);
    functions.push_back(glColor3hNV);
    functions.push_back(glColor3hvNV);
    functions.push_back(glColor4hNV);
    functions.push_back(glColor4hvNV);
    functions.push_back(glTexCoord1hNV);
    functions.push_back(glTexCoord1hvNV);
    functions.push_back(glTexCoord2hNV);
    functions.push_back(glTexCoord2hvNV);
    functions.push_back(glTexCoord3hNV);
    functions.push_back(glTexCoord3hvNV);
    functions.push_back(glTexCoord4hNV);
    functions.push_back(glTexCoord4hvNV);
    functions.push_back(glMultiTexCoord1hNV);
    functions.push_back(glMultiTexCoord1hvNV);
    functions.push_back(glMultiTexCoord2hNV);
    functions.push_back(glMultiTexCoord2hvNV);
    functions.push_back(glMultiTexCoord3hNV);
    functions.push_back(glMultiTexCoord3hvNV);
    functions.push_back(glMultiTexCoord4hNV);
    functions.push_back(glMultiTexCoord4hvNV);
    functions.push_back(glFogCoordhNV);          /* available only if EXT_fog_coord (GL 1.4) is supported, we expect it here */
    functions.push_back(glFogCoordhvNV);
    functions.push_back(glSecondaryColor3hNV);   /* available only if EXT_secondary_color (GL 1.4) is supported, we expect it here */
    functions.push_back(glSecondaryColor3hvNV);
    /*
        EXT_vertex_weighting: NVIDIA no longer supports this extension in driver updates
        after November 2002. Instead, use either ARB_vertex_program & NV_vertex_program.
    */
    // functions.push_back(glVertexWeighthNV);      /* available only if EXT_vertex_weighting is supported, so we don't expect it anymore */
    // functions.push_back(glVertexWeighthvNV);
    functions.push_back(glVertexAttrib1hNV);
    functions.push_back(glVertexAttrib1hvNV);
    functions.push_back(glVertexAttrib2hNV);
    functions.push_back(glVertexAttrib2hvNV);
    functions.push_back(glVertexAttrib3hNV);
    functions.push_back(glVertexAttrib3hvNV);
    functions.push_back(glVertexAttrib4hNV);
    functions.push_back(glVertexAttrib4hvNV);
    functions.push_back(glVertexAttribs1hvNV);
    functions.push_back(glVertexAttribs2hvNV);
    functions.push_back(glVertexAttribs3hvNV);
    functions.push_back(glVertexAttribs4hvNV);
    result &= printExtensionSupported("NV Half-floats are ", "GL_NV_half_float", functions);

    result &= printExtensionSupported("Half-float vx formats are ", "GL_ARB_half_float_vertex", functions);

    /* GL_NV_half_float and GL_ARB_half_float_pixel are not interchangeable.
       GL_ARB_half_float_pixel adds some extra to GL_NV_half_float. The latter was introduced earlier. */

    result &= printExtensionSupported("Half-float px formats are ", "GL_ARB_half_float_pixel", functions);

    /* I saw this GL_EXT_half_float_pixel somewhere but cannot find much about it on the internet.
       I'm skipping it now. Maybe it was the unapproved version of GL_ARB_half_float_pixel. */
    // result &= printExtensionSupported("Half-float px formats are ", "GL_EXT_half_float_pixel", functions);

    functions.push_back(glTexParameterIivEXT);
    functions.push_back(glTexParameterIuivEXT);
    functions.push_back(glGetTexParameterIivEXT);
    functions.push_back(glGetTexParameterIuivEXT);
    functions.push_back(glClearColorIiEXT);
    functions.push_back(glClearColorIuiEXT);
    result &= printExtensionSupported("Integral img formats are ", "GL_EXT_texture_integer", functions);

    result &= printExtensionSupported("Array tex is ", "GL_EXT_texture_array", functions);

    functions.push_back(glColorMaskIndexedEXT);
    functions.push_back(glGetBooleanIndexedvEXT);
    functions.push_back(glGetIntegerIndexedvEXT);
    functions.push_back(glEnableIndexedEXT);
    functions.push_back(glDisableIndexedEXT);
    functions.push_back(glIsEnabledIndexedEXT);
    result &= printExtensionSupported("Per-clr-attach blend enables and clr wmasks are ", "GL_EXT_draw_buffers2", functions);

    bSuppRGTC = printExtensionSupported("Red-Green tex comp (ARB) is ", "GL_ARB_texture_compression_rgtc", functions);
    /* I did not find difference between GL_EXT_texture_compression_rgtc and GL_ARB_texture_compression_rgtc
       so I assume "OR" will be ok for decision ... */
    bSuppRGTC |= printExtensionSupported("Red-Green tex comp (EXT) is ", "GL_EXT_texture_compression_rgtc", functions);

    result &= bSuppRGTC;

    // Transform feedback
    // Evolved from NV_transform_feedback for which we don't check here.
    functions.push_back(glBeginTransformFeedbackEXT);
    functions.push_back(glEndTransformFeedbackEXT);
    functions.push_back(glBindBufferRangeEXT);
    functions.push_back(glBindBufferOffsetEXT);
    functions.push_back(glBindBufferBaseEXT);
    functions.push_back(glTransformFeedbackVaryingsEXT);
    functions.push_back(glGetTransformFeedbackVaryingEXT);
    bool bXformFB = printExtensionSupported("Xform feedback is ", "GL_EXT_transform_feedback", functions);

    if ( !bXformFB )
    {   // if GL_EXT_transform_feedback is not exposed, let's check for GL_EXT_transform_feedback2 which is superset of GL_EXT_transform_feedback
        functions.push_back(glBeginTransformFeedbackEXT);
        functions.push_back(glEndTransformFeedbackEXT);
        functions.push_back(glBindBufferRangeEXT);
        functions.push_back(glBindBufferOffsetEXT);  /* <- dont know why this is NULL for me at runtime ... */
        functions.push_back(glBindBufferBaseEXT);
        functions.push_back(glTransformFeedbackVaryingsEXT);
        functions.push_back(glGetTransformFeedbackVaryingEXT);
        bXformFB = printExtensionSupported("Xform feedback is ", "GL_EXT_transform_feedback2", functions);
    }

    result &= bXformFB;

    // Unclear about difference between GL_ARB_framebuffer_sRGB and GL_EXT_framebuffer_sRGB, so
    // let's treat them as exchangeable ...
    bool bFBsRGB = printExtensionSupported("sRGB FB mode is ", "GL_ARB_framebuffer_sRGB", functions);

    bFBsRGB |= printExtensionSupported("sRGB FB mode is ", "GL_EXT_framebuffer_sRGB", functions);

    result &= bFBsRGB;

    getConsole().O("Expected GLSL version is ");
    if ( (nVidVersionGLSLmaj > 3) || ( (nVidVersionGLSLmaj == 3) && (nVidVersionGLSLmin >= 0) ) )
    {
        getConsole().SOLn("supported!");
    }
    else
    {
        result &= false;
        getConsole().EOLn("NOT supported!");
    }

    getConsole().OO();

    return result;
} // discoverOpenGL_3_0_availability()


bool PRREhwVideoImpl::discoverOpenGL_3_1_availability()
{
    bool result = true;
    std::vector<void*> functions;

    getConsole().OI();

    functions.push_back(glDrawArraysInstanced);
    functions.push_back(glDrawElementsInstanced);
    functions.push_back(glTexBuffer);
    functions.push_back(glPrimitiveRestartIndex);
    result &= printExtensionSupported("Uniform buff objects are ", "ARB_uniform_buffer_object", functions);

    functions.push_back(glDrawArraysInstancedARB);
    functions.push_back(glDrawElementsInstancedARB);
    result &= printExtensionSupported("Instanced render is ", "GL_ARB_draw_instanced", functions);

    functions.push_back(glCopyBufferSubData);
    result &= printExtensionSupported("Data copy bw buff objects is ", "GL_ARB_copy_buffer", functions);

    // I couldn't find GL_EXT_copy_buffer definition in glext.h, maybe it has the same value as the ARB version, need to check ...
    //result &= printExtensionSupported("Data copying between buffer objects is ", "GL_EXT_copy_buffer", functions);

    functions.push_back(glPrimitiveRestartNV);
    functions.push_back(glPrimitiveRestartIndexNV);
    result &= printExtensionSupported("Primitive restart is ", "GL_NV_primitive_restart", functions);

    functions.push_back(glTexBufferARB);
    result &= printExtensionSupported("Buff tex is ", "GL_ARB_texture_buffer_object", functions);

    result &= printExtensionSupported("Rect tex is ", "GL_ARB_texture_rectangle", functions);

    getConsole().O("Expected GLSL version is ");
    if ( (nVidVersionGLSLmaj > 3) || ( (nVidVersionGLSLmaj == 3) && (nVidVersionGLSLmin >= 1) ) )
    {
        getConsole().SOLn("supported!");
    }
    else
    {
        result &= false;
        getConsole().EOLn("NOT supported!");
    }

    getConsole().OO();

    return result;
} // discoverOpenGL_3_1_availability()


bool PRREhwVideoImpl::discoverOpenGL_3_2_availability()
{
    bool result = true;
    std::vector<void*> functions;

    getConsole().OI();

    functions.push_back(glGetGraphicsResetStatusARB);
    functions.push_back(glGetnMapdvARB);
    functions.push_back(glGetnMapfvARB);
    functions.push_back(glGetnMapivARB);
    functions.push_back(glGetnPixelMapfvARB);
    functions.push_back(glGetnPixelMapuivARB);
    functions.push_back(glGetnPixelMapusvARB);
    functions.push_back(glGetnPolygonStippleARB);
    functions.push_back(glGetnColorTableARB);
    functions.push_back(glGetnConvolutionFilterARB);
    functions.push_back(glGetnSeparableFilterARB);
    functions.push_back(glGetnHistogramARB);
    functions.push_back(glGetnMinmaxARB);
    functions.push_back(glGetnTexImageARB);
    functions.push_back(glReadnPixelsARB);
    functions.push_back(glGetnCompressedTexImageARB);
    functions.push_back(glGetnUniformfvARB);
    functions.push_back(glGetnUniformivARB);
    functions.push_back(glGetnUniformuivARB);
    functions.push_back(glGetnUniformdvARB);
    result &= printExtensionSupported("API robustness is ", "GL_ARB_robustness", functions);

    result &= printExtensionSupported("D3D comp clr vx comp order is ", "GL_ARB_vertex_array_bgra", functions);

    functions.push_back(glDrawElementsBaseVertex);
    functions.push_back(glDrawRangeElementsBaseVertex);
    functions.push_back(glDrawElementsInstancedBaseVertex);
    functions.push_back(glMultiDrawElementsBaseVertex);
    result &= printExtensionSupported("Draw w/ modif base vertex ind is ", "GL_ARB_draw_elements_base_vertex", functions);

    result &= printExtensionSupported("Shader fragm coord convention ctrl is ", "GL_ARB_fragment_coord_conventions", functions);

    functions.push_back(glProvokingVertex);
    result &= printExtensionSupported("Provoking vx ctrl is ", "GL_ARB_provoking_vertex", functions);

    result &= printExtensionSupported("Seamless cube map filter is ", "GL_ARB_seamless_cube_map", functions);

    functions.push_back(glTexImage2DMultisample);
    functions.push_back(glTexImage3DMultisample);
    functions.push_back(glGetMultisamplefv);
    functions.push_back(glSampleMaski);
    result &= printExtensionSupported("Multisampled tex and tex samplers are ", "GL_ARB_texture_multisample", functions);

    result &= printExtensionSupported("Fragment depth clamp is ", "GL_ARB_depth_clamp", functions);

    functions.push_back(glFenceSync);
    functions.push_back(glIsSync);
    functions.push_back(glDeleteSync);
    functions.push_back(glClientWaitSync);
    functions.push_back(glWaitSync);
    functions.push_back(glGetInteger64v);
    functions.push_back(glGetSynciv);
    result &= printExtensionSupported("Fence sync objects are ", "GL_ARB_sync", functions);

    functions.push_back(glProgramParameteriARB);
    functions.push_back(glFramebufferTextureARB);
    functions.push_back(glFramebufferTextureLayerARB);
    functions.push_back(glFramebufferTextureFaceARB);
    result &= printExtensionSupported("GS are ", "GL_ARB_geometry_shader4", functions);

    getConsole().O("Expected GLSL version is ");
    if ( (nVidVersionGLSLmaj > 3) || ( (nVidVersionGLSLmaj == 3) && (nVidVersionGLSLmin >= 2) ) )
    {
        getConsole().SOLn("supported!");
    }
    else
    {
        result &= false;
        getConsole().EOLn("NOT supported!");
    }
    getConsole().OO();

    return result;
} // discoverOpenGL_3_2_availability()


bool PRREhwVideoImpl::discoverOpenGL_3_3_availability()
{
    bool result = true;
    std::vector<void*> functions;

    getConsole().OI();

    result &= printExtensionSupported("SL 3.30 is ", "GL_ARB_shader_bit_encoding", functions);

    functions.push_back(glBindFragDataLocationIndexed);
    functions.push_back(glGetFragDataIndex);
    result &= printExtensionSupported("Dual-src blending is ", "GL_ARB_blend_func_extended", functions);

    result &= printExtensionSupported("Shader-defined attr loc is ", "GL_ARB_explicit_attrib_location", functions);

    result &= printExtensionSupported("Simple bool Occlusion Query is ", "GL_ARB_occlusion_query2", functions);

    functions.push_back(glGenSamplers);
    functions.push_back(glDeleteSamplers);
    functions.push_back(glIsSampler);              
    functions.push_back(glBindSampler);            
    functions.push_back(glSamplerParameteri);
    functions.push_back(glSamplerParameteriv);
    functions.push_back(glSamplerParameterf);
    functions.push_back(glSamplerParameterfv);
    functions.push_back(glSamplerParameterIiv);
    functions.push_back(glSamplerParameterIuiv);
    functions.push_back(glGetSamplerParameteriv);
    functions.push_back(glGetSamplerParameterIiv);
    functions.push_back(glGetSamplerParameterfv);
    functions.push_back(glGetSamplerParameterIuiv);
    result &= printExtensionSupported("Sampler objects are ", "GL_ARB_sampler_objects", functions);

    result &= printExtensionSupported("Tex unsigned 10.10.10.2 is ", "GL_ARB_texture_rgb10_a2ui", functions);

    bool bTexSwizzle = printExtensionSupported("Tex swizzle ARB is ", "GL_ARB_texture_swizzle", functions);

    bTexSwizzle |= printExtensionSupported("Tex swizzle EXT is ", "GL_EXT_texture_swizzle", functions);

    result &= bTexSwizzle;

    functions.push_back(glQueryCounter);
    functions.push_back(glGetQueryObjecti64v);
    functions.push_back(glGetQueryObjectui64v);
    result &= printExtensionSupported("Timer queries are ", "GL_ARB_timer_query", functions);

    functions.push_back(glVertexAttribDivisorARB);
    result &= printExtensionSupported("Instanced arrays are ", "GL_ARB_instanced_arrays", functions);

    functions.push_back(glVertexP2ui);
    functions.push_back(glVertexP2uiv);
    functions.push_back(glVertexP3ui);
    functions.push_back(glVertexP3uiv);
    functions.push_back(glVertexP4ui);
    functions.push_back(glVertexP4uiv);
    functions.push_back(glTexCoordP1ui);
    functions.push_back(glTexCoordP1uiv);
    functions.push_back(glTexCoordP2ui);
    functions.push_back(glTexCoordP2uiv);
    functions.push_back(glTexCoordP3ui);
    functions.push_back(glTexCoordP3uiv);
    functions.push_back(glTexCoordP4ui);
    functions.push_back(glTexCoordP4uiv);
    functions.push_back(glMultiTexCoordP1ui);
    functions.push_back(glMultiTexCoordP1uiv);
    functions.push_back(glMultiTexCoordP2ui);
    functions.push_back(glMultiTexCoordP2uiv);
    functions.push_back(glMultiTexCoordP3ui);
    functions.push_back(glMultiTexCoordP3uiv);
    functions.push_back(glMultiTexCoordP4ui);
    functions.push_back(glMultiTexCoordP4uiv);
    functions.push_back(glNormalP3ui);
    functions.push_back(glNormalP3uiv);
    functions.push_back(glColorP3ui);
    functions.push_back(glColorP3uiv); 
    functions.push_back(glColorP4ui);
    functions.push_back(glColorP4uiv); 
    functions.push_back(glSecondaryColorP3ui);
    functions.push_back(glSecondaryColorP3uiv);
    functions.push_back(glVertexAttribP1ui);
    functions.push_back(glVertexAttribP1uiv);
    functions.push_back(glVertexAttribP2ui);
    functions.push_back(glVertexAttribP2uiv);
    functions.push_back(glVertexAttribP3ui);
    functions.push_back(glVertexAttribP3uiv);
    functions.push_back(glVertexAttribP4ui);
    functions.push_back(glVertexAttribP4uiv);
    result &= printExtensionSupported("Vertex attributes 2.10.10.10 are ", "GL_ARB_vertex_type_2_10_10_10_rev", functions);

    getConsole().O("Expected GLSL version is ");
    if ( (nVidVersionGLSLmaj > 3) || ( (nVidVersionGLSLmaj == 3) && (nVidVersionGLSLmin >= 3) ) )
    {
        getConsole().SOLn("supported!");
    }
    else
    {
        result &= false;
        getConsole().EOLn("NOT supported!");
    }

    getConsole().OO();

    return result;
} // discoverOpenGL_3_3_availability()


bool PRREhwVideoImpl::discoverOpenGL_4_0_availability()
{
    bool result = true;
    std::vector<void*> functions;

    getConsole().OI();

    result &= printExtensionSupported("Tex query LOD is ", "GL_ARB_texture_query_lod", functions);

    result &= printExtensionSupported("SL 4.0 ver is ", "GL_ARB_gpu_shader5", functions);

    functions.push_back(glUniform1d);
    functions.push_back(glUniform2d);
    functions.push_back(glUniform3d);
    functions.push_back(glUniform4d);
    functions.push_back(glUniform1dv);
    functions.push_back(glUniform2dv);
    functions.push_back(glUniform3dv);
    functions.push_back(glUniform4dv);
    functions.push_back(glUniformMatrix2dv);
    functions.push_back(glUniformMatrix3dv);
    functions.push_back(glUniformMatrix4dv);
    functions.push_back(glUniformMatrix2x3dv);
    functions.push_back(glUniformMatrix2x4dv);
    functions.push_back(glUniformMatrix3x2dv);
    functions.push_back(glUniformMatrix3x4dv);
    functions.push_back(glUniformMatrix4x2dv);
    functions.push_back(glUniformMatrix4x3dv);
    functions.push_back(glGetUniformdv);
    result &= printExtensionSupported("SL 4.0 FP64 is ", "GL_ARB_gpu_shader_fp64", functions);

    functions.push_back(glGetSubroutineUniformLocation);
    functions.push_back(glGetSubroutineIndex);
    functions.push_back(glGetActiveSubroutineUniformiv);
    functions.push_back(glGetActiveSubroutineUniformName);
    functions.push_back(glGetActiveSubroutineName);
    functions.push_back(glUniformSubroutinesuiv);
    functions.push_back(glGetUniformSubroutineuiv);
    functions.push_back(glGetProgramStageiv);
    result &= printExtensionSupported("SL 4.0 subroutines are ", "GL_ARB_shader_subroutine", functions);

    result &= printExtensionSupported("SL 4.0 tex gather is ", "ARB_texture_gather", functions);

    functions.push_back(glDrawArraysIndirect);
    functions.push_back(glDrawElementsIndirect);
    result &= printExtensionSupported("Draw inst arrays indirect is ", "GL_ARB_draw_indirect", functions);

    functions.push_back(glMinSampleShadingARB);
    result &= printExtensionSupported("Req min num of fragment inputs is ", "GL_ARB_sample_shading", functions);

    functions.push_back(glPatchParameteri);
    functions.push_back(glPatchParameterfv);
    result &= printExtensionSupported("Tessellation is ", "GL_ARB_tessellation_shader", functions);

    result &= printExtensionSupported("RGB32F, RGB32I, RGB32UI buff tex fmts are ", "GL_ARB_texture_buffer_object_rgb32", functions);

    result &= printExtensionSupported("Cube map array tex is ", "GL_ARB_texture_cube_map_array", functions);

    functions.push_back(glBindTransformFeedback);
    functions.push_back(glDeleteTransformFeedbacks);
    functions.push_back(glGenTransformFeedbacks);
    functions.push_back(glIsTransformFeedback);
    functions.push_back(glPauseTransformFeedback);
    functions.push_back(glResumeTransformFeedback);
    functions.push_back(glDrawTransformFeedback);
    result &= printExtensionSupported("Xform FB objects are ", "GL_ARB_transform_feedback2", functions);

    functions.push_back(glDrawTransformFeedbackStream);
    functions.push_back(glBeginQueryIndexed);
    functions.push_back(glEndQueryIndexed);
    functions.push_back(glGetQueryIndexediv);
    result &= printExtensionSupported("Mult Xform FB stream outputs are ", "GL_ARB_transform_feedback3", functions);

    functions.push_back(glBlendEquationiARB);
    functions.push_back(glBlendEquationSeparateiARB);
    functions.push_back(glBlendFunciARB);
    functions.push_back(glBlendFuncSeparateiARB);
    result &= printExtensionSupported("Indi blend equ for each clr output are ", "GL_ARB_draw_buffers_blend", functions);

    getConsole().O("Expected GLSL version is ");
    if ( (nVidVersionGLSLmaj > 4) || ( (nVidVersionGLSLmaj == 4) && (nVidVersionGLSLmin >= 0) ) )
    {
        getConsole().SOLn("supported!");
    }
    else
    {
        result &= false;
        getConsole().EOLn("NOT supported!");
    }

    getConsole().OO();

    return result;
} // discoverOpenGL_4_0_availability()


bool PRREhwVideoImpl::discoverOpenGL_4_1_availability()
{
    bool result = true;
    std::vector<void*> functions;

    getConsole().OI();

    functions.push_back(glGetProgramBinary);
    functions.push_back(glProgramBinary);
    functions.push_back(glProgramParameteri);
    result &= printExtensionSupported("Query and load a bin blob for prg objects is ", "GL_ARB_get_program_binary", functions);

    functions.push_back(glUseProgramStages);
    functions.push_back(glActiveShaderProgram);
    functions.push_back(glCreateShaderProgramv);
    functions.push_back(glBindProgramPipeline);
    functions.push_back(glDeleteProgramPipelines);
    functions.push_back(glGenProgramPipelines);
    functions.push_back(glIsProgramPipeline);
    functions.push_back(glGetProgramPipelineiv);
    functions.push_back(glProgramUniform1i);
    functions.push_back(glProgramUniform1iv);
    functions.push_back(glProgramUniform1f);
    functions.push_back(glProgramUniform1fv);
    functions.push_back(glProgramUniform1d);
    functions.push_back(glProgramUniform1dv);
    functions.push_back(glProgramUniform1ui);
    functions.push_back(glProgramUniform1uiv);
    functions.push_back(glProgramUniform2i);
    functions.push_back(glProgramUniform2iv);
    functions.push_back(glProgramUniform2f);
    functions.push_back(glProgramUniform2fv);
    functions.push_back(glProgramUniform2d);
    functions.push_back(glProgramUniform2dv);
    functions.push_back(glProgramUniform2ui);
    functions.push_back(glProgramUniform2uiv);
    functions.push_back(glProgramUniform3i);
    functions.push_back(glProgramUniform3iv);
    functions.push_back(glProgramUniform3f);
    functions.push_back(glProgramUniform3fv);
    functions.push_back(glProgramUniform3d);
    functions.push_back(glProgramUniform3dv);
    functions.push_back(glProgramUniform3ui);
    functions.push_back(glProgramUniform3uiv);
    functions.push_back(glProgramUniform4i);
    functions.push_back(glProgramUniform4iv);
    functions.push_back(glProgramUniform4f);
    functions.push_back(glProgramUniform4fv);
    functions.push_back(glProgramUniform4d);
    functions.push_back(glProgramUniform4dv);
    functions.push_back(glProgramUniform4ui);
    functions.push_back(glProgramUniform4uiv);
    functions.push_back(glProgramUniformMatrix2fv);
    functions.push_back(glProgramUniformMatrix3fv);
    functions.push_back(glProgramUniformMatrix4fv);
    functions.push_back(glProgramUniformMatrix2dv);
    functions.push_back(glProgramUniformMatrix3dv);
    functions.push_back(glProgramUniformMatrix4dv);
    functions.push_back(glProgramUniformMatrix2x3fv);
    functions.push_back(glProgramUniformMatrix3x2fv);
    functions.push_back(glProgramUniformMatrix2x4fv);
    functions.push_back(glProgramUniformMatrix4x2fv);
    functions.push_back(glProgramUniformMatrix3x4fv);
    functions.push_back(glProgramUniformMatrix4x3fv);
    functions.push_back(glProgramUniformMatrix2x3dv);
    functions.push_back(glProgramUniformMatrix3x2dv);
    functions.push_back(glProgramUniformMatrix2x4dv);
    functions.push_back(glProgramUniformMatrix4x2dv);
    functions.push_back(glProgramUniformMatrix3x4dv);
    functions.push_back(glProgramUniformMatrix4x3dv);
    functions.push_back(glValidateProgramPipeline);
    functions.push_back(glGetProgramPipelineInfoLog);
    result &= printExtensionSupported("Bind prg indi to prog stages is ", "GL_ARB_separate_shader_objects", functions);

    functions.push_back(glReleaseShaderCompiler);
    functions.push_back(glShaderBinary);
    functions.push_back(glGetShaderPrecisionFormat);
    functions.push_back(glDepthRangef);
    functions.push_back(glClearDepthf);
    result &= printExtensionSupported("Func from OpenGL ES 2.0 is ", "GL_ARB_ES2_compatibility", functions);

    result &= printExtensionSupported("Prec reqs for sev FP ops are ", "GL_ARB_shader_precision", functions);

    result &= printExtensionSupported("64-bit FP comp VS inputs are ", "GL_ARB_vertex_attrib_64bit", functions);

    functions.push_back(glViewportArrayv);
    functions.push_back(glViewportIndexedf);
    functions.push_back(glViewportIndexedfv);
    functions.push_back(glScissorArrayv);
    functions.push_back(glScissorIndexed);
    functions.push_back(glScissorIndexedv);
    functions.push_back(glDepthRangeArrayv);
    functions.push_back(glDepthRangeIndexed);
    functions.push_back(glGetFloati_v);
    functions.push_back(glGetDoublei_v);
    result &= printExtensionSupported("Multi viewports for GS are ", "GL_ARB_viewport_array", functions);

    getConsole().O("Expected GLSL version is ");
    if ( (nVidVersionGLSLmaj > 4) || ( (nVidVersionGLSLmaj == 4) && (nVidVersionGLSLmin >= 1) ) )
    {
        getConsole().SOLn("supported!");
    }
    else
    {
        result &= false;
        getConsole().EOLn("NOT supported!");
    }

    getConsole().OO();

    return result;
} // discoverOpenGL_4_1_availability()


bool PRREhwVideoImpl::discoverOpenGL_4_2_availability()
{
    bool result = true;
    std::vector<void*> functions;

    getConsole().OI();

    functions.push_back(glGetActiveAtomicCounterBufferiv);
    result &= printExtensionSupported("Atomic counters are ", "GL_ARB_shader_atomic_counters", functions);

    functions.push_back(glBindImageTexture);
    functions.push_back(glMemoryBarrier);
    result &= printExtensionSupported("R/W images by shaders are ", "GL_ARB_shader_image_load_store", functions);

    functions.push_back(glTexStorage1D);
    functions.push_back(glTexStorage2D);
    functions.push_back(glTexStorage3D);
    functions.push_back(glTextureStorage1DEXT);
    functions.push_back(glTextureStorage2DEXT);
    functions.push_back(glTextureStorage3DEXT);
    result &= printExtensionSupported("Immu storage for tex object is ", "GL_ARB_texture_storage", functions);

    functions.push_back(glDrawTransformFeedbackInstanced);
    functions.push_back(glDrawTransformFeedbackStreamInstanced);
    result &= printExtensionSupported("Inst'd render from Xform FB is ", "GL_ARB_transform_feedback_instanced", functions);

    result &= printExtensionSupported("GLSL 4.2 feature pack is ", "GL_ARB_shading_language_420pack", functions);

    functions.push_back(glDrawArraysInstancedBaseInstance);
    functions.push_back(glDrawElementsInstancedBaseInstance);
    functions.push_back(glDrawElementsInstancedBaseVertexBaseInstance);
    result &= printExtensionSupported("Inst'd render from starting inst is ", "GL_ARB_base_instance", functions);

    functions.push_back(glGetInternalformativ);
    result &= printExtensionSupported("Get max num of samples by img fmt and tex type is ", "GL_ARB_internalformat_query", functions);

    result &= printExtensionSupported("Xfer a sub-rect of compr tex is ", "GL_ARB_compressed_texture_pixel_storage", functions);

    result &= printExtensionSupported("Unpacking 16-bit floats from 32-bit int is ", "GL_ARB_shading_language_packing", functions);

    result &= printExtensionSupported("Min 64-byte-alignment for buff mapping is ", "GL_ARB_map_buffer_alignment", functions);

    result &= printExtensionSupported("Enhanced early depth test by defining FS is ", "GL_ARB_conservative_depth", functions);

    result &= printExtensionSupported("BPTC and BPTC_FLOAT are ", "GL_ARB_texture_compression_bptc", functions);

    getConsole().O("Expected GLSL version is ");
    if ( (nVidVersionGLSLmaj > 4) || ( (nVidVersionGLSLmaj == 4) && (nVidVersionGLSLmin >= 2) ) )
    {
        getConsole().SOLn("supported!");
    }
    else
    {
        result &= false;
        getConsole().EOLn("NOT supported!");
    }

    getConsole().OO();

    return result;
} // discoverOpenGL_4_2_availability()


bool PRREhwVideoImpl::discoverOpenGL_4_3_availability()
{
    bool result = true;
    std::vector<void*> functions;

    getConsole().OI();

    functions.push_back(glDebugMessageControl); 
    functions.push_back(glDebugMessageInsert);
    functions.push_back(glDebugMessageCallback);
    functions.push_back(glGetDebugMessageLog);
    functions.push_back(glPushDebugGroup);
    functions.push_back(glPopDebugGroup);
    functions.push_back(glObjectLabel);
    functions.push_back(glGetObjectLabel);
    functions.push_back(glObjectPtrLabel);
    functions.push_back(glGetObjectPtrLabel);
    result &= printExtensionSupported("Debug messaging is ", "GL_KHR_debug", functions);

    result &= printExtensionSupported("Multidim arrays are ", "GL_ARB_arrays_of_arrays", functions);

    functions.push_back(glClearBufferData);
    functions.push_back(glClearBufferSubData);
    functions.push_back(glClearNamedBufferDataEXT);
    functions.push_back(glClearNamedBufferSubDataEXT);
    result &= printExtensionSupported("Memset-style buff object clear is ", "GL_ARB_clear_buffer_object", functions);

    functions.push_back(glDispatchCompute);
    functions.push_back(glDispatchComputeIndirect);
    result &= printExtensionSupported("CS are ", "GL_ARB_compute_shader", functions);

    functions.push_back(glCopyImageSubData);
    result &= printExtensionSupported("Efficient image copy b/w img buffs is ", "GL_ARB_copy_image", functions);

    result &= printExtensionSupported("OpenGL ES 3.0 comp is ", "GL_ARB_ES3_compatibility", functions);

    result &= printExtensionSupported("Specify uniform loc in shader is ", "GL_ARB_explicit_uniform_location", functions);

    result &= printExtensionSupported("Lyr and viewp indices avail from FS is ", "GL_ARB_fragment_layer_viewport", functions);

    functions.push_back(glFramebufferParameteri);
    functions.push_back(glGetFramebufferParameteriv);
    functions.push_back(glNamedFramebufferParameteriEXT);
    functions.push_back(glGetNamedFramebufferParameterivEXT);
    result &= printExtensionSupported("Render to FBO having no attach's is ", "GL_ARB_framebuffer_no_attachments", functions);

    functions.push_back(glGetInternalformati64v);
    result &= printExtensionSupported("Gen'd queries about img fmts are ", "GL_ARB_internalformat_query2", functions);

    functions.push_back(glInvalidateTexSubImage);
    functions.push_back(glInvalidateTexImage);
    functions.push_back(glInvalidateBufferSubData); 
    functions.push_back(glInvalidateBufferData);
    functions.push_back(glInvalidateFramebuffer);
    functions.push_back(glInvalidateSubFramebuffer);
    result &= printExtensionSupported("Tex, buff object, and FB inv'ion are ", "GL_ARB_invalidate_subdata", functions);

    functions.push_back(glMultiDrawArraysIndirect);
    functions.push_back(glMultiDrawElementsIndirect);
    result &= printExtensionSupported("Multi indir render cmds by 1 draw cmd is ", "GL_ARB_multi_draw_indirect", functions);

    functions.push_back(glGetProgramInterfaceiv);
    functions.push_back(glGetProgramResourceIndex);
    functions.push_back(glGetProgramResourceName);
    functions.push_back(glGetProgramResourceiv);
    functions.push_back(glGetProgramResourceLocation);
    functions.push_back(glGetProgramResourceLocationIndex);
    result &= printExtensionSupported("API to get info about prg object IF is ", "GL_ARB_program_interface_query", functions);

    result &= printExtensionSupported("Get size of img from GLSL is ", "GL_ARB_shader_image_size", functions);

    functions.push_back(glShaderStorageBlockBinding);
    result &= printExtensionSupported("Buff object r/w from shader is ", "GL_ARB_shader_storage_buffer_object", functions);

    result &= printExtensionSupported("Stencil values from depth/stencil tex is ", "GL_ARB_stencil_texturing", functions);

    functions.push_back(glTexBufferRange);
    functions.push_back(glTextureBufferRangeEXT);
    result &= printExtensionSupported("Buff tex bound to a range of a buff object is ", "GL_ARB_texture_buffer_range", functions);

    result &= printExtensionSupported("Detect of avail mipmap pyramid by GLSL is ", "GL_ARB_texture_query_levels", functions);

    functions.push_back(glTexStorage2DMultisample);
    functions.push_back(glTexStorage3DMultisample);
    functions.push_back(glTextureStorage2DMultisampleEXT);
    functions.push_back(glTextureStorage3DMultisampleEXT);
    result &= printExtensionSupported("Immu storage for multisample tex is ", "GL_ARB_texture_storage_multisample", functions);

    functions.push_back(glTextureView);
    result &= printExtensionSupported("Tex views are ", "GL_ARB_texture_view", functions);

    functions.push_back(glBindVertexBuffer);
    functions.push_back(glVertexAttribFormat);
    functions.push_back(glVertexAttribIFormat);
    functions.push_back(glVertexAttribLFormat);
    functions.push_back(glVertexAttribBinding);
    functions.push_back(glVertexBindingDivisor);
    functions.push_back(glVertexArrayBindVertexBufferEXT);
    functions.push_back(glVertexArrayVertexAttribFormatEXT);
    functions.push_back(glVertexArrayVertexAttribIFormatEXT); 
    functions.push_back(glVertexArrayVertexAttribLFormatEXT); 
    functions.push_back(glVertexArrayVertexAttribBindingEXT); 
    functions.push_back(glVertexArrayVertexBindingDivisorEXT);
    result &= printExtensionSupported("Separ of vx fmt from buff object is ", "GL_ARB_vertex_attrib_binding", functions);

    result &= printExtensionSupported("API robust. (buff acc) is ", "GL_ARB_robust_buffer_access_behavior", functions);

    // We don't check for ARB_robustness_isolation because it is advertised by driver only if implementation supports it AND
    // if the context was created with the specific reset isolation bit set! We don't set that bit during init so cannot expect.
    // result &= printExtensionSupported("API robust. (GL isolation) is ", "GL_ARB_robustness_isolation", functions);
    // result &= printExtensionSupported("API robust. (WGL isolation) is ", "WGL_ARB_robustness_isolation", functions);
    // result &= printExtensionSupported("API robust. (GLX isolation) is ", "GLX_ARB_robustness_isolation", functions);

    getConsole().O("Expected GLSL version is ");
    if ( (nVidVersionGLSLmaj > 4) || ( (nVidVersionGLSLmaj == 4) && (nVidVersionGLSLmin >= 3) ) )
    {
        getConsole().SOLn("supported!");
    }
    else
    {
        result &= false;
        getConsole().EOLn("NOT supported!");
    }

    getConsole().OO();

    return result;
} // discoverOpenGL_4_3_availability()


void PRREhwVideoImpl::DiscoverOpenGL_X_Y_availability(const char* text, bool (PRREhwVideoImpl::*f)())
{
    if ( !f )
        return;

    getConsole().OLn("Testing for OpenGL %s features ...", text);
    if ( (this->*(f))() )
        getConsole().SOLn("%s features OK!", text);
    else
        getConsole().EOLn("%s features NOK!", text);
    getConsole().OLn("");
}


void PRREhwVideoImpl::DiscoverOpenGLavailability()
{
    if ( bAlreadyInitializedOnceOGL )
    {
        getConsole().OLn("Discovery has been already done!");
        getConsole().OLn("");
        return;
    }

    glGetIntegerv(GL_MAJOR_VERSION, &(tmpRet[0]));
    glGetIntegerv(GL_MINOR_VERSION, &(tmpRet[1]));
    getConsole().OLn("Reported OpenGL version: %d.%d", tmpRet[0], tmpRet[1]);

    // although we queried reported opengl version, we also would like to know
    // if driver really supports features it should support per version.
    // Neither GLEE nor GLEW does this per-version feature check, so
    // I implement it. Sometimes drivers do not really follow the standard.
    getConsole().OLn("");
    getConsole().OLn("Checking for per-version features ...");
    getConsole().OLn("");

    DiscoverOpenGL_X_Y_availability("1.1 (1997)", &PRREhwVideoImpl::discoverOpenGL_1_1_availability);
    DiscoverOpenGL_X_Y_availability("1.2 (1998)", &PRREhwVideoImpl::discoverOpenGL_1_2_availability);
    DiscoverOpenGL_X_Y_availability("1.3 (2001)", &PRREhwVideoImpl::discoverOpenGL_1_3_availability);
    DiscoverOpenGL_X_Y_availability("1.4 (2002)", &PRREhwVideoImpl::discoverOpenGL_1_4_availability);
    DiscoverOpenGL_X_Y_availability("1.5 (2003)", &PRREhwVideoImpl::discoverOpenGL_1_5_availability);
    DiscoverOpenGL_X_Y_availability("2.0 (2004)", &PRREhwVideoImpl::discoverOpenGL_2_0_availability);
    DiscoverOpenGL_X_Y_availability("2.1 (2006)", &PRREhwVideoImpl::discoverOpenGL_2_1_availability);
    DiscoverOpenGL_X_Y_availability("3.0 (2008)", &PRREhwVideoImpl::discoverOpenGL_3_0_availability);
    DiscoverOpenGL_X_Y_availability("3.1 (2009)", &PRREhwVideoImpl::discoverOpenGL_3_1_availability);
    DiscoverOpenGL_X_Y_availability("3.2 (2009)", &PRREhwVideoImpl::discoverOpenGL_3_2_availability);
    DiscoverOpenGL_X_Y_availability("3.3 (2010)", &PRREhwVideoImpl::discoverOpenGL_3_3_availability);
    DiscoverOpenGL_X_Y_availability("4.0 (2010)", &PRREhwVideoImpl::discoverOpenGL_4_0_availability);
    DiscoverOpenGL_X_Y_availability("4.1 (2010)", &PRREhwVideoImpl::discoverOpenGL_4_1_availability);
    DiscoverOpenGL_X_Y_availability("4.2 (2011)", &PRREhwVideoImpl::discoverOpenGL_4_2_availability);
    DiscoverOpenGL_X_Y_availability("4.3 (2012)", &PRREhwVideoImpl::discoverOpenGL_4_3_availability);

    DiscoverFramebufferAndClipping();
    DiscoverViewportAvailability();
    DiscoverFSAAavailability(hWndDC);
    DiscoverMatrixStacksAvailability();
    DiscoverVertexRenderingAvailability();
    DiscoverTexturingAvailability();
} // DiscoverOpenGLavailability()




/*
   PRREhwVideo
   ###########################################################################
*/


// ############################### PUBLIC ################################


/** 
    Gets the singleton instance.
*/
PRREhwVideo& PRREhwVideo::get()
{
    return PRREhwVideoImpl::get();
} // get()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################