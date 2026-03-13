#include <aristotle3D/core/ars3d_layer.h>
#include <aristotle3D/core/ars3d_application.h>
#include <aristotle3D/core/ars3d_debug.h>

typedef struct Ars3DLayer
{
    ars3d_uchar             m_started;
    ars3d_void *            m_user_ctx;
    Ars3DLayerOnAttachFN    m_onAttach;
    Ars3DLayerOnDetachFN    m_onDetach;
    Ars3DLayerOnStartFN     m_onStart;
    Ars3DLayerOnUpdateFN    m_onUpdate;
    Ars3DLayerOnEventFN     m_onEvent;
} Ars3DLayer;

Ars3DLayer *__ars3dLayerCreate__(
    ars3d_void *            p_user_ctx,
    Ars3DLayerOnAttachFN    p_onAttach,
    Ars3DLayerOnDetachFN    p_onDetach,
    Ars3DLayerOnStartFN     p_onStart,
    Ars3DLayerOnUpdateFN    p_onUpdate,
    Ars3DLayerOnEventFN     p_onEvent
)
{
    if (!p_user_ctx)
    {
        ARS3D_WARN("Required parameters are not provided");
        return ARS3D_NULL;
    }

    Ars3DLayer *new_layer = (Ars3DLayer *)ars3dMalloc( ARS3D_SIZEOF(Ars3DLayer) );

    if (!new_layer)
    {
        ARS3D_WARN("Not enough memory");
        return ARS3D_NULL;
    }

    new_layer->m_started    = 0;
    new_layer->m_user_ctx   = p_user_ctx;
    new_layer->m_onAttach   = p_onAttach;
    new_layer->m_onDetach   = p_onDetach;
    new_layer->m_onStart    = p_onStart;
    new_layer->m_onUpdate   = p_onUpdate;
    new_layer->m_onEvent    = p_onEvent;

    return new_layer;
}

ars3d_void __ars3dLayerDestroy__(Ars3DLayer **p_layer)
{
    if (!p_layer || !(*p_layer))
    {
        ARS3D_WARN("Placeholder is not provided or it points in a NULL address");
        return;
    }

    ars3dFree(*p_layer);

    *p_layer = ARS3D_NULL;
}


ars3d_void __ars3dLayerCallCB__(Ars3DLayer *p_layer, ars3d_int p_cb_option)
{
    if (!p_layer)
    {
        ARS3D_WARN("Required parameters are not provided");
        return;
    }


    switch (p_cb_option)
    {
        // On Attach
        case ARS3D_LAYER_CB_ON_ATTACH:
            if (p_layer->m_onAttach) p_layer->m_onAttach(p_layer->m_user_ctx);
            break;

        // On Detatch
        case ARS3D_LAYER_CB_ON_DETATCH:
            if (p_layer->m_onDetach) p_layer->m_onDetach(p_layer->m_user_ctx);
            break;

        // On Start
        case ARS3D_LAYER_CB_ON_START:
        {
            if ( !p_layer->m_started && p_layer->m_onStart )
            {
                p_layer->m_started = 1;
                p_layer->m_onStart(p_layer->m_user_ctx);
            }
            break;
        }

        default:
            break;
    }
}



ars3d_uchar __ars3dLayerCallEventCB__(Ars3DLayer *p_layer, ars3d_void *p_event, ars3d_int p_event_type)
{
    if (!p_layer)
    {
        ARS3D_WARN("Required parameters are not provided");
        return 0;
    }

    if (p_layer->m_onEvent)
        return p_layer->m_onEvent(p_layer->m_user_ctx, p_event, p_event_type);

    return 0;
}


ars3d_void __ars3dLayerCallUpdateCB__(Ars3DLayer *p_layer, ars3d_float p_delta_time)
{
    if (!p_layer)
    {
        ARS3D_WARN("Required parameters are not provided");
        return;
    }

    if (p_layer->m_onUpdate) p_layer->m_onUpdate(p_layer->m_user_ctx, p_delta_time);
}


ars3d_void *__ars3dLayerGetUserCtx__(Ars3DLayer *p_layer)
{
    if (!p_layer)
    {
        ARS3D_WARN("Required parameters are not provided");
        return ARS3D_NULL;
    }

    return p_layer->m_user_ctx;
}
