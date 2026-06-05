#include "camera.h"
#include <math.h>
#include <aristotle3D/aristotle3D.h>

static void cameraSystemCalculateAxis(CameraSystem *p_cam);


CameraSystem *cameraSystemCreate(vec3s p_start_position)
{
  if (glms_vec3_norm(p_start_position) == 0 || glms_vec3_norm(p_start_position) < 0.001f)
  {
    ARS3D_WARN("The world position of the camera can not be (0,0,0)");
    return ARS3D_NULL;
  }

  CameraSystem *new_camera = (CameraSystem *)ars3dMalloc(ARS3D_SIZEOF(CameraSystem));

  if (!new_camera)
  {
    ARS3D_WARN("Out of memory!");
    return ARS3D_NULL;
  }

  // Initialize basic stuff.
  new_camera->m_position    = p_start_position;
  new_camera->m_front       = glms_vec3_negate(glms_normalize(p_start_position));
  new_camera->m_yaw         = glm_deg(atan2(new_camera->m_front.raw[2], new_camera->m_front.raw[0]));
  new_camera->m_pitch       = glm_deg(asin(new_camera->m_front.raw[1]));
  new_camera->m_helper_up   = (vec3s){{0.0f, 1.0f, 0.0f}};

  // Calculate the camera coordinate space.
  // This initializes the x, y, z axis fields.
  cameraSystemCalculateAxis(new_camera);

  return new_camera;
}


void cameraSystemDestroy(CameraSystem **p_cam)
{
  if (!p_cam || !(*p_cam))
  {
    ARS3D_WARN("User container or the object itself is NULL.");
    return;
  }

  ars3dFree(*p_cam);

  *p_cam = ARS3D_NULL;
}


void cameraSystemFly(CameraSystem *p_cam, vec3s p_dpos, float p_dyaw, float p_dpitch)
{
  if (!p_cam)
  {
    ARS3D_WARN("Function call is missing required parameters.");
    return;
  }

  p_cam->m_yaw   += p_dyaw;
  p_cam->m_pitch += p_dpitch;

  if (p_cam->m_pitch > 89.0f)  p_cam->m_pitch = 89.0f;
  if (p_cam->m_pitch < -89.0f) p_cam->m_pitch = -89.0f;

  cameraSystemCalculateAxis(p_cam);

  vec3s move_x = glms_vec3_scale(p_cam->m_x_axis, p_dpos.raw[0]);
  vec3s move_y = glms_vec3_scale(p_cam->m_y_axis, p_dpos.raw[1]);
  vec3s move_z = glms_vec3_scale(p_cam->m_z_axis, p_dpos.raw[2]); 

  vec3s velocity = glms_vec3_add(move_x, glms_vec3_add(move_y, move_z));

  p_cam->m_position = glms_vec3_add(p_cam->m_position, velocity);

  if (glms_vec3_norm(p_cam->m_position) < 0.001f)
  {
    p_cam->m_position = (vec3s){{0.0f, 0.0f, 0.001f}};
  }
}


mat4s cameraSystemGetViewMatrix(CameraSystem *p_cam)
{
  if (!p_cam)
  {
    ARS3D_WARN("Function call is missing required parameters.");
    return glms_mat4_identity();
  }

  return glms_lookat(
      p_cam->m_position,
      glms_vec3_add(p_cam->m_position, p_cam->m_front),
      p_cam->m_helper_up
  );
}


static void cameraSystemCalculateAxis(CameraSystem *p_cam)
{
  if (!p_cam)
  {
    ARS3D_WARN("Function call is missing required parameters.");
    return;
  }

  vec3s new_front;
  new_front.raw[0]  = cos(glm_rad(p_cam->m_yaw)) * cos(glm_rad(p_cam->m_pitch));
  new_front.raw[1]  = sin(glm_rad(p_cam->m_pitch));
  new_front.raw[2]  = sin(glm_rad(p_cam->m_yaw)) * cos(glm_rad(p_cam->m_pitch));
  p_cam->m_front    = glms_normalize(new_front);

  p_cam->m_z_axis   = glms_normalize(glms_vec3_negate(p_cam->m_front));
  p_cam->m_x_axis   = glms_normalize(glms_cross(p_cam->m_helper_up, p_cam->m_z_axis));
  p_cam->m_y_axis   = glms_normalize(glms_cross(p_cam->m_z_axis, p_cam->m_x_axis));
}
