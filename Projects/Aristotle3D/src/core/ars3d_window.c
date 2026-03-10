#include <aristotle3D/core/ars3d_window.h>
#include <aristotle3D/core/ars3d_string.h>
#include <aristotle3D/core/ars3d_debug.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

typedef struct Ars3DWindow
{
    GLFWwindow      *   m_glfw_window;
    Ars3DDynamicStr *   m_title;
    ars3d_int           m_width;
    ars3d_int           m_height;
} Ars3DWindow;


Ars3DWindow *ars3dCreateWindow(const ars3d_char *p_title, ars3d_int p_width, ars3d_int p_height)
{
    // Try to initialize GLFW.
    if (!glfwInit())
    {
        ars3dPrintf("[Ars3D Error]: Failed to initialize GLFW.\n");
        return ARS3D_NULL;
    }

    // OpenGL CORE Profile version 3.3 (GLFW Hint).
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a GLFW window.
    GLFWwindow *glfw_window = glfwCreateWindow(p_width, p_height, p_title, NULL, NULL);
    if (!glfw_window)
    {
        ars3dPrintf("[Ars3D Error]: GLFW could not create the window.\n");
        glfwTerminate();
        return ARS3D_NULL;
    }

    // Make the window's context (OpenGL Context).
    glfwMakeContextCurrent(glfw_window);

    
    // Initialize GLAD (Fetch all gl function pointers).
    if (!gladLoadGL( (GLADloadfunc) glfwGetProcAddress ))
    {
        ars3dPrintf("[Ars3D Error]: Failed to initialize GLAD.\n");
        glfwTerminate();
        return ARS3D_NULL;
    }

    // -------------Show some OpenGL Info------------- //
    ARS3D_INFO("OpenGL Version: %s", glGetString(GL_VERSION));
    // -------------Show some OpenGL Info------------- //

    // Set the gl viewport.
    glViewport(0, 0, p_width, p_height);

    // Create the Aristotle Window.
    Ars3DWindow *new_window = (Ars3DWindow *)ars3dMalloc( ARS3D_SIZEOF(Ars3DWindow) );

    // Not enough memory.
    if (!new_window)
    {
        ARS3D_WARN("Not enough memory");
        return ARS3D_NULL;
    }

    // Initialize the new instance.
    new_window->m_glfw_window   = glfw_window;
    new_window->m_width         = p_width;
    new_window->m_height        = p_height;

    if (p_title)
        new_window->m_title = ars3dCreateDynamicStr(p_title);

    else
        new_window->m_title = ARS3D_NULL;

    return new_window;
}



ars3d_void ars3dDestroyWindow(Ars3DWindow **p_window)
{
    if (!p_window || !(*p_window))
    {
        ARS3D_WARN("p_window is NULL or points to a NULL object");
        return;
    }

    glfwTerminate();

    if ( (*p_window)->m_title )
        ars3dDestroyDynamicStr( &(*p_window)->m_title );

    ARS3D_INFO("Destroying the Window...");
    ars3dFree(*p_window);

    *p_window = ARS3D_NULL;
}



ars3d_void ars3dUpdateWindow(Ars3DWindow *p_window)
{
    if (!p_window)
    {
        ARS3D_WARN("You called a fuction with a NULL value on a required parameter");
        return;
    }

    /* Swap front and back buffers */
    glfwSwapBuffers(p_window->m_glfw_window);

    /* Poll for and process events */
    glfwPollEvents();
}


ars3d_int ars3dShouldCloseWindow(Ars3DWindow *p_window)
{
    if (!p_window)
    {
        ARS3D_WARN("You called a fuction with a NULL value on a required parameter");
        return 1;
    }

    return glfwWindowShouldClose(p_window->m_glfw_window);
}
