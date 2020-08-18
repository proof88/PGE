#pragma once

/*
    ###################################################################################
    PRREhwVideoDiscoverOpenGL_4_0.h
    This file is part of PRRE.
    Internal header.
    PRRE OpenGL 4.0 capabilities class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../PRREallHeaders.h"
#include "PRREhwVideoDiscoverOpenGLbase.h"

/**
    PRRE OpenGL 4.0 capabilities class.

    This class does not use any API directly.
*/
class PRREhwVideoDiscoverOpenGL_4_0 :
    public PRREhwVideoDiscoverOpenGLbase
{
#ifdef PRRE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PRREhwVideoDiscoverOpenGL_4_0 is included")
#endif

public:
    PRREhwVideoDiscoverOpenGL_4_0(
        const std::string& sVidFeaturesOGL,
        const std::string& sVidFeaturesWGL,
        const std::string& sVidVersionOGL,
        const TPRREuint& nVidVersionGLSLmaj,
        const TPRREuint& nVidVersionGLSLmin) :
    PRREhwVideoDiscoverOpenGLbase(
        sVidFeaturesOGL,
        sVidFeaturesWGL,
        sVidVersionOGL,
        nVidVersionGLSLmaj,
        nVidVersionGLSLmin) {};

    virtual ~PRREhwVideoDiscoverOpenGL_4_0() {};

    virtual std::string getVersion() const
    {
        return "4.0";
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
    }

protected:
    PRREhwVideoDiscoverOpenGL_4_0() {};

    PRREhwVideoDiscoverOpenGL_4_0(const PRREhwVideoDiscoverOpenGL_4_0&) {};
    PRREhwVideoDiscoverOpenGL_4_0& operator=(const PRREhwVideoDiscoverOpenGL_4_0&) { return *this; };

private:
    
};