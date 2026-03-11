#define ARS3D_ENTRY_POINT
#include <aristotle3D/aristotle3D.h>

typedef struct ExampleLayer
{
    ars3d_int m_counter;

} ExampleLayer;


ars3d_void onExampleAttach(ars3d_void *p_ctx)
{
    ARS3D_INFO("onExampleAttach");
    ARS3D_UNUSED(p_ctx);
}

ars3d_void onExampleDetatch(ars3d_void *p_ctx)
{
    ARS3D_INFO("onExampleDetatch");

    ExampleLayer *ex_layer = (ExampleLayer *)p_ctx;

    ars3dFree(ex_layer);
}


ars3d_void onExampleStart(ars3d_void *p_ctx)
{
    ARS3D_INFO("onExampleStart");
    ARS3D_UNUSED(p_ctx);
}

ars3d_void onExampleUpdate(ars3d_void *p_ctx)
{
    ARS3D_UNUSED(p_ctx);
}


ars3d_uchar onExampleEvent(ars3d_void *p_ctx, ars3d_void *p_event, ars3d_int p_event_type)
{
    ARS3D_UNUSED(p_ctx);

    switch (p_event_type)
    {
        case ARS3D_EVENT_TYPE_KEYBOARD_PRESS:
        {   
            ARS3D_INFO("Pressing key: %d", ars3dEventGetKeyboardKey(p_event));
            break;
        }

        case ARS3D_EVENT_TYPE_KEYBOARD_RELEASE:
        {   
            ARS3D_INFO("Releasing key: %d", ars3dEventGetKeyboardKey(p_event));
            break;
        }

        case ARS3D_EVENT_TYPE_KEYBOARD_REPEAT:
        {   
            ARS3D_INFO("Repeating key: %d", ars3dEventGetKeyboardKey(p_event));
            break;
        }

        case ARS3D_EVENT_TYPE_MOUSE_BUTTON_PRESS:
        {   
            ARS3D_INFO("Pressing Button: %d", ars3dEventGetMouseButton(p_event));
            break;
        }

        case ARS3D_EVENT_TYPE_MOUSE_BUTTON_RELEASE:
        {   
            ARS3D_INFO("Releasing Button: %d", ars3dEventGetMouseButton(p_event));
            break;
        }

        case ARS3D_EVENT_TYPE_MOUSE_POSITION:
        {
            ars3d_double x, y;
            ars3dEventGetMousePos(p_event, &x, &y);
            ARS3D_INFO("Mouse Pos = (%.2f, %.2f)",x, y);
            break;
        }

        case ARS3D_EVENT_TYPE_WINDOW_CLOSED:
        {   
            ARS3D_INFO("ARS3D_EVENT_TYPE_WINDOW_CLOSED");
            break;
        }

        case ARS3D_EVENT_TYPE_WINDOW_RESIZED:
        {
            ars3d_int width, height;
            ars3dEventGetWindowSize(p_event, &width, &height);
            ARS3D_INFO("Window Size = (%d, %d)",width, height);
            break;
        }

        case ARS3D_EVENT_TYPE_WINDOW_MAXIMIZED:
        {   
            ARS3D_INFO("ARS3D_EVENT_TYPE_WINDOW_MAXIMIZED");
            break;
        }

        case ARS3D_EVENT_TYPE_WINDOW_RESTORED:
        {   
            ARS3D_INFO("ARS3D_EVENT_TYPE_WINDOW_RESTORED");
            break;
        }

        case ARS3D_EVENT_TYPE_WINDOW_MINIMIZED:
        {   
            ARS3D_INFO("ARS3D_EVENT_TYPE_WINDOW_MINIMIZED");
            break;
        }

        case ARS3D_EVENT_TYPE_WINDOW_FOCUSED:
        {
            ARS3D_INFO("ARS3D_EVENT_TYPE_WINDOW_FOCUSED");
            break;
        }

        case ARS3D_EVENT_TYPE_WINDOW_UNFOCUSED:
        {
            ARS3D_INFO("ARS3D_EVENT_TYPE_WINDOW_UNFOCUSED");
            break;
        }

        default:
            break;
    }

    return 0;
}



ars3d_void ars3dUserEntryPoint(Ars3DApp *p_app)
{
    ExampleLayer *new_ex_layer  = (ExampleLayer *)ars3dMalloc( ARS3D_SIZEOF(ExampleLayer) );
    new_ex_layer->m_counter     = 0;

    ars3dAppAttachLayer(
        p_app,
        (ars3d_void *)new_ex_layer,
        onExampleAttach,
        onExampleDetatch,
        onExampleStart,
        onExampleUpdate,
        onExampleEvent
    );
}



Ars3DApp * ars3dUserAppProvider()
{
    return ars3dAppCreate(ars3dUserEntryPoint, "SANDBOX", 512, 512);
}