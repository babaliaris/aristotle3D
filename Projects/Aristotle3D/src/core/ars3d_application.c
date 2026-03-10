#include <aristotle3D/core/ars3d_application.h>
#include <aristotle3D/core/ars3d_debug.h>
#include <glad/gl.h>
#include <aristotle3D/core/ars3d_window.h>


static Ars3DApp* STATIC_APP_INSTANCE = ARS3D_NULL;


typedef struct Ars3DApp
{
    Ars3DUserEntryPointFN   m_userEntry;
    Ars3DWindow *           m_window;
    ars3d_int               m_argc;
    ars3d_char **           m_argv;

} Ars3DApp;




Ars3DApp *ars3dCreateApp(
    Ars3DUserEntryPointFN p_userEntry,
    const ars3d_char *  p_window_title,
    ars3d_int           p_window_width,
    ars3d_int           p_window_height
)
{
    // Singletone pattern.
    if (STATIC_APP_INSTANCE != ARS3D_NULL)
    {
        ARS3D_WARN("ars3dCreateApp called while an instance already exists!");
        return STATIC_APP_INSTANCE;
    }

    Ars3DApp *new_app = (Ars3DApp *)ars3dMalloc(ARS3D_SIZEOF(Ars3DApp));

    // Not enough memory.
    if (!new_app)
    {
        ARS3D_WARN("Not enough memory");
        return ARS3D_NULL;
    }

    Ars3DWindow *new_window = ars3dCreateWindow(p_window_title, p_window_width, p_window_height);

    // Not enough memory.
    if (!new_window)
    {
        ARS3D_WARN("Not enough memory");
        ars3dFree(new_app);
        return ARS3D_NULL;
    }

    // Initialize the app.
    new_app->m_userEntry    = p_userEntry;
    new_app->m_window       = new_window;

    // Intialize the global static instance.
    STATIC_APP_INSTANCE = new_app;

    return new_app;
}




ars3d_void __ars3dDestroyApp__(Ars3DApp *p_app)
{
    if (!p_app)
    {
        ARS3D_WARN("p_app provided with a NULL value");
        return;
    }

    // Destroy the window.
    ars3dDestroyWindow(&p_app->m_window);

    ARS3D_INFO("Destroying the Application...");
    ars3dFree(p_app);

    STATIC_APP_INSTANCE = ARS3D_NULL;
}




ars3d_void __ars3dInitSubSystems__(Ars3DApp *p_app)
{
    if (!p_app)
    {
        ARS3D_WARN("You are trying to init the subsystems without providing an app instance!");
        return;
    }

    ARS3D_INFO("Initializing Subsystems...");
    ars3dInitDebugConsole();
}




ars3d_int __ars3dMainLoop__(Ars3DApp *p_app)
{
    while ( !ars3dShouldCloseWindow(p_app->m_window) )
    {
        glClear(GL_COLOR_BUFFER_BIT);

        ars3dUpdateWindow(p_app->m_window);
    }

    return 0;
}


ars3d_int __ars3dBootUp__(Ars3DApp *p_app, ars3d_int argc, ars3d_char **argv)
{
    if (!p_app)
    {
        ARS3D_WARN("You are trying to boot up the engine without providing an app instance!");
        return -1;
    }

    ARS3D_INFO("Booting up Aristotle3D...");

    // Save the arguments.
    p_app->m_argc = argc;
    p_app->m_argv = argv;

    // Initialize the SubSystems of the engine.
    __ars3dInitSubSystems__(p_app);

    // Call the user's entry point.
    p_app->m_userEntry(p_app);

    // Start the main Loop.
    ars3d_int exit_value = __ars3dMainLoop__(p_app);

    // Destroy the APP.
    __ars3dDestroyApp__(p_app);

    return exit_value;
}



Ars3DApp *ars3dGetAppInstance(ars3d_void)
{
    return STATIC_APP_INSTANCE;
}
