#pragma once

/*
    ###################################################################################
    PureHwVideoDiscoverOpenGL_2_0.h
    This file is part of Pure.
    Internal header.
    Pure OpenGL 2.0 capabilities class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../../external/PureAllHeaders.h"
#include "../../internal/PureGLextensionFuncs.h"
#include "PureHwVideoDiscoverOpenGLbase.h"

/**
    Pure OpenGL 2.0 capabilities class.
*/
class PureHwVideoDiscoverOpenGL_2_0 :
    public PureHwVideoDiscoverOpenGLbase
{
#ifdef PURE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureHwVideoDiscoverOpenGL_2_0 is included")
#endif

public:
    PureHwVideoDiscoverOpenGL_2_0(
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

    virtual ~PureHwVideoDiscoverOpenGL_2_0() {};

    virtual std::string getVersion() const
    {
        return "2.0";
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
    }

protected:
    PureHwVideoDiscoverOpenGL_2_0() {};

    PureHwVideoDiscoverOpenGL_2_0(const PureHwVideoDiscoverOpenGL_2_0&) {};
    PureHwVideoDiscoverOpenGL_2_0& operator=(const PureHwVideoDiscoverOpenGL_2_0&) { return *this; };

private:
    
};