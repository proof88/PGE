#pragma once

/*
    ###################################################################################
    PRREhwVideoDiscoverOpenGLbase.h
    This file is part of PRRE.
    Internal header.
    PRRE OpenGL capabilities base class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../../../../../../CConsole/CConsole/src/CConsole.h"

#include "../gl/gl.h"
#include "../gl/glu.h"
#include "../gl/glext.h"
#include "../gl/wglext.h"

#include "../../external/PRREallHeaders.h"
#include <vector>

/**
    PRRE OpenGL capabilities base class.
*/
class PRREhwVideoDiscoverOpenGLbase
{
#ifdef PRRE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PRREhwVideoDiscoverOpenGLbase is included")
#endif

public:
    static TPRREbool isExtensionSupported(
        const char* extname,
        const std::string& sVidFeaturesOGL,
        const std::string& sVidFeaturesWGL);   /**< Is the given extension supported? */

    static void CheckAgainstStandardMinValue(
        GLint value, GLint stdValue );         /**< Checks given value against given standard value. */
    static void CheckAgainstStandardMinValuef(
        GLfloat value, GLfloat stdValue );     /**< Checks given value against given standard value. */
    static void CheckAgainstStandardRange(
        GLint val1, GLint val2,
        GLint stdVal1, GLint stdVal2 );        /**< Checks given range against given standard range. */

    PRREhwVideoDiscoverOpenGLbase(
        const std::string& sFtrsOGL,
        const std::string& sFtrsWGL,
        const std::string& sVerOGL,
        const TPRREuint& nVidVerGLSLmaj,
        const TPRREuint& nVidVerGLSLmin);

    virtual ~PRREhwVideoDiscoverOpenGLbase();

    CConsole&  getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */
    const char* getLoggerModuleName() const;          /**< Returns the logger module name of this class. */

    virtual std::string getVersion() const = 0;

    virtual bool discoverOpenGLavailability() = 0;

protected:
    const TPRREuint& nVidVersionGLSLmaj;  /**< GLSL major version. */
    const TPRREuint& nVidVersionGLSLmin;  /**< GLSL minor version. */

    GLint   tmpRet[6];                    /**< Hold for glGetInteger...() results. */
    GLfloat tmpRetf[2];                   /**< Hold for glGetFloat...() results. */

    PRREhwVideoDiscoverOpenGLbase();

    PRREhwVideoDiscoverOpenGLbase(const PRREhwVideoDiscoverOpenGLbase&);
    PRREhwVideoDiscoverOpenGLbase& operator=(const PRREhwVideoDiscoverOpenGLbase&);

    TPRREbool isExtensionSupported(const char* extname) const;   /**< Is the given extension supported? */

    bool printExtensionSupported(const char* title, const char* name, std::vector<void*>& functions);

private:
    const std::string& sVidFeaturesOGL;   /**< VGA features based on OGL. */
    const std::string& sVidFeaturesWGL;   /**< VGA features based on WGL. */
    const std::string& sVidVersionOGL;    /**< OpenGL version. */
    
};