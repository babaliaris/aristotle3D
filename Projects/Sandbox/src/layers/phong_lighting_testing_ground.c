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
  ctx->m_projection = glms_perspective(glm_rad(45.0f), (float)screen_width / (float)screen_height, 0.1f, 100.0f);
  ctx->m_cube.m_position  = (vec3s){{0.0f, 0.0f, -4.0f}};
  ctx->m_cube.m_rotation  = (vec3s){{0.0f, 0.0f, 0.0f}};
  ctx->m_cube.m_scale     = (vec3s){{1.0f, 1.0f, 1.0f}};

  // TODO: Replace these hardcoded paths with a custom path system to dynamically
  // generate the correct file paths, based on project scope based or standalone
  // bundle.
  ctx->m_shader   = ars3dShaderCreateFromFile(
      "Projects/Sandbox/src/shaders/phong.vertex.glsl",
      "Projects/Sandbox/src/shaders/phong.fragment.glsl"
  );

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

  // Destroy the context itself.
  ars3dFree(ctx);
}

ars3d_void onPhongLightingTestingGroundLayerStart(ars3d_void *p_ctx)
{
  ARS3D_UNUSED(p_ctx);
  ARS3D_INFO("[PhongLightingTestingGroundLayer]: onStart()");
}

ars3d_void onPhongLightingTestingGroundLayerUpdate(ars3d_void *p_ctx, ars3d_float p_delta_time)
{
  ARS3D_UNUSED(p_delta_time);
  PhongLightingTestingGroundLayer * const ctx = (PhongLightingTestingGroundLayer *)p_ctx;

  ctx->m_cube.m_rotation.raw[0] += p_delta_time * 50.0f;
  ctx->m_cube.m_rotation.raw[1] += p_delta_time * 50.0f;

  mat4s model = glms_mat4_identity();
  mat4s view  = glms_mat4_identity();

  // Calculate the full model matrix.
  model = glms_translate(model, ctx->m_cube.m_position);
  model = glms_rotate_x(model, glm_rad(ctx->m_cube.m_rotation.raw[0]));
  model = glms_rotate_y(model, glm_rad(ctx->m_cube.m_rotation.raw[1]));
  model = glms_rotate_z(model, glm_rad(ctx->m_cube.m_rotation.raw[2]));
  model = glms_scale(model, ctx->m_cube.m_scale);

  // Calculate the model view projection matrix.
  mat4s u_mvp = glms_mul(ctx->m_projection, view);
  u_mvp       = glms_mul(u_mvp, model);

  // Bind the required GL data states.
  ars3dVertexArrayBind(ctx->m_vao);
  ars3dShaderBind(ctx->m_shader);

  // Upload uniforms.
  ars3dShaderUniformMat4(ctx->m_shader, "u_mvp", &u_mvp);

  // Enable GL specific functionality and render the triangles.
  ars3dGLEnableDepthTest(1);
  ars3dGLRenderTriangles(36);

  // Unbind GL state.
  ars3dVertexArrayUnbind();
  ars3dShaderUnbind();
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
