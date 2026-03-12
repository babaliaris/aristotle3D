#ifndef ARS3D_VERTEX_ARRAY_H
#define ARS3D_VERTEX_ARRAY_H
#include <aristotle3D/core/ars3d_stdio.h>

typedef struct Ars3DVertexArray Ars3DVertexArray;

ARS3D_API Ars3DVertexArray *ars3dVertexArrayCreate();

ARS3D_API ars3d_void ars3dVertexArrayDestroy(Ars3DVertexArray **p_vao);

ARS3D_API ars3d_void ars3dVertexArrayBind(Ars3DVertexArray *p_vao);

ARS3D_API ars3d_void ars3dVertexArrayUnbind();

#endif
