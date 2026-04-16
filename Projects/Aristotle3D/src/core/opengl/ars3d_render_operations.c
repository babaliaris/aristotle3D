#include <aristotle3D/core/opengl/ars3d_render_operations.h>
#include <aristotle3D/core/opengl/ars3d_opengl_debugger.h>
#include <glad/gl.h>

ars3d_void ars3dGLRenderTriangles(ars3d_int p_vertex_count)
{
    ARS3D_OPENGL(glDrawArrays(GL_TRIANGLES, 0, p_vertex_count));
}


ars3d_void ars3dGLRenderPoints(ars3d_int p_vertex_count)
{
    ARS3D_OPENGL(glDrawArrays(GL_POINTS, 0, p_vertex_count));
}


ars3d_void ars3dGLEnableBlending(ars3d_uchar p_enable)
{
    if (p_enable)
    {
        ARS3D_OPENGL(glEnable(GL_BLEND));
    }

    else
    {
        ARS3D_OPENGL(glDisable(GL_BLEND));
    }
}


ars3d_void ars3dGLEnableDepthTest(ars3d_uchar p_enable)
{
    if (p_enable)
    {
        ARS3D_OPENGL(glEnable(GL_DEPTH_TEST));
    }

    else
    {
        ARS3D_OPENGL(glDisable(GL_DEPTH_TEST));
    }
}


ars3d_void ars3dGLEnableFaceCulling(ars3d_uchar p_enable)
{
  if (p_enable)
  {
    ARS3D_OPENGL(glEnable(GL_CULL_FACE));
  }

  else
  {
    ARS3D_OPENGL(glDisable(GL_CULL_FACE));
  }
}



ars3d_void ars3dGLPointSize(ars3d_float p_size)
{
    ARS3D_OPENGL(glPointSize(p_size));
}
