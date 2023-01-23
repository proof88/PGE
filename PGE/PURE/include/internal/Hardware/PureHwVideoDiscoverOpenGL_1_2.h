#pragma once

/*
    ###################################################################################
    PureHwVideoDiscoverOpenGL_1_2.h
    This file is part of PURE.
    Internal header.
    Pure OpenGL 1.2 capabilities class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../../external/PureAllHeaders.h"
#include "PureHwVideoDiscoverOpenGLbase.h"
#include "../PureGLsnippets.h"
#include "../PureGLextensionFuncs.h"

#include <vector>

/**
    Pure OpenGL 1.2 capabilities class.
*/
class PureHwVideoDiscoverOpenGL_1_2 :
    public PureHwVideoDiscoverOpenGLbase
{
#ifdef PURE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureHwVideoDiscoverOpenGL_1_2 is included")
#endif

public:
    PureHwVideoDiscoverOpenGL_1_2(
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
        bSuppBGRAtex = false;
        bSuppDrawRangeElements = false;
    };

    virtual ~PureHwVideoDiscoverOpenGL_1_2() {};

    virtual std::string getVersion() const
    {
        return "1.2";
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

        functions.push_back( glTexImage3DEXT );
        functions.push_back( glTexSubImage3DEXT );
        result &= printExtensionSupported("3D tex is ", "GL_EXT_texture3D", functions);

        bSuppBGRAtex = printExtensionSupported("BGRA px formats are ", "GL_EXT_bgra", functions);
        result &= bSuppBGRAtex;

        result &= printExtensionSupported("Packed px formats are ", "GL_EXT_packed_pixels", functions);

        result &= printExtensionSupported("Normal rescaling is ", "GL_EXT_rescale_normal", functions);

        result &= printExtensionSupported("Separate spec clr is ", "GL_EXT_separate_specular_color", functions);

        bool bTexEdgeClamp = printExtensionSupported("Texcoord edge clamping is ", "GL_SGIS_texture_edge_clamp", functions);

        bTexEdgeClamp |= printExtensionSupported("Texcoord edge clamping is ", "GL_EXT_texture_edge_clamp", functions);
    
        result &= bTexEdgeClamp;

        result &= printExtensionSupported("Tex LOD control is ", "GL_SGIS_texture_lod", functions);

        functions.push_back( glDrawRangeElementsEXT );
        bSuppDrawRangeElements = printExtensionSupported("VA draw element rng is ", "GL_EXT_draw_range_elements", functions);

        result &= bSuppDrawRangeElements;

        getConsole().OO();

        return result;
    }

    /**
        Query draw range elements availability.
    */
    void DiscoverDrawRangeElementsAvailability()
    {
        if ( bSuppDrawRangeElements )
        {
            PureGLsnippets::ClearGLError();

            glGetIntegerv(GL_MAX_ELEMENTS_VERTICES_EXT, &(tmpRet[0]));
            if ( (glGetError() != GL_NO_ERROR) || (tmpRet[0] < 1) )
            {
                getConsole().EOLn("DrawRangeElements ext. is exported, but MRVAV is %d.", tmpRet[0]);
                getConsole().EOLn("DrawRangeElements is disabled, upgrade graphics card driver!");
                bSuppDrawRangeElements = false;
                PureGLsnippets::ClearGLError();
                return;
            }

            glGetIntegerv(GL_MAX_ELEMENTS_INDICES_EXT, &(tmpRet[1]));
            if ( (glGetError() != GL_NO_ERROR) || (tmpRet[1] < 1) )
            {
                getConsole().EOLn("DrawRangeElements ext. is exported, but MRVAI is %d.", tmpRet[1]);
                getConsole().EOLn("DrawRangeElements is disabled, upgrade graphics card driver!");
                bSuppDrawRangeElements = false;
                PureGLsnippets::ClearGLError();
                return;
            }
        
            getConsole().SOLn("DrawRangeElements supported!");
            getConsole().OLn("  Max recommended vertex array vertices: %d", tmpRet[0]);
            getConsole().OLn("  Max recommended vertex array indices: %d", tmpRet[1]);
        }
        else
            getConsole().EOLn("DrawRangeElements not supported!");
    } // DiscoverDrawRangeElementsAvailability()

    /**
        Gets whether uploading of native DIBs (BGRA) is supported or not.
        @return True if uploading of native DIBs (BGRA) is supported, otherwise false.
    */
    TPureBool isNativeDIBFormatSupported() const
    {
        return bSuppBGRAtex;
    } // isNativeDIBFormatSupported()

    /**
        Gets whether DrawRangeElements is supported or not.
    */
    TPureBool isDrawRangeElementsSupported() const
    {
        return bSuppDrawRangeElements;
    } // isDrawRangeElementsSupported()

protected:
    PureHwVideoDiscoverOpenGL_1_2()
    {
        bSuppBGRAtex = false;
        bSuppDrawRangeElements = false;
    };

    PureHwVideoDiscoverOpenGL_1_2(const PureHwVideoDiscoverOpenGL_1_2&) {};
    PureHwVideoDiscoverOpenGL_1_2& operator=(const PureHwVideoDiscoverOpenGL_1_2&) { return *this; };
    
private:
    TPureBool bSuppBGRAtex;               /**< Is uploading textures in BGRA-format available? */
    TPureBool bSuppDrawRangeElements;     /**< Is DrawRangeElements supported? */

};