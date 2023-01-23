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

TPureBool pglHint (GLenum target, GLenum mode);
TPureBool pglFeedbackBuffer (GLsizei size, GLenum type, GLfloat *buffer);

GLuint    pglGenLists (GLsizei range);
TPureBool pglNewList (GLuint list, GLenum mode);
TPureBool pglEndList (void );
TPureBool pglDeleteLists (GLuint list, GLsizei range);

TPureBool pglGenBuffersARB (GLsizei n, GLuint *buffers);
TPureBool pglBindBufferARB (GLenum target, GLuint buffer);
TPureBool pglBufferDataARB (GLenum target, GLsizeiptrARB size, const GLvoid *data, GLenum usage);
TPureBool pglDeleteBuffersARB (GLsizei n, const GLuint *buffers);

TPureBool pglGenTextures (GLsizei n, GLuint *textures);
TPureBool pglBindTexture (GLenum target, GLuint texture);
TPureBool pglTexEnvf (GLenum target, GLenum pname, GLfloat param);
TPureBool pglTexEnvfv (GLenum target, GLenum pname, const GLfloat *params);
TPureBool pglTexEnvi (GLenum target, GLenum pname, GLint param);
TPureBool pglTexEnviv (GLenum target, GLenum pname, const GLint *params);
TPureBool pglTexGenf (GLenum coord, GLenum pname, GLfloat param);
TPureBool pglTexGenfv (GLenum coord, GLenum pname, const GLfloat *params);
TPureBool pglTexGeni (GLenum coord, GLenum pname, GLint param);
TPureBool pglTexGeniv (GLenum coord, GLenum pname, const GLint *params);
TPureBool pglTexParameterf (GLenum target, GLenum pname, GLfloat param);
TPureBool pglTexParameteri (GLenum target, GLenum pname, GLint param);
TPureBool pglTexParameterfv (GLenum target, GLenum pname, const GLfloat *params);
TPureBool pglTexParameteriv (GLenum target, GLenum pname, const GLint *params);
TPureBool pglGetTexParameterfv (GLenum target, GLenum pname, GLfloat *params);
TPureBool pglGetTexParameteriv (GLenum target, GLenum pname, GLint *params);
TPureBool pglTexImage2D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
TPureBool pglGenQueries (GLsizei n, GLuint *ids);