#ifndef SANDBOX_CAMERA_CONTROLLER_H
#define SANDBOX_CAMERA_CONTROLLER_H


typedef struct CameraSystem CameraSystem;


typedef struct
{
  int m_binding_forward;
  int m_binding_backward;
  int m_binding_left;
  int m_binding_right;
  int m_binding_yaw_left;
  int m_binding_yaw_right;
  int m_binding_pitch_up;
  int m_binding_pitch_down;
} CameraControllerInput;

typedef struct
{
  float m_direction_x;
  float m_direction_y;
  float m_direction_z;

  float m_direction_yaw;
  float m_direction_pitch;

  float m_movement_speed;
  float m_yaw_speed;
  float m_pitch_speed;

  CameraControllerInput m_input;
  CameraSystem *m_camera_system;

} CameraController;

/**
 * @brief Initialize a stack allocated camera controller object.
 *
 * @param p_controller The object to be initialized.
 * @param p_camera_system The camera system object that this controller refers to.
 */
void cameraControllerStackInit(CameraController *p_controller, CameraSystem *p_camera_system);

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
);


/**
 * @brief Handles the input events that controlls the camera system.
 *
 * @param p_controller The controller object.
 * @param p_event The input event passed down to this handler.
 * @param p_event_type The event type.
 */
void cameraControllerEventHandler(CameraController *p_controller, void *p_event, int p_event_type);


/**
 * @brief Updates both movement and rotation of the camera system.
 *
 * @param p_controller The controller object.
 * @param p_delta_time The time between two frames, as passed down by the layer system.
 */
void cameraControllerFly(CameraController *p_controller, float p_delta_time);


/**
 * @brief Updates the camera orientation and position to orbit around the origin.
 *
 * It utilizes the active directional states mapped to arrow keys (yaw and pitch)
 * to manipulate the camera dynamically over a spherical shell.
 *
 * @param p_controller The controller object.
 * @param p_delta_time The time between two frames.
 * @param p_radius The distance from the center (0,0,0).
 */
void cameraControllerOrbit(CameraController *p_controller, float p_delta_time, float *p_radius);



/**
 * @brief Sets the movement speed.
 *
 * @param p_controller The controller object.
 * @param p_speed The speed value.
 */
void cameraControllerSetMovementSpeed(CameraController *p_controller, float p_speed);


/**
 * @brief Sets the yaw angular speed (in degrees).
 *
 * @param p_controller The controller object.
 * @param p_speed The speed value.
 */
void cameraControllerSetYawSpeed(CameraController *p_controller, float p_speed);


/**
 * @brief Sets the pitch angular speed (in degrees).
 *
 * @param p_controller The controller object.
 * @param p_speed The speed value.
 */
void cameraControllerSetPitchSpeed(CameraController *p_controller, float p_speed);

#endif
