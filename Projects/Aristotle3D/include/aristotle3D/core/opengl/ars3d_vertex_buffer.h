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


/**
 * @brief Create's a new vertex buffer.
 * 
 * @returns The newly created instance.
 */
ARS3D_API Ars3DVertexBuffer *ars3dVertexBufferCreate();


/**
 * @brief Destroy a vertex buffer.
 * 
 * @param p_vbo The user's placeholder that holds the instance memory address.
 */
ARS3D_API ars3d_void ars3dVertexBufferDestroy(Ars3DVertexBuffer **p_vbo);


/**
 * @brief Binds a vertex buffer.
 * 
 * @param p_vbo The buffer instance.
 */
ARS3D_API ars3d_void ars3dVertexBufferBind(Ars3DVertexBuffer *p_vbo);


/**
 * @brief Unbinds a vertex buffer.
 */
ARS3D_API ars3d_void ars3dVertexBufferUnbind();


/**
 * @brief Uploads data to the buffer / gpu.
 * 
 * If p_data is NULL, then the buffer will be
 * created nut NOT be initialized.
 * 
 * @param p_vbo The buffer instance.
 * @param p_size The size of the buffer.
 * @param p_data The starting mem address of the data buffer in the CPU.
 * @param p_draw_hint The draw hint for the GPU.
 */
ARS3D_API ars3d_void ars3dVertexBufferData(Ars3DVertexBuffer *p_vbo, ars3d_size p_size, ars3d_void *p_data, ars3d_int p_draw_hint);

#endif
