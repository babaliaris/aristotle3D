#ifndef SANDBOX_LAYER_CUBE_H
#define SANDBOX_LAYER_CUBE_H

#include <aristotle3D/aristotle3D.h>
#include <cglm/cglm.h>
#include <cglm/struct.h>

// 9921 % 3 = 0
#define CUBE_LAYER_A              7.0f
#define CUBE_LAYER_B              100.0f
#define CUBE_LAYER_SQUARE_LENGTH  2.0f
#define CUBE_LAYER_ORBIT_RADIUS   6.0f
#define CUBE_LAYER_VX             1.0f
#define CUBE_LAYER_VY             1.0f
#define CUBE_LAYER_VZ             1.0f

// Run Mode ENUM.
#define CUBE_LAYER_RUN_MODE_A 0
#define CUBE_LAYER_RUN_MODE_B 1
#define CUBE_LAYER_RUN_MODE_C 2


typedef struct
{
  vec3s m_position;
  vec3s m_rotation;
  vec3s m_scale;

  mat4s m_model, m_view, m_projection;
} CubeTransform;


typedef struct
{
  CubeTransform m_transform;
  vec3s         m_camera_pos;
  float         m_rotate_angle;
  float         m_scale_factor;
  float         m_pulse_angle;
  float         m_rotation_speed;
  float         m_pulse_speed;
} CubeData;


typedef struct
{
  Ars3DVertexArray *      m_vao;
  Ars3DVertexBuffer *     m_vbo;
  Ars3DVertexAttributes * m_attribs;
  Ars3DShader *           m_shader;

  CubeData m_cube_data;

  ars3d_int m_run_mode;

} CubeLayerCtx;


ars3d_void cubeLayerOnAttach(ars3d_void *p_ctx);

ars3d_void cubeLayerOnDetatch(ars3d_void *p_ctx);

ars3d_void cubeLayerOnStart(ars3d_void *p_ctx);

ars3d_void cubeLayerOnUpdate(ars3d_void *p_ctx, ars3d_float p_delta_time);

ars3d_uchar cubeLayerOnEvent(ars3d_void *p_ctx, ars3d_void *p_event, ars3d_int p_event_type);



CubeLayerCtx *cubeLayerCreate();

#endif
