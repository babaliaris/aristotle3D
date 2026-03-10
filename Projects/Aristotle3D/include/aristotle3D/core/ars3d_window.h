#ifndef ARS3D_WINDOW_H
#define ARS3D_WINDOW_H
#include <aristotle3D/core/ars3d_stdio.h>

typedef struct Ars3DWindow Ars3DWindow;

ARS3D_API Ars3DWindow *ars3dCreateWindow(const ars3d_char *p_title, ars3d_int p_width, ars3d_int p_height);

ARS3D_API ars3d_void ars3dDestroyWindow(Ars3DWindow **p_window);


ARS3D_API ars3d_void ars3dUpdateWindow(Ars3DWindow *p_window);


ARS3D_API ars3d_int ars3dShouldCloseWindow(Ars3DWindow *p_window);

#endif