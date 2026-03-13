#ifndef ARS3D_RENDER_OPERATIONS_H
#define ARS3D_RENDER_OPERATIONS_H
#include <aristotle3D/core/ars3d_stdio.h>

ARS3D_API ars3d_void ars3dGLRenderTriangles(ars3d_int p_vertex_count);

ARS3D_API ars3d_void ars3dGLRenderPoints(ars3d_int p_vertex_count);

ARS3D_API ars3d_void ars3dGLEnableBlending(ars3d_uchar p_enable);

ARS3D_API ars3d_void ars3dGLEnableDepthTest(ars3d_uchar p_enable);

#endif
