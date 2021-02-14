#pragma once

/*
    ###################################################################################
    PRREhwVideoDiscoverOpenGL_1_5.h
    This file is part of PRRE.
    Internal header.
    PRRE OpenGL 1.5 capabilities class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../../external/PRREallHeaders.h"
#include "PRREhwVideoDiscoverOpenGLbase.h"

/**
    PRRE OpenGL 1.5 capabilities class.
*/
class PRREhwVideoDiscoverOpenGL_1_5 :
    public PRREhwVideoDiscoverOpenGLbase
{
#ifdef PRRE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PRREhwVideoDiscoverOpenGL_1_5 is included")
#endif

public:
    PRREhwVideoDiscoverOpenGL_1_5(
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
        bSuppVertexBuffers = false;
    };

    virtual ~PRREhwVideoDiscoverOpenGL_1_5() {};

    virtual std::string getVersion() const
    {
        return "1.5";
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

        functions.push_back( glGenBuffersARB );
        functions.push_back( glBindBufferARB );
        functions.push_back( glBufferDataARB );
        functions.push_back( glBufferSubDataARB );
        functions.push_back( glGetBufferSubDataARB );
        functions.push_back( glDeleteBuffersARB );
        functions.push_back( glIsBufferARB );
        functions.push_back( glMapBufferARB );
        functions.push_back( glUnmapBufferARB );
        functions.push_back( glGetBufferParameterivARB );
        functions.push_back( glGetBufferPointervARB );
        bSuppVertexBuffers = printExtensionSupported("Buff objects are ", "GL_ARB_vertex_buffer_object", functions);
        result &= bSuppVertexBuffers;

        functions.push_back( glGenQueriesARB );
        functions.push_back( glDeleteQueriesARB );
        functions.push_back( glIsQueryARB );
        functions.push_back( glBeginQueryARB );
        functions.push_back( glEndQueryARB );
        functions.push_back( glGetQueryivARB );
        functions.push_back( glGetQueryObjectivARB );
        functions.push_back( glGetQueryObjectuivARB );
        result &= printExtensionSupported("Occlusion query is ", "GL_ARB_occlusion_query", functions);

        result &= printExtensionSupported("Shadow funcs are ", "GL_EXT_shadow_funcs", functions);

        getConsole().OO();

        return result;
    }

    /**
        Query vertex buffers availability.
    */
    void DiscoverVertexBuffersAvailability()
    {
        // bSuppVertexBuffers is already set by DiscoverOpenGL_X_Y_availability()
        if ( bSuppVertexBuffers )
            getConsole().SOLn("Vertex Buffer Objects supported!");
        else
            getConsole().EOLn("Vertex Buffer Objects not supported!");
    } // DiscoverVertexBuffersAvailability()

    /**
        Gets whether Vertex Buffer Object is supported or not.
    */
    TPRREbool isVertexBufferObjectSupported() const
    {
        return bSuppVertexBuffers;
    } // isVertexBufferObjectSupported()

protected:
    PRREhwVideoDiscoverOpenGL_1_5()
    {
        bSuppVertexBuffers = false;
    };

    PRREhwVideoDiscoverOpenGL_1_5(const PRREhwVideoDiscoverOpenGL_1_5&) {};
    PRREhwVideoDiscoverOpenGL_1_5& operator=(const PRREhwVideoDiscoverOpenGL_1_5&) { return *this; };

private:
    TPRREbool bSuppVertexBuffers;         /**< Is Vertex Buffer Object supported? */
    
};