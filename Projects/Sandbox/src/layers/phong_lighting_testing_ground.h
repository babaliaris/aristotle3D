#ifndef PHONG_LIGHTING_TESTING_GROUND_LAYER_H
#define PHONG_LIGHTING_TESTING_GROUND_LAYER_H
#include <aristotle3D/aristotle3D.h>
#include <cglm/cglm.h>
#include <cglm/struct.h>

typedef struct
{
  vec3s m_position;
  vec3s m_rotation;
  vec3s m_scale;
} PhongCube;

typedef struct
{
    Ars3DVertexArray *      m_vao;
    Ars3DVertexBuffer *     m_vbo;
    Ars3DVertexAttributes * m_attribs;
    Ars3DShader *           m_shader;
    mat4s                   m_projection;
    PhongCube               m_cube;
} PhongLightingTestingGroundLayer;

ars3d_void onPhongLightingTestingGroundLayerAttach(ars3d_void *p_ctx);

ars3d_void onPhongLightingTestingGroundLayerDetatch(ars3d_void *p_ctx);

ars3d_void onPhongLightingTestingGroundLayerStart(ars3d_void *p_ctx);

ars3d_void onPhongLightingTestingGroundLayerUpdate(ars3d_void *p_ctx, ars3d_float p_delta_time);

ars3d_uchar onPhongLightingTestingGroundLayerEvent(ars3d_void *p_ctx, ars3d_void *p_event, ars3d_int p_event_type);

/**
 * @brief Creates a new instance WITHOUT INITIALIZING IT!!!
 *
 * The respnsibilit of initializing the layer context lives within the onAttach or onStart
 * functions.
 */
PhongLightingTestingGroundLayer *phongLightingTestingGroundLayerCreate();

#endif
