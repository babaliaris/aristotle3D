#ifndef SANDBOX_LAYER_LAUNCHER_H
#define SANDBOX_LAYER_LAUNCHER_H

#include <aristotle3D/aristotle3D.h>

#define LAUNCHER_LAUNCH_NONE                         -2
#define LAUNCHER_LAUNCH_EXIT                         -1
#define LAUNCHER_LAUNCH_EVENTS_TEST                   0
#define LAUNCHER_LAUNCH_RENDER_TRIANGLE_TEST          1
#define LAUNCHER_LAUNCH_BARNSLEY_FRACTAL              2
#define LAUNCHER_LAUNCH_CUBE                          3
#define LAUNCHER_LAUNCH_PHONG_LIGHTING_TESTING_GROUND 4
#define LAUNCHER_LAUNCH_PROJECT_3                     5

typedef struct
{
  ars3d_int m_layer;
} LauncherLayerCtx;


ars3d_void launcherLayerOnAttach(ars3d_void *p_ctx);

ars3d_void launcherLayerOnDetatch(ars3d_void *p_ctx);

ars3d_void launcherLayerOnUpdate(ars3d_void *p_ctx, ars3d_float p_delta_time);


LauncherLayerCtx *launcherLayerCreate();

#endif

