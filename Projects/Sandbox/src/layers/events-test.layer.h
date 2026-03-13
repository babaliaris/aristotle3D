#ifndef EVENTS_TEST_LAYER_H
#define EVENTS_TEST_LAYER_H
#include <aristotle3D/aristotle3D.h>

typedef struct EventsTestLayer
{
    ars3d_int m_counter;

} EventsTestLayer;

ars3d_void onEventsTestLayerAttach(ars3d_void *p_ctx);

ars3d_void onEventsTestLayerDetatch(ars3d_void *p_ctx);

ars3d_void onEventsTestLayerStart(ars3d_void *p_ctx);

ars3d_void onEventsTestLayerUpdate(ars3d_void *p_ctx, ars3d_float p_delta_time);

ars3d_uchar onEventsTestLayerEvent(ars3d_void *p_ctx, ars3d_void *p_event, ars3d_int p_event_type);

#endif