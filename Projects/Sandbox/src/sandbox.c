#define ARS3D_ENTRY_POINT
#include <aristotle3D/aristotle3D.h>

Ars3DApp * ars3DUserEntryPoint()
{
    Ars3DApp *app = ars3DCreateApp("SANDBOX", 512, 512);

    return app;
}