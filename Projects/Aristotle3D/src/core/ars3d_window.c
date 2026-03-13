#include <aristotle3D/core/ars3d_window.h>
#include <aristotle3D/core/ars3d_string.h>
#include <aristotle3D/core/ars3d_debug.h>
#include <aristotle3D/core/opengl/ars3d_opengl_debugger.h>
#include <aristotle3D/core/ars3d_events.h>
#include <aristotle3D/core/ars3d_application.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

typedef struct Ars3DWindow
{
    GLFWwindow      *   m_glfw_window;
    Ars3DDynamicStr *   m_title;
    ars3d_int           m_width;
    ars3d_int           m_height;
} Ars3DWindow;


typedef struct Ars3DKeyboardEvent
{
    ars3d_int m_type;
    ars3d_int m_key;
} Ars3DKeyboardEvent;


typedef struct Ars3DMouseButtonEvent
{
    ars3d_int m_type;
    ars3d_int m_button;
}  Ars3DMouseButtonEvent;


typedef struct Ars3DMousePositionEvent
{
    ars3d_int m_type;
    ars3d_double m_x;
    ars3d_double m_y;
} Ars3DMousePositionEvent;


typedef struct Ars3DWindowSizeEvent
{
    ars3d_int m_type;
    ars3d_int m_width;
    ars3d_int m_height;
} Ars3DWindowSizeEvent;


static ars3d_void registerEventCallbacks(GLFWwindow *p_glfw_window);


Ars3DWindow *ars3dWindowCreate(const ars3d_char *p_title, ars3d_int p_width, ars3d_int p_height)
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
    ARS3D_OPENGL(glViewport(0, 0, p_width, p_height));

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


    // Set the new window instance as the GLFW User Pointer.
    glfwSetWindowUserPointer(glfw_window, (ars3d_void *)new_window);

    // Regester events.
    registerEventCallbacks(glfw_window);

    return new_window;
}



ars3d_void ars3dWindowDestroy(Ars3DWindow **p_window)
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



ars3d_void ars3dWindowSwapBuffers(Ars3DWindow *p_window)
{
    if (!p_window)
    {
        ARS3D_WARN("You called a fuction with a NULL value on a required parameter");
        return;
    }

    glfwSwapBuffers(p_window->m_glfw_window);
}


ars3d_void ars3dWindowPollEvents()
{
    glfwPollEvents();
}



ars3d_void ars3dWindowCleanBuffers()
{
    ARS3D_OPENGL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}



ars3d_void *ars3dWindowGetNativeWindow(Ars3DWindow *p_window)
{
    if (!p_window)
    {
        ARS3D_WARN("Required parameters are not provided");
        return ARS3D_NULL;
    }

    return (ars3d_void *)p_window->m_glfw_window;
}



ars3d_float ars3dWindowGetTime()
{
    return (float)glfwGetTime();
}



ars3d_void ars3dWindowSetViewport(ars3d_int p_x, ars3d_int p_y, ars3d_int p_width, ars3d_int p_height)
{
    ARS3D_OPENGL(glViewport(p_x, p_y, p_width, p_height));
}



ars3d_void ars3dWindowGetSize(Ars3DWindow *p_window, ars3d_int *p_width, ars3d_int *p_height)
{
    if (!p_window)
    {
        ARS3D_WARN("Required parameters are not provided");
        return;
    }

    if (p_width) *p_width   = p_window->m_width;
    if (p_height) *p_height = p_window->m_height;
}


ars3d_int ars3dEventGetKeyboardKey(ars3d_void *p_event)
{
    if (!p_event)
    {
        ARS3D_WARN("Required parameters are not provided");
        return ARS3D_EVENT_KEY_UNKNOWN;
    }

    Ars3DKeyboardEvent *event = (Ars3DKeyboardEvent *)p_event;

    ARS3D_ASSERT(
        event->m_type == ARS3D_EVENT_TYPE_KEYBOARD_PRESS ||
        event->m_type == ARS3D_EVENT_TYPE_KEYBOARD_RELEASE ||
        event->m_type == ARS3D_EVENT_TYPE_KEYBOARD_REPEAT
    , "You provided the wrong event object");

    return event->m_key;
}


ars3d_int ars3dEventGetMouseButton(ars3d_void *p_event)
{
    if (!p_event)
    {
        ARS3D_WARN("Required parameters are not provided");
        return ARS3D_EVENT_KEY_UNKNOWN;
    }

    Ars3DMouseButtonEvent *event = (Ars3DMouseButtonEvent *)p_event;

    ARS3D_ASSERT(
        event->m_type == ARS3D_EVENT_TYPE_MOUSE_BUTTON_PRESS ||
        event->m_type == ARS3D_EVENT_TYPE_MOUSE_BUTTON_RELEASE
    , "You provided the wrong event object");

    return event->m_button;
}


ars3d_void ars3dEventGetMousePos(ars3d_void *p_event, ars3d_double *p_x, ars3d_double *p_y)
{
    if (!p_event)
    {
        ARS3D_WARN("Required parameters are not provided");
        return;
    }

    Ars3DMousePositionEvent *event = (Ars3DMousePositionEvent *)p_event;

    ARS3D_ASSERT(
        event->m_type == ARS3D_EVENT_TYPE_MOUSE_POSITION
    , "You provided the wrong event object");

    if (p_x) *p_x = event->m_x;
    if (p_y) *p_y = event->m_y;
}


ars3d_void ars3dEventGetWindowSize(ars3d_void *p_event, ars3d_int *p_width, ars3d_int *p_height)
{
    if (!p_event)
    {
        ARS3D_WARN("Required parameters are not provided");
        return;
    }

    Ars3DWindowSizeEvent *event = (Ars3DWindowSizeEvent *)p_event;

    ARS3D_ASSERT(
        event->m_type == ARS3D_EVENT_TYPE_WINDOW_RESIZED
    , "You provided the wrong event object");

    if (p_width) *p_width = event->m_width;
    if (p_height) *p_height = event->m_height;
}



ars3d_void keyboardCB(GLFWwindow* window, ars3d_int key, ars3d_int scancode, ars3d_int action, ars3d_int mods);
ars3d_void mouseButtonCB(GLFWwindow* window, ars3d_int button, ars3d_int action, ars3d_int mods);
ars3d_void mousePositionCB(GLFWwindow* window, ars3d_double xpos, ars3d_double ypos);
ars3d_void windowCloseCB(GLFWwindow* window);
ars3d_void windowResizeCB(GLFWwindow* window, ars3d_int width, ars3d_int height);
ars3d_void windowMaximizeCB(GLFWwindow* window, ars3d_int maximized);
ars3d_void windowIconifiedCB(GLFWwindow* window, ars3d_int iconified);
ars3d_void windowFocusCB(GLFWwindow* window, ars3d_int focused);



static ars3d_void registerEventCallbacks(GLFWwindow *p_glfw_window)
{
    glfwSetKeyCallback(p_glfw_window, keyboardCB);
    glfwSetMouseButtonCallback(p_glfw_window, mouseButtonCB);
    glfwSetCursorPosCallback(p_glfw_window, mousePositionCB);
    glfwSetWindowCloseCallback(p_glfw_window, windowCloseCB);
    glfwSetWindowSizeCallback(p_glfw_window, windowResizeCB);
    glfwSetWindowMaximizeCallback(p_glfw_window, windowMaximizeCB);
    glfwSetWindowIconifyCallback(p_glfw_window, windowIconifiedCB);
    glfwSetWindowFocusCallback(p_glfw_window, windowFocusCB);
}


ars3d_void keyboardCB(GLFWwindow* window, ars3d_int key, ars3d_int scancode, ars3d_int action, ars3d_int mods)
{
    ARS3D_UNUSED(window);
    ARS3D_UNUSED(scancode);
    ARS3D_UNUSED(mods);

    Ars3DApp *app = ars3dAppGet();

    Ars3DKeyboardEvent event;
    event.m_key = key;

    switch (action)
    {
        case GLFW_PRESS:
        {
            event.m_type = ARS3D_EVENT_TYPE_KEYBOARD_PRESS;
            __ars3dAppFireEvent__(app, (ars3d_void *)&event, ARS3D_EVENT_TYPE_KEYBOARD_PRESS);
            break;
        }

        case GLFW_RELEASE:
        {
            event.m_type = ARS3D_EVENT_TYPE_KEYBOARD_RELEASE;
            __ars3dAppFireEvent__(app, (ars3d_void *)&event, ARS3D_EVENT_TYPE_KEYBOARD_RELEASE);
            break;
        }

        case GLFW_REPEAT:
        {
            event.m_type = ARS3D_EVENT_TYPE_KEYBOARD_REPEAT;
            __ars3dAppFireEvent__(app, (ars3d_void *)&event, ARS3D_EVENT_TYPE_KEYBOARD_REPEAT);
            break;
        }
        
        default:
            break;
    }
}



ars3d_void mouseButtonCB(GLFWwindow* window, ars3d_int button, ars3d_int action, ars3d_int mods)
{
    ARS3D_UNUSED(window);
    ARS3D_UNUSED(mods);

    Ars3DApp *app = ars3dAppGet();

    Ars3DMouseButtonEvent event;
    event.m_button = button;

    switch (action)
    {
        case GLFW_PRESS:
        {
            event.m_type = ARS3D_EVENT_TYPE_MOUSE_BUTTON_PRESS;
            __ars3dAppFireEvent__(app, (ars3d_void *)&event, ARS3D_EVENT_TYPE_MOUSE_BUTTON_PRESS);
            break;
        }

        case GLFW_RELEASE:
        {
            event.m_type = ARS3D_EVENT_TYPE_MOUSE_BUTTON_RELEASE;
            __ars3dAppFireEvent__(app, (ars3d_void *)&event, ARS3D_EVENT_TYPE_MOUSE_BUTTON_RELEASE);
            break;
        }
        
        default:
            break;
    }
}


ars3d_void mousePositionCB(GLFWwindow* window, ars3d_double xpos, ars3d_double ypos)
{
    ARS3D_UNUSED(window);

    Ars3DApp *app = ars3dAppGet();

    Ars3DMousePositionEvent event;
    event.m_type    = ARS3D_EVENT_TYPE_MOUSE_POSITION;
    event.m_x       = xpos;
    event.m_y       = ypos;

    __ars3dAppFireEvent__(app, (ars3d_void *)&event, event.m_type);
}


ars3d_void windowCloseCB(GLFWwindow* window)
{
    ARS3D_UNUSED(window);

    Ars3DApp *app = ars3dAppGet();

    __ars3dAppFireEvent__(app, ARS3D_NULL, ARS3D_EVENT_TYPE_WINDOW_CLOSED);
}


ars3d_void windowResizeCB(GLFWwindow* window, ars3d_int width, ars3d_int height)
{
    Ars3DWindow *ars3d_window   = (Ars3DWindow *)glfwGetWindowUserPointer(window);
    ars3d_window->m_width       = width;
    ars3d_window->m_height      = height;

    Ars3DApp *app = ars3dAppGet();

    Ars3DWindowSizeEvent event;
    event.m_type    = ARS3D_EVENT_TYPE_WINDOW_RESIZED;
    event.m_width   = width;
    event.m_height  = height;

    __ars3dAppFireEvent__(app, (ars3d_void *)&event, event.m_type);
}


ars3d_void windowMaximizeCB(GLFWwindow* window, ars3d_int maximized)
{
    ARS3D_UNUSED(window);

    Ars3DApp *app = ars3dAppGet();

    switch (maximized)
    {
        case GLFW_TRUE:
            __ars3dAppFireEvent__(app, ARS3D_NULL, ARS3D_EVENT_TYPE_WINDOW_MAXIMIZED);
            break;
        
        default:
        __ars3dAppFireEvent__(app, ARS3D_NULL, ARS3D_EVENT_TYPE_WINDOW_RESTORED);
            break;
    }
}


ars3d_void windowIconifiedCB(GLFWwindow* window, ars3d_int iconified)
{
    ARS3D_UNUSED(window);

    Ars3DApp *app = ars3dAppGet();

    switch (iconified)
    {
        case GLFW_TRUE:
            __ars3dAppFireEvent__(app, ARS3D_NULL, ARS3D_EVENT_TYPE_WINDOW_MINIMIZED);
            break;
        
        default:
        __ars3dAppFireEvent__(app, ARS3D_NULL, ARS3D_EVENT_TYPE_WINDOW_RESTORED);
            break;
    }
}


ars3d_void windowFocusCB(GLFWwindow* window, ars3d_int focused)
{
    ARS3D_UNUSED(window);

    Ars3DApp *app = ars3dAppGet();

    switch (focused)
    {
        case GLFW_TRUE:
            __ars3dAppFireEvent__(app, ARS3D_NULL, ARS3D_EVENT_TYPE_WINDOW_FOCUSED);
            break;
        
        default:
        __ars3dAppFireEvent__(app, ARS3D_NULL, ARS3D_EVENT_TYPE_WINDOW_UNFOCUSED);
            break;
    }
}
