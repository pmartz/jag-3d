#ifndef __glExtensions_h_
#define __glExtensions_h_ 1

#ifdef __cplusplus
extern "C" {
#endif

/*
** Copyright (c) 2013 The Khronos Group Inc.
**
** Permission is hereby granted, free of charge, to any person obtaining a
** copy of this software and/or associated documentation files (the
** "Materials"), to deal in the Materials without restriction, including
** without limitation the rights to use, copy, modify, merge, publish,
** distribute, sublicense, and/or sell copies of the Materials, and to
** permit persons to whom the Materials are furnished to do so, subject to
** the following conditions:
**
** The above copyright notice and this permission notice shall be included
** in all copies or substantial portions of the Materials.
**
** THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
** IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
** CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
** TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
** MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS.
*/
/*
** This header is generated from the Khronos OpenGL / OpenGL ES XML
** API Registry. The current version of the Registry, generator scripts
** used to make the header, and the header can be found at
**   http://www.opengl.org/registry/
**
** Khronos $Revision$ on $Date$
*/

#if defined(_WIN32) && !defined(APIENTRY) && !defined(__CYGWIN__) && !defined(__SCITECH_SNAP__)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif
#include <windows.h>
#endif

#ifndef APIENTRY
#define APIENTRY
#endif
#ifndef APIENTRYP
#define APIENTRYP APIENTRY *
#endif
#ifndef GLAPI
#define GLAPI extern
#endif

#define GL_GLEXT_VERSION 20130823

/* Generated C header for:
 * API: gl
 * Profile: compatibility
 * Versions considered: .*
 * Versions emitted: 1\.[2-9]|[234]\.[0-9]
 * Default extensions included: gl
 * Additional extensions included: _nomatch_^
 * Extensions removed: _nomatch_^
 */


#ifndef GL_ARB_vertex_buffer_object
#define GL_ARB_vertex_buffer_object 1
typedef ptrdiff_t GLsizeiptrARB;
typedef ptrdiff_t GLintptrARB;
#define GL_BUFFER_SIZE_ARB                0x8764
#define GL_BUFFER_USAGE_ARB               0x8765
#define GL_ARRAY_BUFFER_ARB               0x8892
#define GL_ELEMENT_ARRAY_BUFFER_ARB       0x8893
#define GL_ARRAY_BUFFER_BINDING_ARB       0x8894
#define GL_ELEMENT_ARRAY_BUFFER_BINDING_ARB 0x8895
#define GL_VERTEX_ARRAY_BUFFER_BINDING_ARB 0x8896
#define GL_NORMAL_ARRAY_BUFFER_BINDING_ARB 0x8897
#define GL_COLOR_ARRAY_BUFFER_BINDING_ARB 0x8898
#define GL_INDEX_ARRAY_BUFFER_BINDING_ARB 0x8899
#define GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING_ARB 0x889A
#define GL_EDGE_FLAG_ARRAY_BUFFER_BINDING_ARB 0x889B
#define GL_SECONDARY_COLOR_ARRAY_BUFFER_BINDING_ARB 0x889C
#define GL_FOG_COORDINATE_ARRAY_BUFFER_BINDING_ARB 0x889D
#define GL_WEIGHT_ARRAY_BUFFER_BINDING_ARB 0x889E
#define GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING_ARB 0x889F
#define GL_READ_ONLY_ARB                  0x88B8
#define GL_WRITE_ONLY_ARB                 0x88B9
#define GL_READ_WRITE_ARB                 0x88BA
#define GL_BUFFER_ACCESS_ARB              0x88BB
#define GL_BUFFER_MAPPED_ARB              0x88BC
#define GL_BUFFER_MAP_POINTER_ARB         0x88BD
#define GL_STREAM_DRAW_ARB                0x88E0
#define GL_STREAM_READ_ARB                0x88E1
#define GL_STREAM_COPY_ARB                0x88E2
#define GL_STATIC_DRAW_ARB                0x88E4
#define GL_STATIC_READ_ARB                0x88E5
#define GL_STATIC_COPY_ARB                0x88E6
#define GL_DYNAMIC_DRAW_ARB               0x88E8
#define GL_DYNAMIC_READ_ARB               0x88E9
#define GL_DYNAMIC_COPY_ARB               0x88EA
typedef void (APIENTRYP PFNGLBINDBUFFERARBPROC) (GLenum target, GLuint buffer);
typedef void (APIENTRYP PFNGLDELETEBUFFERSARBPROC) (GLsizei n, const GLuint *buffers);
typedef void (APIENTRYP PFNGLGENBUFFERSARBPROC) (GLsizei n, GLuint *buffers);
typedef GLboolean (APIENTRYP PFNGLISBUFFERARBPROC) (GLuint buffer);
typedef void (APIENTRYP PFNGLBUFFERDATAARBPROC) (GLenum target, GLsizeiptrARB size, const GLvoid *data, GLenum usage);
typedef void (APIENTRYP PFNGLBUFFERSUBDATAARBPROC) (GLenum target, GLintptrARB offset, GLsizeiptrARB size, const GLvoid *data);
typedef void (APIENTRYP PFNGLGETBUFFERSUBDATAARBPROC) (GLenum target, GLintptrARB offset, GLsizeiptrARB size, GLvoid *data);
typedef void *(APIENTRYP PFNGLMAPBUFFERARBPROC) (GLenum target, GLenum access);
typedef GLboolean (APIENTRYP PFNGLUNMAPBUFFERARBPROC) (GLenum target);
typedef void (APIENTRYP PFNGLGETBUFFERPARAMETERIVARBPROC) (GLenum target, GLenum pname, GLint *params);
typedef void (APIENTRYP PFNGLGETBUFFERPOINTERVARBPROC) (GLenum target, GLenum pname, GLvoid **params);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI void APIENTRY glBindBufferARB (GLenum target, GLuint buffer);
GLAPI void APIENTRY glDeleteBuffersARB (GLsizei n, const GLuint *buffers);
GLAPI void APIENTRY glGenBuffersARB (GLsizei n, GLuint *buffers);
GLAPI GLboolean APIENTRY glIsBufferARB (GLuint buffer);
GLAPI void APIENTRY glBufferDataARB (GLenum target, GLsizeiptrARB size, const GLvoid *data, GLenum usage);
GLAPI void APIENTRY glBufferSubDataARB (GLenum target, GLintptrARB offset, GLsizeiptrARB size, const GLvoid *data);
GLAPI void APIENTRY glGetBufferSubDataARB (GLenum target, GLintptrARB offset, GLsizeiptrARB size, GLvoid *data);
GLAPI void *APIENTRY glMapBufferARB (GLenum target, GLenum access);
GLAPI GLboolean APIENTRY glUnmapBufferARB (GLenum target);
GLAPI void APIENTRY glGetBufferParameterivARB (GLenum target, GLenum pname, GLint *params);
GLAPI void APIENTRY glGetBufferPointervARB (GLenum target, GLenum pname, GLvoid **params);
#endif
#endif /* GL_ARB_vertex_buffer_object */

#ifndef GL_EXT_shader_image_load_store
#define GL_EXT_shader_image_load_store 1
#define GL_MAX_IMAGE_UNITS_EXT            0x8F38
#define GL_MAX_COMBINED_IMAGE_UNITS_AND_FRAGMENT_OUTPUTS_EXT 0x8F39
#define GL_IMAGE_BINDING_NAME_EXT         0x8F3A
#define GL_IMAGE_BINDING_LEVEL_EXT        0x8F3B
#define GL_IMAGE_BINDING_LAYERED_EXT      0x8F3C
#define GL_IMAGE_BINDING_LAYER_EXT        0x8F3D
#define GL_IMAGE_BINDING_ACCESS_EXT       0x8F3E
#define GL_IMAGE_1D_EXT                   0x904C
#define GL_IMAGE_2D_EXT                   0x904D
#define GL_IMAGE_3D_EXT                   0x904E
#define GL_IMAGE_2D_RECT_EXT              0x904F
#define GL_IMAGE_CUBE_EXT                 0x9050
#define GL_IMAGE_BUFFER_EXT               0x9051
#define GL_IMAGE_1D_ARRAY_EXT             0x9052
#define GL_IMAGE_2D_ARRAY_EXT             0x9053
#define GL_IMAGE_CUBE_MAP_ARRAY_EXT       0x9054
#define GL_IMAGE_2D_MULTISAMPLE_EXT       0x9055
#define GL_IMAGE_2D_MULTISAMPLE_ARRAY_EXT 0x9056
#define GL_INT_IMAGE_1D_EXT               0x9057
#define GL_INT_IMAGE_2D_EXT               0x9058
#define GL_INT_IMAGE_3D_EXT               0x9059
#define GL_INT_IMAGE_2D_RECT_EXT          0x905A
#define GL_INT_IMAGE_CUBE_EXT             0x905B
#define GL_INT_IMAGE_BUFFER_EXT           0x905C
#define GL_INT_IMAGE_1D_ARRAY_EXT         0x905D
#define GL_INT_IMAGE_2D_ARRAY_EXT         0x905E
#define GL_INT_IMAGE_CUBE_MAP_ARRAY_EXT   0x905F
#define GL_INT_IMAGE_2D_MULTISAMPLE_EXT   0x9060
#define GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY_EXT 0x9061
#define GL_UNSIGNED_INT_IMAGE_1D_EXT      0x9062
#define GL_UNSIGNED_INT_IMAGE_2D_EXT      0x9063
#define GL_UNSIGNED_INT_IMAGE_3D_EXT      0x9064
#define GL_UNSIGNED_INT_IMAGE_2D_RECT_EXT 0x9065
#define GL_UNSIGNED_INT_IMAGE_CUBE_EXT    0x9066
#define GL_UNSIGNED_INT_IMAGE_BUFFER_EXT  0x9067
#define GL_UNSIGNED_INT_IMAGE_1D_ARRAY_EXT 0x9068
#define GL_UNSIGNED_INT_IMAGE_2D_ARRAY_EXT 0x9069
#define GL_UNSIGNED_INT_IMAGE_CUBE_MAP_ARRAY_EXT 0x906A
#define GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_EXT 0x906B
#define GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY_EXT 0x906C
#define GL_MAX_IMAGE_SAMPLES_EXT          0x906D
#define GL_IMAGE_BINDING_FORMAT_EXT       0x906E
#define GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT_EXT 0x00000001
#define GL_ELEMENT_ARRAY_BARRIER_BIT_EXT  0x00000002
#define GL_UNIFORM_BARRIER_BIT_EXT        0x00000004
#define GL_TEXTURE_FETCH_BARRIER_BIT_EXT  0x00000008
#define GL_SHADER_IMAGE_ACCESS_BARRIER_BIT_EXT 0x00000020
#define GL_COMMAND_BARRIER_BIT_EXT        0x00000040
#define GL_PIXEL_BUFFER_BARRIER_BIT_EXT   0x00000080
#define GL_TEXTURE_UPDATE_BARRIER_BIT_EXT 0x00000100
#define GL_BUFFER_UPDATE_BARRIER_BIT_EXT  0x00000200
#define GL_FRAMEBUFFER_BARRIER_BIT_EXT    0x00000400
#define GL_TRANSFORM_FEEDBACK_BARRIER_BIT_EXT 0x00000800
#define GL_ATOMIC_COUNTER_BARRIER_BIT_EXT 0x00001000
#define GL_ALL_BARRIER_BITS_EXT           0xFFFFFFFF
typedef void (APIENTRYP PFNGLBINDIMAGETEXTUREEXTPROC) (GLuint index, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLint format);
typedef void (APIENTRYP PFNGLMEMORYBARRIEREXTPROC) (GLbitfield barriers);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI void APIENTRY glBindImageTextureEXT (GLuint index, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLint format);
GLAPI void APIENTRY glMemoryBarrierEXT (GLbitfield barriers);
#endif
#endif /* GL_EXT_shader_image_load_store */

#ifndef GL_NVX_conditional_render
#define GL_NVX_conditional_render 1
typedef void (APIENTRYP PFNGLBEGINCONDITIONALRENDERNVXPROC) (GLuint id);
typedef void (APIENTRYP PFNGLENDCONDITIONALRENDERNVXPROC) (void);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI void APIENTRY glBeginConditionalRenderNVX (GLuint id);
GLAPI void APIENTRY glEndConditionalRenderNVX (void);
#endif
#endif /* GL_NVX_conditional_render */

#ifndef GL_NV_bindless_multi_draw_indirect
#define GL_NV_bindless_multi_draw_indirect 1
typedef void (APIENTRYP PFNGLMULTIDRAWARRAYSINDIRECTBINDLESSNVPROC) (GLenum mode, const GLvoid *indirect, GLsizei drawCount, GLsizei stride, GLint vertexBufferCount);
typedef void (APIENTRYP PFNGLMULTIDRAWELEMENTSINDIRECTBINDLESSNVPROC) (GLenum mode, GLenum type, const GLvoid *indirect, GLsizei drawCount, GLsizei stride, GLint vertexBufferCount);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI void APIENTRY glMultiDrawArraysIndirectBindlessNV (GLenum mode, const GLvoid *indirect, GLsizei drawCount, GLsizei stride, GLint vertexBufferCount);
GLAPI void APIENTRY glMultiDrawElementsIndirectBindlessNV (GLenum mode, GLenum type, const GLvoid *indirect, GLsizei drawCount, GLsizei stride, GLint vertexBufferCount);
#endif
#endif /* GL_NV_bindless_multi_draw_indirect */

#ifndef GL_NV_bindless_texture
#define GL_NV_bindless_texture 1
typedef GLuint64 (APIENTRYP PFNGLGETTEXTUREHANDLENVPROC) (GLuint texture);
typedef GLuint64 (APIENTRYP PFNGLGETTEXTURESAMPLERHANDLENVPROC) (GLuint texture, GLuint sampler);
typedef void (APIENTRYP PFNGLMAKETEXTUREHANDLERESIDENTNVPROC) (GLuint64 handle);
typedef void (APIENTRYP PFNGLMAKETEXTUREHANDLENONRESIDENTNVPROC) (GLuint64 handle);
typedef GLuint64 (APIENTRYP PFNGLGETIMAGEHANDLENVPROC) (GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum format);
typedef void (APIENTRYP PFNGLMAKEIMAGEHANDLERESIDENTNVPROC) (GLuint64 handle, GLenum access);
typedef void (APIENTRYP PFNGLMAKEIMAGEHANDLENONRESIDENTNVPROC) (GLuint64 handle);
typedef void (APIENTRYP PFNGLUNIFORMHANDLEUI64NVPROC) (GLint location, GLuint64 value);
typedef void (APIENTRYP PFNGLUNIFORMHANDLEUI64VNVPROC) (GLint location, GLsizei count, const GLuint64 *value);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORMHANDLEUI64NVPROC) (GLuint program, GLint location, GLuint64 value);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORMHANDLEUI64VNVPROC) (GLuint program, GLint location, GLsizei count, const GLuint64 *values);
typedef GLboolean (APIENTRYP PFNGLISTEXTUREHANDLERESIDENTNVPROC) (GLuint64 handle);
typedef GLboolean (APIENTRYP PFNGLISIMAGEHANDLERESIDENTNVPROC) (GLuint64 handle);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI GLuint64 APIENTRY glGetTextureHandleNV (GLuint texture);
GLAPI GLuint64 APIENTRY glGetTextureSamplerHandleNV (GLuint texture, GLuint sampler);
GLAPI void APIENTRY glMakeTextureHandleResidentNV (GLuint64 handle);
GLAPI void APIENTRY glMakeTextureHandleNonResidentNV (GLuint64 handle);
GLAPI GLuint64 APIENTRY glGetImageHandleNV (GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum format);
GLAPI void APIENTRY glMakeImageHandleResidentNV (GLuint64 handle, GLenum access);
GLAPI void APIENTRY glMakeImageHandleNonResidentNV (GLuint64 handle);
GLAPI void APIENTRY glUniformHandleui64NV (GLint location, GLuint64 value);
GLAPI void APIENTRY glUniformHandleui64vNV (GLint location, GLsizei count, const GLuint64 *value);
GLAPI void APIENTRY glProgramUniformHandleui64NV (GLuint program, GLint location, GLuint64 value);
GLAPI void APIENTRY glProgramUniformHandleui64vNV (GLuint program, GLint location, GLsizei count, const GLuint64 *values);
GLAPI GLboolean APIENTRY glIsTextureHandleResidentNV (GLuint64 handle);
GLAPI GLboolean APIENTRY glIsImageHandleResidentNV (GLuint64 handle);
#endif
#endif /* GL_NV_bindless_texture */

#ifndef GL_NV_blend_equation_advanced
#define GL_NV_blend_equation_advanced 1
#define GL_BLEND_ADVANCED_COHERENT_NV     0x9285
#define GL_BLEND_OVERLAP_NV               0x9281
#define GL_BLEND_PREMULTIPLIED_SRC_NV     0x9280
#define GL_COLORBURN_NV                   0x929A
#define GL_COLORDODGE_NV                  0x9299
#define GL_CONJOINT_NV                    0x9284
#define GL_CONTRAST_NV                    0x92A1
#define GL_DARKEN_NV                      0x9297
#define GL_DIFFERENCE_NV                  0x929E
#define GL_DISJOINT_NV                    0x9283
#define GL_DST_ATOP_NV                    0x928F
#define GL_DST_IN_NV                      0x928B
#define GL_DST_NV                         0x9287
#define GL_DST_OUT_NV                     0x928D
#define GL_DST_OVER_NV                    0x9289
#define GL_EXCLUSION_NV                   0x92A0
#define GL_HARDLIGHT_NV                   0x929B
#define GL_HARDMIX_NV                     0x92A9
#define GL_HSL_COLOR_NV                   0x92AF
#define GL_HSL_HUE_NV                     0x92AD
#define GL_HSL_LUMINOSITY_NV              0x92B0
#define GL_HSL_SATURATION_NV              0x92AE
#define GL_INVERT_OVG_NV                  0x92B4
#define GL_INVERT_RGB_NV                  0x92A3
#define GL_LIGHTEN_NV                     0x9298
#define GL_LINEARBURN_NV                  0x92A5
#define GL_LINEARDODGE_NV                 0x92A4
#define GL_LINEARLIGHT_NV                 0x92A7
#define GL_MINUS_CLAMPED_NV               0x92B3
#define GL_MINUS_NV                       0x929F
#define GL_MULTIPLY_NV                    0x9294
#define GL_OVERLAY_NV                     0x9296
#define GL_PINLIGHT_NV                    0x92A8
#define GL_PLUS_CLAMPED_ALPHA_NV          0x92B2
#define GL_PLUS_CLAMPED_NV                0x92B1
#define GL_PLUS_DARKER_NV                 0x9292
#define GL_PLUS_NV                        0x9291
#define GL_SCREEN_NV                      0x9295
#define GL_SOFTLIGHT_NV                   0x929C
#define GL_SRC_ATOP_NV                    0x928E
#define GL_SRC_IN_NV                      0x928A
#define GL_SRC_NV                         0x9286
#define GL_SRC_OUT_NV                     0x928C
#define GL_SRC_OVER_NV                    0x9288
#define GL_UNCORRELATED_NV                0x9282
#define GL_VIVIDLIGHT_NV                  0x92A6
typedef void (APIENTRYP PFNGLBLENDPARAMETERINVPROC) (GLenum pname, GLint value);
typedef void (APIENTRYP PFNGLBLENDBARRIERNVPROC) (void);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI void APIENTRY glBlendParameteriNV (GLenum pname, GLint value);
GLAPI void APIENTRY glBlendBarrierNV (void);
#endif
#endif /* GL_NV_blend_equation_advanced */

#ifndef GL_NV_blend_equation_advanced_coherent
#define GL_NV_blend_equation_advanced_coherent 1
#endif /* GL_NV_blend_equation_advanced_coherent */

#ifndef GL_NV_blend_square
#define GL_NV_blend_square 1
#endif /* GL_NV_blend_square */

#ifndef GL_NV_compute_program5
#define GL_NV_compute_program5 1
#define GL_COMPUTE_PROGRAM_NV             0x90FB
#define GL_COMPUTE_PROGRAM_PARAMETER_BUFFER_NV 0x90FC
#endif /* GL_NV_compute_program5 */

#ifndef GL_NV_conditional_render
#define GL_NV_conditional_render 1
#define GL_QUERY_WAIT_NV                  0x8E13
#define GL_QUERY_NO_WAIT_NV               0x8E14
#define GL_QUERY_BY_REGION_WAIT_NV        0x8E15
#define GL_QUERY_BY_REGION_NO_WAIT_NV     0x8E16
typedef void (APIENTRYP PFNGLBEGINCONDITIONALRENDERNVPROC) (GLuint id, GLenum mode);
typedef void (APIENTRYP PFNGLENDCONDITIONALRENDERNVPROC) (void);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI void APIENTRY glBeginConditionalRenderNV (GLuint id, GLenum mode);
GLAPI void APIENTRY glEndConditionalRenderNV (void);
#endif
#endif /* GL_NV_conditional_render */

#ifndef GL_NV_copy_depth_to_color
#define GL_NV_copy_depth_to_color 1
#define GL_DEPTH_STENCIL_TO_RGBA_NV       0x886E
#define GL_DEPTH_STENCIL_TO_BGRA_NV       0x886F
#endif /* GL_NV_copy_depth_to_color */

#ifndef GL_NV_copy_image
#define GL_NV_copy_image 1
typedef void (APIENTRYP PFNGLCOPYIMAGESUBDATANVPROC) (GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei width, GLsizei height, GLsizei depth);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI void APIENTRY glCopyImageSubDataNV (GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei width, GLsizei height, GLsizei depth);
#endif
#endif /* GL_NV_copy_image */

#ifndef GL_NV_deep_texture3D
#define GL_NV_deep_texture3D 1
#define GL_MAX_DEEP_3D_TEXTURE_WIDTH_HEIGHT_NV 0x90D0
#define GL_MAX_DEEP_3D_TEXTURE_DEPTH_NV   0x90D1
#endif /* GL_NV_deep_texture3D */

#ifndef GL_NV_depth_buffer_float
#define GL_NV_depth_buffer_float 1
#define GL_DEPTH_COMPONENT32F_NV          0x8DAB
#define GL_DEPTH32F_STENCIL8_NV           0x8DAC
#define GL_FLOAT_32_UNSIGNED_INT_24_8_REV_NV 0x8DAD
#define GL_DEPTH_BUFFER_FLOAT_MODE_NV     0x8DAF
typedef void (APIENTRYP PFNGLDEPTHRANGEDNVPROC) (GLdouble zNear, GLdouble zFar);
typedef void (APIENTRYP PFNGLCLEARDEPTHDNVPROC) (GLdouble depth);
typedef void (APIENTRYP PFNGLDEPTHBOUNDSDNVPROC) (GLdouble zmin, GLdouble zmax);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI void APIENTRY glDepthRangedNV (GLdouble zNear, GLdouble zFar);
GLAPI void APIENTRY glClearDepthdNV (GLdouble depth);
GLAPI void APIENTRY glDepthBoundsdNV (GLdouble zmin, GLdouble zmax);
#endif
#endif /* GL_NV_depth_buffer_float */

#ifndef GL_NV_depth_clamp
#define GL_NV_depth_clamp 1
#define GL_DEPTH_CLAMP_NV                 0x864F
#endif /* GL_NV_depth_clamp */

#ifndef GL_NV_draw_texture
#define GL_NV_draw_texture 1
typedef void (APIENTRYP PFNGLDRAWTEXTURENVPROC) (GLuint texture, GLuint sampler, GLfloat x0, GLfloat y0, GLfloat x1, GLfloat y1, GLfloat z, GLfloat s0, GLfloat t0, GLfloat s1, GLfloat t1);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI void APIENTRY glDrawTextureNV (GLuint texture, GLuint sampler, GLfloat x0, GLfloat y0, GLfloat x1, GLfloat y1, GLfloat z, GLfloat s0, GLfloat t0, GLfloat s1, GLfloat t1);
#endif
#endif /* GL_NV_draw_texture */

#ifndef GL_NV_evaluators
#define GL_NV_evaluators 1
#define GL_EVAL_2D_NV                     0x86C0
#define GL_EVAL_TRIANGULAR_2D_NV          0x86C1
#define GL_MAP_TESSELLATION_NV            0x86C2
#define GL_MAP_ATTRIB_U_ORDER_NV          0x86C3
#define GL_MAP_ATTRIB_V_ORDER_NV          0x86C4
#define GL_EVAL_FRACTIONAL_TESSELLATION_NV 0x86C5
#define GL_EVAL_VERTEX_ATTRIB0_NV         0x86C6
#define GL_EVAL_VERTEX_ATTRIB1_NV         0x86C7
#define GL_EVAL_VERTEX_ATTRIB2_NV         0x86C8
#define GL_EVAL_VERTEX_ATTRIB3_NV         0x86C9
#define GL_EVAL_VERTEX_ATTRIB4_NV         0x86CA
#define GL_EVAL_VERTEX_ATTRIB5_NV         0x86CB
#define GL_EVAL_VERTEX_ATTRIB6_NV         0x86CC
#define GL_EVAL_VERTEX_ATTRIB7_NV         0x86CD
#define GL_EVAL_VERTEX_ATTRIB8_NV         0x86CE
#define GL_EVAL_VERTEX_ATTRIB9_NV         0x86CF
#define GL_EVAL_VERTEX_ATTRIB10_NV        0x86D0
#define GL_EVAL_VERTEX_ATTRIB11_NV        0x86D1
#define GL_EVAL_VERTEX_ATTRIB12_NV        0x86D2
#define GL_EVAL_VERTEX_ATTRIB13_NV        0x86D3
#define GL_EVAL_VERTEX_ATTRIB14_NV        0x86D4
#define GL_EVAL_VERTEX_ATTRIB15_NV        0x86D5
#define GL_MAX_MAP_TESSELLATION_NV        0x86D6
#define GL_MAX_RATIONAL_EVAL_ORDER_NV     0x86D7
typedef void (APIENTRYP PFNGLMAPCONTROLPOINTSNVPROC) (GLenum target, GLuint index, GLenum type, GLsizei ustride, GLsizei vstride, GLint uorder, GLint vorder, GLboolean packed, const GLvoid *points);
typedef void (APIENTRYP PFNGLMAPPARAMETERIVNVPROC) (GLenum target, GLenum pname, const GLint *params);
typedef void (APIENTRYP PFNGLMAPPARAMETERFVNVPROC) (GLenum target, GLenum pname, const GLfloat *params);
typedef void (APIENTRYP PFNGLGETMAPCONTROLPOINTSNVPROC) (GLenum target, GLuint index, GLenum type, GLsizei ustride, GLsizei vstride, GLboolean packed, GLvoid *points);
typedef void (APIENTRYP PFNGLGETMAPPARAMETERIVNVPROC) (GLenum target, GLenum pname, GLint *params);
typedef void (APIENTRYP PFNGLGETMAPPARAMETERFVNVPROC) (GLenum target, GLenum pname, GLfloat *params);
typedef void (APIENTRYP PFNGLGETMAPATTRIBPARAMETERIVNVPROC) (GLenum target, GLuint index, GLenum pname, GLint *params);
typedef void (APIENTRYP PFNGLGETMAPATTRIBPARAMETERFVNVPROC) (GLenum target, GLuint index, GLenum pname, GLfloat *params);
typedef void (APIENTRYP PFNGLEVALMAPSNVPROC) (GLenum target, GLenum mode);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI void APIENTRY glMapControlPointsNV (GLenum target, GLuint index, GLenum type, GLsizei ustride, GLsizei vstride, GLint uorder, GLint vorder, GLboolean packed, const GLvoid *points);
GLAPI void APIENTRY glMapParameterivNV (GLenum target, GLenum pname, const GLint *params);
GLAPI void APIENTRY glMapParameterfvNV (GLenum target, GLenum pname, const GLfloat *params);
GLAPI void APIENTRY glGetMapControlPointsNV (GLenum target, GLuint index, GLenum type, GLsizei ustride, GLsizei vstride, GLboolean packed, GLvoid *points);
GLAPI void APIENTRY glGetMapParameterivNV (GLenum target, GLenum pname, GLint *params);
GLAPI void APIENTRY glGetMapParameterfvNV (GLenum target, GLenum pname, GLfloat *params);
GLAPI void APIENTRY glGetMapAttribParameterivNV (GLenum target, GLuint index, GLenum pname, GLint *params);
GLAPI void APIENTRY glGetMapAttribParameterfvNV (GLenum target, GLuint index, GLenum pname, GLfloat *params);
GLAPI void APIENTRY glEvalMapsNV (GLenum target, GLenum mode);
#endif
#endif /* GL_NV_evaluators */

#ifndef GL_NV_explicit_multisample
#define GL_NV_explicit_multisample 1
#define GL_SAMPLE_POSITION_NV             0x8E50
#define GL_SAMPLE_MASK_NV                 0x8E51
#define GL_SAMPLE_MASK_VALUE_NV           0x8E52
#define GL_TEXTURE_BINDING_RENDERBUFFER_NV 0x8E53
#define GL_TEXTURE_RENDERBUFFER_DATA_STORE_BINDING_NV 0x8E54
#define GL_TEXTURE_RENDERBUFFER_NV        0x8E55
#define GL_SAMPLER_RENDERBUFFER_NV        0x8E56
#define GL_INT_SAMPLER_RENDERBUFFER_NV    0x8E57
#define GL_UNSIGNED_INT_SAMPLER_RENDERBUFFER_NV 0x8E58
#define GL_MAX_SAMPLE_MASK_WORDS_NV       0x8E59
typedef void (APIENTRYP PFNGLGETMULTISAMPLEFVNVPROC) (GLenum pname, GLuint index, GLfloat *val);
typedef void (APIENTRYP PFNGLSAMPLEMASKINDEXEDNVPROC) (GLuint index, GLbitfield mask);
typedef void (APIENTRYP PFNGLTEXRENDERBUFFERNVPROC) (GLenum target, GLuint renderbuffer);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI void APIENTRY glGetMultisamplefvNV (GLenum pname, GLuint index, GLfloat *val);
GLAPI void APIENTRY glSampleMaskIndexedNV (GLuint index, GLbitfield mask);
GLAPI void APIENTRY glTexRenderbufferNV (GLenum target, GLuint renderbuffer);
#endif
#endif /* GL_NV_explicit_multisample */

#ifndef GL_NV_fence
#define GL_NV_fence 1
#define GL_ALL_COMPLETED_NV               0x84F2
#define GL_FENCE_STATUS_NV                0x84F3
#define GL_FENCE_CONDITION_NV             0x84F4
typedef void (APIENTRYP PFNGLDELETEFENCESNVPROC) (GLsizei n, const GLuint *fences);
typedef void (APIENTRYP PFNGLGENFENCESNVPROC) (GLsizei n, GLuint *fences);
typedef GLboolean (APIENTRYP PFNGLISFENCENVPROC) (GLuint fence);
typedef GLboolean (APIENTRYP PFNGLTESTFENCENVPROC) (GLuint fence);
typedef void (APIENTRYP PFNGLGETFENCEIVNVPROC) (GLuint fence, GLenum pname, GLint *params);
typedef void (APIENTRYP PFNGLFINISHFENCENVPROC) (GLuint fence);
typedef void (APIENTRYP PFNGLSETFENCENVPROC) (GLuint fence, GLenum condition);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI void APIENTRY glDeleteFencesNV (GLsizei n, const GLuint *fences);
GLAPI void APIENTRY glGenFencesNV (GLsizei n, GLuint *fences);
GLAPI GLboolean APIENTRY glIsFenceNV (GLuint fence);
GLAPI GLboolean APIENTRY glTestFenceNV (GLuint fence);
GLAPI void APIENTRY glGetFenceivNV (GLuint fence, GLenum pname, GLint *params);
GLAPI void APIENTRY glFinishFenceNV (GLuint fence);
GLAPI void APIENTRY glSetFenceNV (GLuint fence, GLenum condition);
#endif
#endif /* GL_NV_fence */

#ifndef GL_NV_float_buffer
#define GL_NV_float_buffer 1
#define GL_FLOAT_R_NV                     0x8880
#define GL_FLOAT_RG_NV                    0x8881
#define GL_FLOAT_RGB_NV                   0x8882
#define GL_FLOAT_RGBA_NV                  0x8883
#define GL_FLOAT_R16_NV                   0x8884
#define GL_FLOAT_R32_NV                   0x8885
#define GL_FLOAT_RG16_NV                  0x8886
#define GL_FLOAT_RG32_NV                  0x8887
#define GL_FLOAT_RGB16_NV                 0x8888
#define GL_FLOAT_RGB32_NV                 0x8889
#define GL_FLOAT_RGBA16_NV                0x888A
#define GL_FLOAT_RGBA32_NV                0x888B
#define GL_TEXTURE_FLOAT_COMPONENTS_NV    0x888C
#define GL_FLOAT_CLEAR_COLOR_VALUE_NV     0x888D
#define GL_FLOAT_RGBA_MODE_NV             0x888E
#endif /* GL_NV_float_buffer */

#ifndef GL_NV_fog_distance
#define GL_NV_fog_distance 1
#define GL_FOG_DISTANCE_MODE_NV           0x855A
#define GL_EYE_RADIAL_NV                  0x855B
#define GL_EYE_PLANE_ABSOLUTE_NV          0x855C
#endif /* GL_NV_fog_distance */

#ifndef GL_NV_fragment_program
#define GL_NV_fragment_program 1
#define GL_MAX_FRAGMENT_PROGRAM_LOCAL_PARAMETERS_NV 0x8868
#define GL_FRAGMENT_PROGRAM_NV            0x8870
#define GL_MAX_TEXTURE_COORDS_NV          0x8871
#define GL_MAX_TEXTURE_IMAGE_UNITS_NV     0x8872
#define GL_FRAGMENT_PROGRAM_BINDING_NV    0x8873
#define GL_PROGRAM_ERROR_STRING_NV        0x8874
typedef void (APIENTRYP PFNGLPROGRAMNAMEDPARAMETER4FNVPROC) (GLuint id, GLsizei len, const GLubyte *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (APIENTRYP PFNGLPROGRAMNAMEDPARAMETER4FVNVPROC) (GLuint id, GLsizei len, const GLubyte *name, const GLfloat *v);
typedef void (APIENTRYP PFNGLPROGRAMNAMEDPARAMETER4DNVPROC) (GLuint id, GLsizei len, const GLubyte *name, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (APIENTRYP PFNGLPROGRAMNAMEDPARAMETER4DVNVPROC) (GLuint id, GLsizei len, const GLubyte *name, const GLdouble *v);
typedef void (APIENTRYP PFNGLGETPROGRAMNAMEDPARAMETERFVNVPROC) (GLuint id, GLsizei len, const GLubyte *name, GLfloat *params);
typedef void (APIENTRYP PFNGLGETPROGRAMNAMEDPARAMETERDVNVPROC) (GLuint id, GLsizei len, const GLubyte *name, GLdouble *params);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI void APIENTRY glProgramNamedParameter4fNV (GLuint id, GLsizei len, const GLubyte *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
GLAPI void APIENTRY glProgramNamedParameter4fvNV (GLuint id, GLsizei len, const GLubyte *name, const GLfloat *v);
GLAPI void APIENTRY glProgramNamedParameter4dNV (GLuint id, GLsizei len, const GLubyte *name, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
GLAPI void APIENTRY glProgramNamedParameter4dvNV (GLuint id, GLsizei len, const GLubyte *name, const GLdouble *v);
GLAPI void APIENTRY glGetProgramNamedParameterfvNV (GLuint id, GLsizei len, const GLubyte *name, GLfloat *params);
GLAPI void APIENTRY glGetProgramNamedParameterdvNV (GLuint id, GLsizei len, const GLubyte *name, GLdouble *params);
#endif
#endif /* GL_NV_fragment_program */

#ifndef GL_NV_fragment_program2
#define GL_NV_fragment_program2 1
#define GL_MAX_PROGRAM_EXEC_INSTRUCTIONS_NV 0x88F4
#define GL_MAX_PROGRAM_CALL_DEPTH_NV      0x88F5
#define GL_MAX_PROGRAM_IF_DEPTH_NV        0x88F6
#define GL_MAX_PROGRAM_LOOP_DEPTH_NV      0x88F7
#define GL_MAX_PROGRAM_LOOP_COUNT_NV      0x88F8
#endif /* GL_NV_fragment_program2 */

#ifndef GL_NV_fragment_program4
#define GL_NV_fragment_program4 1
#endif /* GL_NV_fragment_program4 */

#ifndef GL_NV_fragment_program_option
#define GL_NV_fragment_program_option 1
#endif /* GL_NV_fragment_program_option */

#ifndef GL_NV_framebuffer_multisample_coverage
#define GL_NV_framebuffer_multisample_coverage 1
#define GL_RENDERBUFFER_COVERAGE_SAMPLES_NV 0x8CAB
#define GL_RENDERBUFFER_COLOR_SAMPLES_NV  0x8E10
#define GL_MAX_MULTISAMPLE_COVERAGE_MODES_NV 0x8E11
#define GL_MULTISAMPLE_COVERAGE_MODES_NV  0x8E12
typedef void (APIENTRYP PFNGLRENDERBUFFERSTORAGEMULTISAMPLECOVERAGENVPROC) (GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLenum internalformat, GLsizei width, GLsizei height);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI void APIENTRY glRenderbufferStorageMultisampleCoverageNV (GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLenum internalformat, GLsizei width, GLsizei height);
#endif
#endif /* GL_NV_framebuffer_multisample_coverage */

#ifndef GL_NV_geometry_program4
#define GL_NV_geometry_program4 1
#define GL_GEOMETRY_PROGRAM_NV            0x8C26
#define GL_MAX_PROGRAM_OUTPUT_VERTICES_NV 0x8C27
#define GL_MAX_PROGRAM_TOTAL_OUTPUT_COMPONENTS_NV 0x8C28
typedef void (APIENTRYP PFNGLPROGRAMVERTEXLIMITNVPROC) (GLenum target, GLint limit);
typedef void (APIENTRYP PFNGLFRAMEBUFFERTEXTUREEXTPROC) (GLenum target, GLenum attachment, GLuint texture, GLint level);
typedef void (APIENTRYP PFNGLFRAMEBUFFERTEXTURELAYEREXTPROC) (GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
typedef void (APIENTRYP PFNGLFRAMEBUFFERTEXTUREFACEEXTPROC) (GLenum target, GLenum attachment, GLuint texture, GLint level, GLenum face);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI void APIENTRY glProgramVertexLimitNV (GLenum target, GLint limit);
GLAPI void APIENTRY glFramebufferTextureEXT (GLenum target, GLenum attachment, GLuint texture, GLint level);
GLAPI void APIENTRY glFramebufferTextureLayerEXT (GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
GLAPI void APIENTRY glFramebufferTextureFaceEXT (GLenum target, GLenum attachment, GLuint texture, GLint level, GLenum face);
#endif
#endif /* GL_NV_geometry_program4 */

#ifndef GL_NV_geometry_shader4
#define GL_NV_geometry_shader4 1
#endif /* GL_NV_geometry_shader4 */

#ifndef GL_NV_gpu_program4
#define GL_NV_gpu_program4 1
#define GL_MIN_PROGRAM_TEXEL_OFFSET_NV    0x8904
#define GL_MAX_PROGRAM_TEXEL_OFFSET_NV    0x8905
#define GL_PROGRAM_ATTRIB_COMPONENTS_NV   0x8906
#define GL_PROGRAM_RESULT_COMPONENTS_NV   0x8907
#define GL_MAX_PROGRAM_ATTRIB_COMPONENTS_NV 0x8908
#define GL_MAX_PROGRAM_RESULT_COMPONENTS_NV 0x8909
#define GL_MAX_PROGRAM_GENERIC_ATTRIBS_NV 0x8DA5
#define GL_MAX_PROGRAM_GENERIC_RESULTS_NV 0x8DA6
typedef void (APIENTRYP PFNGLPROGRAMLOCALPARAMETERI4INVPROC) (GLenum target, GLuint index, GLint x, GLint y, GLint z, GLint w);
typedef void (APIENTRYP PFNGLPROGRAMLOCALPARAMETERI4IVNVPROC) (GLenum target, GLuint index, const GLint *params);
typedef void (APIENTRYP PFNGLPROGRAMLOCALPARAMETERSI4IVNVPROC) (GLenum target, GLuint index, GLsizei count, const GLint *params);
typedef void (APIENTRYP PFNGLPROGRAMLOCALPARAMETERI4UINVPROC) (GLenum target, GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
typedef void (APIENTRYP PFNGLPROGRAMLOCALPARAMETERI4UIVNVPROC) (GLenum target, GLuint index, const GLuint *params);
typedef void (APIENTRYP PFNGLPROGRAMLOCALPARAMETERSI4UIVNVPROC) (GLenum target, GLuint index, GLsizei count, const GLuint *params);
typedef void (APIENTRYP PFNGLPROGRAMENVPARAMETERI4INVPROC) (GLenum target, GLuint index, GLint x, GLint y, GLint z, GLint w);
typedef void (APIENTRYP PFNGLPROGRAMENVPARAMETERI4IVNVPROC) (GLenum target, GLuint index, const GLint *params);
typedef void (APIENTRYP PFNGLPROGRAMENVPARAMETERSI4IVNVPROC) (GLenum target, GLuint index, GLsizei count, const GLint *params);
typedef void (APIENTRYP PFNGLPROGRAMENVPARAMETERI4UINVPROC) (GLenum target, GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
typedef void (APIENTRYP PFNGLPROGRAMENVPARAMETERI4UIVNVPROC) (GLenum target, GLuint index, const GLuint *params);
typedef void (APIENTRYP PFNGLPROGRAMENVPARAMETERSI4UIVNVPROC) (GLenum target, GLuint index, GLsizei count, const GLuint *params);
typedef void (APIENTRYP PFNGLGETPROGRAMLOCALPARAMETERIIVNVPROC) (GLenum target, GLuint index, GLint *params);
typedef void (APIENTRYP PFNGLGETPROGRAMLOCALPARAMETERIUIVNVPROC) (GLenum target, GLuint index, GLuint *params);
typedef void (APIENTRYP PFNGLGETPROGRAMENVPARAMETERIIVNVPROC) (GLenum target, GLuint index, GLint *params);
typedef void (APIENTRYP PFNGLGETPROGRAMENVPARAMETERIUIVNVPROC) (GLenum target, GLuint index, GLuint *params);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI void APIENTRY glProgramLocalParameterI4iNV (GLenum target, GLuint index, GLint x, GLint y, GLint z, GLint w);
GLAPI void APIENTRY glProgramLocalParameterI4ivNV (GLenum target, GLuint index, const GLint *params);
GLAPI void APIENTRY glProgramLocalParametersI4ivNV (GLenum target, GLuint index, GLsizei count, const GLint *params);
GLAPI void APIENTRY glProgramLocalParameterI4uiNV (GLenum target, GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
GLAPI void APIENTRY glProgramLocalParameterI4uivNV (GLenum target, GLuint index, const GLuint *params);
GLAPI void APIENTRY glProgramLocalParametersI4uivNV (GLenum target, GLuint index, GLsizei count, const GLuint *params);
GLAPI void APIENTRY glProgramEnvParameterI4iNV (GLenum target, GLuint index, GLint x, GLint y, GLint z, GLint w);
GLAPI void APIENTRY glProgramEnvParameterI4ivNV (GLenum target, GLuint index, const GLint *params);
GLAPI void APIENTRY glProgramEnvParametersI4ivNV (GLenum target, GLuint index, GLsizei count, const GLint *params);
GLAPI void APIENTRY glProgramEnvParameterI4uiNV (GLenum target, GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
GLAPI void APIENTRY glProgramEnvParameterI4uivNV (GLenum target, GLuint index, const GLuint *params);
GLAPI void APIENTRY glProgramEnvParametersI4uivNV (GLenum target, GLuint index, GLsizei count, const GLuint *params);
GLAPI void APIENTRY glGetProgramLocalParameterIivNV (GLenum target, GLuint index, GLint *params);
GLAPI void APIENTRY glGetProgramLocalParameterIuivNV (GLenum target, GLuint index, GLuint *params);
GLAPI void APIENTRY glGetProgramEnvParameterIivNV (GLenum target, GLuint index, GLint *params);
GLAPI void APIENTRY glGetProgramEnvParameterIuivNV (GLenum target, GLuint index, GLuint *params);
#endif
#endif /* GL_NV_gpu_program4 */

#ifndef GL_NV_gpu_program5
#define GL_NV_gpu_program5 1
#define GL_MAX_GEOMETRY_PROGRAM_INVOCATIONS_NV 0x8E5A
#define GL_MIN_FRAGMENT_INTERPOLATION_OFFSET_NV 0x8E5B
#define GL_MAX_FRAGMENT_INTERPOLATION_OFFSET_NV 0x8E5C
#define GL_FRAGMENT_PROGRAM_INTERPOLATION_OFFSET_BITS_NV 0x8E5D
#define GL_MIN_PROGRAM_TEXTURE_GATHER_OFFSET_NV 0x8E5E
#define GL_MAX_PROGRAM_TEXTURE_GATHER_OFFSET_NV 0x8E5F
#define GL_MAX_PROGRAM_SUBROUTINE_PARAMETERS_NV 0x8F44
#define GL_MAX_PROGRAM_SUBROUTINE_NUM_NV  0x8F45
typedef void (APIENTRYP PFNGLPROGRAMSUBROUTINEPARAMETERSUIVNVPROC) (GLenum target, GLsizei count, const GLuint *params);
typedef void (APIENTRYP PFNGLGETPROGRAMSUBROUTINEPARAMETERUIVNVPROC) (GLenum target, GLuint index, GLuint *param);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI void APIENTRY glProgramSubroutineParametersuivNV (GLenum target, GLsizei count, const GLuint *params);
GLAPI void APIENTRY glGetProgramSubroutineParameteruivNV (GLenum target, GLuint index, GLuint *param);
#endif
#endif /* GL_NV_gpu_program5 */

#ifndef GL_NV_gpu_program5_mem_extended
#define GL_NV_gpu_program5_mem_extended 1
#endif /* GL_NV_gpu_program5_mem_extended */

#ifndef GL_NV_gpu_shader5
#define GL_NV_gpu_shader5 1
typedef int64_t GLint64EXT;
#define GL_INT64_NV                       0x140E
#define GL_UNSIGNED_INT64_NV              0x140F
#define GL_INT8_NV                        0x8FE0
#define GL_INT8_VEC2_NV                   0x8FE1
#define GL_INT8_VEC3_NV                   0x8FE2
#define GL_INT8_VEC4_NV                   0x8FE3
#define GL_INT16_NV                       0x8FE4
#define GL_INT16_VEC2_NV                  0x8FE5
#define GL_INT16_VEC3_NV                  0x8FE6
#define GL_INT16_VEC4_NV                  0x8FE7
#define GL_INT64_VEC2_NV                  0x8FE9
#define GL_INT64_VEC3_NV                  0x8FEA
#define GL_INT64_VEC4_NV                  0x8FEB
#define GL_UNSIGNED_INT8_NV               0x8FEC
#define GL_UNSIGNED_INT8_VEC2_NV          0x8FED
#define GL_UNSIGNED_INT8_VEC3_NV          0x8FEE
#define GL_UNSIGNED_INT8_VEC4_NV          0x8FEF
#define GL_UNSIGNED_INT16_NV              0x8FF0
#define GL_UNSIGNED_INT16_VEC2_NV         0x8FF1
#define GL_UNSIGNED_INT16_VEC3_NV         0x8FF2
#define GL_UNSIGNED_INT16_VEC4_NV         0x8FF3
#define GL_UNSIGNED_INT64_VEC2_NV         0x8FF5
#define GL_UNSIGNED_INT64_VEC3_NV         0x8FF6
#define GL_UNSIGNED_INT64_VEC4_NV         0x8FF7
#define GL_FLOAT16_NV                     0x8FF8
#define GL_FLOAT16_VEC2_NV                0x8FF9
#define GL_FLOAT16_VEC3_NV                0x8FFA
#define GL_FLOAT16_VEC4_NV                0x8FFB
typedef void (APIENTRYP PFNGLUNIFORM1I64NVPROC) (GLint location, GLint64EXT x);
typedef void (APIENTRYP PFNGLUNIFORM2I64NVPROC) (GLint location, GLint64EXT x, GLint64EXT y);
typedef void (APIENTRYP PFNGLUNIFORM3I64NVPROC) (GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z);
typedef void (APIENTRYP PFNGLUNIFORM4I64NVPROC) (GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z, GLint64EXT w);
typedef void (APIENTRYP PFNGLUNIFORM1I64VNVPROC) (GLint location, GLsizei count, const GLint64EXT *value);
typedef void (APIENTRYP PFNGLUNIFORM2I64VNVPROC) (GLint location, GLsizei count, const GLint64EXT *value);
typedef void (APIENTRYP PFNGLUNIFORM3I64VNVPROC) (GLint location, GLsizei count, const GLint64EXT *value);
typedef void (APIENTRYP PFNGLUNIFORM4I64VNVPROC) (GLint location, GLsizei count, const GLint64EXT *value);
typedef void (APIENTRYP PFNGLUNIFORM1UI64NVPROC) (GLint location, GLuint64EXT x);
typedef void (APIENTRYP PFNGLUNIFORM2UI64NVPROC) (GLint location, GLuint64EXT x, GLuint64EXT y);
typedef void (APIENTRYP PFNGLUNIFORM3UI64NVPROC) (GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z);
typedef void (APIENTRYP PFNGLUNIFORM4UI64NVPROC) (GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z, GLuint64EXT w);
typedef void (APIENTRYP PFNGLUNIFORM1UI64VNVPROC) (GLint location, GLsizei count, const GLuint64EXT *value);
typedef void (APIENTRYP PFNGLUNIFORM2UI64VNVPROC) (GLint location, GLsizei count, const GLuint64EXT *value);
typedef void (APIENTRYP PFNGLUNIFORM3UI64VNVPROC) (GLint location, GLsizei count, const GLuint64EXT *value);
typedef void (APIENTRYP PFNGLUNIFORM4UI64VNVPROC) (GLint location, GLsizei count, const GLuint64EXT *value);
typedef void (APIENTRYP PFNGLGETUNIFORMI64VNVPROC) (GLuint program, GLint location, GLint64EXT *params);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM1I64NVPROC) (GLuint program, GLint location, GLint64EXT x);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM2I64NVPROC) (GLuint program, GLint location, GLint64EXT x, GLint64EXT y);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM3I64NVPROC) (GLuint program, GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM4I64NVPROC) (GLuint program, GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z, GLint64EXT w);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM1I64VNVPROC) (GLuint program, GLint location, GLsizei count, const GLint64EXT *value);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM2I64VNVPROC) (GLuint program, GLint location, GLsizei count, const GLint64EXT *value);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM3I64VNVPROC) (GLuint program, GLint location, GLsizei count, const GLint64EXT *value);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM4I64VNVPROC) (GLuint program, GLint location, GLsizei count, const GLint64EXT *value);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM1UI64NVPROC) (GLuint program, GLint location, GLuint64EXT x);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM2UI64NVPROC) (GLuint program, GLint location, GLuint64EXT x, GLuint64EXT y);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM3UI64NVPROC) (GLuint program, GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM4UI64NVPROC) (GLuint program, GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z, GLuint64EXT w);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM1UI64VNVPROC) (GLuint program, GLint location, GLsizei count, const GLuint64EXT *value);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM2UI64VNVPROC) (GLuint program, GLint location, GLsizei count, const GLuint64EXT *value);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM3UI64VNVPROC) (GLuint program, GLint location, GLsizei count, const GLuint64EXT *value);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORM4UI64VNVPROC) (GLuint program, GLint location, GLsizei count, const GLuint64EXT *value);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI void APIENTRY glUniform1i64NV (GLint location, GLint64EXT x);
GLAPI void APIENTRY glUniform2i64NV (GLint location, GLint64EXT x, GLint64EXT y);
GLAPI void APIENTRY glUniform3i64NV (GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z);
GLAPI void APIENTRY glUniform4i64NV (GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z, GLint64EXT w);
GLAPI void APIENTRY glUniform1i64vNV (GLint location, GLsizei count, const GLint64EXT *value);
GLAPI void APIENTRY glUniform2i64vNV (GLint location, GLsizei count, const GLint64EXT *value);
GLAPI void APIENTRY glUniform3i64vNV (GLint location, GLsizei count, const GLint64EXT *value);
GLAPI void APIENTRY glUniform4i64vNV (GLint location, GLsizei count, const GLint64EXT *value);
GLAPI void APIENTRY glUniform1ui64NV (GLint location, GLuint64EXT x);
GLAPI void APIENTRY glUniform2ui64NV (GLint location, GLuint64EXT x, GLuint64EXT y);
GLAPI void APIENTRY glUniform3ui64NV (GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z);
GLAPI void APIENTRY glUniform4ui64NV (GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z, GLuint64EXT w);
GLAPI void APIENTRY glUniform1ui64vNV (GLint location, GLsizei count, const GLuint64EXT *value);
GLAPI void APIENTRY glUniform2ui64vNV (GLint location, GLsizei count, const GLuint64EXT *value);
GLAPI void APIENTRY glUniform3ui64vNV (GLint location, GLsizei count, const GLuint64EXT *value);
GLAPI void APIENTRY glUniform4ui64vNV (GLint location, GLsizei count, const GLuint64EXT *value);
GLAPI void APIENTRY glGetUniformi64vNV (GLuint program, GLint location, GLint64EXT *params);
GLAPI void APIENTRY glProgramUniform1i64NV (GLuint program, GLint location, GLint64EXT x);
GLAPI void APIENTRY glProgramUniform2i64NV (GLuint program, GLint location, GLint64EXT x, GLint64EXT y);
GLAPI void APIENTRY glProgramUniform3i64NV (GLuint program, GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z);
GLAPI void APIENTRY glProgramUniform4i64NV (GLuint program, GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z, GLint64EXT w);
GLAPI void APIENTRY glProgramUniform1i64vNV (GLuint program, GLint location, GLsizei count, const GLint64EXT *value);
GLAPI void APIENTRY glProgramUniform2i64vNV (GLuint program, GLint location, GLsizei count, const GLint64EXT *value);
GLAPI void APIENTRY glProgramUniform3i64vNV (GLuint program, GLint location, GLsizei count, const GLint64EXT *value);
GLAPI void APIENTRY glProgramUniform4i64vNV (GLuint program, GLint location, GLsizei count, const GLint64EXT *value);
GLAPI void APIENTRY glProgramUniform1ui64NV (GLuint program, GLint location, GLuint64EXT x);
GLAPI void APIENTRY glProgramUniform2ui64NV (GLuint program, GLint location, GLuint64EXT x, GLuint64EXT y);
GLAPI void APIENTRY glProgramUniform3ui64NV (GLuint program, GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z);
GLAPI void APIENTRY glProgramUniform4ui64NV (GLuint program, GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z, GLuint64EXT w);
GLAPI void APIENTRY glProgramUniform1ui64vNV (GLuint program, GLint location, GLsizei count, const GLuint64EXT *value);
GLAPI void APIENTRY glProgramUniform2ui64vNV (GLuint program, GLint location, GLsizei count, const GLuint64EXT *value);
GLAPI void APIENTRY glProgramUniform3ui64vNV (GLuint program, GLint location, GLsizei count, const GLuint64EXT *value);
GLAPI void APIENTRY glProgramUniform4ui64vNV (GLuint program, GLint location, GLsizei count, const GLuint64EXT *value);
#endif
#endif /* GL_NV_gpu_shader5 */

#ifndef GL_NV_half_float
#define GL_NV_half_float 1
typedef unsigned short GLhalfNV;
#define GL_HALF_FLOAT_NV                  0x140B
typedef void (APIENTRYP PFNGLVERTEX2HNVPROC) (GLhalfNV x, GLhalfNV y);
typedef void (APIENTRYP PFNGLVERTEX2HVNVPROC) (const GLhalfNV *v);
typedef void (APIENTRYP PFNGLVERTEX3HNVPROC) (GLhalfNV x, GLhalfNV y, GLhalfNV z);
typedef void (APIENTRYP PFNGLVERTEX3HVNVPROC) (const GLhalfNV *v);
typedef void (APIENTRYP PFNGLVERTEX4HNVPROC) (GLhalfNV x, GLhalfNV y, GLhalfNV z, GLhalfNV w);
typedef void (APIENTRYP PFNGLVERTEX4HVNVPROC) (const GLhalfNV *v);
typedef void (APIENTRYP PFNGLNORMAL3HNVPROC) (GLhalfNV nx, GLhalfNV ny, GLhalfNV nz);
typedef void (APIENTRYP PFNGLNORMAL3HVNVPROC) (const GLhalfNV *v);
typedef void (APIENTRYP PFNGLCOLOR3HNVPROC) (GLhalfNV red, GLhalfNV green, GLhalfNV blue);
typedef void (APIENTRYP PFNGLCOLOR3HVNVPROC) (const GLhalfNV *v);
typedef void (APIENTRYP PFNGLCOLOR4HNVPROC) (GLhalfNV red, GLhalfNV green, GLhalfNV blue, GLhalfNV alpha);
typedef void (APIENTRYP PFNGLCOLOR4HVNVPROC) (const GLhalfNV *v);
typedef void (APIENTRYP PFNGLTEXCOORD1HNVPROC) (GLhalfNV s);
typedef void (APIENTRYP PFNGLTEXCOORD1HVNVPROC) (const GLhalfNV *v);
typedef void (APIENTRYP PFNGLTEXCOORD2HNVPROC) (GLhalfNV s, GLhalfNV t);
typedef void (APIENTRYP PFNGLTEXCOORD2HVNVPROC) (const GLhalfNV *v);
typedef void (APIENTRYP PFNGLTEXCOORD3HNVPROC) (GLhalfNV s, GLhalfNV t, GLhalfNV r);
typedef void (APIENTRYP PFNGLTEXCOORD3HVNVPROC) (const GLhalfNV *v);
typedef void (APIENTRYP PFNGLTEXCOORD4HNVPROC) (GLhalfNV s, GLhalfNV t, GLhalfNV r, GLhalfNV q);
typedef void (APIENTRYP PFNGLTEXCOORD4HVNVPROC) (const GLhalfNV *v);
typedef void (APIENTRYP PFNGLMULTITEXCOORD1HNVPROC) (GLenum target, GLhalfNV s);
typedef void (APIENTRYP PFNGLMULTITEXCOORD1HVNVPROC) (GLenum target, const GLhalfNV *v);
typedef void (APIENTRYP PFNGLMULTITEXCOORD2HNVPROC) (GLenum target, GLhalfNV s, GLhalfNV t);
typedef void (APIENTRYP PFNGLMULTITEXCOORD2HVNVPROC) (GLenum target, const GLhalfNV *v);
typedef void (APIENTRYP PFNGLMULTITEXCOORD3HNVPROC) (GLenum target, GLhalfNV s, GLhalfNV t, GLhalfNV r);
typedef void (APIENTRYP PFNGLMULTITEXCOORD3HVNVPROC) (GLenum target, const GLhalfNV *v);
typedef void (APIENTRYP PFNGLMULTITEXCOORD4HNVPROC) (GLenum target, GLhalfNV s, GLhalfNV t, GLhalfNV r, GLhalfNV q);
typedef void (APIENTRYP PFNGLMULTITEXCOORD4HVNVPROC) (GLenum target, const GLhalfNV *v);
typedef void (APIENTRYP PFNGLFOGCOORDHNVPROC) (GLhalfNV fog);
typedef void (APIENTRYP PFNGLFOGCOORDHVNVPROC) (const GLhalfNV *fog);
typedef void (APIENTRYP PFNGLSECONDARYCOLOR3HNVPROC) (GLhalfNV red, GLhalfNV green, GLhalfNV blue);
typedef void (APIENTRYP PFNGLSECONDARYCOLOR3HVNVPROC) (const GLhalfNV *v);
typedef void (APIENTRYP PFNGLVERTEXWEIGHTHNVPROC) (GLhalfNV weight);
typedef void (APIENTRYP PFNGLVERTEXWEIGHTHVNVPROC) (const GLhalfNV *weight);
typedef void (APIENTRYP PFNGLVERTEXATTRIB1HNVPROC) (GLuint index, GLhalfNV x);
typedef void (APIENTRYP PFNGLVERTEXATTRIB1HVNVPROC) (GLuint index, const GLhalfNV *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIB2HNVPROC) (GLuint index, GLhalfNV x, GLhalfNV y);
typedef void (APIENTRYP PFNGLVERTEXATTRIB2HVNVPROC) (GLuint index, const GLhalfNV *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIB3HNVPROC) (GLuint index, GLhalfNV x, GLhalfNV y, GLhalfNV z);
typedef void (APIENTRYP PFNGLVERTEXATTRIB3HVNVPROC) (GLuint index, const GLhalfNV *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIB4HNVPROC) (GLuint index, GLhalfNV x, GLhalfNV y, GLhalfNV z, GLhalfNV w);
typedef void (APIENTRYP PFNGLVERTEXATTRIB4HVNVPROC) (GLuint index, const GLhalfNV *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBS1HVNVPROC) (GLuint index, GLsizei n, const GLhalfNV *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBS2HVNVPROC) (GLuint index, GLsizei n, const GLhalfNV *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBS3HVNVPROC) (GLuint index, GLsizei n, const GLhalfNV *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBS4HVNVPROC) (GLuint index, GLsizei n, const GLhalfNV *v);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI void APIENTRY glVertex2hNV (GLhalfNV x, GLhalfNV y);
GLAPI void APIENTRY glVertex2hvNV (const GLhalfNV *v);
GLAPI void APIENTRY glVertex3hNV (GLhalfNV x, GLhalfNV y, GLhalfNV z);
GLAPI void APIENTRY glVertex3hvNV (const GLhalfNV *v);
GLAPI void APIENTRY glVertex4hNV (GLhalfNV x, GLhalfNV y, GLhalfNV z, GLhalfNV w);
GLAPI void APIENTRY glVertex4hvNV (const GLhalfNV *v);
GLAPI void APIENTRY glNormal3hNV (GLhalfNV nx, GLhalfNV ny, GLhalfNV nz);
GLAPI void APIENTRY glNormal3hvNV (const GLhalfNV *v);
GLAPI void APIENTRY glColor3hNV (GLhalfNV red, GLhalfNV green, GLhalfNV blue);
GLAPI void APIENTRY glColor3hvNV (const GLhalfNV *v);
GLAPI void APIENTRY glColor4hNV (GLhalfNV red, GLhalfNV green, GLhalfNV blue, GLhalfNV alpha);
GLAPI void APIENTRY glColor4hvNV (const GLhalfNV *v);
GLAPI void APIENTRY glTexCoord1hNV (GLhalfNV s);
GLAPI void APIENTRY glTexCoord1hvNV (const GLhalfNV *v);
GLAPI void APIENTRY glTexCoord2hNV (GLhalfNV s, GLhalfNV t);
GLAPI void APIENTRY glTexCoord2hvNV (const GLhalfNV *v);
GLAPI void APIENTRY glTexCoord3hNV (GLhalfNV s, GLhalfNV t, GLhalfNV r);
GLAPI void APIENTRY glTexCoord3hvNV (const GLhalfNV *v);
GLAPI void APIENTRY glTexCoord4hNV (GLhalfNV s, GLhalfNV t, GLhalfNV r, GLhalfNV q);
GLAPI void APIENTRY glTexCoord4hvNV (const GLhalfNV *v);
GLAPI void APIENTRY glMultiTexCoord1hNV (GLenum target, GLhalfNV s);
GLAPI void APIENTRY glMultiTexCoord1hvNV (GLenum target, const GLhalfNV *v);
GLAPI void APIENTRY glMultiTexCoord2hNV (GLenum target, GLhalfNV s, GLhalfNV t);
GLAPI void APIENTRY glMultiTexCoord2hvNV (GLenum target, const GLhalfNV *v);
GLAPI void APIENTRY glMultiTexCoord3hNV (GLenum target, GLhalfNV s, GLhalfNV t, GLhalfNV r);
GLAPI void APIENTRY glMultiTexCoord3hvNV (GLenum target, const GLhalfNV *v);
GLAPI void APIENTRY glMultiTexCoord4hNV (GLenum target, GLhalfNV s, GLhalfNV t, GLhalfNV r, GLhalfNV q);
GLAPI void APIENTRY glMultiTexCoord4hvNV (GLenum target, const GLhalfNV *v);
GLAPI void APIENTRY glFogCoordhNV (GLhalfNV fog);
GLAPI void APIENTRY glFogCoordhvNV (const GLhalfNV *fog);
GLAPI void APIENTRY glSecondaryColor3hNV (GLhalfNV red, GLhalfNV green, GLhalfNV blue);
GLAPI void APIENTRY glSecondaryColor3hvNV (const GLhalfNV *v);
GLAPI void APIENTRY glVertexWeighthNV (GLhalfNV weight);
GLAPI void APIENTRY glVertexWeighthvNV (const GLhalfNV *weight);
GLAPI void APIENTRY glVertexAttrib1hNV (GLuint index, GLhalfNV x);
GLAPI void APIENTRY glVertexAttrib1hvNV (GLuint index, const GLhalfNV *v);
GLAPI void APIENTRY glVertexAttrib2hNV (GLuint index, GLhalfNV x, GLhalfNV y);
GLAPI void APIENTRY glVertexAttrib2hvNV (GLuint index, const GLhalfNV *v);
GLAPI void APIENTRY glVertexAttrib3hNV (GLuint index, GLhalfNV x, GLhalfNV y, GLhalfNV z);
GLAPI void APIENTRY glVertexAttrib3hvNV (GLuint index, const GLhalfNV *v);
GLAPI void APIENTRY glVertexAttrib4hNV (GLuint index, GLhalfNV x, GLhalfNV y, GLhalfNV z, GLhalfNV w);
GLAPI void APIENTRY glVertexAttrib4hvNV (GLuint index, const GLhalfNV *v);
GLAPI void APIENTRY glVertexAttribs1hvNV (GLuint index, GLsizei n, const GLhalfNV *v);
GLAPI void APIENTRY glVertexAttribs2hvNV (GLuint index, GLsizei n, const GLhalfNV *v);
GLAPI void APIENTRY glVertexAttribs3hvNV (GLuint index, GLsizei n, const GLhalfNV *v);
GLAPI void APIENTRY glVertexAttribs4hvNV (GLuint index, GLsizei n, const GLhalfNV *v);
#endif
#endif /* GL_NV_half_float */

#ifndef GL_NV_light_max_exponent
#define GL_NV_light_max_exponent 1
#define GL_MAX_SHININESS_NV               0x8504
#define GL_MAX_SPOT_EXPONENT_NV           0x8505
#endif /* GL_NV_light_max_exponent */

#ifndef GL_NV_multisample_coverage
#define GL_NV_multisample_coverage 1
#define GL_COLOR_SAMPLES_NV               0x8E20
#endif /* GL_NV_multisample_coverage */

#ifndef GL_NV_multisample_filter_hint
#define GL_NV_multisample_filter_hint 1
#define GL_MULTISAMPLE_FILTER_HINT_NV     0x8534
#endif /* GL_NV_multisample_filter_hint */

#ifndef GL_NV_occlusion_query
#define GL_NV_occlusion_query 1
#define GL_PIXEL_COUNTER_BITS_NV          0x8864
#define GL_CURRENT_OCCLUSION_QUERY_ID_NV  0x8865
#define GL_PIXEL_COUNT_NV                 0x8866
#define GL_PIXEL_COUNT_AVAILABLE_NV       0x8867
typedef void (APIENTRYP PFNGLGENOCCLUSIONQUERIESNVPROC) (GLsizei n, GLuint *ids);
typedef void (APIENTRYP PFNGLDELETEOCCLUSIONQUERIESNVPROC) (GLsizei n, const GLuint *ids);
typedef GLboolean (APIENTRYP PFNGLISOCCLUSIONQUERYNVPROC) (GLuint id);
typedef void (APIENTRYP PFNGLBEGINOCCLUSIONQUERYNVPROC) (GLuint id);
typedef void (APIENTRYP PFNGLENDOCCLUSIONQUERYNVPROC) (void);
typedef void (APIENTRYP PFNGLGETOCCLUSIONQUERYIVNVPROC) (GLuint id, GLenum pname, GLint *params);
typedef void (APIENTRYP PFNGLGETOCCLUSIONQUERYUIVNVPROC) (GLuint id, GLenum pname, GLuint *params);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI void APIENTRY glGenOcclusionQueriesNV (GLsizei n, GLuint *ids);
GLAPI void APIENTRY glDeleteOcclusionQueriesNV (GLsizei n, const GLuint *ids);
GLAPI GLboolean APIENTRY glIsOcclusionQueryNV (GLuint id);
GLAPI void APIENTRY glBeginOcclusionQueryNV (GLuint id);
GLAPI void APIENTRY glEndOcclusionQueryNV (void);
GLAPI void APIENTRY glGetOcclusionQueryivNV (GLuint id, GLenum pname, GLint *params);
GLAPI void APIENTRY glGetOcclusionQueryuivNV (GLuint id, GLenum pname, GLuint *params);
#endif
#endif /* GL_NV_occlusion_query */

#ifndef GL_NV_packed_depth_stencil
#define GL_NV_packed_depth_stencil 1
#define GL_DEPTH_STENCIL_NV               0x84F9
#define GL_UNSIGNED_INT_24_8_NV           0x84FA
#endif /* GL_NV_packed_depth_stencil */

#ifndef GL_NV_parameter_buffer_object
#define GL_NV_parameter_buffer_object 1
#define GL_MAX_PROGRAM_PARAMETER_BUFFER_BINDINGS_NV 0x8DA0
#define GL_MAX_PROGRAM_PARAMETER_BUFFER_SIZE_NV 0x8DA1
#define GL_VERTEX_PROGRAM_PARAMETER_BUFFER_NV 0x8DA2
#define GL_GEOMETRY_PROGRAM_PARAMETER_BUFFER_NV 0x8DA3
#define GL_FRAGMENT_PROGRAM_PARAMETER_BUFFER_NV 0x8DA4
typedef void (APIENTRYP PFNGLPROGRAMBUFFERPARAMETERSFVNVPROC) (GLenum target, GLuint bindingIndex, GLuint wordIndex, GLsizei count, const GLfloat *params);
typedef void (APIENTRYP PFNGLPROGRAMBUFFERPARAMETERSIIVNVPROC) (GLenum target, GLuint bindingIndex, GLuint wordIndex, GLsizei count, const GLint *params);
typedef void (APIENTRYP PFNGLPROGRAMBUFFERPARAMETERSIUIVNVPROC) (GLenum target, GLuint bindingIndex, GLuint wordIndex, GLsizei count, const GLuint *params);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI void APIENTRY glProgramBufferParametersfvNV (GLenum target, GLuint bindingIndex, GLuint wordIndex, GLsizei count, const GLfloat *params);
GLAPI void APIENTRY glProgramBufferParametersIivNV (GLenum target, GLuint bindingIndex, GLuint wordIndex, GLsizei count, const GLint *params);
GLAPI void APIENTRY glProgramBufferParametersIuivNV (GLenum target, GLuint bindingIndex, GLuint wordIndex, GLsizei count, const GLuint *params);
#endif
#endif /* GL_NV_parameter_buffer_object */

#ifndef GL_NV_parameter_buffer_object2
#define GL_NV_parameter_buffer_object2 1
#endif /* GL_NV_parameter_buffer_object2 */

#ifndef GL_NV_path_rendering
#define GL_NV_path_rendering 1
#define GL_PATH_FORMAT_SVG_NV             0x9070
#define GL_PATH_FORMAT_PS_NV              0x9071
#define GL_STANDARD_FONT_NAME_NV          0x9072
#define GL_SYSTEM_FONT_NAME_NV            0x9073
#define GL_FILE_NAME_NV                   0x9074
#define GL_PATH_STROKE_WIDTH_NV           0x9075
#define GL_PATH_END_CAPS_NV               0x9076
#define GL_PATH_INITIAL_END_CAP_NV        0x9077
#define GL_PATH_TERMINAL_END_CAP_NV       0x9078
#define GL_PATH_JOIN_STYLE_NV             0x9079
#define GL_PATH_MITER_LIMIT_NV            0x907A
#define GL_PATH_DASH_CAPS_NV              0x907B
#define GL_PATH_INITIAL_DASH_CAP_NV       0x907C
#define GL_PATH_TERMINAL_DASH_CAP_NV      0x907D
#define GL_PATH_DASH_OFFSET_NV            0x907E
#define GL_PATH_CLIENT_LENGTH_NV          0x907F
#define GL_PATH_FILL_MODE_NV              0x9080
#define GL_PATH_FILL_MASK_NV              0x9081
#define GL_PATH_FILL_COVER_MODE_NV        0x9082
#define GL_PATH_STROKE_COVER_MODE_NV      0x9083
#define GL_PATH_STROKE_MASK_NV            0x9084
#define GL_COUNT_UP_NV                    0x9088
#define GL_COUNT_DOWN_NV                  0x9089
#define GL_PATH_OBJECT_BOUNDING_BOX_NV    0x908A
#define GL_CONVEX_HULL_NV                 0x908B
#define GL_BOUNDING_BOX_NV                0x908D
#define GL_TRANSLATE_X_NV                 0x908E
#define GL_TRANSLATE_Y_NV                 0x908F
#define GL_TRANSLATE_2D_NV                0x9090
#define GL_TRANSLATE_3D_NV                0x9091
#define GL_AFFINE_2D_NV                   0x9092
#define GL_AFFINE_3D_NV                   0x9094
#define GL_TRANSPOSE_AFFINE_2D_NV         0x9096
#define GL_TRANSPOSE_AFFINE_3D_NV         0x9098
#define GL_UTF8_NV                        0x909A
#define GL_UTF16_NV                       0x909B
#define GL_BOUNDING_BOX_OF_BOUNDING_BOXES_NV 0x909C
#define GL_PATH_COMMAND_COUNT_NV          0x909D
#define GL_PATH_COORD_COUNT_NV            0x909E
#define GL_PATH_DASH_ARRAY_COUNT_NV       0x909F
#define GL_PATH_COMPUTED_LENGTH_NV        0x90A0
#define GL_PATH_FILL_BOUNDING_BOX_NV      0x90A1
#define GL_PATH_STROKE_BOUNDING_BOX_NV    0x90A2
#define GL_SQUARE_NV                      0x90A3
#define GL_ROUND_NV                       0x90A4
#define GL_TRIANGULAR_NV                  0x90A5
#define GL_BEVEL_NV                       0x90A6
#define GL_MITER_REVERT_NV                0x90A7
#define GL_MITER_TRUNCATE_NV              0x90A8
#define GL_SKIP_MISSING_GLYPH_NV          0x90A9
#define GL_USE_MISSING_GLYPH_NV           0x90AA
#define GL_PATH_ERROR_POSITION_NV         0x90AB
#define GL_PATH_FOG_GEN_MODE_NV           0x90AC
#define GL_ACCUM_ADJACENT_PAIRS_NV        0x90AD
#define GL_ADJACENT_PAIRS_NV              0x90AE
#define GL_FIRST_TO_REST_NV               0x90AF
#define GL_PATH_GEN_MODE_NV               0x90B0
#define GL_PATH_GEN_COEFF_NV              0x90B1
#define GL_PATH_GEN_COLOR_FORMAT_NV       0x90B2
#define GL_PATH_GEN_COMPONENTS_NV         0x90B3
#define GL_PATH_STENCIL_FUNC_NV           0x90B7
#define GL_PATH_STENCIL_REF_NV            0x90B8
#define GL_PATH_STENCIL_VALUE_MASK_NV     0x90B9
#define GL_PATH_STENCIL_DEPTH_OFFSET_FACTOR_NV 0x90BD
#define GL_PATH_STENCIL_DEPTH_OFFSET_UNITS_NV 0x90BE
#define GL_PATH_COVER_DEPTH_FUNC_NV       0x90BF
#define GL_PATH_DASH_OFFSET_RESET_NV      0x90B4
#define GL_MOVE_TO_RESETS_NV              0x90B5
#define GL_MOVE_TO_CONTINUES_NV           0x90B6
#define GL_CLOSE_PATH_NV                  0x00
#define GL_MOVE_TO_NV                     0x02
#define GL_RELATIVE_MOVE_TO_NV            0x03
#define GL_LINE_TO_NV                     0x04
#define GL_RELATIVE_LINE_TO_NV            0x05
#define GL_HORIZONTAL_LINE_TO_NV          0x06
#define GL_RELATIVE_HORIZONTAL_LINE_TO_NV 0x07
#define GL_VERTICAL_LINE_TO_NV            0x08
#define GL_RELATIVE_VERTICAL_LINE_TO_NV   0x09
#define GL_QUADRATIC_CURVE_TO_NV          0x0A
#define GL_RELATIVE_QUADRATIC_CURVE_TO_NV 0x0B
#define GL_CUBIC_CURVE_TO_NV              0x0C
#define GL_RELATIVE_CUBIC_CURVE_TO_NV     0x0D
#define GL_SMOOTH_QUADRATIC_CURVE_TO_NV   0x0E
#define GL_RELATIVE_SMOOTH_QUADRATIC_CURVE_TO_NV 0x0F
#define GL_SMOOTH_CUBIC_CURVE_TO_NV       0x10
#define GL_RELATIVE_SMOOTH_CUBIC_CURVE_TO_NV 0x11
#define GL_SMALL_CCW_ARC_TO_NV            0x12
#define GL_RELATIVE_SMALL_CCW_ARC_TO_NV   0x13
#define GL_SMALL_CW_ARC_TO_NV             0x14
#define GL_RELATIVE_SMALL_CW_ARC_TO_NV    0x15
#define GL_LARGE_CCW_ARC_TO_NV            0x16
#define GL_RELATIVE_LARGE_CCW_ARC_TO_NV   0x17
#define GL_LARGE_CW_ARC_TO_NV             0x18
#define GL_RELATIVE_LARGE_CW_ARC_TO_NV    0x19
#define GL_RESTART_PATH_NV                0xF0
#define GL_DUP_FIRST_CUBIC_CURVE_TO_NV    0xF2
#define GL_DUP_LAST_CUBIC_CURVE_TO_NV     0xF4
#define GL_RECT_NV                        0xF6
#define GL_CIRCULAR_CCW_ARC_TO_NV         0xF8
#define GL_CIRCULAR_CW_ARC_TO_NV          0xFA
#define GL_CIRCULAR_TANGENT_ARC_TO_NV     0xFC
#define GL_ARC_TO_NV                      0xFE
#define GL_RELATIVE_ARC_TO_NV             0xFF
#define GL_BOLD_BIT_NV                    0x01
#define GL_ITALIC_BIT_NV                  0x02
#define GL_GLYPH_WIDTH_BIT_NV             0x01
#define GL_GLYPH_HEIGHT_BIT_NV            0x02
#define GL_GLYPH_HORIZONTAL_BEARING_X_BIT_NV 0x04
#define GL_GLYPH_HORIZONTAL_BEARING_Y_BIT_NV 0x08
#define GL_GLYPH_HORIZONTAL_BEARING_ADVANCE_BIT_NV 0x10
#define GL_GLYPH_VERTICAL_BEARING_X_BIT_NV 0x20
#define GL_GLYPH_VERTICAL_BEARING_Y_BIT_NV 0x40
#define GL_GLYPH_VERTICAL_BEARING_ADVANCE_BIT_NV 0x80
#define GL_GLYPH_HAS_KERNING_BIT_NV       0x100
#define GL_FONT_X_MIN_BOUNDS_BIT_NV       0x00010000
#define GL_FONT_Y_MIN_BOUNDS_BIT_NV       0x00020000
#define GL_FONT_X_MAX_BOUNDS_BIT_NV       0x00040000
#define GL_FONT_Y_MAX_BOUNDS_BIT_NV       0x00080000
#define GL_FONT_UNITS_PER_EM_BIT_NV       0x00100000
#define GL_FONT_ASCENDER_BIT_NV           0x00200000
#define GL_FONT_DESCENDER_BIT_NV          0x00400000
#define GL_FONT_HEIGHT_BIT_NV             0x00800000
#define GL_FONT_MAX_ADVANCE_WIDTH_BIT_NV  0x01000000
#define GL_FONT_MAX_ADVANCE_HEIGHT_BIT_NV 0x02000000
#define GL_FONT_UNDERLINE_POSITION_BIT_NV 0x04000000
#define GL_FONT_UNDERLINE_THICKNESS_BIT_NV 0x08000000
#define GL_FONT_HAS_KERNING_BIT_NV        0x10000000
#define GL_PRIMARY_COLOR_NV               0x852C
#define GL_SECONDARY_COLOR_NV             0x852D
typedef GLuint (APIENTRYP PFNGLGENPATHSNVPROC) (GLsizei range);
typedef void (APIENTRYP PFNGLDELETEPATHSNVPROC) (GLuint path, GLsizei range);
typedef GLboolean (APIENTRYP PFNGLISPATHNVPROC) (GLuint path);
typedef void (APIENTRYP PFNGLPATHCOMMANDSNVPROC) (GLuint path, GLsizei numCommands, const GLubyte *commands, GLsizei numCoords, GLenum coordType, const GLvoid *coords);
typedef void (APIENTRYP PFNGLPATHCOORDSNVPROC) (GLuint path, GLsizei numCoords, GLenum coordType, const GLvoid *coords);
typedef void (APIENTRYP PFNGLPATHSUBCOMMANDSNVPROC) (GLuint path, GLsizei commandStart, GLsizei commandsToDelete, GLsizei numCommands, const GLubyte *commands, GLsizei numCoords, GLenum coordType, const GLvoid *coords);
typedef void (APIENTRYP PFNGLPATHSUBCOORDSNVPROC) (GLuint path, GLsizei coordStart, GLsizei numCoords, GLenum coordType, const GLvoid *coords);
typedef void (APIENTRYP PFNGLPATHSTRINGNVPROC) (GLuint path, GLenum format, GLsizei length, const GLvoid *pathString);
typedef void (APIENTRYP PFNGLPATHGLYPHSNVPROC) (GLuint firstPathName, GLenum fontTarget, const GLvoid *fontName, GLbitfield fontStyle, GLsizei numGlyphs, GLenum type, const GLvoid *charcodes, GLenum handleMissingGlyphs, GLuint pathParameterTemplate, GLfloat emScale);
typedef void (APIENTRYP PFNGLPATHGLYPHRANGENVPROC) (GLuint firstPathName, GLenum fontTarget, const GLvoid *fontName, GLbitfield fontStyle, GLuint firstGlyph, GLsizei numGlyphs, GLenum handleMissingGlyphs, GLuint pathParameterTemplate, GLfloat emScale);
typedef void (APIENTRYP PFNGLWEIGHTPATHSNVPROC) (GLuint resultPath, GLsizei numPaths, const GLuint *paths, const GLfloat *weights);
typedef void (APIENTRYP PFNGLCOPYPATHNVPROC) (GLuint resultPath, GLuint srcPath);
typedef void (APIENTRYP PFNGLINTERPOLATEPATHSNVPROC) (GLuint resultPath, GLuint pathA, GLuint pathB, GLfloat weight);
typedef void (APIENTRYP PFNGLTRANSFORMPATHNVPROC) (GLuint resultPath, GLuint srcPath, GLenum transformType, const GLfloat *transformValues);
typedef void (APIENTRYP PFNGLPATHPARAMETERIVNVPROC) (GLuint path, GLenum pname, const GLint *value);
typedef void (APIENTRYP PFNGLPATHPARAMETERINVPROC) (GLuint path, GLenum pname, GLint value);
typedef void (APIENTRYP PFNGLPATHPARAMETERFVNVPROC) (GLuint path, GLenum pname, const GLfloat *value);
typedef void (APIENTRYP PFNGLPATHPARAMETERFNVPROC) (GLuint path, GLenum pname, GLfloat value);
typedef void (APIENTRYP PFNGLPATHDASHARRAYNVPROC) (GLuint path, GLsizei dashCount, const GLfloat *dashArray);
typedef void (APIENTRYP PFNGLPATHSTENCILFUNCNVPROC) (GLenum func, GLint ref, GLuint mask);
typedef void (APIENTRYP PFNGLPATHSTENCILDEPTHOFFSETNVPROC) (GLfloat factor, GLfloat units);
typedef void (APIENTRYP PFNGLSTENCILFILLPATHNVPROC) (GLuint path, GLenum fillMode, GLuint mask);
typedef void (APIENTRYP PFNGLSTENCILSTROKEPATHNVPROC) (GLuint path, GLint reference, GLuint mask);
typedef void (APIENTRYP PFNGLSTENCILFILLPATHINSTANCEDNVPROC) (GLsizei numPaths, GLenum pathNameType, const GLvoid *paths, GLuint pathBase, GLenum fillMode, GLuint mask, GLenum transformType, const GLfloat *transformValues);
typedef void (APIENTRYP PFNGLSTENCILSTROKEPATHINSTANCEDNVPROC) (GLsizei numPaths, GLenum pathNameType, const GLvoid *paths, GLuint pathBase, GLint reference, GLuint mask, GLenum transformType, const GLfloat *transformValues);
typedef void (APIENTRYP PFNGLPATHCOVERDEPTHFUNCNVPROC) (GLenum func);
typedef void (APIENTRYP PFNGLPATHCOLORGENNVPROC) (GLenum color, GLenum genMode, GLenum colorFormat, const GLfloat *coeffs);
typedef void (APIENTRYP PFNGLPATHTEXGENNVPROC) (GLenum texCoordSet, GLenum genMode, GLint components, const GLfloat *coeffs);
typedef void (APIENTRYP PFNGLPATHFOGGENNVPROC) (GLenum genMode);
typedef void (APIENTRYP PFNGLCOVERFILLPATHNVPROC) (GLuint path, GLenum coverMode);
typedef void (APIENTRYP PFNGLCOVERSTROKEPATHNVPROC) (GLuint path, GLenum coverMode);
typedef void (APIENTRYP PFNGLCOVERFILLPATHINSTANCEDNVPROC) (GLsizei numPaths, GLenum pathNameType, const GLvoid *paths, GLuint pathBase, GLenum coverMode, GLenum transformType, const GLfloat *transformValues);
typedef void (APIENTRYP PFNGLCOVERSTROKEPATHINSTANCEDNVPROC) (GLsizei numPaths, GLenum pathNameType, const GLvoid *paths, GLuint pathBase, GLenum coverMode, GLenum transformType, const GLfloat *transformValues);
typedef void (APIENTRYP PFNGLGETPATHPARAMETERIVNVPROC) (GLuint path, GLenum pname, GLint *value);
typedef void (APIENTRYP PFNGLGETPATHPARAMETERFVNVPROC) (GLuint path, GLenum pname, GLfloat *value);
typedef void (APIENTRYP PFNGLGETPATHCOMMANDSNVPROC) (GLuint path, GLubyte *commands);
typedef void (APIENTRYP PFNGLGETPATHCOORDSNVPROC) (GLuint path, GLfloat *coords);
typedef void (APIENTRYP PFNGLGETPATHDASHARRAYNVPROC) (GLuint path, GLfloat *dashArray);
typedef void (APIENTRYP PFNGLGETPATHMETRICSNVPROC) (GLbitfield metricQueryMask, GLsizei numPaths, GLenum pathNameType, const GLvoid *paths, GLuint pathBase, GLsizei stride, GLfloat *metrics);
typedef void (APIENTRYP PFNGLGETPATHMETRICRANGENVPROC) (GLbitfield metricQueryMask, GLuint firstPathName, GLsizei numPaths, GLsizei stride, GLfloat *metrics);
typedef void (APIENTRYP PFNGLGETPATHSPACINGNVPROC) (GLenum pathListMode, GLsizei numPaths, GLenum pathNameType, const GLvoid *paths, GLuint pathBase, GLfloat advanceScale, GLfloat kerningScale, GLenum transformType, GLfloat *returnedSpacing);
typedef void (APIENTRYP PFNGLGETPATHCOLORGENIVNVPROC) (GLenum color, GLenum pname, GLint *value);
typedef void (APIENTRYP PFNGLGETPATHCOLORGENFVNVPROC) (GLenum color, GLenum pname, GLfloat *value);
typedef void (APIENTRYP PFNGLGETPATHTEXGENIVNVPROC) (GLenum texCoordSet, GLenum pname, GLint *value);
typedef void (APIENTRYP PFNGLGETPATHTEXGENFVNVPROC) (GLenum texCoordSet, GLenum pname, GLfloat *value);
typedef GLboolean (APIENTRYP PFNGLISPOINTINFILLPATHNVPROC) (GLuint path, GLuint mask, GLfloat x, GLfloat y);
typedef GLboolean (APIENTRYP PFNGLISPOINTINSTROKEPATHNVPROC) (GLuint path, GLfloat x, GLfloat y);
typedef GLfloat (APIENTRYP PFNGLGETPATHLENGTHNVPROC) (GLuint path, GLsizei startSegment, GLsizei numSegments);
typedef GLboolean (APIENTRYP PFNGLPOINTALONGPATHNVPROC) (GLuint path, GLsizei startSegment, GLsizei numSegments, GLfloat distance, GLfloat *x, GLfloat *y, GLfloat *tangentX, GLfloat *tangentY);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI GLuint APIENTRY glGenPathsNV (GLsizei range);
GLAPI void APIENTRY glDeletePathsNV (GLuint path, GLsizei range);
GLAPI GLboolean APIENTRY glIsPathNV (GLuint path);
GLAPI void APIENTRY glPathCommandsNV (GLuint path, GLsizei numCommands, const GLubyte *commands, GLsizei numCoords, GLenum coordType, const GLvoid *coords);
GLAPI void APIENTRY glPathCoordsNV (GLuint path, GLsizei numCoords, GLenum coordType, const GLvoid *coords);
GLAPI void APIENTRY glPathSubCommandsNV (GLuint path, GLsizei commandStart, GLsizei commandsToDelete, GLsizei numCommands, const GLubyte *commands, GLsizei numCoords, GLenum coordType, const GLvoid *coords);
GLAPI void APIENTRY glPathSubCoordsNV (GLuint path, GLsizei coordStart, GLsizei numCoords, GLenum coordType, const GLvoid *coords);
GLAPI void APIENTRY glPathStringNV (GLuint path, GLenum format, GLsizei length, const GLvoid *pathString);
GLAPI void APIENTRY glPathGlyphsNV (GLuint firstPathName, GLenum fontTarget, const GLvoid *fontName, GLbitfield fontStyle, GLsizei numGlyphs, GLenum type, const GLvoid *charcodes, GLenum handleMissingGlyphs, GLuint pathParameterTemplate, GLfloat emScale);
GLAPI void APIENTRY glPathGlyphRangeNV (GLuint firstPathName, GLenum fontTarget, const GLvoid *fontName, GLbitfield fontStyle, GLuint firstGlyph, GLsizei numGlyphs, GLenum handleMissingGlyphs, GLuint pathParameterTemplate, GLfloat emScale);
GLAPI void APIENTRY glWeightPathsNV (GLuint resultPath, GLsizei numPaths, const GLuint *paths, const GLfloat *weights);
GLAPI void APIENTRY glCopyPathNV (GLuint resultPath, GLuint srcPath);
GLAPI void APIENTRY glInterpolatePathsNV (GLuint resultPath, GLuint pathA, GLuint pathB, GLfloat weight);
GLAPI void APIENTRY glTransformPathNV (GLuint resultPath, GLuint srcPath, GLenum transformType, const GLfloat *transformValues);
GLAPI void APIENTRY glPathParameterivNV (GLuint path, GLenum pname, const GLint *value);
GLAPI void APIENTRY glPathParameteriNV (GLuint path, GLenum pname, GLint value);
GLAPI void APIENTRY glPathParameterfvNV (GLuint path, GLenum pname, const GLfloat *value);
GLAPI void APIENTRY glPathParameterfNV (GLuint path, GLenum pname, GLfloat value);
GLAPI void APIENTRY glPathDashArrayNV (GLuint path, GLsizei dashCount, const GLfloat *dashArray);
GLAPI void APIENTRY glPathStencilFuncNV (GLenum func, GLint ref, GLuint mask);
GLAPI void APIENTRY glPathStencilDepthOffsetNV (GLfloat factor, GLfloat units);
GLAPI void APIENTRY glStencilFillPathNV (GLuint path, GLenum fillMode, GLuint mask);
GLAPI void APIENTRY glStencilStrokePathNV (GLuint path, GLint reference, GLuint mask);
GLAPI void APIENTRY glStencilFillPathInstancedNV (GLsizei numPaths, GLenum pathNameType, const GLvoid *paths, GLuint pathBase, GLenum fillMode, GLuint mask, GLenum transformType, const GLfloat *transformValues);
GLAPI void APIENTRY glStencilStrokePathInstancedNV (GLsizei numPaths, GLenum pathNameType, const GLvoid *paths, GLuint pathBase, GLint reference, GLuint mask, GLenum transformType, const GLfloat *transformValues);
GLAPI void APIENTRY glPathCoverDepthFuncNV (GLenum func);
GLAPI void APIENTRY glPathColorGenNV (GLenum color, GLenum genMode, GLenum colorFormat, const GLfloat *coeffs);
GLAPI void APIENTRY glPathTexGenNV (GLenum texCoordSet, GLenum genMode, GLint components, const GLfloat *coeffs);
GLAPI void APIENTRY glPathFogGenNV (GLenum genMode);
GLAPI void APIENTRY glCoverFillPathNV (GLuint path, GLenum coverMode);
GLAPI void APIENTRY glCoverStrokePathNV (GLuint path, GLenum coverMode);
GLAPI void APIENTRY glCoverFillPathInstancedNV (GLsizei numPaths, GLenum pathNameType, const GLvoid *paths, GLuint pathBase, GLenum coverMode, GLenum transformType, const GLfloat *transformValues);
GLAPI void APIENTRY glCoverStrokePathInstancedNV (GLsizei numPaths, GLenum pathNameType, const GLvoid *paths, GLuint pathBase, GLenum coverMode, GLenum transformType, const GLfloat *transformValues);
GLAPI void APIENTRY glGetPathParameterivNV (GLuint path, GLenum pname, GLint *value);
GLAPI void APIENTRY glGetPathParameterfvNV (GLuint path, GLenum pname, GLfloat *value);
GLAPI void APIENTRY glGetPathCommandsNV (GLuint path, GLubyte *commands);
GLAPI void APIENTRY glGetPathCoordsNV (GLuint path, GLfloat *coords);
GLAPI void APIENTRY glGetPathDashArrayNV (GLuint path, GLfloat *dashArray);
GLAPI void APIENTRY glGetPathMetricsNV (GLbitfield metricQueryMask, GLsizei numPaths, GLenum pathNameType, const GLvoid *paths, GLuint pathBase, GLsizei stride, GLfloat *metrics);
GLAPI void APIENTRY glGetPathMetricRangeNV (GLbitfield metricQueryMask, GLuint firstPathName, GLsizei numPaths, GLsizei stride, GLfloat *metrics);
GLAPI void APIENTRY glGetPathSpacingNV (GLenum pathListMode, GLsizei numPaths, GLenum pathNameType, const GLvoid *paths, GLuint pathBase, GLfloat advanceScale, GLfloat kerningScale, GLenum transformType, GLfloat *returnedSpacing);
GLAPI void APIENTRY glGetPathColorGenivNV (GLenum color, GLenum pname, GLint *value);
GLAPI void APIENTRY glGetPathColorGenfvNV (GLenum color, GLenum pname, GLfloat *value);
GLAPI void APIENTRY glGetPathTexGenivNV (GLenum texCoordSet, GLenum pname, GLint *value);
GLAPI void APIENTRY glGetPathTexGenfvNV (GLenum texCoordSet, GLenum pname, GLfloat *value);
GLAPI GLboolean APIENTRY glIsPointInFillPathNV (GLuint path, GLuint mask, GLfloat x, GLfloat y);
GLAPI GLboolean APIENTRY glIsPointInStrokePathNV (GLuint path, GLfloat x, GLfloat y);
GLAPI GLfloat APIENTRY glGetPathLengthNV (GLuint path, GLsizei startSegment, GLsizei numSegments);
GLAPI GLboolean APIENTRY glPointAlongPathNV (GLuint path, GLsizei startSegment, GLsizei numSegments, GLfloat distance, GLfloat *x, GLfloat *y, GLfloat *tangentX, GLfloat *tangentY);
#endif
#endif /* GL_NV_path_rendering */

#ifndef GL_NV_pixel_data_range
#define GL_NV_pixel_data_range 1
#define GL_WRITE_PIXEL_DATA_RANGE_NV      0x8878
#define GL_READ_PIXEL_DATA_RANGE_NV       0x8879
#define GL_WRITE_PIXEL_DATA_RANGE_LENGTH_NV 0x887A
#define GL_READ_PIXEL_DATA_RANGE_LENGTH_NV 0x887B
#define GL_WRITE_PIXEL_DATA_RANGE_POINTER_NV 0x887C
#define GL_READ_PIXEL_DATA_RANGE_POINTER_NV 0x887D
typedef void (APIENTRYP PFNGLPIXELDATARANGENVPROC) (GLenum target, GLsizei length, const GLvoid *pointer);
typedef void (APIENTRYP PFNGLFLUSHPIXELDATARANGENVPROC) (GLenum target);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI void APIENTRY glPixelDataRangeNV (GLenum target, GLsizei length, const GLvoid *pointer);
GLAPI void APIENTRY glFlushPixelDataRangeNV (GLenum target);
#endif
#endif /* GL_NV_pixel_data_range */

#ifndef GL_NV_point_sprite
#define GL_NV_point_sprite 1
#define GL_POINT_SPRITE_NV                0x8861
#define GL_COORD_REPLACE_NV               0x8862
#define GL_POINT_SPRITE_R_MODE_NV         0x8863
typedef void (APIENTRYP PFNGLPOINTPARAMETERINVPROC) (GLenum pname, GLint param);
typedef void (APIENTRYP PFNGLPOINTPARAMETERIVNVPROC) (GLenum pname, const GLint *params);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI void APIENTRY glPointParameteriNV (GLenum pname, GLint param);
GLAPI void APIENTRY glPointParameterivNV (GLenum pname, const GLint *params);
#endif
#endif /* GL_NV_point_sprite */

#ifndef GL_NV_present_video
#define GL_NV_present_video 1
#define GL_FRAME_NV                       0x8E26
#define GL_FIELDS_NV                      0x8E27
#define GL_CURRENT_TIME_NV                0x8E28
#define GL_NUM_FILL_STREAMS_NV            0x8E29
#define GL_PRESENT_TIME_NV                0x8E2A
#define GL_PRESENT_DURATION_NV            0x8E2B
typedef void (APIENTRYP PFNGLPRESENTFRAMEKEYEDNVPROC) (GLuint video_slot, GLuint64EXT minPresentTime, GLuint beginPresentTimeId, GLuint presentDurationId, GLenum type, GLenum target0, GLuint fill0, GLuint key0, GLenum target1, GLuint fill1, GLuint key1);
typedef void (APIENTRYP PFNGLPRESENTFRAMEDUALFILLNVPROC) (GLuint video_slot, GLuint64EXT minPresentTime, GLuint beginPresentTimeId, GLuint presentDurationId, GLenum type, GLenum target0, GLuint fill0, GLenum target1, GLuint fill1, GLenum target2, GLuint fill2, GLenum target3, GLuint fill3);
typedef void (APIENTRYP PFNGLGETVIDEOIVNVPROC) (GLuint video_slot, GLenum pname, GLint *params);
typedef void (APIENTRYP PFNGLGETVIDEOUIVNVPROC) (GLuint video_slot, GLenum pname, GLuint *params);
typedef void (APIENTRYP PFNGLGETVIDEOI64VNVPROC) (GLuint video_slot, GLenum pname, GLint64EXT *params);
typedef void (APIENTRYP PFNGLGETVIDEOUI64VNVPROC) (GLuint video_slot, GLenum pname, GLuint64EXT *params);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI void APIENTRY glPresentFrameKeyedNV (GLuint video_slot, GLuint64EXT minPresentTime, GLuint beginPresentTimeId, GLuint presentDurationId, GLenum type, GLenum target0, GLuint fill0, GLuint key0, GLenum target1, GLuint fill1, GLuint key1);
GLAPI void APIENTRY glPresentFrameDualFillNV (GLuint video_slot, GLuint64EXT minPresentTime, GLuint beginPresentTimeId, GLuint presentDurationId, GLenum type, GLenum target0, GLuint fill0, GLenum target1, GLuint fill1, GLenum target2, GLuint fill2, GLenum target3, GLuint fill3);
GLAPI void APIENTRY glGetVideoivNV (GLuint video_slot, GLenum pname, GLint *params);
GLAPI void APIENTRY glGetVideouivNV (GLuint video_slot, GLenum pname, GLuint *params);
GLAPI void APIENTRY glGetVideoi64vNV (GLuint video_slot, GLenum pname, GLint64EXT *params);
GLAPI void APIENTRY glGetVideoui64vNV (GLuint video_slot, GLenum pname, GLuint64EXT *params);
#endif
#endif /* GL_NV_present_video */

#ifndef GL_NV_primitive_restart
#define GL_NV_primitive_restart 1
#define GL_PRIMITIVE_RESTART_NV           0x8558
#define GL_PRIMITIVE_RESTART_INDEX_NV     0x8559
typedef void (APIENTRYP PFNGLPRIMITIVERESTARTNVPROC) (void);
typedef void (APIENTRYP PFNGLPRIMITIVERESTARTINDEXNVPROC) (GLuint index);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI void APIENTRY glPrimitiveRestartNV (void);
GLAPI void APIENTRY glPrimitiveRestartIndexNV (GLuint index);
#endif
#endif /* GL_NV_primitive_restart */

#ifndef GL_NV_register_combiners
#define GL_NV_register_combiners 1
#define GL_REGISTER_COMBINERS_NV          0x8522
#define GL_VARIABLE_A_NV                  0x8523
#define GL_VARIABLE_B_NV                  0x8524
#define GL_VARIABLE_C_NV                  0x8525
#define GL_VARIABLE_D_NV                  0x8526
#define GL_VARIABLE_E_NV                  0x8527
#define GL_VARIABLE_F_NV                  0x8528
#define GL_VARIABLE_G_NV                  0x8529
#define GL_CONSTANT_COLOR0_NV             0x852A
#define GL_CONSTANT_COLOR1_NV             0x852B
#define GL_SPARE0_NV                      0x852E
#define GL_SPARE1_NV                      0x852F
#define GL_DISCARD_NV                     0x8530
#define GL_E_TIMES_F_NV                   0x8531
#define GL_SPARE0_PLUS_SECONDARY_COLOR_NV 0x8532
#define GL_UNSIGNED_IDENTITY_NV           0x8536
#define GL_UNSIGNED_INVERT_NV             0x8537
#define GL_EXPAND_NORMAL_NV               0x8538
#define GL_EXPAND_NEGATE_NV               0x8539
#define GL_HALF_BIAS_NORMAL_NV            0x853A
#define GL_HALF_BIAS_NEGATE_NV            0x853B
#define GL_SIGNED_IDENTITY_NV             0x853C
#define GL_SIGNED_NEGATE_NV               0x853D
#define GL_SCALE_BY_TWO_NV                0x853E
#define GL_SCALE_BY_FOUR_NV               0x853F
#define GL_SCALE_BY_ONE_HALF_NV           0x8540
#define GL_BIAS_BY_NEGATIVE_ONE_HALF_NV   0x8541
#define GL_COMBINER_INPUT_NV              0x8542
#define GL_COMBINER_MAPPING_NV            0x8543
#define GL_COMBINER_COMPONENT_USAGE_NV    0x8544
#define GL_COMBINER_AB_DOT_PRODUCT_NV     0x8545
#define GL_COMBINER_CD_DOT_PRODUCT_NV     0x8546
#define GL_COMBINER_MUX_SUM_NV            0x8547
#define GL_COMBINER_SCALE_NV              0x8548
#define GL_COMBINER_BIAS_NV               0x8549
#define GL_COMBINER_AB_OUTPUT_NV          0x854A
#define GL_COMBINER_CD_OUTPUT_NV          0x854B
#define GL_COMBINER_SUM_OUTPUT_NV         0x854C
#define GL_MAX_GENERAL_COMBINERS_NV       0x854D
#define GL_NUM_GENERAL_COMBINERS_NV       0x854E
#define GL_COLOR_SUM_CLAMP_NV             0x854F
#define GL_COMBINER0_NV                   0x8550
#define GL_COMBINER1_NV                   0x8551
#define GL_COMBINER2_NV                   0x8552
#define GL_COMBINER3_NV                   0x8553
#define GL_COMBINER4_NV                   0x8554
#define GL_COMBINER5_NV                   0x8555
#define GL_COMBINER6_NV                   0x8556
#define GL_COMBINER7_NV                   0x8557
typedef void (APIENTRYP PFNGLCOMBINERPARAMETERFVNVPROC) (GLenum pname, const GLfloat *params);
typedef void (APIENTRYP PFNGLCOMBINERPARAMETERFNVPROC) (GLenum pname, GLfloat param);
typedef void (APIENTRYP PFNGLCOMBINERPARAMETERIVNVPROC) (GLenum pname, const GLint *params);
typedef void (APIENTRYP PFNGLCOMBINERPARAMETERINVPROC) (GLenum pname, GLint param);
typedef void (APIENTRYP PFNGLCOMBINERINPUTNVPROC) (GLenum stage, GLenum portion, GLenum variable, GLenum input, GLenum mapping, GLenum componentUsage);
typedef void (APIENTRYP PFNGLCOMBINEROUTPUTNVPROC) (GLenum stage, GLenum portion, GLenum abOutput, GLenum cdOutput, GLenum sumOutput, GLenum scale, GLenum bias, GLboolean abDotProduct, GLboolean cdDotProduct, GLboolean muxSum);
typedef void (APIENTRYP PFNGLFINALCOMBINERINPUTNVPROC) (GLenum variable, GLenum input, GLenum mapping, GLenum componentUsage);
typedef void (APIENTRYP PFNGLGETCOMBINERINPUTPARAMETERFVNVPROC) (GLenum stage, GLenum portion, GLenum variable, GLenum pname, GLfloat *params);
typedef void (APIENTRYP PFNGLGETCOMBINERINPUTPARAMETERIVNVPROC) (GLenum stage, GLenum portion, GLenum variable, GLenum pname, GLint *params);
typedef void (APIENTRYP PFNGLGETCOMBINEROUTPUTPARAMETERFVNVPROC) (GLenum stage, GLenum portion, GLenum pname, GLfloat *params);
typedef void (APIENTRYP PFNGLGETCOMBINEROUTPUTPARAMETERIVNVPROC) (GLenum stage, GLenum portion, GLenum pname, GLint *params);
typedef void (APIENTRYP PFNGLGETFINALCOMBINERINPUTPARAMETERFVNVPROC) (GLenum variable, GLenum pname, GLfloat *params);
typedef void (APIENTRYP PFNGLGETFINALCOMBINERINPUTPARAMETERIVNVPROC) (GLenum variable, GLenum pname, GLint *params);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI void APIENTRY glCombinerParameterfvNV (GLenum pname, const GLfloat *params);
GLAPI void APIENTRY glCombinerParameterfNV (GLenum pname, GLfloat param);
GLAPI void APIENTRY glCombinerParameterivNV (GLenum pname, const GLint *params);
GLAPI void APIENTRY glCombinerParameteriNV (GLenum pname, GLint param);
GLAPI void APIENTRY glCombinerInputNV (GLenum stage, GLenum portion, GLenum variable, GLenum input, GLenum mapping, GLenum componentUsage);
GLAPI void APIENTRY glCombinerOutputNV (GLenum stage, GLenum portion, GLenum abOutput, GLenum cdOutput, GLenum sumOutput, GLenum scale, GLenum bias, GLboolean abDotProduct, GLboolean cdDotProduct, GLboolean muxSum);
GLAPI void APIENTRY glFinalCombinerInputNV (GLenum variable, GLenum input, GLenum mapping, GLenum componentUsage);
GLAPI void APIENTRY glGetCombinerInputParameterfvNV (GLenum stage, GLenum portion, GLenum variable, GLenum pname, GLfloat *params);
GLAPI void APIENTRY glGetCombinerInputParameterivNV (GLenum stage, GLenum portion, GLenum variable, GLenum pname, GLint *params);
GLAPI void APIENTRY glGetCombinerOutputParameterfvNV (GLenum stage, GLenum portion, GLenum pname, GLfloat *params);
GLAPI void APIENTRY glGetCombinerOutputParameterivNV (GLenum stage, GLenum portion, GLenum pname, GLint *params);
GLAPI void APIENTRY glGetFinalCombinerInputParameterfvNV (GLenum variable, GLenum pname, GLfloat *params);
GLAPI void APIENTRY glGetFinalCombinerInputParameterivNV (GLenum variable, GLenum pname, GLint *params);
#endif
#endif /* GL_NV_register_combiners */

#ifndef GL_NV_register_combiners2
#define GL_NV_register_combiners2 1
#define GL_PER_STAGE_CONSTANTS_NV         0x8535
typedef void (APIENTRYP PFNGLCOMBINERSTAGEPARAMETERFVNVPROC) (GLenum stage, GLenum pname, const GLfloat *params);
typedef void (APIENTRYP PFNGLGETCOMBINERSTAGEPARAMETERFVNVPROC) (GLenum stage, GLenum pname, GLfloat *params);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI void APIENTRY glCombinerStageParameterfvNV (GLenum stage, GLenum pname, const GLfloat *params);
GLAPI void APIENTRY glGetCombinerStageParameterfvNV (GLenum stage, GLenum pname, GLfloat *params);
#endif
#endif /* GL_NV_register_combiners2 */

#ifndef GL_NV_shader_atomic_counters
#define GL_NV_shader_atomic_counters 1
#endif /* GL_NV_shader_atomic_counters */

#ifndef GL_NV_shader_atomic_float
#define GL_NV_shader_atomic_float 1
#endif /* GL_NV_shader_atomic_float */

#ifndef GL_NV_shader_buffer_load
#define GL_NV_shader_buffer_load 1
#define GL_BUFFER_GPU_ADDRESS_NV          0x8F1D
#define GL_GPU_ADDRESS_NV                 0x8F34
#define GL_MAX_SHADER_BUFFER_ADDRESS_NV   0x8F35
typedef void (APIENTRYP PFNGLMAKEBUFFERRESIDENTNVPROC) (GLenum target, GLenum access);
typedef void (APIENTRYP PFNGLMAKEBUFFERNONRESIDENTNVPROC) (GLenum target);
typedef GLboolean (APIENTRYP PFNGLISBUFFERRESIDENTNVPROC) (GLenum target);
typedef void (APIENTRYP PFNGLMAKENAMEDBUFFERRESIDENTNVPROC) (GLuint buffer, GLenum access);
typedef void (APIENTRYP PFNGLMAKENAMEDBUFFERNONRESIDENTNVPROC) (GLuint buffer);
typedef GLboolean (APIENTRYP PFNGLISNAMEDBUFFERRESIDENTNVPROC) (GLuint buffer);
typedef void (APIENTRYP PFNGLGETBUFFERPARAMETERUI64VNVPROC) (GLenum target, GLenum pname, GLuint64EXT *params);
typedef void (APIENTRYP PFNGLGETNAMEDBUFFERPARAMETERUI64VNVPROC) (GLuint buffer, GLenum pname, GLuint64EXT *params);
typedef void (APIENTRYP PFNGLGETINTEGERUI64VNVPROC) (GLenum value, GLuint64EXT *result);
typedef void (APIENTRYP PFNGLUNIFORMUI64NVPROC) (GLint location, GLuint64EXT value);
typedef void (APIENTRYP PFNGLUNIFORMUI64VNVPROC) (GLint location, GLsizei count, const GLuint64EXT *value);
typedef void (APIENTRYP PFNGLGETUNIFORMUI64VNVPROC) (GLuint program, GLint location, GLuint64EXT *params);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORMUI64NVPROC) (GLuint program, GLint location, GLuint64EXT value);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORMUI64VNVPROC) (GLuint program, GLint location, GLsizei count, const GLuint64EXT *value);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI void APIENTRY glMakeBufferResidentNV (GLenum target, GLenum access);
GLAPI void APIENTRY glMakeBufferNonResidentNV (GLenum target);
GLAPI GLboolean APIENTRY glIsBufferResidentNV (GLenum target);
GLAPI void APIENTRY glMakeNamedBufferResidentNV (GLuint buffer, GLenum access);
GLAPI void APIENTRY glMakeNamedBufferNonResidentNV (GLuint buffer);
GLAPI GLboolean APIENTRY glIsNamedBufferResidentNV (GLuint buffer);
GLAPI void APIENTRY glGetBufferParameterui64vNV (GLenum target, GLenum pname, GLuint64EXT *params);
GLAPI void APIENTRY glGetNamedBufferParameterui64vNV (GLuint buffer, GLenum pname, GLuint64EXT *params);
GLAPI void APIENTRY glGetIntegerui64vNV (GLenum value, GLuint64EXT *result);
GLAPI void APIENTRY glUniformui64NV (GLint location, GLuint64EXT value);
GLAPI void APIENTRY glUniformui64vNV (GLint location, GLsizei count, const GLuint64EXT *value);
GLAPI void APIENTRY glGetUniformui64vNV (GLuint program, GLint location, GLuint64EXT *params);
GLAPI void APIENTRY glProgramUniformui64NV (GLuint program, GLint location, GLuint64EXT value);
GLAPI void APIENTRY glProgramUniformui64vNV (GLuint program, GLint location, GLsizei count, const GLuint64EXT *value);
#endif
#endif /* GL_NV_shader_buffer_load */

#ifndef GL_NV_shader_buffer_store
#define GL_NV_shader_buffer_store 1
#define GL_SHADER_GLOBAL_ACCESS_BARRIER_BIT_NV 0x00000010
#endif /* GL_NV_shader_buffer_store */

#ifndef GL_NV_shader_storage_buffer_object
#define GL_NV_shader_storage_buffer_object 1
#endif /* GL_NV_shader_storage_buffer_object */

#ifndef GL_NV_tessellation_program5
#define GL_NV_tessellation_program5 1
#define GL_MAX_PROGRAM_PATCH_ATTRIBS_NV   0x86D8
#define GL_TESS_CONTROL_PROGRAM_NV        0x891E
#define GL_TESS_EVALUATION_PROGRAM_NV     0x891F
#define GL_TESS_CONTROL_PROGRAM_PARAMETER_BUFFER_NV 0x8C74
#define GL_TESS_EVALUATION_PROGRAM_PARAMETER_BUFFER_NV 0x8C75
#endif /* GL_NV_tessellation_program5 */

#ifndef GL_NV_texgen_emboss
#define GL_NV_texgen_emboss 1
#define GL_EMBOSS_LIGHT_NV                0x855D
#define GL_EMBOSS_CONSTANT_NV             0x855E
#define GL_EMBOSS_MAP_NV                  0x855F
#endif /* GL_NV_texgen_emboss */

#ifndef GL_NV_texgen_reflection
#define GL_NV_texgen_reflection 1
#define GL_NORMAL_MAP_NV                  0x8511
#define GL_REFLECTION_MAP_NV              0x8512
#endif /* GL_NV_texgen_reflection */

#ifndef GL_NV_texture_barrier
#define GL_NV_texture_barrier 1
typedef void (APIENTRYP PFNGLTEXTUREBARRIERNVPROC) (void);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI void APIENTRY glTextureBarrierNV (void);
#endif
#endif /* GL_NV_texture_barrier */

#ifndef GL_NV_texture_compression_vtc
#define GL_NV_texture_compression_vtc 1
#endif /* GL_NV_texture_compression_vtc */

#ifndef GL_NV_texture_env_combine4
#define GL_NV_texture_env_combine4 1
#define GL_COMBINE4_NV                    0x8503
#define GL_SOURCE3_RGB_NV                 0x8583
#define GL_SOURCE3_ALPHA_NV               0x858B
#define GL_OPERAND3_RGB_NV                0x8593
#define GL_OPERAND3_ALPHA_NV              0x859B
#endif /* GL_NV_texture_env_combine4 */

#ifndef GL_NV_texture_expand_normal
#define GL_NV_texture_expand_normal 1
#define GL_TEXTURE_UNSIGNED_REMAP_MODE_NV 0x888F
#endif /* GL_NV_texture_expand_normal */

#ifndef GL_NV_texture_multisample
#define GL_NV_texture_multisample 1
#define GL_TEXTURE_COVERAGE_SAMPLES_NV    0x9045
#define GL_TEXTURE_COLOR_SAMPLES_NV       0x9046
typedef void (APIENTRYP PFNGLTEXIMAGE2DMULTISAMPLECOVERAGENVPROC) (GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations);
typedef void (APIENTRYP PFNGLTEXIMAGE3DMULTISAMPLECOVERAGENVPROC) (GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations);
typedef void (APIENTRYP PFNGLTEXTUREIMAGE2DMULTISAMPLENVPROC) (GLuint texture, GLenum target, GLsizei samples, GLint internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations);
typedef void (APIENTRYP PFNGLTEXTUREIMAGE3DMULTISAMPLENVPROC) (GLuint texture, GLenum target, GLsizei samples, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations);
typedef void (APIENTRYP PFNGLTEXTUREIMAGE2DMULTISAMPLECOVERAGENVPROC) (GLuint texture, GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations);
typedef void (APIENTRYP PFNGLTEXTUREIMAGE3DMULTISAMPLECOVERAGENVPROC) (GLuint texture, GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI void APIENTRY glTexImage2DMultisampleCoverageNV (GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations);
GLAPI void APIENTRY glTexImage3DMultisampleCoverageNV (GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations);
GLAPI void APIENTRY glTextureImage2DMultisampleNV (GLuint texture, GLenum target, GLsizei samples, GLint internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations);
GLAPI void APIENTRY glTextureImage3DMultisampleNV (GLuint texture, GLenum target, GLsizei samples, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations);
GLAPI void APIENTRY glTextureImage2DMultisampleCoverageNV (GLuint texture, GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations);
GLAPI void APIENTRY glTextureImage3DMultisampleCoverageNV (GLuint texture, GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations);
#endif
#endif /* GL_NV_texture_multisample */

#ifndef GL_NV_texture_rectangle
#define GL_NV_texture_rectangle 1
#define GL_TEXTURE_RECTANGLE_NV           0x84F5
#define GL_TEXTURE_BINDING_RECTANGLE_NV   0x84F6
#define GL_PROXY_TEXTURE_RECTANGLE_NV     0x84F7
#define GL_MAX_RECTANGLE_TEXTURE_SIZE_NV  0x84F8
#endif /* GL_NV_texture_rectangle */

#ifndef GL_NV_texture_shader
#define GL_NV_texture_shader 1
#define GL_OFFSET_TEXTURE_RECTANGLE_NV    0x864C
#define GL_OFFSET_TEXTURE_RECTANGLE_SCALE_NV 0x864D
#define GL_DOT_PRODUCT_TEXTURE_RECTANGLE_NV 0x864E
#define GL_RGBA_UNSIGNED_DOT_PRODUCT_MAPPING_NV 0x86D9
#define GL_UNSIGNED_INT_S8_S8_8_8_NV      0x86DA
#define GL_UNSIGNED_INT_8_8_S8_S8_REV_NV  0x86DB
#define GL_DSDT_MAG_INTENSITY_NV          0x86DC
#define GL_SHADER_CONSISTENT_NV           0x86DD
#define GL_TEXTURE_SHADER_NV              0x86DE
#define GL_SHADER_OPERATION_NV            0x86DF
#define GL_CULL_MODES_NV                  0x86E0
#define GL_OFFSET_TEXTURE_MATRIX_NV       0x86E1
#define GL_OFFSET_TEXTURE_SCALE_NV        0x86E2
#define GL_OFFSET_TEXTURE_BIAS_NV         0x86E3
#define GL_OFFSET_TEXTURE_2D_MATRIX_NV    0x86E1
#define GL_OFFSET_TEXTURE_2D_SCALE_NV     0x86E2
#define GL_OFFSET_TEXTURE_2D_BIAS_NV      0x86E3
#define GL_PREVIOUS_TEXTURE_INPUT_NV      0x86E4
#define GL_CONST_EYE_NV                   0x86E5
#define GL_PASS_THROUGH_NV                0x86E6
#define GL_CULL_FRAGMENT_NV               0x86E7
#define GL_OFFSET_TEXTURE_2D_NV           0x86E8
#define GL_DEPENDENT_AR_TEXTURE_2D_NV     0x86E9
#define GL_DEPENDENT_GB_TEXTURE_2D_NV     0x86EA
#define GL_DOT_PRODUCT_NV                 0x86EC
#define GL_DOT_PRODUCT_DEPTH_REPLACE_NV   0x86ED
#define GL_DOT_PRODUCT_TEXTURE_2D_NV      0x86EE
#define GL_DOT_PRODUCT_TEXTURE_CUBE_MAP_NV 0x86F0
#define GL_DOT_PRODUCT_DIFFUSE_CUBE_MAP_NV 0x86F1
#define GL_DOT_PRODUCT_REFLECT_CUBE_MAP_NV 0x86F2
#define GL_DOT_PRODUCT_CONST_EYE_REFLECT_CUBE_MAP_NV 0x86F3
#define GL_HILO_NV                        0x86F4
#define GL_DSDT_NV                        0x86F5
#define GL_DSDT_MAG_NV                    0x86F6
#define GL_DSDT_MAG_VIB_NV                0x86F7
#define GL_HILO16_NV                      0x86F8
#define GL_SIGNED_HILO_NV                 0x86F9
#define GL_SIGNED_HILO16_NV               0x86FA
#define GL_SIGNED_RGBA_NV                 0x86FB
#define GL_SIGNED_RGBA8_NV                0x86FC
#define GL_SIGNED_RGB_NV                  0x86FE
#define GL_SIGNED_RGB8_NV                 0x86FF
#define GL_SIGNED_LUMINANCE_NV            0x8701
#define GL_SIGNED_LUMINANCE8_NV           0x8702
#define GL_SIGNED_LUMINANCE_ALPHA_NV      0x8703
#define GL_SIGNED_LUMINANCE8_ALPHA8_NV    0x8704
#define GL_SIGNED_ALPHA_NV                0x8705
#define GL_SIGNED_ALPHA8_NV               0x8706
#define GL_SIGNED_INTENSITY_NV            0x8707
#define GL_SIGNED_INTENSITY8_NV           0x8708
#define GL_DSDT8_NV                       0x8709
#define GL_DSDT8_MAG8_NV                  0x870A
#define GL_DSDT8_MAG8_INTENSITY8_NV       0x870B
#define GL_SIGNED_RGB_UNSIGNED_ALPHA_NV   0x870C
#define GL_SIGNED_RGB8_UNSIGNED_ALPHA8_NV 0x870D
#define GL_HI_SCALE_NV                    0x870E
#define GL_LO_SCALE_NV                    0x870F
#define GL_DS_SCALE_NV                    0x8710
#define GL_DT_SCALE_NV                    0x8711
#define GL_MAGNITUDE_SCALE_NV             0x8712
#define GL_VIBRANCE_SCALE_NV              0x8713
#define GL_HI_BIAS_NV                     0x8714
#define GL_LO_BIAS_NV                     0x8715
#define GL_DS_BIAS_NV                     0x8716
#define GL_DT_BIAS_NV                     0x8717
#define GL_MAGNITUDE_BIAS_NV              0x8718
#define GL_VIBRANCE_BIAS_NV               0x8719
#define GL_TEXTURE_BORDER_VALUES_NV       0x871A
#define GL_TEXTURE_HI_SIZE_NV             0x871B
#define GL_TEXTURE_LO_SIZE_NV             0x871C
#define GL_TEXTURE_DS_SIZE_NV             0x871D
#define GL_TEXTURE_DT_SIZE_NV             0x871E
#define GL_TEXTURE_MAG_SIZE_NV            0x871F
#endif /* GL_NV_texture_shader */

#ifndef GL_NV_texture_shader2
#define GL_NV_texture_shader2 1
#define GL_DOT_PRODUCT_TEXTURE_3D_NV      0x86EF
#endif /* GL_NV_texture_shader2 */

#ifndef GL_NV_texture_shader3
#define GL_NV_texture_shader3 1
#define GL_OFFSET_PROJECTIVE_TEXTURE_2D_NV 0x8850
#define GL_OFFSET_PROJECTIVE_TEXTURE_2D_SCALE_NV 0x8851
#define GL_OFFSET_PROJECTIVE_TEXTURE_RECTANGLE_NV 0x8852
#define GL_OFFSET_PROJECTIVE_TEXTURE_RECTANGLE_SCALE_NV 0x8853
#define GL_OFFSET_HILO_TEXTURE_2D_NV      0x8854
#define GL_OFFSET_HILO_TEXTURE_RECTANGLE_NV 0x8855
#define GL_OFFSET_HILO_PROJECTIVE_TEXTURE_2D_NV 0x8856
#define GL_OFFSET_HILO_PROJECTIVE_TEXTURE_RECTANGLE_NV 0x8857
#define GL_DEPENDENT_HILO_TEXTURE_2D_NV   0x8858
#define GL_DEPENDENT_RGB_TEXTURE_3D_NV    0x8859
#define GL_DEPENDENT_RGB_TEXTURE_CUBE_MAP_NV 0x885A
#define GL_DOT_PRODUCT_PASS_THROUGH_NV    0x885B
#define GL_DOT_PRODUCT_TEXTURE_1D_NV      0x885C
#define GL_DOT_PRODUCT_AFFINE_DEPTH_REPLACE_NV 0x885D
#define GL_HILO8_NV                       0x885E
#define GL_SIGNED_HILO8_NV                0x885F
#define GL_FORCE_BLUE_TO_ONE_NV           0x8860
#endif /* GL_NV_texture_shader3 */

#ifndef GL_NV_transform_feedback
#define GL_NV_transform_feedback 1
#define GL_BACK_PRIMARY_COLOR_NV          0x8C77
#define GL_BACK_SECONDARY_COLOR_NV        0x8C78
#define GL_TEXTURE_COORD_NV               0x8C79
#define GL_CLIP_DISTANCE_NV               0x8C7A
#define GL_VERTEX_ID_NV                   0x8C7B
#define GL_PRIMITIVE_ID_NV                0x8C7C
#define GL_GENERIC_ATTRIB_NV              0x8C7D
#define GL_TRANSFORM_FEEDBACK_ATTRIBS_NV  0x8C7E
#define GL_TRANSFORM_FEEDBACK_BUFFER_MODE_NV 0x8C7F
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS_NV 0x8C80
#define GL_ACTIVE_VARYINGS_NV             0x8C81
#define GL_ACTIVE_VARYING_MAX_LENGTH_NV   0x8C82
#define GL_TRANSFORM_FEEDBACK_VARYINGS_NV 0x8C83
#define GL_TRANSFORM_FEEDBACK_BUFFER_START_NV 0x8C84
#define GL_TRANSFORM_FEEDBACK_BUFFER_SIZE_NV 0x8C85
#define GL_TRANSFORM_FEEDBACK_RECORD_NV   0x8C86
#define GL_PRIMITIVES_GENERATED_NV        0x8C87
#define GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN_NV 0x8C88
#define GL_RASTERIZER_DISCARD_NV          0x8C89
#define GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS_NV 0x8C8A
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS_NV 0x8C8B
#define GL_INTERLEAVED_ATTRIBS_NV         0x8C8C
#define GL_SEPARATE_ATTRIBS_NV            0x8C8D
#define GL_TRANSFORM_FEEDBACK_BUFFER_NV   0x8C8E
#define GL_TRANSFORM_FEEDBACK_BUFFER_BINDING_NV 0x8C8F
#define GL_LAYER_NV                       0x8DAA
#define GL_NEXT_BUFFER_NV                 -2
#define GL_SKIP_COMPONENTS4_NV            -3
#define GL_SKIP_COMPONENTS3_NV            -4
#define GL_SKIP_COMPONENTS2_NV            -5
#define GL_SKIP_COMPONENTS1_NV            -6
typedef void (APIENTRYP PFNGLBEGINTRANSFORMFEEDBACKNVPROC) (GLenum primitiveMode);
typedef void (APIENTRYP PFNGLENDTRANSFORMFEEDBACKNVPROC) (void);
typedef void (APIENTRYP PFNGLTRANSFORMFEEDBACKATTRIBSNVPROC) (GLuint count, const GLint *attribs, GLenum bufferMode);
typedef void (APIENTRYP PFNGLBINDBUFFERRANGENVPROC) (GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
typedef void (APIENTRYP PFNGLBINDBUFFEROFFSETNVPROC) (GLenum target, GLuint index, GLuint buffer, GLintptr offset);
typedef void (APIENTRYP PFNGLBINDBUFFERBASENVPROC) (GLenum target, GLuint index, GLuint buffer);
typedef void (APIENTRYP PFNGLTRANSFORMFEEDBACKVARYINGSNVPROC) (GLuint program, GLsizei count, const GLint *locations, GLenum bufferMode);
typedef void (APIENTRYP PFNGLACTIVEVARYINGNVPROC) (GLuint program, const GLchar *name);
typedef GLint (APIENTRYP PFNGLGETVARYINGLOCATIONNVPROC) (GLuint program, const GLchar *name);
typedef void (APIENTRYP PFNGLGETACTIVEVARYINGNVPROC) (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name);
typedef void (APIENTRYP PFNGLGETTRANSFORMFEEDBACKVARYINGNVPROC) (GLuint program, GLuint index, GLint *location);
typedef void (APIENTRYP PFNGLTRANSFORMFEEDBACKSTREAMATTRIBSNVPROC) (GLsizei count, const GLint *attribs, GLsizei nbuffers, const GLint *bufstreams, GLenum bufferMode);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI void APIENTRY glBeginTransformFeedbackNV (GLenum primitiveMode);
GLAPI void APIENTRY glEndTransformFeedbackNV (void);
GLAPI void APIENTRY glTransformFeedbackAttribsNV (GLuint count, const GLint *attribs, GLenum bufferMode);
GLAPI void APIENTRY glBindBufferRangeNV (GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
GLAPI void APIENTRY glBindBufferOffsetNV (GLenum target, GLuint index, GLuint buffer, GLintptr offset);
GLAPI void APIENTRY glBindBufferBaseNV (GLenum target, GLuint index, GLuint buffer);
GLAPI void APIENTRY glTransformFeedbackVaryingsNV (GLuint program, GLsizei count, const GLint *locations, GLenum bufferMode);
GLAPI void APIENTRY glActiveVaryingNV (GLuint program, const GLchar *name);
GLAPI GLint APIENTRY glGetVaryingLocationNV (GLuint program, const GLchar *name);
GLAPI void APIENTRY glGetActiveVaryingNV (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name);
GLAPI void APIENTRY glGetTransformFeedbackVaryingNV (GLuint program, GLuint index, GLint *location);
GLAPI void APIENTRY glTransformFeedbackStreamAttribsNV (GLsizei count, const GLint *attribs, GLsizei nbuffers, const GLint *bufstreams, GLenum bufferMode);
#endif
#endif /* GL_NV_transform_feedback */

#ifndef GL_NV_transform_feedback2
#define GL_NV_transform_feedback2 1
#define GL_TRANSFORM_FEEDBACK_NV          0x8E22
#define GL_TRANSFORM_FEEDBACK_BUFFER_PAUSED_NV 0x8E23
#define GL_TRANSFORM_FEEDBACK_BUFFER_ACTIVE_NV 0x8E24
#define GL_TRANSFORM_FEEDBACK_BINDING_NV  0x8E25
typedef void (APIENTRYP PFNGLBINDTRANSFORMFEEDBACKNVPROC) (GLenum target, GLuint id);
typedef void (APIENTRYP PFNGLDELETETRANSFORMFEEDBACKSNVPROC) (GLsizei n, const GLuint *ids);
typedef void (APIENTRYP PFNGLGENTRANSFORMFEEDBACKSNVPROC) (GLsizei n, GLuint *ids);
typedef GLboolean (APIENTRYP PFNGLISTRANSFORMFEEDBACKNVPROC) (GLuint id);
typedef void (APIENTRYP PFNGLPAUSETRANSFORMFEEDBACKNVPROC) (void);
typedef void (APIENTRYP PFNGLRESUMETRANSFORMFEEDBACKNVPROC) (void);
typedef void (APIENTRYP PFNGLDRAWTRANSFORMFEEDBACKNVPROC) (GLenum mode, GLuint id);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI void APIENTRY glBindTransformFeedbackNV (GLenum target, GLuint id);
GLAPI void APIENTRY glDeleteTransformFeedbacksNV (GLsizei n, const GLuint *ids);
GLAPI void APIENTRY glGenTransformFeedbacksNV (GLsizei n, GLuint *ids);
GLAPI GLboolean APIENTRY glIsTransformFeedbackNV (GLuint id);
GLAPI void APIENTRY glPauseTransformFeedbackNV (void);
GLAPI void APIENTRY glResumeTransformFeedbackNV (void);
GLAPI void APIENTRY glDrawTransformFeedbackNV (GLenum mode, GLuint id);
#endif
#endif /* GL_NV_transform_feedback2 */

#ifndef GL_NV_vdpau_interop
#define GL_NV_vdpau_interop 1
typedef GLintptr GLvdpauSurfaceNV;
#define GL_SURFACE_STATE_NV               0x86EB
#define GL_SURFACE_REGISTERED_NV          0x86FD
#define GL_SURFACE_MAPPED_NV              0x8700
#define GL_WRITE_DISCARD_NV               0x88BE
typedef void (APIENTRYP PFNGLVDPAUINITNVPROC) (const GLvoid *vdpDevice, const GLvoid *getProcAddress);
typedef void (APIENTRYP PFNGLVDPAUFININVPROC) (void);
typedef GLvdpauSurfaceNV (APIENTRYP PFNGLVDPAUREGISTERVIDEOSURFACENVPROC) (const GLvoid *vdpSurface, GLenum target, GLsizei numTextureNames, const GLuint *textureNames);
typedef GLvdpauSurfaceNV (APIENTRYP PFNGLVDPAUREGISTEROUTPUTSURFACENVPROC) (const GLvoid *vdpSurface, GLenum target, GLsizei numTextureNames, const GLuint *textureNames);
typedef void (APIENTRYP PFNGLVDPAUISSURFACENVPROC) (GLvdpauSurfaceNV surface);
typedef void (APIENTRYP PFNGLVDPAUUNREGISTERSURFACENVPROC) (GLvdpauSurfaceNV surface);
typedef void (APIENTRYP PFNGLVDPAUGETSURFACEIVNVPROC) (GLvdpauSurfaceNV surface, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values);
typedef void (APIENTRYP PFNGLVDPAUSURFACEACCESSNVPROC) (GLvdpauSurfaceNV surface, GLenum access);
typedef void (APIENTRYP PFNGLVDPAUMAPSURFACESNVPROC) (GLsizei numSurfaces, const GLvdpauSurfaceNV *surfaces);
typedef void (APIENTRYP PFNGLVDPAUUNMAPSURFACESNVPROC) (GLsizei numSurface, const GLvdpauSurfaceNV *surfaces);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI void APIENTRY glVDPAUInitNV (const GLvoid *vdpDevice, const GLvoid *getProcAddress);
GLAPI void APIENTRY glVDPAUFiniNV (void);
GLAPI GLvdpauSurfaceNV APIENTRY glVDPAURegisterVideoSurfaceNV (const GLvoid *vdpSurface, GLenum target, GLsizei numTextureNames, const GLuint *textureNames);
GLAPI GLvdpauSurfaceNV APIENTRY glVDPAURegisterOutputSurfaceNV (const GLvoid *vdpSurface, GLenum target, GLsizei numTextureNames, const GLuint *textureNames);
GLAPI void APIENTRY glVDPAUIsSurfaceNV (GLvdpauSurfaceNV surface);
GLAPI void APIENTRY glVDPAUUnregisterSurfaceNV (GLvdpauSurfaceNV surface);
GLAPI void APIENTRY glVDPAUGetSurfaceivNV (GLvdpauSurfaceNV surface, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values);
GLAPI void APIENTRY glVDPAUSurfaceAccessNV (GLvdpauSurfaceNV surface, GLenum access);
GLAPI void APIENTRY glVDPAUMapSurfacesNV (GLsizei numSurfaces, const GLvdpauSurfaceNV *surfaces);
GLAPI void APIENTRY glVDPAUUnmapSurfacesNV (GLsizei numSurface, const GLvdpauSurfaceNV *surfaces);
#endif
#endif /* GL_NV_vdpau_interop */

#ifndef GL_NV_vertex_array_range
#define GL_NV_vertex_array_range 1
#define GL_VERTEX_ARRAY_RANGE_NV          0x851D
#define GL_VERTEX_ARRAY_RANGE_LENGTH_NV   0x851E
#define GL_VERTEX_ARRAY_RANGE_VALID_NV    0x851F
#define GL_MAX_VERTEX_ARRAY_RANGE_ELEMENT_NV 0x8520
#define GL_VERTEX_ARRAY_RANGE_POINTER_NV  0x8521
typedef void (APIENTRYP PFNGLFLUSHVERTEXARRAYRANGENVPROC) (void);
typedef void (APIENTRYP PFNGLVERTEXARRAYRANGENVPROC) (GLsizei length, const GLvoid *pointer);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI void APIENTRY glFlushVertexArrayRangeNV (void);
GLAPI void APIENTRY glVertexArrayRangeNV (GLsizei length, const GLvoid *pointer);
#endif
#endif /* GL_NV_vertex_array_range */

#ifndef GL_NV_vertex_array_range2
#define GL_NV_vertex_array_range2 1
#define GL_VERTEX_ARRAY_RANGE_WITHOUT_FLUSH_NV 0x8533
#endif /* GL_NV_vertex_array_range2 */

#ifndef GL_NV_vertex_attrib_integer_64bit
#define GL_NV_vertex_attrib_integer_64bit 1
typedef void (APIENTRYP PFNGLVERTEXATTRIBL1I64NVPROC) (GLuint index, GLint64EXT x);
typedef void (APIENTRYP PFNGLVERTEXATTRIBL2I64NVPROC) (GLuint index, GLint64EXT x, GLint64EXT y);
typedef void (APIENTRYP PFNGLVERTEXATTRIBL3I64NVPROC) (GLuint index, GLint64EXT x, GLint64EXT y, GLint64EXT z);
typedef void (APIENTRYP PFNGLVERTEXATTRIBL4I64NVPROC) (GLuint index, GLint64EXT x, GLint64EXT y, GLint64EXT z, GLint64EXT w);
typedef void (APIENTRYP PFNGLVERTEXATTRIBL1I64VNVPROC) (GLuint index, const GLint64EXT *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBL2I64VNVPROC) (GLuint index, const GLint64EXT *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBL3I64VNVPROC) (GLuint index, const GLint64EXT *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBL4I64VNVPROC) (GLuint index, const GLint64EXT *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBL1UI64NVPROC) (GLuint index, GLuint64EXT x);
typedef void (APIENTRYP PFNGLVERTEXATTRIBL2UI64NVPROC) (GLuint index, GLuint64EXT x, GLuint64EXT y);
typedef void (APIENTRYP PFNGLVERTEXATTRIBL3UI64NVPROC) (GLuint index, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z);
typedef void (APIENTRYP PFNGLVERTEXATTRIBL4UI64NVPROC) (GLuint index, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z, GLuint64EXT w);
typedef void (APIENTRYP PFNGLVERTEXATTRIBL1UI64VNVPROC) (GLuint index, const GLuint64EXT *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBL2UI64VNVPROC) (GLuint index, const GLuint64EXT *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBL3UI64VNVPROC) (GLuint index, const GLuint64EXT *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBL4UI64VNVPROC) (GLuint index, const GLuint64EXT *v);
typedef void (APIENTRYP PFNGLGETVERTEXATTRIBLI64VNVPROC) (GLuint index, GLenum pname, GLint64EXT *params);
typedef void (APIENTRYP PFNGLGETVERTEXATTRIBLUI64VNVPROC) (GLuint index, GLenum pname, GLuint64EXT *params);
typedef void (APIENTRYP PFNGLVERTEXATTRIBLFORMATNVPROC) (GLuint index, GLint size, GLenum type, GLsizei stride);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI void APIENTRY glVertexAttribL1i64NV (GLuint index, GLint64EXT x);
GLAPI void APIENTRY glVertexAttribL2i64NV (GLuint index, GLint64EXT x, GLint64EXT y);
GLAPI void APIENTRY glVertexAttribL3i64NV (GLuint index, GLint64EXT x, GLint64EXT y, GLint64EXT z);
GLAPI void APIENTRY glVertexAttribL4i64NV (GLuint index, GLint64EXT x, GLint64EXT y, GLint64EXT z, GLint64EXT w);
GLAPI void APIENTRY glVertexAttribL1i64vNV (GLuint index, const GLint64EXT *v);
GLAPI void APIENTRY glVertexAttribL2i64vNV (GLuint index, const GLint64EXT *v);
GLAPI void APIENTRY glVertexAttribL3i64vNV (GLuint index, const GLint64EXT *v);
GLAPI void APIENTRY glVertexAttribL4i64vNV (GLuint index, const GLint64EXT *v);
GLAPI void APIENTRY glVertexAttribL1ui64NV (GLuint index, GLuint64EXT x);
GLAPI void APIENTRY glVertexAttribL2ui64NV (GLuint index, GLuint64EXT x, GLuint64EXT y);
GLAPI void APIENTRY glVertexAttribL3ui64NV (GLuint index, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z);
GLAPI void APIENTRY glVertexAttribL4ui64NV (GLuint index, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z, GLuint64EXT w);
GLAPI void APIENTRY glVertexAttribL1ui64vNV (GLuint index, const GLuint64EXT *v);
GLAPI void APIENTRY glVertexAttribL2ui64vNV (GLuint index, const GLuint64EXT *v);
GLAPI void APIENTRY glVertexAttribL3ui64vNV (GLuint index, const GLuint64EXT *v);
GLAPI void APIENTRY glVertexAttribL4ui64vNV (GLuint index, const GLuint64EXT *v);
GLAPI void APIENTRY glGetVertexAttribLi64vNV (GLuint index, GLenum pname, GLint64EXT *params);
GLAPI void APIENTRY glGetVertexAttribLui64vNV (GLuint index, GLenum pname, GLuint64EXT *params);
GLAPI void APIENTRY glVertexAttribLFormatNV (GLuint index, GLint size, GLenum type, GLsizei stride);
#endif
#endif /* GL_NV_vertex_attrib_integer_64bit */

#ifndef GL_NV_vertex_buffer_unified_memory
#define GL_NV_vertex_buffer_unified_memory 1
#define GL_VERTEX_ATTRIB_ARRAY_UNIFIED_NV 0x8F1E
#define GL_ELEMENT_ARRAY_UNIFIED_NV       0x8F1F
#define GL_VERTEX_ATTRIB_ARRAY_ADDRESS_NV 0x8F20
#define GL_VERTEX_ARRAY_ADDRESS_NV        0x8F21
#define GL_NORMAL_ARRAY_ADDRESS_NV        0x8F22
#define GL_COLOR_ARRAY_ADDRESS_NV         0x8F23
#define GL_INDEX_ARRAY_ADDRESS_NV         0x8F24
#define GL_TEXTURE_COORD_ARRAY_ADDRESS_NV 0x8F25
#define GL_EDGE_FLAG_ARRAY_ADDRESS_NV     0x8F26
#define GL_SECONDARY_COLOR_ARRAY_ADDRESS_NV 0x8F27
#define GL_FOG_COORD_ARRAY_ADDRESS_NV     0x8F28
#define GL_ELEMENT_ARRAY_ADDRESS_NV       0x8F29
#define GL_VERTEX_ATTRIB_ARRAY_LENGTH_NV  0x8F2A
#define GL_VERTEX_ARRAY_LENGTH_NV         0x8F2B
#define GL_NORMAL_ARRAY_LENGTH_NV         0x8F2C
#define GL_COLOR_ARRAY_LENGTH_NV          0x8F2D
#define GL_INDEX_ARRAY_LENGTH_NV          0x8F2E
#define GL_TEXTURE_COORD_ARRAY_LENGTH_NV  0x8F2F
#define GL_EDGE_FLAG_ARRAY_LENGTH_NV      0x8F30
#define GL_SECONDARY_COLOR_ARRAY_LENGTH_NV 0x8F31
#define GL_FOG_COORD_ARRAY_LENGTH_NV      0x8F32
#define GL_ELEMENT_ARRAY_LENGTH_NV        0x8F33
#define GL_DRAW_INDIRECT_UNIFIED_NV       0x8F40
#define GL_DRAW_INDIRECT_ADDRESS_NV       0x8F41
#define GL_DRAW_INDIRECT_LENGTH_NV        0x8F42
typedef void (APIENTRYP PFNGLBUFFERADDRESSRANGENVPROC) (GLenum pname, GLuint index, GLuint64EXT address, GLsizeiptr length);
typedef void (APIENTRYP PFNGLVERTEXFORMATNVPROC) (GLint size, GLenum type, GLsizei stride);
typedef void (APIENTRYP PFNGLNORMALFORMATNVPROC) (GLenum type, GLsizei stride);
typedef void (APIENTRYP PFNGLCOLORFORMATNVPROC) (GLint size, GLenum type, GLsizei stride);
typedef void (APIENTRYP PFNGLINDEXFORMATNVPROC) (GLenum type, GLsizei stride);
typedef void (APIENTRYP PFNGLTEXCOORDFORMATNVPROC) (GLint size, GLenum type, GLsizei stride);
typedef void (APIENTRYP PFNGLEDGEFLAGFORMATNVPROC) (GLsizei stride);
typedef void (APIENTRYP PFNGLSECONDARYCOLORFORMATNVPROC) (GLint size, GLenum type, GLsizei stride);
typedef void (APIENTRYP PFNGLFOGCOORDFORMATNVPROC) (GLenum type, GLsizei stride);
typedef void (APIENTRYP PFNGLVERTEXATTRIBFORMATNVPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride);
typedef void (APIENTRYP PFNGLVERTEXATTRIBIFORMATNVPROC) (GLuint index, GLint size, GLenum type, GLsizei stride);
typedef void (APIENTRYP PFNGLGETINTEGERUI64I_VNVPROC) (GLenum value, GLuint index, GLuint64EXT *result);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI void APIENTRY glBufferAddressRangeNV (GLenum pname, GLuint index, GLuint64EXT address, GLsizeiptr length);
GLAPI void APIENTRY glVertexFormatNV (GLint size, GLenum type, GLsizei stride);
GLAPI void APIENTRY glNormalFormatNV (GLenum type, GLsizei stride);
GLAPI void APIENTRY glColorFormatNV (GLint size, GLenum type, GLsizei stride);
GLAPI void APIENTRY glIndexFormatNV (GLenum type, GLsizei stride);
GLAPI void APIENTRY glTexCoordFormatNV (GLint size, GLenum type, GLsizei stride);
GLAPI void APIENTRY glEdgeFlagFormatNV (GLsizei stride);
GLAPI void APIENTRY glSecondaryColorFormatNV (GLint size, GLenum type, GLsizei stride);
GLAPI void APIENTRY glFogCoordFormatNV (GLenum type, GLsizei stride);
GLAPI void APIENTRY glVertexAttribFormatNV (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride);
GLAPI void APIENTRY glVertexAttribIFormatNV (GLuint index, GLint size, GLenum type, GLsizei stride);
GLAPI void APIENTRY glGetIntegerui64i_vNV (GLenum value, GLuint index, GLuint64EXT *result);
#endif
#endif /* GL_NV_vertex_buffer_unified_memory */

#ifndef GL_NV_vertex_program
#define GL_NV_vertex_program 1
#define GL_VERTEX_PROGRAM_NV              0x8620
#define GL_VERTEX_STATE_PROGRAM_NV        0x8621
#define GL_ATTRIB_ARRAY_SIZE_NV           0x8623
#define GL_ATTRIB_ARRAY_STRIDE_NV         0x8624
#define GL_ATTRIB_ARRAY_TYPE_NV           0x8625
#define GL_CURRENT_ATTRIB_NV              0x8626
#define GL_PROGRAM_LENGTH_NV              0x8627
#define GL_PROGRAM_STRING_NV              0x8628
#define GL_MODELVIEW_PROJECTION_NV        0x8629
#define GL_IDENTITY_NV                    0x862A
#define GL_INVERSE_NV                     0x862B
#define GL_TRANSPOSE_NV                   0x862C
#define GL_INVERSE_TRANSPOSE_NV           0x862D
#define GL_MAX_TRACK_MATRIX_STACK_DEPTH_NV 0x862E
#define GL_MAX_TRACK_MATRICES_NV          0x862F
#define GL_MATRIX0_NV                     0x8630
#define GL_MATRIX1_NV                     0x8631
#define GL_MATRIX2_NV                     0x8632
#define GL_MATRIX3_NV                     0x8633
#define GL_MATRIX4_NV                     0x8634
#define GL_MATRIX5_NV                     0x8635
#define GL_MATRIX6_NV                     0x8636
#define GL_MATRIX7_NV                     0x8637
#define GL_CURRENT_MATRIX_STACK_DEPTH_NV  0x8640
#define GL_CURRENT_MATRIX_NV              0x8641
#define GL_VERTEX_PROGRAM_POINT_SIZE_NV   0x8642
#define GL_VERTEX_PROGRAM_TWO_SIDE_NV     0x8643
#define GL_PROGRAM_PARAMETER_NV           0x8644
#define GL_ATTRIB_ARRAY_POINTER_NV        0x8645
#define GL_PROGRAM_TARGET_NV              0x8646
#define GL_PROGRAM_RESIDENT_NV            0x8647
#define GL_TRACK_MATRIX_NV                0x8648
#define GL_TRACK_MATRIX_TRANSFORM_NV      0x8649
#define GL_VERTEX_PROGRAM_BINDING_NV      0x864A
#define GL_PROGRAM_ERROR_POSITION_NV      0x864B
#define GL_VERTEX_ATTRIB_ARRAY0_NV        0x8650
#define GL_VERTEX_ATTRIB_ARRAY1_NV        0x8651
#define GL_VERTEX_ATTRIB_ARRAY2_NV        0x8652
#define GL_VERTEX_ATTRIB_ARRAY3_NV        0x8653
#define GL_VERTEX_ATTRIB_ARRAY4_NV        0x8654
#define GL_VERTEX_ATTRIB_ARRAY5_NV        0x8655
#define GL_VERTEX_ATTRIB_ARRAY6_NV        0x8656
#define GL_VERTEX_ATTRIB_ARRAY7_NV        0x8657
#define GL_VERTEX_ATTRIB_ARRAY8_NV        0x8658
#define GL_VERTEX_ATTRIB_ARRAY9_NV        0x8659
#define GL_VERTEX_ATTRIB_ARRAY10_NV       0x865A
#define GL_VERTEX_ATTRIB_ARRAY11_NV       0x865B
#define GL_VERTEX_ATTRIB_ARRAY12_NV       0x865C
#define GL_VERTEX_ATTRIB_ARRAY13_NV       0x865D
#define GL_VERTEX_ATTRIB_ARRAY14_NV       0x865E
#define GL_VERTEX_ATTRIB_ARRAY15_NV       0x865F
#define GL_MAP1_VERTEX_ATTRIB0_4_NV       0x8660
#define GL_MAP1_VERTEX_ATTRIB1_4_NV       0x8661
#define GL_MAP1_VERTEX_ATTRIB2_4_NV       0x8662
#define GL_MAP1_VERTEX_ATTRIB3_4_NV       0x8663
#define GL_MAP1_VERTEX_ATTRIB4_4_NV       0x8664
#define GL_MAP1_VERTEX_ATTRIB5_4_NV       0x8665
#define GL_MAP1_VERTEX_ATTRIB6_4_NV       0x8666
#define GL_MAP1_VERTEX_ATTRIB7_4_NV       0x8667
#define GL_MAP1_VERTEX_ATTRIB8_4_NV       0x8668
#define GL_MAP1_VERTEX_ATTRIB9_4_NV       0x8669
#define GL_MAP1_VERTEX_ATTRIB10_4_NV      0x866A
#define GL_MAP1_VERTEX_ATTRIB11_4_NV      0x866B
#define GL_MAP1_VERTEX_ATTRIB12_4_NV      0x866C
#define GL_MAP1_VERTEX_ATTRIB13_4_NV      0x866D
#define GL_MAP1_VERTEX_ATTRIB14_4_NV      0x866E
#define GL_MAP1_VERTEX_ATTRIB15_4_NV      0x866F
#define GL_MAP2_VERTEX_ATTRIB0_4_NV       0x8670
#define GL_MAP2_VERTEX_ATTRIB1_4_NV       0x8671
#define GL_MAP2_VERTEX_ATTRIB2_4_NV       0x8672
#define GL_MAP2_VERTEX_ATTRIB3_4_NV       0x8673
#define GL_MAP2_VERTEX_ATTRIB4_4_NV       0x8674
#define GL_MAP2_VERTEX_ATTRIB5_4_NV       0x8675
#define GL_MAP2_VERTEX_ATTRIB6_4_NV       0x8676
#define GL_MAP2_VERTEX_ATTRIB7_4_NV       0x8677
#define GL_MAP2_VERTEX_ATTRIB8_4_NV       0x8678
#define GL_MAP2_VERTEX_ATTRIB9_4_NV       0x8679
#define GL_MAP2_VERTEX_ATTRIB10_4_NV      0x867A
#define GL_MAP2_VERTEX_ATTRIB11_4_NV      0x867B
#define GL_MAP2_VERTEX_ATTRIB12_4_NV      0x867C
#define GL_MAP2_VERTEX_ATTRIB13_4_NV      0x867D
#define GL_MAP2_VERTEX_ATTRIB14_4_NV      0x867E
#define GL_MAP2_VERTEX_ATTRIB15_4_NV      0x867F
typedef GLboolean (APIENTRYP PFNGLAREPROGRAMSRESIDENTNVPROC) (GLsizei n, const GLuint *programs, GLboolean *residences);
typedef void (APIENTRYP PFNGLBINDPROGRAMNVPROC) (GLenum target, GLuint id);
typedef void (APIENTRYP PFNGLDELETEPROGRAMSNVPROC) (GLsizei n, const GLuint *programs);
typedef void (APIENTRYP PFNGLEXECUTEPROGRAMNVPROC) (GLenum target, GLuint id, const GLfloat *params);
typedef void (APIENTRYP PFNGLGENPROGRAMSNVPROC) (GLsizei n, GLuint *programs);
typedef void (APIENTRYP PFNGLGETPROGRAMPARAMETERDVNVPROC) (GLenum target, GLuint index, GLenum pname, GLdouble *params);
typedef void (APIENTRYP PFNGLGETPROGRAMPARAMETERFVNVPROC) (GLenum target, GLuint index, GLenum pname, GLfloat *params);
typedef void (APIENTRYP PFNGLGETPROGRAMIVNVPROC) (GLuint id, GLenum pname, GLint *params);
typedef void (APIENTRYP PFNGLGETPROGRAMSTRINGNVPROC) (GLuint id, GLenum pname, GLubyte *program);
typedef void (APIENTRYP PFNGLGETTRACKMATRIXIVNVPROC) (GLenum target, GLuint address, GLenum pname, GLint *params);
typedef void (APIENTRYP PFNGLGETVERTEXATTRIBDVNVPROC) (GLuint index, GLenum pname, GLdouble *params);
typedef void (APIENTRYP PFNGLGETVERTEXATTRIBFVNVPROC) (GLuint index, GLenum pname, GLfloat *params);
typedef void (APIENTRYP PFNGLGETVERTEXATTRIBIVNVPROC) (GLuint index, GLenum pname, GLint *params);
typedef void (APIENTRYP PFNGLGETVERTEXATTRIBPOINTERVNVPROC) (GLuint index, GLenum pname, GLvoid **pointer);
typedef GLboolean (APIENTRYP PFNGLISPROGRAMNVPROC) (GLuint id);
typedef void (APIENTRYP PFNGLLOADPROGRAMNVPROC) (GLenum target, GLuint id, GLsizei len, const GLubyte *program);
typedef void (APIENTRYP PFNGLPROGRAMPARAMETER4DNVPROC) (GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (APIENTRYP PFNGLPROGRAMPARAMETER4DVNVPROC) (GLenum target, GLuint index, const GLdouble *v);
typedef void (APIENTRYP PFNGLPROGRAMPARAMETER4FNVPROC) (GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (APIENTRYP PFNGLPROGRAMPARAMETER4FVNVPROC) (GLenum target, GLuint index, const GLfloat *v);
typedef void (APIENTRYP PFNGLPROGRAMPARAMETERS4DVNVPROC) (GLenum target, GLuint index, GLsizei count, const GLdouble *v);
typedef void (APIENTRYP PFNGLPROGRAMPARAMETERS4FVNVPROC) (GLenum target, GLuint index, GLsizei count, const GLfloat *v);
typedef void (APIENTRYP PFNGLREQUESTRESIDENTPROGRAMSNVPROC) (GLsizei n, const GLuint *programs);
typedef void (APIENTRYP PFNGLTRACKMATRIXNVPROC) (GLenum target, GLuint address, GLenum matrix, GLenum transform);
typedef void (APIENTRYP PFNGLVERTEXATTRIBPOINTERNVPROC) (GLuint index, GLint fsize, GLenum type, GLsizei stride, const GLvoid *pointer);
typedef void (APIENTRYP PFNGLVERTEXATTRIB1DNVPROC) (GLuint index, GLdouble x);
typedef void (APIENTRYP PFNGLVERTEXATTRIB1DVNVPROC) (GLuint index, const GLdouble *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIB1FNVPROC) (GLuint index, GLfloat x);
typedef void (APIENTRYP PFNGLVERTEXATTRIB1FVNVPROC) (GLuint index, const GLfloat *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIB1SNVPROC) (GLuint index, GLshort x);
typedef void (APIENTRYP PFNGLVERTEXATTRIB1SVNVPROC) (GLuint index, const GLshort *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIB2DNVPROC) (GLuint index, GLdouble x, GLdouble y);
typedef void (APIENTRYP PFNGLVERTEXATTRIB2DVNVPROC) (GLuint index, const GLdouble *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIB2FNVPROC) (GLuint index, GLfloat x, GLfloat y);
typedef void (APIENTRYP PFNGLVERTEXATTRIB2FVNVPROC) (GLuint index, const GLfloat *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIB2SNVPROC) (GLuint index, GLshort x, GLshort y);
typedef void (APIENTRYP PFNGLVERTEXATTRIB2SVNVPROC) (GLuint index, const GLshort *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIB3DNVPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z);
typedef void (APIENTRYP PFNGLVERTEXATTRIB3DVNVPROC) (GLuint index, const GLdouble *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIB3FNVPROC) (GLuint index, GLfloat x, GLfloat y, GLfloat z);
typedef void (APIENTRYP PFNGLVERTEXATTRIB3FVNVPROC) (GLuint index, const GLfloat *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIB3SNVPROC) (GLuint index, GLshort x, GLshort y, GLshort z);
typedef void (APIENTRYP PFNGLVERTEXATTRIB3SVNVPROC) (GLuint index, const GLshort *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIB4DNVPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (APIENTRYP PFNGLVERTEXATTRIB4DVNVPROC) (GLuint index, const GLdouble *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIB4FNVPROC) (GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (APIENTRYP PFNGLVERTEXATTRIB4FVNVPROC) (GLuint index, const GLfloat *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIB4SNVPROC) (GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
typedef void (APIENTRYP PFNGLVERTEXATTRIB4SVNVPROC) (GLuint index, const GLshort *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIB4UBNVPROC) (GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
typedef void (APIENTRYP PFNGLVERTEXATTRIB4UBVNVPROC) (GLuint index, const GLubyte *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBS1DVNVPROC) (GLuint index, GLsizei count, const GLdouble *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBS1FVNVPROC) (GLuint index, GLsizei count, const GLfloat *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBS1SVNVPROC) (GLuint index, GLsizei count, const GLshort *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBS2DVNVPROC) (GLuint index, GLsizei count, const GLdouble *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBS2FVNVPROC) (GLuint index, GLsizei count, const GLfloat *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBS2SVNVPROC) (GLuint index, GLsizei count, const GLshort *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBS3DVNVPROC) (GLuint index, GLsizei count, const GLdouble *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBS3FVNVPROC) (GLuint index, GLsizei count, const GLfloat *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBS3SVNVPROC) (GLuint index, GLsizei count, const GLshort *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBS4DVNVPROC) (GLuint index, GLsizei count, const GLdouble *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBS4FVNVPROC) (GLuint index, GLsizei count, const GLfloat *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBS4SVNVPROC) (GLuint index, GLsizei count, const GLshort *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBS4UBVNVPROC) (GLuint index, GLsizei count, const GLubyte *v);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI GLboolean APIENTRY glAreProgramsResidentNV (GLsizei n, const GLuint *programs, GLboolean *residences);
GLAPI void APIENTRY glBindProgramNV (GLenum target, GLuint id);
GLAPI void APIENTRY glDeleteProgramsNV (GLsizei n, const GLuint *programs);
GLAPI void APIENTRY glExecuteProgramNV (GLenum target, GLuint id, const GLfloat *params);
GLAPI void APIENTRY glGenProgramsNV (GLsizei n, GLuint *programs);
GLAPI void APIENTRY glGetProgramParameterdvNV (GLenum target, GLuint index, GLenum pname, GLdouble *params);
GLAPI void APIENTRY glGetProgramParameterfvNV (GLenum target, GLuint index, GLenum pname, GLfloat *params);
GLAPI void APIENTRY glGetProgramivNV (GLuint id, GLenum pname, GLint *params);
GLAPI void APIENTRY glGetProgramStringNV (GLuint id, GLenum pname, GLubyte *program);
GLAPI void APIENTRY glGetTrackMatrixivNV (GLenum target, GLuint address, GLenum pname, GLint *params);
GLAPI void APIENTRY glGetVertexAttribdvNV (GLuint index, GLenum pname, GLdouble *params);
GLAPI void APIENTRY glGetVertexAttribfvNV (GLuint index, GLenum pname, GLfloat *params);
GLAPI void APIENTRY glGetVertexAttribivNV (GLuint index, GLenum pname, GLint *params);
GLAPI void APIENTRY glGetVertexAttribPointervNV (GLuint index, GLenum pname, GLvoid **pointer);
GLAPI GLboolean APIENTRY glIsProgramNV (GLuint id);
GLAPI void APIENTRY glLoadProgramNV (GLenum target, GLuint id, GLsizei len, const GLubyte *program);
GLAPI void APIENTRY glProgramParameter4dNV (GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
GLAPI void APIENTRY glProgramParameter4dvNV (GLenum target, GLuint index, const GLdouble *v);
GLAPI void APIENTRY glProgramParameter4fNV (GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
GLAPI void APIENTRY glProgramParameter4fvNV (GLenum target, GLuint index, const GLfloat *v);
GLAPI void APIENTRY glProgramParameters4dvNV (GLenum target, GLuint index, GLsizei count, const GLdouble *v);
GLAPI void APIENTRY glProgramParameters4fvNV (GLenum target, GLuint index, GLsizei count, const GLfloat *v);
GLAPI void APIENTRY glRequestResidentProgramsNV (GLsizei n, const GLuint *programs);
GLAPI void APIENTRY glTrackMatrixNV (GLenum target, GLuint address, GLenum matrix, GLenum transform);
GLAPI void APIENTRY glVertexAttribPointerNV (GLuint index, GLint fsize, GLenum type, GLsizei stride, const GLvoid *pointer);
GLAPI void APIENTRY glVertexAttrib1dNV (GLuint index, GLdouble x);
GLAPI void APIENTRY glVertexAttrib1dvNV (GLuint index, const GLdouble *v);
GLAPI void APIENTRY glVertexAttrib1fNV (GLuint index, GLfloat x);
GLAPI void APIENTRY glVertexAttrib1fvNV (GLuint index, const GLfloat *v);
GLAPI void APIENTRY glVertexAttrib1sNV (GLuint index, GLshort x);
GLAPI void APIENTRY glVertexAttrib1svNV (GLuint index, const GLshort *v);
GLAPI void APIENTRY glVertexAttrib2dNV (GLuint index, GLdouble x, GLdouble y);
GLAPI void APIENTRY glVertexAttrib2dvNV (GLuint index, const GLdouble *v);
GLAPI void APIENTRY glVertexAttrib2fNV (GLuint index, GLfloat x, GLfloat y);
GLAPI void APIENTRY glVertexAttrib2fvNV (GLuint index, const GLfloat *v);
GLAPI void APIENTRY glVertexAttrib2sNV (GLuint index, GLshort x, GLshort y);
GLAPI void APIENTRY glVertexAttrib2svNV (GLuint index, const GLshort *v);
GLAPI void APIENTRY glVertexAttrib3dNV (GLuint index, GLdouble x, GLdouble y, GLdouble z);
GLAPI void APIENTRY glVertexAttrib3dvNV (GLuint index, const GLdouble *v);
GLAPI void APIENTRY glVertexAttrib3fNV (GLuint index, GLfloat x, GLfloat y, GLfloat z);
GLAPI void APIENTRY glVertexAttrib3fvNV (GLuint index, const GLfloat *v);
GLAPI void APIENTRY glVertexAttrib3sNV (GLuint index, GLshort x, GLshort y, GLshort z);
GLAPI void APIENTRY glVertexAttrib3svNV (GLuint index, const GLshort *v);
GLAPI void APIENTRY glVertexAttrib4dNV (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
GLAPI void APIENTRY glVertexAttrib4dvNV (GLuint index, const GLdouble *v);
GLAPI void APIENTRY glVertexAttrib4fNV (GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
GLAPI void APIENTRY glVertexAttrib4fvNV (GLuint index, const GLfloat *v);
GLAPI void APIENTRY glVertexAttrib4sNV (GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
GLAPI void APIENTRY glVertexAttrib4svNV (GLuint index, const GLshort *v);
GLAPI void APIENTRY glVertexAttrib4ubNV (GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
GLAPI void APIENTRY glVertexAttrib4ubvNV (GLuint index, const GLubyte *v);
GLAPI void APIENTRY glVertexAttribs1dvNV (GLuint index, GLsizei count, const GLdouble *v);
GLAPI void APIENTRY glVertexAttribs1fvNV (GLuint index, GLsizei count, const GLfloat *v);
GLAPI void APIENTRY glVertexAttribs1svNV (GLuint index, GLsizei count, const GLshort *v);
GLAPI void APIENTRY glVertexAttribs2dvNV (GLuint index, GLsizei count, const GLdouble *v);
GLAPI void APIENTRY glVertexAttribs2fvNV (GLuint index, GLsizei count, const GLfloat *v);
GLAPI void APIENTRY glVertexAttribs2svNV (GLuint index, GLsizei count, const GLshort *v);
GLAPI void APIENTRY glVertexAttribs3dvNV (GLuint index, GLsizei count, const GLdouble *v);
GLAPI void APIENTRY glVertexAttribs3fvNV (GLuint index, GLsizei count, const GLfloat *v);
GLAPI void APIENTRY glVertexAttribs3svNV (GLuint index, GLsizei count, const GLshort *v);
GLAPI void APIENTRY glVertexAttribs4dvNV (GLuint index, GLsizei count, const GLdouble *v);
GLAPI void APIENTRY glVertexAttribs4fvNV (GLuint index, GLsizei count, const GLfloat *v);
GLAPI void APIENTRY glVertexAttribs4svNV (GLuint index, GLsizei count, const GLshort *v);
GLAPI void APIENTRY glVertexAttribs4ubvNV (GLuint index, GLsizei count, const GLubyte *v);
#endif
#endif /* GL_NV_vertex_program */

#ifndef GL_NV_vertex_program1_1
#define GL_NV_vertex_program1_1 1
#endif /* GL_NV_vertex_program1_1 */

#ifndef GL_NV_vertex_program2
#define GL_NV_vertex_program2 1
#endif /* GL_NV_vertex_program2 */

#ifndef GL_NV_vertex_program2_option
#define GL_NV_vertex_program2_option 1
#endif /* GL_NV_vertex_program2_option */

#ifndef GL_NV_vertex_program3
#define GL_NV_vertex_program3 1
#endif /* GL_NV_vertex_program3 */

#ifndef GL_NV_vertex_program4
#define GL_NV_vertex_program4 1
#define GL_VERTEX_ATTRIB_ARRAY_INTEGER_NV 0x88FD
typedef void (APIENTRYP PFNGLVERTEXATTRIBI1IEXTPROC) (GLuint index, GLint x);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI2IEXTPROC) (GLuint index, GLint x, GLint y);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI3IEXTPROC) (GLuint index, GLint x, GLint y, GLint z);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI4IEXTPROC) (GLuint index, GLint x, GLint y, GLint z, GLint w);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI1UIEXTPROC) (GLuint index, GLuint x);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI2UIEXTPROC) (GLuint index, GLuint x, GLuint y);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI3UIEXTPROC) (GLuint index, GLuint x, GLuint y, GLuint z);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI4UIEXTPROC) (GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI1IVEXTPROC) (GLuint index, const GLint *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI2IVEXTPROC) (GLuint index, const GLint *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI3IVEXTPROC) (GLuint index, const GLint *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI4IVEXTPROC) (GLuint index, const GLint *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI1UIVEXTPROC) (GLuint index, const GLuint *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI2UIVEXTPROC) (GLuint index, const GLuint *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI3UIVEXTPROC) (GLuint index, const GLuint *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI4UIVEXTPROC) (GLuint index, const GLuint *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI4BVEXTPROC) (GLuint index, const GLbyte *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI4SVEXTPROC) (GLuint index, const GLshort *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI4UBVEXTPROC) (GLuint index, const GLubyte *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBI4USVEXTPROC) (GLuint index, const GLushort *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBIPOINTEREXTPROC) (GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
typedef void (APIENTRYP PFNGLGETVERTEXATTRIBIIVEXTPROC) (GLuint index, GLenum pname, GLint *params);
typedef void (APIENTRYP PFNGLGETVERTEXATTRIBIUIVEXTPROC) (GLuint index, GLenum pname, GLuint *params);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI void APIENTRY glVertexAttribI1iEXT (GLuint index, GLint x);
GLAPI void APIENTRY glVertexAttribI2iEXT (GLuint index, GLint x, GLint y);
GLAPI void APIENTRY glVertexAttribI3iEXT (GLuint index, GLint x, GLint y, GLint z);
GLAPI void APIENTRY glVertexAttribI4iEXT (GLuint index, GLint x, GLint y, GLint z, GLint w);
GLAPI void APIENTRY glVertexAttribI1uiEXT (GLuint index, GLuint x);
GLAPI void APIENTRY glVertexAttribI2uiEXT (GLuint index, GLuint x, GLuint y);
GLAPI void APIENTRY glVertexAttribI3uiEXT (GLuint index, GLuint x, GLuint y, GLuint z);
GLAPI void APIENTRY glVertexAttribI4uiEXT (GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
GLAPI void APIENTRY glVertexAttribI1ivEXT (GLuint index, const GLint *v);
GLAPI void APIENTRY glVertexAttribI2ivEXT (GLuint index, const GLint *v);
GLAPI void APIENTRY glVertexAttribI3ivEXT (GLuint index, const GLint *v);
GLAPI void APIENTRY glVertexAttribI4ivEXT (GLuint index, const GLint *v);
GLAPI void APIENTRY glVertexAttribI1uivEXT (GLuint index, const GLuint *v);
GLAPI void APIENTRY glVertexAttribI2uivEXT (GLuint index, const GLuint *v);
GLAPI void APIENTRY glVertexAttribI3uivEXT (GLuint index, const GLuint *v);
GLAPI void APIENTRY glVertexAttribI4uivEXT (GLuint index, const GLuint *v);
GLAPI void APIENTRY glVertexAttribI4bvEXT (GLuint index, const GLbyte *v);
GLAPI void APIENTRY glVertexAttribI4svEXT (GLuint index, const GLshort *v);
GLAPI void APIENTRY glVertexAttribI4ubvEXT (GLuint index, const GLubyte *v);
GLAPI void APIENTRY glVertexAttribI4usvEXT (GLuint index, const GLushort *v);
GLAPI void APIENTRY glVertexAttribIPointerEXT (GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
GLAPI void APIENTRY glGetVertexAttribIivEXT (GLuint index, GLenum pname, GLint *params);
GLAPI void APIENTRY glGetVertexAttribIuivEXT (GLuint index, GLenum pname, GLuint *params);
#endif
#endif /* GL_NV_vertex_program4 */

#ifndef GL_NV_video_capture
#define GL_NV_video_capture 1
#define GL_VIDEO_BUFFER_NV                0x9020
#define GL_VIDEO_BUFFER_BINDING_NV        0x9021
#define GL_FIELD_UPPER_NV                 0x9022
#define GL_FIELD_LOWER_NV                 0x9023
#define GL_NUM_VIDEO_CAPTURE_STREAMS_NV   0x9024
#define GL_NEXT_VIDEO_CAPTURE_BUFFER_STATUS_NV 0x9025
#define GL_VIDEO_CAPTURE_TO_422_SUPPORTED_NV 0x9026
#define GL_LAST_VIDEO_CAPTURE_STATUS_NV   0x9027
#define GL_VIDEO_BUFFER_PITCH_NV          0x9028
#define GL_VIDEO_COLOR_CONVERSION_MATRIX_NV 0x9029
#define GL_VIDEO_COLOR_CONVERSION_MAX_NV  0x902A
#define GL_VIDEO_COLOR_CONVERSION_MIN_NV  0x902B
#define GL_VIDEO_COLOR_CONVERSION_OFFSET_NV 0x902C
#define GL_VIDEO_BUFFER_INTERNAL_FORMAT_NV 0x902D
#define GL_PARTIAL_SUCCESS_NV             0x902E
#define GL_SUCCESS_NV                     0x902F
#define GL_FAILURE_NV                     0x9030
#define GL_YCBYCR8_422_NV                 0x9031
#define GL_YCBAYCR8A_4224_NV              0x9032
#define GL_Z6Y10Z6CB10Z6Y10Z6CR10_422_NV  0x9033
#define GL_Z6Y10Z6CB10Z6A10Z6Y10Z6CR10Z6A10_4224_NV 0x9034
#define GL_Z4Y12Z4CB12Z4Y12Z4CR12_422_NV  0x9035
#define GL_Z4Y12Z4CB12Z4A12Z4Y12Z4CR12Z4A12_4224_NV 0x9036
#define GL_Z4Y12Z4CB12Z4CR12_444_NV       0x9037
#define GL_VIDEO_CAPTURE_FRAME_WIDTH_NV   0x9038
#define GL_VIDEO_CAPTURE_FRAME_HEIGHT_NV  0x9039
#define GL_VIDEO_CAPTURE_FIELD_UPPER_HEIGHT_NV 0x903A
#define GL_VIDEO_CAPTURE_FIELD_LOWER_HEIGHT_NV 0x903B
#define GL_VIDEO_CAPTURE_SURFACE_ORIGIN_NV 0x903C
typedef void (APIENTRYP PFNGLBEGINVIDEOCAPTURENVPROC) (GLuint video_capture_slot);
typedef void (APIENTRYP PFNGLBINDVIDEOCAPTURESTREAMBUFFERNVPROC) (GLuint video_capture_slot, GLuint stream, GLenum frame_region, GLintptrARB offset);
typedef void (APIENTRYP PFNGLBINDVIDEOCAPTURESTREAMTEXTURENVPROC) (GLuint video_capture_slot, GLuint stream, GLenum frame_region, GLenum target, GLuint texture);
typedef void (APIENTRYP PFNGLENDVIDEOCAPTURENVPROC) (GLuint video_capture_slot);
typedef void (APIENTRYP PFNGLGETVIDEOCAPTUREIVNVPROC) (GLuint video_capture_slot, GLenum pname, GLint *params);
typedef void (APIENTRYP PFNGLGETVIDEOCAPTURESTREAMIVNVPROC) (GLuint video_capture_slot, GLuint stream, GLenum pname, GLint *params);
typedef void (APIENTRYP PFNGLGETVIDEOCAPTURESTREAMFVNVPROC) (GLuint video_capture_slot, GLuint stream, GLenum pname, GLfloat *params);
typedef void (APIENTRYP PFNGLGETVIDEOCAPTURESTREAMDVNVPROC) (GLuint video_capture_slot, GLuint stream, GLenum pname, GLdouble *params);
typedef GLenum (APIENTRYP PFNGLVIDEOCAPTURENVPROC) (GLuint video_capture_slot, GLuint *sequence_num, GLuint64EXT *capture_time);
typedef void (APIENTRYP PFNGLVIDEOCAPTURESTREAMPARAMETERIVNVPROC) (GLuint video_capture_slot, GLuint stream, GLenum pname, const GLint *params);
typedef void (APIENTRYP PFNGLVIDEOCAPTURESTREAMPARAMETERFVNVPROC) (GLuint video_capture_slot, GLuint stream, GLenum pname, const GLfloat *params);
typedef void (APIENTRYP PFNGLVIDEOCAPTURESTREAMPARAMETERDVNVPROC) (GLuint video_capture_slot, GLuint stream, GLenum pname, const GLdouble *params);
#ifdef GL_GLEXT_PROTOTYPES
GLAPI void APIENTRY glBeginVideoCaptureNV (GLuint video_capture_slot);
GLAPI void APIENTRY glBindVideoCaptureStreamBufferNV (GLuint video_capture_slot, GLuint stream, GLenum frame_region, GLintptrARB offset);
GLAPI void APIENTRY glBindVideoCaptureStreamTextureNV (GLuint video_capture_slot, GLuint stream, GLenum frame_region, GLenum target, GLuint texture);
GLAPI void APIENTRY glEndVideoCaptureNV (GLuint video_capture_slot);
GLAPI void APIENTRY glGetVideoCaptureivNV (GLuint video_capture_slot, GLenum pname, GLint *params);
GLAPI void APIENTRY glGetVideoCaptureStreamivNV (GLuint video_capture_slot, GLuint stream, GLenum pname, GLint *params);
GLAPI void APIENTRY glGetVideoCaptureStreamfvNV (GLuint video_capture_slot, GLuint stream, GLenum pname, GLfloat *params);
GLAPI void APIENTRY glGetVideoCaptureStreamdvNV (GLuint video_capture_slot, GLuint stream, GLenum pname, GLdouble *params);
GLAPI GLenum APIENTRY glVideoCaptureNV (GLuint video_capture_slot, GLuint *sequence_num, GLuint64EXT *capture_time);
GLAPI void APIENTRY glVideoCaptureStreamParameterivNV (GLuint video_capture_slot, GLuint stream, GLenum pname, const GLint *params);
GLAPI void APIENTRY glVideoCaptureStreamParameterfvNV (GLuint video_capture_slot, GLuint stream, GLenum pname, const GLfloat *params);
GLAPI void APIENTRY glVideoCaptureStreamParameterdvNV (GLuint video_capture_slot, GLuint stream, GLenum pname, const GLdouble *params);
#endif
#endif /* GL_NV_video_capture */

#ifdef __cplusplus
}
#endif

#endif
