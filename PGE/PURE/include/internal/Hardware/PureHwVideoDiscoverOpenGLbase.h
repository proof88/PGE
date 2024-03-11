#pragma once

/*
    ###################################################################################
    PureHwVideoDiscoverOpenGLbase.h
    This file is part of PURE.
    Internal header.
    Pure OpenGL capabilities base class.
    Made by PR00F88
    ###################################################################################
*/

#include <string>
#include <vector>

#include "../gl/gl.h"
#include "../gl/glu.h"
#include "../gl/glext.h"
#include "../gl/wglext.h"

#include "../../../../../../Console/CConsole/src/CConsole.h"

#include "../../external/PureAllHeaders.h"
#include "../../external/PureTypes.h"

/**
    Pure OpenGL capabilities base class.
*/
class PureHwVideoDiscoverOpenGLbase
{
#ifdef PURE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureHwVideoDiscoverOpenGLbase is included")
#endif

public:
    static TPureBool isExtensionSupported(
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

    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------
    
    PureHwVideoDiscoverOpenGLbase(
        const std::string& sFtrsOGL,
        const std::string& sFtrsWGL,
        const std::string& sVerOGL,
        const TPureUInt& nVidVerGLSLmaj,
        const TPureUInt& nVidVerGLSLmin);

    virtual ~PureHwVideoDiscoverOpenGLbase();

    CConsole&  getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */

    virtual std::string getVersion() const = 0;

    virtual bool discoverOpenGLavailability() = 0;

protected:
    const TPureUInt& nVidVersionGLSLmaj;  /**< GLSL major version. */
    const TPureUInt& nVidVersionGLSLmin;  /**< GLSL minor version. */

    GLint   tmpRet[6];                    /**< Hold for glGetInteger...() results. */
    GLfloat tmpRetf[2];                   /**< Hold for glGetFloat...() results. */

    PureHwVideoDiscoverOpenGLbase();

    PureHwVideoDiscoverOpenGLbase(const PureHwVideoDiscoverOpenGLbase&);
    PureHwVideoDiscoverOpenGLbase& operator=(const PureHwVideoDiscoverOpenGLbase&);

    TPureBool isExtensionSupported(const char* extname) const;   /**< Is the given extension supported? */

    bool printExtensionSupported(const char* title, const char* name, std::vector<void*>& functions);

private:
    const std::string& sVidFeaturesOGL;   /**< VGA features based on OGL. */
    const std::string& sVidFeaturesWGL;   /**< VGA features based on WGL. */
    const std::string& sVidVersionOGL;    /**< OpenGL version. */
    
};