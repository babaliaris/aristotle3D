#ifndef BARNSLEY_FERN_LAYER_H
#define BARNSLEY_FERN_LAYER_H
#include <aristotle3D/aristotle3D.h>
#include <cglm/cglm.h>
#include <cglm/struct.h>

#define BARNSLEY_MODE_DEFAULT   1 //Default, 15000 points Green color.
#define BARNSLEY_MODE_20000    2 //20000 points random color.
#define BARNSLEY_MODE_30000    3 //30000 points random color.
#define BARNSLEY_MODE_40000    4 //40000 points random color.


typedef struct BarnsleyLayer
{
    Ars3DVertexArray *      m_vao;
    Ars3DVertexBuffer *     m_vbo;
    Ars3DVertexAttributes * m_attribs;
    Ars3DShader *           m_shader;
    ars3d_int               m_num_of_points;
    ars3d_size              m_buffer_size;
    ars3d_float *           m_points_buffer;
    ars3d_int               m_barnsley_mode;
    ars3d_int               m_scale_toggler;
    ars3d_int               m_move_toggler;
    vec3s                   m_pos;
    vec3s                   m_rotate;
    vec3s                   m_scale;
    vec3s                   m_color;
    mat4s                   m_model;
    mat4s                   m_view;
    mat4s                   m_projection;
} BarnsleyLayer;

ars3d_void onBarnsleyLayerAttach(ars3d_void *p_ctx);

ars3d_void onBarnsleyLayerDetatch(ars3d_void *p_ctx);

ars3d_void onBarnsleyLayerStart(ars3d_void *p_ctx);

ars3d_void onBarnsleyLayerUpdate(ars3d_void *p_ctx, ars3d_float p_delta_time);

ars3d_uchar onBarnsleyLayerEvent(ars3d_void *p_ctx, ars3d_void *p_event, ars3d_int p_event_type);

#endif
