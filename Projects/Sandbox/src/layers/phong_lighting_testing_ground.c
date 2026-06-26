#include "phong_lighting_testing_ground.h"

// Cube vertex data.
ars3d_float cube_data[] =
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


ars3d_void onPhongLightingTestingGroundLayerAttach(ars3d_void *p_ctx)
{
  PhongLightingTestingGroundLayer *ctx = (PhongLightingTestingGroundLayer *)p_ctx;

  // Get the window width and height.
  ars3d_int screen_width, screen_height;
  ars3dWindowGetSize(
      ars3dAppGetWindow(ars3dAppGet()),
      &screen_width,
      &screen_height
  );

  ctx->m_camera     = cameraSystemCreate((vec3s){{0.0f, 0.0f, 1.0f}}, 0.0f, -90.0f);
  ctx->m_projection = glms_perspective(glm_rad(45.0f), (float)screen_width / (float)screen_height, 0.1f, 100.0f);
  ctx->m_cube       = ars3dGameObjectCreate(
    (vec3s){{0.0f, 0.0f, -4.0f}},
    (vec3s){{0.0f, 0.0f, 0.0f}},
    (vec3s){{1.0f, 1.0f, 1.0f}}
  );
  ctx->m_light      = ars3dGameObjectCreate(
    (vec3s){{0.0f, 4.0f, -4.0f}},
    (vec3s){{0.0f, 0.0f, 0.0f}},
    (vec3s){{0.5f, 0.5f, 0.5f}}
  );

  // TODO: Replace these hardcoded paths with a custom path system to dynamically
  // generate the correct file paths, based on project scope based or standalone
  // bundle.
  ctx->m_shader   = ars3dShaderCreateFromFile(
      "Projects/Sandbox/src/shaders/phong.vertex.glsl",
      "Projects/Sandbox/src/shaders/phong.fragment.glsl"
  );

  ctx->m_light_shader = ars3dShaderCreateFromFile(
      "Projects/Sandbox/src/shaders/light.vertex.glsl",
      "Projects/Sandbox/src/shaders/light.fragment.glsl"
  );

  // Initialize the camera controller.
  cameraControllerStackInit(&ctx->m_cam_controller, ctx->m_camera);
}

ars3d_void onPhongLightingTestingGroundLayerDetatch(ars3d_void *p_ctx)
{
  PhongLightingTestingGroundLayer *ctx = (PhongLightingTestingGroundLayer *)p_ctx;

  // Destroy the context data.
  ars3dShaderDestroy(&ctx->m_shader);
  ars3dShaderDestroy(&ctx->m_light_shader);
  ars3dGameObjectDestroy(&ctx->m_cube);
  ars3dGameObjectDestroy(&ctx->m_light);
  cameraSystemDestroy(&ctx->m_camera);

  // Destroy the context itself.
  ars3dFree(ctx);
}

ars3d_void onPhongLightingTestingGroundLayerStart(ars3d_void *p_ctx)
{
  PhongLightingTestingGroundLayer * const ctx = (PhongLightingTestingGroundLayer *)p_ctx;


  // Create the mesh for the cube.
  ars3dGameObjectSetMesh(
      ctx->m_cube,
      ars3dMeshCreate(
        cube_data,
        ARS3D_SIZEOF(cube_data),
        (Ars3DMeshAttribute[])
        {
          {"Positions", ARS3D_VERTEX_ATTRIBUTE_FLOAT, 3},
          {"Normals", ARS3D_VERTEX_ATTRIBUTE_FLOAT, 3}
        },
        2
      )
  );


  // Create the mesh for the light.
  ars3dGameObjectSetMesh(
      ctx->m_light,
      ars3dMeshCreate(
        cube_data,
        ARS3D_SIZEOF(cube_data),
        (Ars3DMeshAttribute[])
        {
          {"Positions", ARS3D_VERTEX_ATTRIBUTE_FLOAT, 3},
          {"Normals", ARS3D_VERTEX_ATTRIBUTE_FLOAT, 3},
        },
        2
      )
  );

  vec3s light_pos = ars3dGameObjectGetPosition(ctx->m_light);

  ars3dShaderBind(ctx->m_shader);

  // Material.
  ars3dShaderUniformFloat3(ctx->m_shader, "u_material.m_ambient", 1.0f, 0.0f, 0.0f);
  ars3dShaderUniformFloat3(ctx->m_shader, "u_material.m_diffuse", 1.0f, 0.0f, 0.0f);
  ars3dShaderUniformFloat3(ctx->m_shader, "u_material.m_specular", 1.0f, 0.0f, 0.0f);
  ars3dShaderUniformFloat(ctx->m_shader, "u_material.m_shininess", 64.0f);

  // Ambient Light.
  ars3dShaderUniformFloat(ctx->m_shader, "u_ambient.m_strength", 0.2f);
  ars3dShaderUniformFloat3(ctx->m_shader, "u_ambient.m_ambient", 1.0f, 1.0f, 1.0f);

  // Point Light.
  ars3dShaderUniformFloat(ctx->m_shader, "u_point_light.m_strength", 1.0f);
  ars3dShaderUniformFloat3(ctx->m_shader, "u_point_light.m_position", light_pos.raw[0], light_pos.raw[1], light_pos.raw[2]);
  ars3dShaderUniformFloat3(ctx->m_shader, "u_point_light.m_diffuse", 1.0f, 1.0f, 1.0f);
  ars3dShaderUniformFloat3(ctx->m_shader, "u_point_light.m_specular", 1.0f, 1.0f, 1.0f);
  ars3dShaderUniformFloat(ctx->m_shader, "u_point_light.m_attenuation.m_constant", 1.0f);
  ars3dShaderUniformFloat(ctx->m_shader, "u_point_light.m_attenuation.m_linear", 0.045f);
  ars3dShaderUniformFloat(ctx->m_shader, "u_point_light.m_attenuation.m_quadratic", 0.0075f);

  // Spot Light.
  ars3dShaderUniformFloat(ctx->m_shader, "u_spot_light.m_inner_cutoff_cos", cosf(glm_rad(12.5f)));
  ars3dShaderUniformFloat(ctx->m_shader, "u_spot_light.m_outer_cutoff_cos", cosf(glm_rad(17.5f)));
  ars3dShaderUniformFloat3(ctx->m_shader, "u_spot_light.m_direction", 0.0f, 0.0f, 0.0f);
  ars3dShaderUniformFloat(ctx->m_shader, "u_spot_light.m_light.m_strength", 0.5f);
  ars3dShaderUniformFloat3(ctx->m_shader, "u_spot_light.m_light.m_position", light_pos.raw[0], light_pos.raw[1], light_pos.raw[2]);
  ars3dShaderUniformFloat3(ctx->m_shader, "u_spot_light.m_light.m_diffuse", 1.0f, 1.0f, 1.0f);
  ars3dShaderUniformFloat3(ctx->m_shader, "u_spot_light.m_light.m_specular", 1.0f, 1.0f, 1.0f);
  ars3dShaderUniformFloat(ctx->m_shader, "u_spot_light.m_light.m_attenuation.m_constant", 1.0f);
  ars3dShaderUniformFloat(ctx->m_shader, "u_spot_light.m_light.m_attenuation.m_linear", 0.045f);
  ars3dShaderUniformFloat(ctx->m_shader, "u_spot_light.m_light.m_attenuation.m_quadratic", 0.0075f);


  ars3dShaderUnbind();
}

ars3d_void onPhongLightingTestingGroundLayerUpdate(ars3d_void *p_ctx, ars3d_float p_delta_time)
{
  PhongLightingTestingGroundLayer * const ctx = (PhongLightingTestingGroundLayer *)p_ctx;

  ars3dGameObjectUpdateRotation(
      ctx->m_cube,
      (vec3s){{p_delta_time * 50.0f, p_delta_time * 50.0f, 0.0f}}
  );

  // Update Camera Movement.
  cameraControllerFly(&ctx->m_cam_controller, p_delta_time);

  mat4s view  = cameraSystemGetViewMatrix(ctx->m_camera);

  // Calculate the full model matrix.
  mat4s model = ars3dGameObjectGetModelMatrix(ctx->m_cube);

  // Calculate the model view projection matrix.
  mat4s u_mvp = glms_mul(ctx->m_projection, view);
  u_mvp       = glms_mul(u_mvp, model);

  // u_normal_mat     = [ model^{-1} ]^{T}
  mat4s u_normal_mat  = glms_mat4_inv(model);
  u_normal_mat        = glms_mat4_transpose(u_normal_mat);


  mat4s model_light = ars3dGameObjectGetModelMatrix(ctx->m_light);


  // Calculate the ligth full model view projection matrix.
  mat4s u_mvp_light = glms_mul(ctx->m_projection, view);
  u_mvp_light       = glms_mul(u_mvp_light, model_light);


  vec3s cam_pos   = cameraSystemGetPosition(ctx->m_camera);
  vec3s cam_front = cameraSystemGetFront(ctx->m_camera);


  // ------------------------Draw The Material Cube------------------------ //
  // Bind the required GL data states.
  ars3dVertexArrayBind(ars3dMeshGetVertexArray(ars3dGameObjectGetMesh(ctx->m_cube)));
  ars3dShaderBind(ctx->m_shader);

  // Upload uniforms.
  ars3dShaderUniformMat4(ctx->m_shader, "u_mvp", &u_mvp);
  ars3dShaderUniformMat4(ctx->m_shader, "u_model", &model);
  ars3dShaderUniformMat4(ctx->m_shader, "u_normal_mat", &u_normal_mat);
  ars3dShaderUniformFloat3(ctx->m_shader, "u_cam_pos", cam_pos.raw[0], cam_pos.raw[1], cam_pos.raw[2]);
  ars3dShaderUniformFloat3(ctx->m_shader, "u_spot_light.m_direction", cam_front.raw[0], cam_front.raw[1], cam_front.raw[2]);
  ars3dShaderUniformFloat3(ctx->m_shader, "u_spot_light.m_light.m_position", cam_pos.raw[0], cam_pos.raw[1], cam_pos.raw[2]);

  // Enable GL specific functionality and render the triangles.
  ars3dGLEnableDepthTest(1);
  ars3dGLRenderTriangles(36);

  // Unbind GL state.
  ars3dVertexArrayUnbind();
  ars3dShaderUnbind();
  // ------------------------Draw The Material Cube------------------------ //

  // ------------------------Draw The Light Cube------------------------ //

  // Bind the required GL data states.
  ars3dVertexArrayBind(ars3dMeshGetVertexArray(ars3dGameObjectGetMesh(ctx->m_light)));
  ars3dShaderBind(ctx->m_light_shader);

  // Upload uniforms.
  ars3dShaderUniformMat4(ctx->m_light_shader, "u_mvp", &u_mvp_light);
  ars3dShaderUniformFloat3(ctx->m_light_shader, "u_light_color", 1.0f, 1.0f, 1.0f);

  // Enable GL specific functionality and render the triangles.
  ars3dGLEnableDepthTest(1);
  ars3dGLRenderTriangles(36);

  // Unbind GL state.
  ars3dVertexArrayUnbind();
  ars3dShaderUnbind();
  // ------------------------Draw The Light Cube------------------------ //
}

ars3d_uchar onPhongLightingTestingGroundLayerEvent(ars3d_void *p_ctx, ars3d_void *p_event, ars3d_int p_event_type)
{
  PhongLightingTestingGroundLayer * const ctx = (PhongLightingTestingGroundLayer *)p_ctx;

  switch (p_event_type)
  {
    // Update the projection matrix each time the window resizes.
    case ARS3D_EVENT_TYPE_WINDOW_RESIZED:
    {
      ars3d_int width, height;
      ars3dEventGetWindowSize(p_event, &width, &height);
      ctx->m_projection = glms_perspective(glm_rad(45.0f), (float)width/(float)height, 0.1f, 100.0f);
      break;
    }

    default:
      break;
  }

  // Call the camera controller event handler.
  cameraControllerEventHandler(&ctx->m_cam_controller, p_event, p_event_type);

  return 0;
}


PhongLightingTestingGroundLayer *phongLightingTestingGroundLayerCreate()
{
  PhongLightingTestingGroundLayer *new_layer
    = (PhongLightingTestingGroundLayer *)ars3dMalloc( ARS3D_SIZEOF(PhongLightingTestingGroundLayer) );

  if (!new_layer)
  {
    ARS3D_WARN("Ouf of memory.");
    return ARS3D_NULL;
  }

  return new_layer;
}
