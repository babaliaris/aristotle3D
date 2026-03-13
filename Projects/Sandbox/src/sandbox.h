#ifndef SANDBOX_H
#define SANDBOX_H

#define SANDBOX_RUN_LAYER_TEST_EVENTS "test-events"
#define SANDBOX_RUN_LAYER_RENDER_TRIANGLE "render-triangle"
#define SANDBOX_RUN_LAYER_BARNSLEY "barnsley"


static ars3d_void showAvailableLayers()
{
    ars3dPrintf("This is a list of available layers. USAGE: ./sandbox <layer-name>\n");
    ars3dPrintf("%s\n", SANDBOX_RUN_LAYER_TEST_EVENTS);
    ars3dPrintf("%s\n", SANDBOX_RUN_LAYER_RENDER_TRIANGLE);
    ars3dPrintf("%s\n", SANDBOX_RUN_LAYER_BARNSLEY);
}

#endif
