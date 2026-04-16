#include "cube.h"
#include <math.h>


ars3d_void runUI(CubeLayerCtx *ctx)
{
  mu_Context *mu = ars3dUIGetContext();

  if (mu_begin_window(mu, "Cube Controls", mu_rect(20, 20, 220, 280)))
  {
    mu_layout_row(mu, 1, (int[]) { -1 }, 0);
    mu_label(mu, "Select Mode:");

    if (mu_button(mu, "Mode A: Pulse")) ctx->m_run_mode = CUBE_LAYER_RUN_MODE_A;
    if (mu_button(mu, "Mode B: Shear")) ctx->m_run_mode = CUBE_LAYER_RUN_MODE_B;
    if (mu_button(mu, "Mode C: Orbit")) ctx->m_run_mode = CUBE_LAYER_RUN_MODE_C;

    // Slider για την ταχύτητα περιστροφής (από 0x έως 5x ταχύτητα)
    mu_label(mu, "Rotation Speed:");
    mu_slider(mu, &ctx->m_cube_data.m_rotation_speed, 0.0f, 10.0f);

    // Slider για την ταχύτητα του Pulse
    mu_label(mu, "Pulse Speed:");
    mu_slider(mu, &ctx->m_cube_data.m_pulse_speed, 0.0f, 10.0f);

    mu_end_window(mu);
  }
}



// Square defined in the x=y plane.
float square_vertices[] =
{
  // 1 / sqrt(2) = 0.707106

  // Triangle 1
  0.707106f,  0.707106f,  1.0f,     // V1
  0.707106f,  0.707106f,  -1.0f,    // V2
  -0.707106f, -0.707106f, -1.0f,    // V3

  // Triangle 2
  -0.707106f,  -0.707106f, -1.0f, // V3
  -0.707106f, -0.707106f, 1.0f,  // V4
  0.707106f,  0.707106f,  1.0f   // V1
};

const char *vertex_shader_src =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;"
"uniform mat4 u_model, u_view, u_proj;"
"void main()"
"{"
    "gl_Position = u_proj * u_view * u_model * vec4(aPos, 1.0);"
"}";


const char *fragment_shader_src =
"#version 330 core\n"
"uniform vec3 u_color;"
"out vec4 fragColor;"
"void main()"
"{"
    "fragColor = vec4(u_color, 1.0);"
"}";


mat4s calculateProjection(ars3d_void *p_event)
{
  int screen_width, screen_height;

  if ( !p_event )
  {
    ars3dWindowGetSize(
        ars3dAppGetWindow(ars3dAppGet()),
        &screen_width,
        &screen_height
    );
  }

  else
  {
    ars3dEventGetWindowSize(p_event, &screen_width, &screen_height);
  }

  float aspect  = (float)screen_width / (float)screen_height;


  float size    = 8.0f; // This determines "zoomed in"

  return glms_ortho(
      -size * aspect,
      size * aspect,
      -size,
      size,
      -10.0f,
      400.f
  );
}



ars3d_void cubeLayerOnAttach(ars3d_void *p_ctx)
{
  CubeLayerCtx *ctx = (CubeLayerCtx *)p_ctx;

  // Cube data.
  ctx->m_cube_data.m_rotate_angle   = 0.0f;
  ctx->m_cube_data.m_scale_factor   = 1.0f;
  ctx->m_cube_data.m_pulse_angle    = 0.0f;
  ctx->m_cube_data.m_camera_pos     = (vec3s){{0.0f, 0.0f, 0.0f}};
  ctx->m_cube_data.m_rotation_speed = 1.0f;
  ctx->m_cube_data.m_pulse_speed    = 0.95f * CUBE_LAYER_A;

  // Cube Transform Vectors.
  ctx->m_cube_data.m_transform.m_position   = (vec3s){{0.0f, 0.0f, -CUBE_LAYER_RUN_MODE_B}};
  ctx->m_cube_data.m_transform.m_rotation   = (vec3s){{CUBE_LAYER_VX, CUBE_LAYER_VY, CUBE_LAYER_VZ}};
  ctx->m_cube_data.m_transform.m_scale      = (vec3s){{1.0f, 1.0f, 1.0f}};

  // Cube Transform Matrices.
  ctx->m_cube_data.m_transform.m_model      = glms_mat4_identity();
  ctx->m_cube_data.m_transform.m_view       = glms_translate(glms_mat4_identity(), ctx->m_cube_data.m_camera_pos);
  ctx->m_cube_data.m_transform.m_projection = calculateProjection(ARS3D_NULL);

  // Layer Context Graphics Data.
  ctx->m_vao      = ars3dVertexArrayCreate();
  ctx->m_vbo      = ars3dVertexBufferCreate();
  ctx->m_attribs  = ars3dVertexAttributesCreate();
  ctx->m_shader   = ars3dShaderCreateFromStrings(
      "Cube Shader",
      vertex_shader_src,
      ARS3D_NULL,
      fragment_shader_src
  );

  // Other initializations.
  ctx->m_run_mode = 0;
}


ars3d_void cubeLayerOnDetatch(ars3d_void *p_ctx)
{
  CubeLayerCtx *ctx = (CubeLayerCtx *)p_ctx;

  // Destroy all the HEAP allocated Objects.
  ars3dVertexArrayDestroy(&ctx->m_vao);
  ars3dVertexBufferDestroy(&ctx->m_vbo);
  ars3dVertexAttributesDestroy(&ctx->m_attribs);
  ars3dShaderDestroy(&ctx->m_shader);

  // Free the layer Context.
  ars3dFree(p_ctx);
}


ars3d_void cubeLayerOnStart(ars3d_void *p_ctx)
{
  CubeLayerCtx *ctx = (CubeLayerCtx *)p_ctx;

  //Prepare the drawing State...

  // Bind the buffers.
  ars3dVertexArrayBind(ctx->m_vao);
  ars3dVertexBufferBind(ctx->m_vbo);

  // Upload the SQUARE vertex data to the GPU.
  ars3dVertexBufferData(
      ctx->m_vbo,
      ARS3D_SIZEOF(square_vertices),
      square_vertices,
      ARS3D_VBO_STATIC_DRAW
  );

  // Create the atrribute pointers.
  ars3dVertexAttributesPush(
      ctx->m_attribs,
      "Positions",
      ARS3D_VERTEX_ATTRIBUTE_FLOAT,
      3
  );
  ars3dVertexAttributesBind(ctx->m_attribs);

  // Unbind everything for safety.
  ars3dVertexArrayUnbind();
  ars3dVertexBufferUnbind();
}


ars3d_void cubeLayerOnUpdate(ars3d_void *p_ctx, ars3d_float p_delta_time)
{
  // Run The UI Logic.
  runUI((CubeLayerCtx *)p_ctx);

  CubeLayerCtx *ctx         = (CubeLayerCtx *)p_ctx;
  CubeTransform *transform  = &ctx->m_cube_data.m_transform;

  // Update the rotation angle with delta time.
  ctx->m_cube_data.m_rotate_angle += p_delta_time * ctx->m_cube_data.m_rotation_speed;
  ctx->m_cube_data.m_pulse_angle  += p_delta_time * ctx->m_cube_data.m_pulse_speed;


  // Calculate the pulse factor and update the scale vector.
  float full_scale    = CUBE_LAYER_A / CUBE_LAYER_SQUARE_LENGTH;
  float half_scale    = full_scale / 2.0f;
  float avg_scale     = (full_scale + half_scale) / 2.0f;
  float amplitude     = full_scale - avg_scale;
  float pulse         = avg_scale + sinf(ctx->m_cube_data.m_pulse_angle) * amplitude;
  transform->m_scale  = (vec3s){{pulse, pulse, pulse}};

  /* * SHEAR MATRIX LOOK (X-shear based on Y):
  * * Column-Major representation (OpenGL convention):
  * [ 1        0        0        0 ]  <-- Column 0 (X-axis)
  * [ cot(th)  1        0        0 ]  <-- Column 1 (Y-axis) affects X
  * [ 0        0        1        0 ]  <-- Column 2 (Z-axis)
  * [ 0        0        0        1 ]  <-- Column 3 (Translation)
  */
  float avg_angle = (90.0f + 45.0f) / 2.0f;
  float radius    = 22.5f;
  float theta_deg = avg_angle + sinf(ctx->m_cube_data.m_rotate_angle) * radius;
  float cot_theta = 1.0f / tanf(glm_rad(theta_deg)); // cot(theta) = 1/tan(theta)
  mat4s shear_matrix          = glms_mat4_identity();
  shear_matrix.col[1].raw[0]  = cot_theta;

  // Bind the SHADER.
  ars3dShaderBind(ctx->m_shader);

  // Draw 6 squares in different places. Theses are the faces of the CUBE!!!
  for (int i = 0; i < 6; i++)
  {
    transform->m_model = glms_mat4_identity();

    // PULSE.
    if (ctx->m_run_mode == CUBE_LAYER_RUN_MODE_A)
    {
      transform->m_model = glms_translate(transform->m_model, transform->m_position);
      transform->m_model = glms_rotate(transform->m_model, ctx->m_cube_data.m_rotate_angle, transform->m_rotation);
      transform->m_model = glms_scale(transform->m_model, transform->m_scale);
    }

    // SHEAR
    else if (ctx->m_run_mode == CUBE_LAYER_RUN_MODE_B)
    {
      transform->m_scale = (vec3s){{3.5f, 3.5f, 3.5f}};
      transform->m_model = glms_translate(transform->m_model, transform->m_position);
      transform->m_model = glms_rotate(transform->m_model, ctx->m_cube_data.m_rotate_angle, transform->m_rotation);
      transform->m_model = glms_mat4_mul(transform->m_model, shear_matrix);
      transform->m_model = glms_scale(transform->m_model, transform->m_scale);
    }

    // ORBIT + PULSE
    else if (ctx->m_run_mode == CUBE_LAYER_RUN_MODE_C)
    {
      float z            = (-8 * CUBE_LAYER_B) / 10.0f;
      transform->m_model = glms_translate(transform->m_model, (vec3s){{0.0f, 0.0f, z}});
      transform->m_model = glms_rotate(transform->m_model, ctx->m_cube_data.m_rotate_angle, transform->m_rotation);
      transform->m_model = glms_translate(transform->m_model, (vec3s){{CUBE_LAYER_ORBIT_RADIUS, 0.0f, 0.0f}});
      transform->m_model = glms_scale(transform->m_model, transform->m_scale);
    }

    // Local (Relative offsets) of each FACE from the center of the cube.
    // Since we ALREADY translated to transform->m_position (cube center)
    // AND we did the scaling (3.5 to each axis), we can now OFFSET each FACE
    // with a UNIT LENGTH (1) to position each face relative to the center.
    switch (i)
    {
      // Front FACE (Parallel to the XY PLANE).
      case 0:

        // Translate it to the calculated position above.
        transform->m_model = glms_translate(transform->m_model, (vec3s){{0.0f, 0.0f, 1.0f}});

        // Rotate it "outside of the x=y plane" to be parallel with the plane and face the CAMERA.
        transform->m_model = glms_rotate(transform->m_model, glm_rad(90.0f), (vec3s){{0, 1, 0}});
        transform->m_model = glms_rotate(transform->m_model, glm_rad(45.0f), (vec3s){{0, 0, 1}});


        ars3dShaderUniformFloat3(ctx->m_shader, "u_color", 1.0f, 0.0f, 0.0f); // RED
        break;

      // Back FACE (Parallel to the XY PLANE).
      case 1:

        transform->m_model = glms_translate(transform->m_model, (vec3s){{0.0f, 0.0f, -1.0f}});

        transform->m_model = glms_rotate(transform->m_model, glm_rad(90.0f), (vec3s){{0, 1, 0}});
        transform->m_model = glms_rotate(transform->m_model, glm_rad(45.0f), (vec3s){{0, 0, 1}});

        ars3dShaderUniformFloat3(ctx->m_shader, "u_color", 0.0f, 1.0f, 1.0f); // CYAN
        break;

      // LEFT FACE (Parallel to the YZ PLANE).
      case 2:

        transform->m_model = glms_translate(transform->m_model, (vec3s){{-1.0f, 0.0f, 0.0f}});
        transform->m_model = glms_rotate(transform->m_model, glm_rad(45.0f), (vec3s){{0, 0, 1}});

        ars3dShaderUniformFloat3(ctx->m_shader, "u_color", 0.0f, 1.0f, 0.0f); // GREEN
        break;


      // RIGHT FACE (Parallel to the YZ PLANE).
      case 3:

        transform->m_model = glms_translate(transform->m_model, (vec3s){{1.0f, 0.0f, 0.0f}});
        transform->m_model = glms_rotate(transform->m_model, glm_rad(45.0f), (vec3s){{0, 0, 1}});

        ars3dShaderUniformFloat3(ctx->m_shader, "u_color", 1.0f, 0.0f, 1.0f); // MAGNETA
        break;

      // TOP FACE (Parallel to the XZ PLANE).
      case 4:

        transform->m_model = glms_translate(transform->m_model, (vec3s){{0.0f, 1.0f, 0.0f}});
        transform->m_model = glms_rotate(transform->m_model, glm_rad(-45.0f), (vec3s){{0, 0, 1}});

        ars3dShaderUniformFloat3(ctx->m_shader, "u_color", 0.0f, 0.0f, 1.0f); // BLUE
        break;

      // BOTTOM FACE (Parallel to the XZ PLANE).
      case 5:

        transform->m_model = glms_translate(transform->m_model, (vec3s){{0.0f, -1.0f, 0.0f}});
        transform->m_model = glms_rotate(transform->m_model, glm_rad(-45.0f), (vec3s){{0, 0, 1}});

        ars3dShaderUniformFloat3(ctx->m_shader, "u_color", 1.0f, 1.0f, 0.0f); // YELLOW
        break;

      default:
        break;
    }


    // Bind the Shader and Update the Uniform Variables.
    ars3dShaderUniformMat4(ctx->m_shader, "u_model", &transform->m_model);
    ars3dShaderUniformMat4(ctx->m_shader, "u_view", &transform->m_view);
    ars3dShaderUniformMat4(ctx->m_shader, "u_proj", &transform->m_projection);

    // Bind VAO and Draw the face.
    ars3dVertexArrayBind(ctx->m_vao);
    ars3dGLEnableDepthTest(1);
    ars3dGLRenderTriangles(6);
  }
}


ars3d_uchar cubeLayerOnEvent(ars3d_void *p_ctx, ars3d_void *p_event, ars3d_int p_event_type)
{
  CubeLayerCtx *ctx = (CubeLayerCtx *)p_ctx;

  switch (p_event_type)
  {
    // On window resize, re-calculate the Projection Matrix.
    case ARS3D_EVENT_TYPE_WINDOW_RESIZED:
      ctx->m_cube_data.m_transform.m_projection = calculateProjection(p_event);
      break;
  }

  return 0;
}


CubeLayerCtx *cubeLayerCreate()
{
  CubeLayerCtx *new_ctx = (CubeLayerCtx *)ars3dMalloc(ARS3D_SIZEOF(CubeLayerCtx));

  if (!new_ctx) return ARS3D_NULL;

  // Initialization will happen in the function cubeLayerOnAttach()
  return new_ctx;
}

