#include "core/unit_testing_framework.h"

ARS3D_DECLARE_UNIT_TEST(Stdio, StringLength);
ARS3D_DECLARE_UNIT_TEST(Stdio, MallocSafety);

ars3d_void main(ars3d_int argc, ars3d_char **argv)
{
    Ars3DUnitTestingApp *app = ars3dCreateUnitTestingApp();

    ARS3D_REGISTER_UNIT_TEST(app, Stdio, MallocSafety);
    ARS3D_REGISTER_UNIT_TEST(app, Stdio, StringLength);

    ars3dUniTesingEntryPoint(app, argc, argv);

    ars3dDestroyUnitTestingApp(app);
}