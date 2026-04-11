#include <aristotle3D/layers/ars3d_ui_layer.h>

#include <aristotle3D/core/opengl/ars3d_render_operations.h>
#include <aristotle3D/core/opengl/ars3d_shader.h>
#include <aristotle3D/core/opengl/ars3d_vertex_array.h>
#include <aristotle3D/core/opengl/ars3d_vertex_buffer.h>
#include <aristotle3D/core/opengl/ars3d_vertex_attributes.h>

#include "aristotle3D/core/ars3d_debug.h"
#include "aristotle3D/core/ars3d_stdio.h"
#include <aristotle3D/core/ars3d_ui.h>
#include <aristotle3D/core/ars3d_application.h>
#include <aristotle3D/core/ars3d_window.h>

#include <glad/gl.h>
#include <aristotle3D/core/opengl/ars3d_opengl_debugger.h>

#include <microui/src/microui.h>
#include <microui/demo/atlas.inl>



typedef struct {
    float x, y;
    float u, v;
    float r, g, b, a;
} UIVertex;

const char* ui_vertex_shader = 
    "#version 330 core\n"
    "layout (location = 0) in vec2 aPos;"
    "layout (location = 1) in vec2 aTexCoord;"
    "layout (location = 2) in vec4 aColor;"
    "uniform mat4 u_proj;"
    "out vec2 vTexCoord;"
    "out vec4 vColor;"
    "void main() {"
    "  vTexCoord = aTexCoord;"
    "  vColor = aColor;"
    "  gl_Position = u_proj * vec4(aPos, 0.0, 1.0);"
    "}";

const char* ui_fragment_shader = 
    "#version 330 core\n"
    "in vec2 vTexCoord;"
    "in vec4 vColor;"
    "uniform sampler2D u_font_texture;"
    "out vec4 fragColor;"
    "void main() {"
    "    // If it's a rectangle (UV 0,0), it will sample a white pixel from the atlas\n"
    "    // If it's text, it will sample the glyph\n"
    "    fragColor = texture(u_font_texture, vTexCoord) * vColor;"
    "}";


ars3d_void ars3dUiLayerOnAttach(ars3d_void *p_ctx)
{
  Ars3DUILayer* ctx = (Ars3DUILayer *)p_ctx;

  ctx->m_shader = ars3dShaderCreateFromStrings(
      "UI Shader",
      ui_vertex_shader,
      ARS3D_NULL,
      ui_fragment_shader
  );

  ctx->m_vao      = ars3dVertexArrayCreate();
  ctx->m_vbo      = ars3dVertexBufferCreate();
  ctx->m_attribs  = ars3dVertexAttributesCreate();

  ars3dVertexArrayBind(ctx->m_vao);
  ars3dVertexBufferBind(ctx->m_vbo);
  
  // We'll use a dynamic buffer to push UI rectangles every frame
  // Max 4096 vertices for now.
  ars3dVertexBufferData(ctx->m_vbo, sizeof(UIVertex) * ARS3D_UI_MAX_VERTICES, NULL, ARS3D_VBO_DYNAMIC_DRAW);

  // Create the vertex array pointer attributes.
  ars3dVertexAttributesPush(ctx->m_attribs, "Position", ARS3D_VERTEX_ATTRIBUTE_FLOAT, 2);
  ars3dVertexAttributesPush(ctx->m_attribs, "UV's", ARS3D_VERTEX_ATTRIBUTE_FLOAT, 2);
  ars3dVertexAttributesPush(ctx->m_attribs, "Color", ARS3D_VERTEX_ATTRIBUTE_FLOAT, 4);
  ars3dVertexAttributesBind(ctx->m_attribs);

  // Unbind the buffers for safety reasons.
  ars3dVertexArrayUnbind();
  ars3dVertexBufferUnbind();

  // --------------------|Create a Texture for the UI Text Font|-------------------- //
  glGenTextures(1, &ctx->m_font_texture);
  glBindTexture(GL_TEXTURE_2D, ctx->m_font_texture);

  // The atlas is 128x128, 1 byte per pixel (Alpha)
  glTexImage2D(
      GL_TEXTURE_2D, 
      0, 
      GL_RED, 
      ATLAS_WIDTH, 
      ATLAS_HEIGHT, 
      0, 
      GL_RED, 
      GL_UNSIGNED_BYTE, 
      atlas_texture
  );

  // Essential: Set filtering to NEAREST for crisp pixel text
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // Swizzle mask: MicroUI provides alpha, we want it to apply to the color in the shader
  // This maps the Red channel of our texture to the Alpha channel in the shader
  GLint swizzleMask[] = {GL_ONE, GL_ONE, GL_ONE, GL_RED};
  glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
  // --------------------|Create a Texture for the UI Text Font|-------------------- //
}



ars3d_void ars3dUiLayerOnDetach(ars3d_void *p_ctx)
{
  Ars3DUILayer* ctx = (Ars3DUILayer *)p_ctx;

  ars3dShaderDestroy(&ctx->m_shader);
  ars3dVertexArrayDestroy(&ctx->m_vao);
  ars3dVertexBufferDestroy(&ctx->m_vbo);
  ars3dVertexAttributesDestroy(&ctx->m_attribs);

  ars3dFree(ctx);
}


ars3d_void ars3dUiLayerOnUpdate(ars3d_void *p_ctx, ars3d_float p_delta_time)
{
  ARS3D_UNUSED(p_delta_time);
  Ars3DUILayer* ctx = (Ars3DUILayer *)p_ctx;
  mu_Context* mu    = ars3dUIGetContext();

  ars3d_int w, h;
  ars3dWindowGetSize(ars3dAppGetWindow(ars3dAppGet()), &w, &h);
  ctx->m_proj = glms_ortho(0.0f, (float)w, (float)h, 0.0f, -1.0f, 1.0f);

  UIVertex vertices[ARS3D_UI_MAX_VERTICES];
  int vert_count = 0;

  mu_Command *cmd = ARS3D_NULL;
  while (mu_next_command(mu, &cmd))
  {
    mu_Rect src;
    mu_Rect r;
    mu_Color c;

    if (cmd->type == MU_COMMAND_RECT)
    {
      mu_RectCommand *rect_cmd = (mu_RectCommand*)cmd;
      r = rect_cmd->rect;
      c = rect_cmd->color;
      src = atlas[ATLAS_WHITE]; // Use the white pixel for solid boxes
    }

    else if (cmd->type == MU_COMMAND_TEXT)
    {
      mu_TextCommand *t = (mu_TextCommand*)cmd;
      c = t->color;
      int cur_x = t->pos.x;

      for (const char *p = t->str; *p; p++)
      {
        if (vert_count + 6 > ARS3D_UI_MAX_VERTICES) break;

        src = atlas[ATLAS_FONT + (unsigned char)*p];
        float rf = c.r/255.f, gf = c.g/255.f, bf = c.b/255.f, af = c.a/255.f;

        float u1 = (float)src.x / ATLAS_WIDTH;
        float v1 = (float)src.y / ATLAS_HEIGHT;
        float u2 = (float)(src.x + src.w) / ATLAS_WIDTH;
        float v2 = (float)(src.y + src.h) / ATLAS_HEIGHT;

        vertices[vert_count++] = (UIVertex){ (float)cur_x,         (float)t->pos.y, u1, v1, rf, gf, bf, af };
        vertices[vert_count++] = (UIVertex){ (float)cur_x + src.w, (float)t->pos.y, u2, v1, rf, gf, bf, af };
        vertices[vert_count++] = (UIVertex){ (float)cur_x,         (float)t->pos.y + src.h, u1, v2, rf, gf, bf, af };
        vertices[vert_count++] = (UIVertex){ (float)cur_x + src.w, (float)t->pos.y, u2, v1, rf, gf, bf, af };
        vertices[vert_count++] = (UIVertex){ (float)cur_x + src.w, (float)t->pos.y + src.h, u2, v2, rf, gf, bf, af };
        vertices[vert_count++] = (UIVertex){ (float)cur_x,         (float)t->pos.y + src.h, u1, v2, rf, gf, bf, af };

        cur_x += src.w;
      }

      continue; // Text is handled in its own loop, skip common rect logic below
    }

    else
    {
      continue;
    } // Handle other commands (icons, etc) similarly

    // Common logic for RECT (and Icons if you add them)
    if (vert_count + 6 > ARS3D_UI_MAX_VERTICES) break;


    float rf = c.r/255.f, gf = c.g/255.f, bf = c.b/255.f, af = c.a/255.f;
    float u1 = (float)src.x / ATLAS_WIDTH;
    float v1 = (float)src.y / ATLAS_HEIGHT;
    float u2 = (float)(src.x + src.w) / ATLAS_WIDTH;
    float v2 = (float)(src.y + src.h) / ATLAS_HEIGHT;

    vertices[vert_count++] = (UIVertex){ (float)r.x,      (float)r.y,      u1, v1, rf, gf, bf, af };
    vertices[vert_count++] = (UIVertex){ (float)r.x+r.w,  (float)r.y,      u2, v1, rf, gf, bf, af };
    vertices[vert_count++] = (UIVertex){ (float)r.x,      (float)r.y+r.h,  u1, v2, rf, gf, bf, af };
    vertices[vert_count++] = (UIVertex){ (float)r.x+r.w,  (float)r.y,      u2, v1, rf, gf, bf, af };
    vertices[vert_count++] = (UIVertex){ (float)r.x+r.w,  (float)r.y+r.h,  u2, v2, rf, gf, bf, af };
    vertices[vert_count++] = (UIVertex){ (float)r.x,      (float)r.y+r.h,  u1, v2, rf, gf, bf, af };
  }

  // --- |RENDER BLOCK| --- //
  if (vert_count > 0) {
    ars3dGLEnableDepthTest(0);
    ars3dGLEnableBlending(1);
    // Ensure standard Alpha blending
    ARS3D_OPENGL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    ARS3D_OPENGL(glActiveTexture(GL_TEXTURE0));
    ARS3D_OPENGL(glBindTexture(GL_TEXTURE_2D, ctx->m_font_texture));

    ars3dShaderBind(ctx->m_shader);
    ars3dShaderUniformMat4(ctx->m_shader, "u_proj", &ctx->m_proj);
    ars3dShaderUniformInt(ctx->m_shader, "u_font_texture", 0);

    ars3dVertexArrayBind(ctx->m_vao);
    ars3dVertexBufferSubData(ctx->m_vbo, 0, sizeof(UIVertex) * vert_count, vertices);
    ars3dGLRenderTriangles(vert_count);

    ars3dVertexArrayUnbind();
    ars3dShaderUnbind();
  }
}

Ars3DUILayer *ars3dUILayerCreate()
{
  Ars3DUILayer *new_layer = (Ars3DUILayer *)ars3dMalloc( ARS3D_SIZEOF(Ars3DUILayer) );

  if (!new_layer)
  {
    return ARS3D_NULL;
  }

  // The onAttach callback will initialize it.
  return new_layer;
}

