#include "barnsley.h"


ars3d_void calculateBarnsleyBuffer(ars3d_void *p_context, ars3d_int p_mode);


ars3d_void onBarnsleyLayerAttach(ars3d_void *p_ctx)
{
    BarnsleyLayer *ctx = (BarnsleyLayer *)p_ctx;

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

    // Get window width and height.
    ars3d_int window_width, window_height;
    ars3dWindowGetSize(ars3dAppGetWindow(ars3dAppGet()), &window_width, &window_height);

    // --------------------|Initialize the Barnsley Context|-------------------- //
    ctx->m_vao              = ars3dVertexArrayCreate();
    ctx->m_vbo              = ars3dVertexBufferCreate();
    ctx->m_attribs          = ars3dVertexAttributesCreate();

    ctx->m_shader           = ars3dShaderCreateFromStrings(
        "Barnsley Shader",
        vertex_shader_src,
        ARS3D_NULL,
        fragment_shader_src
    );
    ARS3D_ASSERT(ctx->m_shader, "Probably shader compilation failed");

    ctx->m_num_of_points    = 15000;
    ctx->m_buffer_size      = 0;
    ctx->m_points_buffer    = ARS3D_NULL;
    ctx->m_barnsley_mode    = BARNSLEY_MODE_DEFAULT;
    ctx->m_scale_toggler    = 0;
    ctx->m_move_toggler     = 0;
    ctx->m_pos              = (vec3s){{0.0f, -6.0f, 0.0f}};
    ctx->m_rotate           = (vec3s){{0.0f, 0.0f, 0.0f}};
    ctx->m_scale            = (vec3s){{1.0f, 1.0f, 1.0f}};
    ctx->m_color            = (vec3s){{0.0f, 1.0f, 0.0f}};
    ctx->m_model            = glms_mat4_identity();
    ctx->m_view             = glms_translate(glms_mat4_identity(), (vec3s){{0.0f, 0.0f, -15.0f}});
    ctx->m_projection       = glms_perspective(glm_rad(45.0f), (float)window_width / (float)window_height, 0.1f, 100.0f);
    // --------------------|Initialize the Barnsley Context|-------------------- //


    // Bind the buffers.
    ars3dVertexArrayBind(ctx->m_vao);
    ars3dVertexBufferBind(ctx->m_vbo);

    // Bind and link the VBO and its attributes to the current VAO.
    ars3dVertexAttributesPush(ctx->m_attribs, "Positions", ARS3D_VERTEX_ATTRIBUTE_FLOAT, 3);
    ars3dVertexAttributesBind(ctx->m_attribs);

    // Unbind the buffers.
    ars3dVertexArrayUnbind();
    ars3dVertexBufferUnbind();

    // Create the BARNSLEY Buffer.
    calculateBarnsleyBuffer(p_ctx, BARNSLEY_MODE_DEFAULT);
}




ars3d_void onBarnsleyLayerDetatch(ars3d_void *p_ctx)
{
    BarnsleyLayer *ctx = (BarnsleyLayer *)p_ctx;

    // Destroy the Graphics Objects.
    ars3dVertexArrayDestroy(&ctx->m_vao);
    ars3dVertexBufferDestroy(&ctx->m_vbo);
    ars3dVertexAttributesDestroy(&ctx->m_attribs);
    ars3dShaderDestroy(&ctx->m_shader);

    // Free the points buffer.
    ars3dFree(ctx->m_points_buffer);

    // Free the context.
    ars3dFree(ctx);
}




ars3d_void onBarnsleyLayerStart(ars3d_void *p_ctx)
{
    BarnsleyLayer *ctx = (BarnsleyLayer *)p_ctx;

    // Set the window title.
    ars3dWindowSetTitle(ars3dAppGetWindow(ars3dAppGet()) ,"SPACE = toggle size, 1 = 15000, 2 = 20000, 3 = 30000, 4 = 40000");

    // Initialize the uniforms.
    ars3dShaderBind(ctx->m_shader);
    ars3dShaderUniformMat4(ctx->m_shader, "u_model", &ctx->m_model);
    ars3dShaderUniformMat4(ctx->m_shader, "u_view", &ctx->m_view);
    ars3dShaderUniformMat4(ctx->m_shader, "u_proj", &ctx->m_projection);
    ars3dShaderUniformFloat3(ctx->m_shader, "u_color", ctx->m_color.raw[0], ctx->m_color.raw[1], ctx->m_color.raw[2]);
    ars3dShaderUnbind();

    // Increase the points size.
    ars3dGLPointSize(2.0f);
}




ars3d_void onBarnsleyLayerUpdate(ars3d_void *p_ctx, ars3d_float p_delta_time)
{
    ARS3D_UNUSED(p_delta_time);
    BarnsleyLayer *ctx = (BarnsleyLayer *)p_ctx;

    // Calculate the scale vector.
    ctx->m_scale    = ctx->m_scale_toggler ? (vec3s){{0.5f, 0.5f, 0.5f}} : (vec3s){{1.0f, 1.0f, 1.0f}};

    // Calculate the model matrix.
    ctx->m_model    = glms_translate(glms_mat4_identity(), ctx->m_pos);
    ctx->m_model    = glms_rotate(ctx->m_model , 0.0f, ctx->m_rotate);
    ctx->m_model    = glms_scale(ctx->m_model, ctx->m_scale);

    // Update uniforms.
    ars3dShaderBind(ctx->m_shader);
    ars3dShaderUniformMat4(ctx->m_shader, "u_model", &ctx->m_model);
    ars3dShaderUniformFloat3(ctx->m_shader, "u_color", ctx->m_color.raw[0], ctx->m_color.raw[1], ctx->m_color.raw[2]);


    // Bind the VAO and render the points.
    ars3dVertexArrayBind(ctx->m_vao);
    ars3dGLRenderPoints(ctx->m_num_of_points);
}




ars3d_uchar onBarnsleyLayerEvent(ars3d_void *p_ctx, ars3d_void *p_event, ars3d_int p_event_type)
{
    BarnsleyLayer *ctx = (BarnsleyLayer *)p_ctx;


    switch (p_event_type)
    {
        // Window Resized.
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

        // Mouse MOVED!
        case ARS3D_EVENT_TYPE_MOUSE_POSITION:
        {
            if (ctx->m_move_toggler)
            {
                /**
                 * Since I don't have a camera system (yet)
                 * I will fake one by converting the screen space
                 * mouse position to normilized decide coordinates
                 * based on a fixed hardcoded view matrix that "lives" in the
                 * position (0.0f, 0.0f, -15.0f) -15 units in the
                 * z axis.
                 */

                ars3d_double mouse_x, mouse_y;
                ars3d_int w, h;
                ars3dEventGetMousePos(p_event, &mouse_x, &mouse_y);
                ars3dWindowGetSize(ars3dAppGetWindow(ars3dAppGet()), &w, &h);

                // Convert mouse to [-1.0 , 1.0] range (NDC)
                float ndc_x = (float)(mouse_x / w) * 2.0f - 1.0f;
                float ndc_y = 1.0f - (float)(mouse_y / h) * 2.0f; // Flip Y (GLFW negative Y is UP!!!)

                // Map to world units (approximate for a camera at z = -15)
                ctx->m_pos = (vec3s){{ndc_x * 8.0f, ndc_y * 8.0f}};


                if (ctx->m_barnsley_mode != BARNSLEY_MODE_DEFAULT)
                {
                    // Create a new vibrant color
                    // We use rand() % 3 to pick one channel to 'kill' or keep low,
                    // which prevents the colors from washing out into white.
                    int dominant_channel = rand() % 3;

                    float nr = ((float)rand() / RAND_MAX);
                    float ng = ((float)rand() / RAND_MAX);
                    float nb = ((float)rand() / RAND_MAX);

                    if (dominant_channel == 0) nr = 1.0f; // Force high Red
                    if (dominant_channel == 1) ng = 1.0f; // Force high Green
                    if (dominant_channel == 2) nb = 1.0f; // Force high Blue

                    // "dim" the non-dominant channels
                    if (dominant_channel != 0) nr *= 0.5f;
                    if (dominant_channel != 1) ng *= 0.5f;
                    if (dominant_channel != 2) nb *= 0.5f;

                    ctx->m_color        = (vec3s){{nr, ng, nb}};
                }

                else
                    ctx->m_color        = (vec3s){{0.0f, 1.0f, 0.0f}};
            }

            break;
        }

        // Keyboard key PRESSED.
        case ARS3D_EVENT_TYPE_KEYBOARD_PRESS:
        {
            ars3d_int key = ars3dEventGetKeyboardKey(p_event);

            switch (key)
            {
                case ARS3D_EVENT_KEY_1:
                    calculateBarnsleyBuffer(p_ctx ,BARNSLEY_MODE_DEFAULT);
                    ctx->m_barnsley_mode = BARNSLEY_MODE_DEFAULT;
                    break;

                case ARS3D_EVENT_KEY_2:
                    calculateBarnsleyBuffer(p_ctx ,BARNSLEY_MODE_20000);
                    ctx->m_barnsley_mode = BARNSLEY_MODE_20000;
                    break;

                case ARS3D_EVENT_KEY_3:
                    calculateBarnsleyBuffer(p_ctx ,BARNSLEY_MODE_30000);
                    ctx->m_barnsley_mode = BARNSLEY_MODE_30000;
                    break;

                case ARS3D_EVENT_KEY_4:
                    calculateBarnsleyBuffer(p_ctx ,BARNSLEY_MODE_40000);
                    ctx->m_barnsley_mode = BARNSLEY_MODE_40000;
                    break;

                case ARS3D_EVENT_KEY_SPACE:
                    ctx->m_scale_toggler = !ctx->m_scale_toggler;
                    break;

                default:
                    break;
                }

            break;
        }

        // Mouse Button PRESSED
        case ARS3D_EVENT_TYPE_MOUSE_BUTTON_PRESS:
        {
            ars3d_int button = ars3dEventGetMouseButton(p_event);

            if (button == ARS3D_EVENT_MOUSE_BUTTON_1) ctx->m_move_toggler = 1;

            break;
        }

        // Mouse Button RELEASED
        case ARS3D_EVENT_TYPE_MOUSE_BUTTON_RELEASE:
        {
            ars3d_int button = ars3dEventGetMouseButton(p_event);

            if (button == ARS3D_EVENT_MOUSE_BUTTON_1) ctx->m_move_toggler = 0;

            break;
        }

        default:
            break;
    }

    return 0;
}



ars3d_void calculateBarnsleyBuffer(ars3d_void *p_context, ars3d_int p_mode)
{
    BarnsleyLayer *ctx = (BarnsleyLayer *)p_context;

    // If this is not the first time this function is called
    // make sure to return in case the mode is not already running
    // This makes sure the buffer is already under allocation and
    // does not reset it if the running mode is the same.
    // Also it avoids trying to free a memory that is ALREADY FREEDED!!!
    if (ctx->m_points_buffer && p_mode == ctx->m_barnsley_mode) return;

    // Select mode.
    switch (p_mode)
    {
        case BARNSLEY_MODE_DEFAULT:
            ctx->m_num_of_points = 15000;
            break;

        case BARNSLEY_MODE_20000:
            ctx->m_num_of_points    = 20000;
            break;

        case BARNSLEY_MODE_30000:
            ctx->m_num_of_points    = 30000;
            break;

        case BARNSLEY_MODE_40000:
            ctx->m_num_of_points    = 40000;
            break;

        default:
            ctx->m_num_of_points = 15000;
            break;
    }

    // Free the previous buffer.
    ars3dFree(ctx->m_points_buffer);

    // Calculate the buffer size (3 floats each point).
    ctx->m_buffer_size = ctx->m_num_of_points * 3 * ARS3D_SIZEOF(ars3d_float);

    // Allocate memory for the buffer.
    ctx->m_points_buffer = ars3dMalloc(ctx->m_buffer_size); // x, y, z (Array of structs)

    // Calculate the vertices using the Barnsley Algorithm.
    ars3d_float x = 0, y = 0;
    for(ars3d_int i = 0; i < ctx->m_num_of_points; i++)
    {
        ars3d_float r = (ars3d_float)rand() / RAND_MAX;
        ars3d_float xn, yn;

        if (r < 0.01f)      { xn = 0; yn = 0.16f * y; }
        else if (r < 0.86f) { xn = 0.85f * x + 0.04f * y; yn = -0.04f * x + 0.85f * y + 1.6f; }
        else if (r < 0.93f) { xn = 0.2f * x - 0.26f * y; yn = 0.23f * x + 0.22f * y + 1.6f; }
        else                { xn = -0.15f * x + 0.28f * y; yn = 0.26f * x + 0.24f * y + 0.44f; }
        
        ctx->m_points_buffer[i*3 + 0] = xn;
        ctx->m_points_buffer[i*3 + 1] = yn;
        ctx->m_points_buffer[i*3 + 2] = 0.0f;

        x = xn; y = yn;
    }

    // Transfer the data to the GPU.
    ars3dVertexBufferBind(ctx->m_vbo);
    ars3dVertexBufferData(ctx->m_vbo, ctx->m_buffer_size, ctx->m_points_buffer, ARS3D_VBO_STATIC_DRAW);
    ars3dVertexBufferUnbind();
}

