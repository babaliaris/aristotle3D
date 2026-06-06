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

  ctx->m_vao        = ars3dVertexArrayCreate();
  ctx->m_vbo        = ars3dVertexBufferCreate();
  ctx->m_attribs    = ars3dVertexAttributesCreate();
  ctx->m_camera     = cameraSystemCreate((vec3s){{0.0f, 0.0f, 1.0f}});
  ctx->m_projection = glms_perspective(glm_rad(45.0f), (float)screen_width / (float)screen_height, 0.1f, 100.0f);
  ctx->m_cube.m_position  = (vec3s){{0.0f, 0.0f, -4.0f}};
  ctx->m_cube.m_rotation  = (vec3s){{0.0f, 0.0f, 0.0f}};
  ctx->m_cube.m_scale     = (vec3s){{1.0f, 1.0f, 1.0f}};
  ctx->m_light.m_position  = (vec3s){{0.0f, 4.0f, -4.0f}};
  ctx->m_light.m_rotation  = (vec3s){{0.0f, 0.0f, 0.0f}};
  ctx->m_light.m_scale     = (vec3s){{0.5f, 0.5f, 0.5f}};

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

  ars3dVertexArrayBind(ctx->m_vao);
  ars3dVertexBufferBind(ctx->m_vbo);
  ars3dVertexBufferData(ctx->m_vbo, ARS3D_SIZEOF(cube_data), cube_data, ARS3D_VBO_STATIC_DRAW);

  ars3dVertexAttributesPush(ctx->m_attribs, "Positions", ARS3D_VERTEX_ATTRIBUTE_FLOAT, 3);
  ars3dVertexAttributesPush(ctx->m_attribs, "Normals", ARS3D_VERTEX_ATTRIBUTE_FLOAT, 3);
  ars3dVertexAttributesBind(ctx->m_attribs);

  ars3dVertexBufferUnbind();
  ars3dVertexArrayUnbind();
}

ars3d_void onPhongLightingTestingGroundLayerDetatch(ars3d_void *p_ctx)
{
  PhongLightingTestingGroundLayer *ctx = (PhongLightingTestingGroundLayer *)p_ctx;

  // Destroy the context data.
  ars3dVertexArrayDestroy(&ctx->m_vao);
  ars3dVertexBufferDestroy(&ctx->m_vbo);
  ars3dVertexAttributesDestroy(&ctx->m_attribs);
  ars3dShaderDestroy(&ctx->m_shader);
  ars3dShaderDestroy(&ctx->m_light_shader);
  cameraSystemDestroy(&ctx->m_camera);

  // Destroy the context itself.
  ars3dFree(ctx);
}

ars3d_void onPhongLightingTestingGroundLayerStart(ars3d_void *p_ctx)
{
  PhongLightingTestingGroundLayer * const ctx = (PhongLightingTestingGroundLayer *)p_ctx;

  ars3dShaderBind(ctx->m_shader);

  // Material.
  ars3dShaderUniformFloat3(ctx->m_shader, "u_material.m_ambient", 1.0f, 0.0f, 0.0f);
  ars3dShaderUniformFloat3(ctx->m_shader, "u_material.m_diffuse", 1.0f, 0.0f, 0.0f);
  ars3dShaderUniformFloat3(ctx->m_shader, "u_material.m_specular", 1.0f, 0.0f, 0.0f);
  ars3dShaderUniformFloat(ctx->m_shader, "u_material.m_shininess", 64.0f);

  // Ambient Light.
  ars3dShaderUniformFloat(ctx->m_shader, "u_ambient.m_strength", 0.2f);
  ars3dShaderUniformFloat3(ctx->m_shader, "u_ambient.m_position", 0.0f, 0.0f, 0.0f);
  ars3dShaderUniformFloat3(ctx->m_shader, "u_ambient.m_ambient", 1.0f, 1.0f, 1.0f);
  ars3dShaderUniformFloat3(ctx->m_shader, "u_ambient.m_diffuse", 1.0f, 1.0f, 1.0f);
  ars3dShaderUniformFloat3(ctx->m_shader, "u_ambient.m_specular", 1.0f, 1.0f, 1.0f);
  ars3dShaderUniformFloat(ctx->m_shader, "u_ambient.m_attenuation.m_constant", 0.0f);
  ars3dShaderUniformFloat(ctx->m_shader, "u_ambient.m_attenuation.m_linear", 0.0f);
  ars3dShaderUniformFloat(ctx->m_shader, "u_ambient.m_attenuation.m_quadratic", 0.0f);

  // Spot Light.
  ars3dShaderUniformFloat(ctx->m_shader, "u_point_light.m_strength", 1.0f);
  ars3dShaderUniformFloat3(ctx->m_shader, "u_point_light.m_position", ctx->m_light.m_position.raw[0], ctx->m_light.m_position.raw[1], ctx->m_light.m_position.raw[2]);
  ars3dShaderUniformFloat3(ctx->m_shader, "u_point_light.m_ambient", 1.0f, 1.0f, 1.0f);
  ars3dShaderUniformFloat3(ctx->m_shader, "u_point_light.m_diffuse", 1.0f, 1.0f, 1.0f);
  ars3dShaderUniformFloat3(ctx->m_shader, "u_point_light.m_specular", 1.0f, 1.0f, 1.0f);
  ars3dShaderUniformFloat(ctx->m_shader, "u_point_light.m_attenuation.m_constant", 1.0f);
  ars3dShaderUniformFloat(ctx->m_shader, "u_point_light.m_attenuation.m_linear", 0.045f);
  ars3dShaderUniformFloat(ctx->m_shader, "u_point_light.m_attenuation.m_quadratic", 0.0075f);

  ars3dShaderUnbind();
}

ars3d_void onPhongLightingTestingGroundLayerUpdate(ars3d_void *p_ctx, ars3d_float p_delta_time)
{
  PhongLightingTestingGroundLayer * const ctx = (PhongLightingTestingGroundLayer *)p_ctx;

  ctx->m_cube.m_rotation.raw[0] += p_delta_time * 50.0f;
  ctx->m_cube.m_rotation.raw[1] += p_delta_time * 50.0f;

  // Update Camera Movement.
  cameraControllerFly(&ctx->m_cam_controller, p_delta_time);

  mat4s view  = cameraSystemGetViewMatrix(ctx->m_camera);

  // Calculate the full model matrix.
  mat4s model = glms_mat4_identity();
  model = glms_translate(model, ctx->m_cube.m_position);
  model = glms_rotate_x(model, glm_rad(ctx->m_cube.m_rotation.raw[0]));
  model = glms_rotate_y(model, glm_rad(ctx->m_cube.m_rotation.raw[1]));
  model = glms_rotate_z(model, glm_rad(ctx->m_cube.m_rotation.raw[2]));
  model = glms_scale(model, ctx->m_cube.m_scale);

  // Calculate the model view projection matrix.
  mat4s u_mvp = glms_mul(ctx->m_projection, view);
  u_mvp       = glms_mul(u_mvp, model);

  // u_normal_mat     = [ model^{-1} ]^{T}
  mat4s u_normal_mat  = glms_mat4_inv(model);
  u_normal_mat        = glms_mat4_transpose(u_normal_mat);


  mat4s model_light = glms_mat4_identity();

  // Calculate the light model matrix.
  model_light = glms_translate(model_light, ctx->m_light.m_position);
  model_light = glms_scale(model_light, ctx->m_light.m_scale);

  // Calculate the ligth full model view projection matrix.
  mat4s u_mvp_light = glms_mul(ctx->m_projection, view);
  u_mvp_light       = glms_mul(u_mvp_light, model_light);


  // ------------------------Draw The Material Cube------------------------ //
  // Bind the required GL data states.
  ars3dVertexArrayBind(ctx->m_vao);
  ars3dShaderBind(ctx->m_shader);

  // Upload uniforms.
  ars3dShaderUniformMat4(ctx->m_shader, "u_mvp", &u_mvp);
  ars3dShaderUniformMat4(ctx->m_shader, "u_model", &model);
  ars3dShaderUniformMat4(ctx->m_shader, "u_normal_mat", &u_normal_mat);

  // Enable GL specific functionality and render the triangles.
  ars3dGLEnableDepthTest(1);
  ars3dGLRenderTriangles(36);

  // Unbind GL state.
  ars3dVertexArrayUnbind();
  ars3dShaderUnbind();
  // ------------------------Draw The Material Cube------------------------ //

  // ------------------------Draw The Light Cube------------------------ //

  // Bind the required GL data states.
  ars3dVertexArrayBind(ctx->m_vao);
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
