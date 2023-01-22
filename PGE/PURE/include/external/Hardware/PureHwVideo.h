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
#ifdef Pure_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureHwVideo is included")
#endif

public:
    static PureHwVideo& get();  /**< Gets the singleton instance. */

    // ---------------------------------------------------------------------------

    /** Sets members to real values within the singleton instance. */
    virtual void Initialize(
        HGLRC rc, HDC wnd_dc,
        TPureuint nResX, TPureuint nResY,
        TPureint nColorBits, TPureint nDepthBits,
        TPureint nStencilBits, TPureint nFSAAlevel ) = 0;

    virtual TPurebool  isAcceleratorDetected() const = 0;     /**< Gets whether video accelerator is detected or not. */
    virtual TPureuint  getUsedVideoMemory() const = 0;        /**< Gets the amount of used VRAM. */
    virtual TPureuint  getUsedTextureMemory() const = 0;      /**< Gets the amount uf used texture memory. */
    virtual TPureuint  getColorBufferPixelCount() const = 0;  /**< Gets the number of pixels in the Frame Buffer (width x height). */
    virtual TPureuint  getColorBufferSize() const = 0;        /**< Gets the amount of VRAM used by the Color Buffer in Bytes. */
    virtual TPureuint  getDepthBufferSize() const = 0;        /**< Gets the amount of VRAM used by the Depth Buffer in Bytes. */
    virtual TPureuint  getStencilBufferSize() const = 0;      /**< Gets the amount of VRAM used by the Stencil Buffer in Bytes. */
    virtual TPureuint  getSampleBufferSize() const = 0;       /**< Gets the amount of VRAM used by the Sample Buffer in Bytes. */
    virtual TPureuint  getUsedFrameBufferMemory() const = 0;  /**< Gets the amount of VRAM used by the Frame Buffer in Bytes. */
    virtual TPurebool  isVSyncSupported() const = 0;          /**< Gets whether VSync is supported or not. */
    virtual TPurebool  isMultiTexturingSupported() const = 0; /**< Gets whether multitexturing is supported or not. */
    virtual TPureint   getTextureUnitsCount() const = 0;      /**< Gets the number of texture units. */
    virtual TPurebool  isHardwareMipMapGenerationSupported() const = 0; /**< Gets whether HW MIP map generation is supported or not. */
    virtual TPurebool  isNativeDIBFormatSupported() const = 0;          /**< Gets whether uploading of native DIBs (BGRA) is supported or not. */
    virtual TPurebool  isAnisoFilteringSupported() const = 0;           /**< Gets whether anisotropic filtering is supported or not. */
    virtual TPurefloat getMaximumAnisoFiltering() const = 0;            /**< Gets the maximum available level of anisotropy. */
    virtual TPurebool  isTextureCompressionSupported() const = 0;       /**< Gets whether texture compression is supported or not. */
    virtual TPurebool  isVTCSupported() const = 0;                      /**< Gets whether Volume Texture Compression is supported or not. */
    virtual TPurebool  is3DcSupported() const = 0;                      /**< Gets whether 3Dc/ATI2/DXN compression is supported or not. */
    virtual TPurebool  isRGTCSupported() const = 0;                     /**< Gets whether Red-Green compression is supported or not. */
    virtual TPurebool  isLATCSupported() const = 0;                     /**< Gets whether Luminance-Alpha compression is supported or not. */
    virtual TPurebool  isAdvancedPixelFormatSetSupported() const = 0;   /**< Gets whether advanced pixel formats supported or not. */
    virtual TPurebool  isFullSceneAntiAliasingSupported() const = 0;    /**< Gets whether FSAA is supported or not. */
    virtual TPureint   getMaxSamplesCount() const = 0;                  /**< Gets the maximum FSAA level. */
    virtual TPurebool  isSamplesSupported(
        TPureint numsamples) const = 0;                                 /**< Gets whether the given MSAA-level is supported or not. */
    virtual TPurebool  isMultiDrawArraysSupported() const = 0;          /**< Gets whether MultiDrawArrays is supported or not. */
    virtual TPurebool  isDrawRangeElementsSupported() const = 0;        /**< Gets whether DrawRangeElements is supported or not. */
    virtual TPurebool  isCompiledVertexArraySupported() const = 0;      /**< Gets whether Compiled Vertex Array is supported or not. */
    virtual TPurebool  isNVVertexArrayRangeSupported() const = 0;       /**< Gets whether the nVidia-specific Vertex Array Range is supported. */
    virtual TPurebool  isNVElementArraySupported() const = 0;           /**< Gets whether the nVidia-specific Element Array is supported. */
    virtual TPurebool  isATIVertexArrayObjectSupported() const = 0;     /**< Gets whether the ATI-specific Vertex Array Object is supported. */
    virtual TPurebool  isATIMapObjectBufferSupported() const = 0;       /**< Gets whether the ATI-specific MapObjectBuffer (for Vertex Array Object) is supported. */
    virtual TPurebool  isATIElementArraySupported() const = 0;          /**< Gets whether the ATI-specific Element Array is supported. */
    virtual TPurebool  isVertexBufferObjectSupported() const = 0;       /**< Gets whether Vertex Buffer Object is supported or not. */
    virtual TPurebool  isATITruFormSupported() const = 0;               /**< Gets whether the ATI-specific TruForm is supported. */
    virtual TPurebool  isOcclusionQuerySupported() const = 0;           /**< Gets whether Occlusion Query is supported. */
    virtual TPurebool  isBooleanOcclusionQuerySupported() const = 0;    /**< Gets whether Boolean Occlusion Query is supported. */
    virtual const std::string& getVideocardOSName() const = 0;          /**< Gets the Windows-based VGA name. */
    virtual const std::string& getVideocardOGLName() const = 0;         /**< Gets the OpenGL-based VGA name. */
    virtual const std::string& getVideocardOGLVersion() const = 0;      /**< Gets the OpenGL version supported by the driver. */
    virtual const std::string  getVideocardGLSLVersion() const = 0;     /**< Gets the GLSL version supported by the driver. */
    virtual const std::string& getVideocardOGLVendor() const = 0;       /**< Gets the OpenGL-based VGA vendor. */
    virtual const std::string& getVideocardOGLFeatureList() const = 0;  /**< Gets the OpenGL features supported by the driver. */
    virtual const std::string& getVideocardWGLFeatureList() const = 0;  /**< Gets the WGL features supported by the driver. */

    virtual void WriteStats() = 0;    /**< Writes statistics to the console. */

};
