#pragma once

/*
    ###################################################################################
    PureHwVideoDiscoverOpenGL_3_2.h
    This file is part of Pure.
    Internal header.
    Pure OpenGL 3.2 capabilities class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../../external/PureAllHeaders.h"
#include "../../internal/PureGLextensionFuncs.h"
#include "PureHwVideoDiscoverOpenGLbase.h"

/**
    Pure OpenGL 3.2 capabilities class.
*/
class PureHwVideoDiscoverOpenGL_3_2 :
    public PureHwVideoDiscoverOpenGLbase
{
#ifdef PURE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureHwVideoDiscoverOpenGL_3_2 is included")
#endif

public:
    PureHwVideoDiscoverOpenGL_3_2(
        const std::string& sVidFeaturesOGL,
        const std::string& sVidFeaturesWGL,
        const std::string& sVidVersionOGL,
        const TPureUInt& nVidVersionGLSLmaj,
        const TPureUInt& nVidVersionGLSLmin) :
    PureHwVideoDiscoverOpenGLbase(
        sVidFeaturesOGL,
        sVidFeaturesWGL,
        sVidVersionOGL,
        nVidVersionGLSLmaj,
        nVidVersionGLSLmin) {};

    virtual ~PureHwVideoDiscoverOpenGL_3_2() {};

    virtual std::string getVersion() const
    {
        return "3.2";
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

        if ( (nVidVersionGLSLmaj > 3) || ( (nVidVersionGLSLmaj == 3) && (nVidVersionGLSLmin >= 2) ) )
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
    PureHwVideoDiscoverOpenGL_3_2() {};

    PureHwVideoDiscoverOpenGL_3_2(const PureHwVideoDiscoverOpenGL_3_2&) {};
    PureHwVideoDiscoverOpenGL_3_2& operator=(const PureHwVideoDiscoverOpenGL_3_2&) { return *this; };

private:
    
};