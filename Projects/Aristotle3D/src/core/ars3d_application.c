#include <aristotle3D/core/ars3d_application.h>
#include <aristotle3D/core/ars3d_debug.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>


typedef struct Ars3DApp
{
    ars3d_char          m_window_title[ARS3D_APP_MAX_STRING];
    ars3d_int           m_window_width;
    ars3d_int           m_window_height;
} Ars3DApp;




Ars3DApp *ars3dCreateApp(
    const ars3d_char *  p_window_title,
    ars3d_int           p_window_width,
    ars3d_int           p_window_height
)
{
    Ars3DApp *new_app = (Ars3DApp *)ars3dMalloc(ARS3D_SIZEOF(Ars3DApp));

    if (!new_app) return ARS3D_NULL;

    if (ars3dStrLen(p_window_title) > ARS3D_APP_MAX_STRING - 1)
    {
        ars3dPrintf("[Ars3DApp WARN]: window_title is more than %d characters, truncating...\n", ARS3D_APP_MAX_STRING - 1);
        ars3dMemCopy(new_app->m_window_title, p_window_title, ARS3D_APP_MAX_STRING - 1);
        new_app->m_window_title[ARS3D_APP_MAX_STRING - 1] = '\0';
    }

    else
    {
        ars3dStrCopy(new_app->m_window_title, p_window_title);
    }

    new_app->m_window_width     = p_window_width;
    new_app->m_window_height    = p_window_height;

    return new_app;
}




ars3d_void __ars3dDestroyApp__(Ars3DApp *p_app)
{
    if (!p_app) return;

    ARS3D_INFO("Destroying the Application...");

    ars3dFree(p_app);
}




ars3d_void __ars3dInitSubSystems__(Ars3DApp *p_app)
{
    ARS3D_INFO("Initializing Subsystems...");
    ars3dInitDebugConsole();
}




ars3d_int __ars3dMainLoop__(Ars3DApp *p_app)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
    {
        ars3dPrintf("[Ars3D Error]: Failed to initialize GLFW.\n");
        return -1;
    }

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(p_app->m_window_width, p_app->m_window_height, p_app->m_window_title, NULL, NULL);
    if (!window)
    {
        ars3dPrintf("[Ars3D Error]: GLFW could not create the window.\n");
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    
    if (!gladLoadGL( (GLADloadfunc) glfwGetProcAddress ))
    {
        ars3dPrintf("[Ars3D Error]: Failed to initialize GLAD.\n");
        glfwTerminate();
        return -1;
    }

    ARS3D_INFO("OpenGL Version: %s", glGetString(GL_VERSION));

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}


ars3d_int __ars3dBootUp__(Ars3DApp *p_app)
{
    ARS3D_INFO("Booting up Aristotle3D...");

    // Initialize the SubSystems of the engine.
    __ars3dInitSubSystems__(p_app);

    // Call the user's entry point.
    ars3dUserEntryPoint(p_app);

    // Start the main Loop.
    ars3d_int exit_value = __ars3dMainLoop__(p_app);

    // Destroy the APP.
    __ars3dDestroyApp__(p_app);

    return exit_value;
}
