#include "launcher.h"
#include "events-test.layer.h"
#include "render-triangle-test.layer.h"
#include "barnsley.h"
#include "cube.h"

ars3d_void uiRun(LauncherLayerCtx *ctx)
{
  mu_Context *mu = ars3dUIGetContext();

  if (mu_begin_window(mu, "Ars3D Demo Launcher", mu_rect(50, 50, 250, 220)))
  {
    mu_layout_row(mu, 1, (int[]) { -1 }, 0);

    mu_label(mu, "Available Demos:");

    if (mu_button(mu, "Events Test")) 
    {
      ctx->m_layer = LAUNCHER_LAUNCH_EVENTS_TEST;
    }

    if (mu_button(mu, "Triangle Test")) 
    {
      ctx->m_layer =LAUNCHER_LAUNCH_RENDER_TRIANGLE_TEST;
    }

    if (mu_button(mu, "Barnsley Fractal")) 
    {
      ctx->m_layer = LAUNCHER_LAUNCH_BARNSLEY_FRACTAL;
    }

    if (mu_button(mu, "3D Animated Cube")) 
    {
      ctx->m_layer = LAUNCHER_LAUNCH_CUBE;
    }

    if (mu_button(mu, "EXIT"))
    {
      ctx->m_layer = LAUNCHER_LAUNCH_EXIT;
    }


    mu_end_window(mu);
  }
}


ars3d_void launcherLayerOnAttach(ars3d_void *p_ctx)
{
  LauncherLayerCtx *ctx = (LauncherLayerCtx *)p_ctx;

  ctx->m_layer = LAUNCHER_LAUNCH_NONE;
}

ars3d_void launcherLayerOnDetatch(ars3d_void *p_ctx)
{
  ars3dFree(p_ctx);
}

ars3d_void launcherLayerOnUpdate(ars3d_void *p_ctx, ars3d_float p_delta_time)
{
  ARS3D_UNUSED(p_delta_time);
  LauncherLayerCtx *ctx = (LauncherLayerCtx *)p_ctx;

  if (ctx->m_layer == LAUNCHER_LAUNCH_NONE)
  {
    // Run the UI.
    uiRun(ctx);

    switch (ctx->m_layer)
    {
      case LAUNCHER_LAUNCH_EVENTS_TEST:
      {
        ARS3D_INFO("Starting up: Events Test");
        EventsTestLayer *layer = (EventsTestLayer *)ars3dMalloc(sizeof(EventsTestLayer));
        layer->m_counter = 0;
        ars3dAppAttachLayer(
            ars3dAppGet(), layer, onEventsTestLayerAttach, onEventsTestLayerDetatch,
            onEventsTestLayerStart, onEventsTestLayerUpdate, onEventsTestLayerEvent
        );
        break;
      }

      case LAUNCHER_LAUNCH_RENDER_TRIANGLE_TEST:
      {
        ARS3D_INFO("Starting up: Triangle Render");
        RenderTriangleTestLayer *layer = (RenderTriangleTestLayer *)ars3dMalloc(sizeof(RenderTriangleTestLayer));
        ars3dAppAttachLayer(
            ars3dAppGet(), layer, onRenderTriangleTestLayerAttach, onRenderTriangleTestLayerDetatch, 
            onRenderTriangleTestLayerStart, onRenderTriangleTestLayerUpdate, onRenderTriangleTestLayerEvent
        );
        break;
      }

      case LAUNCHER_LAUNCH_BARNSLEY_FRACTAL:
      {
        ARS3D_INFO("Starting up: Barnsley Fern");
        BarnsleyLayer *layer = (BarnsleyLayer *)ars3dMalloc(sizeof(BarnsleyLayer));
        ars3dAppAttachLayer(
            ars3dAppGet(), layer, onBarnsleyLayerAttach, onBarnsleyLayerDetatch, 
            onBarnsleyLayerStart, onBarnsleyLayerUpdate, onBarnsleyLayerEvent
        );
        break;
      }

      case LAUNCHER_LAUNCH_CUBE:
      {
        ARS3D_INFO("Starting up: Cube");
        CubeLayerCtx *layer = cubeLayerCreate();
        ars3dAppAttachLayer(
            ars3dAppGet(), layer, cubeLayerOnAttach,
            cubeLayerOnDetatch, cubeLayerOnStart,
            cubeLayerOnUpdate, cubeLayerOnEvent
        );
        break;
      }

      case LAUNCHER_LAUNCH_EXIT:
        ARS3D_INFO("Exiting...");
        __ars3dAppDestroy__(ars3dAppGet());
        ars3dExit(0);
        break;

      default:
        return;
    }
  }

  // Once a Demo is Launched, destroy this LAYER.
  else
  {
    ars3dAppDetatchLayer(ars3dAppGet(), ctx);
  }
}


LauncherLayerCtx *launcherLayerCreate()
{
  LauncherLayerCtx *new_ctx = (LauncherLayerCtx *)ars3dMalloc( ARS3D_SIZEOF(LauncherLayerCtx) );

  if (!new_ctx) return ARS3D_NULL;

  return new_ctx;
}


