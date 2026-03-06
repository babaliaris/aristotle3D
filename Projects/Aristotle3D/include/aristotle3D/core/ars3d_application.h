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
 * @brief Create a new Application.
 * 
 * This function is supposed to be called within
 * ars3DUserEntryPoint() where the user MUST define.
 * 
 * @param p_window_title The window title.
 * @param p_window_width The window width.
 * @param p_window_height The window height.
 * 
 * @return The memory address of the newly created application.
 */
ARS3D_API Ars3DApp *ars3DCreateApp(
    const ars3d_char *  p_window_title,
    ars3d_int           p_window_width,
    ars3d_int           p_window_height
);


/**
 * @brief Destroy the application.
 * 
 * USER MUST NOT USE THIS FUNCION. The entry point of
 * Aristotle3D will make sure to destroy the app instance
 * AFTER the user returns it from ars3DUserEntryPoint()
 * 
 * @param p_app The memory address of the application.
 */
ARS3D_API ars3d_void ars3DDestroyApp(Ars3DApp *p_app);


/**
 * @brief Main Loop
 * 
 * USER MUST NOT USE THIS FUNCION.It is called automatically
 * by Aristotle3D entry point.
 * 
 * @param p_app The memory address of the application.
 */
ARS3D_API ars3d_int ars3DMainLoop(Ars3DApp *p_app);


/**
 * @brief Aristotle3D User Entry.
 * 
 * This function MUST be implemented by the user.
 * The user should call ars3DCreateApp() to start
 * the boot process of the engine and return it so
 * the entry point can handle the rest.
 * 
 * @returns The memory address of the newly created application.
 */
Ars3DApp * ars3DUserEntryPoint();

#endif
