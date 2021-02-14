#pragma once

/*
    ###################################################################################
    PRREhwVideoDiscoverOpenGL_2_1.h
    This file is part of PRRE.
    Internal header.
    PRRE OpenGL 2.1 capabilities class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../../external/PRREallHeaders.h"
#include "PRREhwVideoDiscoverOpenGLbase.h"

/**
    PRRE OpenGL 2.1 capabilities class.
*/
class PRREhwVideoDiscoverOpenGL_2_1 :
    public PRREhwVideoDiscoverOpenGLbase
{
#ifdef PRRE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PRREhwVideoDiscoverOpenGL_2_1 is included")
#endif

public:
    PRREhwVideoDiscoverOpenGL_2_1(
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

    virtual ~PRREhwVideoDiscoverOpenGL_2_1() {};

    virtual std::string getVersion() const
    {
        return "2.1";
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

        result &= printExtensionSupported("Px buff objects are ", "GL_ARB_pixel_buffer_object", functions);

        result &= printExtensionSupported("sRGB textures are ", "GL_EXT_texture_sRGB", functions);

        getConsole().O("Expected GLSL version is ");
        if ( (nVidVersionGLSLmaj > 2) || ( (nVidVersionGLSLmaj == 2) && (nVidVersionGLSLmin >= 1) ) )
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
    PRREhwVideoDiscoverOpenGL_2_1() {};

    PRREhwVideoDiscoverOpenGL_2_1(const PRREhwVideoDiscoverOpenGL_2_1&) {};
    PRREhwVideoDiscoverOpenGL_2_1& operator=(const PRREhwVideoDiscoverOpenGL_2_1&) { return *this; };

private:
    
};