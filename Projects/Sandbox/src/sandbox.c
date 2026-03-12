#define ARS3D_ENTRY_POINT
#include <aristotle3D/aristotle3D.h>

#include "layers/events-test.layer.h"



ars3d_void ars3dUserEntryPoint(Ars3DApp *p_app)
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



Ars3DApp * ars3dUserAppProvider()
{
    return ars3dAppCreate(ars3dUserEntryPoint, "SANDBOX", 512, 512);
}