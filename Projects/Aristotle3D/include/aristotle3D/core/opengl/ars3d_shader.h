#ifndef ARS3D_SHADER_H
#define ARS3D_SHADER_H
#include <aristotle3D/core/ars3d_stdio.h>
#include <cglm/struct.h>

typedef struct Ars3DShader Ars3DShader;

ARS3D_API Ars3DShader *ars3dShaderCreateFromStrings(
    const char *p_name,
    const char *p_vertex_src,
    const char *p_geometry_src,
    const char *p_fragment_src
);


ARS3D_API ars3d_void ars3dShaderDestroy(Ars3DShader **p_shader);

ARS3D_API ars3d_void ars3dShaderBind(Ars3DShader *p_shader);

ARS3D_API ars3d_void ars3dShaderUnbind();

ARS3D_API ars3d_void ars3dShaderUniformMat4(Ars3DShader *p_shader, const ars3d_char *p_name, mat4s *p_mat4);

ARS3D_API ars3d_void ars3dShaderUniformFloat(Ars3DShader *p_shader, const ars3d_char *p_name, float p_value);

ARS3D_API ars3d_void ars3dShaderUniformFloat3(Ars3DShader *p_shader, const ars3d_char *p_name, float p_v1, float p_v2, float p_v3);

#endif
