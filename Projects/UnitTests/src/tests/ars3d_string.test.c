#include "../core/unit_testing_framework.h"
#include <aristotle3D/core/ars3d_string.h>

ARS3D_DECLARE_UNIT_TEST(DynamicString, create_and_destroy)
{
    Ars3DDynamicStr *str1 = ars3dCreateDynamicStr("Hello String");

    ARS3D_UNIT_TEST_ASSERT(str1, "String allocation failed");

    ars3dDestroyDynamicStr(&str1);

    ARS3D_UNIT_TEST_ASSERT(str1 == ARS3D_NULL, "After deletion, placeholder should be NULL");
}


ARS3D_DECLARE_UNIT_TEST(DynamicString, get_c_string)
{
    Ars3DDynamicStr *str1 = ars3dCreateDynamicStr("Hello String");

    ars3d_char *c_str = ars3dGetCStr(str1); 

    ARS3D_UNIT_TEST_ASSERT(c_str, "Should return the valid char buffer");

    ars3dDestroyDynamicStr(&str1);
}


ARS3D_DECLARE_UNIT_TEST(DynamicString, length_of_string)
{
    Ars3DDynamicStr *str1 = ars3dCreateDynamicStr("Hello String");

    ARS3D_UNIT_TEST_ASSERT(ars3dLenDynamicStr(str1) == 12);

    ars3dDestroyDynamicStr(&str1);
}


ARS3D_DECLARE_UNIT_TEST(DynamicString, compare_strings)
{
    Ars3DDynamicStr *str_a1 = ars3dCreateDynamicStr("a");
    Ars3DDynamicStr *str_a2 = ars3dCreateDynamicStr("a");
    Ars3DDynamicStr *str_b0 = ars3dCreateDynamicStr("b");
    Ars3DDynamicStr *str_c0 = ars3dCreateDynamicStr("c");


    ARS3D_UNIT_TEST_ASSERT(ars3dCompareDynamicStr(str_a1, str_a2) == 0, "%s should be equal to %s",
    ars3dGetCStr(str_a1), ars3dGetCStr(str_a2)
    );

    ARS3D_UNIT_TEST_ASSERT(ars3dCompareDynamicStr(str_a1, str_b0) < 0, "%s should be less than %s",
    ars3dGetCStr(str_a1), ars3dGetCStr(str_b0)
    );

    ARS3D_UNIT_TEST_ASSERT(ars3dCompareDynamicStr(str_c0, str_b0) > 0, "%s should be greater than %s",
    ars3dGetCStr(str_c0), ars3dGetCStr(str_b0)
    );

    ars3dDestroyDynamicStr(&str_a1);
    ars3dDestroyDynamicStr(&str_a2);
    ars3dDestroyDynamicStr(&str_b0);
    ars3dDestroyDynamicStr(&str_c0);
}
