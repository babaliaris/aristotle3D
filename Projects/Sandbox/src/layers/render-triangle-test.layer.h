#ifndef RENDER_TRIANGLE_TEST_LAYER_H
#define RENDER_TRIANGLE_TEST_LAYER_H
#include <aristotle3D/aristotle3D.h>
#include <cglm/cglm.h>
#include <cglm/struct.h>

typedef struct RenderTriangleTestLayer
{
    Ars3DVertexArray *      m_vao;
    Ars3DVertexBuffer *     m_vbo;
    Ars3DVertexAttributes * m_attribs;
    Ars3DShader *           m_shader;
    mat4s                   m_model;
    mat4s                   m_view;
    mat4s                   m_projection;
} RenderTriangleTestLayer;

ars3d_void onRenderTriangleTestLayerAttach(ars3d_void *p_ctx);

ars3d_void onRenderTriangleTestLayerDetatch(ars3d_void *p_ctx);

ars3d_void onRenderTriangleTestLayerStart(ars3d_void *p_ctx);

ars3d_void onRenderTriangleTestLayerUpdate(ars3d_void *p_ctx);

ars3d_uchar onRenderTriangleTestLayerEvent(ars3d_void *p_ctx, ars3d_void *p_event, ars3d_int p_event_type);

#endif
