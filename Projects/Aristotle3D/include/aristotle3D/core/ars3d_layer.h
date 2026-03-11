#ifndef ARS3D_LAYER_H
#define ARS3D_LAYER_H
#include <aristotle3D/core/ars3d_stdio.h>

#define ARS3D_LAYER_CB_ON_ATTACH 0
#define ARS3D_LAYER_CB_ON_DETATCH 1
#define ARS3D_LAYER_CB_ON_START 2
#define ARS3D_LAYER_CB_ON_UPDATE 3

typedef struct Ars3DApp Ars3DApp;
typedef struct Ars3DLayer Ars3DLayer;

/**
 * @brief On Attach callback function signature.
 * 
 * This is called automatically by the Aristotle3D engine
 * when the layer is attached to the layers list.
 * 
 * @param p_user_ctx The user's context object.
 */
typedef ars3d_void (*Ars3DLayerOnAttachFN)(ars3d_void *p_user_ctx);

/**
 * @brief On Detatch callback function signature.
 * 
 * This is called automatically by the Aristotle3D engine
 * when the layer is removed from the layers list.
 * 
 * @param p_user_ctx The user's context object.
 */
typedef ars3d_void (*Ars3DLayerOnDetachFN)(ars3d_void *p_user_ctx);

/**
 * @brief On Start callback function signature.
 * 
 * This is called automatically by the Aristotle3D engine
 * ONLY ONCE after the Main Loop of the engine has started.
 * 
 * @param p_user_ctx The user's context object.
 */
typedef ars3d_void (*Ars3DLayerOnStartFN)(ars3d_void *p_user_ctx);

/**
 * @brief On Update callback function signature.
 * 
 * This is called automatically by the Aristotle3D engine
 * EVERY FRAME after the Ars3DLayerOnStartFN has been called.
 * 
 * @param p_user_ctx The user's context object.
 */
typedef ars3d_void (*Ars3DLayerOnUpdateFN)(ars3d_void *p_user_ctx);




/**
 * @brief Create a new Layer Object.
 * 
 * This is meant to be used by the Aristotle3D engine
 * and NOT by the user!!!
 * 
 * @param p_user_ctx The user's context object.
 * @param p_onAttach The User's on attach function callback.
 * @param p_onDetachThe User's on detatch function callback.
 * @param p_onStart User's on start function callback.
 * @param p_onUpdate  User's on update function callback.
 * 
 * @returns The newly created layer instance.
 */
ARS3D_API Ars3DLayer *__ars3dLayerCreate__(
    ars3d_void *            p_user_ctx,
    Ars3DLayerOnAttachFN    p_onAttach,
    Ars3DLayerOnDetachFN    p_onDetach,
    Ars3DLayerOnStartFN     p_onStart,
    Ars3DLayerOnUpdateFN    p_onUpdate
);


/**
 * @brief Destroy a layer object.
 * 
 * This is meant to be used by the Aristotle3D engine
 * and NOT by the user!!!
 * 
 * @param p_layer The user's placeholder that contains the mem address of the Ars3DLayer
 */
ARS3D_API ars3d_void __ars3dLayerDestroy__(Ars3DLayer **p_layer);


/**
 * @brief Call a layer callback function.
 * 
 * This is meant to be used by the Aristotle3D engine
 * and NOT by the user!!!
 * 
 * @param p_layer The layer instance.
 * @param p_cb_option The enum options (which callback to call).
 */
ARS3D_API ars3d_void __ars3dLayerCallCB__(Ars3DLayer *p_layer, ars3d_int p_cb_option);



/**
 * @brief Get the User's context.
 * 
 * This is meant to be used by the Aristotle3D engine
 * and NOT by the user!!!
 * 
 * @param p_layer The layer instance.
 * 
 * @returns Th user's context object.
 */
ARS3D_API ars3d_void *__ars3dLayerGetUserCtx__(Ars3DLayer *p_layer);

#endif
