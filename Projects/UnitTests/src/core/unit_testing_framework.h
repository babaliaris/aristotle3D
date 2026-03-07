#ifndef ARS3D_UNIT_TESTING_FRAMEWORK_H
#define ARS3D_UNIT_TESTING_FRAMEWORK_H
#include <aristotle3D/core/ars3d_debug.h>

#define ARS3D_UNIT_TESTING_MAX_STRING 128
#define ARS3D_UNIT_TESTING_MAX_TESTS 1024


typedef struct Ars3DUnitTest        Ars3DUnitTest;
typedef struct Ars3DUnitTestingApp  Ars3DUnitTestingApp;

typedef ars3d_void (*Ars3DUnitTestFN)(Ars3DUnitTest *__p_test__);

typedef struct Ars3DUnitTest
{
    Ars3DUnitTestFN m_unitTest;
    ars3d_char      m_test_name[ARS3D_UNIT_TESTING_MAX_STRING];
    ars3d_char      m_suite_name[ARS3D_UNIT_TESTING_MAX_STRING];
    ars3d_uchar     m_failed;
} Ars3DUnitTest;


/**
 * @brief Create a new Unit Testing Application.
 * 
 * @returns The memory address of the newly created Ars3DUnitTestingApp.
 */
Ars3DUnitTestingApp *ars3dCreateUnitTestingApp();


/**
 * @brief Destroys a Ars3DUnitTestingApp application.
 * 
 * @param p_app The memory address of the Ars3DUnitTestingApp instance.
 */
ars3d_void ars3dDestroyUnitTestingApp(Ars3DUnitTestingApp *p_app);



/**
 * @brief Appends a new test instance in the tests list of the framework.
 * 
 * @param p_app The application instance.
 * @param p_unitTest The function pointer of the test.
 * @param p_suite_name The suite name.
 * @param p_test_name The test name.
 */
ars3d_void __ars3dAddUnitTest__(
    Ars3DUnitTestingApp *   p_app,
    Ars3DUnitTestFN         p_unitTest,
    const ars3d_char *      p_suite_name,
    const ars3d_char *      p_test_name
);


/**
 * @brief The entry point of the Unit Testing Framework.
 * 
 * @param p_app The application instance.
 * @param argc The amount of arguments passed.
 * @param argv The list of arguments.
 */
ars3d_void ars3dUniTesingEntryPoint(Ars3DUnitTestingApp *p_app, ars3d_int argc, ars3d_char **argv);


#define ARS3D_DECLARE_UNIT_TEST(suite_name, test_name)\
    ars3d_void suite_name##_##test_name(Ars3DUnitTest *__p_test__)

#define ARS3D_REGISTER_UNIT_TEST(app, suite_name, test_name)\
    __ars3dAddUnitTest__(app, suite_name##_##test_name, #suite_name, #test_name)

#define ARS3D_UNIT_TEST_ASSERT(cond, ...)\
    do\
    {\
        if (!(cond))\
        {\
            __ars3dColorfulDebugHelper__(\
                __p_test__->m_suite_name, __p_test__->m_test_name, __FILE__, __LINE__, ARS3D_COLOR_B_RED, \
                "Condition failed: [" #cond "] " __VA_ARGS__\
            );\
            __p_test__->m_failed = 1;\
            return;\
        }\
    } while (0)

#endif
