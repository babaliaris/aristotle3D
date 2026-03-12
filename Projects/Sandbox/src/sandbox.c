#define ARS3D_ENTRY_POINT
#include <aristotle3D/aristotle3D.h>
#include "sandbox.h"

#include "layers/events-test.layer.h"
#include "layers/render-triangle-test.layer.h"

ars3d_void ars3dUserEntryPoint(Ars3DApp *p_app)
{
    ars3d_int argc              = ars3dAppGetProcessArgc(p_app);
    ars3d_char **argv           = ars3dAppGetProcessArgv(p_app);

    if ( argc == 2 )
    {
        ARS3D_INFO("Starting up LAYER: %s", argv[1]);

        // Show the number of layers.
        if ( ars3dStrCmp(argv[1], "list") == 0 )
        {
            __ars3dAppDestroy__(p_app);
            showAvailableLayers();
            ars3dExit(-1);
        }

        // LAYER: Test Events.
        else if ( ars3dStrCmp(argv[1], SANDBOX_RUN_LAYER_TEST_EVENTS) == 0 )
        {
            EventsTestLayer *new_events_test_layer  = (EventsTestLayer *)ars3dMalloc( ARS3D_SIZEOF(EventsTestLayer) );
            new_events_test_layer->m_counter        = 0;

            ars3dAppAttachLayer(
                p_app,
                (ars3d_void *)new_events_test_layer,
                onEventsTestLayerAttach,
                onEventsTestLayerDetatch,
                onEventsTestLayerStart,
                onEventsTestLayerUpdate,
                onEventsTestLayerEvent
            );
        }

        // LAYER: Render Triangle.
        else if ( ars3dStrCmp(argv[1], SANDBOX_RUN_LAYER_RENDER_TRIANGLE) == 0  )
        {
                RenderTriangleTestLayer *triangle_layer = (RenderTriangleTestLayer *)ars3dMalloc( ARS3D_SIZEOF(RenderTriangleTestLayer) );
                ars3dAppAttachLayer(
                    p_app,
                    (ars3d_void *)triangle_layer,
                    onRenderTriangleTestLayerAttach,
                    onRenderTriangleTestLayerDetatch,
                    onRenderTriangleTestLayerStart,
                    onRenderTriangleTestLayerUpdate,
                    onRenderTriangleTestLayerEvent
                );
        }

        else
        {
            ARS3D_ERROR("Uknown run argument.");
        }
    }

    else
    {
        ARS3D_ERROR("You did not provided a run argument.");
        __ars3dAppDestroy__(p_app);
        showAvailableLayers();
        ars3dExit(-1);
    }
}



Ars3DApp * ars3dUserAppProvider()
{
    return ars3dAppCreate(ars3dUserEntryPoint, "SANDBOX", 512, 512);
}
