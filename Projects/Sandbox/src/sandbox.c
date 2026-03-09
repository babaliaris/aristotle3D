#define ARS3D_ENTRY_POINT
#include <aristotle3D/aristotle3D.h>

ars3d_void ars3dUserEntryPoint(Ars3DApp *p_app)
{
}

Ars3DApp * ars3dUserAppProvider()
{
    return ars3dCreateApp("SANDBOX", 512, 512);
}