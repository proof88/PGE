#pragma once

/*
    ###################################################################################
    PureHwVideoDiscoverOpenGL_3_1.h
    This file is part of PURE.
    Internal header.
    Pure OpenGL 3.1 capabilities class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../../external/PureAllHeaders.h"
#include "../../internal/PureGLextensionFuncs.h"
#include "PureHwVideoDiscoverOpenGLbase.h"

/**
    Pure OpenGL 3.1 capabilities class.
*/
class PureHwVideoDiscoverOpenGL_3_1 :
    public PureHwVideoDiscoverOpenGLbase
{
#ifdef PURE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureHwVideoDiscoverOpenGL_3_1 is included")
#endif

public:
    PureHwVideoDiscoverOpenGL_3_1(
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

    virtual ~PureHwVideoDiscoverOpenGL_3_1() {};

    virtual std::string getVersion() const
    {
        return "3.1";
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

        functions.push_back(glDrawArraysInstanced);
        functions.push_back(glDrawElementsInstanced);
        functions.push_back(glTexBuffer);
        functions.push_back(glPrimitiveRestartIndex);
        result &= printExtensionSupported("Uniform buff objects are ", "ARB_uniform_buffer_object", functions);

        functions.push_back(glDrawArraysInstancedARB);
        functions.push_back(glDrawElementsInstancedARB);
        result &= printExtensionSupported("Instanced render is ", "GL_ARB_draw_instanced", functions);

        functions.push_back(glCopyBufferSubData);
        result &= printExtensionSupported("Data copy bw buff objects is ", "GL_ARB_copy_buffer", functions);

        // I couldn't find GL_EXT_copy_buffer definition in glext.h, maybe it has the same value as the ARB version, need to check ...
        //result &= printExtensionSupported("Data copying between buffer objects is ", "GL_EXT_copy_buffer", functions);

        functions.push_back(glPrimitiveRestartNV);
        functions.push_back(glPrimitiveRestartIndexNV);
        result &= printExtensionSupported("Primitive restart is ", "GL_NV_primitive_restart", functions);

        functions.push_back(glTexBufferARB);
        result &= printExtensionSupported("Buff tex is ", "GL_ARB_texture_buffer_object", functions);

        result &= printExtensionSupported("Rect tex is ", "GL_ARB_texture_rectangle", functions);

        if ( (nVidVersionGLSLmaj > 3) || ( (nVidVersionGLSLmaj == 3) && (nVidVersionGLSLmin >= 1) ) )
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
    PureHwVideoDiscoverOpenGL_3_1() {};

    PureHwVideoDiscoverOpenGL_3_1(const PureHwVideoDiscoverOpenGL_3_1&) {};
    PureHwVideoDiscoverOpenGL_3_1& operator=(const PureHwVideoDiscoverOpenGL_3_1&) { return *this; };

private:
    
};