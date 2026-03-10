#include "core/unit_testing_framework.h"

ARS3D_DECLARE_UNIT_TEST(DynamicString, compare_strings);
ARS3D_DECLARE_UNIT_TEST(DynamicString, length_of_string);
ARS3D_DECLARE_UNIT_TEST(DynamicString, get_c_string);
ARS3D_DECLARE_UNIT_TEST(DynamicString, create_and_destroy);
ARS3D_DECLARE_UNIT_TEST(Stdio, StringLength);
ARS3D_DECLARE_UNIT_TEST(Stdio, MallocSafety);

ars3d_int main(ars3d_int argc, ars3d_char **argv)
{
    Ars3DUnitTestingApp *app = ars3dCreateUnitTestingApp();

    ARS3D_REGISTER_UNIT_TEST(app, DynamicString, compare_strings);
    ARS3D_REGISTER_UNIT_TEST(app, DynamicString, length_of_string);
    ARS3D_REGISTER_UNIT_TEST(app, DynamicString, get_c_string);
    ARS3D_REGISTER_UNIT_TEST(app, DynamicString, create_and_destroy);
    ARS3D_REGISTER_UNIT_TEST(app, Stdio, MallocSafety);
    ARS3D_REGISTER_UNIT_TEST(app, Stdio, StringLength);

    ars3dUniTesingEntryPoint(app, argc, argv);

    ars3dDestroyUnitTestingApp(app);

    return 0;
}