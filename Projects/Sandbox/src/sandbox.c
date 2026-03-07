#define ARS3D_ENTRY_POINT
#include <aristotle3D/aristotle3D.h>

ars3d_void ars3dUserEntryPoint(Ars3DApp *p_app)
{
    //ars3dDebugTestMacros();
    ARS3D_ASSERT_SOFT(1 == 2, "%s", "1 is not equal to 2");
}

Ars3DApp * ars3dUserAppProvider()
{
    return ars3dCreateApp("SANDBOX", 512, 512);
}