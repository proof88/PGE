/*
    ###################################################################################
    PureGLsafeFuncs.cpp
    This file is part of Pure.
    Safe wrapper functions for OpenGL functions which return true or false based on the last GL error.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PurebaseIncludes.h"  // PCH
#include "../include/internal/PureGLsafeFuncs.h"
#include "../include/internal/PureGLextensionFuncs.h"
#include "../include/internal/PureGLsnippets.h"
#include "../include/internal/Purepragmas.h"

// Note that since GL 4.3 there is callback mechanism that can be set up with glDebugMessageCallback().
// We don't use it that though, mostly because our implementation is not at GL 4.3 level, and it would
// need debug context, and I'm not sure how much performance penalty it makes.

// The following functions below could be replaced by a single functions with varying parameter count.
// We would pass a GL function pointer and its parameters, and the function would call the GL function
// with the given varying number of parameters. However, logging is custom in each function, so at least
// that couldn't be generalized. User could still provide error string format specifier string to the
// function from outside but then it would be also difficult to pass with parameters should the logger
// take is input during parsing the format string.
// So I stick to redundant copy-paste code here. Only a few GL functions get wrappers like this anyway.

TPurebool pglHint (GLenum target, GLenum mode)
{
    PureGLsnippets::ClearGLError();
    glHint(target, mode);
    if ( PureGLsnippets::isGLErrorPresent() )
    {
        CConsole::getConsoleInstance().EOLnOO("ERROR: %s(%d, %d) failed: %s", __FUNCTION__, target, mode, PureGLsnippets::getGLErrorTextFromEnum( PureGLsnippets::getLastSavedGLError() ));
        return false;
    }
    return true;
}

TPurebool pglFeedbackBuffer (GLsizei size, GLenum type, GLfloat *buffer)
{
    PureGLsnippets::ClearGLError();
    glFeedbackBuffer(size, type, buffer);
    if ( PureGLsnippets::isGLErrorPresent() )
    {
        CConsole::getConsoleInstance().EOLnOO("ERROR: %s(%d, %d, buffer %s) failed: %s", __FUNCTION__, size, type, (buffer ? "non-nullptr" : "nullptr"),
            PureGLsnippets::getGLErrorTextFromEnum( PureGLsnippets::getLastSavedGLError() ));
        return false;
    }
    return true;
}

GLuint pglGenLists (GLsizei range)
{
    PureGLsnippets::ClearGLError();
    const GLuint result = glGenLists(range);
    if ( PureGLsnippets::isGLErrorPresent() )
    {
        CConsole::getConsoleInstance().EOLnOO("ERROR: %s(%d) failed: %s", __FUNCTION__, range, PureGLsnippets::getGLErrorTextFromEnum( PureGLsnippets::getLastSavedGLError() ));
    }
    return result;
}

TPurebool pglNewList (GLuint list, GLenum mode)
{
    PureGLsnippets::ClearGLError();
    glNewList(list, mode);
    if ( PureGLsnippets::isGLErrorPresent() )
    {
        CConsole::getConsoleInstance().EOLnOO("ERROR: %s(%d, %d) failed: %s", __FUNCTION__, list, mode,
            PureGLsnippets::getGLErrorTextFromEnum( PureGLsnippets::getLastSavedGLError() ));
        return false;
    }
    return true;
}

TPurebool pglEndList (void)
{
    PureGLsnippets::ClearGLError();
    glEndList();
    if ( PureGLsnippets::isGLErrorPresent() )
    {
        CConsole::getConsoleInstance().EOLnOO("ERROR: %s() failed: %s", __FUNCTION__, PureGLsnippets::getGLErrorTextFromEnum( PureGLsnippets::getLastSavedGLError() ));
        return false;
    }
    return true;
}

TPurebool pglDeleteLists (GLuint list, GLsizei range)
{
    PureGLsnippets::ClearGLError();
    glDeleteLists(list, range);
    if ( PureGLsnippets::isGLErrorPresent() )
    {
        CConsole::getConsoleInstance().EOLnOO("ERROR: %s(%d, %d) failed: %s", __FUNCTION__, list, range,
            PureGLsnippets::getGLErrorTextFromEnum( PureGLsnippets::getLastSavedGLError() ));
        return false;
    }
    return true;
}


TPurebool pglGenBuffersARB (GLsizei n, GLuint *buffers)
{
    PureGLsnippets::ClearGLError();
    glGenBuffersARB(n, buffers);
    if ( PureGLsnippets::isGLErrorPresent() )
    {
        CConsole::getConsoleInstance().EOLnOO("ERROR: %s(%d, buffers %s) failed: %s", __FUNCTION__, n, (buffers ? "non-nullptr" : "nullptr"),
            PureGLsnippets::getGLErrorTextFromEnum( PureGLsnippets::getLastSavedGLError() ));
        return false;
    }
    return true;
}

TPurebool pglBindBufferARB (GLenum target, GLuint buffer)
{
    PureGLsnippets::ClearGLError();
    glBindBufferARB(target, buffer);
    if ( PureGLsnippets::isGLErrorPresent() )
    {
        CConsole::getConsoleInstance().EOLnOO("ERROR: %s(%d, %d) failed: %s", __FUNCTION__, target, buffer,
            PureGLsnippets::getGLErrorTextFromEnum( PureGLsnippets::getLastSavedGLError() ));
        return false;
    }
    return true;
}

TPurebool pglBufferDataARB (GLenum target, GLsizeiptrARB size, const GLvoid *data, GLenum usage)
{
    PureGLsnippets::ClearGLError();
    glBufferDataARB(target, size, data, usage);
    if ( PureGLsnippets::isGLErrorPresent() )
    {
        CConsole::getConsoleInstance().EOLnOO("ERROR: %s(%d, %d, data %s, usage) failed: %s", __FUNCTION__, target, size, (data ? "non-nullptr" : "nullptr"), usage,
            PureGLsnippets::getGLErrorTextFromEnum( PureGLsnippets::getLastSavedGLError() ));
        return false;
    }
    return true;
}

TPurebool pglDeleteBuffersARB (GLsizei n, const GLuint *buffers)
{
    PureGLsnippets::ClearGLError();
    glDeleteBuffersARB(n, buffers);
    if ( PureGLsnippets::isGLErrorPresent() )
    {
        CConsole::getConsoleInstance().EOLnOO("ERROR: %s(%d, buffers %s) failed: %s", __FUNCTION__, n, (buffers ? "non-nullptr" : "nullptr"),
            PureGLsnippets::getGLErrorTextFromEnum( PureGLsnippets::getLastSavedGLError() ));
        return false;
    }
    return true;
}

TPurebool pglGenTextures (GLsizei n, GLuint *textures)
{
    PureGLsnippets::ClearGLError();
    glGenTextures(n, textures);
    if ( PureGLsnippets::isGLErrorPresent() )
    {
        CConsole::getConsoleInstance().EOLn("ERROR: %s(%d) failed: %s", __FUNCTION__, n, PureGLsnippets::getGLErrorTextFromEnum( PureGLsnippets::getLastSavedGLError() ));
        return false;
    }
    return true;
}

TPurebool pglBindTexture (GLenum target, GLuint texture)
{
    PureGLsnippets::ClearGLError();
    glBindTexture(target, texture);
    if ( PureGLsnippets::isGLErrorPresent() )
    {
        CConsole::getConsoleInstance().EOLn("ERROR: %s(%d, %d) failed: %s", __FUNCTION__, target, texture, PureGLsnippets::getGLErrorTextFromEnum( PureGLsnippets::getLastSavedGLError() ));
        return false;
    }
    return true;
}

TPurebool pglTexEnvf (GLenum target, GLenum pname, GLfloat param)
{
    PureGLsnippets::ClearGLError();
    glTexEnvf(target, pname, param);
    if ( PureGLsnippets::isGLErrorPresent() )
    {
        CConsole::getConsoleInstance().EOLn("ERROR: %s(%d, %d, %f) failed: %s", __FUNCTION__, target, pname, param, PureGLsnippets::getGLErrorTextFromEnum( PureGLsnippets::getLastSavedGLError() ));
        return false;
    }
    return true;
}

TPurebool pglTexEnvfv (GLenum target, GLenum pname, const GLfloat *params)
{
    if ( !params )
    {
        CConsole::getConsoleInstance().EOLn("ERROR: %s(%d, %d) params is NULLPTR!", __FUNCTION__, target, pname);
        return false;
    }
    PureGLsnippets::ClearGLError();
    glTexEnvfv(target, pname, params);
    if ( PureGLsnippets::isGLErrorPresent() )
    {
        CConsole::getConsoleInstance().EOLn("ERROR: %s(%d, %d) failed: %s", __FUNCTION__, target, pname, PureGLsnippets::getGLErrorTextFromEnum( PureGLsnippets::getLastSavedGLError() ));
        return false;
    }
    return true;
}

TPurebool pglTexEnvi (GLenum target, GLenum pname, GLint param)
{
    PureGLsnippets::ClearGLError();
    glTexEnvi(target, pname, param);
    if ( PureGLsnippets::isGLErrorPresent() )
    {
        CConsole::getConsoleInstance().EOLn("ERROR: %s(%d, %d, %d) failed: %s", __FUNCTION__, target, pname, param, PureGLsnippets::getGLErrorTextFromEnum( PureGLsnippets::getLastSavedGLError() ));
        return false;
    }
    return true;
}

TPurebool pglTexEnviv (GLenum target, GLenum pname, const GLint *params)
{
    if ( !params )
    {
        CConsole::getConsoleInstance().EOLn("ERROR: %s(%d, %d) params is NULLPTR!", __FUNCTION__, target, pname);
        return false;
    }
    PureGLsnippets::ClearGLError();
    glTexEnviv(target, pname, params);
    if ( PureGLsnippets::isGLErrorPresent() )
    {
        CConsole::getConsoleInstance().EOLn("ERROR: %s(%d, %d) failed: %s", __FUNCTION__, target, pname, PureGLsnippets::getGLErrorTextFromEnum( PureGLsnippets::getLastSavedGLError() ));
        return false;
    }
    return true;
}

TPurebool pglTexGenf (GLenum coord, GLenum pname, GLfloat param)
{
    PureGLsnippets::ClearGLError();
    glTexGenf(coord, pname, param);
    if ( PureGLsnippets::isGLErrorPresent() )
    {
        CConsole::getConsoleInstance().EOLn("ERROR: %s(%d, %d, %f) failed: %s", __FUNCTION__, coord, pname, param, PureGLsnippets::getGLErrorTextFromEnum( PureGLsnippets::getLastSavedGLError() ));
        return false;
    }
    return true;
}

TPurebool pglTexGenfv (GLenum coord, GLenum pname, const GLfloat *params)
{
    if ( !params )
    {
        CConsole::getConsoleInstance().EOLn("ERROR: %s(%d, %d) params is NULLPTR!", __FUNCTION__, coord, pname);
        return false;
    }
    PureGLsnippets::ClearGLError();
    glTexGenfv(coord, pname, params);
    if ( PureGLsnippets::isGLErrorPresent() )
    {
        CConsole::getConsoleInstance().EOLn("ERROR: %s(%d, %d) failed: %s", __FUNCTION__, coord, pname, PureGLsnippets::getGLErrorTextFromEnum( PureGLsnippets::getLastSavedGLError() ));
        return false;
    }
    return true;
}

TPurebool pglTexGeni (GLenum coord, GLenum pname, GLint param)
{
    PureGLsnippets::ClearGLError();
    glTexGeni(coord, pname, param);
    if ( PureGLsnippets::isGLErrorPresent() )
    {
        CConsole::getConsoleInstance().EOLn("ERROR: %s(%d, %d, %d) failed: %s", __FUNCTION__, coord, pname, param, PureGLsnippets::getGLErrorTextFromEnum( PureGLsnippets::getLastSavedGLError() ));
        return false;
    }
    return true;
}

TPurebool pglTexGeniv (GLenum coord, GLenum pname, const GLint *params)
{
    if ( !params )
    {
        CConsole::getConsoleInstance().EOLn("ERROR: %s(%d, %d) params is NULLPTR!", __FUNCTION__, coord, pname);
        return false;
    }
    PureGLsnippets::ClearGLError();
    glTexGeniv(coord, pname, params);
    if ( PureGLsnippets::isGLErrorPresent() )
    {
        CConsole::getConsoleInstance().EOLn("ERROR: %s(%d, %d) failed: %s", __FUNCTION__, coord, pname, PureGLsnippets::getGLErrorTextFromEnum( PureGLsnippets::getLastSavedGLError() ));
        return false;
    }
    return true;
}

TPurebool pglTexParameterf (GLenum target, GLenum pname, GLfloat param)
{
    PureGLsnippets::ClearGLError();
    glTexParameterf(target, pname, param);
    if ( PureGLsnippets::isGLErrorPresent() )
    {
        CConsole::getConsoleInstance().EOLn("ERROR: %s(%d, %d, %f) failed: %s", __FUNCTION__, target, pname, param, PureGLsnippets::getGLErrorTextFromEnum( PureGLsnippets::getLastSavedGLError() ));
        return false;
    }
    return true;
}

TPurebool pglTexParameteri (GLenum target, GLenum pname, GLint param)
{
    PureGLsnippets::ClearGLError();
    glTexParameteri(target, pname, param);
    if ( PureGLsnippets::isGLErrorPresent() )
    {
        CConsole::getConsoleInstance().EOLn("ERROR: %s(%d, %d, %d) failed: %s", __FUNCTION__, target, pname, param, PureGLsnippets::getGLErrorTextFromEnum( PureGLsnippets::getLastSavedGLError() ));
        return false;
    }
    return true;
}

TPurebool pglTexParameterfv (GLenum target, GLenum pname, const GLfloat *params)
{
    if ( !params )
    {
        CConsole::getConsoleInstance().EOLn("ERROR: %s(%d, %d) params is NULLPTR!", __FUNCTION__, target, pname);
        return false;
    }
    PureGLsnippets::ClearGLError();
    glTexParameterfv(target, pname, params);
    if ( PureGLsnippets::isGLErrorPresent() )
    {
        CConsole::getConsoleInstance().EOLn("ERROR: %s(%d, %d) failed: %s", __FUNCTION__, target, pname, PureGLsnippets::getGLErrorTextFromEnum( PureGLsnippets::getLastSavedGLError() ));
        return false;
    }
    return true;
}

TPurebool pglTexParameteriv (GLenum target, GLenum pname, const GLint *params)
{
    if ( !params )
    {
        CConsole::getConsoleInstance().EOLn("ERROR: %s(%d, %d) params is NULLPTR!", __FUNCTION__, target, pname);
        return false;
    }
    PureGLsnippets::ClearGLError();
    glTexParameteriv(target, pname, params);
    if ( PureGLsnippets::isGLErrorPresent() )
    {
        CConsole::getConsoleInstance().EOLn("ERROR: %s(%d, %d) failed: %s", __FUNCTION__, target, pname, PureGLsnippets::getGLErrorTextFromEnum( PureGLsnippets::getLastSavedGLError() ));
        return false;
    }
    return true;
}

TPurebool pglGetTexParameterfv (GLenum target, GLenum pname, GLfloat *params)
{
    if ( !params )
    {
        CConsole::getConsoleInstance().EOLn("ERROR: %s(%d, %d) params is NULLPTR!", __FUNCTION__, target, pname);
        return false;
    }
    PureGLsnippets::ClearGLError();
    glGetTexParameterfv(target, pname, params);
    if ( PureGLsnippets::isGLErrorPresent() )
    {
        CConsole::getConsoleInstance().EOLn("ERROR: %s(%d, %d) failed: %s", __FUNCTION__, target, pname, PureGLsnippets::getGLErrorTextFromEnum( PureGLsnippets::getLastSavedGLError() ));
        return false;
    }
    return true;
}

TPurebool pglGetTexParameteriv (GLenum target, GLenum pname, GLint *params)
{
    if ( !params )
    {
        CConsole::getConsoleInstance().EOLn("ERROR: %s(%d, %d) params is NULLPTR!", __FUNCTION__, target, pname);
        return false;
    }
    PureGLsnippets::ClearGLError();
    glGetTexParameteriv(target, pname, params);
    if ( PureGLsnippets::isGLErrorPresent() )
    {
        CConsole::getConsoleInstance().EOLn("ERROR: %s(%d, %d) failed: %s", __FUNCTION__, target, pname, PureGLsnippets::getGLErrorTextFromEnum( PureGLsnippets::getLastSavedGLError() ));
        return false;
    }
    return true;
}

TPurebool pglTexImage2D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels)
{
    // pixels is allowed to be NULL
    PureGLsnippets::ClearGLError();
    glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
    if ( PureGLsnippets::isGLErrorPresent() )
    {
        CConsole::getConsoleInstance().EOLn("ERROR: %s(%d, %d, %d, %d, %d, %d, %d, %d, pixels %s) failed: %s",
            __FUNCTION__, target, level, internalformat, width, height, border, format, type, (pixels ? "non-nullptr" : "nullptr"),
            PureGLsnippets::getGLErrorTextFromEnum( PureGLsnippets::getLastSavedGLError() ));
        return false;
    }
    return true;
}

TPurebool pglGenQueries (GLsizei n, GLuint *ids)
{
    PureGLsnippets::ClearGLError();
    glGenQueriesARB(n, ids);
    if ( PureGLsnippets::isGLErrorPresent() )
    {
        CConsole::getConsoleInstance().EOLn("ERROR: %s(%d) failed: %s", __FUNCTION__, n, PureGLsnippets::getGLErrorTextFromEnum( PureGLsnippets::getLastSavedGLError() ));
        return false;
    }
    return true;
}