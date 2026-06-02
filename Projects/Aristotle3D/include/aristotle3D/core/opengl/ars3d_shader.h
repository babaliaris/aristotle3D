#ifndef ARS3D_SHADER_H
#define ARS3D_SHADER_H
#include <aristotle3D/core/ars3d_stdio.h>
#include <cglm/struct.h>

typedef struct Ars3DShader Ars3DShader;

/**
 * @brief Creates a new shader program by string sources.
 * 
 * Sources can bee NULL if you don't wish to use 
 * a specific shader.
 * 
 * @param p_name A debug name for the shader program.
 * @param p_vertex_src The vertex shader source.
 * @param p_geometry_src The geometry shader source.
 * @param p_fragment_src The fragment shader source.
 * 
 * @returns The newly created instance.
 */
ARS3D_API Ars3DShader *ars3dShaderCreateFromStrings(
    const char *p_name,
    const char *p_vertex_src,
    const char *p_geometry_src,
    const char *p_fragment_src
);


/**
 * @brief Creates a new shader program by reading files.
 *
 * Store your shader source code in a text file and then
 * feed this function with the filepaths required.
 *
 * @param p_vertex_path The path of the vertex source file.
 * @param p_fragment_path The path of the fragment source file.
 *
 * @returns The newly created instance.
 */
ARS3D_API Ars3DShader *ars3dShaderCreateFromFile(
  const char *p_vertex_path,
  const char *p_fragment_path
);


/**
 * @brief Destroys a shader program object.
 * 
 * @param p_shader The user's placeholder that holds the mem address of the instance.
 */
ARS3D_API ars3d_void ars3dShaderDestroy(Ars3DShader **p_shader);


/**
 * @brief Bind a shader object.
 * 
 * @param p_shader The shader instance.
 */
ARS3D_API ars3d_void ars3dShaderBind(Ars3DShader *p_shader);


/**
 * @brief Unbind a shader object.
 * 
 * @param p_shader The shader instance.
 */
ARS3D_API ars3d_void ars3dShaderUnbind();


/**
 * @brief Upload a 4x4 matrix uniform in the GPU.
 * 
 * @param p_shader The shader instance.
 * @param p_name The name of the uniform, as defined in the shader source.
 * @param p_mat4 The glm matrix object that contains the 16 float values of the 4x4 matrix.
 */
ARS3D_API ars3d_void ars3dShaderUniformMat4(Ars3DShader *p_shader, const ars3d_char *p_name, mat4s *p_mat4);


/**
 * @brief Upload a single float uniform in the GPU.
 * 
 * @param p_shader The shader instance.
 * @param p_name The name of the uniform, as defined in the shader source.
 * @param p_value The value to upload.
 */
ARS3D_API ars3d_void ars3dShaderUniformFloat(Ars3DShader *p_shader, const ars3d_char *p_name, float p_value);


/**
 * @brief Upload a vec3 uniform in the GPU.
 * 
 * @param p_shader The shader instance.
 * @param p_name The name of the uniform, as defined in the shader source.
 * @param p_v1 The x (first) value of the vector.
 * @param p_v2 The y (second) value of the vector.
 * @param p_v3 The z (third) value of the vector.
 */
ARS3D_API ars3d_void ars3dShaderUniformFloat3(Ars3DShader *p_shader, const ars3d_char *p_name, float p_v1, float p_v2, float p_v3);



/**
 * @brief Upload an integer uniform in the GPU.
 * 
 * @param p_shader The shader instance.
 * @param p_name The name of the uniform, as defined in the shader source.
 * @param p_value The integer value.
 */
ARS3D_API ars3d_void ars3dShaderUniformInt(Ars3DShader *p_shader, const ars3d_char *p_name, ars3d_int p_value);


#endif
