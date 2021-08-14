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
#include "../../include/external/Hardware/PRREhwVideo.h"
#include "../../include/internal/PRREGLsnippets.h"
#include "../../include/internal/PRREGLextensionFuncs.h"
#include "../../include/internal/PRREpragmas.h"
#include "../../include/internal/Display/PRRESharedSettings.h"
#include "../../include/internal/Hardware/PRREhwVideoDiscoverOpenGL_1_1.h"
#include "../../include/internal/Hardware/PRREhwVideoDiscoverOpenGL_1_2.h"
#include "../../include/internal/Hardware/PRREhwVideoDiscoverOpenGL_1_3.h"
#include "../../include/internal/Hardware/PRREhwVideoDiscoverOpenGL_1_4.h"
#include "../../include/internal/Hardware/PRREhwVideoDiscoverOpenGL_1_5.h"
#include "../../include/internal/Hardware/PRREhwVideoDiscoverOpenGL_2_0.h"
#include "../../include/internal/Hardware/PRREhwVideoDiscoverOpenGL_2_1.h"
#include "../../include/internal/Hardware/PRREhwVideoDiscoverOpenGL_3_0.h"
#include "../../include/internal/Hardware/PRREhwVideoDiscoverOpenGL_3_1.h"
#include "../../include/internal/Hardware/PRREhwVideoDiscoverOpenGL_3_2.h"
#include "../../include/internal/Hardware/PRREhwVideoDiscoverOpenGL_3_3.h"
#include "../../include/internal/Hardware/PRREhwVideoDiscoverOpenGL_4_0.h"
#include "../../include/internal/Hardware/PRREhwVideoDiscoverOpenGL_4_1.h"
#include "../../include/internal/Hardware/PRREhwVideoDiscoverOpenGL_4_2.h"
#include "../../include/internal/Hardware/PRREhwVideoDiscoverOpenGL_4_3.h"

using namespace std;

class PRREhwVideoImpl :
    public PRREhwVideo
{

public:
    static PRREhwVideoImpl& get();

    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    CConsole&  getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */

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
    TPRREbool  isOcclusionQuerySupported() const;
    TPRREbool  isBooleanOcclusionQuerySupported() const;
    const std::string& getVideocardOSName() const;          
    const std::string& getVideocardOGLName() const;        
    const std::string& getVideocardOGLVersion() const;
    const std::string  getVideocardGLSLVersion() const;
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

    static PRREhwVideoImpl hwVideoInstance;
    static HGLRC hGLRC;                          /**< OpenGL rendering context handle. */
    static HDC   hWndDC;                         /**< Our main window's device context. */
    static TPRREuint nResX, nResY;               /**< Display resolution. */      
    static TPRREint  nColorBits, nDepthBits;     /**< Display color depth, ZBuffer-depth. */
    static TPRREint  nStencilBits, nFSAAlevel;   /**< Stencil Buffer depth, FSAA-level. */

    static bool      bAlreadyInitializedOnceOGL; /**< True if OGL-initialization has been done once already.
                                                      This is to speed up consecutive stop-start initializations with same OGL renderer. */
    static TPRREbool bSuppAnisoFiltering;        /**< Is anisotropic filtering available? */
    static GLfloat   nAnisoFilterMax;            /**< Maximum level of anisotropic filtering. */
    static TPRREbool bSuppHWmipmapGen;           /**< Is HW MIP map generation available? */
    static TPRREbool bSuppCompiledVertexArrays;  /**< Is CompiledVertexArrays supported? */
    static TPRREbool bSuppNVVertexArrayRange;    /**< Is the nVidia-specific Vertex Array Range supported? */
    static TPRREbool bSuppNVVertexArrayRange2;   /**< Is the nVidia-specific Vertex Array Range 2 supported? */
    static TPRREbool bSuppNVElementArray;        /**< Is the nVidia-specific Element Array supported? */
    static TPRREbool bSuppATIVertexArrayObject;  /**< Is the ATI-specific Vertex Array Object supported? */
    static TPRREbool bSuppATIMapObjectBuffer;    /**< Is the ATI-specific MapObjectBuffer (for Vertex Array Object) supported? */
    static TPRREbool bSuppATIElementArray;       /**< Is the ATI-specific Element Array supported? */
    static TPRREbool bSuppATITruForm;            /**< Is the ATI-specific TruForm supported? */

    static std::string sVidNameWin;              /**< VGA name based on Windows. */
    static std::string sVidNameOGL;              /**< VGA name based on OpenGL. */
    static std::string sVidVendorOGL;            /**< VGA vendor based on OpenGL. */
    static std::string sVidFeaturesOGL;          /**< VGA features based on OGL. */
    static std::string sVidFeaturesWGL;          /**< VGA features based on WGL. */
    static std::string sVidVersionOGL;           /**< OpenGL version. */
    static TPRREuint   nVidVersionGLSLmaj;       /**< GLSL major version. */
    static TPRREuint   nVidVersionGLSLmin;       /**< GLSL minor version. */

    // ---------------------------------------------------------------------------

    PRRESharedSettings& sharedSettings;  /**< Pointer to shared settings. */

    GLint   tmpRet[6];                   /**< Hold for glGetInteger...() results. */
    GLfloat tmpRetf[2];                  /**< Hold for glGetFloat...() results. */

    DISPLAY_DEVICEA dispdev;             /**< To query the Windows name of the VGA. */

    std::vector<PRREhwVideoDiscoverOpenGLbase*> discoverOpenGLversions;  // ah please use C++11, make this const and initialize here or in ctor initializes list!!!
    PRREhwVideoDiscoverOpenGL_1_1 discoverGL_1_1;
    PRREhwVideoDiscoverOpenGL_1_2 discoverGL_1_2;
    PRREhwVideoDiscoverOpenGL_1_3 discoverGL_1_3;
    PRREhwVideoDiscoverOpenGL_1_4 discoverGL_1_4;
    PRREhwVideoDiscoverOpenGL_1_5 discoverGL_1_5;
    PRREhwVideoDiscoverOpenGL_2_0 discoverGL_2_0;
    PRREhwVideoDiscoverOpenGL_2_1 discoverGL_2_1;
    PRREhwVideoDiscoverOpenGL_3_0 discoverGL_3_0;
    PRREhwVideoDiscoverOpenGL_3_1 discoverGL_3_1;
    PRREhwVideoDiscoverOpenGL_3_2 discoverGL_3_2;
    PRREhwVideoDiscoverOpenGL_3_3 discoverGL_3_3;
    PRREhwVideoDiscoverOpenGL_4_0 discoverGL_4_0;
    PRREhwVideoDiscoverOpenGL_4_1 discoverGL_4_1;
    PRREhwVideoDiscoverOpenGL_4_2 discoverGL_4_2;
    PRREhwVideoDiscoverOpenGL_4_3 discoverGL_4_3;

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

    void printExtensionList(
        const std::string& features,
         const char* endstring);         /**< Writes the given extensions list to console. */           
    void printOGLextensions();           /**< Writes OGL extension list to console. */
    void printWGLextensions();           /**< Writes WGL extension list to console. */

    void DiscoverViewportAvailability();
    void DiscoverMatrixStacksAvailability();
    void DiscoverFramebufferAndClipping();
    void DiscoverCompiledVertexArraysAvailability();
    void DiscoverNVVertexArrayRangeAvailability();
    void DiscoverNVElementArrayAvailability();
    void DiscoverATIVertexArrayObjectAvailability();
    void DiscoverATIMapObjectBufferAvailability();
    void DiscoverATIElementArrayAvailability();
    void DiscoverATITruFormAvailability();
    void DiscoverVertexRenderingAvailability();
    void DiscoverAnisoFilteringAvailability();
    void DiscoverTexturingAvailability();
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
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& PRREhwVideoImpl::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* PRREhwVideoImpl::getLoggerModuleName()
{
    return "PRREhwVideo";
} // getLoggerModuleName()


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
    return sharedSettings.get(PRRE_SSET_VSYNC_SUPPORTED);
} // isVSyncSupported()


/**
    Gets whether multitexturing is supported or not.
    @return True if multitexturing is supported, false otherwise.
*/
TPRREbool PRREhwVideoImpl::isMultiTexturingSupported() const
{
    return isInitialized() && discoverGL_1_3.isMultiTexturingSupported();
} // isMultiTexturingSupported()


/**
    Gets the number of texture units.
    @return The number of texture units.
*/
TPRREint PRREhwVideoImpl::getTextureUnitsCount() const
{
    return isInitialized() ? discoverGL_1_3.getTextureUnitsCount() : (TPRREint) 0;
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
    return isInitialized() && discoverGL_1_2.isNativeDIBFormatSupported();
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
    return isInitialized() && discoverGL_1_3.isTextureCompressionSupported();
} // isTextureCompressionSupported()


/**
    Gets whether Volume Texture Compression is supported or not.
    @return True if Volume Texture Compression is supported, otherwise false.
*/
TPRREbool PRREhwVideoImpl::isVTCSupported() const
{
    return isInitialized() && discoverGL_1_3.isVTCSupported();
} // isVTCSupported()


/**
    Gets whether 3Dc/ATI2/DXN compression is supported or not.
    @return True if 3Dc/ATI2/DXN compression is supported, otherwise false.
*/
TPRREbool PRREhwVideoImpl::is3DcSupported() const
{
    return isInitialized() && discoverGL_1_3.is3DcSupported();
} // is3DcSupported()


/**
    Gets whether Red-Green compression is supported or not.
    @return True if Red-Green compression is supported, otherwise false.
*/
TPRREbool PRREhwVideoImpl::isRGTCSupported() const
{
    return isInitialized() && discoverGL_1_3.isLATCSupported();
} // isRGTCSupported()


/**
    Gets whether Luminance-Alpha compression is supported or not.
    @return True if Luminance-Alpha compression is supported, otherwise false.
*/
TPRREbool PRREhwVideoImpl::isLATCSupported() const
{
    return isInitialized() && discoverGL_1_3.isLATCSupported();
} // isLATCsupported()


/**
    Gets whether advanced pixel formats supported or not.
    Advanced pixel formats are required for various extra features like MSAA.
*/
TPRREbool PRREhwVideoImpl::isAdvancedPixelFormatSetSupported() const
{
    return wglGetPixelFormatAttribivARB && wglGetPixelFormatAttribfvARB && wglChoosePixelFormatARB &&
        PRREhwVideoDiscoverOpenGLbase::isExtensionSupported("WGL_ARB_pixel_format", sVidFeaturesOGL, sVidFeaturesWGL);
} // isAdvancedPixelFormatSetSupported()


/** 
    Gets whether FSAA is supported or not.
    @return True if FSAA is supported, otherwise false.
*/
TPRREbool PRREhwVideoImpl::isFullSceneAntiAliasingSupported() const
{
    return isInitialized() && discoverGL_1_3.isFullSceneAntiAliasingSupported();
} // isFullSceneAntiAliasingSupported()


/**
    Gets the maximum FSAA level.
    @return The maximum available FSAA-level.
*/
TPRREint PRREhwVideoImpl::getMaxSamplesCount() const
{
    return isInitialized() ? discoverGL_1_3.getMaxSamplesCount() : (TPRREint) 0;
} // getMaxSamplesCount()


/**
    Gets whether the given MSAA-level is supported or not.
    @return True if the given MSAA-level is supported, otherwise false.
*/
TPRREbool PRREhwVideoImpl::isSamplesSupported(TPRREint numsamples) const
{
    return isInitialized() && discoverGL_1_3.isSamplesSupported(numsamples);
} // isSamplesSupported()


/**
    Gets whether MultiDrawArrays is supported or not.
*/
TPRREbool PRREhwVideoImpl::isMultiDrawArraysSupported() const
{
    return isInitialized() && discoverGL_1_4.isMultiDrawArraysSupported();
} // isMultiDrawArraysSupported()


/**
    Gets whether DrawRangeElements is supported or not.
*/
TPRREbool PRREhwVideoImpl::isDrawRangeElementsSupported() const
{
    return isInitialized() && discoverGL_1_2.isDrawRangeElementsSupported();
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
    return isInitialized() && discoverGL_1_5.isVertexBufferObjectSupported();
} // isVertexBufferObjectSupported()


/**
    Gets whether the ATI-specific TruForm is supported.
*/
TPRREbool PRREhwVideoImpl::isATITruFormSupported() const
{
    return isInitialized() && bSuppATITruForm;
} // isATITruFormSupported()


/**
    Gets whether HW occlusion culling is supported.
*/
TPRREbool PRREhwVideoImpl::isOcclusionQuerySupported() const
{
    return isInitialized() && discoverGL_1_5.isOcclusionQuerySupported();
} // isOcclusionQuerySupported()


/**
    Gets whether HW boolean occlusion culling is supported.
*/
TPRREbool PRREhwVideoImpl::isBooleanOcclusionQuerySupported() const
{
    return isInitialized() && discoverGL_3_3.isBooleanOcclusionQuerySupported();
} // isBooleanOcclusionQuerySupported()


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
const string PRREhwVideoImpl::getVideocardGLSLVersion() const
{
    const std::string sVer = std::to_string(nVidVersionGLSLmaj) + "." + std::to_string(nVidVersionGLSLmin);
    return isInitialized() ? sVer : "";
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
    return isInitialized() ? sVidFeaturesOGL : sEmpty;
} // getVideocardOGLFeatureList()


/**
    Gets the WGL features supported by the driver.
    @return The WGL features supported by the driver.
*/
const string& PRREhwVideoImpl::getVideocardWGLFeatureList() const 
{
    static const std::string sEmpty = "";
    return isInitialized() ? sVidFeaturesWGL : sEmpty;
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
    sharedSettings.Set(PRRE_SSET_VSYNC_SUPPORTED, false);
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
        sharedSettings.Set(PRRE_SSET_VSYNC_SUPPORTED, false);
        getConsole().OO();
        getConsole().OO();

        return true;
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
            std::string sVidVersionGLSL = (char*) ptmp;
            sscanf((const char*) ptmp, "%d.%d", &nVidVersionGLSLmaj, &nVidVersionGLSLmin);
            getConsole().OLn("glslVersion = %s (%d.%d)", sVidVersionGLSL.c_str(), nVidVersionGLSLmaj, nVidVersionGLSLmin);
        }
        else
        {
            getConsole().EOLn("WARNING: failed to query GL_SHADING_LANGUAGE_VERSION!");
        }

        // OGL error may be generated due to querying GL_SHADING_LANGUAGE_VERSION which may not be supported by all implementations
        PRREGLsnippets::ClearGLError();
        
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
        PRREGLsnippets::ClearGLError();

        getConsole().OLn("Extensions:");
        getConsole().OLn("");
        printOGLextensions();
        printWGLextensions();
    } // !bAlreadyInitializedOnceOGL

    sharedSettings.Set(
        PRRE_SSET_VSYNC_SUPPORTED,
        PRREhwVideoDiscoverOpenGLbase::isExtensionSupported("WGL_EXT_swap_control", sVidFeaturesOGL, sVidFeaturesWGL) && (wglSwapIntervalEXT != PGENULL) );

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
} // deinitializeBase()


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
bool      PRREhwVideoImpl::bSuppHWmipmapGen          = false;
bool      PRREhwVideoImpl::bSuppCompiledVertexArrays = false;
bool      PRREhwVideoImpl::bSuppNVVertexArrayRange   = false;
bool      PRREhwVideoImpl::bSuppNVVertexArrayRange2  = false;
bool      PRREhwVideoImpl::bSuppNVElementArray       = false;
bool      PRREhwVideoImpl::bSuppATIVertexArrayObject = false;
bool      PRREhwVideoImpl::bSuppATIMapObjectBuffer   = false;
bool      PRREhwVideoImpl::bSuppATIElementArray      = false;
bool      PRREhwVideoImpl::bSuppATITruForm           = false;

std::string     PRREhwVideoImpl::sVidNameWin;       
std::string     PRREhwVideoImpl::sVidNameOGL;       
std::string     PRREhwVideoImpl::sVidVendorOGL;     
std::string     PRREhwVideoImpl::sVidFeaturesOGL;   
std::string     PRREhwVideoImpl::sVidFeaturesWGL;   
std::string     PRREhwVideoImpl::sVidVersionOGL;    
TPRREuint       PRREhwVideoImpl::nVidVersionGLSLmaj = 0;
TPRREuint       PRREhwVideoImpl::nVidVersionGLSLmin = 0;


/** 
    Sets members to default values.
*/
PRREhwVideoImpl::PRREhwVideoImpl() :
    sharedSettings( PRRESharedSettings::createAndGet() ),
    discoverGL_1_1(sVidFeaturesOGL, sVidFeaturesWGL, sVidVersionOGL, nVidVersionGLSLmaj, nVidVersionGLSLmin),
    discoverGL_1_2(sVidFeaturesOGL, sVidFeaturesWGL, sVidVersionOGL, nVidVersionGLSLmaj, nVidVersionGLSLmin),
    discoverGL_1_3(sVidFeaturesOGL, sVidFeaturesWGL, sVidVersionOGL, nVidVersionGLSLmaj, nVidVersionGLSLmin, nColorBits, nDepthBits, nStencilBits),
    discoverGL_1_4(sVidFeaturesOGL, sVidFeaturesWGL, sVidVersionOGL, nVidVersionGLSLmaj, nVidVersionGLSLmin),
    discoverGL_1_5(sVidFeaturesOGL, sVidFeaturesWGL, sVidVersionOGL, nVidVersionGLSLmaj, nVidVersionGLSLmin),
    discoverGL_2_0(sVidFeaturesOGL, sVidFeaturesWGL, sVidVersionOGL, nVidVersionGLSLmaj, nVidVersionGLSLmin),
    discoverGL_2_1(sVidFeaturesOGL, sVidFeaturesWGL, sVidVersionOGL, nVidVersionGLSLmaj, nVidVersionGLSLmin),
    discoverGL_3_0(sVidFeaturesOGL, sVidFeaturesWGL, sVidVersionOGL, nVidVersionGLSLmaj, nVidVersionGLSLmin),
    discoverGL_3_1(sVidFeaturesOGL, sVidFeaturesWGL, sVidVersionOGL, nVidVersionGLSLmaj, nVidVersionGLSLmin),
    discoverGL_3_2(sVidFeaturesOGL, sVidFeaturesWGL, sVidVersionOGL, nVidVersionGLSLmaj, nVidVersionGLSLmin),
    discoverGL_3_3(sVidFeaturesOGL, sVidFeaturesWGL, sVidVersionOGL, nVidVersionGLSLmaj, nVidVersionGLSLmin),
    discoverGL_4_0(sVidFeaturesOGL, sVidFeaturesWGL, sVidVersionOGL, nVidVersionGLSLmaj, nVidVersionGLSLmin),
    discoverGL_4_1(sVidFeaturesOGL, sVidFeaturesWGL, sVidVersionOGL, nVidVersionGLSLmaj, nVidVersionGLSLmin),
    discoverGL_4_2(sVidFeaturesOGL, sVidFeaturesWGL, sVidVersionOGL, nVidVersionGLSLmaj, nVidVersionGLSLmin),
    discoverGL_4_3(sVidFeaturesOGL, sVidFeaturesWGL, sVidVersionOGL, nVidVersionGLSLmaj, nVidVersionGLSLmin)
{
    PreInitialize(NULL, NULL, 0, 0, 0, 0, 0, 0);
} // PRREhwVideoImpl()


PRREhwVideoImpl::PRREhwVideoImpl(const PRREhwVideoImpl&) :
    sharedSettings( PRRESharedSettings::createAndGet() ),
    discoverGL_1_1(sVidFeaturesOGL, sVidFeaturesWGL, sVidVersionOGL, nVidVersionGLSLmaj, nVidVersionGLSLmin),
    discoverGL_1_2(sVidFeaturesOGL, sVidFeaturesWGL, sVidVersionOGL, nVidVersionGLSLmaj, nVidVersionGLSLmin),
    discoverGL_1_3(sVidFeaturesOGL, sVidFeaturesWGL, sVidVersionOGL, nVidVersionGLSLmaj, nVidVersionGLSLmin, nColorBits, nDepthBits, nStencilBits),
    discoverGL_1_4(sVidFeaturesOGL, sVidFeaturesWGL, sVidVersionOGL, nVidVersionGLSLmaj, nVidVersionGLSLmin),
    discoverGL_1_5(sVidFeaturesOGL, sVidFeaturesWGL, sVidVersionOGL, nVidVersionGLSLmaj, nVidVersionGLSLmin),
    discoverGL_2_0(sVidFeaturesOGL, sVidFeaturesWGL, sVidVersionOGL, nVidVersionGLSLmaj, nVidVersionGLSLmin),
    discoverGL_2_1(sVidFeaturesOGL, sVidFeaturesWGL, sVidVersionOGL, nVidVersionGLSLmaj, nVidVersionGLSLmin),
    discoverGL_3_0(sVidFeaturesOGL, sVidFeaturesWGL, sVidVersionOGL, nVidVersionGLSLmaj, nVidVersionGLSLmin),
    discoverGL_3_1(sVidFeaturesOGL, sVidFeaturesWGL, sVidVersionOGL, nVidVersionGLSLmaj, nVidVersionGLSLmin),
    discoverGL_3_2(sVidFeaturesOGL, sVidFeaturesWGL, sVidVersionOGL, nVidVersionGLSLmaj, nVidVersionGLSLmin),
    discoverGL_3_3(sVidFeaturesOGL, sVidFeaturesWGL, sVidVersionOGL, nVidVersionGLSLmaj, nVidVersionGLSLmin),
    discoverGL_4_0(sVidFeaturesOGL, sVidFeaturesWGL, sVidVersionOGL, nVidVersionGLSLmaj, nVidVersionGLSLmin),
    discoverGL_4_1(sVidFeaturesOGL, sVidFeaturesWGL, sVidVersionOGL, nVidVersionGLSLmaj, nVidVersionGLSLmin),
    discoverGL_4_2(sVidFeaturesOGL, sVidFeaturesWGL, sVidVersionOGL, nVidVersionGLSLmaj, nVidVersionGLSLmin),
    discoverGL_4_3(sVidFeaturesOGL, sVidFeaturesWGL, sVidVersionOGL, nVidVersionGLSLmaj, nVidVersionGLSLmin)
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
    Query viewport details.
    Note that it is highly recommended to call all DiscoverOpenGL_X_Y_availability() functions before calling any DiscoverXYZ() function.
    This feature is not bound to specific OpenGL version hence we keep this out of any DiscoverOpenGL_X_Y class.
*/
void PRREhwVideoImpl::DiscoverViewportAvailability()
{
    getConsole().OLnOI("Viewport:");

    PRREGLsnippets::ClearGLError();

    glGetIntegerv(GL_MAX_VIEWPORT_DIMS, tmpRet);
    if ( glGetError() == GL_NO_ERROR )
        getConsole().OLn("Maximum viewport size: %d x %d", tmpRet[0], tmpRet[1]);
    else getConsole().EOLn("Maximum viewport size: ? x ?");

    glGetIntegerv(GL_VIEWPORT_BOUNDS_RANGE, tmpRet);
    if ( glGetError() == GL_NO_ERROR )
    {
        getConsole().OLn("Viewport bounds range: [%d, %d]", tmpRet[0], tmpRet[1]);
        PRREhwVideoDiscoverOpenGLbase::CheckAgainstStandardRange(tmpRet[0], tmpRet[1],
                                  GL_STANDARD_MIN_VIEWPORT_BOUNDS_RANGE[0],
                                  GL_STANDARD_MIN_VIEWPORT_BOUNDS_RANGE[1]);
    }
    else getConsole().EOLn("Viewport bounds range: [?, ?]");

    glGetIntegerv(GL_MAX_VIEWPORTS, tmpRet);
    if ( glGetError() == GL_NO_ERROR )
    {
        getConsole().OLn("Max viewports: %d", tmpRet[0]);
        PRREhwVideoDiscoverOpenGLbase::CheckAgainstStandardMinValue(tmpRet[0], GL_STANDARD_MIN_VIEWPORTS);
    }
    else getConsole().EOLn("Max viewports: ?");

    getConsole().OLnOO("");
} // DiscoverViewportAvailability()


/**
    Query matrix stacks details.
    Note that it is highly recommended to call all DiscoverOpenGL_X_Y_availability() functions before calling any DiscoverXYZ() function.
    This feature is not bound to specific OpenGL version hence we keep this out of any DiscoverOpenGL_X_Y class.
*/
void PRREhwVideoImpl::DiscoverMatrixStacksAvailability()
{
    getConsole().OLnOI("Matrix stacks:");

    glGetIntegerv(GL_MAX_PROJECTION_STACK_DEPTH, tmpRet);
    getConsole().OLn("Max projection matrix stack depth: %d", tmpRet[0]);
    PRREhwVideoDiscoverOpenGLbase::CheckAgainstStandardMinValue(tmpRet[0], GL_STANDARD_MIN_MATRIX_STACK_DEPTH_PROJECTION);

    glGetIntegerv(GL_MAX_MODELVIEW_STACK_DEPTH, tmpRet);
    getConsole().OLn("Max modelview matrix stack depth: %d", tmpRet[0]);
    PRREhwVideoDiscoverOpenGLbase::CheckAgainstStandardMinValue(tmpRet[0], GL_STANDARD_MIN_MATRIX_STACK_DEPTH_MODELVIEW);

    glGetIntegerv(GL_MAX_COLOR_MATRIX_STACK_DEPTH, tmpRet);
    getConsole().OLn("Max color matrix stack depth: %d", tmpRet[0]);
    PRREhwVideoDiscoverOpenGLbase::CheckAgainstStandardMinValue(tmpRet[0], GL_STANDARD_MIN_MATRIX_STACK_DEPTH_COLOR);

    glGetIntegerv(GL_MAX_TEXTURE_STACK_DEPTH, tmpRet);
    getConsole().OLn("Max texture stack depth: %d", tmpRet[0]);
    PRREhwVideoDiscoverOpenGLbase::CheckAgainstStandardMinValue(tmpRet[0], GL_STANDARD_MIN_MATRIX_STACK_DEPTH_TEXTURE);

    glGetIntegerv(GL_MAX_ATTRIB_STACK_DEPTH, tmpRet);
    getConsole().OLn("Max server attribute stack depth: %d", tmpRet[0]);
    PRREhwVideoDiscoverOpenGLbase::CheckAgainstStandardMinValue(tmpRet[0], GL_STANDARD_MIN_STACK_DEPTH_ATTRIB);

    glGetIntegerv(GL_MAX_CLIENT_ATTRIB_STACK_DEPTH, tmpRet);
    getConsole().OLn("Max client attribute stack depth: %d", tmpRet[0]);
    PRREhwVideoDiscoverOpenGLbase::CheckAgainstStandardMinValue(tmpRet[0], GL_STANDARD_MIN_STACK_DEPTH_CLIENT_ATTRIB);

    getConsole().OLnOO("");
} // DiscoverMatrixStacksAvailability()


/**
    Query framebuffer and clipping details.
    Note that it is highly recommended to call all DiscoverOpenGL_X_Y_availability() functions before calling any DiscoverXYZ() function.
    This feature is not bound to specific OpenGL version hence we keep this out of any DiscoverOpenGL_X_Y class.
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
    PRREhwVideoDiscoverOpenGLbase::CheckAgainstStandardMinValue(tmpRet[0], GL_STANDARD_MIN_CLIPPING_PLANES);

    getConsole().OLnOO("");
} // DiscoverFrambufferAndClipping()

/**
    Query compiled vertex arrays availability.
    Note that it is highly recommended to call all DiscoverOpenGL_X_Y_availability() functions before calling any DiscoverXYZ() function.
    This feature is not bound to specific OpenGL version hence we keep this out of any DiscoverOpenGL_X_Y class.
*/
void PRREhwVideoImpl::DiscoverCompiledVertexArraysAvailability()
{
    // bSuppCompiledVertexArrays is not set before, we need to check extension and funcpointers here
    if ( bSuppCompiledVertexArrays = PRREhwVideoDiscoverOpenGLbase::isExtensionSupported("GL_EXT_compiled_vertex_array", sVidFeaturesOGL, sVidFeaturesWGL) )
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
    This feature is not bound to specific OpenGL version hence we keep this out of any DiscoverOpenGL_X_Y class.
*/
void PRREhwVideoImpl::DiscoverNVVertexArrayRangeAvailability()
{
    // bSuppNVVertexArrayRange is not set before, we need to check extension and funcpointers here
    if ( bSuppNVVertexArrayRange = PRREhwVideoDiscoverOpenGLbase::isExtensionSupported("GL_NV_vertex_array_range", sVidFeaturesOGL, sVidFeaturesWGL) )
    {
        if ( bSuppNVVertexArrayRange = (glFlushVertexArrayRangeNV && glVertexArrayRangeNV) )
        {
            getConsole().SOLn("nVidia Vertex Array Range supported!");

            if ( bSuppNVVertexArrayRange2 = PRREhwVideoDiscoverOpenGLbase::isExtensionSupported("GL_NV_vertex_array_range2", sVidFeaturesOGL, sVidFeaturesWGL) )
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
    This feature is not bound to specific OpenGL version hence we keep this out of any DiscoverOpenGL_X_Y class.
*/
void PRREhwVideoImpl::DiscoverNVElementArrayAvailability()
{
    if ( bSuppNVElementArray = PRREhwVideoDiscoverOpenGLbase::isExtensionSupported("GL_NV_element_array", sVidFeaturesOGL, sVidFeaturesWGL) )
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
    This feature is not bound to specific OpenGL version hence we keep this out of any DiscoverOpenGL_X_Y class.
*/
void PRREhwVideoImpl::DiscoverATIVertexArrayObjectAvailability()
{
    // bSuppATIVertexArrayObject is not set before by DiscoverOpenGL_X_Y_availability(), we need to check extension and funcpointers here
    if ( bSuppATIVertexArrayObject = PRREhwVideoDiscoverOpenGLbase::isExtensionSupported("GL_ATI_vertex_array_object", sVidFeaturesOGL, sVidFeaturesWGL) )
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
    This feature is not bound to specific OpenGL version hence we keep this out of any DiscoverOpenGL_X_Y class.
*/
void PRREhwVideoImpl::DiscoverATIMapObjectBufferAvailability()
{
    // bSuppATIMapObjectBuffer is not set before by DiscoverOpenGL_X_Y_availability(), we need to check extension and funcpointers here
    if ( bSuppATIMapObjectBuffer = PRREhwVideoDiscoverOpenGLbase::isExtensionSupported("GL_ATI_map_object_buffer", sVidFeaturesOGL, sVidFeaturesWGL) )
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
    This feature is not bound to specific OpenGL version hence we keep this out of any DiscoverOpenGL_X_Y class.
*/
void PRREhwVideoImpl::DiscoverATIElementArrayAvailability()
{
    // bSuppATIElementArray is not set before by DiscoverOpenGL_X_Y_availability(), we need to check extension and funcpointers here
    if ( bSuppATIElementArray = PRREhwVideoDiscoverOpenGLbase::isExtensionSupported("GL_ATI_element_array", sVidFeaturesOGL, sVidFeaturesWGL) )
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
    Query ATI TruForm availability.
    Note that it is highly recommended to call all DiscoverOpenGL_X_Y_availability() functions before calling any DiscoverXYZ() function.
    This feature is not bound to specific OpenGL version hence we keep this out of any DiscoverOpenGL_X_Y class.
*/
void PRREhwVideoImpl::DiscoverATITruFormAvailability()
{
    // bSuppATITruForm is not set before by DiscoverOpenGL_X_Y_availability(), we need to check extension and funcpointers here
    if ( bSuppATITruForm = PRREhwVideoDiscoverOpenGLbase::isExtensionSupported("GL_ATI_pn_triangles", sVidFeaturesOGL, sVidFeaturesWGL) )
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
    PRREhwVideoDiscoverOpenGLbase::CheckAgainstStandardMinValue(tmpRet[0], GL_STANDARD_MIN_DISPLAY_LIST_NESTING_LEVEL);

    discoverGL_1_4.DiscoverMultiDrawArraysAvailability();
    discoverGL_1_2.DiscoverDrawRangeElementsAvailability();
    DiscoverCompiledVertexArraysAvailability();
    DiscoverNVVertexArrayRangeAvailability();
    DiscoverATIVertexArrayObjectAvailability();
    DiscoverATIMapObjectBufferAvailability();
    discoverGL_1_5.DiscoverVertexBuffersAvailability();
    discoverGL_1_5.DiscoverHwOcclusionQueryAvailability();
    discoverGL_3_3.DiscoverBooleanHwOcclusionQueryAvailability(discoverGL_1_5.isOcclusionQuerySupported());
    DiscoverATITruFormAvailability();

    getConsole().OLnOO("");
} // DiscoverVertexRenderingAvailability()

/**
    Query anisotropic filtering availability.
    Note that it is highly recommended to call all DiscoverOpenGL_X_Y_availability() functions before calling any DiscoverXYZ() function.
    This feature is not bound to specific OpenGL version hence we keep this out of any DiscoverOpenGL_X_Y class.
*/
void PRREhwVideoImpl::DiscoverAnisoFilteringAvailability()
{
    // bSuppAnisoFiltering is not set before by DiscoverOpenGL_X_Y_availability(), we need to check extension here
    if ( bSuppAnisoFiltering = PRREhwVideoDiscoverOpenGLbase::isExtensionSupported("GL_EXT_texture_filter_anisotropic", sVidFeaturesOGL, sVidFeaturesWGL) )  // todo: opengl 4.6 added support for GL_ARB_texture_filter_anisotropic
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
            PRREGLsnippets::ClearGLError();
        }
    }
    else
        getConsole().EOLn("Anisotropic filtering not supported.");
} // DiscoverAnisoFilteringAvailability()

/**
    Query texturing capabilities.
    Note that it is highly recommended to call all DiscoverOpenGL_X_Y_availability() functions before calling any DiscoverXYZ() function.
    This feature is not bound to specific OpenGL version hence we keep this out of any DiscoverOpenGL_X_Y class.
*/
void PRREhwVideoImpl::DiscoverTexturingAvailability()
{
    getConsole().OLnOI("Texturing Caps:");

    PRREGLsnippets::ClearGLError();

    glGetFloatv(GL_MAX_TEXTURE_LOD_BIAS_EXT, tmpRetf);
    if ( glGetError() == GL_NO_ERROR )
    {
        getConsole().OLn("Max texture LOD bias: %f", tmpRetf[0]);
        PRREhwVideoDiscoverOpenGLbase::CheckAgainstStandardMinValuef(tmpRetf[0], GL_STANDARD_MIN_TEXTURE_LOD_BIAS);
    }
    else getConsole().EOLn("Max texture LOD bias: ?");

    glGetIntegerv(GL_MAX_TEXTURE_SIZE, tmpRet);
    if ( glGetError() == GL_NO_ERROR )
    {
        getConsole().OLn("Max texture size: %d x %d", tmpRet[0], tmpRet[0]);
        PRREhwVideoDiscoverOpenGLbase::CheckAgainstStandardMinValue(tmpRet[0], GL_STANDARD_MIN_TEXTURE_SIZE);
    }
    else getConsole().EOLn("Max texture size: ? x ?");
    
    glGetIntegerv(GL_MAX_RECTANGLE_TEXTURE_SIZE_ARB, tmpRet);
    if ( glGetError() == GL_NO_ERROR )
    {
        getConsole().OLn("Max rectangle texture size: %d x %d", tmpRet[0], tmpRet[0]);
        PRREhwVideoDiscoverOpenGLbase::CheckAgainstStandardMinValue(tmpRet[0], GL_STANDARD_MIN_RECTANGLE_TEXTURE_SIZE);
    }
    else getConsole().EOLn("Max rectangle texture size: ? x ?");

    glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE_EXT, tmpRet);
    if ( glGetError() == GL_NO_ERROR )
    {
        getConsole().OLn("Max 3D texture size: %d x %d x %d", tmpRet[0], tmpRet[0], tmpRet[0]);
        PRREhwVideoDiscoverOpenGLbase::CheckAgainstStandardMinValue(tmpRet[0], GL_STANDARD_MIN_3D_TEXTURE_SIZE);
    }
    else getConsole().EOLn("Max 3D texture size: ? x ? x ?");
    
    glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE_EXT, tmpRet);
    if ( glGetError() == GL_NO_ERROR )
    {
        getConsole().OLn("Max cube map texture size: %d x %d", tmpRet[0], tmpRet[0]);
        PRREhwVideoDiscoverOpenGLbase::CheckAgainstStandardMinValue(tmpRet[0], GL_STANDARD_MIN_CUBE_MAP_TEXTURE_SIZE);
    }
    else getConsole().EOLn("Max cube map texture size: ? x ?");

    discoverGL_1_3.DiscoverMultiTexturingAvailability();
    DiscoverAnisoFilteringAvailability();  

    if ( bSuppHWmipmapGen = PRREhwVideoDiscoverOpenGLbase::isExtensionSupported("GL_SGIS_generate_mipmap", sVidFeaturesOGL, sVidFeaturesWGL) )
        getConsole().SOLn("Hardware mipmap generation supported.");
    else
        getConsole().EOLn("Hardware mipmap generation not supported, using software.");

    // bSuppBGRAtex is already set by DiscoverOpenGL_X_Y_availability()
    if ( discoverGL_1_2.isNativeDIBFormatSupported() )
        getConsole().SOLn("Native support for BGR/BGRA formats (DIBs).");
    else
        getConsole().EOLn("Native support for BGR/BGRA formats (DIBs) not found, using software.");
    
    discoverGL_1_3.DiscoverTextureCompressionAvailability();

    /*
        TODO: NPOT texture check, texture rectangle check
        http://www.opengl.org/registry/specs/ARB/texture_non_power_of_two.txt
        http://www.opengl.org/registry/specs/ARB/texture_rectangle.txt 
        http://www.opengl.org/registry/specs/NV/texture_rectangle.txt
    */

    getConsole().OLnOO("");

} // DiscoverTexturingAvailability()

void PRREhwVideoImpl::DiscoverOpenGLavailability()
{
    if ( bAlreadyInitializedOnceOGL )
    {
        getConsole().OLn("Discovery has been already done!");
        getConsole().OLn("");
        return;
    }

    if ( discoverOpenGLversions.empty() )
    {
        discoverOpenGLversions.push_back(&discoverGL_1_1);
        discoverOpenGLversions.push_back(&discoverGL_1_2);
        discoverOpenGLversions.push_back(&discoverGL_1_3);
        discoverOpenGLversions.push_back(&discoverGL_1_4);
        discoverOpenGLversions.push_back(&discoverGL_1_5);
        discoverOpenGLversions.push_back(&discoverGL_2_0);
        discoverOpenGLversions.push_back(&discoverGL_2_1);
        discoverOpenGLversions.push_back(&discoverGL_3_0);
        discoverOpenGLversions.push_back(&discoverGL_3_1);
        discoverOpenGLversions.push_back(&discoverGL_3_2);
        discoverOpenGLversions.push_back(&discoverGL_3_3);
        discoverOpenGLversions.push_back(&discoverGL_4_0);
        discoverOpenGLversions.push_back(&discoverGL_4_1);
        discoverOpenGLversions.push_back(&discoverGL_4_2);
        discoverOpenGLversions.push_back(&discoverGL_4_3);
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

    for (size_t i = 0; i < discoverOpenGLversions.size(); i++)
    {
        const std::string& ver = discoverOpenGLversions[i]->getVersion();
        getConsole().OLn("Testing for OpenGL %s features ...", ver.c_str());
        if ( discoverOpenGLversions[i]->discoverOpenGLavailability() )
            getConsole().SOLn("%s features OK!", ver.c_str());
        else
            getConsole().EOLn("%s features NOK!", ver.c_str());
        getConsole().OLn("");
    }

    DiscoverFramebufferAndClipping();
    DiscoverViewportAvailability();
    discoverGL_1_3.DiscoverFSAAavailability(hWndDC);
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