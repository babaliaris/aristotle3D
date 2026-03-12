#include <aristotle3D/core/opengl/ars3d_render_operations.h>
#include <glad/gl.h>

ars3d_void ars3dRenderTriangles(ars3d_int p_vertex_count)
{
    glDrawArrays(GL_TRIANGLES, 0, p_vertex_count);
}
