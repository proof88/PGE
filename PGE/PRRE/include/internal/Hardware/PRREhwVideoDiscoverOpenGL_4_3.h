#pragma once

/*
    ###################################################################################
    PRREhwVideoDiscoverOpenGL_4_3.h
    This file is part of PRRE.
    Internal header.
    PRRE OpenGL 4.3 capabilities class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../../external/PRREallHeaders.h"
#include "PRREhwVideoDiscoverOpenGLbase.h"

/**
    PRRE OpenGL 4.3 capabilities class.

    This class does not use any API directly.
*/
class PRREhwVideoDiscoverOpenGL_4_3 :
    public PRREhwVideoDiscoverOpenGLbase
{
#ifdef PRRE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PRREhwVideoDiscoverOpenGL_4_3 is included")
#endif

public:
    PRREhwVideoDiscoverOpenGL_4_3(
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

    virtual ~PRREhwVideoDiscoverOpenGL_4_3() {};

    virtual std::string getVersion() const
    {
        return "4.3";
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

        functions.push_back(glDebugMessageControl); 
        functions.push_back(glDebugMessageInsert);
        functions.push_back(glDebugMessageCallback);
        functions.push_back(glGetDebugMessageLog);
        functions.push_back(glPushDebugGroup);
        functions.push_back(glPopDebugGroup);
        functions.push_back(glObjectLabel);
        functions.push_back(glGetObjectLabel);
        functions.push_back(glObjectPtrLabel);
        functions.push_back(glGetObjectPtrLabel);
        result &= printExtensionSupported("Debug messaging is ", "GL_KHR_debug", functions);

        result &= printExtensionSupported("Multidim arrays are ", "GL_ARB_arrays_of_arrays", functions);

        functions.push_back(glClearBufferData);
        functions.push_back(glClearBufferSubData);
        functions.push_back(glClearNamedBufferDataEXT);
        functions.push_back(glClearNamedBufferSubDataEXT);
        result &= printExtensionSupported("Memset-style buff object clear is ", "GL_ARB_clear_buffer_object", functions);

        functions.push_back(glDispatchCompute);
        functions.push_back(glDispatchComputeIndirect);
        result &= printExtensionSupported("CS are ", "GL_ARB_compute_shader", functions);

        functions.push_back(glCopyImageSubData);
        result &= printExtensionSupported("Efficient image copy b/w img buffs is ", "GL_ARB_copy_image", functions);

        result &= printExtensionSupported("OpenGL ES 3.0 comp is ", "GL_ARB_ES3_compatibility", functions);

        result &= printExtensionSupported("Specify uniform loc in shader is ", "GL_ARB_explicit_uniform_location", functions);

        result &= printExtensionSupported("Lyr and viewp indices avail from FS is ", "GL_ARB_fragment_layer_viewport", functions);

        functions.push_back(glFramebufferParameteri);
        functions.push_back(glGetFramebufferParameteriv);
        functions.push_back(glNamedFramebufferParameteriEXT);
        functions.push_back(glGetNamedFramebufferParameterivEXT);
        result &= printExtensionSupported("Render to FBO having no attach's is ", "GL_ARB_framebuffer_no_attachments", functions);

        functions.push_back(glGetInternalformati64v);
        result &= printExtensionSupported("Gen'd queries about img fmts are ", "GL_ARB_internalformat_query2", functions);

        functions.push_back(glInvalidateTexSubImage);
        functions.push_back(glInvalidateTexImage);
        functions.push_back(glInvalidateBufferSubData); 
        functions.push_back(glInvalidateBufferData);
        functions.push_back(glInvalidateFramebuffer);
        functions.push_back(glInvalidateSubFramebuffer);
        result &= printExtensionSupported("Tex, buff object, and FB inv'ion are ", "GL_ARB_invalidate_subdata", functions);

        functions.push_back(glMultiDrawArraysIndirect);
        functions.push_back(glMultiDrawElementsIndirect);
        result &= printExtensionSupported("Multi indir render cmds by 1 draw cmd is ", "GL_ARB_multi_draw_indirect", functions);

        functions.push_back(glGetProgramInterfaceiv);
        functions.push_back(glGetProgramResourceIndex);
        functions.push_back(glGetProgramResourceName);
        functions.push_back(glGetProgramResourceiv);
        functions.push_back(glGetProgramResourceLocation);
        functions.push_back(glGetProgramResourceLocationIndex);
        result &= printExtensionSupported("API to get info about prg object IF is ", "GL_ARB_program_interface_query", functions);

        result &= printExtensionSupported("Get size of img from GLSL is ", "GL_ARB_shader_image_size", functions);

        functions.push_back(glShaderStorageBlockBinding);
        result &= printExtensionSupported("Buff object r/w from shader is ", "GL_ARB_shader_storage_buffer_object", functions);

        result &= printExtensionSupported("Stencil values from depth/stencil tex is ", "GL_ARB_stencil_texturing", functions);

        functions.push_back(glTexBufferRange);
        functions.push_back(glTextureBufferRangeEXT);
        result &= printExtensionSupported("Buff tex bound to a range of a buff object is ", "GL_ARB_texture_buffer_range", functions);

        result &= printExtensionSupported("Detect of avail mipmap pyramid by GLSL is ", "GL_ARB_texture_query_levels", functions);

        functions.push_back(glTexStorage2DMultisample);
        functions.push_back(glTexStorage3DMultisample);
        functions.push_back(glTextureStorage2DMultisampleEXT);
        functions.push_back(glTextureStorage3DMultisampleEXT);
        result &= printExtensionSupported("Immu storage for multisample tex is ", "GL_ARB_texture_storage_multisample", functions);

        functions.push_back(glTextureView);
        result &= printExtensionSupported("Tex views are ", "GL_ARB_texture_view", functions);

        functions.push_back(glBindVertexBuffer);
        functions.push_back(glVertexAttribFormat);
        functions.push_back(glVertexAttribIFormat);
        functions.push_back(glVertexAttribLFormat);
        functions.push_back(glVertexAttribBinding);
        functions.push_back(glVertexBindingDivisor);
        functions.push_back(glVertexArrayBindVertexBufferEXT);
        functions.push_back(glVertexArrayVertexAttribFormatEXT);
        functions.push_back(glVertexArrayVertexAttribIFormatEXT); 
        functions.push_back(glVertexArrayVertexAttribLFormatEXT); 
        functions.push_back(glVertexArrayVertexAttribBindingEXT); 
        functions.push_back(glVertexArrayVertexBindingDivisorEXT);
        result &= printExtensionSupported("Separ of vx fmt from buff object is ", "GL_ARB_vertex_attrib_binding", functions);

        result &= printExtensionSupported("API robust. (buff acc) is ", "GL_ARB_robust_buffer_access_behavior", functions);

        // We don't check for ARB_robustness_isolation because it is advertised by driver only if implementation supports it AND
        // if the context was created with the specific reset isolation bit set! We don't set that bit during init so cannot expect.
        // result &= printExtensionSupported("API robust. (GL isolation) is ", "GL_ARB_robustness_isolation", functions);
        // result &= printExtensionSupported("API robust. (WGL isolation) is ", "WGL_ARB_robustness_isolation", functions);
        // result &= printExtensionSupported("API robust. (GLX isolation) is ", "GLX_ARB_robustness_isolation", functions);

        getConsole().O("Expected GLSL version is ");
        if ( (nVidVersionGLSLmaj > 4) || ( (nVidVersionGLSLmaj == 4) && (nVidVersionGLSLmin >= 3) ) )
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
    PRREhwVideoDiscoverOpenGL_4_3() {};

    PRREhwVideoDiscoverOpenGL_4_3(const PRREhwVideoDiscoverOpenGL_4_3&) {};
    PRREhwVideoDiscoverOpenGL_4_3& operator=(const PRREhwVideoDiscoverOpenGL_4_3&) { return *this; };

private:
    
};