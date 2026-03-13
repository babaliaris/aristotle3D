#ifndef BARNSLEY_FERN_LAYER_H
#define BARNSLEY_FERN_LAYER_H
#include <aristotle3D/aristotle3D.h>
#include <cglm/cglm.h>
#include <cglm/struct.h>

typedef struct BarnsleyLayer
{
    Ars3DVertexArray *      m_vao;
    Ars3DVertexBuffer *     m_vbo;
    Ars3DVertexAttributes * m_attribs;
    Ars3DShader *           m_shader;
    ars3d_float *           m_points;
    vec3s                   m_pos;
    vec3s                   m_rotate;
    vec3s                   m_scale;
    mat4s                   m_model;
    mat4s                   m_view;
    mat4s                   m_projection;
} BarnsleyLayer;

ars3d_void onBarnsleyLayerAttach(ars3d_void *p_ctx);

ars3d_void onBarnsleyLayerDetatch(ars3d_void *p_ctx);

ars3d_void onBarnsleyLayerStart(ars3d_void *p_ctx);

ars3d_void onBarnsleyLayerUpdate(ars3d_void *p_ctx);

ars3d_uchar onBarnsleyLayerEvent(ars3d_void *p_ctx, ars3d_void *p_event, ars3d_int p_event_type);

#endif
