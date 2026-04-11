#ifndef ARS3D_UI_H
#define ARS3D_UI_H

#include "aristotle3D/core/ars3d_api.h"
#include <aristotle3D/core/ars3d_stdio.h>
#include <microui.h>

// Initializes the MicroUI context
ARS3D_API ars3d_void ars3dUIInit();

// Called at the start of every frame
ARS3D_API ars3d_void ars3dUITerminate();

// Begins drawing frames.
ARS3D_API ars3d_void ars3dUIBegin();

// Called at the end of every frame to prepare commands
ARS3D_API ars3d_void ars3dUIEnd();

// Returns true if the UI is using the mouse
ARS3D_API ars3d_int ars3dUIWantCaptureMouse();

// Accessor for the internal context (needed by the renderer)
ARS3D_API mu_Context* ars3dUIGetContext();

// Process the events.
ARS3D_API ars3d_void ars3dUIProcessEvent(ars3d_void* p_event, ars3d_int p_event_type);

#endif
