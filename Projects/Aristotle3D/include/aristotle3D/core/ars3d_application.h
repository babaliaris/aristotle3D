#ifndef ARS3D_APPLICATION_H
#define ARS3D_APPLICATION_H
#include <aristotle3D/core/ars3d_stdio.h>
#include <aristotle3D/core/ars3d_layer.h>

/**
 * @file
 * @brief The backbone of Aristotle3D
 * 
 * This is where everything starts.
 */


typedef struct Ars3DApp Ars3DApp;
typedef struct Ars3DWindow Ars3DWindow;


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
typedef ars3d_void (*Ars3DUserEntryPointFN)(Ars3DApp *p_app);



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
ARS3D_API Ars3DApp *ars3dAppCreate(
    Ars3DUserEntryPointFN   p_userEntry,
    const ars3d_char *      p_window_title,
    ars3d_int               p_window_width,
    ars3d_int               p_window_height
);


/**
 * @brief Get the App singletone instance.
 * 
 * @returns The app instance. 
 */
ARS3D_API Ars3DApp *ars3dAppGet(ars3d_void);



/**
 * @brief Attach a new Layer.
 * 
 * This function will create a new layer object and
 * attach it to the layers list. Usually YOU DO NOT
 * want multiple layers to share the same context
 * and callback functions but it is not forbidden.
 * 
 * @param p_app The app instance.
 * @param p_user_ctx The user's context object.
 * @param p_onAttach The User's on attach function callback.
 * @param p_onDetachThe User's on detatch function callback.
 * @param p_onStart User's on start function callback.
 * @param p_onUpdate  User's on update function callback.
 */
ARS3D_API ars3d_void ars3dAppAttachLayer(
    Ars3DApp *              p_app,
    ars3d_void *            p_user_ctx,
    Ars3DLayerOnAttachFN    p_onAttach,
    Ars3DLayerOnDetachFN    p_onDetach,
    Ars3DLayerOnStartFN     p_onStart,
    Ars3DLayerOnUpdateFN    p_onUpdate,
    Ars3DLayerOnEventFN     p_onEvent
);



/**
 * @brief Detatch a Layer.
 * 
 * This function uses the users context object in
 * order to determine which layer to detatch. If
 * more than one layers use the same context, all of them will
 * be detached. The detatched layers are being destroyed, so
 * make USE of the Ars3DLayerOnDetachFN callback to clean up
 * your context.
 * 
 * @param p_app The app instance.
 * @param p_user_ctx The user's context object.
 */
ARS3D_API ars3d_void ars3dAppDetatchLayer(Ars3DApp *p_app, ars3d_void *p_user_ctx);



/**
 * @brief Get the window object.
 * 
 * @param p_app The app instance.
 * 
 * @returns The window object.
 */
ARS3D_API Ars3DWindow *ars3dAppGetWindow(Ars3DApp *p_app);


/**
 * @brief Fires the event to all the layers in reverse order.
 * 
 * This function is meant to be called by the engine, no the user.
 * 
 * @param p_app The App instance.
 * @param p_event The event instance.
 * @param p_event_type The event type enum value.
 */
ARS3D_API ars3d_void __ars3dAppFireEvent__(Ars3DApp *p_app, ars3d_void *p_event, ars3d_int p_event_type);




/**
 * @brief Destroys the entire application.
 * 
 * USER MUST NOT USE THIS FUNCION.It is called automatically
 * by Aristotle3D wherever it is needed.
 * 
 * @param p_app The memory address of the application.
 */
ARS3D_API ars3d_void __ars3dAppDestroy__(Ars3DApp *p_app);



/**
 * @brief Boot up the Aristotle3D engine.
 * 
 * USER MUST NOT USE THIS FUNCION.It is called automatically
 * by Aristotle3D entry point.
 * 
 * @param p_app The memory address of the application.
 * @param argc The comman line paramateres amount.
 * @param argv The command line parameters.
 */
ARS3D_API ars3d_int __ars3dAppBootUp__(Ars3DApp *p_app, ars3d_int argc, ars3d_char **argv);

#endif
