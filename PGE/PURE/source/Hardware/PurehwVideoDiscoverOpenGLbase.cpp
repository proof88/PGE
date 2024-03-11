/*
    ###################################################################################
    PureHwVideoDiscoverOpenGLbase.cpp
    This file is part of PURE.
    Pure OpenGL capabilities base class.
    Made by PR00F88
    ###################################################################################
*/


#include "PureBaseIncludes.h"  // PCH
#include "../../include/internal/Hardware/PureHwVideoDiscoverOpenGLbase.h"
#include "../../include/internal/PurePragmas.h"
#include "../../include/internal/PureGLsnippets.h"
#include "../../include/internal/PureGLextensionFuncs.h"


// ############################### PUBLIC ################################


/**
    Is the given extension supported?
    @return True if the given extension name can be found in either sVidFeaturesOGL or sVidFeaturesWGL, false otherwise.
*/
TPureBool PureHwVideoDiscoverOpenGLbase::isExtensionSupported(
    const char* extname,
    const std::string& sVidFeaturesOGL,
    const std::string& sVidFeaturesWGL)
{
    return ( (strstr(sVidFeaturesOGL.c_str(),extname) != NULL) || (strstr(sVidFeaturesWGL.c_str(),extname) != NULL) );
} // isExtensionSupported()

/**
    Checks given value against given standard value.
*/
void PureHwVideoDiscoverOpenGLbase::CheckAgainstStandardMinValue(GLint value, GLint stdValue)
{
    if ( value < stdValue )
        CConsole::getConsoleInstance().EOLn("%d is less than the standard minimum of %d!", value, stdValue);
} // CheckAgainstStandardMinValue()

/**
    Checks given value against given standard value.
*/
void PureHwVideoDiscoverOpenGLbase::CheckAgainstStandardMinValuef(GLfloat value, GLfloat stdValue)
{
    if ( value < stdValue )
        CConsole::getConsoleInstance().EOLn("%f is less than the standard minimum of %f!", value, stdValue);
} // CheckAgainstStandardMinValuef()

/**
    Checks given range against given standard range.
*/
void PureHwVideoDiscoverOpenGLbase::CheckAgainstStandardRange(GLint val1, GLint val2, GLint stdVal1, GLint stdVal2)
{
    if ( ( val1 > stdVal1 ) || ( val2 < stdVal2 ) )
        CConsole::getConsoleInstance().EOLn("Range problem: [%d, %d] vs standard [%d, %d]!",
                           val1, val2, stdVal1, stdVal2);
} // CheckAgainstStandardRange()

PureHwVideoDiscoverOpenGLbase::PureHwVideoDiscoverOpenGLbase(
    const std::string& sFtrsOGL,
    const std::string& sFtrsWGL,
    const std::string& sVerOGL,
    const TPureUInt& nVidVerGLSLmaj,
    const TPureUInt& nVidVerGLSLmin) :
    sVidFeaturesOGL(sFtrsOGL),
    sVidFeaturesWGL(sFtrsWGL),
    sVidVersionOGL(sVerOGL),
    nVidVersionGLSLmaj(nVidVerGLSLmaj),
    nVidVersionGLSLmin(nVidVerGLSLmin)
{
}

PureHwVideoDiscoverOpenGLbase::~PureHwVideoDiscoverOpenGLbase()
{
}

/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& PureHwVideoDiscoverOpenGLbase::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* PureHwVideoDiscoverOpenGLbase::getLoggerModuleName()
{
    return "PureHwVideoDiscoverOpenGLbase";
} // getLoggerModuleName()


// ############################## PROTECTED ##############################


/**
    Is the given extension supported?
    @return True if the given extension is supported (exported by the driver), false otherwise.
*/
TPureBool PureHwVideoDiscoverOpenGLbase::isExtensionSupported(const char* extname) const
{
    /*
        There are some online OpenGL capability reports can be used to find out how widely an extension is supported:
        https://www.gpuinfo.org/
         - https://opengl.gpuinfo.org/listextensions.php
         - https://opengles.gpuinfo.org/
        https://feedback.wildfiregames.com/report/opengl/

        Some other good DBs for getting HW info:
        https://www.techpowerup.com/gpu-specs/
    */

    return isExtensionSupported(extname, sVidFeaturesOGL, sVidFeaturesWGL);
} // isExtensionSupported()

/**
    Checks if given extension is supported and prints the result.
    Not only extension name string but extension-specific function pointers can be also checked.

    @param title     A human-readable short form title of the extension/feature.
    @param name      The extension name we are checking for.
    @param functions Extension-specific function pointers to be checked. This vector becomes empty after the call.

    @return True if given extension name string is exported by driver and optionally specified function pointers
            are also valid, otherwise false.
*/
bool PureHwVideoDiscoverOpenGLbase::printExtensionSupported(const char* title, const char* name, std::vector<void*>& functions)
{
    bool result;

    if ( result = isExtensionSupported(name) )
    {
        for (std::size_t i = 0; (i < functions.size()) && result; i++)
            result &= (functions[i] != PGENULL);

        if ( result )
            getConsole().SOLn("%sOK!", title);
        else
            getConsole().EOLn("%sNOK (func)!", title);
    }
    else
    {
        getConsole().EOLn("%sNOK (ext)!", title);
    }

    functions.clear();
    return result;
}


// ############################### PRIVATE ###############################


PureHwVideoDiscoverOpenGLbase::PureHwVideoDiscoverOpenGLbase() :
    sVidFeaturesOGL(sVidFeaturesOGL),
    sVidFeaturesWGL(sVidFeaturesWGL),
    sVidVersionOGL(sVidVersionOGL),
    nVidVersionGLSLmaj(nVidVersionGLSLmaj),
    nVidVersionGLSLmin(nVidVersionGLSLmin)
{
}

PureHwVideoDiscoverOpenGLbase::PureHwVideoDiscoverOpenGLbase(const PureHwVideoDiscoverOpenGLbase&) :
    sVidFeaturesOGL(sVidFeaturesOGL),
    sVidFeaturesWGL(sVidFeaturesWGL),
    sVidVersionOGL(sVidVersionOGL),
    nVidVersionGLSLmaj(nVidVersionGLSLmaj),
    nVidVersionGLSLmin(nVidVersionGLSLmin)
{
}

PureHwVideoDiscoverOpenGLbase& PureHwVideoDiscoverOpenGLbase::operator=(const PureHwVideoDiscoverOpenGLbase&)
{
    return *this;
}
