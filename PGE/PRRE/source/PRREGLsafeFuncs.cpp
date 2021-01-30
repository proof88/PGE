/*
    ###################################################################################
    PRREGLsafeFuncs.cpp
    This file is part of PRRE.
    Safe wrapper functions for OpenGL functions which return true or false based on the last GL error.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREbaseIncludes.h"  // PCH
#include "../include/internal/PRREGLsafeFuncs.h"
#include "../include/internal/PRREGLsnippets.h"
#include "../include/internal/PRREpragmas.h"

TPRREbool pglHint (GLenum target, GLenum mode)
{
    PRREGLsnippets::ClearGLerror();
    glHint(target, mode);
    if ( PRREGLsnippets::isGLerrorPresent() )
    {
        CConsole::getConsoleInstance().EOLnOO("ERROR: %s(%d, %d) failed: %s", __FUNCTION__, target, mode, PRREGLsnippets::getGLerrorTextFromEnum( PRREGLsnippets::getLastSavedGLerror() ));
        return false;
    }
    return true;
}

TPRREbool pglGenTextures (GLsizei n, GLuint *textures)
{
    PRREGLsnippets::ClearGLerror();
    glGenTextures(n, textures);
    if ( PRREGLsnippets::isGLerrorPresent() )
    {
        CConsole::getConsoleInstance().EOLn("ERROR: %s(%d) failed: %s", __FUNCTION__, n, PRREGLsnippets::getGLerrorTextFromEnum( PRREGLsnippets::getLastSavedGLerror() ));
        return false;
    }
    return true;
}

TPRREbool pglBindTexture (GLenum target, GLuint texture)
{
    PRREGLsnippets::ClearGLerror();
    glBindTexture(target, texture);
    if ( PRREGLsnippets::isGLerrorPresent() )
    {
        CConsole::getConsoleInstance().EOLn("ERROR: %s(%d, %d) failed: %s", __FUNCTION__, target, texture, PRREGLsnippets::getGLerrorTextFromEnum( PRREGLsnippets::getLastSavedGLerror() ));
        return false;
    }
    return true;
}

TPRREbool pglTexEnvf (GLenum target, GLenum pname, GLfloat param)
{
    PRREGLsnippets::ClearGLerror();
    glTexEnvf(target, pname, param);
    if ( PRREGLsnippets::isGLerrorPresent() )
    {
        CConsole::getConsoleInstance().EOLn("ERROR: %s(%d, %d, %f) failed: %s", __FUNCTION__, target, pname, param, PRREGLsnippets::getGLerrorTextFromEnum( PRREGLsnippets::getLastSavedGLerror() ));
        return false;
    }
    return true;
}

TPRREbool pglTexEnvfv (GLenum target, GLenum pname, const GLfloat *params)
{
    if ( !params )
    {
        CConsole::getConsoleInstance().EOLn("ERROR: %s(%d, %d) params is NULLPTR!", __FUNCTION__, target, pname);
        return false;
    }
    PRREGLsnippets::ClearGLerror();
    glTexEnvfv(target, pname, params);
    if ( PRREGLsnippets::isGLerrorPresent() )
    {
        CConsole::getConsoleInstance().EOLn("ERROR: %s(%d, %d) failed: %s", __FUNCTION__, target, pname, PRREGLsnippets::getGLerrorTextFromEnum( PRREGLsnippets::getLastSavedGLerror() ));
        return false;
    }
    return true;
}

TPRREbool pglTexEnvi (GLenum target, GLenum pname, GLint param)
{
    PRREGLsnippets::ClearGLerror();
    glTexEnvi(target, pname, param);
    if ( PRREGLsnippets::isGLerrorPresent() )
    {
        CConsole::getConsoleInstance().EOLn("ERROR: %s(%d, %d, %d) failed: %s", __FUNCTION__, target, pname, param, PRREGLsnippets::getGLerrorTextFromEnum( PRREGLsnippets::getLastSavedGLerror() ));
        return false;
    }
    return true;
}

TPRREbool pglTexEnviv (GLenum target, GLenum pname, const GLint *params)
{
    if ( !params )
    {
        CConsole::getConsoleInstance().EOLn("ERROR: %s(%d, %d) params is NULLPTR!", __FUNCTION__, target, pname);
        return false;
    }
    PRREGLsnippets::ClearGLerror();
    glTexEnviv(target, pname, params);
    if ( PRREGLsnippets::isGLerrorPresent() )
    {
        CConsole::getConsoleInstance().EOLn("ERROR: %s(%d, %d) failed: %s", __FUNCTION__, target, pname, PRREGLsnippets::getGLerrorTextFromEnum( PRREGLsnippets::getLastSavedGLerror() ));
        return false;
    }
    return true;
}

TPRREbool pglTexGenf (GLenum coord, GLenum pname, GLfloat param)
{
    PRREGLsnippets::ClearGLerror();
    glTexGenf(coord, pname, param);
    if ( PRREGLsnippets::isGLerrorPresent() )
    {
        CConsole::getConsoleInstance().EOLn("ERROR: %s(%d, %d, %f) failed: %s", __FUNCTION__, coord, pname, param, PRREGLsnippets::getGLerrorTextFromEnum( PRREGLsnippets::getLastSavedGLerror() ));
        return false;
    }
    return true;
}

TPRREbool pglTexGenfv (GLenum coord, GLenum pname, const GLfloat *params)
{
    if ( !params )
    {
        CConsole::getConsoleInstance().EOLn("ERROR: %s(%d, %d) params is NULLPTR!", __FUNCTION__, coord, pname);
        return false;
    }
    PRREGLsnippets::ClearGLerror();
    glTexGenfv(coord, pname, params);
    if ( PRREGLsnippets::isGLerrorPresent() )
    {
        CConsole::getConsoleInstance().EOLn("ERROR: %s(%d, %d) failed: %s", __FUNCTION__, coord, pname, PRREGLsnippets::getGLerrorTextFromEnum( PRREGLsnippets::getLastSavedGLerror() ));
        return false;
    }
    return true;
}

TPRREbool pglTexGeni (GLenum coord, GLenum pname, GLint param)
{
    PRREGLsnippets::ClearGLerror();
    glTexGeni(coord, pname, param);
    if ( PRREGLsnippets::isGLerrorPresent() )
    {
        CConsole::getConsoleInstance().EOLn("ERROR: %s(%d, %d, %d) failed: %s", __FUNCTION__, coord, pname, param, PRREGLsnippets::getGLerrorTextFromEnum( PRREGLsnippets::getLastSavedGLerror() ));
        return false;
    }
    return true;
}

TPRREbool pglTexGeniv (GLenum coord, GLenum pname, const GLint *params)
{
    if ( !params )
    {
        CConsole::getConsoleInstance().EOLn("ERROR: %s(%d, %d) params is NULLPTR!", __FUNCTION__, coord, pname);
        return false;
    }
    PRREGLsnippets::ClearGLerror();
    glTexGeniv(coord, pname, params);
    if ( PRREGLsnippets::isGLerrorPresent() )
    {
        CConsole::getConsoleInstance().EOLn("ERROR: %s(%d, %d) failed: %s", __FUNCTION__, coord, pname, PRREGLsnippets::getGLerrorTextFromEnum( PRREGLsnippets::getLastSavedGLerror() ));
        return false;
    }
    return true;
}

TPRREbool pglTexParameterf (GLenum target, GLenum pname, GLfloat param)
{
    PRREGLsnippets::ClearGLerror();
    glTexParameterf(target, pname, param);
    if ( PRREGLsnippets::isGLerrorPresent() )
    {
        CConsole::getConsoleInstance().EOLn("ERROR: %s(%d, %d, %f) failed: %s", __FUNCTION__, target, pname, param, PRREGLsnippets::getGLerrorTextFromEnum( PRREGLsnippets::getLastSavedGLerror() ));
        return false;
    }
    return true;
}

TPRREbool pglTexParameteri (GLenum target, GLenum pname, GLint param)
{
    PRREGLsnippets::ClearGLerror();
    glTexParameteri(target, pname, param);
    if ( PRREGLsnippets::isGLerrorPresent() )
    {
        CConsole::getConsoleInstance().EOLn("ERROR: %s(%d, %d, %d) failed: %s", __FUNCTION__, target, pname, param, PRREGLsnippets::getGLerrorTextFromEnum( PRREGLsnippets::getLastSavedGLerror() ));
        return false;
    }
    return true;
}

TPRREbool pglTexParameterfv (GLenum target, GLenum pname, const GLfloat *params)
{
    if ( !params )
    {
        CConsole::getConsoleInstance().EOLn("ERROR: %s(%d, %d) params is NULLPTR!", __FUNCTION__, target, pname);
        return false;
    }
    PRREGLsnippets::ClearGLerror();
    glTexParameterfv(target, pname, params);
    if ( PRREGLsnippets::isGLerrorPresent() )
    {
        CConsole::getConsoleInstance().EOLn("ERROR: %s(%d, %d) failed: %s", __FUNCTION__, target, pname, PRREGLsnippets::getGLerrorTextFromEnum( PRREGLsnippets::getLastSavedGLerror() ));
        return false;
    }
    return true;
}

TPRREbool pglTexParameteriv (GLenum target, GLenum pname, const GLint *params)
{
    if ( !params )
    {
        CConsole::getConsoleInstance().EOLn("ERROR: %s(%d, %d) params is NULLPTR!", __FUNCTION__, target, pname);
        return false;
    }
    PRREGLsnippets::ClearGLerror();
    glTexParameteriv(target, pname, params);
    if ( PRREGLsnippets::isGLerrorPresent() )
    {
        CConsole::getConsoleInstance().EOLn("ERROR: %s(%d, %d) failed: %s", __FUNCTION__, target, pname, PRREGLsnippets::getGLerrorTextFromEnum( PRREGLsnippets::getLastSavedGLerror() ));
        return false;
    }
    return true;
}

TPRREbool pglGetTexParameterfv (GLenum target, GLenum pname, GLfloat *params)
{
    if ( !params )
    {
        CConsole::getConsoleInstance().EOLn("ERROR: %s(%d, %d) params is NULLPTR!", __FUNCTION__, target, pname);
        return false;
    }
    PRREGLsnippets::ClearGLerror();
    glGetTexParameterfv(target, pname, params);
    if ( PRREGLsnippets::isGLerrorPresent() )
    {
        CConsole::getConsoleInstance().EOLn("ERROR: %s(%d, %d) failed: %s", __FUNCTION__, target, pname, PRREGLsnippets::getGLerrorTextFromEnum( PRREGLsnippets::getLastSavedGLerror() ));
        return false;
    }
    return true;
}

TPRREbool pglGetTexParameteriv (GLenum target, GLenum pname, GLint *params)
{
    if ( !params )
    {
        CConsole::getConsoleInstance().EOLn("ERROR: %s(%d, %d) params is NULLPTR!", __FUNCTION__, target, pname);
        return false;
    }
    PRREGLsnippets::ClearGLerror();
    glGetTexParameteriv(target, pname, params);
    if ( PRREGLsnippets::isGLerrorPresent() )
    {
        CConsole::getConsoleInstance().EOLn("ERROR: %s(%d, %d) failed: %s", __FUNCTION__, target, pname, PRREGLsnippets::getGLerrorTextFromEnum( PRREGLsnippets::getLastSavedGLerror() ));
        return false;
    }
    return true;
}

TPRREbool pglTexImage2D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels)
{
    // pixels is allowed to be NULL
    PRREGLsnippets::ClearGLerror();
    glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
    if ( PRREGLsnippets::isGLerrorPresent() )
    {
        CConsole::getConsoleInstance().EOLn("ERROR: %s(%d, %d, %d, %d, %d, %d, %d, %d, %s) failed: %s", __FUNCTION__, target, level, internalformat, width, height, border, format, type, pixels,
            (pixels ? "non-nullptr" : "nullptr"),
            PRREGLsnippets::getGLerrorTextFromEnum( PRREGLsnippets::getLastSavedGLerror() ));
        return false;
    }
    return true;
}