#pragma once

/*
    ###################################################################################
    PRREhwVideoDiscoverOpenGL_3_3.h
    This file is part of PRRE.
    Internal header.
    PRRE OpenGL 3.3 capabilities class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../../external/PRREallHeaders.h"
#include "PRREhwVideoDiscoverOpenGLbase.h"

/**
    PRRE OpenGL 3.3 capabilities class.
*/
class PRREhwVideoDiscoverOpenGL_3_3 :
    public PRREhwVideoDiscoverOpenGLbase
{
#ifdef PRRE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PRREhwVideoDiscoverOpenGL_3_3 is included")
#endif

public:
    PRREhwVideoDiscoverOpenGL_3_3(
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

    virtual ~PRREhwVideoDiscoverOpenGL_3_3() {};

    virtual std::string getVersion() const
    {
        return "3.3";
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

        result &= printExtensionSupported("SL 3.30 is ", "GL_ARB_shader_bit_encoding", functions);

        functions.push_back(glBindFragDataLocationIndexed);
        functions.push_back(glGetFragDataIndex);
        result &= printExtensionSupported("Dual-src blending is ", "GL_ARB_blend_func_extended", functions);

        result &= printExtensionSupported("Shader-defined attr loc is ", "GL_ARB_explicit_attrib_location", functions);

        bSuppBooleanOcclusionQuery = printExtensionSupported("Boolean Occlusion Query is ", "GL_ARB_occlusion_query2", functions);
        result &= bSuppBooleanOcclusionQuery;

        functions.push_back(glGenSamplers);
        functions.push_back(glDeleteSamplers);
        functions.push_back(glIsSampler);              
        functions.push_back(glBindSampler);            
        functions.push_back(glSamplerParameteri);
        functions.push_back(glSamplerParameteriv);
        functions.push_back(glSamplerParameterf);
        functions.push_back(glSamplerParameterfv);
        functions.push_back(glSamplerParameterIiv);
        functions.push_back(glSamplerParameterIuiv);
        functions.push_back(glGetSamplerParameteriv);
        functions.push_back(glGetSamplerParameterIiv);
        functions.push_back(glGetSamplerParameterfv);
        functions.push_back(glGetSamplerParameterIuiv);
        result &= printExtensionSupported("Sampler objects are ", "GL_ARB_sampler_objects", functions);

        result &= printExtensionSupported("Tex unsigned 10.10.10.2 is ", "GL_ARB_texture_rgb10_a2ui", functions);

        bool bTexSwizzle = printExtensionSupported("Tex swizzle ARB is ", "GL_ARB_texture_swizzle", functions);

        bTexSwizzle |= printExtensionSupported("Tex swizzle EXT is ", "GL_EXT_texture_swizzle", functions);

        result &= bTexSwizzle;

        functions.push_back(glQueryCounter);
        functions.push_back(glGetQueryObjecti64v);
        functions.push_back(glGetQueryObjectui64v);
        result &= printExtensionSupported("Timer queries are ", "GL_ARB_timer_query", functions);

        functions.push_back(glVertexAttribDivisorARB);
        result &= printExtensionSupported("Instanced arrays are ", "GL_ARB_instanced_arrays", functions);

        functions.push_back(glVertexP2ui);
        functions.push_back(glVertexP2uiv);
        functions.push_back(glVertexP3ui);
        functions.push_back(glVertexP3uiv);
        functions.push_back(glVertexP4ui);
        functions.push_back(glVertexP4uiv);
        functions.push_back(glTexCoordP1ui);
        functions.push_back(glTexCoordP1uiv);
        functions.push_back(glTexCoordP2ui);
        functions.push_back(glTexCoordP2uiv);
        functions.push_back(glTexCoordP3ui);
        functions.push_back(glTexCoordP3uiv);
        functions.push_back(glTexCoordP4ui);
        functions.push_back(glTexCoordP4uiv);
        functions.push_back(glMultiTexCoordP1ui);
        functions.push_back(glMultiTexCoordP1uiv);
        functions.push_back(glMultiTexCoordP2ui);
        functions.push_back(glMultiTexCoordP2uiv);
        functions.push_back(glMultiTexCoordP3ui);
        functions.push_back(glMultiTexCoordP3uiv);
        functions.push_back(glMultiTexCoordP4ui);
        functions.push_back(glMultiTexCoordP4uiv);
        functions.push_back(glNormalP3ui);
        functions.push_back(glNormalP3uiv);
        functions.push_back(glColorP3ui);
        functions.push_back(glColorP3uiv); 
        functions.push_back(glColorP4ui);
        functions.push_back(glColorP4uiv); 
        functions.push_back(glSecondaryColorP3ui);
        functions.push_back(glSecondaryColorP3uiv);
        functions.push_back(glVertexAttribP1ui);
        functions.push_back(glVertexAttribP1uiv);
        functions.push_back(glVertexAttribP2ui);
        functions.push_back(glVertexAttribP2uiv);
        functions.push_back(glVertexAttribP3ui);
        functions.push_back(glVertexAttribP3uiv);
        functions.push_back(glVertexAttribP4ui);
        functions.push_back(glVertexAttribP4uiv);
        result &= printExtensionSupported("Vertex attributes 2.10.10.10 are ", "GL_ARB_vertex_type_2_10_10_10_rev", functions);

        getConsole().O("Expected GLSL version is ");
        if ( (nVidVersionGLSLmaj > 3) || ( (nVidVersionGLSLmaj == 3) && (nVidVersionGLSLmin >= 3) ) )
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
    } // discoverOpenGLavailability()

    /**
        Query boolean HW occlusion query availability.
        @param isRegularHwOcclusionQueryAvailable True if the original HW occlusion query functionality is supported.
               Higher-level logic has the correct value when it invokes this function.
    */
    void DiscoverBooleanHwOcclusionQueryAvailability(TPRREbool isRegularHwOcclusionQueryAvailable)
    {
        if ( bSuppBooleanOcclusionQuery )
        {
            if ( !isRegularHwOcclusionQueryAvailable )
            {
                getConsole().EOLn("Boolean occl. query ext. is exported, but original HW occl. query is unavailable!");
                getConsole().EOLn("Boolean Occlusion query is disabled, upgrade graphics card driver!");
                bSuppBooleanOcclusionQuery = false;
                return;
            }
        
            getConsole().SOLn("Boolean Occlusion query supported!");
        }
        else
            getConsole().EOLn("Boolean Occlusion query not supported!");
    } // DiscoverBooleanHwOcclusionQueryAvailability()

    /**
        Gets whether boolean (true/false) HW occlusion query is supported or not.
        @return True if boolean (true/false) HW occlusion query is supported, otherwise false.
    */
    TPRREbool isBooleanOcclusionQuerySupported() const
    {
        return bSuppBooleanOcclusionQuery;
    } // isOcclusionQuerySupported()

protected:
    PRREhwVideoDiscoverOpenGL_3_3()
    {
        bSuppBooleanOcclusionQuery = false;
    };

    PRREhwVideoDiscoverOpenGL_3_3(const PRREhwVideoDiscoverOpenGL_3_3&) {};
    PRREhwVideoDiscoverOpenGL_3_3& operator=(const PRREhwVideoDiscoverOpenGL_3_3&) { return *this; };

private:

    TPRREbool bSuppBooleanOcclusionQuery;        /**< Is true/false HW occlusion query supported? */
    
};