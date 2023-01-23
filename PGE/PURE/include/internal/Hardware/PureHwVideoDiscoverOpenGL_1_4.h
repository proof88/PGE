#pragma once

/*
    ###################################################################################
    PureHwVideoDiscoverOpenGL_1_4.h
    This file is part of PURE.
    Internal header.
    Pure OpenGL 1.4 capabilities class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../../external/PureAllHeaders.h"
#include "../../internal/PureGLextensionFuncs.h"
#include "PureHwVideoDiscoverOpenGLbase.h"

/**
    Pure OpenGL 1.2 capabilities class.
*/
class PureHwVideoDiscoverOpenGL_1_4 :
    public PureHwVideoDiscoverOpenGLbase
{
#ifdef PURE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureHwVideoDiscoverOpenGL_1_4 is included")
#endif

public:
    PureHwVideoDiscoverOpenGL_1_4(
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
        nVidVersionGLSLmin)
    {
        bSuppMultiDrawArrays = false;
    };

    virtual ~PureHwVideoDiscoverOpenGL_1_4() {};

    virtual std::string getVersion() const
    {
        return "1.4";
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

        result &= printExtensionSupported("Auto MIP map gen is ", "GL_SGIS_generate_mipmap", functions);

        result &= printExtensionSupported("Blend squaring is ", "GL_NV_blend_square", functions);

        result &= printExtensionSupported("Depth textures are ", "GL_ARB_depth_texture", functions);

        result &= printExtensionSupported("Depth shadows are ", "GL_ARB_shadow", functions);

        result &= printExtensionSupported("Fog coordinates are ", "GL_EXT_fog_coord", functions);

        functions.push_back( glMultiDrawArraysEXT );
        functions.push_back( glMultiDrawElementsEXT );
        bSuppMultiDrawArrays = printExtensionSupported("Multiple draw arrays EXT is ", "GL_EXT_multi_draw_arrays", functions);

        functions.push_back( glMultiDrawArraysEXT );
        functions.push_back( glMultiDrawElementsEXT );
        bSuppMultiDrawArrays |= printExtensionSupported("Multiple draw arrays SUN is ", "GL_SUN_multi_draw_arrays", functions);
    
        result &= bSuppMultiDrawArrays;

        functions.push_back( glPointParameterfARB );
        functions.push_back( glPointParameterfvARB );
        result &= printExtensionSupported("Point params are ", "GL_ARB_point_parameters", functions);

        functions.push_back( glSecondaryColor3bEXT );
        functions.push_back( glSecondaryColor3bvEXT );
        functions.push_back( glSecondaryColor3dEXT );
        functions.push_back( glSecondaryColor3dvEXT );
        functions.push_back( glSecondaryColor3fEXT );
        functions.push_back( glSecondaryColor3fvEXT );
        functions.push_back( glSecondaryColor3iEXT );
        functions.push_back( glSecondaryColor3ivEXT );
        functions.push_back( glSecondaryColor3sEXT );
        functions.push_back( glSecondaryColor3svEXT );
        functions.push_back( glSecondaryColor3ubEXT );
        functions.push_back( glSecondaryColor3ubvEXT );
        functions.push_back( glSecondaryColor3uiEXT );
        functions.push_back( glSecondaryColor3uivEXT );
        functions.push_back( glSecondaryColor3usEXT );
        functions.push_back( glSecondaryColor3usvEXT );
        functions.push_back( glSecondaryColorPointerEXT );
        result &= printExtensionSupported("Sec color is ", "GL_EXT_secondary_color", functions);

        functions.push_back( glBlendFuncSeparateEXT );
        result &= printExtensionSupported("Sep blend funcs are ", "GL_EXT_blend_func_separate", functions);

        result &= printExtensionSupported("Stencil wrap is ", "GL_EXT_stencil_wrap", functions);

        result &= printExtensionSupported("Tex xbar env mode is ", "GL_ARB_texture_env_crossbar", functions);

        result &= printExtensionSupported("Tex LOD bias is ", "GL_EXT_texture_lod_bias", functions);

        result &= printExtensionSupported("Tex ARB mirr rpt is ", "GL_ARB_texture_mirrored_repeat", functions);

        result &= printExtensionSupported("Tex IBM mirr rpt is ", "GL_IBM_texture_mirrored_repeat", functions);

        functions.push_back( glSecondaryColor3bEXT );
        functions.push_back( glSecondaryColor3bvEXT );
        functions.push_back( glSecondaryColor3dEXT );
        functions.push_back( glSecondaryColor3dvEXT );
        functions.push_back( glSecondaryColor3fEXT );
        functions.push_back( glSecondaryColor3fvEXT );
        functions.push_back( glSecondaryColor3iEXT );
        functions.push_back( glSecondaryColor3ivEXT );
        functions.push_back( glSecondaryColor3sEXT );
        functions.push_back( glSecondaryColor3svEXT );
        functions.push_back( glSecondaryColor3ubEXT );
        functions.push_back( glSecondaryColor3ubvEXT );
        functions.push_back( glSecondaryColor3uiEXT );
        functions.push_back( glSecondaryColor3uivEXT );
        functions.push_back( glSecondaryColor3usEXT );
        functions.push_back( glSecondaryColor3usvEXT );
        functions.push_back( glSecondaryColorPointerEXT );
        result &= printExtensionSupported("Wnd raster pos is ", "GL_ARB_window_pos", functions);

        getConsole().OO();

        return result;
    }

    /**
        Query multi draw arrays availability.
    */
    void DiscoverMultiDrawArraysAvailability()
    {
        // bSuppMultiDrawArrays is already set by DiscoverOpenGL_X_Y_availability()
        if ( bSuppMultiDrawArrays )
        {
            getConsole().SOLn("MultiDrawArrays supported!");
        }
        else
            getConsole().EOLn("MultiDrawArrays not supported!");
    } // DiscoverMultiDrawArraysAvailability()

    /**
        Gets whether MultiDrawArrays is supported or not.
    */
    TPureBool isMultiDrawArraysSupported() const
    {
        return bSuppMultiDrawArrays;
    } // isMultiDrawArraysSupported()

protected:
    PureHwVideoDiscoverOpenGL_1_4()
    {
        bSuppMultiDrawArrays = false;
    };

    PureHwVideoDiscoverOpenGL_1_4(const PureHwVideoDiscoverOpenGL_1_4&) {};
    PureHwVideoDiscoverOpenGL_1_4& operator=(const PureHwVideoDiscoverOpenGL_1_4&) { return *this; };

private:
    TPureBool bSuppMultiDrawArrays;       /**< Is MultiDrawArrays supported? */
    
};