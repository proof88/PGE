#pragma once

/*
    ###################################################################################
    PRREhwVideo.h
    This file is part of PRRE.
    External header.
    PRRE video capabilities class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../PRREallHeaders.h"
#include "PRREhwBase.h"

/**
    PRRE video capabilities class.

    This class directly uses WinAPI, OpenGL, WGL.
*/
class PRREhwVideo :
    public PRREhwBase
{
#ifdef PRRE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PRREhwVideo is included")
#endif

public:
    static PRREhwVideo& get();  /**< Gets the singleton instance. */

    // ---------------------------------------------------------------------------

    /** Sets members to real values within the singleton instance. */
    virtual void Initialize(
        HGLRC rc, HDC wnd_dc,
        TPRREuint nResX, TPRREuint nResY,
        TPRREint nColorBits, TPRREint nDepthBits,
        TPRREint nStencilBits, TPRREint nFSAAlevel ) = 0;

    virtual TPRREbool  isAcceleratorDetected() const = 0;     /**< Gets whether video accelerator is detected or not. */
    virtual TPRREuint  getUsedVideoMemory() const = 0;        /**< Gets the amount of used VRAM. */
    virtual TPRREuint  getUsedTextureMemory() const = 0;      /**< Gets the amount uf used texture memory. */
    virtual TPRREuint  getColorBufferPixelCount() const = 0;  /**< Gets the number of pixels in the Frame Buffer (width x height). */
    virtual TPRREuint  getColorBufferSize() const = 0;        /**< Gets the amount of VRAM used by the Color Buffer in Bytes. */
    virtual TPRREuint  getDepthBufferSize() const = 0;        /**< Gets the amount of VRAM used by the Depth Buffer in Bytes. */
    virtual TPRREuint  getStencilBufferSize() const = 0;      /**< Gets the amount of VRAM used by the Stencil Buffer in Bytes. */
    virtual TPRREuint  getSampleBufferSize() const = 0;       /**< Gets the amount of VRAM used by the Sample Buffer in Bytes. */
    virtual TPRREuint  getUsedFrameBufferMemory() const = 0;  /**< Gets the amount of VRAM used by the Frame Buffer in Bytes. */
    virtual TPRREbool  isVSyncSupported() const = 0;          /**< Gets whether VSync is supported or not. */
    virtual TPRREbool  isMultiTexturingSupported() const = 0; /**< Gets whether multitexturing is supported or not. */
    virtual TPRREint   getTextureUnitsCount() const = 0;      /**< Gets the number of texture units. */
    virtual TPRREbool  isHardwareMipMapGenerationSupported() const = 0; /**< Gets whether HW MIP map generation is supported or not. */
    virtual TPRREbool  isNativeDIBFormatSupported() const = 0;          /**< Gets whether uploading of native DIBs (BGRA) is supported or not. */
    virtual TPRREbool  isAnisoFilteringSupported() const = 0;           /**< Gets whether anisotropic filtering is supported or not. */
    virtual TPRREfloat getMaximumAnisoFiltering() const = 0;            /**< Gets the maximum available level of anisotropy. */
    virtual TPRREbool  isTextureCompressionSupported() const = 0;       /**< Gets whether texture compression is supported or not. */
    virtual TPRREbool  isVTCSupported() const = 0;                      /**< Gets whether Volume Texture Compression is supported or not. */
    virtual TPRREbool  is3DcSupported() const = 0;                      /**< Gets whether 3Dc/ATI2/DXN compression is supported or not. */
    virtual TPRREbool  isRGTCSupported() const = 0;                     /**< Gets whether Red-Green compression is supported or not. */
    virtual TPRREbool  isLATCSupported() const = 0;                     /**< Gets whether Luminance-Alpha compression is supported or not. */
    virtual TPRREbool  isAdvancedPixelFormatSetSupported() const = 0;   /**< Gets whether advanced pixel formats supported or not. */
    virtual TPRREbool  isFullSceneAntiAliasingSupported() const = 0;    /**< Gets whether FSAA is supported or not. */
    virtual TPRREint   getMaxSamplesCount() const = 0;                  /**< Gets the maximum FSAA level. */
    virtual TPRREbool  isSamplesSupported(
        TPRREint numsamples) const = 0;                                 /**< Gets whether the given MSAA-level is supported or not. */
    virtual TPRREbool  isMultiDrawArraysSupported() const = 0;          /**< Gets whether MultiDrawArrays is supported or not. */
    virtual TPRREbool  isDrawRangeElementsSupported() const = 0;        /**< Gets whether DrawRangeElements is supported or not. */
    virtual TPRREbool  isCompiledVertexArraySupported() const = 0;      /**< Gets whether Compiled Vertex Array is supported or not. */
    virtual TPRREbool  isNVVertexArrayRangeSupported() const = 0;       /**< Gets whether the nVidia-specific Vertex Array Range is supported. */
    virtual TPRREbool  isNVElementArraySupported() const = 0;           /**< Gets whether the nVidia-specific Element Array is supported. */
    virtual TPRREbool  isATIVertexArrayObjectSupported() const = 0;     /**< Gets whether the ATI-specific Vertex Array Object is supported. */
    virtual TPRREbool  isATIMapObjectBufferSupported() const = 0;       /**< Gets whether the ATI-specific MapObjectBuffer (for Vertex Array Object) is supported. */
    virtual TPRREbool  isATIElementArraySupported() const = 0;          /**< Gets whether the ATI-specific Element Array is supported. */
    virtual TPRREbool  isVertexBufferObjectSupported() const = 0;       /**< Gets whether Vertex Buffer Object is supported or not. */
    virtual TPRREbool  isATITruFormSupported() const = 0;               /**< Gets whether the ATI-specific TruForm is supported. */
    virtual const std::string& getVideocardOSName() const = 0;          /**< Gets the Windows-based VGA name. */
    virtual const std::string& getVideocardOGLName() const = 0;         /**< Gets the OpenGL-based VGA name. */
    virtual const std::string& getVideocardOGLVersion() const = 0;      /**< Gets the OpenGL version supported by the driver. */
    virtual const std::string  getVideocardGLSLVersion() const = 0;     /**< Gets the GLSL version supported by the driver. */
    virtual const std::string& getVideocardOGLVendor() const = 0;       /**< Gets the OpenGL-based VGA vendor. */
    virtual const std::string& getVideocardOGLFeatureList() const = 0;  /**< Gets the OpenGL features supported by the driver. */
    virtual const std::string& getVideocardWGLFeatureList() const = 0;  /**< Gets the WGL features supported by the driver. */

    virtual void WriteStats() = 0;    /**< Writes statistics to the console. */

};
