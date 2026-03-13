#include <aristotle3D/core/opengl/ars3d_shader.h>
#include <aristotle3D/core/ars3d_debug.h>
#include <aristotle3D/core/opengl/ars3d_opengl_debugger.h>
#include <aristotle3D/core/ars3d_string.h>
#include <glad/gl.h>


#define CONVERT_ENUM_TO_STR(enum) #enum


ars3d_uchar compileShader(GLuint p_shader, const char *p_type_name, const GLchar *p_src, const char *p_shader_name);

Ars3DShader *createShaderProgram(
    const char *p_name,
    const char *p_vertex_src,
    const char *p_geometry_src,
    const char *p_fragment_src
);


typedef struct Ars3DShader
{
    ars3d_uint          m_program;
    Ars3DDynamicStr *   m_name;
} Ars3DShader;


ARS3D_API Ars3DShader *ars3dShaderCreateFromStrings(
    const char *p_name,
    const char *p_vertex_src,
    const char *p_geometry_src,
    const char *p_fragment_src
)
{
    if (!p_name)
    {
        ARS3D_WARN("Required parameters are NULL");
        return ARS3D_NULL;
    }

    return createShaderProgram(
        p_name,
        p_vertex_src,
        p_geometry_src,
        p_fragment_src
    );
}


ARS3D_API ars3d_void ars3dShaderDestroy(Ars3DShader **p_shader)
{
    if (!p_shader || !(*p_shader))
    {
        ARS3D_WARN("The placeholder or its pointing address is NULL");
        return;
    }

    ars3dDestroyDynamicStr(&(*p_shader)->m_name);

    ARS3D_OPENGL(glDeleteProgram((*p_shader)->m_program));

    ars3dFree(*p_shader);

    *p_shader = ARS3D_NULL;
}


ARS3D_API ars3d_void ars3dShaderBind(Ars3DShader *p_shader)
{
    if (!p_shader)
    {
        ARS3D_WARN("Required parameters are NULL");
        return;
    }

    ARS3D_OPENGL(glUseProgram(p_shader->m_program));
}


ARS3D_API ars3d_void ars3dShaderUnbind()
{
    ARS3D_OPENGL(glUseProgram(0));
}



ars3d_uchar compileShader(GLuint p_shader, const char *p_type_name, const GLchar *p_src, const char *p_shader_name)
{
    const GLchar *src = p_src;

    ARS3D_OPENGL(glShaderSource(p_shader, 1, &src, ARS3D_NULL));

    ARS3D_OPENGL(glCompileShader(p_shader));

    GLint compile_status;
    GLsizei info_length;

    ARS3D_OPENGL(glGetShaderiv(p_shader, GL_COMPILE_STATUS, &compile_status));
    ARS3D_OPENGL(glGetShaderiv(p_shader, GL_INFO_LOG_LENGTH, &info_length));

    if (!compile_status)
    {
        ARS3D_ASSERT(info_length > 0, "The info length should be greater than zero");

        ars3d_char *info = (ars3d_char *)ars3dMalloc( ARS3D_SIZEOF(ars3d_char) * info_length);

        ARS3D_OPENGL(glGetShaderInfoLog(p_shader, info_length, ARS3D_NULL, info));

        ARS3D_ERROR(
            "[Shader Compilation:%s] %s compilation failed because: %s",
            p_shader_name,
            p_type_name,
            info
        );

        ars3dFree(info);
        return 0;
    }

    ARS3D_INFO(
        "[Shader Compilation:%s] %s compiled successfully!",
        p_shader_name,
        p_type_name
    );

    ARS3D_UNUSED(p_type_name);
    ARS3D_UNUSED(p_shader_name);

    return 1;
}


Ars3DShader *createShaderProgram(
    const char *p_name,
    const char *p_vertex_src,
    const char *p_geometry_src,
    const char *p_fragment_src
)
{
    Ars3DShader *new_shader = (Ars3DShader *)ars3dMalloc( ARS3D_SIZEOF(Ars3DShader) );

    if (!new_shader)
    {
        ARS3D_WARN("Not enough memory");
        return ARS3D_NULL;
    }

    // Initialize the instance.
    new_shader->m_name      = ars3dCreateDynamicStr(p_name);
    ARS3D_OPENGL(new_shader->m_program   = glCreateProgram());

    // Create the shaders.
    ARS3D_OPENGL(GLuint vertex_shader    = p_vertex_src   ? glCreateShader(GL_VERTEX_SHADER)   : 0);
    ARS3D_OPENGL(GLuint geometry_shader  = p_geometry_src ? glCreateShader(GL_GEOMETRY_SHADER) : 0);
    ARS3D_OPENGL(GLuint fragment_shader  = p_fragment_src ? glCreateShader(GL_FRAGMENT_SHADER) : 0);

    // Compile the shaders and attach them to the program.
    ars3d_uchar compilation_success = 0;

    if (vertex_shader)
    {
        compilation_success = compileShader(vertex_shader, CONVERT_ENUM_TO_STR(GL_VERTEX_SHADER), p_vertex_src, p_name);
        ARS3D_OPENGL(glAttachShader(new_shader->m_program, vertex_shader));
    }

    if (geometry_shader)
    {
        compilation_success = compileShader(geometry_shader, CONVERT_ENUM_TO_STR(GL_GEOMETRY_SHADER), p_geometry_src, p_name);
        ARS3D_OPENGL(glAttachShader(new_shader->m_program, geometry_shader));
    }

    if (fragment_shader)
    {
        compilation_success = compileShader(fragment_shader, CONVERT_ENUM_TO_STR(GL_FRAGMENT_SHADER), p_fragment_src, p_name);
        ARS3D_OPENGL(glAttachShader(new_shader->m_program, fragment_shader));
    }

    // A shader FAILED to COMPILE.
    // Clean Up and return NULL.
    if (!compilation_success)
    {
        ars3dShaderDestroy(&new_shader);
        ARS3D_OPENGL(glDeleteShader(vertex_shader));
        ARS3D_OPENGL(glDeleteShader(geometry_shader));
        ARS3D_OPENGL(glDeleteShader(fragment_shader));
        return ARS3D_NULL;
    }

    //Link the program
    ARS3D_OPENGL(glLinkProgram(new_shader->m_program));

    // Delete the shaders.
    ARS3D_OPENGL(glDeleteShader(vertex_shader));
    ARS3D_OPENGL(glDeleteShader(geometry_shader));
    ARS3D_OPENGL(glDeleteShader(fragment_shader));

    // --------------------Check Linkage Status-------------------- //
    GLint   link_status;
    GLsizei info_length;

    ARS3D_OPENGL(glGetProgramiv(new_shader->m_program, GL_LINK_STATUS, &link_status));
    ARS3D_OPENGL(glGetProgramiv(new_shader->m_program, GL_INFO_LOG_LENGTH, &info_length));

    if (!link_status)
    {
        ARS3D_ASSERT(info_length > 0, "The info length should be greater than zero");

        ars3d_char *info = (ars3d_char *)ars3dMalloc( ARS3D_SIZEOF(ars3d_char) * info_length);

        ARS3D_OPENGL(glGetProgramInfoLog(new_shader->m_program, info_length, ARS3D_NULL, info));

        ARS3D_ERROR(
            "[Program Linkage:%s] Program linkage failed because: %s",
            p_name,
            info
        );

        // Clean UP and leave.
        ars3dFree(info);
        ars3dShaderDestroy(&new_shader);
        return ARS3D_NULL;
    }
    // --------------------Check Linkage Status-------------------- //

    ARS3D_INFO(
            "[Program Linkage:%s] Program linkage was successfull!",
            p_name
        );

    return new_shader;
}


ars3d_void ars3dShaderUniformMat4(Ars3DShader *p_shader, const ars3d_char *p_name, mat4s *p_mat4)
{
    if (!p_shader)
    {
        ARS3D_WARN("Required parameters are NULL");
        return;
    }

    ARS3D_OPENGL(GLint loc = glGetUniformLocation(p_shader->m_program, p_name));

    if (loc == -1)
    {
        ARS3D_WARN("[SHADER: %s] Uniform %s location was not found.", ars3dGetCStr(p_shader->m_name), p_name);
        return;
    }

    ARS3D_OPENGL(glUniformMatrix4fv(loc, 1, GL_FALSE, &p_mat4->raw[0][0]));
}



ars3d_void ars3dShaderUniformFloat(Ars3DShader *p_shader, const ars3d_char *p_name, float p_value)
{
    if (!p_shader)
    {
        ARS3D_WARN("Required parameters are NULL");
        return;
    }

    ARS3D_OPENGL(GLint loc = glGetUniformLocation(p_shader->m_program, p_name));

    if (loc == -1)
    {
        ARS3D_WARN("[SHADER: %s] Uniform %s location was not found.", ars3dGetCStr(p_shader->m_name), p_name);
        return;
    }

    ARS3D_OPENGL(glUniform1f(loc, p_value));
}


ars3d_void ars3dShaderUniformFloat3(Ars3DShader *p_shader, const ars3d_char *p_name, float p_v1, float p_v2, float p_v3)
{
    if (!p_shader)
    {
        ARS3D_WARN("Required parameters are NULL");
        return;
    }

    ARS3D_OPENGL(GLint loc = glGetUniformLocation(p_shader->m_program, p_name));

    if (loc == -1)
    {
        ARS3D_WARN("[SHADER: %s] Uniform %s location was not found.", ars3dGetCStr(p_shader->m_name), p_name);
        return;
    }

    ARS3D_OPENGL(glUniform3f(loc, p_v1, p_v2, p_v3));
}

