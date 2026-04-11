#ifndef ARS3D_UI_LAYER_H
#define ARS3D_UI_LAYER_H

#include <aristotle3D/core/ars3d_stdio.h>
#include <cglm/cglm.h>
#include <cglm/struct.h>


typedef struct Ars3DShader Ars3DShader;
typedef struct Ars3DVertexArray Ars3DVertexArray;
typedef struct Ars3DVertexBuffer Ars3DVertexBuffer;
typedef struct Ars3DVertexAttributes Ars3DVertexAttributes;

#define ARS3D_UI_MAX_VERTICES 4096

typedef struct Ars3DUILayer
{
  Ars3DShader *           m_shader;
  Ars3DVertexArray *      m_vao;
  Ars3DVertexBuffer *     m_vbo;
  Ars3DVertexAttributes * m_attribs;
  ars3d_uint              m_font_texture;
  mat4s                   m_proj;
} Ars3DUILayer;

Ars3DUILayer *ars3dUILayerCreate();

ars3d_void ars3dUiLayerOnAttach(ars3d_void *p_ctx);

ars3d_void ars3dUiLayerOnDetach(ars3d_void *p_ctx);

ars3d_void ars3dUiLayerOnUpdate(ars3d_void *p_ctx, ars3d_float p_delta_time);

#endif

