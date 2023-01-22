#pragma once

/*
    ###################################################################################
    PureGLsafeFuncs.h
    This file is part of PURE.
    Internal header.
    Safe wrapper functions for OpenGL functions which return true or false based on the last GL error.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../external/PureAllHeaders.h"

TPurebool pglHint (GLenum target, GLenum mode);
TPurebool pglFeedbackBuffer (GLsizei size, GLenum type, GLfloat *buffer);

GLuint    pglGenLists (GLsizei range);
TPurebool pglNewList (GLuint list, GLenum mode);
TPurebool pglEndList (void );
TPurebool pglDeleteLists (GLuint list, GLsizei range);

TPurebool pglGenBuffersARB (GLsizei n, GLuint *buffers);
TPurebool pglBindBufferARB (GLenum target, GLuint buffer);
TPurebool pglBufferDataARB (GLenum target, GLsizeiptrARB size, const GLvoid *data, GLenum usage);
TPurebool pglDeleteBuffersARB (GLsizei n, const GLuint *buffers);

TPurebool pglGenTextures (GLsizei n, GLuint *textures);
TPurebool pglBindTexture (GLenum target, GLuint texture);
TPurebool pglTexEnvf (GLenum target, GLenum pname, GLfloat param);
TPurebool pglTexEnvfv (GLenum target, GLenum pname, const GLfloat *params);
TPurebool pglTexEnvi (GLenum target, GLenum pname, GLint param);
TPurebool pglTexEnviv (GLenum target, GLenum pname, const GLint *params);
TPurebool pglTexGenf (GLenum coord, GLenum pname, GLfloat param);
TPurebool pglTexGenfv (GLenum coord, GLenum pname, const GLfloat *params);
TPurebool pglTexGeni (GLenum coord, GLenum pname, GLint param);
TPurebool pglTexGeniv (GLenum coord, GLenum pname, const GLint *params);
TPurebool pglTexParameterf (GLenum target, GLenum pname, GLfloat param);
TPurebool pglTexParameteri (GLenum target, GLenum pname, GLint param);
TPurebool pglTexParameterfv (GLenum target, GLenum pname, const GLfloat *params);
TPurebool pglTexParameteriv (GLenum target, GLenum pname, const GLint *params);
TPurebool pglGetTexParameterfv (GLenum target, GLenum pname, GLfloat *params);
TPurebool pglGetTexParameteriv (GLenum target, GLenum pname, GLint *params);
TPurebool pglTexImage2D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
TPurebool pglGenQueries (GLsizei n, GLuint *ids);