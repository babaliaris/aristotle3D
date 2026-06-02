#include "phong_lighting_testing_ground.h"



ars3d_void onPhongLightingTestingGroundLayerAttach(ars3d_void *p_ctx)
{
  ARS3D_UNUSED(p_ctx);
}

ars3d_void onPhongLightingTestingGroundLayerDetatch(ars3d_void *p_ctx)
{
  PhongLightingTestingGroundLayer *ctx = (PhongLightingTestingGroundLayer *)p_ctx;

  ars3dFree(ctx);
}

ars3d_void onPhongLightingTestingGroundLayerStart(ars3d_void *p_ctx)
{
  ARS3D_UNUSED(p_ctx);
  ARS3D_INFO("[PhongLightingTestingGroundLayer]: onStart()");
}

ars3d_void onPhongLightingTestingGroundLayerUpdate(ars3d_void *p_ctx, ars3d_float p_delta_time)
{
  ARS3D_UNUSED(p_ctx);
  ARS3D_UNUSED(p_delta_time);
}

ars3d_uchar onPhongLightingTestingGroundLayerEvent(ars3d_void *p_ctx, ars3d_void *p_event, ars3d_int p_event_type)
{
  ARS3D_UNUSED(p_ctx);
  ARS3D_UNUSED(p_event);
  ARS3D_UNUSED(p_event_type);

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
