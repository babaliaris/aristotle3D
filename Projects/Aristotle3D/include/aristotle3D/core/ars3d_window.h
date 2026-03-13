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
ARS3D_API Ars3DWindow *ars3dWindowCreate(const ars3d_char *p_title, ars3d_int p_width, ars3d_int p_height);



/**
 * @brief Destroy a window.
 * 
 * Free's the memory that the placeholder points at
 * and then it sets it to NULL.
 * 
 * @param p_window The user's placeholder memory address.
 */
ARS3D_API ars3d_void ars3dWindowDestroy(Ars3DWindow **p_window);



/**
 * @brief Swap the render buffers.
 * 
 * Swaps the buffers (double buffering).
 * 
 * @param p_window The window instance.
 */
ARS3D_API ars3d_void ars3dWindowSwapBuffers(Ars3DWindow *p_window);



/**
 * @brief Poll the events.
 */
ARS3D_API ars3d_void ars3dWindowPollEvents();


/**
 * @brief Clean the window render buffers.
 */
ARS3D_API ars3d_void ars3dWindowCleanBuffers();



/**
 * @brief Get the native window (OS level).
 * 
 * @param p_window The window instance.
 * 
 * @returns The instance of the native window object.
 */
ARS3D_API ars3d_void *ars3dWindowGetNativeWindow(Ars3DWindow *p_window);


/**
 * @brief Set the graphics viewport (drawing area).
 * 
 * @param p_window The window instance.
 * @param p_x The x position of the viewport window.
 * @param p_y The y position of the viewport window.
 * @param p_width The width of the viewport.
 * @param p_height The height of the viewport.
 */
ARS3D_API ars3d_void ars3dWindowSetViewport(ars3d_int p_x, ars3d_int p_y, ars3d_int p_width, ars3d_int p_height);



/**
 * @brief Get the window Size.
 * 
 * @param p_window The window instance.
 * @param p_width The user's memory address to store the width.
 * @param p_height The user's memory address to store the height.
 */
ARS3D_API ars3d_void ars3dWindowGetSize(Ars3DWindow *p_window, ars3d_int *p_width, ars3d_int *p_height);



/**
 * @brief Get the keyboard key.
 * 
 * @param p_event The event instance.
 * 
 * @returns The key code that has been pressed.
 */
ARS3D_API ars3d_int ars3dEventGetKeyboardKey(ars3d_void *p_event);



/**
 * @brief Get the mouse button.
 * 
 * @param p_event The event instance.
 * 
 * @returns The mouse button code that has been pressed.
 */
ARS3D_API ars3d_int ars3dEventGetMouseButton(ars3d_void *p_event);


/**
 * @brief Get the mouse position.
 * 
 * @param p_event The event instance.
 * @param p_x The user's memory address to store the x position.
 * @param p_y The user's memory address to store the y position.
 */
ARS3D_API ars3d_void ars3dEventGetMousePos(ars3d_void *p_event, ars3d_double *p_x, ars3d_double *p_y);


/**
 * @brief Get the window size.
 * 
 * @param p_event The event instance.
 * @param p_width The user's memory address to store the width.
 * @param p_height The user's memory address to store the height.
 */
ARS3D_API ars3d_void ars3dEventGetWindowSize(ars3d_void *p_event, ars3d_int *p_width, ars3d_int *p_height);

#endif