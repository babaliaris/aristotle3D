#ifndef ARS3D_ENTRY_POINT_H
#define ARS3D_ENTRY_POINT_H
#include <aristotle3D/core/ars3d_stdio.h>
#include <aristotle3D/core/ars3d_application.h>

#ifdef ARS3D_ENTRY_POINT

    #if defined(ARS3D_LINUX)
    int main(ars3d_int argc, ars3d_char **argv)
    {
        // Get the app instance by calling User's Entry Point.
        Ars3DApp *app = ars3dUserAppProvider();

        // BootUp the engine and return to the OS once finished.
        return __ars3dBootUp__(app);
    }

    #elif defined(ARS3D_WINDOWS)
    #include <windows.h>

    int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
    {
        return ars3d_GLFW_TEST();
    }
    #endif

#endif

#endif
