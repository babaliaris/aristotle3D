#ifndef ARS3D_OPENGL_DEBUGGER_H
#define ARS3D_OPENGL_DEBUGGER_H
#include <aristotle3D/core/ars3d_stdio.h>

/**
 * @file
 * @brief Wrapper for opengl calls.
 */

ARS3D_API ars3d_void __ars3dOpenGLDebuggerCleanAllErrors__();
ARS3D_API ars3d_void __ars3dOpenGLDebuggerGetErrors__(const ars3d_char *p_file, ars3d_int p_line);


#if defined(ARS3D_DEBUG)

    /**
     * @brief Wraps opengl code and checks for errors.
     * 
     * This macro will check if the wrapped code has
     * generated any openGL errors, and log them.
     * 
     * @param code The opengl code to be wrapped.
     */
    #define ARS3D_OPENGL(code)\
        __ars3dOpenGLDebuggerCleanAllErrors__();\
        code;\
        __ars3dOpenGLDebuggerGetErrors__(__FILE__, __LINE__)

#else
    #define ARS3D_OPENGL(code) code

#endif

#endif
