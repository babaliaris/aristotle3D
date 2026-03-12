#include <aristotle3D/core/opengl/ars3d_render_operations.h>
#include <aristotle3D/core/opengl/ars3d_opengl_debugger.h>
#include <glad/gl.h>

ars3d_void ars3dRenderTriangles(ars3d_int p_vertex_count)
{
    ARS3D_OPENGL(glDrawArrays(GL_TRIANGLES, 0, p_vertex_count));
}
