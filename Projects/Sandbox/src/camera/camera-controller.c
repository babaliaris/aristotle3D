#include "camera-controller.h"
#include "camera.h"
#include <cglm/cglm.h>
#include <cglm/struct.h>
#include <aristotle3D/aristotle3D.h>

void cameraControllerStackInit(CameraController *p_controller, CameraSystem *p_camera_system)
{
  if (!p_controller || !p_camera_system)
  {
    ARS3D_WARN("Required parameters are not provided");
    return;
  }

  p_controller->m_camera_system   = p_camera_system;
  p_controller->m_direction_x     = 0;
  p_controller->m_direction_y     = 0;
  p_controller->m_direction_z     = 0;
  p_controller->m_direction_yaw   = 0;
  p_controller->m_direction_pitch = 0;
  p_controller->m_movement_speed  = 10.0f;
  p_controller->m_yaw_speed       = 80.0f;
  p_controller->m_pitch_speed     = 80.0f;

  p_controller->m_input.m_binding_forward   = ARS3D_EVENT_KEY_W;
  p_controller->m_input.m_binding_backward  = ARS3D_EVENT_KEY_S;
  p_controller->m_input.m_binding_left      = ARS3D_EVENT_KEY_A;
  p_controller->m_input.m_binding_right     = ARS3D_EVENT_KEY_D;
  p_controller->m_input.m_binding_yaw_left  = ARS3D_EVENT_KEY_LEFT;
  p_controller->m_input.m_binding_yaw_right = ARS3D_EVENT_KEY_RIGHT;
  p_controller->m_input.m_binding_pitch_up  = ARS3D_EVENT_KEY_UP;
  p_controller->m_input.m_binding_pitch_down= ARS3D_EVENT_KEY_DOWN;
}


void cameraControllerSetBindings(
  CameraController *p_controller,
  int p_binding_forward,
  int p_binding_backward,
  int p_binding_left,
  int p_binding_right,
  int p_binding_yaw_left,
  int p_binding_yaw_right,
  int p_binding_pitch_up,
  int p_binding_pitch_down
)
{
  if (!p_controller)
  {
    ARS3D_WARN("Required parameters are not provided");
    return;
  }

  p_controller->m_input.m_binding_forward   = p_binding_forward;
  p_controller->m_input.m_binding_backward  = p_binding_backward;
  p_controller->m_input.m_binding_left      = p_binding_left;
  p_controller->m_input.m_binding_right     = p_binding_right;
  p_controller->m_input.m_binding_yaw_left  = p_binding_yaw_left;
  p_controller->m_input.m_binding_yaw_right = p_binding_yaw_right;
  p_controller->m_input.m_binding_pitch_up  = p_binding_pitch_up;
  p_controller->m_input.m_binding_pitch_down= p_binding_pitch_down;
}


void cameraControllerEventHandler(CameraController *p_controller, void *p_event, int p_event_type)
{
  if (!p_controller)
  {
    ARS3D_WARN("Required parameters are not provided");
    return;
  }

  if (p_event_type == ARS3D_EVENT_TYPE_KEYBOARD_PRESS)
  {
    int key = ars3dEventGetKeyboardKey(p_event);

    if (key == p_controller->m_input.m_binding_forward) p_controller->m_direction_z -=1;
    if (key == p_controller->m_input.m_binding_backward) p_controller->m_direction_z +=1;
    if (key == p_controller->m_input.m_binding_left) p_controller->m_direction_x -=1;
    if (key == p_controller->m_input.m_binding_right) p_controller->m_direction_x +=1;
    if (key == p_controller->m_input.m_binding_yaw_left) p_controller->m_direction_yaw -=1;
    if (key == p_controller->m_input.m_binding_yaw_right) p_controller->m_direction_yaw +=1;
    if (key == p_controller->m_input.m_binding_pitch_up) p_controller->m_direction_pitch +=1;
    if (key == p_controller->m_input.m_binding_pitch_down) p_controller->m_direction_pitch -=1;
  }


  if (p_event_type == ARS3D_EVENT_TYPE_KEYBOARD_RELEASE)
  {
    int key = ars3dEventGetKeyboardKey(p_event);

    if (key == p_controller->m_input.m_binding_forward) p_controller->m_direction_z +=1;
    if (key == p_controller->m_input.m_binding_backward) p_controller->m_direction_z -=1;
    if (key == p_controller->m_input.m_binding_left) p_controller->m_direction_x +=1;
    if (key == p_controller->m_input.m_binding_right) p_controller->m_direction_x -=1;
    if (key == p_controller->m_input.m_binding_yaw_left) p_controller->m_direction_yaw +=1;
    if (key == p_controller->m_input.m_binding_yaw_right) p_controller->m_direction_yaw -=1;
    if (key == p_controller->m_input.m_binding_pitch_up) p_controller->m_direction_pitch -=1;
    if (key == p_controller->m_input.m_binding_pitch_down) p_controller->m_direction_pitch +=1;
  }
}

void cameraControllerFly(CameraController *p_controller, float p_delta_time)
{
  if (!p_controller)
  {
    ARS3D_WARN("Required parameters are not provided");
    return;
  }

  // Generate the velocity direction.
  vec3s velocity_direction =
    glms_vec3_normalize((vec3s)
    {{
    p_controller->m_direction_x,
    p_controller->m_direction_y,
    p_controller->m_direction_z
    }}
  );

  cameraSystemFly(
      p_controller->m_camera_system,
      glms_vec3_scale(velocity_direction, p_controller->m_movement_speed * p_delta_time),
      p_controller->m_direction_yaw * p_controller->m_yaw_speed * p_delta_time,
      p_controller->m_direction_pitch * p_controller->m_pitch_speed * p_delta_time
  );
}



void cameraControllerOrbit(CameraController *p_controller, float p_delta_time, float *p_radius)
{
  if (!p_controller)
  {
    ARS3D_WARN("Required parameters are not provided");
    return;
  }

  float zoom_speed = 30.0f;
  *p_radius += p_controller->m_direction_z * zoom_speed * p_delta_time;

  if (*p_radius < 5.0f)   *p_radius = 5.0f;
  if (*p_radius > 250.0f) *p_radius = 250.0f;

  // Calculate the yaw and pitch deltas.
  float dyaw   = p_controller->m_direction_yaw * p_controller->m_yaw_speed * p_delta_time;
  float dpitch = p_controller->m_direction_pitch * p_controller->m_pitch_speed * p_delta_time;

  // Call the camera system orbit function.
  cameraSystemOrbit(
      p_controller->m_camera_system,
      dyaw,
      dpitch,
      *p_radius
  );
}


void cameraControllerSetMovementSpeed(CameraController *p_controller, float p_speed)
{
  if (!p_controller)
  {
    ARS3D_WARN("Required parameters are not provided");
    return;
  }

  p_controller->m_movement_speed = p_speed;
}


void cameraControllerSetYawSpeed(CameraController *p_controller, float p_speed)
{
  if (!p_controller)
  {
    ARS3D_WARN("Required parameters are not provided");
    return;
  }

  p_controller->m_yaw_speed = p_speed;
}


void cameraControllerSetPitchSpeed(CameraController *p_controller, float p_speed)
{
  if (!p_controller)
  {
    ARS3D_WARN("Required parameters are not provided");
    return;
  }

  p_controller->m_pitch_speed = p_speed;
}

