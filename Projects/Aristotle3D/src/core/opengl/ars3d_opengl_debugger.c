#include <aristotle3D/core/opengl/ars3d_opengl_debugger.h>
#include <aristotle3D/core/ars3d_debug.h>
#include <glad/gl.h>


const char *glErrorToString(GLenum p_error)
{
    switch (p_error)
    {
        case GL_INVALID_ENUM:
            return "GL_INVALID_ENUM";

        case GL_INVALID_VALUE:
            return "GL_INVALID_VALUE";

        case GL_INVALID_OPERATION:
            return "GL_INVALID_OPERATION";

        case GL_INVALID_FRAMEBUFFER_OPERATION:
            return "GL_INVALID_FRAMEBUFFER_OPERATION";

        case GL_OUT_OF_MEMORY:
            return "GL_OUT_OF_MEMORY";

        default:
            return "GL_NO_ERROR";
        }
}




ars3d_void __ars3dOpenGLDebuggerCleanAllErrors__()
{
    while ( glGetError() );
}



ars3d_void __ars3dOpenGLDebuggerGetErrors__(const ars3d_char *p_file, ars3d_int p_line)
{
    GLenum error;

    while (  ( error = glGetError() ) )
    {
        __ars3dColorfulDebugHelper__(
            "OpenGL",
            "ERROR",
            p_file,
            p_line,
            ARS3D_COLOR_R_RED,
            "%s",
            glErrorToString(error));
    }
}
