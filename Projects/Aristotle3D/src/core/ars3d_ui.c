#include "aristotle3D/core/ars3d_stdio.h"
#include <aristotle3D/core/ars3d_ui.h>
#include <aristotle3D/core/ars3d_debug.h>
#include <aristotle3D/core/ars3d_events.h>
#include <aristotle3D/core/ars3d_window.h>

static mu_Context *s_mu_ctx = ARS3D_NULL;
static ars3d_double s_last_mouse_x, s_last_mouse_y;


// TODO: Use the font arg later, that MicroUI provides for better width calculation.
static ars3d_int text_width(mu_Font font, const char *text, int len)
{
  ARS3D_UNUSED(font);
  if (len == -1) len = strlen(text);
  return len * 8; 
}

// TODO: Use the font arg later, for better height calculation.
static ars3d_int text_height(mu_Font font)
{
  ARS3D_UNUSED(font);
  return 16;
}


void ars3dUIInit()
{
  ARS3D_ASSERT(s_mu_ctx == ARS3D_NULL, "s_mui_ctx MUST be null at this point (singleton pattern)");
  s_mu_ctx = ars3dMalloc(sizeof(mu_Context));
  s_last_mouse_x = 0;
  s_last_mouse_y = 0;
  mu_init(s_mu_ctx);
  s_mu_ctx->text_width = text_width;
  s_mu_ctx->text_height = text_height;
}


void ars3dUITerminate()
{
  ARS3D_ASSERT(s_mu_ctx != ARS3D_NULL);
  ars3dFree(s_mu_ctx);
  s_mu_ctx = ARS3D_NULL;
}


void ars3dUIBegin()
{
  ARS3D_ASSERT(s_mu_ctx != ARS3D_NULL);
  mu_begin(s_mu_ctx);
}


void ars3dUIEnd()
{
  ARS3D_ASSERT(s_mu_ctx != ARS3D_NULL);
  mu_end(s_mu_ctx);
}


mu_Context* ars3dUIGetContext()
{
  ARS3D_ASSERT(s_mu_ctx != ARS3D_NULL);
  return s_mu_ctx;
}


ars3d_int ars3dUIWantCaptureMouse()
{
  ARS3D_ASSERT(s_mu_ctx != ARS3D_NULL);
  return (s_mu_ctx->hover_root != NULL || s_mu_ctx->focus != 0);
}


ars3d_void ars3dUIProcessEvent(ars3d_void* p_event, ars3d_int p_event_type)
{
  mu_Context* mu = ars3dUIGetContext();

  switch (p_event_type)
  {
    case ARS3D_EVENT_TYPE_MOUSE_POSITION:
    {
      ars3dEventGetMousePos(p_event, &s_last_mouse_x, &s_last_mouse_y);
      mu_input_mousemove(mu, (int)s_last_mouse_x, (int)s_last_mouse_y);
      break;
    }

    case ARS3D_EVENT_TYPE_MOUSE_BUTTON_PRESS:
    {
      ars3d_int btn = ars3dEventGetMouseButton(p_event);
      if (btn == ARS3D_EVENT_MOUSE_BUTTON_1)
      {
        mu_input_mousedown(mu, (int)s_last_mouse_x, (int)s_last_mouse_y, MU_MOUSE_LEFT);
      }
      break;
    }

    case ARS3D_EVENT_TYPE_MOUSE_BUTTON_RELEASE:
    {
      ars3d_int btn = ars3dEventGetMouseButton(p_event);
      if (btn == ARS3D_EVENT_MOUSE_BUTTON_1)
      {
        mu_input_mouseup(mu, (int)s_last_mouse_x, (int)s_last_mouse_y, MU_MOUSE_LEFT);
      }
      break;
    }
      // Add SCROLL and KEYBOARD similarly
  }
}

