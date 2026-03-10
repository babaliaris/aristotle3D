#include "unit_testing_framework.h"
#include <aristotle3D/core/ars3d_application.h>

// Define them to satisfy Aristotle3D. Since we do not
// #define ARS3D_ENTRY_POINT anywhere, these do no matter.
ars3d_void ars3dUserEntryPoint(Ars3DApp *p_app){}
Ars3DApp * ars3dUserAppProvider(){return ARS3D_NULL;}



typedef struct Ars3DUnitTestingApp
{
    ars3d_size          m_total_tests;
    ars3d_size          m_total_run;
    ars3d_size          m_total_skipped;
    ars3d_size          m_failed_tests;
    ars3d_int           m_argc;
    ars3d_char **       m_argv;
    Ars3DUnitTest *     m_tests[ARS3D_UNIT_TESTING_MAX_TESTS];
} Ars3DUnitTestingApp;



Ars3DUnitTestingApp *ars3dCreateUnitTestingApp()
{
    Ars3DUnitTestingApp *new_app = (Ars3DUnitTestingApp *)ars3dMalloc( ARS3D_SIZEOF(Ars3DUnitTestingApp) );

    if (!new_app)
    {
        ARS3D_USER_FATAL("ars3dCreateUnitTestingApp() failed. MAX HEAP MEMORY REACHED!");
        return ARS3D_NULL;
    }

    new_app->m_total_tests      = 0;
    new_app->m_total_run        = 0;
    new_app->m_total_skipped    = 0;
    new_app->m_failed_tests     = 0;

    return new_app;
}


ars3d_void ars3dDestroyUnitTestingApp(Ars3DUnitTestingApp *p_app)
{
    if (!p_app) return;

    // Free all the test objects.
    for (ars3d_size i = 0; i < p_app->m_total_tests; i++)
    {
        if (p_app->m_tests[i]) ars3dFree(p_app->m_tests[i]);
    }

    ars3dFree(p_app);
}


ars3d_void __ars3dAddUnitTest__(
    Ars3DUnitTestingApp *   p_app,
    Ars3DUnitTestFN         p_unitTest,
    const ars3d_char *      p_suite_name,
    const ars3d_char *      p_test_name
)
{
    // Not a valid app object || test callback.
    if (!p_unitTest || !p_app)
    {
        ARS3D_WARN("ars3dAddUnitTest() was called without providing a valid p_app or p_unitTest.");
        return;
    }

    // ARS3D_UNIT_TESTING_MAX_STRING - 2 must be the length without the \0 char.
    else if (ars3dStrLen(p_suite_name) > ARS3D_UNIT_TESTING_MAX_STRING - 2 || ars3dStrLen(p_test_name) > ARS3D_UNIT_TESTING_MAX_STRING - 2)
    {
        ARS3D_ERROR("ars3dAddUnitTest() p_suite_name or p_test_name is more than %d characters. Test will be skipped!", ARS3D_UNIT_TESTING_MAX_STRING - 2);
        p_app->m_total_skipped++;
        return;
    }

    // Unit testing list is full.
    else if (p_app->m_total_tests > ARS3D_UNIT_TESTING_MAX_TESTS - 1)
    {
        ARS3D_ERROR("The unit testing framework is full (MAX TESTS: %d). Test will be skipped!", ARS3D_UNIT_TESTING_MAX_TESTS - 1);
        p_app->m_total_skipped++;
        return;
    }

    // Allocate memory for the new test.
    Ars3DUnitTest *new_test =  (Ars3DUnitTest *)ars3dMalloc( ARS3D_SIZEOF(Ars3DUnitTest) );

    // Return if there is no enough memory.
    if (!new_test)
    {
        ARS3D_USER_FATAL("__ars3dAddUnitTest__() failed. MAX HEAP MEMORY REACHED!");
        p_app->m_total_skipped++;
        return;
    }
    
    // Initialize the new test.
    new_test->m_unitTest = p_unitTest;
    ars3dStrCopy(new_test->m_suite_name, p_suite_name);
    ars3dStrCopy(new_test->m_test_name, p_test_name);
    new_test->m_failed = 0;

    // Append the test in the array.
    p_app->m_tests[p_app->m_total_tests] = new_test;
    p_app->m_total_tests++;
}



ars3d_void ars3dUniTesingEntryPoint(Ars3DUnitTestingApp *p_app, ars3d_int argc, ars3d_char **argv)
{
    if (!p_app)
    {
        ARS3D_WARN("ars3dUniTesingEntryPoint() was called without providing a valid p_app.");
        return;
    }

    p_app->m_argc = argc;
    p_app->m_argv = argv;

    // Loop through all the tests.
    for (ars3d_size i = 0; i < p_app->m_total_tests; i++)
    {
        // Call the test.
        p_app->m_tests[i]->m_unitTest( p_app->m_tests[i] );
        p_app->m_total_run++;

        // If the test failed, increase the number of failed tests.
        if (p_app->m_tests[i]->m_failed) p_app->m_failed_tests++;
    }

    if (p_app->m_failed_tests == 0)
        ars3dPrintf(ARS3D_COLOR_B_GREEN "\nALL TESTS PASSED SUCCESSFULLY!\n\n" ARS3D_COLOR_RESET);

    else ars3dPrintf(ARS3D_COLOR_B_RED "\nTESTS FAILED!\n\n" ARS3D_COLOR_RESET);

    ars3dPrintf(ARS3D_COLOR_B_CYAN "Total Tests  : " ARS3D_COLOR_R_GREEN "%d\n" ARS3D_COLOR_RESET, p_app->m_total_tests);
    ars3dPrintf(ARS3D_COLOR_B_CYAN "Total Run    : " ARS3D_COLOR_R_GREEN "%d\n" ARS3D_COLOR_RESET, p_app->m_total_run);
    ars3dPrintf(ARS3D_COLOR_B_CYAN "Total Failed : " ARS3D_COLOR_R_RED "%d\n" ARS3D_COLOR_RESET,   p_app->m_failed_tests);
    ars3dPrintf(ARS3D_COLOR_B_CYAN "Total Passed : " ARS3D_COLOR_R_GREEN "%d\n" ARS3D_COLOR_RESET, p_app->m_total_run - p_app->m_failed_tests);
    ars3dPrintf(ARS3D_COLOR_B_CYAN "Total Skipped: " ARS3D_COLOR_R_GREEN "%d\n" ARS3D_COLOR_RESET, p_app->m_total_skipped);
}

