#pragma once

/*
    ###################################################################################
    include/internal/PureGLextensionFuncs.h
    This file is part of PURE.
    Internal header.
    Pure useful GL extension functions and non-1.1 function pointers.
    Made by PR00F88
    ###################################################################################
*/

#include "../external/PureAllHeaders.h"
#include "gl/glext.h"
#include "gl/wglext.h"

// Note that although I could define GL_GLEXT_PROTOTYPES before including glext.h to have the function prototypes defined,
// that would mean static linking would be expected to gl libs exporting all those extension and/or newer GL core functions,
// but general existence of such gl lib is likely impossible. So, I still continue declaring the function pointers here.

// non-1.1 WGL function pointers
// ---------------------------------------------------------------------------

extern PFNWGLGETEXTENSIONSSTRINGARBPROC    wglGetExtensionsStringARB;

// pixel format
extern PFNWGLGETPIXELFORMATATTRIBIVARBPROC wglGetPixelFormatAttribivARB;
extern PFNWGLGETPIXELFORMATATTRIBFVARBPROC wglGetPixelFormatAttribfvARB;
extern PFNWGLCHOOSEPIXELFORMATARBPROC      wglChoosePixelFormatARB;

// VSync
extern PFNWGLGETSWAPINTERVALEXTPROC        wglGetSwapIntervalEXT;
extern PFNWGLSWAPINTERVALEXTPROC           wglSwapIntervalEXT;


// non-1.1 GL function pointers
// ---------------------------------------------------------------------------

// multitexturing
extern PFNGLACTIVETEXTUREARBPROC       glActiveTextureARB;
extern PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB;
extern PFNGLMULTITEXCOORD1DARBPROC     glMultiTexCoord1dARB;
extern PFNGLMULTITEXCOORD1DVARBPROC    glMultiTexCoord1dvARB;
extern PFNGLMULTITEXCOORD1FARBPROC     glMultiTexCoord1fARB;
extern PFNGLMULTITEXCOORD1FVARBPROC    glMultiTexCoord1fvARB;
extern PFNGLMULTITEXCOORD1IARBPROC     glMultiTexCoord1iARB;
extern PFNGLMULTITEXCOORD1IVARBPROC    glMultiTexCoord1ivARB;
extern PFNGLMULTITEXCOORD1SARBPROC     glMultiTexCoord1sARB;
extern PFNGLMULTITEXCOORD1SVARBPROC    glMultiTexCoord1svARB;
extern PFNGLMULTITEXCOORD2DARBPROC     glMultiTexCoord2dARB;
extern PFNGLMULTITEXCOORD2DVARBPROC    glMultiTexCoord2dvARB;
extern PFNGLMULTITEXCOORD2FARBPROC     glMultiTexCoord2fARB;
extern PFNGLMULTITEXCOORD2FVARBPROC    glMultiTexCoord2fvARB;
extern PFNGLMULTITEXCOORD2IARBPROC     glMultiTexCoord2iARB;
extern PFNGLMULTITEXCOORD2IVARBPROC    glMultiTexCoord2ivARB;
extern PFNGLMULTITEXCOORD2SARBPROC     glMultiTexCoord2sARB;
extern PFNGLMULTITEXCOORD2SVARBPROC    glMultiTexCoord2svARB;
extern PFNGLMULTITEXCOORD3DARBPROC     glMultiTexCoord3dARB;
extern PFNGLMULTITEXCOORD3DVARBPROC    glMultiTexCoord3dvARB;
extern PFNGLMULTITEXCOORD3FARBPROC     glMultiTexCoord3fARB;
extern PFNGLMULTITEXCOORD3FVARBPROC    glMultiTexCoord3fvARB;
extern PFNGLMULTITEXCOORD3IARBPROC     glMultiTexCoord3iARB;
extern PFNGLMULTITEXCOORD3IVARBPROC    glMultiTexCoord3ivARB;
extern PFNGLMULTITEXCOORD3SARBPROC     glMultiTexCoord3sARB;
extern PFNGLMULTITEXCOORD3SVARBPROC    glMultiTexCoord3svARB;
extern PFNGLMULTITEXCOORD4DARBPROC     glMultiTexCoord4dARB;
extern PFNGLMULTITEXCOORD4DVARBPROC    glMultiTexCoord4dvARB;
extern PFNGLMULTITEXCOORD4FARBPROC     glMultiTexCoord4fARB;
extern PFNGLMULTITEXCOORD4FVARBPROC    glMultiTexCoord4fvARB;
extern PFNGLMULTITEXCOORD4IARBPROC     glMultiTexCoord4iARB;
extern PFNGLMULTITEXCOORD4IVARBPROC    glMultiTexCoord4ivARB;
extern PFNGLMULTITEXCOORD4SARBPROC     glMultiTexCoord4sARB;
extern PFNGLMULTITEXCOORD4SVARBPROC    glMultiTexCoord4svARB;

// texture compression
extern PFNGLCOMPRESSEDTEXIMAGE1DPROC    glCompressedTexImage1DARB;
extern PFNGLCOMPRESSEDTEXIMAGE2DPROC    glCompressedTexImage2DARB;
extern PFNGLCOMPRESSEDTEXIMAGE3DPROC    glCompressedTexImage3DARB;
extern PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC glCompressedTexSubImage1DARB;
extern PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC glCompressedTexSubImage2DARB;
extern PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC glCompressedTexSubImage3DARB;
extern PFNGLGETCOMPRESSEDTEXIMAGEPROC   glGetCompressedTexImageARB;

// 3D textures
extern PFNGLTEXIMAGE3DEXTPROC    glTexImage3DEXT;
extern PFNGLTEXSUBIMAGE3DEXTPROC glTexSubImage3DEXT;

// multisampling
extern PFNGLSAMPLECOVERAGEARBPROC glSampleCoverageARB;

// multi draw arrays
extern PFNGLMULTIDRAWARRAYSEXTPROC   glMultiDrawArraysEXT;
extern PFNGLMULTIDRAWELEMENTSEXTPROC glMultiDrawElementsEXT;

// ATI vertex array object
extern PFNGLNEWOBJECTBUFFERATIPROC         glNewObjectBufferATI;        
extern PFNGLISOBJECTBUFFERATIPROC          glIsObjectBufferATI;         
extern PFNGLUPDATEOBJECTBUFFERATIPROC      glUpdateObjectBufferATI;     
extern PFNGLGETOBJECTBUFFERFVATIPROC       glGetObjectBufferfvATI;      
extern PFNGLGETOBJECTBUFFERIVATIPROC       glGetObjectBufferivATI;      
extern PFNGLFREEOBJECTBUFFERATIPROC        glFreeObjectBufferATI;       
extern PFNGLARRAYOBJECTATIPROC             glArrayObjectATI;            
extern PFNGLGETARRAYOBJECTFVATIPROC        glGetArrayObjectfvATI;       
extern PFNGLGETARRAYOBJECTIVATIPROC        glGetArrayObjectivATI;       
extern PFNGLVARIANTARRAYOBJECTATIPROC      glVariantArrayObjectATI;    
extern PFNGLGETVARIANTARRAYOBJECTFVATIPROC glGetVariantArrayObjectfvATI;
extern PFNGLGETVARIANTARRAYOBJECTIVATIPROC glGetVariantArrayObjectivATI;

// ATI map object buffer
extern PFNGLMAPOBJECTBUFFERATIPROC   glMapObjectBufferATI;
extern PFNGLUNMAPOBJECTBUFFERATIPROC glUnmapObjectBufferATI;

// ATI element array
extern PFNGLELEMENTPOINTERATIPROC        glElementPointerATI;
extern PFNGLDRAWELEMENTARRAYATIPROC      glDrawElementArrayATI;
extern PFNGLDRAWRANGEELEMENTARRAYATIPROC glDrawRangeElementArrayATI;

// ATI TruForm
extern PFNGLPNTRIANGLESIATIPROC glPNTrianglesiATI;
extern PFNGLPNTRIANGLESFATIPROC glPNTrianglesfATI;

// NV vertex array range
extern PFNGLFLUSHVERTEXARRAYRANGENVPROC glFlushVertexArrayRangeNV;
extern PFNGLVERTEXARRAYRANGENVPROC      glVertexArrayRangeNV;

// point parameters
extern PFNGLPOINTPARAMETERFARBPROC  glPointParameterfARB;
extern PFNGLPOINTPARAMETERFVARBPROC glPointParameterfvARB;

// secondary color
extern PFNGLSECONDARYCOLOR3BEXTPROC      glSecondaryColor3bEXT;
extern PFNGLSECONDARYCOLOR3BVEXTPROC     glSecondaryColor3bvEXT;
extern PFNGLSECONDARYCOLOR3DEXTPROC      glSecondaryColor3dEXT;
extern PFNGLSECONDARYCOLOR3DVEXTPROC     glSecondaryColor3dvEXT;
extern PFNGLSECONDARYCOLOR3FEXTPROC      glSecondaryColor3fEXT;
extern PFNGLSECONDARYCOLOR3FVEXTPROC     glSecondaryColor3fvEXT;
extern PFNGLSECONDARYCOLOR3IEXTPROC      glSecondaryColor3iEXT;
extern PFNGLSECONDARYCOLOR3IVEXTPROC     glSecondaryColor3ivEXT;
extern PFNGLSECONDARYCOLOR3SEXTPROC      glSecondaryColor3sEXT;
extern PFNGLSECONDARYCOLOR3SVEXTPROC     glSecondaryColor3svEXT;
extern PFNGLSECONDARYCOLOR3UBEXTPROC     glSecondaryColor3ubEXT;
extern PFNGLSECONDARYCOLOR3UBVEXTPROC    glSecondaryColor3ubvEXT;
extern PFNGLSECONDARYCOLOR3UIEXTPROC     glSecondaryColor3uiEXT;
extern PFNGLSECONDARYCOLOR3UIVEXTPROC    glSecondaryColor3uivEXT;
extern PFNGLSECONDARYCOLOR3USEXTPROC     glSecondaryColor3usEXT;
extern PFNGLSECONDARYCOLOR3USVEXTPROC    glSecondaryColor3usvEXT;
extern PFNGLSECONDARYCOLORPOINTEREXTPROC glSecondaryColorPointerEXT;

// blend func separate
extern PFNGLBLENDFUNCSEPARATEEXTPROC glBlendFuncSeparateEXT;

// window pos
extern PFNGLWINDOWPOS2DARBPROC  glWindowPos2dARB; 
extern PFNGLWINDOWPOS2DVARBPROC glWindowPos2dvARB;
extern PFNGLWINDOWPOS2FARBPROC  glWindowPos2fARB; 
extern PFNGLWINDOWPOS2FVARBPROC glWindowPos2fvARB;
extern PFNGLWINDOWPOS2IARBPROC  glWindowPos2iARB; 
extern PFNGLWINDOWPOS2IVARBPROC glWindowPos2ivARB;
extern PFNGLWINDOWPOS2SARBPROC  glWindowPos2sARB; 
extern PFNGLWINDOWPOS2SVARBPROC glWindowPos2svARB;
extern PFNGLWINDOWPOS3DARBPROC  glWindowPos3dARB; 
extern PFNGLWINDOWPOS3DVARBPROC glWindowPos3dvARB;
extern PFNGLWINDOWPOS3FARBPROC  glWindowPos3fARB; 
extern PFNGLWINDOWPOS3FVARBPROC glWindowPos3fvARB;
extern PFNGLWINDOWPOS3IARBPROC  glWindowPos3iARB; 
extern PFNGLWINDOWPOS3IVARBPROC glWindowPos3ivARB;
extern PFNGLWINDOWPOS3SARBPROC  glWindowPos3sARB; 
extern PFNGLWINDOWPOS3SVARBPROC glWindowPos3svARB;

// occlusion query
extern PFNGLGENQUERIESARBPROC        glGenQueriesARB;
extern PFNGLDELETEQUERIESARBPROC     glDeleteQueriesARB;
extern PFNGLISQUERYARBPROC           glIsQueryARB;
extern PFNGLBEGINQUERYARBPROC        glBeginQueryARB;
extern PFNGLENDQUERYARBPROC          glEndQueryARB;
extern PFNGLGETQUERYIVARBPROC        glGetQueryivARB;
extern PFNGLGETQUERYOBJECTIVARBPROC  glGetQueryObjectivARB;
extern PFNGLGETQUERYOBJECTUIVARBPROC glGetQueryObjectuivARB;

// compiled vertex array
extern PFNGLLOCKARRAYSEXTPROC   glLockArraysEXT;
extern PFNGLUNLOCKARRAYSEXTPROC glUnlockArraysEXT;

// drawRangeElements
extern PFNGLDRAWRANGEELEMENTSEXTPROC glDrawRangeElementsEXT;

// vertex buffer object
extern PFNGLGENBUFFERSARBPROC           glGenBuffersARB;
extern PFNGLBINDBUFFERARBPROC           glBindBufferARB;
extern PFNGLBUFFERDATAARBPROC           glBufferDataARB;
extern PFNGLBUFFERSUBDATAARBPROC        glBufferSubDataARB;
extern PFNGLGETBUFFERSUBDATAARBPROC     glGetBufferSubDataARB;
extern PFNGLDELETEBUFFERSARBPROC        glDeleteBuffersARB;
extern PFNGLISBUFFERARBPROC             glIsBufferARB;
extern PFNGLMAPBUFFERARBPROC            glMapBufferARB;
extern PFNGLUNMAPBUFFERARBPROC          glUnmapBufferARB;
extern PFNGLGETBUFFERPARAMETERIVARBPROC glGetBufferParameterivARB;
extern PFNGLGETBUFFERPOINTERVARBPROC    glGetBufferPointervARB;

// shader objects
extern PFNGLDELETEOBJECTARBPROC         glDeleteObjectARB;
extern PFNGLGETHANDLEARBPROC            glGetHandleARB;
extern PFNGLDETACHOBJECTARBPROC         glDetachObjectARB;
extern PFNGLCREATESHADEROBJECTARBPROC   glCreateShaderObjectARB;
extern PFNGLSHADERSOURCEARBPROC         glShaderSourceARB;
extern PFNGLCOMPILESHADERARBPROC        glCompileShaderARB;
extern PFNGLCREATEPROGRAMOBJECTARBPROC  glCreateProgramObjectARB;
extern PFNGLATTACHOBJECTARBPROC         glAttachObjectARB;
extern PFNGLLINKPROGRAMARBPROC          glLinkProgramARB;
extern PFNGLUSEPROGRAMOBJECTARBPROC     glUseProgramObjectARB;
extern PFNGLVALIDATEPROGRAMARBPROC      glValidateProgramARB;
extern PFNGLUNIFORM1FARBPROC            glUniform1fARB;
extern PFNGLUNIFORM2FARBPROC            glUniform2fARB;
extern PFNGLUNIFORM3FARBPROC            glUniform3fARB;
extern PFNGLUNIFORM4FARBPROC            glUniform4fARB;
extern PFNGLUNIFORM1IARBPROC            glUniform1iARB;
extern PFNGLUNIFORM2IARBPROC            glUniform2iARB;
extern PFNGLUNIFORM3IARBPROC            glUniform3iARB;
extern PFNGLUNIFORM4IARBPROC            glUniform4iARB;
extern PFNGLUNIFORM1FVARBPROC           glUniform1fvARB;
extern PFNGLUNIFORM2FVARBPROC           glUniform2fvARB;
extern PFNGLUNIFORM3FVARBPROC           glUniform3fvARB;
extern PFNGLUNIFORM4FVARBPROC           glUniform4fvARB;
extern PFNGLUNIFORM1IVARBPROC           glUniform1ivARB;
extern PFNGLUNIFORM2IVARBPROC           glUniform2ivARB;
extern PFNGLUNIFORM3IVARBPROC           glUniform3ivARB;
extern PFNGLUNIFORM4IVARBPROC           glUniform4ivARB;
extern PFNGLUNIFORMMATRIX2FVARBPROC     glUniformMatrix2fvARB;
extern PFNGLUNIFORMMATRIX3FVARBPROC     glUniformMatrix3fvARB;
extern PFNGLUNIFORMMATRIX4FVARBPROC     glUniformMatrix4fvARB;
extern PFNGLGETOBJECTPARAMETERFVARBPROC glGetObjectParameterfvARB;
extern PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB;
extern PFNGLGETINFOLOGARBPROC           glGetInfoLogARB;
extern PFNGLGETATTACHEDOBJECTSARBPROC   glGetAttachedObjectsARB;
extern PFNGLGETUNIFORMLOCATIONARBPROC   glGetUniformLocationARB;
extern PFNGLGETACTIVEUNIFORMARBPROC     glGetActiveUniformARB;
extern PFNGLGETUNIFORMFVARBPROC         glGetUniformfvARB;
extern PFNGLGETUNIFORMIVARBPROC         glGetUniformivARB;
extern PFNGLGETSHADERSOURCEARBPROC      glGetShaderSourceARB;

// vertex shader
extern PFNGLBINDATTRIBLOCATIONARBPROC glBindAttribLocationARB;
extern PFNGLGETACTIVEATTRIBARBPROC    glGetActiveAttribARB;
extern PFNGLGETATTRIBLOCATIONARBPROC  glGetAttribLocationARB;

// draw buffers
extern PFNGLDRAWBUFFERSARBPROC glDrawBuffersARB;

// framebuffer object
extern PFNGLISRENDERBUFFERPROC                        glIsRenderbuffer;
extern PFNGLBINDRENDERBUFFERPROC                      glBindRenderbuffer;
extern PFNGLDELETERENDERBUFFERSPROC                   glDeleteRenderbuffers;
extern PFNGLGENRENDERBUFFERSPROC                      glGenRenderbuffers;
extern PFNGLRENDERBUFFERSTORAGEPROC                   glRenderbufferStorage;
extern PFNGLGETRENDERBUFFERPARAMETERIVPROC            glGetRenderbufferParameteriv;
extern PFNGLISFRAMEBUFFERPROC                         glIsFramebuffer;
extern PFNGLBINDFRAMEBUFFERPROC                       glBindFramebuffer;
extern PFNGLDELETEFRAMEBUFFERSPROC                    glDeleteFramebuffers;
extern PFNGLGENFRAMEBUFFERSPROC                       glGenFramebuffers;
extern PFNGLCHECKFRAMEBUFFERSTATUSPROC                glCheckFramebufferStatus;
extern PFNGLFRAMEBUFFERTEXTURE1DPROC                  glFramebufferTexture1D;
extern PFNGLFRAMEBUFFERTEXTURE2DPROC                  glFramebufferTexture2D;
extern PFNGLFRAMEBUFFERTEXTURE3DPROC                  glFramebufferTexture3D;
extern PFNGLFRAMEBUFFERRENDERBUFFERPROC               glFramebufferRenderbuffer;
extern PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC   glGetFramebufferAttachmentParameteriv;
extern PFNGLGENERATEMIPMAPPROC                        glGenerateMipmap;
extern PFNGLBLITFRAMEBUFFERPROC                       glBlitFramebuffer;
extern PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC        glRenderbufferStorageMultisample;
extern PFNGLFRAMEBUFFERTEXTURELAYERPROC               glFramebufferTextureLayer;

// vertex array object
extern PFNGLBINDVERTEXARRAYPROC     glBindVertexArray;
extern PFNGLDELETEVERTEXARRAYSPROC  glDeleteVertexArrays;
extern PFNGLGENVERTEXARRAYSPROC     glGenVertexArrays;
extern PFNGLISVERTEXARRAYPROC       glIsVertexArray;

// ATI separate stencil
extern PFNGLSTENCILOPSEPARATEATIPROC   glStencilOpSeparateATI;
extern PFNGLSTENCILFUNCSEPARATEATIPROC glStencilFuncSeparateATI;

// Two side stencil
extern PFNGLACTIVESTENCILFACEEXTPROC glActiveStencilFaceEXT;

// Conditional rendering
extern PFNGLBEGINCONDITIONALRENDERNVPROC glBeginConditionalRenderNV;
extern PFNGLENDCONDITIONALRENDERNVPROC   glEndConditionalRenderNV;

// Floating point internal formats
extern PFNGLCLAMPCOLORARBPROC glClampColorARB;

// NV depth buffer float
extern PFNGLDEPTHRANGEDNVPROC  glDepthRangedNV; 
extern PFNGLCLEARDEPTHDNVPROC  glClearDepthdNV; 
extern PFNGLDEPTHBOUNDSDNVPROC glDepthBoundsdNV;

// NV half-floats
extern PFNGLVERTEX2HNVPROC          glVertex2hNV;
extern PFNGLVERTEX2HVNVPROC         glVertex2hvNV;
extern PFNGLVERTEX3HNVPROC          glVertex3hNV;
extern PFNGLVERTEX3HVNVPROC         glVertex3hvNV;
extern PFNGLVERTEX4HNVPROC          glVertex4hNV;
extern PFNGLVERTEX4HVNVPROC         glVertex4hvNV;
extern PFNGLNORMAL3HNVPROC          glNormal3hNV;
extern PFNGLNORMAL3HVNVPROC         glNormal3hvNV;
extern PFNGLCOLOR3HNVPROC           glColor3hNV;
extern PFNGLCOLOR3HVNVPROC          glColor3hvNV;
extern PFNGLCOLOR4HNVPROC           glColor4hNV;
extern PFNGLCOLOR4HVNVPROC          glColor4hvNV;
extern PFNGLTEXCOORD1HNVPROC        glTexCoord1hNV;
extern PFNGLTEXCOORD1HVNVPROC       glTexCoord1hvNV;
extern PFNGLTEXCOORD2HNVPROC        glTexCoord2hNV;
extern PFNGLTEXCOORD2HVNVPROC       glTexCoord2hvNV;
extern PFNGLTEXCOORD3HNVPROC        glTexCoord3hNV;
extern PFNGLTEXCOORD3HVNVPROC       glTexCoord3hvNV;
extern PFNGLTEXCOORD4HNVPROC        glTexCoord4hNV;
extern PFNGLTEXCOORD4HVNVPROC       glTexCoord4hvNV;
extern PFNGLMULTITEXCOORD1HNVPROC   glMultiTexCoord1hNV;
extern PFNGLMULTITEXCOORD1HVNVPROC  glMultiTexCoord1hvNV;
extern PFNGLMULTITEXCOORD2HNVPROC   glMultiTexCoord2hNV;
extern PFNGLMULTITEXCOORD2HVNVPROC  glMultiTexCoord2hvNV;
extern PFNGLMULTITEXCOORD3HNVPROC   glMultiTexCoord3hNV;
extern PFNGLMULTITEXCOORD3HVNVPROC  glMultiTexCoord3hvNV;
extern PFNGLMULTITEXCOORD4HNVPROC   glMultiTexCoord4hNV;
extern PFNGLMULTITEXCOORD4HVNVPROC  glMultiTexCoord4hvNV;
extern PFNGLFOGCOORDHNVPROC         glFogCoordhNV;
extern PFNGLFOGCOORDHVNVPROC        glFogCoordhvNV;
extern PFNGLSECONDARYCOLOR3HNVPROC  glSecondaryColor3hNV;
extern PFNGLSECONDARYCOLOR3HVNVPROC glSecondaryColor3hvNV;
extern PFNGLVERTEXWEIGHTHNVPROC     glVertexWeighthNV;
extern PFNGLVERTEXWEIGHTHVNVPROC    glVertexWeighthvNV;
extern PFNGLVERTEXATTRIB1HNVPROC    glVertexAttrib1hNV;
extern PFNGLVERTEXATTRIB1HVNVPROC   glVertexAttrib1hvNV;
extern PFNGLVERTEXATTRIB2HNVPROC    glVertexAttrib2hNV;
extern PFNGLVERTEXATTRIB2HVNVPROC   glVertexAttrib2hvNV;
extern PFNGLVERTEXATTRIB3HNVPROC    glVertexAttrib3hNV;
extern PFNGLVERTEXATTRIB3HVNVPROC   glVertexAttrib3hvNV;
extern PFNGLVERTEXATTRIB4HNVPROC    glVertexAttrib4hNV;
extern PFNGLVERTEXATTRIB4HVNVPROC   glVertexAttrib4hvNV;
extern PFNGLVERTEXATTRIBS1HVNVPROC  glVertexAttribs1hvNV;
extern PFNGLVERTEXATTRIBS2HVNVPROC  glVertexAttribs2hvNV;
extern PFNGLVERTEXATTRIBS3HVNVPROC  glVertexAttribs3hvNV;
extern PFNGLVERTEXATTRIBS4HVNVPROC  glVertexAttribs4hvNV;

// Integer texture parameters
extern PFNGLTEXPARAMETERIIVEXTPROC      glTexParameterIivEXT;
extern PFNGLTEXPARAMETERIUIVEXTPROC     glTexParameterIuivEXT;
extern PFNGLGETTEXPARAMETERIIVEXTPROC   glGetTexParameterIivEXT; 
extern PFNGLGETTEXPARAMETERIUIVEXTPROC  glGetTexParameterIuivEXT;
extern PFNGLCLEARCOLORIIEXTPROC         glClearColorIiEXT;
extern PFNGLCLEARCOLORIUIEXTPROC        glClearColorIuiEXT;

// Draw buffers 2
extern PFNGLCOLORMASKINDEXEDEXTPROC    glColorMaskIndexedEXT;
extern PFNGLGETBOOLEANINDEXEDVEXTPROC  glGetBooleanIndexedvEXT;
extern PFNGLGETINTEGERINDEXEDVEXTPROC  glGetIntegerIndexedvEXT;
extern PFNGLENABLEINDEXEDEXTPROC       glEnableIndexedEXT;
extern PFNGLDISABLEINDEXEDEXTPROC      glDisableIndexedEXT;
extern PFNGLISENABLEDINDEXEDEXTPROC    glIsEnabledIndexedEXT;

// Transform feedback EXT
extern PFNGLBEGINTRANSFORMFEEDBACKEXTPROC       glBeginTransformFeedbackEXT;
extern PFNGLENDTRANSFORMFEEDBACKEXTPROC         glEndTransformFeedbackEXT;
extern PFNGLBINDBUFFERRANGEEXTPROC              glBindBufferRangeEXT;
extern PFNGLBINDBUFFEROFFSETEXTPROC             glBindBufferOffsetEXT;
extern PFNGLBINDBUFFERBASEEXTPROC               glBindBufferBaseEXT;
extern PFNGLTRANSFORMFEEDBACKVARYINGSEXTPROC    glTransformFeedbackVaryingsEXT;
extern PFNGLGETTRANSFORMFEEDBACKVARYINGEXTPROC  glGetTransformFeedbackVaryingEXT;

// Uniform buffer object
extern PFNGLDRAWARRAYSINSTANCEDPROC    glDrawArraysInstanced;
extern PFNGLDRAWELEMENTSINSTANCEDPROC  glDrawElementsInstanced;
extern PFNGLTEXBUFFERPROC              glTexBuffer;
extern PFNGLPRIMITIVERESTARTINDEXPROC  glPrimitiveRestartIndex;

// Instanced draw
extern PFNGLDRAWARRAYSINSTANCEDARBPROC   glDrawArraysInstancedARB;
extern PFNGLDRAWELEMENTSINSTANCEDARBPROC glDrawElementsInstancedARB;

// Buffer copy
extern PFNGLCOPYBUFFERSUBDATAPROC glCopyBufferSubData;

// Primitive restart
extern PFNGLPRIMITIVERESTARTNVPROC      glPrimitiveRestartNV;
extern PFNGLPRIMITIVERESTARTINDEXNVPROC glPrimitiveRestartIndexNV;

// Texture buffer object
extern PFNGLTEXBUFFERARBPROC glTexBufferARB;

// API robustness
extern PFNGLGETGRAPHICSRESETSTATUSARBPROC  glGetGraphicsResetStatusARB;
extern PFNGLGETNMAPDVARBPROC               glGetnMapdvARB;
extern PFNGLGETNMAPFVARBPROC               glGetnMapfvARB;
extern PFNGLGETNMAPIVARBPROC               glGetnMapivARB;
extern PFNGLGETNPIXELMAPFVARBPROC          glGetnPixelMapfvARB;
extern PFNGLGETNPIXELMAPUIVARBPROC         glGetnPixelMapuivARB;
extern PFNGLGETNPIXELMAPUSVARBPROC         glGetnPixelMapusvARB;
extern PFNGLGETNPOLYGONSTIPPLEARBPROC      glGetnPolygonStippleARB;
extern PFNGLGETNCOLORTABLEARBPROC          glGetnColorTableARB;
extern PFNGLGETNCONVOLUTIONFILTERARBPROC   glGetnConvolutionFilterARB;
extern PFNGLGETNSEPARABLEFILTERARBPROC     glGetnSeparableFilterARB;
extern PFNGLGETNHISTOGRAMARBPROC           glGetnHistogramARB;
extern PFNGLGETNMINMAXARBPROC              glGetnMinmaxARB;
extern PFNGLGETNTEXIMAGEARBPROC            glGetnTexImageARB;
extern PFNGLREADNPIXELSARBPROC             glReadnPixelsARB;
extern PFNGLGETNCOMPRESSEDTEXIMAGEARBPROC  glGetnCompressedTexImageARB;
extern PFNGLGETNUNIFORMFVARBPROC           glGetnUniformfvARB;
extern PFNGLGETNUNIFORMIVARBPROC           glGetnUniformivARB;
extern PFNGLGETNUNIFORMUIVARBPROC          glGetnUniformuivARB;
extern PFNGLGETNUNIFORMDVARBPROC           glGetnUniformdvARB;

// Draw elements base vertex
extern PFNGLDRAWELEMENTSBASEVERTEXPROC           glDrawElementsBaseVertex;
extern PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC      glDrawRangeElementsBaseVertex;
extern PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC  glDrawElementsInstancedBaseVertex;
extern PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC      glMultiDrawElementsBaseVertex;

// Provoking vertex
extern PFNGLPROVOKINGVERTEXPROC glProvokingVertex;

// Texture multisample
extern PFNGLTEXIMAGE2DMULTISAMPLEPROC glTexImage2DMultisample;
extern PFNGLTEXIMAGE3DMULTISAMPLEPROC glTexImage3DMultisample;
extern PFNGLGETMULTISAMPLEFVPROC      glGetMultisamplefv;
extern PFNGLSAMPLEMASKIPROC           glSampleMaski;

// Sync
extern PFNGLFENCESYNCPROC       glFenceSync;
extern PFNGLISSYNCPROC          glIsSync;
extern PFNGLDELETESYNCPROC      glDeleteSync;
extern PFNGLCLIENTWAITSYNCPROC  glClientWaitSync;
extern PFNGLWAITSYNCPROC        glWaitSync;
extern PFNGLGETINTEGER64VPROC   glGetInteger64v;
extern PFNGLGETSYNCIVPROC       glGetSynciv;

// Geometry shader
extern PFNGLPROGRAMPARAMETERIARBPROC       glProgramParameteriARB;
extern PFNGLFRAMEBUFFERTEXTUREARBPROC      glFramebufferTextureARB;
extern PFNGLFRAMEBUFFERTEXTURELAYERARBPROC glFramebufferTextureLayerARB;
extern PFNGLFRAMEBUFFERTEXTUREFACEARBPROC  glFramebufferTextureFaceARB; 

// Extended blend source
extern PFNGLBINDFRAGDATALOCATIONINDEXEDPROC glBindFragDataLocationIndexed;
extern PFNGLGETFRAGDATAINDEXPROC            glGetFragDataIndex;

// Sampler objects
extern PFNGLGENSAMPLERSPROC             glGenSamplers;
extern PFNGLDELETESAMPLERSPROC          glDeleteSamplers;
extern PFNGLISSAMPLERPROC               glIsSampler;
extern PFNGLBINDSAMPLERPROC             glBindSampler;
extern PFNGLSAMPLERPARAMETERIPROC       glSamplerParameteri;
extern PFNGLSAMPLERPARAMETERIVPROC      glSamplerParameteriv;
extern PFNGLSAMPLERPARAMETERFPROC       glSamplerParameterf;
extern PFNGLSAMPLERPARAMETERFVPROC      glSamplerParameterfv;
extern PFNGLSAMPLERPARAMETERIIVPROC     glSamplerParameterIiv;
extern PFNGLSAMPLERPARAMETERIUIVPROC    glSamplerParameterIuiv;
extern PFNGLGETSAMPLERPARAMETERIVPROC   glGetSamplerParameteriv;
extern PFNGLGETSAMPLERPARAMETERIIVPROC  glGetSamplerParameterIiv;
extern PFNGLGETSAMPLERPARAMETERFVPROC   glGetSamplerParameterfv;
extern PFNGLGETSAMPLERPARAMETERIUIVPROC glGetSamplerParameterIuiv;

// Timer query
extern PFNGLQUERYCOUNTERPROC         glQueryCounter;
extern PFNGLGETQUERYOBJECTI64VPROC   glGetQueryObjecti64v; 
extern PFNGLGETQUERYOBJECTUI64VPROC  glGetQueryObjectui64v;

// Instanced arrays
extern PFNGLVERTEXATTRIBDIVISORARBPROC glVertexAttribDivisorARB;

// Vertex type 2.10.10.10 rev
extern PFNGLVERTEXP2UIPROC          glVertexP2ui;
extern PFNGLVERTEXP2UIVPROC         glVertexP2uiv;
extern PFNGLVERTEXP3UIPROC          glVertexP3ui;
extern PFNGLVERTEXP3UIVPROC         glVertexP3uiv;
extern PFNGLVERTEXP4UIPROC          glVertexP4ui;
extern PFNGLVERTEXP4UIVPROC         glVertexP4uiv;
extern PFNGLTEXCOORDP1UIPROC        glTexCoordP1ui;
extern PFNGLTEXCOORDP1UIVPROC       glTexCoordP1uiv;
extern PFNGLTEXCOORDP2UIPROC        glTexCoordP2ui;
extern PFNGLTEXCOORDP2UIVPROC       glTexCoordP2uiv;
extern PFNGLTEXCOORDP3UIPROC        glTexCoordP3ui;
extern PFNGLTEXCOORDP3UIVPROC       glTexCoordP3uiv;
extern PFNGLTEXCOORDP4UIPROC        glTexCoordP4ui;
extern PFNGLTEXCOORDP4UIVPROC       glTexCoordP4uiv;
extern PFNGLMULTITEXCOORDP1UIPROC   glMultiTexCoordP1ui;
extern PFNGLMULTITEXCOORDP1UIVPROC  glMultiTexCoordP1uiv; 
extern PFNGLMULTITEXCOORDP2UIPROC   glMultiTexCoordP2ui;
extern PFNGLMULTITEXCOORDP2UIVPROC  glMultiTexCoordP2uiv;
extern PFNGLMULTITEXCOORDP3UIPROC   glMultiTexCoordP3ui;
extern PFNGLMULTITEXCOORDP3UIVPROC  glMultiTexCoordP3uiv;
extern PFNGLMULTITEXCOORDP4UIPROC   glMultiTexCoordP4ui;
extern PFNGLMULTITEXCOORDP4UIVPROC  glMultiTexCoordP4uiv;
extern PFNGLNORMALP3UIPROC          glNormalP3ui; 
extern PFNGLNORMALP3UIVPROC         glNormalP3uiv;
extern PFNGLCOLORP3UIPROC           glColorP3ui;
extern PFNGLCOLORP3UIVPROC          glColorP3uiv; 
extern PFNGLCOLORP4UIPROC           glColorP4ui;
extern PFNGLCOLORP4UIVPROC          glColorP4uiv; 
extern PFNGLSECONDARYCOLORP3UIPROC  glSecondaryColorP3ui;
extern PFNGLSECONDARYCOLORP3UIVPROC glSecondaryColorP3uiv;
extern PFNGLVERTEXATTRIBP1UIPROC    glVertexAttribP1ui;
extern PFNGLVERTEXATTRIBP1UIVPROC   glVertexAttribP1uiv;
extern PFNGLVERTEXATTRIBP2UIPROC    glVertexAttribP2ui;
extern PFNGLVERTEXATTRIBP2UIVPROC   glVertexAttribP2uiv;
extern PFNGLVERTEXATTRIBP3UIPROC    glVertexAttribP3ui;
extern PFNGLVERTEXATTRIBP3UIVPROC   glVertexAttribP3uiv;
extern PFNGLVERTEXATTRIBP4UIPROC    glVertexAttribP4ui;
extern PFNGLVERTEXATTRIBP4UIVPROC   glVertexAttribP4uiv;

// GPU shader FP64
extern PFNGLUNIFORM1DPROC          glUniform1d;
extern PFNGLUNIFORM2DPROC          glUniform2d;
extern PFNGLUNIFORM3DPROC          glUniform3d;
extern PFNGLUNIFORM4DPROC          glUniform4d;
extern PFNGLUNIFORM1DVPROC         glUniform1dv;
extern PFNGLUNIFORM2DVPROC         glUniform2dv;
extern PFNGLUNIFORM3DVPROC         glUniform3dv;
extern PFNGLUNIFORM4DVPROC         glUniform4dv;
extern PFNGLUNIFORMMATRIX2DVPROC   glUniformMatrix2dv;
extern PFNGLUNIFORMMATRIX3DVPROC   glUniformMatrix3dv;
extern PFNGLUNIFORMMATRIX4DVPROC   glUniformMatrix4dv;
extern PFNGLUNIFORMMATRIX2X3DVPROC glUniformMatrix2x3dv;
extern PFNGLUNIFORMMATRIX2X4DVPROC glUniformMatrix2x4dv;
extern PFNGLUNIFORMMATRIX3X2DVPROC glUniformMatrix3x2dv;
extern PFNGLUNIFORMMATRIX3X4DVPROC glUniformMatrix3x4dv;
extern PFNGLUNIFORMMATRIX4X2DVPROC glUniformMatrix4x2dv;
extern PFNGLUNIFORMMATRIX4X3DVPROC glUniformMatrix4x3dv;
extern PFNGLGETUNIFORMDVPROC       glGetUniformdv;

// Shader subroutine
extern PFNGLGETSUBROUTINEUNIFORMLOCATIONPROC    glGetSubroutineUniformLocation; 
extern PFNGLGETSUBROUTINEINDEXPROC              glGetSubroutineIndex;
extern PFNGLGETACTIVESUBROUTINEUNIFORMIVPROC    glGetActiveSubroutineUniformiv;
extern PFNGLGETACTIVESUBROUTINEUNIFORMNAMEPROC  glGetActiveSubroutineUniformName;
extern PFNGLGETACTIVESUBROUTINENAMEPROC         glGetActiveSubroutineName;
extern PFNGLUNIFORMSUBROUTINESUIVPROC           glUniformSubroutinesuiv;
extern PFNGLGETUNIFORMSUBROUTINEUIVPROC         glGetUniformSubroutineuiv;
extern PFNGLGETPROGRAMSTAGEIVPROC               glGetProgramStageiv;

// Draw indirect
extern PFNGLDRAWARRAYSINDIRECTPROC   glDrawArraysIndirect;
extern PFNGLDRAWELEMENTSINDIRECTPROC glDrawElementsIndirect;

// Minimum sample shading
extern PFNGLMINSAMPLESHADINGARBPROC glMinSampleShadingARB;

// Tessellation
extern PFNGLPATCHPARAMETERIPROC  glPatchParameteri; 
extern PFNGLPATCHPARAMETERFVPROC glPatchParameterfv;

// Transform feedback 2
extern PFNGLBINDTRANSFORMFEEDBACKPROC     glBindTransformFeedback;
extern PFNGLDELETETRANSFORMFEEDBACKSPROC  glDeleteTransformFeedbacks;
extern PFNGLGENTRANSFORMFEEDBACKSPROC     glGenTransformFeedbacks;
extern PFNGLISTRANSFORMFEEDBACKPROC       glIsTransformFeedback;
extern PFNGLPAUSETRANSFORMFEEDBACKPROC    glPauseTransformFeedback;
extern PFNGLRESUMETRANSFORMFEEDBACKPROC   glResumeTransformFeedback; 
extern PFNGLDRAWTRANSFORMFEEDBACKPROC     glDrawTransformFeedback;

// Transform feedback 3
extern PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC glDrawTransformFeedbackStream;
extern PFNGLBEGINQUERYINDEXEDPROC           glBeginQueryIndexed;
extern PFNGLENDQUERYINDEXEDPROC             glEndQueryIndexed;
extern PFNGLGETQUERYINDEXEDIVPROC           glGetQueryIndexediv;

// Draw buffers blend
extern PFNGLBLENDEQUATIONIARBPROC         glBlendEquationiARB;
extern PFNGLBLENDEQUATIONSEPARATEIARBPROC glBlendEquationSeparateiARB;
extern PFNGLBLENDFUNCIARBPROC             glBlendFunciARB;
extern PFNGLBLENDFUNCSEPARATEIARBPROC     glBlendFuncSeparateiARB;

// Program binary
extern PFNGLGETPROGRAMBINARYPROC  glGetProgramBinary;
extern PFNGLPROGRAMBINARYPROC     glProgramBinary;
extern PFNGLPROGRAMPARAMETERIPROC glProgramParameteri;

// Separate shader objects
extern PFNGLUSEPROGRAMSTAGESPROC          glUseProgramStages;
extern PFNGLACTIVESHADERPROGRAMPROC       glActiveShaderProgram;
extern PFNGLCREATESHADERPROGRAMVPROC      glCreateShaderProgramv;
extern PFNGLBINDPROGRAMPIPELINEPROC       glBindProgramPipeline;
extern PFNGLDELETEPROGRAMPIPELINESPROC    glDeleteProgramPipelines;
extern PFNGLGENPROGRAMPIPELINESPROC       glGenProgramPipelines;
extern PFNGLISPROGRAMPIPELINEPROC         glIsProgramPipeline;
extern PFNGLGETPROGRAMPIPELINEIVPROC      glGetProgramPipelineiv;
extern PFNGLPROGRAMUNIFORM1IPROC          glProgramUniform1i;
extern PFNGLPROGRAMUNIFORM1IVPROC         glProgramUniform1iv;
extern PFNGLPROGRAMUNIFORM1FPROC          glProgramUniform1f;
extern PFNGLPROGRAMUNIFORM1FVPROC         glProgramUniform1fv;
extern PFNGLPROGRAMUNIFORM1DPROC          glProgramUniform1d;
extern PFNGLPROGRAMUNIFORM1DVPROC         glProgramUniform1dv;
extern PFNGLPROGRAMUNIFORM1UIPROC         glProgramUniform1ui;
extern PFNGLPROGRAMUNIFORM1UIVPROC        glProgramUniform1uiv;
extern PFNGLPROGRAMUNIFORM2IPROC          glProgramUniform2i;
extern PFNGLPROGRAMUNIFORM2IVPROC         glProgramUniform2iv;
extern PFNGLPROGRAMUNIFORM2FPROC          glProgramUniform2f;
extern PFNGLPROGRAMUNIFORM2FVPROC         glProgramUniform2fv;
extern PFNGLPROGRAMUNIFORM2DPROC          glProgramUniform2d;
extern PFNGLPROGRAMUNIFORM2DVPROC         glProgramUniform2dv;
extern PFNGLPROGRAMUNIFORM2UIPROC         glProgramUniform2ui;
extern PFNGLPROGRAMUNIFORM2UIVPROC        glProgramUniform2uiv;
extern PFNGLPROGRAMUNIFORM3IPROC          glProgramUniform3i;
extern PFNGLPROGRAMUNIFORM3IVPROC         glProgramUniform3iv;
extern PFNGLPROGRAMUNIFORM3FPROC          glProgramUniform3f;
extern PFNGLPROGRAMUNIFORM3FVPROC         glProgramUniform3fv;
extern PFNGLPROGRAMUNIFORM3DPROC          glProgramUniform3d;
extern PFNGLPROGRAMUNIFORM3DVPROC         glProgramUniform3dv;
extern PFNGLPROGRAMUNIFORM3UIPROC         glProgramUniform3ui;
extern PFNGLPROGRAMUNIFORM3UIVPROC        glProgramUniform3uiv;
extern PFNGLPROGRAMUNIFORM4IPROC          glProgramUniform4i;
extern PFNGLPROGRAMUNIFORM4IVPROC         glProgramUniform4iv;
extern PFNGLPROGRAMUNIFORM4FPROC          glProgramUniform4f;
extern PFNGLPROGRAMUNIFORM4FVPROC         glProgramUniform4fv;
extern PFNGLPROGRAMUNIFORM4DPROC          glProgramUniform4d;
extern PFNGLPROGRAMUNIFORM4DVPROC         glProgramUniform4dv;
extern PFNGLPROGRAMUNIFORM4UIPROC         glProgramUniform4ui;
extern PFNGLPROGRAMUNIFORM4UIVPROC        glProgramUniform4uiv;
extern PFNGLPROGRAMUNIFORMMATRIX2FVPROC   glProgramUniformMatrix2fv;
extern PFNGLPROGRAMUNIFORMMATRIX3FVPROC   glProgramUniformMatrix3fv;
extern PFNGLPROGRAMUNIFORMMATRIX4FVPROC   glProgramUniformMatrix4fv;
extern PFNGLPROGRAMUNIFORMMATRIX2DVPROC   glProgramUniformMatrix2dv;
extern PFNGLPROGRAMUNIFORMMATRIX3DVPROC   glProgramUniformMatrix3dv;
extern PFNGLPROGRAMUNIFORMMATRIX4DVPROC   glProgramUniformMatrix4dv;
extern PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC glProgramUniformMatrix2x3fv;
extern PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC glProgramUniformMatrix3x2fv;
extern PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC glProgramUniformMatrix2x4fv;
extern PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC glProgramUniformMatrix4x2fv;
extern PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC glProgramUniformMatrix3x4fv;
extern PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC glProgramUniformMatrix4x3fv;
extern PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC glProgramUniformMatrix2x3dv;
extern PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC glProgramUniformMatrix3x2dv;
extern PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC glProgramUniformMatrix2x4dv;
extern PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC glProgramUniformMatrix4x2dv;
extern PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC glProgramUniformMatrix3x4dv;
extern PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC glProgramUniformMatrix4x3dv;
extern PFNGLVALIDATEPROGRAMPIPELINEPROC   glValidateProgramPipeline;
extern PFNGLGETPROGRAMPIPELINEINFOLOGPROC glGetProgramPipelineInfoLog;

// ES2 compatibility
extern PFNGLRELEASESHADERCOMPILERPROC     glReleaseShaderCompiler;
extern PFNGLSHADERBINARYPROC              glShaderBinary;
extern PFNGLGETSHADERPRECISIONFORMATPROC  glGetShaderPrecisionFormat;
extern PFNGLDEPTHRANGEFPROC               glDepthRangef;
extern PFNGLCLEARDEPTHFPROC               glClearDepthf;

// Viewport array
extern PFNGLVIEWPORTARRAYVPROC    glViewportArrayv;
extern PFNGLVIEWPORTINDEXEDFPROC  glViewportIndexedf; 
extern PFNGLVIEWPORTINDEXEDFVPROC glViewportIndexedfv;
extern PFNGLSCISSORARRAYVPROC     glScissorArrayv;
extern PFNGLSCISSORINDEXEDPROC    glScissorIndexed;
extern PFNGLSCISSORINDEXEDVPROC   glScissorIndexedv;
extern PFNGLDEPTHRANGEARRAYVPROC  glDepthRangeArrayv;
extern PFNGLDEPTHRANGEINDEXEDPROC glDepthRangeIndexed;
extern PFNGLGETFLOATI_VPROC       glGetFloati_v;
extern PFNGLGETDOUBLEI_VPROC      glGetDoublei_v;

// Atomic counters
extern PFNGLGETACTIVEATOMICCOUNTERBUFFERIVPROC glGetActiveAtomicCounterBufferiv;

// Shader image load/store
extern PFNGLBINDIMAGETEXTUREPROC glBindImageTexture;
extern PFNGLMEMORYBARRIERPROC    glMemoryBarrier;

// Texture storage
extern PFNGLTEXSTORAGE1DPROC        glTexStorage1D;
extern PFNGLTEXSTORAGE2DPROC        glTexStorage2D;
extern PFNGLTEXSTORAGE3DPROC        glTexStorage3D;
extern PFNGLTEXTURESTORAGE1DEXTPROC glTextureStorage1DEXT;
extern PFNGLTEXTURESTORAGE2DEXTPROC glTextureStorage2DEXT;
extern PFNGLTEXTURESTORAGE3DEXTPROC glTextureStorage3DEXT;

// Draw transform instanced
extern PFNGLDRAWTRANSFORMFEEDBACKINSTANCEDPROC       glDrawTransformFeedbackInstanced;
extern PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCEDPROC glDrawTransformFeedbackStreamInstanced;

// Instanced rendering from indexed instance
extern PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC             glDrawArraysInstancedBaseInstance;
extern PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC           glDrawElementsInstancedBaseInstance;
extern PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC glDrawElementsInstancedBaseVertexBaseInstance;

// Internal format query
extern PFNGLGETINTERNALFORMATIVPROC glGetInternalformativ;

// KHR debug
extern PFNGLDEBUGMESSAGECONTROLPROC  glDebugMessageControl; 
extern PFNGLDEBUGMESSAGEINSERTPROC   glDebugMessageInsert;
extern PFNGLDEBUGMESSAGECALLBACKPROC glDebugMessageCallback;
extern PFNGLGETDEBUGMESSAGELOGPROC   glGetDebugMessageLog;
extern PFNGLPUSHDEBUGGROUPPROC       glPushDebugGroup;
extern PFNGLPOPDEBUGGROUPPROC        glPopDebugGroup;
extern PFNGLOBJECTLABELPROC          glObjectLabel;
extern PFNGLGETOBJECTLABELPROC       glGetObjectLabel;
extern PFNGLOBJECTPTRLABELPROC       glObjectPtrLabel;
extern PFNGLGETOBJECTPTRLABELPROC    glGetObjectPtrLabel;

// Clear buffer object
extern PFNGLCLEARBUFFERDATAPROC            glClearBufferData;
extern PFNGLCLEARBUFFERSUBDATAPROC         glClearBufferSubData;
extern PFNGLCLEARNAMEDBUFFERDATAEXTPROC    glClearNamedBufferDataEXT;
extern PFNGLCLEARNAMEDBUFFERSUBDATAEXTPROC glClearNamedBufferSubDataEXT;

// Compute shaders
extern PFNGLDISPATCHCOMPUTEPROC          glDispatchCompute;
extern PFNGLDISPATCHCOMPUTEINDIRECTPROC  glDispatchComputeIndirect;

// Image copy between image buffers
extern PFNGLCOPYIMAGESUBDATAPROC glCopyImageSubData;

// Rendering to framebuffer object w/o attachment
extern PFNGLFRAMEBUFFERPARAMETERIPROC             glFramebufferParameteri;
extern PFNGLGETFRAMEBUFFERPARAMETERIVPROC         glGetFramebufferParameteriv;
extern PFNGLNAMEDFRAMEBUFFERPARAMETERIEXTPROC     glNamedFramebufferParameteriEXT;
extern PFNGLGETNAMEDFRAMEBUFFERPARAMETERIVEXTPROC glGetNamedFramebufferParameterivEXT;

// Generalized query for image formats
extern PFNGLGETINTERNALFORMATI64VPROC glGetInternalformati64v;

// Invalidate data
extern PFNGLINVALIDATETEXSUBIMAGEPROC    glInvalidateTexSubImage;
extern PFNGLINVALIDATETEXIMAGEPROC       glInvalidateTexImage;
extern PFNGLINVALIDATEBUFFERSUBDATAPROC  glInvalidateBufferSubData; 
extern PFNGLINVALIDATEBUFFERDATAPROC     glInvalidateBufferData;
extern PFNGLINVALIDATEFRAMEBUFFERPROC    glInvalidateFramebuffer;
extern PFNGLINVALIDATESUBFRAMEBUFFERPROC glInvalidateSubFramebuffer;

// Multi indirect draw calls from a single draw call
extern PFNGLMULTIDRAWARRAYSINDIRECTPROC   glMultiDrawArraysIndirect;
extern PFNGLMULTIDRAWELEMENTSINDIRECTPROC glMultiDrawElementsIndirect;

// Program object interface query
extern PFNGLGETPROGRAMINTERFACEIVPROC            glGetProgramInterfaceiv;
extern PFNGLGETPROGRAMRESOURCEINDEXPROC          glGetProgramResourceIndex;
extern PFNGLGETPROGRAMRESOURCENAMEPROC           glGetProgramResourceName;
extern PFNGLGETPROGRAMRESOURCEIVPROC             glGetProgramResourceiv;
extern PFNGLGETPROGRAMRESOURCELOCATIONPROC       glGetProgramResourceLocation;
extern PFNGLGETPROGRAMRESOURCELOCATIONINDEXPROC  glGetProgramResourceLocationIndex;

// Buffer object read-write access from shader
extern PFNGLSHADERSTORAGEBLOCKBINDINGPROC glShaderStorageBlockBinding;

// Texture buffer range
extern PFNGLTEXBUFFERRANGEPROC        glTexBufferRange;
extern PFNGLTEXTUREBUFFERRANGEEXTPROC glTextureBufferRangeEXT;

// Immutable storage for multisample textures
extern PFNGLTEXSTORAGE2DMULTISAMPLEPROC        glTexStorage2DMultisample;
extern PFNGLTEXSTORAGE3DMULTISAMPLEPROC        glTexStorage3DMultisample;
extern PFNGLTEXTURESTORAGE2DMULTISAMPLEEXTPROC glTextureStorage2DMultisampleEXT;
extern PFNGLTEXTURESTORAGE3DMULTISAMPLEEXTPROC glTextureStorage3DMultisampleEXT;

// Texture views
extern PFNGLTEXTUREVIEWPROC glTextureView;

// Separation of vertex format from buffer object
extern PFNGLBINDVERTEXBUFFERPROC                   glBindVertexBuffer;
extern PFNGLVERTEXATTRIBFORMATPROC                 glVertexAttribFormat;
extern PFNGLVERTEXATTRIBIFORMATPROC                glVertexAttribIFormat;
extern PFNGLVERTEXATTRIBLFORMATPROC                glVertexAttribLFormat;
extern PFNGLVERTEXATTRIBBINDINGPROC                glVertexAttribBinding;
extern PFNGLVERTEXBINDINGDIVISORPROC               glVertexBindingDivisor;
extern PFNGLVERTEXARRAYBINDVERTEXBUFFEREXTPROC     glVertexArrayBindVertexBufferEXT;
extern PFNGLVERTEXARRAYVERTEXATTRIBFORMATEXTPROC   glVertexArrayVertexAttribFormatEXT;
extern PFNGLVERTEXARRAYVERTEXATTRIBIFORMATEXTPROC  glVertexArrayVertexAttribIFormatEXT; 
extern PFNGLVERTEXARRAYVERTEXATTRIBLFORMATEXTPROC  glVertexArrayVertexAttribLFormatEXT; 
extern PFNGLVERTEXARRAYVERTEXATTRIBBINDINGEXTPROC  glVertexArrayVertexAttribBindingEXT; 
extern PFNGLVERTEXARRAYVERTEXBINDINGDIVISOREXTPROC glVertexArrayVertexBindingDivisorEXT;

void PureGLgetFunctionPointers();   /**< Queries OpenGL for all non-1.1 GL and WGL function pointers. */

