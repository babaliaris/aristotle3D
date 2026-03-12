#ifndef RENDER_TRIANGLE_TEST_LAYER_H
#define RENDER_TRIANGLE_TEST_LAYER_H
#include <aristotle3D/aristotle3D.h>

typedef struct RenderTriangleTestLayer
{
    Ars3DVertexArray *      m_vao;
    Ars3DVertexBuffer *     m_vbo;
    Ars3DVertexAttributes * m_attribs;
    Ars3DShader *           m_shader;
} RenderTriangleTestLayer;

ars3d_void onRenderTriangleTestLayerAttach(ars3d_void *p_ctx);

ars3d_void onRenderTriangleTestLayerDetatch(ars3d_void *p_ctx);

ars3d_void onRenderTriangleTestLayerStart(ars3d_void *p_ctx);

ars3d_void onRenderTriangleTestLayerUpdate(ars3d_void *p_ctx);

ars3d_uchar onRenderTriangleTestLayerEvent(ars3d_void *p_ctx, ars3d_void *p_event, ars3d_int p_event_type);

#endif
