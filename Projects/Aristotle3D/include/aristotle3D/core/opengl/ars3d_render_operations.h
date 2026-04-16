#ifndef ARS3D_RENDER_OPERATIONS_H
#define ARS3D_RENDER_OPERATIONS_H
#include <aristotle3D/core/ars3d_stdio.h>


/**
 * @brief Triangles Draw Call.
 * 
 * This issues a draw calling using TRIANGLES for the
 * primitives.
 * 
 * @param p_vertex_count The amount of vertices to be drawn.
 */
ARS3D_API ars3d_void ars3dGLRenderTriangles(ars3d_int p_vertex_count);


/**
 * @brief Points Draw Call.
 * 
 * This issues a draw calling using POINTS for the
 * primitives.
 * 
 * @param p_vertex_count The amount of vertices to be drawn.
 */
ARS3D_API ars3d_void ars3dGLRenderPoints(ars3d_int p_vertex_count);


/**
 * @brief Enable blending.
 * 
 * @param p_enable 0 = disable, true = enable
 */
ARS3D_API ars3d_void ars3dGLEnableBlending(ars3d_uchar p_enable);


/**
 * @brief Enable depth testing.
 * 
 * @param p_enable 0 = disable, true = enable
 */
ARS3D_API ars3d_void ars3dGLEnableDepthTest(ars3d_uchar p_enable);


/**
 * @brief Enable face culling.
 * 
 * @param p_enable 0 = disable, true = enable
 */
ARS3D_API ars3d_void ars3dGLEnableFaceCulling(ars3d_uchar p_enable);



/**
 * @brief Set point size.
 * 
 * This tells the GPU how big to draw points.
 * 
 * @param p_size The new size of the points.
 */
ARS3D_API ars3d_void ars3dGLPointSize(ars3d_float p_size);

#endif
