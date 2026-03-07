#include "../core/unit_testing_framework.h"

ARS3D_DECLARE_UNIT_TEST(Stdio, StringLength)
{
    ars3d_char * test_str   = "Aristotle";
    ars3d_size length       = ars3dStrLen(test_str);
    
    ARS3D_UNIT_TEST_ASSERT(length == 9, "Expected length 9, but got %zu", length);
}

ARS3D_DECLARE_UNIT_TEST(Stdio, MallocSafety)
{
    ars3d_void* ptr = ars3dMalloc(1024);
    
    ARS3D_UNIT_TEST_ASSERT(ptr != ARS3D_NULL, "Failed to allocate 1024 bytes");
    
    ars3dFree(ptr);
}
