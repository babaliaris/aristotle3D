#ifndef ARS3D_VERTEX_BUFFER_H
#define ARS3D_VERTEX_BUFFER_H
#include <aristotle3D/core/ars3d_stdio.h>

#define ARS3D_VBO_STREAM_DRAW   0
#define ARS3D_VBO_STREAM_READ   1
#define ARS3D_VBO_STREAM_COPY   2
#define ARS3D_VBO_STATIC_DRAW   3
#define ARS3D_VBO_STATIC_READ   4
#define ARS3D_VBO_STATIC_COPY   5
#define ARS3D_VBO_DYNAMIC_DRAW  6
#define ARS3D_VBO_DYNAMIC_READ  7
#define ARS3D_VBO_DYNAMIC_COPY  8


typedef struct Ars3DVertexBuffer Ars3DVertexBuffer;

ARS3D_API Ars3DVertexBuffer *ars3dVertexBufferCreate();

ARS3D_API ars3d_void ars3dVertexBufferDestroy(Ars3DVertexBuffer **p_vbo);

ARS3D_API ars3d_void ars3dVertexBufferBind(Ars3DVertexBuffer *p_vbo);

ARS3D_API ars3d_void ars3dVertexBufferUnbind();

ARS3D_API ars3d_void ars3dVertexBufferData(Ars3DVertexBuffer *p_vbo, ars3d_size p_size, ars3d_void *p_data, ars3d_int p_draw_hint);

#endif
