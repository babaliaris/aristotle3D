#define ARS3D_ENTRY_POINT
#include <aristotle3D/aristotle3D.h>
#include "layers/launcher.h"

ars3d_void ars3dUserEntryPoint(Ars3DApp *p_app)
{
  ars3dAppAttachLayer(
      p_app,
      launcherLayerCreate(),
      launcherLayerOnAttach,
      launcherLayerOnDetatch,
      ARS3D_NULL,
      launcherLayerOnUpdate,
      ARS3D_NULL
  );
}


Ars3DApp * ars3dUserAppProvider()
{
    return ars3dAppCreate(ars3dUserEntryPoint, "SANDBOX", 512, 512);
}
