#ifndef SANDBOX_CAMERA_H
#define SANDBOX_CAMERA_H

#include <cglm/cglm.h>
#include <cglm/struct.h>


/**
 * @file camera.h
 *
 * A camera system that generates a view matrix
 * based on the LookAt() functionality.
 */


typedef struct CameraSystem
{
  float m_yaw, m_pitch;

  vec3s m_position;
  vec3s m_front;
  vec3s m_helper_up;

  vec3s m_x_axis;
  vec3s m_y_axis;
  vec3s m_z_axis;

} CameraSystem;

/**
 * @brief Create a new camera object.
 *
 * @param p_start_position The starting world position of the camera.
 *
 * @returns The newly created object.
 */
CameraSystem *cameraSystemCreate(vec3s p_start_position);

/**
 * @brief Destroy a camera object.
 *
 * @param p_cam The memory address of the user's object container.
 */
void cameraSystemDestroy(CameraSystem **p_cam);

/**
 * @brief Move the camera both in position & orientation.
 *
 * Moves and rotates the camera based on delta changes in both
 * the position and direction (euler's angles).
 *
 * @param p_cam The memory address of the user's object container.
 * @param p_dpos The change in the position (for each component) relative to the camera space.
 * @param p_dyaw The change in the yaw angle in degrees.
 * @param p_dpitch The change in the pitch angle in degrees.
 */
void cameraSystemFly(CameraSystem *p_cam, vec3s p_dpos, float p_dyaw, float p_dpitch);


/**
 * @brief Get the projection matric of the camera.
 *
 * @param p_cam The camera object.
 *
 * @returns The calculated view 4x4 matrix.
 */
mat4s cameraSystemGetViewMatrix(CameraSystem *p_cam);


/**
 * @brief Get the projection matric of the camera.
 *
 * @param p_cam The camera object.
 *
 * @returns The world position of the camera.
 */
vec3s cameraSystemGetPosition(CameraSystem *p_cam);


/**
 * @brief Get the front direction of the camera.
 *
 * @param p_cam The camera object.
 *
 * @returns The direction that the camera is looking at.
 */
vec3s cameraSystemGetFront(CameraSystem *p_cam);

#endif
