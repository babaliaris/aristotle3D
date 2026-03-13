#define ARS3D_ENTRY_POINT
#include <aristotle3D/aristotle3D.h>

#include "layers/events-test.layer.h"
#include "layers/render-triangle-test.layer.h"
#include "layers/barnsley.h"

ars3d_void ars3dUserEntryPoint(Ars3DApp *p_app)
{
    int choice = -1;

    // 1. Display the CLI Menu
    printf("\n--- Aristotle3D Engine Sandbox ---\n");
    printf("1. Test Events Layer\n");
    printf("2. Render Triangle Test\n");
    printf("3. Barnsley Fern Fractal\n");
    printf("0. Exit\n");
    printf("Select a layer to run: ");
    
    // Get the user's input.
    if (scanf("%d", &choice) != 1) {
        ARS3D_ERROR("Invalid input. Please enter a number.");
        ars3dExit(-1);
    }

    // Attach the chosen layer
    switch (choice)
    {
        case 1:
        {
            ARS3D_INFO("Starting up: Events Test");
            EventsTestLayer *layer = (EventsTestLayer *)ars3dMalloc(sizeof(EventsTestLayer));
            layer->m_counter = 0;
            ars3dAppAttachLayer(
                p_app, layer, onEventsTestLayerAttach, onEventsTestLayerDetatch, 
                onEventsTestLayerStart, onEventsTestLayerUpdate, onEventsTestLayerEvent
            );
            break;
        }

        case 2:
        {
            ARS3D_INFO("Starting up: Triangle Render");
            RenderTriangleTestLayer *layer = (RenderTriangleTestLayer *)ars3dMalloc(sizeof(RenderTriangleTestLayer));
            ars3dAppAttachLayer(
                p_app, layer, onRenderTriangleTestLayerAttach, onRenderTriangleTestLayerDetatch, 
                onRenderTriangleTestLayerStart, onRenderTriangleTestLayerUpdate, onRenderTriangleTestLayerEvent
            );
            break;
        }

        case 3:
        {
            ARS3D_INFO("Starting up: Barnsley Fern");
            BarnsleyLayer *layer = (BarnsleyLayer *)ars3dMalloc(sizeof(BarnsleyLayer));
            ars3dAppAttachLayer(
                p_app, layer, onBarnsleyLayerAttach, onBarnsleyLayerDetatch, 
                onBarnsleyLayerStart, onBarnsleyLayerUpdate, onBarnsleyLayerEvent
            );
            break;
        }

        case 0:
            ARS3D_INFO("Exiting...");
            __ars3dAppDestroy__(p_app);
            ars3dExit(0);
            return;

        default:
            ARS3D_ERROR("Unknown choice. Exiting.");
            __ars3dAppDestroy__(p_app);
            ars3dExit(-1);
            return;
    }
}



Ars3DApp * ars3dUserAppProvider()
{
    return ars3dAppCreate(ars3dUserEntryPoint, "SANDBOX", 512, 512);
}
