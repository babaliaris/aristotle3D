#ifndef ARS3D_APPLICATION_H
#define ARS3D_APPLICATION_H
#include <aristotle3D/core/ars3d_stdio.h>

/**
 * @file
 * @brief The backbone of Aristotle3D
 * 
 * This is where everything starts.
 */

#define ARS3D_APP_MAX_STRING 64


typedef struct Ars3DApp Ars3DApp;


/**
 * @brief Provides the user generated app to the entry point of Aristotle3D.
 * 
 * This function MUST be implemented by the user.
 * The user should call ars3DCreateApp() to start
 * create a new Aristotle3D Application. This is called
 * automatically by Aristotle3D Entry Point.
 * 
 * @returns The memory address of the newly created application.
 */
Ars3DApp * ars3dUserAppProvider();


/**
 * @brief Aristotle3D User Entry.
 * 
 * This function MUST be implemented by the user.
 * It provides the entry point of the user's code
 * after the Application has finished the boot
 * process and before entering the MainLoop.
 * 
 * @param p_app The memory address of the application.
 * 
 * @returns The memory address of the newly created application.
 */
ars3d_void ars3dUserEntryPoint(Ars3DApp *p_app);



/**
 * @brief Create a new Application.
 * 
 * This function is supposed to be called within
 * ars3dUserAppProvider() where the user MUST define.
 * 
 * @param p_window_title The window title.
 * @param p_window_width The window width.
 * @param p_window_height The window height.
 * 
 * @return The memory address of the newly created application.
 */
ARS3D_API Ars3DApp *ars3dCreateApp(
    const ars3d_char *  p_window_title,
    ars3d_int           p_window_width,
    ars3d_int           p_window_height
);



/**
 * @brief Boot up the Aristotle3D engine.
 * 
 * USER MUST NOT USE THIS FUNCION.It is called automatically
 * by Aristotle3D entry point.
 * 
 * @param p_app The memory address of the application.
 */
ARS3D_API ars3d_int __ars3dBootUp__(Ars3DApp *p_app);

#endif
