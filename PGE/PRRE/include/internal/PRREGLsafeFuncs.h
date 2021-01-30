#pragma once

/*
    ###################################################################################
    PRREGLsafeFuncs.h
    This file is part of PRRE.
    Internal header.
    Safe wrapper functions for OpenGL functions which return true or false based on the last GL error.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../external/PRREallHeaders.h"

TPRREbool pglHint (GLenum target, GLenum mode);
TPRREbool pglGenTextures (GLsizei n, GLuint *textures);
TPRREbool pglBindTexture (GLenum target, GLuint texture);
TPRREbool pglTexEnvf (GLenum target, GLenum pname, GLfloat param);
TPRREbool pglTexEnvfv (GLenum target, GLenum pname, const GLfloat *params);
TPRREbool pglTexEnvi (GLenum target, GLenum pname, GLint param);
TPRREbool pglTexEnviv (GLenum target, GLenum pname, const GLint *params);
TPRREbool pglTexGenf (GLenum coord, GLenum pname, GLfloat param);
TPRREbool pglTexGenfv (GLenum coord, GLenum pname, const GLfloat *params);
TPRREbool pglTexGeni (GLenum coord, GLenum pname, GLint param);
TPRREbool pglTexGeniv (GLenum coord, GLenum pname, const GLint *params);
TPRREbool pglTexParameterf (GLenum target, GLenum pname, GLfloat param);
TPRREbool pglTexParameteri (GLenum target, GLenum pname, GLint param);
TPRREbool pglTexParameterfv (GLenum target, GLenum pname, const GLfloat *params);
TPRREbool pglTexParameteriv (GLenum target, GLenum pname, const GLint *params);
TPRREbool pglGetTexParameterfv (GLenum target, GLenum pname, GLfloat *params);
TPRREbool pglGetTexParameteriv (GLenum target, GLenum pname, GLint *params);
TPRREbool pglTexImage2D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);