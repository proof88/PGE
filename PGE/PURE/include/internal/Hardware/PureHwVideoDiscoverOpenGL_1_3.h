#pragma once

/*
    ###################################################################################
    PureHwVideoDiscoverOpenGL_1_3.h
    This file is part of PURE.
    Internal header.
    Pure OpenGL 1.3 capabilities class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include <map>
#include "../../external/PureAllHeaders.h"
#include "../../internal/PureGLextensionFuncs.h"
#include "../../internal/PureGLsnippets.h"
#include "PureHwVideoDiscoverOpenGLbase.h"

/**
    Pure OpenGL 1.3 capabilities class.
*/
class PureHwVideoDiscoverOpenGL_1_3 :
    public PureHwVideoDiscoverOpenGLbase
{
#ifdef PURE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureHwVideoDiscoverOpenGL_1_3 is included")
#endif

public:
    PureHwVideoDiscoverOpenGL_1_3(
        const std::string& sVidFeaturesOGL,
        const std::string& sVidFeaturesWGL,
        const std::string& sVidVersionOGL,
        const TPureUInt& nVidVersionGLSLmaj,
        const TPureUInt& nVidVersionGLSLmin,
        const TPureInt&  nClrBits,
        const TPureInt&  nDpthBits,
        const TPureInt&  nStnclBits) :
    PureHwVideoDiscoverOpenGLbase(
        sVidFeaturesOGL,
        sVidFeaturesWGL,
        sVidVersionOGL,
        nVidVersionGLSLmaj,
        nVidVersionGLSLmin),
    nColorBits(nClrBits),
    nDepthBits(nDpthBits),
    nStencilBits(nStnclBits)
    {
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

        bSuppTexCompression = false;
        bSuppVTC = false;
        bSupp3Dc = false;
        bSuppLATC = false;
        bSuppMultiTexturing = false;
        nTMUcount = 0;
        nMSAAmaxLevel = 0;
        for (int i = 0; i < MSAA_SUPP_LEVELS_ARRAY_SIZE; i++)
            bMSAAlevels[i] = false;
    };

    virtual ~PureHwVideoDiscoverOpenGL_1_3() {};

    virtual std::string getVersion() const
    {
        return "1.3";
    }

    /**
        This method checks for related OpenGL extensions, however final verdict on extension support is declared by Discover...Availability() functions of this class.
        Some higher level manager first invokes this method, then later based on some logic it invokes the Discover...Availability() functions of this class.
        This class doesn't limit the way you invoke its public functions as this class is not intended to have that kind of logic, however the order is important
        from the perspective of the logic of the higher-level class, including logging.  
    */
    virtual bool discoverOpenGLavailability()
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
    }

    /**
        Query texture compression availability.
    */
    void DiscoverTextureCompressionAvailability()
    {
        // bSuppTexCompression is already set by DiscoverOpenGL_X_Y_availability()
        if ( bSuppTexCompression )
        {
            getConsole().SOLn("Texture compression supported!");
            glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS_ARB, tmpRet);
            if ( !PureGLsnippets::isGLErrorPresent() )
            {
                getConsole().OLn("  %d compressed texture formats available:", tmpRet[0]);
                CheckAgainstStandardMinValue(tmpRet[0], GL_STANDARD_MIN_COMPRESSED_TEXTURE_FORMATS);
                try
                {
                    GLint* compressedFormats = new GLint[tmpRet[0]];
                    glGetIntegerv(GL_COMPRESSED_TEXTURE_FORMATS_ARB, compressedFormats);
                    if ( !PureGLsnippets::isGLErrorPresent() )
                    {
                        getConsole().OI();
                        for (GLint i = 0; i < tmpRet[0]; i++)
                            if ( vTexComprFmts.find( compressedFormats[i] ) == vTexComprFmts.end() )
                                getConsole().OLn("  Unknown: %d", compressedFormats[i]);
                            else
                                getConsole().OLn("  %s", vTexComprFmts[ compressedFormats[i] ].c_str());   
                        getConsole().OO();
                    }
                    else
                    {
                        getConsole().EOLn("  glGetIntegerv() failed: %s", PureGLsnippets::getGLErrorTextFromEnum( PureGLsnippets::getLastSavedGLError() ));
                    }
                    delete[] compressedFormats;
                }
                catch (const std::bad_alloc&)
                {
                    getConsole().EOLn("  Compressed texture formats available: ? (error: failed to allocate array!)");
                }
            }
            else
                getConsole().EOLn("  Compressed texture formats available: ? (error: %s)", PureGLsnippets::getGLErrorTextFromEnum( PureGLsnippets::getLastSavedGLError() ));

            if ( bSuppVTC = isExtensionSupported("GL_NV_texture_compression_vtc") )
                getConsole().SOLn("  VTC also supported.");
            if ( bSupp3Dc = isExtensionSupported("GL_ATI_texture_compression_3dc") )
                getConsole().SOLn("  3Dc also supported.");
            if ( bSuppLATC = isExtensionSupported("GL_EXT_texture_compression_latc") || isExtensionSupported("GL_NV_texture_compression_latc") )
                getConsole().SOLn("  LATC also supported.");
        }
        else
            getConsole().EOLn("Texture compression not supported.");
    } // DiscoverTextureCompressionAvailability()

    /**
        Query multitexturing availability.
    */
    void DiscoverMultiTexturingAvailability()
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
                PureGLsnippets::ClearGLError();
            }
        }
        else
        {
            nTMUcount = 1;
            getConsole().EOLn("Multitexturing not supported.");
        }
    } // DiscoverMultiTexturingAvailability()

    /**
        Query FSAA availability.
    */
    void DiscoverFSAAavailability(HDC dc)
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
    
        TPureInt samples = MSAA_SUPP_LEVELS_ARRAY_SIZE;
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
        std::string sMsaaLevels;
        for (TPureInt i = 0; i < MSAA_SUPP_LEVELS_ARRAY_SIZE; i++)
        {
            if (bMSAAlevels[i])
            {
                sMsaaLevels += " " + std::to_string(i);
            }
        }
        getConsole().OLn("%s", sMsaaLevels.c_str());

        getConsole().OLnOO("");
    } // DiscoverFSAAavailability()

    /**
        Gets whether texture compression is supported or not.
        @return True if texture compression is supported, otherwise false.
    */
    TPureBool isTextureCompressionSupported() const
    {
        return bSuppTexCompression;
    } // isTextureCompressionSupported()

    /**
        Gets whether Volume Texture Compression is supported or not.
        @return True if Volume Texture Compression is supported, otherwise false.
    */
    TPureBool isVTCSupported() const
    {
        return bSuppVTC;
    } // isVTCSupported()

    /**
        Gets whether 3Dc/ATI2/DXN compression is supported or not.
        @return True if 3Dc/ATI2/DXN compression is supported, otherwise false.
    */
    TPureBool is3DcSupported() const
    {
        return bSupp3Dc;
    } // is3DcSupported()

    /**
        Gets whether Luminance-Alpha compression is supported or not.
        @return True if Luminance-Alpha compression is supported, otherwise false.
    */
    TPureBool isLATCSupported() const
    {
        return bSuppLATC;
    } // isLATCsupported()

    /**
        Gets whether multitexturing is supported or not.
        @return True if multitexturing is supported, false otherwise.
    */
    TPureBool isMultiTexturingSupported() const
    {
        return bSuppMultiTexturing;
    } // isMultiTexturingSupported()

    /**
        Gets the number of texture units.
        @return The number of texture units.
    */
    TPureInt getTextureUnitsCount() const
    {
        return nTMUcount;
    } // getTextureUnitsCount()

    /** 
        Gets whether FSAA is supported or not.
        @return True if FSAA is supported, otherwise false.
    */
    TPureBool isFullSceneAntiAliasingSupported() const
    {
        return  nMSAAmaxLevel > 1;
    } // isFullSceneAntiAliasingSupported()


    /**
        Gets the maximum FSAA level.
        @return The maximum available FSAA-level.
    */
    TPureInt getMaxSamplesCount() const
    {
        return nMSAAmaxLevel;
    } // getMaxSamplesCount()


    /**
        Gets whether the given MSAA-level is supported or not.
        @return True if the given MSAA-level is supported, otherwise false.
    */
    TPureBool isSamplesSupported(TPureInt numsamples) const
    {
        return (numsamples > 0) && ((numsamples < MSAA_SUPP_LEVELS_ARRAY_SIZE) ? bMSAAlevels[numsamples] : false);
    } // isSamplesSupported()

protected:
    PureHwVideoDiscoverOpenGL_1_3() :
        nColorBits(nColorBits),
        nDepthBits(nDepthBits),
        nStencilBits(nStencilBits)
    {
        bSuppTexCompression = false;
        bSuppVTC = false;
        bSupp3Dc = false;
        bSuppLATC = false;
        bSuppMultiTexturing = false;
        nTMUcount = 0;
        nMSAAmaxLevel = 0;
        for (int i = 0; i < MSAA_SUPP_LEVELS_ARRAY_SIZE; i++)
            bMSAAlevels[i] = false;
    };

    PureHwVideoDiscoverOpenGL_1_3(const PureHwVideoDiscoverOpenGL_1_3&) :
        nColorBits(nColorBits),
        nDepthBits(nDepthBits),
        nStencilBits(nStencilBits)
    {};
    PureHwVideoDiscoverOpenGL_1_3& operator=(const PureHwVideoDiscoverOpenGL_1_3&) { return *this; };

private:
    static const GLint    GL_STANDARD_MIN_COMPRESSED_TEXTURE_FORMATS = 4;
    static const TPureInt MSAA_SUPP_LEVELS_ARRAY_SIZE = 33;                /**< Size of array which holds the support of MSAA-levels. */

    const TPureInt&  nColorBits;
    const TPureInt&  nDepthBits;
    const TPureInt&  nStencilBits;

    // Would be better to use C++11 uniform initialization style to fill the map ...
    std::map<GLint, std::string> vTexComprFmts;  /**< List of compressed texture formats with names. */
    TPureBool bSuppTexCompression;        /**< Is texture compression available? */
    TPureBool bSuppVTC;                   /**< Is Volume Texture Compression available? */
    TPureBool bSupp3Dc;                   /**< Is 3Dc/ATI2/DXN compression supported? */
    TPureBool bSuppLATC;                  /**< Is Luminance-Alpha compression supported? */
    TPureBool bSuppMultiTexturing;        /**< Is multitexturing available (fixed func. pipeline)? */
    TPureInt  nTMUcount;                  /**< Number of texture units. */
    TPureBool bMSAAlevels[MSAA_SUPP_LEVELS_ARRAY_SIZE];  /**< Is the specified MSAA-level available? */
    TPureInt  nMSAAmaxLevel;              /**< Maximum level of MSAA. */
};