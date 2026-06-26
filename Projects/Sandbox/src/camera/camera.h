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
 * @param p_pitch The pitch angle in degrees.
 * @param p_yaw The yaw angle in degrees.
 *
 * @returns The newly created object.
 */
CameraSystem *cameraSystemCreate(vec3s p_start_position, float p_pitch, float p_yaw);

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
 * @brief Orbit the camera around the world origin (0,0,0).
 *
 * Controls the spherical coordinates (yaw for left/right, pitch for up/down)
 * and recalculates the position and axis so the camera points to the origin.
 *
 * @param p_cam The camera object.
 * @param p_dyaw The change in the horizontal angle (left/right) in degrees.
 * @param p_dpitch The change in the vertical angle (up/down) in degrees.
 * @param p_radius The distance from the center (0,0,0).
 */
void cameraSystemOrbit(CameraSystem *p_cam, float p_dyaw, float p_dpitch, float p_radius);


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


/**
 * @brief Set the pitch of the camera.
 *
 * @param p_cam The camera object.
 * @param p_pitch The pitch in degrees.
 *
 */
void cameraSystemSetPitch(CameraSystem *p_cam, float p_pitch);


/**
 * @brief Set the yaw of the camera.
 *
 * @param p_cam The camera object.
 * @param p_yaw The yaw in degrees.
 *
 */
void cameraSystemSetYaw(CameraSystem *p_cam, float p_yaw);



#endif
