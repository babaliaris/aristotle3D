#include "barnsley.h"


ars3d_void onBarnsleyLayerAttach(ars3d_void *p_ctx)
{
    BarnsleyLayer *ctx = (BarnsleyLayer *)p_ctx;

    ctx->m_vao      = ars3dVertexArrayCreate();
    ctx->m_vbo      = ars3dVertexBufferCreate();
    ctx->m_attribs  = ars3dVertexAttributesCreate();

    // Create the points.
    int num_points  = 15000;
    ctx->m_points   = malloc(num_points * 3 * sizeof(float)); // x, y, z

    float x = 0, y = 0;
    for(int i = 0; i < num_points; i++)
    {
        float r = (float)rand() / RAND_MAX;
        float xn, yn;
        
        if (r < 0.01f) { xn = 0; yn = 0.16f * y; }
        else if (r < 0.86f) { xn = 0.85f * x + 0.04f * y; yn = -0.04f * x + 0.85f * y + 1.6f; }
        else if (r < 0.93f) { xn = 0.2f * x - 0.26f * y; yn = 0.23f * x + 0.22f * y + 1.6f; }
        else { xn = -0.15f * x + 0.28f * y; yn = 0.26f * x + 0.24f * y + 0.44f; }
        
        ctx->m_points[i*3 + 0] = xn;
        ctx->m_points[i*3 + 1] = yn;
        ctx->m_points[i*3 + 2] = 0.0f;
        x = xn; y = yn;
    }

    ars3dVertexArrayBind(ctx->m_vao);
    ars3dVertexBufferBind(ctx->m_vbo);
    ars3dVertexBufferData(ctx->m_vbo, num_points * 3 * sizeof(float), ctx->m_points, ARS3D_VBO_STATIC_DRAW);

    ars3dVertexAttributesPush(ctx->m_attribs, "Positions", ARS3D_VERTEX_ATTRIBUTE_FLOAT, 3);
    ars3dVertexAttributesBind(ctx->m_attribs);

    ars3dVertexBufferUnbind();
    ars3dVertexArrayUnbind();


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
    "out vec4 fragColor;"
    "void main()"
    "{"
        "fragColor = vec4(1.0, 1.0, 1.0, 1.0);"
    "}";

    ctx->m_shader = ars3dShaderCreateFromStrings(
        "Render Triangle Shader",
        vertex_shader_src,
        ARS3D_NULL,
        fragment_shader_src
    );
}

ars3d_void onBarnsleyLayerDetatch(ars3d_void *p_ctx)
{
    BarnsleyLayer *ctx = (BarnsleyLayer *)p_ctx;

    ars3dVertexArrayDestroy(&ctx->m_vao);
    ars3dVertexBufferDestroy(&ctx->m_vbo);
    ars3dVertexAttributesDestroy(&ctx->m_attribs);
    ars3dShaderDestroy(&ctx->m_shader);

    ars3dFree(ctx->m_points);
    ars3dFree(ctx);
}

ars3d_void onBarnsleyLayerStart(ars3d_void *p_ctx)
{
    BarnsleyLayer *ctx = (BarnsleyLayer *)p_ctx;

    ars3d_int window_width, window_height;
    ars3dWindowGetSize(ars3dAppGetWindow(ars3dAppGet()), &window_width, &window_height);

    ctx->m_model        = glms_mat4_identity();
    ctx->m_view         = glms_translate(glms_mat4_identity(), (vec3s){{0.0f, 0.0f, -3.0f}});
    ctx->m_projection   = glms_perspective(glm_rad(45.0f), (float)window_width / (float)window_height, 0.1f, 100.0f);

    ars3dShaderBind(ctx->m_shader);
    ars3dShaderUniformMat4(ctx->m_shader, "u_model", &ctx->m_model);
    ars3dShaderUniformMat4(ctx->m_shader, "u_view", &ctx->m_view);
    ars3dShaderUniformMat4(ctx->m_shader, "u_proj", &ctx->m_projection);
    ars3dShaderUnbind();
}

ars3d_void onBarnsleyLayerUpdate(ars3d_void *p_ctx)
{
    BarnsleyLayer *ctx = (BarnsleyLayer *)p_ctx;

    ars3dGLEnableDepthTest(1);
    ars3dShaderBind(ctx->m_shader);
    ars3dVertexArrayBind(ctx->m_vao);
    ars3dGLRenderPoints(15000);
    ars3dGLEnableDepthTest(0);
}

ars3d_uchar onBarnsleyLayerEvent(ars3d_void *p_ctx, ars3d_void *p_event, ars3d_int p_event_type)
{
    BarnsleyLayer *ctx = (BarnsleyLayer *)p_ctx;

    switch (p_event_type)
    {
        case ARS3D_EVENT_TYPE_WINDOW_RESIZED:
        {
            ars3d_int width, height;
            ars3dEventGetWindowSize(p_event, &width, &height);
            ctx->m_projection = glms_perspective(glm_rad(45.0f), (float)width / (float)height, 0.1f, 100.0f);
            ars3dShaderBind(ctx->m_shader);
            ars3dShaderUniformMat4(ctx->m_shader, "u_proj", &ctx->m_projection);
            ars3dShaderUnbind();
            break;
        }

        
        case ARS3D_EVENT_TYPE_MOUSE_POSITION:
        {
            
            ars3d_double mouse_x, mouse_y;
            ars3d_int width, height;
            ars3dEventGetMousePos(p_event, &mouse_x, &mouse_y);
            ars3dWindowGetSize(ars3dAppGetWindow(ars3dAppGet()), &width, &height);

            // Convert pixels to Normalized Device Coordinates (-1.0 to 1.0)
            float ndc_x = (float)(mouse_x / width) * 2.0f - 1.0f;
            float ndc_y = 1.0f - (float)(mouse_y / height) * 2.0f; // Y is inverted in screen space

            // Scale it so it stays within the camera view 
            // (At z=0 with 45deg FoV, the visible area is roughly 2.5 units wide)
            float world_x = ndc_x * 2.5f; 
            float world_y = ndc_y * 2.5f;

            ctx->m_pos = (vec3s){{world_x, world_y, 0.0f}};

            // Rebuild the matrix.
            ctx->m_model = glms_translate(glms_mat4_identity(), ctx->m_pos);

            ARS3D_INFO("Pos = (%.2f, %.2f)", world_x, world_y);

            // Upload the uniform.
            ars3dShaderBind(ctx->m_shader);
            ars3dShaderUniformMat4(ctx->m_shader, "u_model", &ctx->m_model);
            ars3dShaderUnbind();
            break;
        }
        

        default:
            break;
    }

    return 0;
}
