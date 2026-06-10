#version 330 core

in vec3 pipe_normal;
in vec3 pipe_frag_pos;

struct Material
{
  vec3  m_ambient;
  vec3  m_diffuse;
  vec3  m_specular;
  float m_shininess;
};

struct LightAttenuation
{
  float m_constant;
  float m_linear;
  float m_quadratic;
};

struct Light
{
  float             m_strength;
  vec3              m_position;
  vec3              m_ambient;
  vec3              m_diffuse;
  vec3              m_specular;
  LightAttenuation  m_attenuation;
};

struct SpotLight
{
  float m_inner_cutoff_angle;
  float m_outer_cutoff_angle;
  vec3  m_direction;
  Light m_light;
};

uniform Material    u_material;    // Different draw calls for each material.
uniform Light       u_ambient;     // General ambient lighting settings.
uniform Light       u_point_light; // Sun or Moon.
//uniform SpotLight   u_spot_light;  // Camera flashlight.
uniform vec3        u_cam_pos;     // Camera (eye) position.

out vec4 pipe_color;

vec3 calculateAmbientLighting(vec3 p_light_ambient, vec3 p_material_ambient, float p_light_strength);
vec3 calculateDiffusePointLighting(Light p_light, Material p_mat);
vec3 calculateSpecularPointLighting(Light p_light, Material p_mat);

void main()
{
  vec3 ambient        = calculateAmbientLighting(u_ambient.m_ambient, u_material.m_ambient, u_ambient.m_strength);
  vec3 point_diffuse  = calculateDiffusePointLighting(u_point_light, u_material);
  vec3 point_specular = calculateSpecularPointLighting(u_point_light, u_material);

  vec3 final_color = ambient + point_diffuse + point_specular;

  pipe_color = vec4(final_color, 1.0);
}


/**
 * @brief Calculates the light attenuation factor.
 *
 * @param p_constant The constant coefficient of the quadratic formula.
 * @param p_linear The linear coefficient of the quadratic formula.
 * @param p_quadratic The quadratic coefficient of the quadratic formula.
 * @param p_distance The variable distance of the quadratic formula.
 *
 * @returns The light attenuation factor that belongs [0,1], as long as all coefficients > 0.
 */
float calculateLightAttenuation(float p_constant, float p_linear, float p_quadratic, float p_distance)
{
  return 1.0 / (p_constant + p_linear * p_distance + p_quadratic * p_distance * p_distance);
}


/**
 * @brief Calculates the absorption factor of a lightt direction.
 *
 * It uses the dot product between the light direction and the
 * compare direction (normal or view direction or something else).
 * CAUTION: MAKE SURE BOTH DIRECTIONS POINT OUTWARDS FROM THE FRAGMENT.
 *
 * @param p_light_direction The light direction, pointing away from the fragment.
 * @param p_compare_direction The compare direction, pointing away from the fragment.
 *
 * @returns The absorption factor that belongs [0,1]
 */
float calculateLightAbsorptionFactor(vec3 p_light_direction, vec3 p_compare_direction)
{
  // Dont trust the user, and make sure they are normalized.
  vec3 light_dir  = normalize(p_light_direction);
  vec3 cmp_dir    = normalize(p_compare_direction);

  // Calculate the dot product.
  float dot_product = dot(light_dir, cmp_dir);

  return max(dot_product, 0.0);
}


/**
 * @brief Calculates the constant ambient lighting.
 *
 * This is just a constant color contribution that stays
 * the same no matter what.
 *
 * @param p_light_ambient The ambient lighting source.
 * @param p_material_ambient The material ambient color.
 * @param p_light_strength The strength of the light.
 *
 * @returns The final color of the ambient lighting.
 */
vec3 calculateAmbientLighting(vec3 p_light_ambient, vec3 p_material_ambient, float p_light_strength)
{
  return p_light_ambient * p_material_ambient * p_light_strength;
}


/**
 * @brief Calculate the diffuse lighting of a point light.
 *
 * @param p_light The point light source.
 * @param p_mat The material object.
 *
 * @returns The final color of the diffuse point lighting.
 */
vec3 calculateDiffusePointLighting(Light p_light, Material p_mat)
{
  vec3 frag_light_v = p_light.m_position - pipe_frag_pos;
  float absorption  = calculateLightAbsorptionFactor(frag_light_v, pipe_normal);
  float attenuation = calculateLightAttenuation(
      p_light.m_attenuation.m_constant,
      p_light.m_attenuation.m_linear,
      p_light.m_attenuation.m_quadratic,
      length(frag_light_v)
  );

  return p_light.m_diffuse * p_mat.m_diffuse * p_light.m_strength * absorption * attenuation;
}


/**
 * @brief Calculate the specular lighting of a point light.
 *
 * @param p_light The point light source.
 * @param p_mat The material object.
 *
 * @returns The final color of the specular point lighting.
 */
vec3 calculateSpecularPointLighting(Light p_light, Material p_mat)
{
  vec3 normalized_normal  = normalize(pipe_normal);
  vec3 light_frag_v       = pipe_frag_pos - p_light.m_position;
  vec3 reflect_v          = reflect(light_frag_v, normalized_normal);
  vec3 frag_to_cam_v      = u_cam_pos - pipe_frag_pos;

  float absorption  = calculateLightAbsorptionFactor(reflect_v, frag_to_cam_v);
  float specular    = pow(absorption, p_mat.m_shininess);

  float attenuation = calculateLightAttenuation(
      p_light.m_attenuation.m_constant,
      p_light.m_attenuation.m_linear,
      p_light.m_attenuation.m_quadratic,
      length(light_frag_v)
  );

  return p_light.m_specular * p_mat.m_specular * p_light.m_strength * specular * attenuation;
}




