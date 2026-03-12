#include "render-triangle-test.layer.h"


ars3d_void onRenderTriangleTestLayerAttach(ars3d_void *p_ctx)
{
    RenderTriangleTestLayer *ctx = (RenderTriangleTestLayer *)p_ctx;

    ctx->m_vao      = ars3dVertexArrayCreate();
    ctx->m_vbo      = ars3dVertexBufferCreate();
    ctx->m_attribs  = ars3dVertexAttributesCreate();

    ars3d_float triangle_vertices[] = {
        // Positions            // Colors
        -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f,      0.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.0f,       0.0f, 0.0f, 1.0f
    };

    ars3dVertexArrayBind(ctx->m_vao);
    ars3dVertexBufferBind(ctx->m_vbo);
    ars3dVertexBufferData(ctx->m_vbo, ARS3D_SIZEOF(triangle_vertices), triangle_vertices, ARS3D_VBO_STATIC_DRAW);

    ars3dVertexAttributesPush(ctx->m_attribs, "Positions", ARS3D_VERTEX_ATTRIBUTE_FLOAT, 3);
    ars3dVertexAttributesPush(ctx->m_attribs, "Colors", ARS3D_VERTEX_ATTRIBUTE_FLOAT, 3);
    ars3dVertexAttributesBind(ctx->m_attribs);

    ars3dVertexBufferUnbind();
    ars3dVertexArrayUnbind();


    const char *vertex_shader_src =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;"
    "layout (location = 1) in vec3 aColor;"
    "uniform mat4 u_model, u_view, u_proj;"
    "out vec3 passColor;"
    "void main()"
    "{"
        "passColor      = aColor;"
        "gl_Position    = u_proj * u_view * u_model * vec4(aPos, 1.0);"
    "}";


    const char *fragment_shader_src =
    "#version 330 core\n"
    "in vec3 passColor;"
    "out vec4 fragColor;"
    "void main()"
    "{"
        "fragColor      = vec4(passColor, 1.0);"
    "}";

    ctx->m_shader = ars3dShaderCreateFromStrings(
        "Render Triangle Shader",
        vertex_shader_src,
        ARS3D_NULL,
        fragment_shader_src
    );
}

ars3d_void onRenderTriangleTestLayerDetatch(ars3d_void *p_ctx)
{
    RenderTriangleTestLayer *ctx = (RenderTriangleTestLayer *)p_ctx;

    ars3dVertexArrayDestroy(&ctx->m_vao);
    ars3dVertexBufferDestroy(&ctx->m_vbo);
    ars3dVertexAttributesDestroy(&ctx->m_attribs);
    ars3dShaderDestroy(&ctx->m_shader);

    ars3dFree(ctx);
}

ars3d_void onRenderTriangleTestLayerStart(ars3d_void *p_ctx)
{
    RenderTriangleTestLayer *ctx = (RenderTriangleTestLayer *)p_ctx;

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

ars3d_void onRenderTriangleTestLayerUpdate(ars3d_void *p_ctx)
{
    RenderTriangleTestLayer *ctx = (RenderTriangleTestLayer *)p_ctx;

    ars3dShaderBind(ctx->m_shader);
    ars3dVertexArrayBind(ctx->m_vao);
    ars3dRenderTriangles(3);
}

ars3d_uchar onRenderTriangleTestLayerEvent(ars3d_void *p_ctx, ars3d_void *p_event, ars3d_int p_event_type)
{
    RenderTriangleTestLayer *ctx = (RenderTriangleTestLayer *)p_ctx;

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
        
        default:
            break;
    }

    return 0;
}
