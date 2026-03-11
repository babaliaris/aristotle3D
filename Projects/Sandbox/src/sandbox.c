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
    ExampleLayer *ctx = (ExampleLayer *)p_ctx;
    ARS3D_INFO("onExampleStart ctx.m_counter = %d", ctx->m_counter);
    ctx->m_counter++;
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
        onExampleUpdate
    );
}



Ars3DApp * ars3dUserAppProvider()
{
    return ars3dAppCreate(ars3dUserEntryPoint, "SANDBOX", 512, 512);
}