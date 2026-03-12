#include <aristotle3D/core/ars3d_application.h>
#include <aristotle3D/core/ars3d_debug.h>
#include <aristotle3D/core/opengl/ars3d_opengl_debugger.h>
#include <aristotle3D/core/data-structures/ars3d_list.h>
#include <glad/gl.h>
#include <aristotle3D/core/ars3d_window.h>
#include <aristotle3D/core/ars3d_events.h>


#define APPLICATION_STATUS_RUNNING  0
#define APPLICATION_STATUS_PAUSED   1


static Ars3DApp* STATIC_APP_INSTANCE = ARS3D_NULL;


typedef struct Ars3DApp
{
    Ars3DUserEntryPointFN   m_userEntry;
    Ars3DWindow *           m_window;
    ars3d_int               m_argc;
    ars3d_char **           m_argv;
    Ars3DList *             m_layers;
    ars3d_int               m_exit;
    ars3d_int               m_status;

} Ars3DApp;




Ars3DApp *ars3dAppCreate(
    Ars3DUserEntryPointFN p_userEntry,
    const ars3d_char *  p_window_title,
    ars3d_int           p_window_width,
    ars3d_int           p_window_height
)
{
    // Singletone pattern.
    if (STATIC_APP_INSTANCE != ARS3D_NULL)
    {
        ARS3D_WARN("ars3dCreateApp called while an instance already exists!");
        return STATIC_APP_INSTANCE;
    }

    // Create the app.
    Ars3DApp *new_app = (Ars3DApp *)ars3dMalloc(ARS3D_SIZEOF(Ars3DApp));

    // Not enough memory.
    if (!new_app)
    {
        ARS3D_WARN("Not enough memory");
        return ARS3D_NULL;
    }

    // Create the Window.
    Ars3DWindow *new_window = ars3dWindowCreate(p_window_title, p_window_width, p_window_height);

    // Not enough memory.
    if (!new_window)
    {
        ARS3D_WARN("Not enough memory");
        ars3dFree(new_app);
        return ARS3D_NULL;
    }

    // Create the layers list.
    Ars3DList *new_layers_list = ars3dListCreate();

    // Not enough memory.
    if (!new_layers_list)
    {
        ARS3D_WARN("Not enough memory");
        ars3dWindowDestroy(&new_window);
        ars3dFree(new_app);
        return ARS3D_NULL;
    }

    // Initialize the app.
    new_app->m_userEntry    = p_userEntry;
    new_app->m_window       = new_window;
    new_app->m_layers       = new_layers_list;
    new_app->m_exit         = 0;
    new_app->m_status       = APPLICATION_STATUS_RUNNING;

    // Intialize the global static instance.
    STATIC_APP_INSTANCE = new_app;

    return new_app;
}




static ars3d_void layersDestructorCB(ars3d_void *p_data)
{
    Ars3DLayer *layer = (Ars3DLayer *)p_data;

    __ars3dLayerCallCB__(layer, ARS3D_LAYER_CB_ON_DETATCH);

    __ars3dLayerDestroy__(&layer);
}


ars3d_void __ars3dAppDestroy__(Ars3DApp *p_app)
{
    if (!p_app)
    {
        ARS3D_WARN("p_app provided with a NULL value");
        return;
    }

    // Destroy the layers list.
    ARS3D_INFO("Destroying the Layers...");
    ars3dListDestroy(&p_app->m_layers, layersDestructorCB);

    // Destroy the window.
    ars3dWindowDestroy(&p_app->m_window);

    ARS3D_INFO("Destroying the Application...");
    ars3dFree(p_app);

    STATIC_APP_INSTANCE = ARS3D_NULL;
}




ars3d_void __ars3dInitSubSystems__(Ars3DApp *p_app)
{
    if (!p_app)
    {
        ARS3D_WARN("You are trying to init the subsystems without providing an app instance!");
        return;
    }

    ARS3D_INFO("Initializing Subsystems...");
    ars3dInitDebugConsole();
}



static ars3d_uchar loopThroughLayersCB(ars3d_void *p_data, ars3d_void *p_context)
{
    ARS3D_UNUSED(p_context);
    Ars3DLayer *layer = (Ars3DLayer *)p_data;

    __ars3dLayerCallCB__(layer, ARS3D_LAYER_CB_ON_START);
    __ars3dLayerCallCB__(layer, ARS3D_LAYER_CB_ON_UPDATE);

    return 0; //Continue looping until the end of the list.
}




static ars3d_int __ars3dMainLoop__(Ars3DApp *p_app)
{
    while ( !p_app->m_exit )
    {

        // Render happens inside here!!!
        if (p_app->m_status == APPLICATION_STATUS_RUNNING)
        {
            glClear(GL_COLOR_BUFFER_BIT);

            // Loop through each layer.
            ars3dListLoopThrough(
                p_app->m_layers,
                loopThroughLayersCB,
                ARS3D_NULL,
                0
            );

            ars3dWindowSwapBuffers(p_app->m_window);
        }

        // Poll the events.
        ars3dWindowPollEvents();
    }

    return 0;
}


ars3d_int __ars3dAppBootUp__(Ars3DApp *p_app, ars3d_int argc, ars3d_char **argv)
{
    if (!p_app)
    {
        ARS3D_WARN("You are trying to boot up the engine without providing an app instance!");
        return -1;
    }

    ARS3D_INFO("Booting up Aristotle3D...");

    // Save the arguments.
    p_app->m_argc = argc;
    p_app->m_argv = argv;

    // Initialize the SubSystems of the engine.
    __ars3dInitSubSystems__(p_app);

    // Call the user's entry point.
    p_app->m_userEntry(p_app);

    // Start the main Loop.
    ars3d_int exit_value = __ars3dMainLoop__(p_app);

    // Destroy the APP.
    __ars3dAppDestroy__(p_app);

    return exit_value;
}



Ars3DApp *ars3dAppGet(ars3d_void)
{
    return STATIC_APP_INSTANCE;
}



ars3d_void ars3dAppAttachLayer(
    Ars3DApp *              p_app,
    ars3d_void *            p_user_ctx,
    Ars3DLayerOnAttachFN    p_onAttach,
    Ars3DLayerOnDetachFN    p_onDetach,
    Ars3DLayerOnStartFN     p_onStart,
    Ars3DLayerOnUpdateFN    p_onUpdate,
    Ars3DLayerOnEventFN     p_onEvent
)
{
    if (!p_app || !p_user_ctx)
    {
        ARS3D_WARN("Required params are not provided");
        return;
    }

    Ars3DLayer *new_layer = __ars3dLayerCreate__(
        p_user_ctx,
        p_onAttach,
        p_onDetach,
        p_onStart,
        p_onUpdate,
        p_onEvent
    );

    if (!new_layer) return;

    ars3dListAppend(p_app->m_layers, (ars3d_void *)new_layer);

    __ars3dLayerCallCB__(new_layer, ARS3D_LAYER_CB_ON_ATTACH);
}



static ars3d_uchar layerCompareEqualCB(ars3d_void *p_data, ars3d_void *p_cmp_data)
{
    Ars3DLayer *layer = (Ars3DLayer *)p_data;

    return __ars3dLayerGetUserCtx__(layer) == p_cmp_data;
}


ars3d_void ars3dAppDetatchLayer(Ars3DApp *p_app, ars3d_void *p_user_ctx)
{
    if (!p_app || !p_user_ctx)
    {
        ARS3D_WARN("Required params are not provided");
        return;
    }

    Ars3DLayer *layer = (Ars3DLayer *)ars3dListRemove(p_app->m_layers, layerCompareEqualCB, p_user_ctx);

    if (!layer)
    {
        ARS3D_WARN("[ars3dAppDetatchLayer()] Layer not found");
        return;
    }

    __ars3dLayerCallCB__(layer, ARS3D_LAYER_CB_ON_DETATCH);

    __ars3dLayerDestroy__(&layer);
}


Ars3DWindow *ars3dAppGetWindow(Ars3DApp *p_app)
{
    if (!p_app)
    {
        ARS3D_WARN("Required params are not provided");
        return ARS3D_NULL;
    }

    return p_app->m_window;
}


ars3d_int ars3dAppGetProcessArgc(Ars3DApp *p_app)
{
    if (!p_app)
    {
        ARS3D_WARN("Required params are not provided");
        return -1;
    }

    return p_app->m_argc;
}


ars3d_char **ars3dAppGetProcessArgv(Ars3DApp *p_app)
{
    if (!p_app)
    {
        ARS3D_WARN("Required params are not provided");
        return ARS3D_NULL;
    }

    return p_app->m_argv;
}






static ars3d_void applicationEventHanlder(Ars3DApp *p_app, ars3d_void *p_event, ars3d_int p_event_type)
{
    switch (p_event_type)
    {
        case ARS3D_EVENT_TYPE_WINDOW_CLOSED:
            p_app->m_exit = 1;
            break;

        case ARS3D_EVENT_TYPE_WINDOW_MINIMIZED:
            p_app->m_status = APPLICATION_STATUS_PAUSED;
            break;

        case ARS3D_EVENT_TYPE_WINDOW_RESTORED:
            p_app->m_status = APPLICATION_STATUS_RUNNING;
            break;

        case ARS3D_EVENT_TYPE_WINDOW_RESIZED:
        {
            ars3d_int width, height;
            ars3dEventGetWindowSize(p_event, &width, &height);
            ars3dWindowSetViewport(0, 0, width, height);
            break;
        }

        default:
            break;
    }
}



typedef struct EventContext
{
    ars3d_void *m_event;
    ars3d_int   m_event_type;
} EventContext;



static ars3d_uchar eventsReversedLoopCB(ars3d_void *p_data, ars3d_void *p_context)
{
    Ars3DLayer *layer = (Ars3DLayer *)p_data;
    EventContext *ctx = (EventContext *)p_context;

    ars3d_uchar handled = __ars3dLayerCallEventCB__(layer, ctx->m_event, ctx->m_event_type);

    if (handled) return 1; // Stop the loop.

    return 0; //Continue looping.
}


ars3d_void __ars3dAppFireEvent__(Ars3DApp *p_app, ars3d_void *p_event, ars3d_int p_event_type)
{
    if (!p_app)
    {
        ARS3D_WARN("Required params are not provided");
        return;
    }

    EventContext ctx;
    ctx.m_event         = p_event;
    ctx.m_event_type    = p_event_type;

    // Send all events EXCEPT Window Closed to the application handler.
    switch (p_event_type)
    {
        case ARS3D_EVENT_TYPE_WINDOW_CLOSED:
            break;

        default:
            applicationEventHanlder(p_app, p_event, p_event_type);
            break;
    }

    // Loop though each layer in reverse.
    // Pass the event to the layer system.
    ars3dListLoopThrough(p_app->m_layers, eventsReversedLoopCB, (ars3d_void *)&ctx, 1);

    // Send the WINDOW CLOSED event to the application handler, AFTER layers have recieved it.
    if (p_event_type == ARS3D_EVENT_TYPE_WINDOW_CLOSED) applicationEventHanlder(p_app, p_event, p_event_type);
}
