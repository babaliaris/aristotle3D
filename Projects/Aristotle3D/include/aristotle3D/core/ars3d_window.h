#ifndef ARS3D_WINDOW_H
#define ARS3D_WINDOW_H
#include <aristotle3D/core/ars3d_stdio.h>

typedef struct Ars3DWindow Ars3DWindow;


/**
 * @brief Create a new aristotle3D Window.
 * 
 * @param p_title The title of the window.
 * @param p_width The width of the window.
 * @param p_height The height of the window.
 * 
 * @returns The newly created window instance.
 */
ARS3D_API Ars3DWindow *ars3dCreateWindow(const ars3d_char *p_title, ars3d_int p_width, ars3d_int p_height);



/**
 * @brief Destroy a window.
 * 
 * Free's the memory that the placeholder points at
 * and then it sets it to NULL.
 * 
 * @param p_window The user's placeholder memory address.
 */
ARS3D_API ars3d_void ars3dDestroyWindow(Ars3DWindow **p_window);



/**
 * @brief Update a window.
 * 
 * Swaps the buffers (double buffering) and polls the events.
 * 
 * @param p_window The window instance.
 */
ARS3D_API ars3d_void ars3dUpdateWindow(Ars3DWindow *p_window);



/**
 * @brief Check's if the window should close (User event).
 * 
 * @param p_window The window instance.
 * 
 * @returns 0 if the close event is NOT triggered, true otherwise.
 */
ARS3D_API ars3d_int ars3dShouldCloseWindow(Ars3DWindow *p_window);

#endif