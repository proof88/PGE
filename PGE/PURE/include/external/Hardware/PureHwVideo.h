#pragma once

/*
    ###################################################################################
    PureHwVideo.h
    This file is part of PURE.
    External header.
    Pure video capabilities class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../PureAllHeaders.h"
#include "PurehwBase.h"

/**
    Pure video capabilities class.
*/
class PureHwVideo :
    public PurehwBase
{
#ifdef PURE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureHwVideo is included")
#endif

public:
    static PureHwVideo& get();  /**< Gets the singleton instance. */

    // ---------------------------------------------------------------------------

    /** Sets members to real values within the singleton instance. */
    virtual void Initialize(
        HGLRC rc, HDC wnd_dc,
        TPureUInt nResX, TPureUInt nResY,
        TPureInt nColorBits, TPureInt nDepthBits,
        TPureInt nStencilBits, TPureInt nFSAAlevel ) = 0;

    virtual TPureBool  isAcceleratorDetected() const = 0;     /**< Gets whether video accelerator is detected or not. */
    virtual TPureUInt  getUsedVideoMemory() const = 0;        /**< Gets the amount of used VRAM. */
    virtual TPureUInt  getUsedTextureMemory() const = 0;      /**< Gets the amount uf used texture memory. */
    virtual TPureUInt  getColorBufferPixelCount() const = 0;  /**< Gets the number of pixels in the Frame Buffer (width x height). */
    virtual TPureUInt  getColorBufferSize() const = 0;        /**< Gets the amount of VRAM used by the Color Buffer in Bytes. */
    virtual TPureUInt  getDepthBufferSize() const = 0;        /**< Gets the amount of VRAM used by the Depth Buffer in Bytes. */
    virtual TPureUInt  getStencilBufferSize() const = 0;      /**< Gets the amount of VRAM used by the Stencil Buffer in Bytes. */
    virtual TPureUInt  getSampleBufferSize() const = 0;       /**< Gets the amount of VRAM used by the Sample Buffer in Bytes. */
    virtual TPureUInt  getUsedFrameBufferMemory() const = 0;  /**< Gets the amount of VRAM used by the Frame Buffer in Bytes. */
    virtual TPureBool  isVSyncSupported() const = 0;          /**< Gets whether VSync is supported or not. */
    virtual TPureBool  isMultiTexturingSupported() const = 0; /**< Gets whether multitexturing is supported or not. */
    virtual TPureInt   getTextureUnitsCount() const = 0;      /**< Gets the number of texture units. */
    virtual TPureBool  isHardwareMipMapGenerationSupported() const = 0; /**< Gets whether HW MIP map generation is supported or not. */
    virtual TPureBool  isNativeDIBFormatSupported() const = 0;          /**< Gets whether uploading of native DIBs (BGRA) is supported or not. */
    virtual TPureBool  isAnisoFilteringSupported() const = 0;           /**< Gets whether anisotropic filtering is supported or not. */
    virtual TPureFloat getMaximumAnisoFiltering() const = 0;            /**< Gets the maximum available level of anisotropy. */
    virtual TPureBool  isTextureCompressionSupported() const = 0;       /**< Gets whether texture compression is supported or not. */
    virtual TPureBool  isVTCSupported() const = 0;                      /**< Gets whether Volume Texture Compression is supported or not. */
    virtual TPureBool  is3DcSupported() const = 0;                      /**< Gets whether 3Dc/ATI2/DXN compression is supported or not. */
    virtual TPureBool  isRGTCSupported() const = 0;                     /**< Gets whether Red-Green compression is supported or not. */
    virtual TPureBool  isLATCSupported() const = 0;                     /**< Gets whether Luminance-Alpha compression is supported or not. */
    virtual TPureBool  isAdvancedPixelFormatSetSupported() const = 0;   /**< Gets whether advanced pixel formats supported or not. */
    virtual TPureBool  isFullSceneAntiAliasingSupported() const = 0;    /**< Gets whether FSAA is supported or not. */
    virtual TPureInt   getMaxSamplesCount() const = 0;                  /**< Gets the maximum FSAA level. */
    virtual TPureBool  isSamplesSupported(
        TPureInt numsamples) const = 0;                                 /**< Gets whether the given MSAA-level is supported or not. */
    virtual TPureBool  isMultiDrawArraysSupported() const = 0;          /**< Gets whether MultiDrawArrays is supported or not. */
    virtual TPureBool  isDrawRangeElementsSupported() const = 0;        /**< Gets whether DrawRangeElements is supported or not. */
    virtual TPureBool  isCompiledVertexArraySupported() const = 0;      /**< Gets whether Compiled Vertex Array is supported or not. */
    virtual TPureBool  isNVVertexArrayRangeSupported() const = 0;       /**< Gets whether the nVidia-specific Vertex Array Range is supported. */
    virtual TPureBool  isNVElementArraySupported() const = 0;           /**< Gets whether the nVidia-specific Element Array is supported. */
    virtual TPureBool  isATIVertexArrayObjectSupported() const = 0;     /**< Gets whether the ATI-specific Vertex Array Object is supported. */
    virtual TPureBool  isATIMapObjectBufferSupported() const = 0;       /**< Gets whether the ATI-specific MapObjectBuffer (for Vertex Array Object) is supported. */
    virtual TPureBool  isATIElementArraySupported() const = 0;          /**< Gets whether the ATI-specific Element Array is supported. */
    virtual TPureBool  isVertexBufferObjectSupported() const = 0;       /**< Gets whether Vertex Buffer Object is supported or not. */
    virtual TPureBool  isATITruFormSupported() const = 0;               /**< Gets whether the ATI-specific TruForm is supported. */
    virtual TPureBool  isOcclusionQuerySupported() const = 0;           /**< Gets whether Occlusion Query is supported. */
    virtual TPureBool  isBooleanOcclusionQuerySupported() const = 0;    /**< Gets whether Boolean Occlusion Query is supported. */
    virtual const std::string& getVideocardOSName() const = 0;          /**< Gets the Windows-based VGA name. */
    virtual const std::string& getVideocardOGLName() const = 0;         /**< Gets the OpenGL-based VGA name. */
    virtual const std::string& getVideocardOGLVersion() const = 0;      /**< Gets the OpenGL version supported by the driver. */
    virtual const std::string  getVideocardGLSLVersion() const = 0;     /**< Gets the GLSL version supported by the driver. */
    virtual const std::string& getVideocardOGLVendor() const = 0;       /**< Gets the OpenGL-based VGA vendor. */
    virtual const std::string& getVideocardOGLFeatureList() const = 0;  /**< Gets the OpenGL features supported by the driver. */
    virtual const std::string& getVideocardWGLFeatureList() const = 0;  /**< Gets the WGL features supported by the driver. */

    virtual void WriteStats() = 0;    /**< Writes statistics to the console. */

};
