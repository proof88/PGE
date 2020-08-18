#pragma once

/*
    ###################################################################################
    PRREhwVideoDiscoverOpenGL_3_0.h
    This file is part of PRRE.
    Internal header.
    PRRE OpenGL 3.0 capabilities class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../PRREallHeaders.h"
#include "PRREhwVideoDiscoverOpenGLbase.h"

/**
    PRRE OpenGL 3.0 capabilities class.

    This class does not use any API directly.
*/
class PRREhwVideoDiscoverOpenGL_3_0 :
    public PRREhwVideoDiscoverOpenGLbase
{
#ifdef PRRE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PRREhwVideoDiscoverOpenGL_3_0 is included")
#endif

public:
    PRREhwVideoDiscoverOpenGL_3_0(
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
        nVidVersionGLSLmin)
    {
        bSuppRGTC = false;
    };

    virtual ~PRREhwVideoDiscoverOpenGL_3_0() {};

    virtual std::string getVersion() const
    {
        return "3.0";
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

        functions.push_back(glIsRenderbuffer);
        functions.push_back(glBindRenderbuffer);
        functions.push_back(glDeleteRenderbuffers);
        functions.push_back(glGenRenderbuffers);
        functions.push_back(glRenderbufferStorage);
        functions.push_back(glGetRenderbufferParameteriv);
        functions.push_back(glIsFramebuffer);
        functions.push_back(glBindFramebuffer);
        functions.push_back(glDeleteFramebuffers);
        functions.push_back(glGenFramebuffers);
        functions.push_back(glCheckFramebufferStatus);
        functions.push_back(glFramebufferTexture1D);
        functions.push_back(glFramebufferTexture2D);
        functions.push_back(glFramebufferTexture3D);
        functions.push_back(glFramebufferRenderbuffer);
        functions.push_back(glGetFramebufferAttachmentParameteriv);
        functions.push_back(glGenerateMipmap);
        functions.push_back(glBlitFramebuffer);
        functions.push_back(glRenderbufferStorageMultisample);
        functions.push_back(glFramebufferTextureLayer);
        result &= printExtensionSupported("FB objects are ", "GL_ARB_framebuffer_object", functions);

        functions.push_back(glBindVertexArray);
        functions.push_back(glDeleteVertexArrays);
        functions.push_back(glGenVertexArrays);
        functions.push_back(glIsVertexArray);
        result &= printExtensionSupported("VA objects are ", "GL_ARB_vertex_array_object", functions);

        functions.push_back(glBeginConditionalRenderNV);
        functions.push_back(glEndConditionalRenderNV);
        result &= printExtensionSupported("Cond render is ", "GL_NV_conditional_render", functions);

        functions.push_back(glClampColorARB);
        result &= printExtensionSupported("FP internal formats are ", "GL_ARB_color_buffer_float", functions);

        /*
            GL_NV_depth_buffer_float is from 2006 by nVidia to support floating-point values in textures
            by adding some new internal formats.
            GL_ARB_depth_buffer_float is for the same purpose, it was approved by ARB in 2008.
            The NV extension needs 3 special functions because the core functions clamp input values by
            default to [0,1] range.
            However the ARB extensions doesn't need special functions since core functionality got changed
            then also so input values are not clamped anymore to [0,1] range.
            Summary: both can be used, but NV's needs the special functions. Hence we "OR" to "result" here.
        */

        functions.push_back(glDepthRangedNV);
        functions.push_back(glClearDepthdNV);
        functions.push_back(glDepthBoundsdNV);
        bool bDepthBuffFloat = printExtensionSupported("NV depth buff float is ", "GL_NV_depth_buffer_float", functions);

        bDepthBuffFloat |= printExtensionSupported("ARB depth buff float is ", "GL_ARB_depth_buffer_float", functions);

        result &= bDepthBuffFloat;

        /* Following 3 extensions are not interchangeable, need to check separately and "AND" to "result". */

        result &= printExtensionSupported("FP internal formats are ", "GL_ARB_texture_float", functions);

        result &= printExtensionSupported("FP internal formats are ", "GL_EXT_packed_float", functions);

        result &= printExtensionSupported("FP internal formats are ", "GL_EXT_texture_shared_exponent", functions);

        /* GL_NV_half_float and GL_ARB_half_float_vertex are not interchangeable.
           GL_ARB_half_float_vertex adds some extra to GL_NV_half_float. The latter was introduced earlier. */

        functions.push_back(glVertex2hNV);
        functions.push_back(glVertex2hvNV);
        functions.push_back(glVertex3hNV);
        functions.push_back(glVertex3hvNV);
        functions.push_back(glVertex4hNV);
        functions.push_back(glVertex4hvNV);
        functions.push_back(glNormal3hNV);
        functions.push_back(glNormal3hvNV);
        functions.push_back(glColor3hNV);
        functions.push_back(glColor3hvNV);
        functions.push_back(glColor4hNV);
        functions.push_back(glColor4hvNV);
        functions.push_back(glTexCoord1hNV);
        functions.push_back(glTexCoord1hvNV);
        functions.push_back(glTexCoord2hNV);
        functions.push_back(glTexCoord2hvNV);
        functions.push_back(glTexCoord3hNV);
        functions.push_back(glTexCoord3hvNV);
        functions.push_back(glTexCoord4hNV);
        functions.push_back(glTexCoord4hvNV);
        functions.push_back(glMultiTexCoord1hNV);
        functions.push_back(glMultiTexCoord1hvNV);
        functions.push_back(glMultiTexCoord2hNV);
        functions.push_back(glMultiTexCoord2hvNV);
        functions.push_back(glMultiTexCoord3hNV);
        functions.push_back(glMultiTexCoord3hvNV);
        functions.push_back(glMultiTexCoord4hNV);
        functions.push_back(glMultiTexCoord4hvNV);
        functions.push_back(glFogCoordhNV);          /* available only if EXT_fog_coord (GL 1.4) is supported, we expect it here */
        functions.push_back(glFogCoordhvNV);
        functions.push_back(glSecondaryColor3hNV);   /* available only if EXT_secondary_color (GL 1.4) is supported, we expect it here */
        functions.push_back(glSecondaryColor3hvNV);
        /*
            EXT_vertex_weighting: NVIDIA no longer supports this extension in driver updates
            after November 2002. Instead, use either ARB_vertex_program & NV_vertex_program.
        */
        // functions.push_back(glVertexWeighthNV);      /* available only if EXT_vertex_weighting is supported, so we don't expect it anymore */
        // functions.push_back(glVertexWeighthvNV);
        functions.push_back(glVertexAttrib1hNV);
        functions.push_back(glVertexAttrib1hvNV);
        functions.push_back(glVertexAttrib2hNV);
        functions.push_back(glVertexAttrib2hvNV);
        functions.push_back(glVertexAttrib3hNV);
        functions.push_back(glVertexAttrib3hvNV);
        functions.push_back(glVertexAttrib4hNV);
        functions.push_back(glVertexAttrib4hvNV);
        functions.push_back(glVertexAttribs1hvNV);
        functions.push_back(glVertexAttribs2hvNV);
        functions.push_back(glVertexAttribs3hvNV);
        functions.push_back(glVertexAttribs4hvNV);
        result &= printExtensionSupported("NV Half-floats are ", "GL_NV_half_float", functions);

        result &= printExtensionSupported("Half-float vx formats are ", "GL_ARB_half_float_vertex", functions);

        /* GL_NV_half_float and GL_ARB_half_float_pixel are not interchangeable.
           GL_ARB_half_float_pixel adds some extra to GL_NV_half_float. The latter was introduced earlier. */

        result &= printExtensionSupported("Half-float px formats are ", "GL_ARB_half_float_pixel", functions);

        /* I saw this GL_EXT_half_float_pixel somewhere but cannot find much about it on the internet.
           I'm skipping it now. Maybe it was the unapproved version of GL_ARB_half_float_pixel. */
        // result &= printExtensionSupported("Half-float px formats are ", "GL_EXT_half_float_pixel", functions);

        functions.push_back(glTexParameterIivEXT);
        functions.push_back(glTexParameterIuivEXT);
        functions.push_back(glGetTexParameterIivEXT);
        functions.push_back(glGetTexParameterIuivEXT);
        functions.push_back(glClearColorIiEXT);
        functions.push_back(glClearColorIuiEXT);
        result &= printExtensionSupported("Integral img formats are ", "GL_EXT_texture_integer", functions);

        result &= printExtensionSupported("Array tex is ", "GL_EXT_texture_array", functions);

        functions.push_back(glColorMaskIndexedEXT);
        functions.push_back(glGetBooleanIndexedvEXT);
        functions.push_back(glGetIntegerIndexedvEXT);
        functions.push_back(glEnableIndexedEXT);
        functions.push_back(glDisableIndexedEXT);
        functions.push_back(glIsEnabledIndexedEXT);
        result &= printExtensionSupported("Per-clr-attach blend enables and clr wmasks are ", "GL_EXT_draw_buffers2", functions);

        bSuppRGTC = printExtensionSupported("Red-Green tex comp (ARB) is ", "GL_ARB_texture_compression_rgtc", functions);
        /* I did not find difference between GL_EXT_texture_compression_rgtc and GL_ARB_texture_compression_rgtc
           so I assume "OR" will be ok for decision ... */
        bSuppRGTC |= printExtensionSupported("Red-Green tex comp (EXT) is ", "GL_EXT_texture_compression_rgtc", functions);

        result &= bSuppRGTC;

        // Transform feedback
        // Evolved from NV_transform_feedback for which we don't check here.
        functions.push_back(glBeginTransformFeedbackEXT);
        functions.push_back(glEndTransformFeedbackEXT);
        functions.push_back(glBindBufferRangeEXT);
        functions.push_back(glBindBufferOffsetEXT);
        functions.push_back(glBindBufferBaseEXT);
        functions.push_back(glTransformFeedbackVaryingsEXT);
        functions.push_back(glGetTransformFeedbackVaryingEXT);
        bool bXformFB = printExtensionSupported("Xform feedback is ", "GL_EXT_transform_feedback", functions);

        if ( !bXformFB )
        {   // if GL_EXT_transform_feedback is not exposed, let's check for GL_EXT_transform_feedback2 which is superset of GL_EXT_transform_feedback
            functions.push_back(glBeginTransformFeedbackEXT);
            functions.push_back(glEndTransformFeedbackEXT);
            functions.push_back(glBindBufferRangeEXT);
            functions.push_back(glBindBufferOffsetEXT);  /* <- dont know why this is NULL for me at runtime ... */
            functions.push_back(glBindBufferBaseEXT);
            functions.push_back(glTransformFeedbackVaryingsEXT);
            functions.push_back(glGetTransformFeedbackVaryingEXT);
            bXformFB = printExtensionSupported("Xform feedback is ", "GL_EXT_transform_feedback2", functions);
        }

        result &= bXformFB;

        // Unclear about difference between GL_ARB_framebuffer_sRGB and GL_EXT_framebuffer_sRGB, so
        // let's treat them as exchangeable ...
        bool bFBsRGB = printExtensionSupported("sRGB FB mode is ", "GL_ARB_framebuffer_sRGB", functions);

        bFBsRGB |= printExtensionSupported("sRGB FB mode is ", "GL_EXT_framebuffer_sRGB", functions);

        result &= bFBsRGB;

        getConsole().O("Expected GLSL version is ");
        if ( (nVidVersionGLSLmaj > 3) || ( (nVidVersionGLSLmaj == 3) && (nVidVersionGLSLmin >= 0) ) )
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

    /**
        Gets whether Red-Green compression is supported or not.
        @return True if Red-Green compression is supported, otherwise false.
    */
    TPRREbool isRGTCSupported() const
    {
        return bSuppRGTC;
    } // isRGTCSupported()

protected:
    PRREhwVideoDiscoverOpenGL_3_0()
    {
        bSuppRGTC = false;
    };

    PRREhwVideoDiscoverOpenGL_3_0(const PRREhwVideoDiscoverOpenGL_3_0&) {};
    PRREhwVideoDiscoverOpenGL_3_0& operator=(const PRREhwVideoDiscoverOpenGL_3_0&) { return *this; };

private:
    TPRREbool bSuppRGTC;                  /**< Is Red-Green compression supported? */
    
};