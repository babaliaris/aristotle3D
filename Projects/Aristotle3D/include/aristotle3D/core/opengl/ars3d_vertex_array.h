#ifndef ARS3D_VERTEX_ARRAY_H
#define ARS3D_VERTEX_ARRAY_H
#include <aristotle3D/core/ars3d_stdio.h>

typedef struct Ars3DVertexArray Ars3DVertexArray;


/**
 * @brief Create a new vertex array.
 * 
 * @returns The newly created instance.
 */
ARS3D_API Ars3DVertexArray *ars3dVertexArrayCreate();


/**
 * @brief Destroy a vertex array object.
 * 
 * @param p_vao The user's placeholder that holds the instance mem address.
 */
ARS3D_API ars3d_void ars3dVertexArrayDestroy(Ars3DVertexArray **p_vao);


/**
 * @brief Bind a vertex array object.
 * 
 * @param p_vao The vertex array instance.
 */
ARS3D_API ars3d_void ars3dVertexArrayBind(Ars3DVertexArray *p_vao);


/**
 * @brief Unbind a vertex array object.
 * 
 * @param p_vao The vertex array instance.
 */
ARS3D_API ars3d_void ars3dVertexArrayUnbind();

#endif
