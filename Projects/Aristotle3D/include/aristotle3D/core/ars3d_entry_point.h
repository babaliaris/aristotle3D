#ifndef ARS3D_ENTRY_POINT_H
#define ARS3D_ENTRY_POINT_H
#include <aristotle3D/core/ars3d_stdio.h>
#include <aristotle3D/core/ars3d_application.h>

#ifdef ARS3D_ENTRY_POINT

    #if defined(ARS3D_LINUX)
    int main(ars3d_int argc, ars3d_char **argv)
    {
        // Get the app instance by calling the User's App Provider.
        Ars3DApp *app = ars3dUserAppProvider();

        // BootUp the engine and return to the OS once finished.
        return __ars3dBootUp__(app, argc, argv);
    }

    #elif defined(ARS3D_WINDOWS)
    int main(ars3d_int argc, ars3d_char** argv)
    {
        // Get the app instance by calling the User's App Provider.
        Ars3DApp* app = ars3dUserAppProvider();

        // BootUp the engine and return to the OS once finished.
        return __ars3dBootUp__(app, argc, argv);
    }
    #endif

#endif

#endif
