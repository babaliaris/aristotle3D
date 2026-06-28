#include "project3.h"

void initializeGeometry(SandboxProject3Layer *ctx);
void initializeMaterials(SandboxProject3Layer *ctx);
void initializeGameObjects(SandboxProject3Layer *ctx);
void calculateModelMatric(GameObject *obj);
void renderGameObject(SandboxProject3Layer *ctx, GameObject *obj);
void generateGridTerrain(float *grid_vertices);
void updateCelestialTrajectory(SandboxProject3Layer *ctx, float p_delta_time);
void uiProject3Run(SandboxProject3Layer *ctx);
void generateSubdividedSphere(float *out_buffer, int *p_vertex_count);



// onAttach()
ars3d_void onProject3LayerAttach(ars3d_void *p_ctx)
{
  SandboxProject3Layer *ctx = (SandboxProject3Layer *)p_ctx;

  // Get the window width and height.
  ars3d_int screen_width, screen_height;
  ars3dWindowGetSize(
      ars3dAppGetWindow(ars3dAppGet()),
      &screen_width,
      &screen_height
  );

  ctx->m_camera       = cameraSystemCreate((vec3s){{0.0f, 8.0f, 20.0f}}, 25.0f, -90.);
  ctx->m_projection   = glms_perspective(glm_rad(CAMER_FOV), (float)screen_width / (float)screen_height, CAMERA_NEAR, CAMERA_FAR);
  ctx->m_phong_shader = ars3dShaderCreateFromFile(
      "Projects/Sandbox/src/shaders/phong.vertex.glsl",
      "Projects/Sandbox/src/shaders/phong.fragment.glsl"
  );
  ctx->m_light_shader = ars3dShaderCreateFromFile(
      "Projects/Sandbox/src/shaders/light.vertex.glsl",
      "Projects/Sandbox/src/shaders/light.fragment.glsl"
  );
  ctx->m_sun_theta          = GLM_PI;
  ctx->m_orbit_radius       = 120.0f;
  ctx->m_is_day             = 1;
  ctx->m_is_camera_orbiting = 1;

  // Initialize the camera controller.
  cameraControllerStackInit(&ctx->m_cam_controller, ctx->m_camera);

  // Initialize The Rest.
  initializeGeometry(ctx);
  initializeMaterials(ctx);
  initializeGameObjects(ctx);
}




// onDetach()
ars3d_void onProject3LayerDetatch(ars3d_void *p_ctx)
{
  SandboxProject3Layer *ctx = (SandboxProject3Layer *)p_ctx;

  // Destroy the camera and the shaders.
  cameraSystemDestroy(&ctx->m_camera);
  ars3dShaderDestroy(&ctx->m_light_shader);
  ars3dShaderDestroy(&ctx->m_phong_shader);

  // Destroy the cube mesh.
  ars3dVertexArrayDestroy(&ctx->m_cube_mesh->m_vao);
  ars3dVertexBufferDestroy(&ctx->m_cube_mesh->m_vbo);
  ars3dVertexAttributesDestroy(&ctx->m_cube_mesh->m_attributes);
  ars3dFree(ctx->m_cube_mesh);

  // Destroy the roof mesh.
  ars3dVertexArrayDestroy(&ctx->m_roof_mesh->m_vao);
  ars3dVertexBufferDestroy(&ctx->m_roof_mesh->m_vbo);
  ars3dVertexAttributesDestroy(&ctx->m_roof_mesh->m_attributes);
  ars3dFree(ctx->m_roof_mesh);

  // Destroy the single plane mesh.
  ars3dVertexArrayDestroy(&ctx->m_single_plane_mesh->m_vao);
  ars3dVertexBufferDestroy(&ctx->m_single_plane_mesh->m_vbo);
  ars3dVertexAttributesDestroy(&ctx->m_single_plane_mesh->m_attributes);
  ars3dFree(ctx->m_single_plane_mesh);


  // Destroy the grid plane mesh.
  ars3dVertexArrayDestroy(&ctx->m_grid_mesh->m_vao);
  ars3dVertexBufferDestroy(&ctx->m_grid_mesh->m_vbo);
  ars3dVertexAttributesDestroy(&ctx->m_grid_mesh->m_attributes);
  ars3dFree(ctx->m_grid_mesh);

  // Destroy the sphere mesh.
  ars3dVertexArrayDestroy(&ctx->m_sphere_mesh->m_vao);
  ars3dVertexBufferDestroy(&ctx->m_sphere_mesh->m_vbo);
  ars3dVertexAttributesDestroy(&ctx->m_sphere_mesh->m_attributes);
  ars3dFree(ctx->m_sphere_mesh);


  // Destroy the Materials.
  ars3dFree(ctx->m_brown_mat);
  ars3dFree(ctx->m_blue_mat);
  ars3dFree(ctx->m_pink_mat);
  ars3dFree(ctx->m_sun_mat);
  ars3dFree(ctx->m_moon_mat);
  ars3dFree(ctx->m_spot_mat);
  ars3dFree(ctx->m_roof_mat);

  // Destroy the layer.
  ars3dFree(ctx);
}




// onStart()
ars3d_void onProject3LayerStart(ars3d_void *p_ctx)
{
  ARS3D_UNUSED(p_ctx);
  ars3dGLEnableDither(1);
}




// onUpdate()
ars3d_void onProject3LayerUpdate(ars3d_void *p_ctx, ars3d_float p_delta_time)
{
  SandboxProject3Layer *ctx = (SandboxProject3Layer *)p_ctx;


  uiProject3Run(ctx);

  if (ctx->m_is_camera_orbiting)
  {
    cameraControllerOrbit(&ctx->m_cam_controller, p_delta_time, &ctx->m_orbit_radius);
  }

  else
  {
    cameraControllerFly(&ctx->m_cam_controller, p_delta_time);
  }

  // Update the SUN or Moon Trajectory (Including SUN Emission).
  updateCelestialTrajectory(ctx, p_delta_time);

  // Render House 0.
  renderGameObject(ctx, &ctx->m_house_base0);
  renderGameObject(ctx, &ctx->m_house_roof0);

  // Render Blue House.
  renderGameObject(ctx, &ctx->m_house_base_blue);
  renderGameObject(ctx, &ctx->m_house_roof_blue);

  // Render Pink House.
  renderGameObject(ctx, &ctx->m_house_base_pink);
  renderGameObject(ctx, &ctx->m_house_roof_pink);

  // Render the floor.
  renderGameObject(ctx, &ctx->m_floor);

  // Render light objects.
  renderGameObject(ctx, &ctx->m_ambient);
  renderGameObject(ctx, &ctx->m_sun_or_moon);
  renderGameObject(ctx, &ctx->m_flashlight);
}



// onEvent()
ars3d_uchar onProject3LayerEvent(ars3d_void *p_ctx, ars3d_void *p_event, ars3d_int p_event_type)
{
  SandboxProject3Layer *ctx = (SandboxProject3Layer *)p_ctx;

  switch (p_event_type)
  {
    // Update the projection matrix each time the window resizes.
    case ARS3D_EVENT_TYPE_WINDOW_RESIZED:
    {
      ars3d_int width, height;
      ars3dEventGetWindowSize(p_event, &width, &height);
      ctx->m_projection = glms_perspective(glm_rad(CAMER_FOV), (float)width/(float)height, CAMERA_NEAR, CAMERA_FAR);
      break;
    }

    default:
      break;
  }

  // Call the camera controller event handler.
  cameraControllerEventHandler(&ctx->m_cam_controller, p_event, p_event_type);
  return 0;
}



// createLayer()
SandboxProject3Layer *project3LayerCreateContext()
{
  SandboxProject3Layer *new_layer = (SandboxProject3Layer *)ars3dMalloc(ARS3D_SIZEOF(SandboxProject3Layer));

  if (!new_layer)
  {
    ARS3D_WARN("Out of memory!");
    return ARS3D_NULL;
  }

  return new_layer;
}



// initializeGeometry()
void initializeGeometry(SandboxProject3Layer *ctx)
{
  if (!ctx)
  {
    ARS3D_WARN("Required parameters are not provided!");
    return;
  }

  // Cube vertex data.
  float cube_data[] =
  {
      // Position                        // Normal
      -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
      0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
      0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
      0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
      -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
      -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

      -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
      0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
      -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

      -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
      -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
      -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
      -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
      -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
      -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
      0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
      0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
      0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
      0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

      -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
      0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
      0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
      0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

      -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
      0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
      -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
      -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
  };


  // Roof Vertex Data.
  float roof_data[] =
  {
      // =========================================================================
      // Right Face (Rectangular by 2 Triangles)
      // Normal: (0.866, 0.5, 0.0)
      // =========================================================================
      // Positions (X, Y, Z)                        // Normals (NX, NY, NZ)
      0.5f,  0.0f,  0.5f,               0.866f,  0.5f,  0.0f,
      0.5f,  0.0f, -0.5f,               0.866f,  0.5f,  0.0f,
      0.0f,  0.866f, -0.5f,          0.866f,  0.5f,  0.0f,

      0.0f,  0.866f, -0.5f,          0.866f,  0.5f,  0.0f,
      0.0f,  0.866f,  0.5f,          0.866f,  0.5f,  0.0f,
      0.5f,  0.0f,  0.5f,            0.866f,  0.5f,  0.0f,

      // =========================================================================
      // Left Face
      // Normal: (-0.866, 0.5, 0.0)
      // =========================================================================
      -0.5f,  0.0f, -0.5f,           -0.866f,  0.5f,  0.0f,
      -0.5f,  0.0f,  0.5f,           -0.866f,  0.5f,  0.0f,
      0.0f,  0.866f,  0.5f,          -0.866f,  0.5f,  0.0f,

      0.0f,  0.866f,  0.5f,          -0.866f,  0.5f,  0.0f,
      0.0f,  0.866f, -0.5f,          -0.866f,  0.5f,  0.0f,
      -0.5f,  0.0f, -0.5f,           -0.866f,  0.5f,  0.0f,

      // =========================================================================
      // Front Face (1 Triangle)
      // Normal: (0.0, 0.0, 1.0)
      // =========================================================================
      -0.5f,  0.0f,  0.5f,           0.0f,   0.0f,  1.0f,
      0.5f,  0.0f,  0.5f,            0.0f,   0.0f,  1.0f,
      0.0f,  0.866f,  0.5f,          0.0f,   0.0f,  1.0f,

      // =========================================================================
      // Back Face (1 Triangle)
      // Normal: (0.0, 0.0, -1.0)
      // =========================================================================
      0.5f,  0.0f, -0.5f,            0.0f,   0.0f, -1.0f,
      -0.5f,  0.0f, -0.5f,           0.0f,   0.0f, -1.0f,
      0.0f,  0.866f, -0.5f,       0.0f,   0.0f, -1.0f
  };


  // Single Plane Data.
  float single_plane_data[] =
  {
      // Positions                            //Normals
      -50.0f, 0.0f, -50.0f,       0.0f, 1.0f, 0.0f,
      50.0f, 0.0f, -50.0f,        0.0f, 1.0f, 0.0f,
      50.0f, 0.0f,  50.0f,     0.0f, 1.0f, 0.0f,

      // Triangle 2
      50.0f, 0.0f,  50.0f,     0.0f, 1.0f, 0.0f,
      -50.0f, 0.0f,  50.0f,    0.0f, 1.0f, 0.0f,
      -50.0f, 0.0f, -50.0f,    0.0f, 1.0f, 0.0f
  };

  // Generate and populate the 10x10 grid data.
  float grid_data[GRID_DIVISIONS * GRID_DIVISIONS * VERTICES_PER_QUAD * FLOATS_PER_VERTEX];
  generateGridTerrain(grid_data);

  // We start with 4 triangles.
  // For depth=4 we have 4*4*4*4*4=4^5=1024 triangles
  // 3 vertices * 1024 = 3072 vertices.
  // 3 floats * 3072   = 9216 floats.
  float sphere_vertices[9216];
  int sphere_count = 0;
  generateSubdividedSphere(sphere_vertices, &sphere_count);



  // --------------------------------|Create And Initialize The Cube Mesh|-------------------------------- //
  // Create the mesh and initialize it.
  ctx->m_cube_mesh                    = (Mesh *)ars3dMalloc(ARS3D_SIZEOF(Mesh));
  ctx->m_cube_mesh->m_vao             = ars3dVertexArrayCreate();
  ctx->m_cube_mesh->m_vbo             = ars3dVertexBufferCreate();
  ctx->m_cube_mesh->m_attributes      = ars3dVertexAttributesCreate();
  ctx->m_cube_mesh->m_num_of_vertices = 36;

  // Bind the buffers.
  ars3dVertexArrayBind(ctx->m_cube_mesh->m_vao);
  ars3dVertexBufferBind(ctx->m_cube_mesh->m_vbo);

  // Transfer the data to the GPU.
  ars3dVertexBufferData(ctx->m_cube_mesh->m_vbo, ARS3D_SIZEOF(cube_data), cube_data, ARS3D_VBO_STATIC_DRAW);

  // Create and Bind the vertex attributes.
  ars3dVertexAttributesPush(ctx->m_cube_mesh->m_attributes, "Positions", ARS3D_VERTEX_ATTRIBUTE_FLOAT, 3);
  ars3dVertexAttributesPush(ctx->m_cube_mesh->m_attributes, "Normals", ARS3D_VERTEX_ATTRIBUTE_FLOAT, 3);
  ars3dVertexAttributesBind(ctx->m_cube_mesh->m_attributes);

  // Unbind the buffers for safety.
  ars3dVertexArrayUnbind();
  ars3dVertexBufferUnbind();
  // --------------------------------|Create And Initialize The Cube Mesh|-------------------------------- //


  // --------------------------------|Create And Initialize The ROOF Mesh|-------------------------------- //
  // Create the mesh and initialize it.
  ctx->m_roof_mesh                    = (Mesh *)ars3dMalloc(ARS3D_SIZEOF(Mesh));
  ctx->m_roof_mesh->m_vao             = ars3dVertexArrayCreate();
  ctx->m_roof_mesh->m_vbo             = ars3dVertexBufferCreate();
  ctx->m_roof_mesh->m_attributes      = ars3dVertexAttributesCreate();
  ctx->m_roof_mesh->m_num_of_vertices = 18;

  // Bind the buffers.
  ars3dVertexArrayBind(ctx->m_roof_mesh->m_vao);
  ars3dVertexBufferBind(ctx->m_roof_mesh->m_vbo);

  // Transfer the data to the GPU.
  ars3dVertexBufferData(ctx->m_roof_mesh->m_vbo, ARS3D_SIZEOF(roof_data), roof_data, ARS3D_VBO_STATIC_DRAW);

  // Create and Bind the vertex attributes.
  ars3dVertexAttributesPush(ctx->m_roof_mesh->m_attributes, "Positions", ARS3D_VERTEX_ATTRIBUTE_FLOAT, 3);
  ars3dVertexAttributesPush(ctx->m_roof_mesh->m_attributes, "Normals", ARS3D_VERTEX_ATTRIBUTE_FLOAT, 3);
  ars3dVertexAttributesBind(ctx->m_roof_mesh->m_attributes);

  // Unbind the buffers for safety.
  ars3dVertexArrayUnbind();
  ars3dVertexBufferUnbind();
  // --------------------------------|Create And Initialize The ROOF Mesh|-------------------------------- //


  // ----------------------------|Create And Initialize The Single Plane Mesh|---------------------------- //
  // Create the mesh and initialize it.
  ctx->m_single_plane_mesh                    = (Mesh *)ars3dMalloc(ARS3D_SIZEOF(Mesh));
  ctx->m_single_plane_mesh->m_vao             = ars3dVertexArrayCreate();
  ctx->m_single_plane_mesh->m_vbo             = ars3dVertexBufferCreate();
  ctx->m_single_plane_mesh->m_attributes      = ars3dVertexAttributesCreate();
  ctx->m_single_plane_mesh->m_num_of_vertices = 6;

  // Bind the buffers.
  ars3dVertexArrayBind(ctx->m_single_plane_mesh->m_vao);
  ars3dVertexBufferBind(ctx->m_single_plane_mesh->m_vbo);

  // Transfer the data to the GPU.
  ars3dVertexBufferData(ctx->m_single_plane_mesh->m_vbo, ARS3D_SIZEOF(single_plane_data), single_plane_data, ARS3D_VBO_STATIC_DRAW);

  // Create and Bind the vertex attributes.
  ars3dVertexAttributesPush(ctx->m_single_plane_mesh->m_attributes, "Positions", ARS3D_VERTEX_ATTRIBUTE_FLOAT, 3);
  ars3dVertexAttributesPush(ctx->m_single_plane_mesh->m_attributes, "Normals", ARS3D_VERTEX_ATTRIBUTE_FLOAT, 3);
  ars3dVertexAttributesBind(ctx->m_single_plane_mesh->m_attributes);

  // Unbind the buffers for safety.
  ars3dVertexArrayUnbind();
  ars3dVertexBufferUnbind();
  // ----------------------------|Create And Initialize The Single Plane Mesh|---------------------------- //


  // -----------------------------|Create And Initialize The Grid Plane Mesh|----------------------------- //
  // Create the mesh and initialize it.
  ctx->m_grid_mesh                    = (Mesh *)ars3dMalloc(ARS3D_SIZEOF(Mesh));
  ctx->m_grid_mesh->m_vao             = ars3dVertexArrayCreate();
  ctx->m_grid_mesh->m_vbo             = ars3dVertexBufferCreate();
  ctx->m_grid_mesh->m_attributes      = ars3dVertexAttributesCreate();
  ctx->m_grid_mesh->m_num_of_vertices = GRID_DIVISIONS * GRID_DIVISIONS * VERTICES_PER_QUAD;

  // Bind the buffers.
  ars3dVertexArrayBind(ctx->m_grid_mesh->m_vao);
  ars3dVertexBufferBind(ctx->m_grid_mesh->m_vbo);

  // Transfer the data to the GPU.
  ars3dVertexBufferData(ctx->m_grid_mesh->m_vbo, ARS3D_SIZEOF(grid_data), grid_data, ARS3D_VBO_STATIC_DRAW);

  // Create and Bind the vertex attributes.
  ars3dVertexAttributesPush(ctx->m_grid_mesh->m_attributes, "Positions", ARS3D_VERTEX_ATTRIBUTE_FLOAT, 3);
  ars3dVertexAttributesPush(ctx->m_grid_mesh->m_attributes, "Normals", ARS3D_VERTEX_ATTRIBUTE_FLOAT, 3);
  ars3dVertexAttributesBind(ctx->m_grid_mesh->m_attributes);

  // Unbind the buffers for safety.
  ars3dVertexArrayUnbind();
  ars3dVertexBufferUnbind();
  // -----------------------------|Create And Initialize The Grid Plane Mesh|----------------------------- //


  // -------------------------------|Create And Initialize The Sphere Mesh|------------------------------- //
  // Create the mesh and initialize it.
  ctx->m_sphere_mesh                    = (Mesh *)ars3dMalloc(ARS3D_SIZEOF(Mesh));
  ctx->m_sphere_mesh->m_vao             = ars3dVertexArrayCreate();
  ctx->m_sphere_mesh->m_vbo             = ars3dVertexBufferCreate();
  ctx->m_sphere_mesh->m_attributes      = ars3dVertexAttributesCreate();
  ctx->m_sphere_mesh->m_num_of_vertices = sphere_count;

  // Bind the buffers.
  ars3dVertexArrayBind(ctx->m_sphere_mesh->m_vao);
  ars3dVertexBufferBind(ctx->m_sphere_mesh->m_vbo);

  // Transfer the data to the GPU.
  ars3dVertexBufferData(ctx->m_sphere_mesh->m_vbo, ARS3D_SIZEOF(sphere_vertices), sphere_vertices, ARS3D_VBO_STATIC_DRAW);

  // Create and Bind the vertex attributes.
  ars3dVertexAttributesPush(ctx->m_sphere_mesh->m_attributes, "Positions", ARS3D_VERTEX_ATTRIBUTE_FLOAT, 3);
  ars3dVertexAttributesBind(ctx->m_sphere_mesh->m_attributes);

  // Unbind the buffers for safety.
  ars3dVertexArrayUnbind();
  ars3dVertexBufferUnbind();
  // -------------------------------|Create And Initialize The Sphere Mesh|------------------------------- //

}




void initializeMaterials(SandboxProject3Layer *ctx)
{
  if (!ctx)
  {
    ARS3D_WARN("Required parameters are not provided!");
    return;
  }

  // Allocate memory for all the materials.
  ctx->m_brown_mat  = (Material *)ars3dMalloc(ARS3D_SIZEOF(Material));
  ctx->m_blue_mat   = (Material *)ars3dMalloc(ARS3D_SIZEOF(Material));
  ctx->m_pink_mat   = (Material *)ars3dMalloc(ARS3D_SIZEOF(Material));
  ctx->m_roof_mat   = (Material *)ars3dMalloc(ARS3D_SIZEOF(Material));
  ctx->m_sun_mat    = (Material *)ars3dMalloc(ARS3D_SIZEOF(Material));
  ctx->m_moon_mat   = (Material *)ars3dMalloc(ARS3D_SIZEOF(Material));
  ctx->m_spot_mat   = (Material *)ars3dMalloc(ARS3D_SIZEOF(Material));
  ctx->m_floor_mat  = (Material *)ars3dMalloc(ARS3D_SIZEOF(Material));

  // BROWN
  ctx->m_brown_mat->m_ambient   = (vec3s){{0.545f, 0.271f, 0.075f}};
  ctx->m_brown_mat->m_diffuse   = (vec3s){{0.545f, 0.271f, 0.075f}};
  ctx->m_brown_mat->m_specular  = (vec3s){{0.0f, 0.0f, 0.0f}};
  ctx->m_brown_mat->m_shininess = 0;

  //BLUE
  ctx->m_blue_mat->m_ambient   = (vec3s){{0.275f, 0.510f, 0.706f}};
  ctx->m_blue_mat->m_diffuse   = (vec3s){{0.275f, 0.510f, 0.706f}};
  ctx->m_blue_mat->m_specular  = (vec3s){{0.0f, 0.0f, 0.0f}};
  ctx->m_blue_mat->m_shininess = 0;

  // PINK
  ctx->m_pink_mat->m_ambient   = (vec3s){{0.859f, 0.439f, 0.576f}};
  ctx->m_pink_mat->m_diffuse   = (vec3s){{0.859f, 0.439f, 0.576f}};
  ctx->m_pink_mat->m_specular  = (vec3s){{0.0f, 0.0f, 0.0f}};
  ctx->m_pink_mat->m_shininess = 0;

  // Sun (White)
  ctx->m_sun_mat->m_ambient   = (vec3s){{1.0f, 1.0f, 1.0f}};
  ctx->m_sun_mat->m_diffuse   = (vec3s){{1.0f, 1.0f, 1.0f}};
  ctx->m_sun_mat->m_specular  = (vec3s){{0.0f, 0.0f, 0.0f}};
  ctx->m_sun_mat->m_shininess = 0;


  // Moon (Grey - Silver)
  ctx->m_moon_mat->m_ambient   = (vec3s){{0.753f, 0.753f, 0.753f}};
  ctx->m_moon_mat->m_diffuse   = (vec3s){{0.753f, 0.753f, 0.753f}};
  ctx->m_moon_mat->m_specular  = (vec3s){{0.0f, 0.0f, 0.0f}};
  ctx->m_moon_mat->m_shininess = 0;

  // Flashlight (Yellow)
  ctx->m_spot_mat->m_ambient   = (vec3s){{1.0f, 0.875f, 0.0f}};
  ctx->m_spot_mat->m_diffuse   = (vec3s){{1.0f, 0.875f, 0.0f}};
  ctx->m_spot_mat->m_specular  = (vec3s){{0.0f, 0.0f, 0.0f}};
  ctx->m_spot_mat->m_shininess = 0;

  // ROOF (Redish)
  ctx->m_roof_mat->m_ambient   = (vec3s){{0.698f, 0.133f, 0.133f}};
  ctx->m_roof_mat->m_diffuse   = (vec3s){{0.698f, 0.133f, 0.133f}};
  ctx->m_roof_mat->m_specular  = (vec3s){{1.0f, 1.0f, 1.0f}};
  ctx->m_roof_mat->m_shininess = 256;

  // Floor (Greenish)
  ctx->m_floor_mat->m_ambient   = (vec3s){{0.25f, 0.45f, 0.20f}};
  ctx->m_floor_mat->m_diffuse   = (vec3s){{0.25f, 0.45f, 0.20f}};
  ctx->m_floor_mat->m_specular  = (vec3s){{0.0f, 0.0f, 0.0f}};
  ctx->m_floor_mat->m_shininess = 0;
}


void initializeGameObjects(SandboxProject3Layer *ctx)
{
  if (!ctx)
  {
    ARS3D_WARN("Required parameters are not provided!");
    return;
  }

  // Ambient Light.
  ctx->m_ambient.m_transform.m_position       = (vec3s){{0.0f, 0.0f, 0.0f}};
  ctx->m_ambient.m_transform.m_rotation       = (vec3s){{0.0f, 0.0f, 0.0f}};
  ctx->m_ambient.m_transform.m_scale          = (vec3s){{1.0f, 1.0f, 1.0f}};
  ctx->m_ambient.m_transform.m_model          = glms_mat4_identity();
  ctx->m_ambient.m_transform.m_normal         = glms_mat4_identity();
  ctx->m_ambient.m_transform.m_is_dirty       = 1;
  ctx->m_ambient.m_mesh                       = ARS3D_NULL;
  ctx->m_ambient.m_material                   = ARS3D_NULL;
  ctx->m_ambient.m_light.m_ambient.m_ambient  = (vec3s){{1.0f, 1.0f, 1.0f}};
  ctx->m_ambient.m_light.m_ambient.m_strength = 0.2f;
  ctx->m_ambient.m_type                       = GAMEOBJECT_TYPE_LIGHT_AMBIENT;


  // Sun Or Moon (POINT LIGHT)
  ctx->m_sun_or_moon.m_transform.m_position       = (vec3s){{0.0f, 4.0f, 0.0f}};
  ctx->m_sun_or_moon.m_transform.m_rotation       = (vec3s){{0.0f, 0.0f, 0.0f}};
  ctx->m_sun_or_moon.m_transform.m_scale          = (vec3s){{1.0f, 1.0f, 1.0f}};
  ctx->m_sun_or_moon.m_transform.m_model          = glms_mat4_identity();
  ctx->m_sun_or_moon.m_transform.m_normal         = glms_mat4_identity();
  ctx->m_sun_or_moon.m_transform.m_is_dirty       = 1;
  ctx->m_sun_or_moon.m_mesh                       = ctx->m_sphere_mesh;
  ctx->m_sun_or_moon.m_material                   = ctx->m_sun_mat;
  ctx->m_sun_or_moon.m_light.m_point.m_diffuse    = (vec3s){{1.0f, 1.0f, 1.0f}};
  ctx->m_sun_or_moon.m_light.m_point.m_specular   = (vec3s){{1.0f, 1.0f, 1.0f}};
  ctx->m_sun_or_moon.m_light.m_point.m_strength   = 1.0f;
  ctx->m_sun_or_moon.m_light.m_point.m_constant   = 1.0f;
  ctx->m_sun_or_moon.m_light.m_point.m_linear     = 0.0f;
  ctx->m_sun_or_moon.m_light.m_point.m_quadratic  = 0.0f;
  ctx->m_sun_or_moon.m_type                       = GAMEOBJECT_TYPE_LIGHT_POINT;


  // Flashlight (SPOT Light)
  ctx->m_flashlight.m_transform.m_position            = (vec3s){{0.0f, 0.0f, 4.0f}};
  ctx->m_flashlight.m_transform.m_rotation            = (vec3s){{0.0f, 0.0f, 0.0f}};
  ctx->m_flashlight.m_transform.m_scale               = (vec3s){{1.0f, 1.0f, 1.0f}};
  ctx->m_flashlight.m_transform.m_model               = glms_mat4_identity();
  ctx->m_flashlight.m_transform.m_normal              = glms_mat4_identity();
  ctx->m_flashlight.m_transform.m_is_dirty            = 1;
  ctx->m_flashlight.m_mesh                            = ctx->m_cube_mesh;
  ctx->m_flashlight.m_material                        = ctx->m_spot_mat;
  ctx->m_flashlight.m_light.m_spot.m_direction        = (vec3s){{0.0f, 0.0f, -1.0f}};
  ctx->m_flashlight.m_light.m_spot.m_diffuse          = (vec3s){{1.0f, 1.0f, 1.0f}};
  ctx->m_flashlight.m_light.m_spot.m_specular         = (vec3s){{1.0f, 1.0f, 1.0f}};
  ctx->m_flashlight.m_light.m_spot.m_inner_cutoff_cos = cosf(glm_rad(12.5f));
  ctx->m_flashlight.m_light.m_spot.m_outer_cutoff_cos = cosf(glm_rad(17.5f));
  ctx->m_flashlight.m_light.m_spot.m_strength         = 1.0f;
  ctx->m_flashlight.m_light.m_spot.m_constant         = 1.0f;
  ctx->m_flashlight.m_light.m_spot.m_linear           = 0.045f;
  ctx->m_flashlight.m_light.m_spot.m_quadratic        = 0.0075f;
  ctx->m_flashlight.m_type                            = GAMEOBJECT_TYPE_LIGHT_SPOT;

  // House Base 0.
  ctx->m_house_base0.m_transform.m_position       = (vec3s){{0.0f, 0.5f, 0.0f}};
  ctx->m_house_base0.m_transform.m_rotation       = (vec3s){{0.0f, 0.0f, 0.0f}};
  ctx->m_house_base0.m_transform.m_scale          = (vec3s){{1.0f, 1.0f, 1.0f}};
  ctx->m_house_base0.m_transform.m_model          = glms_mat4_identity();
  ctx->m_house_base0.m_transform.m_normal         = glms_mat4_identity();
  ctx->m_house_base0.m_transform.m_is_dirty       = 1;
  ctx->m_house_base0.m_mesh                       = ctx->m_cube_mesh;
  ctx->m_house_base0.m_material                   = ctx->m_brown_mat;
  ctx->m_house_base0.m_type                       = GAMEOBJECT_TYPE_GENERAL;

  // House Roof 0.
  ctx->m_house_roof0.m_transform.m_position       = (vec3s){{0.0f, 1.0f, 0.0f}};
  ctx->m_house_roof0.m_transform.m_rotation       = (vec3s){{0.0f, 0.0f, 0.0f}};
  ctx->m_house_roof0.m_transform.m_scale          = (vec3s){{1.0f, 1.0f, 1.0f}};
  ctx->m_house_roof0.m_transform.m_model          = glms_mat4_identity();
  ctx->m_house_roof0.m_transform.m_normal         = glms_mat4_identity();
  ctx->m_house_roof0.m_transform.m_is_dirty       = 1;
  ctx->m_house_roof0.m_mesh                       = ctx->m_roof_mesh;
  ctx->m_house_roof0.m_material                   = ctx->m_roof_mat;
  ctx->m_house_roof0.m_type                       = GAMEOBJECT_TYPE_GENERAL;


  // Blue House Base (Left: X=-2.0, And a little bit back: Z=-1.5)
  ctx->m_house_base_blue.m_transform.m_position   = (vec3s){{-2.0f, 0.5f, -1.5f}};
  ctx->m_house_base_blue.m_transform.m_rotation   = (vec3s){{0.0f, 0.0f, 0.0f}};
  ctx->m_house_base_blue.m_transform.m_scale      = (vec3s){{1.0f, 1.0f, 1.0f}};
  ctx->m_house_base_blue.m_transform.m_model      = glms_mat4_identity();
  ctx->m_house_base_blue.m_transform.m_normal     = glms_mat4_identity();
  ctx->m_house_base_blue.m_transform.m_is_dirty   = 1;
  ctx->m_house_base_blue.m_mesh                   = ctx->m_cube_mesh;
  ctx->m_house_base_blue.m_material               = ctx->m_blue_mat;
  ctx->m_house_base_blue.m_type                   = GAMEOBJECT_TYPE_GENERAL;

  // Blue House Roof Y=0.5, X and Y same as the house base.
  ctx->m_house_roof_blue.m_transform.m_position   = (vec3s){{-2.0f, 1.0f, -1.5f}};
  ctx->m_house_roof_blue.m_transform.m_rotation   = (vec3s){{0.0f, 0.0f, 0.0f}};
  ctx->m_house_roof_blue.m_transform.m_scale      = (vec3s){{1.0f, 1.0f, 1.0f}};
  ctx->m_house_roof_blue.m_transform.m_model      = glms_mat4_identity();
  ctx->m_house_roof_blue.m_transform.m_normal     = glms_mat4_identity();
  ctx->m_house_roof_blue.m_transform.m_is_dirty   = 1;
  ctx->m_house_roof_blue.m_mesh                   = ctx->m_roof_mesh;
  ctx->m_house_roof_blue.m_material               = ctx->m_roof_mat;
  ctx->m_house_roof_blue.m_type                   = GAMEOBJECT_TYPE_GENERAL;

  // Pink House Base (Right: X=2.0, same depth as the main house Z=0.0)
  ctx->m_house_base_pink.m_transform.m_position   = (vec3s){{2.0f, 0.5f, 0.0f}};
  ctx->m_house_base_pink.m_transform.m_rotation   = (vec3s){{0.0f, 0.0f, 0.0f}};
  ctx->m_house_base_pink.m_transform.m_scale      = (vec3s){{1.0f, 1.0f, 1.0f}};
  ctx->m_house_base_pink.m_transform.m_model      = glms_mat4_identity();
  ctx->m_house_base_pink.m_transform.m_normal     = glms_mat4_identity();
  ctx->m_house_base_pink.m_transform.m_is_dirty   = 1;
  ctx->m_house_base_pink.m_mesh                   = ctx->m_cube_mesh;
  ctx->m_house_base_pink.m_material               = ctx->m_pink_mat;
  ctx->m_house_base_pink.m_type                   = GAMEOBJECT_TYPE_GENERAL;

  // Pink Roof Top Y=0.5
  ctx->m_house_roof_pink.m_transform.m_position   = (vec3s){{2.0f, 1.0f, 0.0f}};
  ctx->m_house_roof_pink.m_transform.m_rotation   = (vec3s){{0.0f, 0.0f, 0.0f}};
  ctx->m_house_roof_pink.m_transform.m_scale      = (vec3s){{1.0f, 1.0f, 1.0f}};
  ctx->m_house_roof_pink.m_transform.m_model      = glms_mat4_identity();
  ctx->m_house_roof_pink.m_transform.m_normal     = glms_mat4_identity();
  ctx->m_house_roof_pink.m_transform.m_is_dirty   = 1;
  ctx->m_house_roof_pink.m_mesh                   = ctx->m_roof_mesh;
  ctx->m_house_roof_pink.m_material               = ctx->m_roof_mat;
  ctx->m_house_roof_pink.m_type                   = GAMEOBJECT_TYPE_GENERAL;


  // Floor
  ctx->m_floor.m_transform.m_position   = (vec3s){{0.0f, 0.0f, 0.0f}};
  ctx->m_floor.m_transform.m_rotation   = (vec3s){{0.0f, 0.0f, 0.0f}};
  ctx->m_floor.m_transform.m_scale      = (vec3s){{1.0f, 1.0f, 1.0f}};
  ctx->m_floor.m_transform.m_model      = glms_mat4_identity();
  ctx->m_floor.m_transform.m_normal     = glms_mat4_identity();
  ctx->m_floor.m_transform.m_is_dirty   = 1;
  ctx->m_floor.m_mesh                   = ctx->m_single_plane_mesh;
  ctx->m_floor.m_material               = ctx->m_floor_mat;
  ctx->m_floor.m_type                   = GAMEOBJECT_TYPE_GENERAL;
}



void calculateModelMatric(GameObject *obj)
{
  if (!obj)
  {
    ARS3D_WARN("Required parameters are not provided!");
    return;
  }

  if (obj->m_transform.m_is_dirty)
  {
    // Get the gameobject matrices.
    mat4s *model = &obj->m_transform.m_model;
    mat4s *normal= &obj->m_transform.m_normal;

    // Calculate the MODEL matrix.
    *model = glms_mat4_identity();
    *model = glms_translate(*model, obj->m_transform.m_position);
    *model = glms_rotate(*model, glm_rad(obj->m_transform.m_rotation.raw[0]), (vec3s){{1.0f, 0.0f, 0.0f}});
    *model = glms_rotate(*model, glm_rad(obj->m_transform.m_rotation.raw[1]), (vec3s){{0.0f, 1.0f, 0.0f}});
    *model = glms_rotate(*model, glm_rad(obj->m_transform.m_rotation.raw[2]), (vec3s){{0.0f, 0.0f, 1.0f}});
    *model = glms_scale(*model, obj->m_transform.m_scale);

    // Calculate the NORMAL matrix.
    *normal = glms_mat4_transpose(glms_mat4_inv(*model));

    // Set the dirty flag to false.
    obj->m_transform.m_is_dirty = 0;
  }
}



void renderGameObject(SandboxProject3Layer *ctx, GameObject *obj)
{
  if (!ctx || !obj)
  {
    ARS3D_WARN("Required parameters are not provided!");
    return;
  }

  // If there is no mesh, no reason to render.
  if ( !obj->m_mesh ) return;

  // Make sure depth test is enabled.
  ars3dGLEnableDepthTest(1);

  // Calculate the model matrix.
  calculateModelMatric(obj);
  mat4s view = cameraSystemGetViewMatrix(ctx->m_camera);
  mat4s proj = ctx->m_projection;
  mat4s mvp  = glms_mul(glms_mul(proj, view), obj->m_transform.m_model);

  vec3s cam_pos = cameraSystemGetPosition(ctx->m_camera);

  AmbientLight *ambient_light = &ctx->m_ambient.m_light.m_ambient;
  vec3s ambient               = ambient_light->m_ambient;
  float ambient_strength      = ambient_light->m_strength;

  PointLight *point_light = &ctx->m_sun_or_moon.m_light.m_point;
  vec3s point_pos         = ctx->m_sun_or_moon.m_transform.m_position;
  vec3s point_diffuse     = point_light->m_diffuse;
  vec3s point_specular    = point_light->m_specular;
  float point_strenght    = point_light->m_strength;
  float point_constant    = point_light->m_constant;
  float point_linear      = point_light->m_linear;
  float point_quadratic   = point_light->m_quadratic;

  SpotLight *spot_light = &ctx->m_flashlight.m_light.m_spot;
  vec3s spot_pos         = ctx->m_flashlight.m_transform.m_position;
  vec3s spot_direction   = spot_light->m_direction;
  vec3s spot_diffuse     = spot_light->m_diffuse;
  vec3s spot_specular    = spot_light->m_specular;
  float spot_inner_cos   = spot_light->m_inner_cutoff_cos;
  float spot_outer_cos   = spot_light->m_outer_cutoff_cos;
  float spot_strenght    = spot_light->m_strength;
  float spot_constant    = spot_light->m_constant;
  float spot_linear      = spot_light->m_linear;
  float spot_quadratic   = spot_light->m_quadratic;

  // Bind the phong shader.
  ars3dShaderBind(ctx->m_phong_shader);

  // Ambient Light.
  ars3dShaderUniformFloat(ctx->m_phong_shader, "u_ambient.m_strength", ambient_strength);
  ars3dShaderUniformFloat3(ctx->m_phong_shader, "u_ambient.m_ambient", ambient.raw[0], ambient.raw[1], ambient.raw[2]);

  // Point Light.
  ars3dShaderUniformFloat(ctx->m_phong_shader, "u_point_light.m_strength", point_strenght);
  ars3dShaderUniformFloat3(ctx->m_phong_shader, "u_point_light.m_position", point_pos.raw[0], point_pos.raw[1], point_pos.raw[2]);
  ars3dShaderUniformFloat3(ctx->m_phong_shader, "u_point_light.m_diffuse", point_diffuse.raw[0], point_diffuse.raw[1], point_diffuse.raw[2]);
  ars3dShaderUniformFloat3(ctx->m_phong_shader, "u_point_light.m_specular", point_specular.raw[0], point_specular.raw[1], point_specular.raw[2]);
  ars3dShaderUniformFloat(ctx->m_phong_shader, "u_point_light.m_attenuation.m_constant", point_constant);
  ars3dShaderUniformFloat(ctx->m_phong_shader, "u_point_light.m_attenuation.m_linear", point_linear);
  ars3dShaderUniformFloat(ctx->m_phong_shader, "u_point_light.m_attenuation.m_quadratic", point_quadratic);

  // Spot Light
  ars3dShaderUniformFloat(ctx->m_phong_shader, "u_spot_light.m_inner_cutoff_cos", spot_inner_cos);
  ars3dShaderUniformFloat(ctx->m_phong_shader, "u_spot_light.m_outer_cutoff_cos", spot_outer_cos);
  ars3dShaderUniformFloat3(ctx->m_phong_shader, "u_spot_light.m_direction", spot_direction.raw[0], spot_direction.raw[1], spot_direction.raw[2]);
  ars3dShaderUniformFloat(ctx->m_phong_shader, "u_spot_light.m_light.m_strength", spot_strenght);
  ars3dShaderUniformFloat3(ctx->m_phong_shader, "u_spot_light.m_light.m_position", spot_pos.raw[0], spot_pos.raw[1], spot_pos.raw[2]);
  ars3dShaderUniformFloat3(ctx->m_phong_shader, "u_spot_light.m_light.m_diffuse", spot_diffuse.raw[0], spot_diffuse.raw[1], spot_diffuse.raw[2]);
  ars3dShaderUniformFloat3(ctx->m_phong_shader, "u_spot_light.m_light.m_specular", spot_specular.raw[0], spot_specular.raw[1], spot_specular.raw[2]);
  ars3dShaderUniformFloat(ctx->m_phong_shader, "u_spot_light.m_light.m_attenuation.m_constant", spot_constant);
  ars3dShaderUniformFloat(ctx->m_phong_shader, "u_spot_light.m_light.m_attenuation.m_linear", spot_linear);
  ars3dShaderUniformFloat(ctx->m_phong_shader, "u_spot_light.m_light.m_attenuation.m_quadratic", spot_quadratic);


  // REGULAR OBJECT (Receives Lighting).
  if (obj->m_type == GAMEOBJECT_TYPE_GENERAL && obj->m_material)
  {
    // Set the material properties.
    Material *mat = obj->m_material;
    ars3dShaderUniformFloat3(ctx->m_phong_shader, "u_material.m_ambient", mat->m_ambient.raw[0], mat->m_ambient.raw[1], mat->m_ambient.raw[2]);
    ars3dShaderUniformFloat3(ctx->m_phong_shader, "u_material.m_diffuse", mat->m_diffuse.raw[0], mat->m_diffuse.raw[1], mat->m_diffuse.raw[2]);
    ars3dShaderUniformFloat3(ctx->m_phong_shader, "u_material.m_specular", mat->m_specular.raw[0], mat->m_specular.raw[1], mat->m_specular.raw[2]);
    ars3dShaderUniformFloat(ctx->m_phong_shader, "u_material.m_shininess", mat->m_shininess);


    // Set the matrix uniforms and the camera position.
    ars3dShaderUniformMat4(ctx->m_phong_shader, "u_mvp", &mvp);
    ars3dShaderUniformMat4(ctx->m_phong_shader, "u_model", &obj->m_transform.m_model);
    ars3dShaderUniformMat4(ctx->m_phong_shader, "u_normal_mat", &obj->m_transform.m_normal);
    ars3dShaderUniformFloat3(ctx->m_phong_shader, "u_cam_pos", cam_pos.raw[0], cam_pos.raw[1], cam_pos.raw[2]);

    // Draw call.
    ars3dVertexArrayBind(obj->m_mesh->m_vao);
    ars3dGLRenderTriangles(obj->m_mesh->m_num_of_vertices);
    ars3dVertexArrayUnbind();
  }

  ars3dShaderUnbind();

  // Render Light Object.
  if (obj->m_type != GAMEOBJECT_TYPE_GENERAL && obj->m_material)
  {
    ars3dShaderBind(ctx->m_light_shader);

    // Set the material properties.
    // For a light, we just use a simple color, nothing else.
    Material *mat = obj->m_material;
    ars3dShaderUniformFloat3(ctx->m_light_shader, "u_light_color", mat->m_ambient.raw[0], mat->m_ambient.raw[1], mat->m_ambient.raw[2]);
    ars3dShaderUniformMat4(ctx->m_light_shader, "u_mvp", &mvp);


    // Draw call.
    ars3dVertexArrayBind(obj->m_mesh->m_vao);
    ars3dGLRenderTriangles(obj->m_mesh->m_num_of_vertices);
    ars3dVertexArrayUnbind();
    ars3dShaderUnbind();
  }

}

// Gerate a 10x10 grid (100 total in size).
void generateGridTerrain(float *grid_vertices)
{
    float size = 100.0f;                    // Total Terrain Size.
    float step = size / GRID_DIVISIONS;     // Size of each quad (10.0f)
    float start = -size / 2.0f;             // Start from the center of the terrain.
    float nx = 0.0f, ny = 1.0f, nz = 0.0f;  // Normal always looks UP for each quad.

    int index = 0;

    for (int z = 0; z < GRID_DIVISIONS; z++)
    {
      for (int x = 0; x < GRID_DIVISIONS; x++)
      {
        // Calculate the boundaries of the quad.
        float x_min = start + (x * step);
        float x_max = x_min + step;
        float z_min = start + (z * step);
        float z_max = z_min + step;


        // --- First Triangle ---
        // Vertex 1: Back Left
        grid_vertices[index++] = x_min; grid_vertices[index++] = 0.0f; grid_vertices[index++] = z_min;
        grid_vertices[index++] = nx;    grid_vertices[index++] = ny;    grid_vertices[index++] = nz;

        // Vertex 2: Back Right
        grid_vertices[index++] = x_max; grid_vertices[index++] = 0.0f; grid_vertices[index++] = z_min;
        grid_vertices[index++] = nx;    grid_vertices[index++] = ny;    grid_vertices[index++] = nz;

        // Vertex 3: Front Right
        grid_vertices[index++] = x_max; grid_vertices[index++] = 0.0f; grid_vertices[index++] = z_max;
        grid_vertices[index++] = nx;    grid_vertices[index++] = ny;    grid_vertices[index++] = nz;


        // --- Second Triangle ---
        // Vertex 4: Front Right
        grid_vertices[index++] = x_max; grid_vertices[index++] = 0.0f; grid_vertices[index++] = z_max;
        grid_vertices[index++] = nx;    grid_vertices[index++] = ny;    grid_vertices[index++] = nz;

        // Vertex 5: Front Left
        grid_vertices[index++] = x_min; grid_vertices[index++] = 0.0f; grid_vertices[index++] = z_max;
        grid_vertices[index++] = nx;    grid_vertices[index++] = ny;    grid_vertices[index++] = nz;

        // Vertex 6: Back Left
        grid_vertices[index++] = x_min; grid_vertices[index++] = 0.0f; grid_vertices[index++] = z_min;
        grid_vertices[index++] = nx;    grid_vertices[index++] = ny;    grid_vertices[index++] = nz;
      }
    }
}


void updateCelestialTrajectory(SandboxProject3Layer *ctx, float p_delta_time)
{
    // Reduce the angle to move from left to right (starting with PI)
    ctx->m_sun_theta -= 0.2f * p_delta_time;

    // Once the angle becomes <= 0 (WEST) we reset back to PI (east)
    if (ctx->m_sun_theta <= 0.0f)
    {
        ctx->m_sun_theta = GLM_PI;
    }

    // Some important variables.
    float radius = 100.0f;
    float cos_theta = cosf(ctx->m_sun_theta);
    float sin_theta = sinf(ctx->m_sun_theta);

    // X = R * cos(theta), Y = R * sin(theta), Z = 0.0f
    vec3s new_position = (vec3s){{ radius * cos_theta, radius * sin_theta, 0.0f }};

    // Update the sun_or_moon gameobject position.
    ctx->m_sun_or_moon.m_transform.m_position = new_position;

    // Controll the sun brigtness and the swap between SUN & MOON (DAY OR NIGHT).
    if (ctx->m_is_day)
    {
        // sun: Emission East Horizon (0.4, 0.4, 0.4) ---> Zenith (1.0, 1.0, 1.0) ---> West Horizon (0.4, 0.4. 0.4).
        // I will use sin_theta (0.0 East (PI),  1.0 Zenith (PI/2) -> 0.0 West (0) ).
        float intensity_factor  = 0.4f + 0.6f * sin_theta;
        vec3s sun_light_color   = (vec3s){{ intensity_factor, intensity_factor, intensity_factor }};

        // Update the point light of the m_sun_or_moon gameobject.
        ARS3D_ASSERT(ctx->m_sun_or_moon.m_type == GAMEOBJECT_TYPE_LIGHT_POINT);
        ctx->m_sun_or_moon.m_light.m_point.m_diffuse  = sun_light_color;
        ctx->m_sun_or_moon.m_light.m_point.m_specular = sun_light_color;

        // Set the SUN material.
        ctx->m_sun_or_moon.m_material = ctx->m_sun_mat;
    }

    // Moon.
    else
    {
        // Moon has a fixed Emission (0.5, 0.5, 0.5)
        vec3s moon_light_color = (vec3s){{ 0.5f, 0.5f, 0.5f }};

        // Set the point light colors.
        ARS3D_ASSERT(ctx->m_sun_or_moon.m_type == GAMEOBJECT_TYPE_LIGHT_POINT);
        ctx->m_sun_or_moon.m_light.m_point.m_diffuse  = moon_light_color;
        ctx->m_sun_or_moon.m_light.m_point.m_specular = moon_light_color;

        // Set the MOON material.
        ctx->m_sun_or_moon.m_material = ctx->m_moon_mat;
    }

    // Make the transform dirty, to force recalculation of the Matrices.
    ctx->m_sun_or_moon.m_transform.m_is_dirty = 1;
}


void uiProject3Run(SandboxProject3Layer *ctx)
{
  mu_Context *mu = ars3dUIGetContext();

  // Create a new UI window.
  if (mu_begin_window(mu, "Project 3 Controls", mu_rect(50, 50, 300, 260)))
  {
    mu_layout_row(mu, 1, (int[]) { -1 }, 0);


    mu_label(mu, "Camera Mode:");

    char cam_mode_text[32];
    sprintf(cam_mode_text, "Current Mode: %s", ctx->m_is_camera_orbiting ? "ORBIT" : "FLY");

    if (mu_button(mu, cam_mode_text))
    {
      ctx->m_is_camera_orbiting = !ctx->m_is_camera_orbiting;

      // 1. Clean up του controller input για να μην έχουμε ghost κινήσεις
      ctx->m_cam_controller.m_direction_x = 0.0f;
      ctx->m_cam_controller.m_direction_y = 0.0f;
      ctx->m_cam_controller.m_direction_z = 0.0f;
      ctx->m_cam_controller.m_direction_yaw = 0.0f;
      ctx->m_cam_controller.m_direction_pitch = 0.0f;

      if (ctx->m_is_camera_orbiting)
      {
        // -------------------------------------------------------------------------
        // ΜΕΤΑΒΑΣΗ: FLY -> ORBIT
        // Η νέα ακτίνα πρέπει να είναι η τρέχουσα απόσταση της κάμερας από το κέντρο (0,0,0)
        // -------------------------------------------------------------------------
        ctx->m_orbit_radius = glms_vec3_norm(ctx->m_camera->m_position);
        
        // Ασφάλεια για να μην μηδενιστεί η ακτίνα αν είμαστε στο (0,0,0)
        if (ctx->m_orbit_radius < 1.0f) ctx->m_orbit_radius = 5.0f;

        // Υπολογίζουμε τη γωνία (yaw/pitch) με βάση τη θέση που βρισκόταν η Fly κάμερα
        // ώστε να μην κάνει "πήδημα" (snap) σε άσχετο σημείο της σφαίρας
        float x = ctx->m_camera->m_position.raw[0];
        float y = ctx->m_camera->m_position.raw[1];
        float z = ctx->m_camera->m_position.raw[2];

        ctx->m_camera->m_pitch = glm_deg(asinf(y / ctx->m_orbit_radius));
        ctx->m_camera->m_yaw   = glm_deg(atan2f(z, x));

        // Αναγκάζουμε την Orbit να κάτσει ακριβώς εκεί και να κοιτάξει το κέντρο
        cameraSystemOrbit(ctx->m_camera, 0.0f, 0.0f, ctx->m_orbit_radius);
      }
      else
      {
        // -------------------------------------------------------------------------
        // ΜΕΤΑΒΑΣΗ: ORBIT -> FLY
        // Πρέπει να μετατρέψουμε το m_front της Orbit (που κοιτάζει το κέντρο)
        // σε σωστά yaw και pitch που καταλαβαίνει το Fly Mode.
        // -------------------------------------------------------------------------
        vec3s front = ctx->m_camera->m_front;

        ctx->m_camera->m_pitch = glm_deg(asinf(front.raw[1]));
        ctx->m_camera->m_yaw   = glm_deg(atan2f(front.raw[2], front.raw[0]));

        // Επαναϋπολογισμός των αξόνων της Fly κάμερας με βάση τα νέα yaw/pitch
        // Χρησιμοποιούμε δέλτα 0 για να μην κουνηθεί, απλά να γίνουν calibrate οι άξονες
        cameraSystemFly(ctx->m_camera, (vec3s){{0.0f, 0.0f, 0.0f}}, 0.0f, 0.0f);
      }
    }

    mu_label(mu, "Environment Lighting:");

    // Change the DAY/NIGHT state.
    char day_night_text[32];
    sprintf(day_night_text, "Time of Day: %s", ctx->m_is_day ? "DAY" : "NIGHT");
    if (mu_button(mu, day_night_text))
    {
      ctx->m_is_day = !ctx->m_is_day;
    }


    // Select floor mesh.
    mu_label(mu, "Floor Mesh Selection:");

    char floor_text[40];
    int is_grid = (ctx->m_floor.m_mesh == ctx->m_grid_mesh);
    sprintf(floor_text, "Current Floor: %s", is_grid ? "Terrain Grid" : "Single Plane");

    if (mu_button(mu, floor_text))
    {
      if (is_grid)
      {
        ctx->m_floor.m_mesh = ctx->m_single_plane_mesh;
      }
      else
      {
        ctx->m_floor.m_mesh = ctx->m_grid_mesh;
      }
      // Επειδή αλλάξαμε mesh, "βρωμίζουμε" το transform για σιγουριά (re-render/re-calculate αν χρειαστεί)
      ctx->m_floor.m_transform.m_is_dirty = 1;
    }


    // Flashlight cone slider.
    mu_label(mu, "Flashlight Cone Angle:");


    // Get the inner and outer angles in degrees.
    float current_inner_deg = glm_deg(acosf(ctx->m_flashlight.m_light.m_spot.m_inner_cutoff_cos));
    float current_outer_deg = glm_deg(acosf(ctx->m_flashlight.m_light.m_spot.m_outer_cutoff_cos));
    float previous_inner    = current_inner_deg;

    mu_label(mu, "Inner Angle (Degrees):");
    mu_slider(mu, &current_inner_deg, 1.0f, 45.0f); // Όριο από 1 έως 45 μοίρες

    if (current_inner_deg != previous_inner)
    {
      current_outer_deg = current_inner_deg + 5.0f;

      // Save the new cosine values.
      ctx->m_flashlight.m_light.m_spot.m_inner_cutoff_cos = cosf(glm_rad(current_inner_deg));
      ctx->m_flashlight.m_light.m_spot.m_outer_cutoff_cos = cosf(glm_rad(current_outer_deg));
    }

    mu_end_window(mu);
  }
}


// Normalize the point to "sit" directly into a spherical surface.
point3 normalizePoint(point3 p)
{
  float len = sqrtf(p.x * p.x + p.y * p.y + p.z * p.z);
  if (len > 0.0f)
  {
    p.x /= len;
    p.y /= len;
    p.z /= len;
  }
  return p;
}



// Triangle Subdivizion.
void subdivide(point3 v1, point3 v2, point3 v3, int depth, float *out_buffer, int *p_index)
{
  if (depth == 0)
  {
      // Base case: Φτάσαμε στο βάθος 4, αποθηκεύουμε το τελικό τρίγωνο στο buffer μας
      // Vertex 1
      out_buffer[(*p_index)++] = v1.x;
      out_buffer[(*p_index)++] = v1.y;
      out_buffer[(*p_index)++] = v1.z;

      // Vertex 2
      out_buffer[(*p_index)++] = v2.x;
      out_buffer[(*p_index)++] = v2.y;
      out_buffer[(*p_index)++] = v2.z;

      // Vertex 3
      out_buffer[(*p_index)++] = v3.x;
      out_buffer[(*p_index)++] = v3.y;
      out_buffer[(*p_index)++] = v3.z;
      return;
  }

  // Calculate the new edges.
  point3 v12 = { (v1.x + v2.x) * 0.5f, (v1.y + v2.y) * 0.5f, (v1.z + v2.z) * 0.5f };
  point3 v23 = { (v2.x + v3.x) * 0.5f, (v2.y + v3.y) * 0.5f, (v2.z + v3.z) * 0.5f };
  point3 v31 = { (v3.x + v1.x) * 0.5f, (v3.y + v1.y) * 0.5f, (v3.z + v1.z) * 0.5f };

  // Normalize the new vertices to sit on the surface of a sphere.
  v12 = normalizePoint(v12);
  v23 = normalizePoint(v23);
  v31 = normalizePoint(v31);

  // Recurse on the new 4 triangles.
  subdivide(v1,  v12, v31, depth - 1, out_buffer, p_index);
  subdivide(v12, v2,  v23, depth - 1, out_buffer, p_index);
  subdivide(v31, v23, v3,  depth - 1, out_buffer, p_index);
  subdivide(v12, v23, v31, depth - 1, out_buffer, p_index);
}


// Generates a sphere by subdividing triangles.
void generateSubdividedSphere(float *out_buffer, int *p_vertex_count)
{
  // Project 3 DATA.
  point3 v[4] =
  {
    { 0.0f,       0.0f,       1.0f },
    { 0.0f,       0.942809f, -0.333333f },
    {-0.816497f, -0.471405f, -0.333333f },
    { 0.816497f, -0.471405f, -0.333333f }
  };

  int float_index = 0;
  int depth = 4;

  // Subdivide each triangle recursivly.
  subdivide(v[0], v[1], v[2], depth, out_buffer, &float_index);
  subdivide(v[0], v[2], v[3], depth, out_buffer, &float_index);
  subdivide(v[0], v[3], v[1], depth, out_buffer, &float_index);
  subdivide(v[3], v[2], v[1], depth, out_buffer, &float_index);

  // Store the number of vertices.
  *p_vertex_count = float_index / 3;
}

