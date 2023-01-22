#pragma once

/*
    ###################################################################################
    PureHwVideoDiscoverOpenGL_4_2.h
    This file is part of Pure.
    Internal header.
    Pure OpenGL 4.2 capabilities class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../../external/PureAllHeaders.h"
#include "../../internal/PureGLextensionFuncs.h"
#include "PureHwVideoDiscoverOpenGLbase.h"

/**
    Pure OpenGL 4.2 capabilities class.
*/
class PureHwVideoDiscoverOpenGL_4_2 :
    public PureHwVideoDiscoverOpenGLbase
{
#ifdef Pure_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureHwVideoDiscoverOpenGL_4_2 is included")
#endif

public:
    PureHwVideoDiscoverOpenGL_4_2(
        const std::string& sVidFeaturesOGL,
        const std::string& sVidFeaturesWGL,
        const std::string& sVidVersionOGL,
        const TPureuint& nVidVersionGLSLmaj,
        const TPureuint& nVidVersionGLSLmin) :
    PureHwVideoDiscoverOpenGLbase(
        sVidFeaturesOGL,
        sVidFeaturesWGL,
        sVidVersionOGL,
        nVidVersionGLSLmaj,
        nVidVersionGLSLmin) {};

    virtual ~PureHwVideoDiscoverOpenGL_4_2() {};

    virtual std::string getVersion() const
    {
        return "4.2";
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

        if ( (nVidVersionGLSLmaj > 4) || ( (nVidVersionGLSLmaj == 4) && (nVidVersionGLSLmin >= 2) ) )
        {
            getConsole().SOLn("Expected GLSL version is supported!");
        }
        else
        {
            result &= false;
            getConsole().EOLn("Expected GLSL version is NOT supported!");
        }

        getConsole().OO();

        return result;
    }

protected:
    PureHwVideoDiscoverOpenGL_4_2() {};

    PureHwVideoDiscoverOpenGL_4_2(const PureHwVideoDiscoverOpenGL_4_2&) {};
    PureHwVideoDiscoverOpenGL_4_2& operator=(const PureHwVideoDiscoverOpenGL_4_2&) { return *this; };

private:
    
};