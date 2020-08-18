#pragma once

/*
    ###################################################################################
    PRREhwVideoDiscoverOpenGL_4_1.h
    This file is part of PRRE.
    Internal header.
    PRRE OpenGL 4.1 capabilities class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../PRREallHeaders.h"
#include "PRREhwVideoDiscoverOpenGLbase.h"

/**
    PRRE OpenGL 4.1 capabilities class.

    This class does not use any API directly.
*/
class PRREhwVideoDiscoverOpenGL_4_1 :
    public PRREhwVideoDiscoverOpenGLbase
{
#ifdef PRRE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PRREhwVideoDiscoverOpenGL_4_1 is included")
#endif

public:
    PRREhwVideoDiscoverOpenGL_4_1(
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

    virtual ~PRREhwVideoDiscoverOpenGL_4_1() {};

    virtual std::string getVersion() const
    {
        return "4.1";
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

        functions.push_back(glGetProgramBinary);
        functions.push_back(glProgramBinary);
        functions.push_back(glProgramParameteri);
        result &= printExtensionSupported("Query and load a bin blob for prg objects is ", "GL_ARB_get_program_binary", functions);

        functions.push_back(glUseProgramStages);
        functions.push_back(glActiveShaderProgram);
        functions.push_back(glCreateShaderProgramv);
        functions.push_back(glBindProgramPipeline);
        functions.push_back(glDeleteProgramPipelines);
        functions.push_back(glGenProgramPipelines);
        functions.push_back(glIsProgramPipeline);
        functions.push_back(glGetProgramPipelineiv);
        functions.push_back(glProgramUniform1i);
        functions.push_back(glProgramUniform1iv);
        functions.push_back(glProgramUniform1f);
        functions.push_back(glProgramUniform1fv);
        functions.push_back(glProgramUniform1d);
        functions.push_back(glProgramUniform1dv);
        functions.push_back(glProgramUniform1ui);
        functions.push_back(glProgramUniform1uiv);
        functions.push_back(glProgramUniform2i);
        functions.push_back(glProgramUniform2iv);
        functions.push_back(glProgramUniform2f);
        functions.push_back(glProgramUniform2fv);
        functions.push_back(glProgramUniform2d);
        functions.push_back(glProgramUniform2dv);
        functions.push_back(glProgramUniform2ui);
        functions.push_back(glProgramUniform2uiv);
        functions.push_back(glProgramUniform3i);
        functions.push_back(glProgramUniform3iv);
        functions.push_back(glProgramUniform3f);
        functions.push_back(glProgramUniform3fv);
        functions.push_back(glProgramUniform3d);
        functions.push_back(glProgramUniform3dv);
        functions.push_back(glProgramUniform3ui);
        functions.push_back(glProgramUniform3uiv);
        functions.push_back(glProgramUniform4i);
        functions.push_back(glProgramUniform4iv);
        functions.push_back(glProgramUniform4f);
        functions.push_back(glProgramUniform4fv);
        functions.push_back(glProgramUniform4d);
        functions.push_back(glProgramUniform4dv);
        functions.push_back(glProgramUniform4ui);
        functions.push_back(glProgramUniform4uiv);
        functions.push_back(glProgramUniformMatrix2fv);
        functions.push_back(glProgramUniformMatrix3fv);
        functions.push_back(glProgramUniformMatrix4fv);
        functions.push_back(glProgramUniformMatrix2dv);
        functions.push_back(glProgramUniformMatrix3dv);
        functions.push_back(glProgramUniformMatrix4dv);
        functions.push_back(glProgramUniformMatrix2x3fv);
        functions.push_back(glProgramUniformMatrix3x2fv);
        functions.push_back(glProgramUniformMatrix2x4fv);
        functions.push_back(glProgramUniformMatrix4x2fv);
        functions.push_back(glProgramUniformMatrix3x4fv);
        functions.push_back(glProgramUniformMatrix4x3fv);
        functions.push_back(glProgramUniformMatrix2x3dv);
        functions.push_back(glProgramUniformMatrix3x2dv);
        functions.push_back(glProgramUniformMatrix2x4dv);
        functions.push_back(glProgramUniformMatrix4x2dv);
        functions.push_back(glProgramUniformMatrix3x4dv);
        functions.push_back(glProgramUniformMatrix4x3dv);
        functions.push_back(glValidateProgramPipeline);
        functions.push_back(glGetProgramPipelineInfoLog);
        result &= printExtensionSupported("Bind prg indi to prog stages is ", "GL_ARB_separate_shader_objects", functions);

        functions.push_back(glReleaseShaderCompiler);
        functions.push_back(glShaderBinary);
        functions.push_back(glGetShaderPrecisionFormat);
        functions.push_back(glDepthRangef);
        functions.push_back(glClearDepthf);
        result &= printExtensionSupported("Func from OpenGL ES 2.0 is ", "GL_ARB_ES2_compatibility", functions);

        result &= printExtensionSupported("Prec reqs for sev FP ops are ", "GL_ARB_shader_precision", functions);

        result &= printExtensionSupported("64-bit FP comp VS inputs are ", "GL_ARB_vertex_attrib_64bit", functions);

        functions.push_back(glViewportArrayv);
        functions.push_back(glViewportIndexedf);
        functions.push_back(glViewportIndexedfv);
        functions.push_back(glScissorArrayv);
        functions.push_back(glScissorIndexed);
        functions.push_back(glScissorIndexedv);
        functions.push_back(glDepthRangeArrayv);
        functions.push_back(glDepthRangeIndexed);
        functions.push_back(glGetFloati_v);
        functions.push_back(glGetDoublei_v);
        result &= printExtensionSupported("Multi viewports for GS are ", "GL_ARB_viewport_array", functions);

        getConsole().O("Expected GLSL version is ");
        if ( (nVidVersionGLSLmaj > 4) || ( (nVidVersionGLSLmaj == 4) && (nVidVersionGLSLmin >= 1) ) )
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
    PRREhwVideoDiscoverOpenGL_4_1() {};

    PRREhwVideoDiscoverOpenGL_4_1(const PRREhwVideoDiscoverOpenGL_4_1&) {};
    PRREhwVideoDiscoverOpenGL_4_1& operator=(const PRREhwVideoDiscoverOpenGL_4_1&) { return *this; };

private:
    
};