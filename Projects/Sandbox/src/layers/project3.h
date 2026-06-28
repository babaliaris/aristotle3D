#ifndef SANDBOX_PROJECT3_H
#define SANDBOX_PROJECT3_H

#include "../camera/camera.h"
#include "../camera/camera-controller.h"
#include <aristotle3D/aristotle3D.h>
#include <cglm/struct.h>

#define CAMER_FOV   45.0f
#define CAMERA_NEAR 0.1f
#define CAMERA_FAR  400.0f

#define GAMEOBJECT_TYPE_GENERAL         0
#define GAMEOBJECT_TYPE_LIGHT_AMBIENT   1
#define GAMEOBJECT_TYPE_LIGHT_POINT     2
#define GAMEOBJECT_TYPE_LIGHT_SPOT      3

#define GRID_DIVISIONS    10
#define FLOATS_PER_VERTEX 6
#define VERTICES_PER_QUAD 6


typedef struct
{
    float x, y, z;
} point3;


typedef struct Mesh
{
  Ars3DVertexArray      *m_vao;
  Ars3DVertexBuffer     *m_vbo;
  Ars3DVertexAttributes *m_attributes;
  int                    m_num_of_vertices;
} Mesh;

typedef struct Transform
{
  vec3s m_position, m_rotation, m_scale;
  mat4s m_model, m_normal;
  int   m_is_dirty;
} Transform;

typedef struct Material
{
  vec3s m_ambient, m_diffuse, m_specular;
  float m_shininess;
} Material;

typedef struct AmbientLight
{
  vec3s m_ambient;
  float m_strength;
} AmbientLight;

typedef struct PointLight
{
  vec3s m_diffuse, m_specular;
  float m_strength, m_constant, m_linear, m_quadratic;
} PointLight;

typedef struct SpotLight
{
  vec3s m_direction, m_diffuse, m_specular;
  float m_strength, m_constant, m_linear, m_quadratic;
  float m_inner_cutoff_cos, m_outer_cutoff_cos;
} SpotLight;

typedef union Light
{
  AmbientLight m_ambient;
  PointLight   m_point;
  SpotLight    m_spot;
} Light;

typedef struct GameObject
{
  Transform   m_transform;
  Mesh      * m_mesh;       // References a mesh object.
  Material  * m_material;   // References a material object.
  Light       m_light;
  int         m_type;
} GameObject;


typedef struct SandboxProject3Layer
{
    Ars3DShader *     m_phong_shader; // Live for the entire app duration.
    Ars3DShader *     m_light_shader; // Live for the entire app duration.
    CameraSystem *    m_camera;       // Live for the entire app duration.
    CameraController  m_cam_controller;
    mat4s             m_projection;
    Mesh             *m_cube_mesh, *m_roof_mesh, *m_single_plane_mesh, *m_grid_mesh, *m_sphere_mesh;
    Material         *m_brown_mat, *m_blue_mat, *m_pink_mat, *m_roof_mat, *m_sun_mat, *m_moon_mat, *m_spot_mat;
    Material         *m_floor_mat;
    GameObject        m_ambient, m_sun_or_moon, m_flashlight;
    GameObject        m_house_base0, m_house_roof0;
    GameObject        m_house_base_blue, m_house_roof_blue;
    GameObject        m_house_base_pink, m_house_roof_pink;
    GameObject        m_floor;

    float             m_sun_theta, m_orbit_radius; // In radians.
    int               m_is_day, m_is_camera_orbiting, m_flat_shading;

} SandboxProject3Layer;


// =================================|LAYER CALLBACKS|================================= //
/**
 * @brief Gets called ONLY ONCE after the engine attaches the layer to the layers list.
 *
 * @param p_ctx The layer context object.
 */
ars3d_void onProject3LayerAttach(ars3d_void *p_ctx);

/**
 * @brief Is called ONLY ONCE after the engine removes the layer from the list.
 *
 * @param p_ctx The layer context object.
 */
ars3d_void onProject3LayerDetatch(ars3d_void *p_ctx);


/**
 * @brief Is called ONLY ONCE after inside the MainLoop.
 *
 * @param p_ctx The layer context object.
 */
ars3d_void onProject3LayerStart(ars3d_void *p_ctx);


/**
 * @brief Is called EVERY FRAME and after the onStart().
 *
 * @param p_ctx The layer context object.
 * @param p_delta_time The time difference between the current and previous frames.
 */
ars3d_void onProject3LayerUpdate(ars3d_void *p_ctx, ars3d_float p_delta_time);

/**
 * @brief It is called every time an input or window event occurs.
 *
 * @param p_ctx The layer context object.
 * @param p_event The event object.
 * @param p_event_type The event type.
 */
ars3d_uchar onProject3LayerEvent(ars3d_void *p_ctx, ars3d_void *p_event, ars3d_int p_event_type);

/**
 * @brief Creates a new  WITHOUT INITIALIZING IT!!!
 *
 * The responsibility of initializing the layer context lives within the onAttach or onStart
 * functions.
 */
SandboxProject3Layer *project3LayerCreateContext();
// =================================|LAYER CALLBACKS|================================= //

#endif
