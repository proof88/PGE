/*
    ###################################################################################
    PRREhwVideoDiscoverOpenGLbase.cpp
    This file is part of PRRE.
    PRRE OpenGL capabilities base class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/


#include "PRREbaseIncludes.h"  // PCH
#include "../PRREpragmas.h"
#include "PRREhwVideoDiscoverOpenGLbase.h"
#include "../PRREGLsnippets.h"
#include "../PRREGLextensionFuncs.h"


// ############################### PUBLIC ################################


/**
    Is the given extension supported?
    @return True if the given extension name can be found in either sVidFeaturesOGL or sVidFeaturesWGL, false otherwise.
*/
TPRREbool PRREhwVideoDiscoverOpenGLbase::isExtensionSupported(
    const char* extname,
    const std::string& sVidFeaturesOGL,
    const std::string& sVidFeaturesWGL)
{
    return ( (strstr(sVidFeaturesOGL.c_str(),extname) != NULL) || (strstr(sVidFeaturesWGL.c_str(),extname) != NULL) );
} // isExtensionSupported()

/**
    Checks given value against given standard value.
*/
void PRREhwVideoDiscoverOpenGLbase::CheckAgainstStandardMinValue(GLint value, GLint stdValue)
{
    if ( value < stdValue )
        CConsole::getConsoleInstance().EOLn("%d is less than the standard minimum of %d!", value, stdValue);
} // CheckAgainstStandardMinValue()

/**
    Checks given value against given standard value.
*/
void PRREhwVideoDiscoverOpenGLbase::CheckAgainstStandardMinValuef(GLfloat value, GLfloat stdValue)
{
    if ( value < stdValue )
        CConsole::getConsoleInstance().EOLn("%f is less than the standard minimum of %f!", value, stdValue);
} // CheckAgainstStandardMinValuef()

/**
    Checks given range against given standard range.
*/
void PRREhwVideoDiscoverOpenGLbase::CheckAgainstStandardRange(GLint val1, GLint val2, GLint stdVal1, GLint stdVal2)
{
    if ( ( val1 > stdVal1 ) || ( val2 < stdVal2 ) )
        CConsole::getConsoleInstance().EOLn("Range problem: [%d, %d] vs standard [%d, %d]!",
                           val1, val2, stdVal1, stdVal2);
} // CheckAgainstStandardRange()

PRREhwVideoDiscoverOpenGLbase::PRREhwVideoDiscoverOpenGLbase(
    const std::string& sFtrsOGL,
    const std::string& sFtrsWGL,
    const std::string& sVerOGL,
    const TPRREuint& nVidVerGLSLmaj,
    const TPRREuint& nVidVerGLSLmin) :
    sVidFeaturesOGL(sFtrsOGL),
    sVidFeaturesWGL(sFtrsWGL),
    sVidVersionOGL(sVerOGL),
    nVidVersionGLSLmaj(nVidVerGLSLmaj),
    nVidVersionGLSLmin(nVidVerGLSLmin)
{
}

PRREhwVideoDiscoverOpenGLbase::~PRREhwVideoDiscoverOpenGLbase()
{
}


// ############################## PROTECTED ##############################


/**
    Is the given extension supported?
    @return True if the given extension is supported (exported by the driver), false otherwise.
*/
TPRREbool PRREhwVideoDiscoverOpenGLbase::isExtensionSupported(const char* extname) const
{
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
bool PRREhwVideoDiscoverOpenGLbase::printExtensionSupported(const char* title, const char* name, std::vector<void*>& functions)
{
    bool result;

    getConsole().O(title);
    if ( result = isExtensionSupported(name) )
    {
        for (std::size_t i = 0; (i < functions.size()) && result; i++)
            result &= (functions[i] != PGENULL);

        if ( result )
            getConsole().SOLn("OK!");
        else
            getConsole().EOLn("NOK (func)!");
    }
    else
        getConsole().EOLn("NOK (ext)!");

    functions.clear();
    return result;
}


// ############################### PRIVATE ###############################


PRREhwVideoDiscoverOpenGLbase::PRREhwVideoDiscoverOpenGLbase() :
    sVidFeaturesOGL(sVidFeaturesOGL),
    sVidFeaturesWGL(sVidFeaturesWGL),
    sVidVersionOGL(sVidVersionOGL),
    nVidVersionGLSLmaj(nVidVersionGLSLmaj),
    nVidVersionGLSLmin(nVidVersionGLSLmin)
{
}

PRREhwVideoDiscoverOpenGLbase::PRREhwVideoDiscoverOpenGLbase(const PRREhwVideoDiscoverOpenGLbase&) :
    sVidFeaturesOGL(sVidFeaturesOGL),
    sVidFeaturesWGL(sVidFeaturesWGL),
    sVidVersionOGL(sVidVersionOGL),
    nVidVersionGLSLmaj(nVidVersionGLSLmaj),
    nVidVersionGLSLmin(nVidVersionGLSLmin)
{
}

PRREhwVideoDiscoverOpenGLbase& PRREhwVideoDiscoverOpenGLbase::operator=(const PRREhwVideoDiscoverOpenGLbase&)
{
    return *this;
}
