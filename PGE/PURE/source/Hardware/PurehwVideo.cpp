/*
    ###################################################################################
    PureHwVideoImpl.cpp
    This file is part of PURE.
    Pure video capabilities class.
    Made by PR00F88
    ###################################################################################
*/


#include "PureBaseIncludes.h"  // PCH
#include "../../include/external/Hardware/PureHwVideo.h"
#include "../../include/internal/PureGLsnippets.h"
#include "../../include/internal/PureGLextensionFuncs.h"
#include "../../include/internal/PurePragmas.h"
#include "../../include/internal/Display/PureSharedSettings.h"
#include "../../include/internal/Hardware/PureHwVideoDiscoverOpenGL_1_1.h"
#include "../../include/internal/Hardware/PureHwVideoDiscoverOpenGL_1_2.h"
#include "../../include/internal/Hardware/PureHwVideoDiscoverOpenGL_1_3.h"
#include "../../include/internal/Hardware/PureHwVideoDiscoverOpenGL_1_4.h"
#include "../../include/internal/Hardware/PureHwVideoDiscoverOpenGL_1_5.h"
#include "../../include/internal/Hardware/PureHwVideoDiscoverOpenGL_2_0.h"
#include "../../include/internal/Hardware/PureHwVideoDiscoverOpenGL_2_1.h"
#include "../../include/internal/Hardware/PureHwVideoDiscoverOpenGL_3_0.h"
#include "../../include/internal/Hardware/PureHwVideoDiscoverOpenGL_3_1.h"
#include "../../include/internal/Hardware/PureHwVideoDiscoverOpenGL_3_2.h"
#include "../../include/internal/Hardware/PureHwVideoDiscoverOpenGL_3_3.h"
#include "../../include/internal/Hardware/PureHwVideoDiscoverOpenGL_4_0.h"
#include "../../include/internal/Hardware/PureHwVideoDiscoverOpenGL_4_1.h"
#include "../../include/internal/Hardware/PureHwVideoDiscoverOpenGL_4_2.h"
#include "../../include/internal/Hardware/PureHwVideoDiscoverOpenGL_4_3.h"

using namespace std;

class PureHwVideoImpl :
    public PureHwVideo
{

public:
    static PureHwVideoImpl& get();

    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    CConsole&  getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */

    void Initialize(
        HGLRC rc, HDC wnd_dc,
        TPureUInt nResX, TPureUInt nResY,
        TPureInt nColorBits, TPureInt nDepthBits,
        TPureInt nStencilBits, TPureInt nFSAAlevel );

    TPureBool  isAcceleratorDetected() const;
    TPureUInt  getUsedVideoMemory() const;       
    TPureUInt  getUsedTextureMemory() const;  
    TPureUInt  getColorBufferPixelCount() const; 
    TPureUInt  getColorBufferSize() const;      
    TPureUInt  getDepthBufferSize() const;      
    TPureUInt  getStencilBufferSize() const;   
    TPureUInt  getSampleBufferSize() const;       
    TPureUInt  getUsedFrameBufferMemory() const; 
    TPureBool  isVSyncSupported() const;          
    TPureBool  isMultiTexturingSupported() const; 
    TPureInt   getTextureUnitsCount() const;    
    TPureBool  isHardwareMipMapGenerationSupported() const;  
    TPureBool  isNativeDIBFormatSupported() const;       
    TPureBool  isAnisoFilteringSupported() const;      
    TPureFloat getMaximumAnisoFiltering() const;       
    TPureBool  isTextureCompressionSupported() const;   
    TPureBool  isVTCSupported() const;                  
    TPureBool  is3DcSupported() const;                  
    TPureBool  isRGTCSupported() const;                
    TPureBool  isLATCSupported() const;                
    TPureBool  isAdvancedPixelFormatSetSupported() const;
    TPureBool  isFullSceneAntiAliasingSupported() const;  
    TPureInt   getMaxSamplesCount() const;               
    TPureBool  isSamplesSupported(TPureInt numsamples) const;                   
    TPureBool  isMultiDrawArraysSupported() const;     
    TPureBool  isDrawRangeElementsSupported() const;   
    TPureBool  isCompiledVertexArraySupported() const;  
    TPureBool  isNVVertexArrayRangeSupported() const;    
    TPureBool  isNVElementArraySupported() const;       
    TPureBool  isATIVertexArrayObjectSupported() const; 
    TPureBool  isATIMapObjectBufferSupported() const;    
    TPureBool  isATIElementArraySupported() const;      
    TPureBool  isVertexBufferObjectSupported() const;    
    TPureBool  isATITruFormSupported() const;   
    TPureBool  isOcclusionQuerySupported() const;
    TPureBool  isBooleanOcclusionQuerySupported() const;
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

    static PureHwVideoImpl hwVideoInstance;
    static HGLRC hGLRC;                            /**< OpenGL rendering context handle. */
    static HDC   hWndDC;                           /**< Our main window's device context. */
    static TPureUInt m_nResX, m_nResY;             /**< Display resolution. */      
    static TPureInt  m_nColorBits, m_nDepthBits;   /**< Display color depth, ZBuffer-depth. */
    static TPureInt  m_nStencilBits, m_nFSAAlevel; /**< Stencil Buffer depth, FSAA-level. */

    static bool      bAlreadyInitializedOnceOGL; /**< True if OGL-initialization has been done once already.
                                                      This is to speed up consecutive stop-start initializations with same OGL renderer. */
    static TPureBool bSuppAnisoFiltering;        /**< Is anisotropic filtering available? */
    static GLfloat   nAnisoFilterMax;            /**< Maximum level of anisotropic filtering. */
    static TPureBool bSuppHWmipmapGen;           /**< Is HW MIP map generation available? */
    static TPureBool bSuppCompiledVertexArrays;  /**< Is CompiledVertexArrays supported? */
    static TPureBool bSuppNVVertexArrayRange;    /**< Is the nVidia-specific Vertex Array Range supported? */
    static TPureBool bSuppNVVertexArrayRange2;   /**< Is the nVidia-specific Vertex Array Range 2 supported? */
    static TPureBool bSuppNVElementArray;        /**< Is the nVidia-specific Element Array supported? */
    static TPureBool bSuppATIVertexArrayObject;  /**< Is the ATI-specific Vertex Array Object supported? */
    static TPureBool bSuppATIMapObjectBuffer;    /**< Is the ATI-specific MapObjectBuffer (for Vertex Array Object) supported? */
    static TPureBool bSuppATIElementArray;       /**< Is the ATI-specific Element Array supported? */
    static TPureBool bSuppATITruForm;            /**< Is the ATI-specific TruForm supported? */

    static std::string sVidNameWin;              /**< VGA name based on Windows. */
    static std::string sVidNameOGL;              /**< VGA name based on OpenGL. */
    static std::string sVidVendorOGL;            /**< VGA vendor based on OpenGL. */
    static std::string sVidFeaturesOGL;          /**< VGA features based on OGL. */
    static std::string sVidFeaturesWGL;          /**< VGA features based on WGL. */
    static std::string sVidVersionOGL;           /**< OpenGL version. */
    static TPureUInt   nVidVersionGLSLmaj;       /**< GLSL major version. */
    static TPureUInt   nVidVersionGLSLmin;       /**< GLSL minor version. */

    // ---------------------------------------------------------------------------

    PureSharedSettings& sharedSettings;  /**< Pointer to shared settings. */

    GLint   tmpRet[6];                   /**< Hold for glGetInteger...() results. */
    GLfloat tmpRetf[2];                  /**< Hold for glGetFloat...() results. */

    DISPLAY_DEVICEA dispdev;             /**< To query the Windows name of the VGA. */

    std::vector<PureHwVideoDiscoverOpenGLbase*> discoverOpenGLversions;  // ah please use C++11, make this const and initialize here or in ctor initializes list!!!
    PureHwVideoDiscoverOpenGL_1_1 discoverGL_1_1;
    PureHwVideoDiscoverOpenGL_1_2 discoverGL_1_2;
    PureHwVideoDiscoverOpenGL_1_3 discoverGL_1_3;
    PureHwVideoDiscoverOpenGL_1_4 discoverGL_1_4;
    PureHwVideoDiscoverOpenGL_1_5 discoverGL_1_5;
    PureHwVideoDiscoverOpenGL_2_0 discoverGL_2_0;
    PureHwVideoDiscoverOpenGL_2_1 discoverGL_2_1;
    PureHwVideoDiscoverOpenGL_3_0 discoverGL_3_0;
    PureHwVideoDiscoverOpenGL_3_1 discoverGL_3_1;
    PureHwVideoDiscoverOpenGL_3_2 discoverGL_3_2;
    PureHwVideoDiscoverOpenGL_3_3 discoverGL_3_3;
    PureHwVideoDiscoverOpenGL_4_0 discoverGL_4_0;
    PureHwVideoDiscoverOpenGL_4_1 discoverGL_4_1;
    PureHwVideoDiscoverOpenGL_4_2 discoverGL_4_2;
    PureHwVideoDiscoverOpenGL_4_3 discoverGL_4_3;

    // ---------------------------------------------------------------------------

    PureHwVideoImpl();                  /**< Sets members to default values. */

    PureHwVideoImpl(const PureHwVideoImpl&);
    PureHwVideoImpl& operator=(const PureHwVideoImpl&);

    virtual ~PureHwVideoImpl();

    /** Preinitializes members. */
    void      PreInitialize(
        HGLRC rc, HDC dc,
        TPureUInt resx, TPureUInt resy,
        TPureInt cbits, TPureInt dbits,
        TPureInt sbits, TPureInt fsaa);
    void      PreInitialize();        /**< Preinitializes members. */
    TPureBool initializeBase();       /**< Does the real initializations. */
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
PureHwVideoImpl& PureHwVideoImpl::get()
{
    return hwVideoInstance;
} // get()


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& PureHwVideoImpl::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* PureHwVideoImpl::getLoggerModuleName()
{
    return "PureHwVideo";
} // getLoggerModuleName()


/**
    Sets members to real values within the singleton instance.
    Does nothing if rc is NULL.
*/
void PureHwVideoImpl::Initialize(
    HGLRC rc, HDC wnd_dc,
    TPureUInt nResX, TPureUInt nResY,
    TPureInt nColorBits, TPureInt nDepthBits,
    TPureInt nStencilBits, TPureInt nFSAAlevel )
{
    getConsole().OIOLn("PureHwVideo::Initialize() ...");
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
    PureHwBase::Initialize();  // here we trigger initializeBase()
    
} // Initialize(...)


/**
    Gets whether video accelerator is detected or not.
    @return True if non-software renderer is initialized, false otherwise.
*/
TPureBool PureHwVideoImpl::isAcceleratorDetected() const
{
    return isInitialized() && ( hGLRC != NULL );
}


/**
    Gets the amount of used VRAM.
    @return The amount of used VRAM in Bytes.
*/
TPureUInt PureHwVideoImpl::getUsedVideoMemory() const
{
    return ( getUsedTextureMemory() + getUsedFrameBufferMemory() );
} // getUsedVideoMemory()


/**
    Gets the amount of used texture memory.
    @return The amount of used texture memory in Bytes.
*/
TPureUInt PureHwVideoImpl::getUsedTextureMemory() const
{
    // todo: go thru all textures in all texturemgrs and sum texture memory usage
    return 0;
} // getUsedTextureMemory()


/**
    Gets the number of pixels in the Frame Buffer (width x height).
    @return The number of pixels in the Frame Buffer (width x height).
*/
TPureUInt PureHwVideoImpl::getColorBufferPixelCount() const
{
    return ( m_nResX * m_nResY );
} // getColorBufferPixelCount()


/** 
    Gets the amount of VRAM used by the Color Buffer in Bytes.
    @return The amount of VRAM used by the Color Buffer in Bytes.
*/
TPureUInt PureHwVideoImpl::getColorBufferSize() const
{
    return ( getColorBufferPixelCount()*2 * m_nColorBits/8 );
} // getColorBufferSize()


/** 
    Gets the amount of VRAM used by the Depth Buffer in Bytes.
    @return The amount of VRAM used by the Depth Buffer in Bytes.
*/
TPureUInt PureHwVideoImpl::getDepthBufferSize() const
{
    return ( m_nDepthBits/8 * getColorBufferPixelCount() );
} // getDepthBufferSize()


/**
    Gets the amount of VRAM used by the Stencil Buffer in Bytes.
    @return The amount of VRAM used by the Stencil Buffer in Bytes.
*/
TPureUInt PureHwVideoImpl::getStencilBufferSize() const
{
    return ( m_nStencilBits/8 * getColorBufferPixelCount() );
} // getStencilBufferSize()


/**
    Gets the amount of VRAM used by the Sample Buffer in Bytes.
    @return The amount of VRAM used by the Sample Buffer in Bytes.
*/
TPureUInt PureHwVideoImpl::getSampleBufferSize() const
{
    return ( m_nFSAAlevel * (getColorBufferSize()/2 + getDepthBufferSize() + getStencilBufferSize()) );
} // getSampleBufferSize()


/**
    Gets the amount of VRAM used by the Frame Buffer in Bytes.
    @return The amount of VRAM used by the Frame Buffer in Bytes.
*/
TPureUInt PureHwVideoImpl::getUsedFrameBufferMemory() const
{
    return ( getColorBufferSize() + getDepthBufferSize() + getStencilBufferSize() + getSampleBufferSize() );
} // getUsedFrameBufferMemory()
 

/**
    Gets whether VSync is supported or not.
    @return True if VSync is supported, false otherwise.
*/
TPureBool PureHwVideoImpl::isVSyncSupported() const
{
    return sharedSettings.get(PURE_SSET_VSYNC_SUPPORTED);
} // isVSyncSupported()


/**
    Gets whether multitexturing is supported or not.
    @return True if multitexturing is supported, false otherwise.
*/
TPureBool PureHwVideoImpl::isMultiTexturingSupported() const
{
    return isInitialized() && discoverGL_1_3.isMultiTexturingSupported();
} // isMultiTexturingSupported()


/**
    Gets the number of texture units.
    @return The number of texture units.
*/
TPureInt PureHwVideoImpl::getTextureUnitsCount() const
{
    return isInitialized() ? discoverGL_1_3.getTextureUnitsCount() : (TPureInt) 0;
} // getTextureUnitsCount()


/**
    Gets whether HW MIP map generation is supported or not.
    @return True if HW MIP map generation is supported, otherwise false.
*/
TPureBool PureHwVideoImpl::isHardwareMipMapGenerationSupported() const
{
    return isInitialized() && bSuppHWmipmapGen;
} // isHardwareMipMapGenerationSupported()


/**
    Gets whether uploading of native DIBs (BGRA) is supported or not.
    @return True if uploading of native DIBs (BGRA) is supported, otherwise false.
*/
TPureBool PureHwVideoImpl::isNativeDIBFormatSupported() const
{
    return isInitialized() && discoverGL_1_2.isNativeDIBFormatSupported();
} // isNativeDIBFormatSupported()


/**
    Gets whether anisotropic filtering is supported or not.
    @return True if anisotropic filtering is supported, false otherwise.
*/
TPureBool PureHwVideoImpl::isAnisoFilteringSupported() const
{
    return isInitialized() && bSuppAnisoFiltering;
} // isAnisoFilteringSupported()


/**
    Gets the maximum available level of anisotropy.
    @return The maximum available level of anisotropy.
*/
TPureFloat PureHwVideoImpl::getMaximumAnisoFiltering() const
{
    return isInitialized() ? (TPureFloat) nAnisoFilterMax : (TPureFloat) 0.0f;
} // getMaximumAnisoFiltering()


/**
    Gets whether texture compression is supported or not.
    @return True if texture compression is supported, otherwise false.
*/
TPureBool PureHwVideoImpl::isTextureCompressionSupported() const
{
    return isInitialized() && discoverGL_1_3.isTextureCompressionSupported();
} // isTextureCompressionSupported()


/**
    Gets whether Volume Texture Compression is supported or not.
    @return True if Volume Texture Compression is supported, otherwise false.
*/
TPureBool PureHwVideoImpl::isVTCSupported() const
{
    return isInitialized() && discoverGL_1_3.isVTCSupported();
} // isVTCSupported()


/**
    Gets whether 3Dc/ATI2/DXN compression is supported or not.
    @return True if 3Dc/ATI2/DXN compression is supported, otherwise false.
*/
TPureBool PureHwVideoImpl::is3DcSupported() const
{
    return isInitialized() && discoverGL_1_3.is3DcSupported();
} // is3DcSupported()


/**
    Gets whether Red-Green compression is supported or not.
    @return True if Red-Green compression is supported, otherwise false.
*/
TPureBool PureHwVideoImpl::isRGTCSupported() const
{
    return isInitialized() && discoverGL_1_3.isLATCSupported();
} // isRGTCSupported()


/**
    Gets whether Luminance-Alpha compression is supported or not.
    @return True if Luminance-Alpha compression is supported, otherwise false.
*/
TPureBool PureHwVideoImpl::isLATCSupported() const
{
    return isInitialized() && discoverGL_1_3.isLATCSupported();
} // isLATCsupported()


/**
    Gets whether advanced pixel formats supported or not.
    Advanced pixel formats are required for various extra features like MSAA.
*/
TPureBool PureHwVideoImpl::isAdvancedPixelFormatSetSupported() const
{
    return wglGetPixelFormatAttribivARB && wglGetPixelFormatAttribfvARB && wglChoosePixelFormatARB &&
        PureHwVideoDiscoverOpenGLbase::isExtensionSupported("WGL_ARB_pixel_format", sVidFeaturesOGL, sVidFeaturesWGL);
} // isAdvancedPixelFormatSetSupported()


/** 
    Gets whether FSAA is supported or not.
    @return True if FSAA is supported, otherwise false.
*/
TPureBool PureHwVideoImpl::isFullSceneAntiAliasingSupported() const
{
    return isInitialized() && discoverGL_1_3.isFullSceneAntiAliasingSupported();
} // isFullSceneAntiAliasingSupported()


/**
    Gets the maximum FSAA level.
    @return The maximum available FSAA-level.
*/
TPureInt PureHwVideoImpl::getMaxSamplesCount() const
{
    return isInitialized() ? discoverGL_1_3.getMaxSamplesCount() : (TPureInt) 0;
} // getMaxSamplesCount()


/**
    Gets whether the given MSAA-level is supported or not.
    @return True if the given MSAA-level is supported, otherwise false.
*/
TPureBool PureHwVideoImpl::isSamplesSupported(TPureInt numsamples) const
{
    return isInitialized() && discoverGL_1_3.isSamplesSupported(numsamples);
} // isSamplesSupported()


/**
    Gets whether MultiDrawArrays is supported or not.
*/
TPureBool PureHwVideoImpl::isMultiDrawArraysSupported() const
{
    return isInitialized() && discoverGL_1_4.isMultiDrawArraysSupported();
} // isMultiDrawArraysSupported()


/**
    Gets whether DrawRangeElements is supported or not.
*/
TPureBool PureHwVideoImpl::isDrawRangeElementsSupported() const
{
    return isInitialized() && discoverGL_1_2.isDrawRangeElementsSupported();
} // isDrawRangeElementsSupported()


/**
    Gets whether Compiled Vertex Array is supported or not.
*/
TPureBool PureHwVideoImpl::isCompiledVertexArraySupported() const 
{
    return isInitialized() && bSuppCompiledVertexArrays;
} // isCompiledVertexArraySupported()


/**
    Gets whether the nVidia-specific Vertex Array Range is supported.
*/
TPureBool PureHwVideoImpl::isNVVertexArrayRangeSupported() const
{
    return isInitialized() && bSuppNVVertexArrayRange;
} // isNVVertexArrayRangeSupported() 


/**
    Gets whether the nVidia-specific Element Array is supported.
*/
TPureBool PureHwVideoImpl::isNVElementArraySupported() const
{
    return isInitialized() && bSuppNVElementArray;
} // isNVElementArraySupported()


/**
    Gets whether the ATI-specific Vertex Array Object is supported.
*/
TPureBool PureHwVideoImpl::isATIVertexArrayObjectSupported() const
{
    return isInitialized() && bSuppATIVertexArrayObject;
} // isATIVertexArrayObjectSupported()  


/**
    Gets whether the ATI-specific MapObjectBuffer (for Vertex Array Object) is supported.
*/
TPureBool PureHwVideoImpl::isATIMapObjectBufferSupported() const
{
    return isInitialized() && bSuppATIMapObjectBuffer;
} // isATIMapObjectBufferSupported()


/**
    Gets whether the ATI-specific Element Array is supported.
*/
TPureBool PureHwVideoImpl::isATIElementArraySupported() const
{
    return isInitialized() && bSuppATIElementArray;
} // isATIElementArraySupported()


/**
    Gets whether Vertex Buffer Object is supported or not.
*/
TPureBool PureHwVideoImpl::isVertexBufferObjectSupported() const
{
    return isInitialized() && discoverGL_1_5.isVertexBufferObjectSupported();
} // isVertexBufferObjectSupported()


/**
    Gets whether the ATI-specific TruForm is supported.
*/
TPureBool PureHwVideoImpl::isATITruFormSupported() const
{
    return isInitialized() && bSuppATITruForm;
} // isATITruFormSupported()


/**
    Gets whether HW occlusion culling is supported.
*/
TPureBool PureHwVideoImpl::isOcclusionQuerySupported() const
{
    return isInitialized() && discoverGL_1_5.isOcclusionQuerySupported();
} // isOcclusionQuerySupported()


/**
    Gets whether HW boolean occlusion culling is supported.
*/
TPureBool PureHwVideoImpl::isBooleanOcclusionQuerySupported() const
{
    return isInitialized() && discoverGL_3_3.isBooleanOcclusionQuerySupported();
} // isBooleanOcclusionQuerySupported()


/**
    Gets the Windows-based VGA name.
    @return The Windows-based VGA name.
*/
const string& PureHwVideoImpl::getVideocardOSName() const 
{
    static const std::string sEmpty = "";
    return isInitialized() ? sVidNameWin : sEmpty;
} // getVideocardOSName()


/**
    Gets the OpenGL-based VGA name.
    @return The OpenGL-based VGA name.
*/
const string& PureHwVideoImpl::getVideocardOGLName() const 
{
    static const std::string sEmpty = "";
    return isInitialized() ? sVidNameOGL : sEmpty;
} // getVideocardOGLName()


/**
    Gets the OpenGL version supported by the driver.
    @return The OpenGL version.
*/
const string& PureHwVideoImpl::getVideocardOGLVersion() const 
{
    static const std::string sEmpty = "";
    return isInitialized() ? sVidVersionOGL : sEmpty;
} // getVideocardOGLVersion()


/**
    Gets the GLSL version supported by the driver.
*/
const string PureHwVideoImpl::getVideocardGLSLVersion() const
{
    const std::string sVer = std::to_string(nVidVersionGLSLmaj) + "." + std::to_string(nVidVersionGLSLmin);
    return isInitialized() ? sVer : "";
} // getVideocardGLSLVersion()


/**
    Gets the OpenGL-based VGA vendor.
    @return The VGA vendor based on OpenGL.
*/
const string& PureHwVideoImpl::getVideocardOGLVendor() const 
{
    static const std::string sEmpty = "";
    return isInitialized() ? sVidVendorOGL : sEmpty;
} // getVideocardOGLVendor()


/**
    Gets the OpenGL features supported by the driver.
    @return The OpenGL features supported by the driver.
*/
const string& PureHwVideoImpl::getVideocardOGLFeatureList() const 
{
    static const std::string sEmpty = "";
    return isInitialized() ? sVidFeaturesOGL : sEmpty;
} // getVideocardOGLFeatureList()


/**
    Gets the WGL features supported by the driver.
    @return The WGL features supported by the driver.
*/
const string& PureHwVideoImpl::getVideocardWGLFeatureList() const 
{
    static const std::string sEmpty = "";
    return isInitialized() ? sVidFeaturesWGL : sEmpty;
} // getVideocardWGLFeatureList()


/**
    Writes statistics to the console.
*/
void PureHwVideoImpl::WriteStats()
{
    getConsole().OLn("PureHwVideo::WriteStats()");
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
void PureHwVideoImpl::PreInitialize(HGLRC rc, HDC dc, TPureUInt resx, TPureUInt resy, TPureInt cbits, TPureInt dbits, TPureInt sbits, TPureInt fsaa)
{
    hGLRC = rc;
    hWndDC = dc;
    m_nResX = resx;
    m_nResY = resy;
    m_nColorBits = cbits;
    m_nDepthBits = dbits;
    m_nStencilBits = sbits;
    m_nFSAAlevel   = fsaa;

    memset(&dispdev, 0, sizeof(dispdev));
    dispdev.cb = sizeof(dispdev);

} // PreInitialize()


/**
    Preinitializes members.
*/
void PureHwVideoImpl::PreInitialize()
{
    PreInitialize(hGLRC, hWndDC, m_nResX, m_nResY, m_nColorBits, m_nDepthBits, m_nStencilBits, m_nFSAAlevel);
    sharedSettings.Set(PURE_SSET_VSYNC_SUPPORTED, false);
} // deinitializeBase()


/**
    Does the real initialization.
*/
TPureBool PureHwVideoImpl::initializeBase()
{
    EnumDisplayDevicesA(NULL, 0, &dispdev, NULL);
    sVidNameWin = dispdev.DeviceString;

    if ( hGLRC == NULL )
    {
        // app may be using software renderer
        getConsole().OLn("GFX card: %s", sVidNameWin.c_str());
        sharedSettings.Set(PURE_SSET_VSYNC_SUPPORTED, false);
        getConsole().OO();
        getConsole().OO();

        return true;
    }

    if (bAlreadyInitializedOnceOGL)
    {
        getConsole().OLn("This process already initialized OpenGL during a previous initialization, no need to fetch pointers again!");
    }
    else
    {
        PureGLgetFunctionPointers();

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
        PureGLsnippets::ClearGLError();
        
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
        PureGLsnippets::ClearGLError();

        getConsole().OLn("Extensions:");
        getConsole().OLn("");
        printOGLextensions();
        printWGLextensions();
    } // bAlreadyInitializedOnceOGL

    sharedSettings.Set(
        PURE_SSET_VSYNC_SUPPORTED,
        PureHwVideoDiscoverOpenGLbase::isExtensionSupported("WGL_EXT_swap_control", sVidFeaturesOGL, sVidFeaturesWGL) && (wglSwapIntervalEXT != PGENULL) );

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
void PureHwVideoImpl::DeinitializeBase()
{
    hGLRC = NULL;
    hWndDC = NULL;
    m_nResX = 0;
    m_nResY = 0;
    m_nColorBits = 0;
    m_nDepthBits = 0;
    m_nStencilBits = 0;
    m_nFSAAlevel   = 0;
} // deinitializeBase()


// ############################### PRIVATE ###############################


PureHwVideoImpl PureHwVideoImpl::hwVideoInstance;


const GLint   PureHwVideoImpl::GL_STANDARD_MIN_CLIPPING_PLANES = 6;
const GLint   PureHwVideoImpl::GL_STANDARD_MIN_VIEWPORTS = 16;
const GLint   PureHwVideoImpl::GL_STANDARD_MIN_VIEWPORT_BOUNDS_RANGE[] = {-32768, 32767};
const GLint   PureHwVideoImpl::GL_STANDARD_MIN_MATRIX_STACK_DEPTH_PROJECTION = 2;
const GLint   PureHwVideoImpl::GL_STANDARD_MIN_MATRIX_STACK_DEPTH_MODELVIEW = 32;
const GLint   PureHwVideoImpl::GL_STANDARD_MIN_MATRIX_STACK_DEPTH_COLOR = 2;
const GLint   PureHwVideoImpl::GL_STANDARD_MIN_MATRIX_STACK_DEPTH_TEXTURE = 2;
const GLint   PureHwVideoImpl::GL_STANDARD_MIN_STACK_DEPTH_ATTRIB = 16;
const GLint   PureHwVideoImpl::GL_STANDARD_MIN_STACK_DEPTH_CLIENT_ATTRIB = 16;
const GLint   PureHwVideoImpl::GL_STANDARD_MIN_DISPLAY_LIST_NESTING_LEVEL = 64;
const GLfloat PureHwVideoImpl::GL_STANDARD_MIN_TEXTURE_LOD_BIAS = 4.0f;
const GLint   PureHwVideoImpl::GL_STANDARD_MIN_TEXTURE_SIZE = 64;
const GLint   PureHwVideoImpl::GL_STANDARD_MIN_RECTANGLE_TEXTURE_SIZE = 1024;
const GLint   PureHwVideoImpl::GL_STANDARD_MIN_3D_TEXTURE_SIZE = 64;
const GLint   PureHwVideoImpl::GL_STANDARD_MIN_CUBE_MAP_TEXTURE_SIZE = 1024;

HGLRC     PureHwVideoImpl::hGLRC = NULL;
HDC       PureHwVideoImpl::hWndDC = NULL;
TPureUInt PureHwVideoImpl::m_nResX = 0;
TPureUInt PureHwVideoImpl::m_nResY = 0;
TPureInt  PureHwVideoImpl::m_nColorBits = 0;
TPureInt  PureHwVideoImpl::m_nDepthBits = 0;
TPureInt  PureHwVideoImpl::m_nStencilBits = 0;
TPureInt  PureHwVideoImpl::m_nFSAAlevel = 0;

bool      PureHwVideoImpl::bAlreadyInitializedOnceOGL = false; 
bool      PureHwVideoImpl::bSuppAnisoFiltering       = false;
GLfloat   PureHwVideoImpl::nAnisoFilterMax           = 0;
bool      PureHwVideoImpl::bSuppHWmipmapGen          = false;
bool      PureHwVideoImpl::bSuppCompiledVertexArrays = false;
bool      PureHwVideoImpl::bSuppNVVertexArrayRange   = false;
bool      PureHwVideoImpl::bSuppNVVertexArrayRange2  = false;
bool      PureHwVideoImpl::bSuppNVElementArray       = false;
bool      PureHwVideoImpl::bSuppATIVertexArrayObject = false;
bool      PureHwVideoImpl::bSuppATIMapObjectBuffer   = false;
bool      PureHwVideoImpl::bSuppATIElementArray      = false;
bool      PureHwVideoImpl::bSuppATITruForm           = false;

std::string     PureHwVideoImpl::sVidNameWin;       
std::string     PureHwVideoImpl::sVidNameOGL;       
std::string     PureHwVideoImpl::sVidVendorOGL;     
std::string     PureHwVideoImpl::sVidFeaturesOGL;   
std::string     PureHwVideoImpl::sVidFeaturesWGL;   
std::string     PureHwVideoImpl::sVidVersionOGL;    
TPureUInt       PureHwVideoImpl::nVidVersionGLSLmaj = 0;
TPureUInt       PureHwVideoImpl::nVidVersionGLSLmin = 0;


/** 
    Sets members to default values.
*/
PureHwVideoImpl::PureHwVideoImpl() :
    sharedSettings( PureSharedSettings::createAndGet() ),
    discoverGL_1_1(sVidFeaturesOGL, sVidFeaturesWGL, sVidVersionOGL, nVidVersionGLSLmaj, nVidVersionGLSLmin),
    discoverGL_1_2(sVidFeaturesOGL, sVidFeaturesWGL, sVidVersionOGL, nVidVersionGLSLmaj, nVidVersionGLSLmin),
    discoverGL_1_3(sVidFeaturesOGL, sVidFeaturesWGL, sVidVersionOGL, nVidVersionGLSLmaj, nVidVersionGLSLmin, m_nColorBits, m_nDepthBits, m_nStencilBits),
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
} // PureHwVideoImpl()


PureHwVideoImpl::PureHwVideoImpl(const PureHwVideoImpl&) :
    sharedSettings( PureSharedSettings::createAndGet() ),
    discoverGL_1_1(sVidFeaturesOGL, sVidFeaturesWGL, sVidVersionOGL, nVidVersionGLSLmaj, nVidVersionGLSLmin),
    discoverGL_1_2(sVidFeaturesOGL, sVidFeaturesWGL, sVidVersionOGL, nVidVersionGLSLmaj, nVidVersionGLSLmin),
    discoverGL_1_3(sVidFeaturesOGL, sVidFeaturesWGL, sVidVersionOGL, nVidVersionGLSLmaj, nVidVersionGLSLmin, m_nColorBits, m_nDepthBits, m_nStencilBits),
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


PureHwVideoImpl& PureHwVideoImpl::operator=(const PureHwVideoImpl&)
{
    return *this;
}


PureHwVideoImpl::~PureHwVideoImpl()
{
    Deinitialize();
} // ~PureHwVideoImpl


/**
    Writes the given extension list to console.
*/
void PureHwVideoImpl::printExtensionList(const std::string& features, const char* endstring)
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
            // O(): CConsole known issue B) but we still use it here, reason is written a few lines later below ...
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
                // although usage of O() is not recommended when multiple threads are allowed to log due to
                // CConsole known issue B), I still leave this here ... because it is complicated to refactor
                // this part, because of another known issue D) which is about problem with how nbsp chars are
                // put into output html log file.
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
void PureHwVideoImpl::printOGLextensions()
{
    printExtensionList(sVidFeaturesOGL, "parsed %d GL extensions");
} // printOGLextensions()


/**
    Writes WGL extension list to console.
*/
void PureHwVideoImpl::printWGLextensions()
{
    printExtensionList(sVidFeaturesWGL, "parsed %d WGL extensions");
} // printWGLextensions()

/**
    Query viewport details.
    Note that it is highly recommended to call all DiscoverOpenGL_X_Y_availability() functions before calling any DiscoverXYZ() function.
    This feature is not bound to specific OpenGL version hence we keep this out of any DiscoverOpenGL_X_Y class.
*/
void PureHwVideoImpl::DiscoverViewportAvailability()
{
    getConsole().OLnOI("Viewport:");

    PureGLsnippets::ClearGLError();

    glGetIntegerv(GL_MAX_VIEWPORT_DIMS, tmpRet);
    if ( glGetError() == GL_NO_ERROR )
        getConsole().OLn("Maximum viewport size: %d x %d", tmpRet[0], tmpRet[1]);
    else getConsole().EOLn("Maximum viewport size: ? x ?");

    glGetIntegerv(GL_VIEWPORT_BOUNDS_RANGE, tmpRet);
    if ( glGetError() == GL_NO_ERROR )
    {
        getConsole().OLn("Viewport bounds range: [%d, %d]", tmpRet[0], tmpRet[1]);
        PureHwVideoDiscoverOpenGLbase::CheckAgainstStandardRange(tmpRet[0], tmpRet[1],
                                  GL_STANDARD_MIN_VIEWPORT_BOUNDS_RANGE[0],
                                  GL_STANDARD_MIN_VIEWPORT_BOUNDS_RANGE[1]);
    }
    else getConsole().EOLn("Viewport bounds range: [?, ?]");

    glGetIntegerv(GL_MAX_VIEWPORTS, tmpRet);
    if ( glGetError() == GL_NO_ERROR )
    {
        getConsole().OLn("Max viewports: %d", tmpRet[0]);
        PureHwVideoDiscoverOpenGLbase::CheckAgainstStandardMinValue(tmpRet[0], GL_STANDARD_MIN_VIEWPORTS);
    }
    else getConsole().EOLn("Max viewports: ?");

    getConsole().OLnOO("");
} // DiscoverViewportAvailability()


/**
    Query matrix stacks details.
    Note that it is highly recommended to call all DiscoverOpenGL_X_Y_availability() functions before calling any DiscoverXYZ() function.
    This feature is not bound to specific OpenGL version hence we keep this out of any DiscoverOpenGL_X_Y class.
*/
void PureHwVideoImpl::DiscoverMatrixStacksAvailability()
{
    getConsole().OLnOI("Matrix stacks:");

    glGetIntegerv(GL_MAX_PROJECTION_STACK_DEPTH, tmpRet);
    getConsole().OLn("Max projection matrix stack depth: %d", tmpRet[0]);
    PureHwVideoDiscoverOpenGLbase::CheckAgainstStandardMinValue(tmpRet[0], GL_STANDARD_MIN_MATRIX_STACK_DEPTH_PROJECTION);

    glGetIntegerv(GL_MAX_MODELVIEW_STACK_DEPTH, tmpRet);
    getConsole().OLn("Max modelview matrix stack depth: %d", tmpRet[0]);
    PureHwVideoDiscoverOpenGLbase::CheckAgainstStandardMinValue(tmpRet[0], GL_STANDARD_MIN_MATRIX_STACK_DEPTH_MODELVIEW);

    glGetIntegerv(GL_MAX_COLOR_MATRIX_STACK_DEPTH, tmpRet);
    getConsole().OLn("Max color matrix stack depth: %d", tmpRet[0]);
    PureHwVideoDiscoverOpenGLbase::CheckAgainstStandardMinValue(tmpRet[0], GL_STANDARD_MIN_MATRIX_STACK_DEPTH_COLOR);

    glGetIntegerv(GL_MAX_TEXTURE_STACK_DEPTH, tmpRet);
    getConsole().OLn("Max texture stack depth: %d", tmpRet[0]);
    PureHwVideoDiscoverOpenGLbase::CheckAgainstStandardMinValue(tmpRet[0], GL_STANDARD_MIN_MATRIX_STACK_DEPTH_TEXTURE);

    glGetIntegerv(GL_MAX_ATTRIB_STACK_DEPTH, tmpRet);
    getConsole().OLn("Max server attribute stack depth: %d", tmpRet[0]);
    PureHwVideoDiscoverOpenGLbase::CheckAgainstStandardMinValue(tmpRet[0], GL_STANDARD_MIN_STACK_DEPTH_ATTRIB);

    glGetIntegerv(GL_MAX_CLIENT_ATTRIB_STACK_DEPTH, tmpRet);
    getConsole().OLn("Max client attribute stack depth: %d", tmpRet[0]);
    PureHwVideoDiscoverOpenGLbase::CheckAgainstStandardMinValue(tmpRet[0], GL_STANDARD_MIN_STACK_DEPTH_CLIENT_ATTRIB);

    getConsole().OLnOO("");
} // DiscoverMatrixStacksAvailability()


/**
    Query framebuffer and clipping details.
    Note that it is highly recommended to call all DiscoverOpenGL_X_Y_availability() functions before calling any DiscoverXYZ() function.
    This feature is not bound to specific OpenGL version hence we keep this out of any DiscoverOpenGL_X_Y class.
*/
void PureHwVideoImpl::DiscoverFramebufferAndClipping()
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
    PureHwVideoDiscoverOpenGLbase::CheckAgainstStandardMinValue(tmpRet[0], GL_STANDARD_MIN_CLIPPING_PLANES);

    getConsole().OLnOO("");
} // DiscoverFrambufferAndClipping()

/**
    Query compiled vertex arrays availability.
    Note that it is highly recommended to call all DiscoverOpenGL_X_Y_availability() functions before calling any DiscoverXYZ() function.
    This feature is not bound to specific OpenGL version hence we keep this out of any DiscoverOpenGL_X_Y class.
*/
void PureHwVideoImpl::DiscoverCompiledVertexArraysAvailability()
{
    // bSuppCompiledVertexArrays is not set before, we need to check extension and funcpointers here
    if ( bSuppCompiledVertexArrays = PureHwVideoDiscoverOpenGLbase::isExtensionSupported("GL_EXT_compiled_vertex_array", sVidFeaturesOGL, sVidFeaturesWGL) )
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
void PureHwVideoImpl::DiscoverNVVertexArrayRangeAvailability()
{
    // bSuppNVVertexArrayRange is not set before, we need to check extension and funcpointers here
    if ( bSuppNVVertexArrayRange = PureHwVideoDiscoverOpenGLbase::isExtensionSupported("GL_NV_vertex_array_range", sVidFeaturesOGL, sVidFeaturesWGL) )
    {
        if ( bSuppNVVertexArrayRange = (glFlushVertexArrayRangeNV && glVertexArrayRangeNV) )
        {
            getConsole().SOLn("nVidia Vertex Array Range supported!");

            if ( bSuppNVVertexArrayRange2 = PureHwVideoDiscoverOpenGLbase::isExtensionSupported("GL_NV_vertex_array_range2", sVidFeaturesOGL, sVidFeaturesWGL) )
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
void PureHwVideoImpl::DiscoverNVElementArrayAvailability()
{
    if ( bSuppNVElementArray = PureHwVideoDiscoverOpenGLbase::isExtensionSupported("GL_NV_element_array", sVidFeaturesOGL, sVidFeaturesWGL) )
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
void PureHwVideoImpl::DiscoverATIVertexArrayObjectAvailability()
{
    // bSuppATIVertexArrayObject is not set before by DiscoverOpenGL_X_Y_availability(), we need to check extension and funcpointers here
    if ( bSuppATIVertexArrayObject = PureHwVideoDiscoverOpenGLbase::isExtensionSupported("GL_ATI_vertex_array_object", sVidFeaturesOGL, sVidFeaturesWGL) )
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
void PureHwVideoImpl::DiscoverATIMapObjectBufferAvailability()
{
    // bSuppATIMapObjectBuffer is not set before by DiscoverOpenGL_X_Y_availability(), we need to check extension and funcpointers here
    if ( bSuppATIMapObjectBuffer = PureHwVideoDiscoverOpenGLbase::isExtensionSupported("GL_ATI_map_object_buffer", sVidFeaturesOGL, sVidFeaturesWGL) )
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
void PureHwVideoImpl::DiscoverATIElementArrayAvailability()
{
    // bSuppATIElementArray is not set before by DiscoverOpenGL_X_Y_availability(), we need to check extension and funcpointers here
    if ( bSuppATIElementArray = PureHwVideoDiscoverOpenGLbase::isExtensionSupported("GL_ATI_element_array", sVidFeaturesOGL, sVidFeaturesWGL) )
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
void PureHwVideoImpl::DiscoverATITruFormAvailability()
{
    // bSuppATITruForm is not set before by DiscoverOpenGL_X_Y_availability(), we need to check extension and funcpointers here
    if ( bSuppATITruForm = PureHwVideoDiscoverOpenGLbase::isExtensionSupported("GL_ATI_pn_triangles", sVidFeaturesOGL, sVidFeaturesWGL) )
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
void PureHwVideoImpl::DiscoverVertexRenderingAvailability()
{
    getConsole().OLnOI("Vertex Rendering Caps:");

    glGetIntegerv(GL_MAX_LIST_NESTING, tmpRet);
    getConsole().OLn("Max display list nesting level: %d", tmpRet[0]);
    PureHwVideoDiscoverOpenGLbase::CheckAgainstStandardMinValue(tmpRet[0], GL_STANDARD_MIN_DISPLAY_LIST_NESTING_LEVEL);

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
void PureHwVideoImpl::DiscoverAnisoFilteringAvailability()
{
    // bSuppAnisoFiltering is not set before by DiscoverOpenGL_X_Y_availability(), we need to check extension here
    if ( bSuppAnisoFiltering = PureHwVideoDiscoverOpenGLbase::isExtensionSupported("GL_EXT_texture_filter_anisotropic", sVidFeaturesOGL, sVidFeaturesWGL) )  // todo: opengl 4.6 added support for GL_ARB_texture_filter_anisotropic
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
            PureGLsnippets::ClearGLError();
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
void PureHwVideoImpl::DiscoverTexturingAvailability()
{
    getConsole().OLnOI("Texturing Caps:");

    PureGLsnippets::ClearGLError();

    glGetFloatv(GL_MAX_TEXTURE_LOD_BIAS_EXT, tmpRetf);
    if ( glGetError() == GL_NO_ERROR )
    {
        getConsole().OLn("Max texture LOD bias: %f", tmpRetf[0]);
        PureHwVideoDiscoverOpenGLbase::CheckAgainstStandardMinValuef(tmpRetf[0], GL_STANDARD_MIN_TEXTURE_LOD_BIAS);
    }
    else getConsole().EOLn("Max texture LOD bias: ?");

    glGetIntegerv(GL_MAX_TEXTURE_SIZE, tmpRet);
    if ( glGetError() == GL_NO_ERROR )
    {
        getConsole().OLn("Max texture size: %d x %d", tmpRet[0], tmpRet[0]);
        PureHwVideoDiscoverOpenGLbase::CheckAgainstStandardMinValue(tmpRet[0], GL_STANDARD_MIN_TEXTURE_SIZE);
    }
    else getConsole().EOLn("Max texture size: ? x ?");
    
    glGetIntegerv(GL_MAX_RECTANGLE_TEXTURE_SIZE_ARB, tmpRet);
    if ( glGetError() == GL_NO_ERROR )
    {
        getConsole().OLn("Max rectangle texture size: %d x %d", tmpRet[0], tmpRet[0]);
        PureHwVideoDiscoverOpenGLbase::CheckAgainstStandardMinValue(tmpRet[0], GL_STANDARD_MIN_RECTANGLE_TEXTURE_SIZE);
    }
    else getConsole().EOLn("Max rectangle texture size: ? x ?");

    glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE_EXT, tmpRet);
    if ( glGetError() == GL_NO_ERROR )
    {
        getConsole().OLn("Max 3D texture size: %d x %d x %d", tmpRet[0], tmpRet[0], tmpRet[0]);
        PureHwVideoDiscoverOpenGLbase::CheckAgainstStandardMinValue(tmpRet[0], GL_STANDARD_MIN_3D_TEXTURE_SIZE);
    }
    else getConsole().EOLn("Max 3D texture size: ? x ? x ?");
    
    glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE_EXT, tmpRet);
    if ( glGetError() == GL_NO_ERROR )
    {
        getConsole().OLn("Max cube map texture size: %d x %d", tmpRet[0], tmpRet[0]);
        PureHwVideoDiscoverOpenGLbase::CheckAgainstStandardMinValue(tmpRet[0], GL_STANDARD_MIN_CUBE_MAP_TEXTURE_SIZE);
    }
    else getConsole().EOLn("Max cube map texture size: ? x ?");

    discoverGL_1_3.DiscoverMultiTexturingAvailability();
    DiscoverAnisoFilteringAvailability();  

    if ( bSuppHWmipmapGen = PureHwVideoDiscoverOpenGLbase::isExtensionSupported("GL_SGIS_generate_mipmap", sVidFeaturesOGL, sVidFeaturesWGL) )
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

void PureHwVideoImpl::DiscoverOpenGLavailability()
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
   PureHwVideo
   ###########################################################################
*/


// ############################### PUBLIC ################################


/** 
    Gets the singleton instance.
*/
PureHwVideo& PureHwVideo::get()
{
    return PureHwVideoImpl::get();
} // get()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################