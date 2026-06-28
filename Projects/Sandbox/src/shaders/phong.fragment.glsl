#version 330 core

in vec3 pipe_normal;
in vec3 pipe_frag_pos;
flat in vec3 pipe_flat_normal;

struct Material
{
  vec3  m_ambient;
  vec3  m_diffuse;
  vec3  m_specular;
  float m_shininess;
};

struct AmbientLight
{
  float m_strength;
  vec3  m_ambient;
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
  vec3              m_diffuse;
  vec3              m_specular;
  LightAttenuation  m_attenuation;
};

struct SpotLight
{
  float m_inner_cutoff_cos;
  float m_outer_cutoff_cos;
  vec3  m_direction;
  Light m_light;
};

uniform Material      u_material;    // Different draw calls for each material.
uniform AmbientLight  u_ambient;     // Global ambient lighting settings.
uniform Light         u_point_light; // Sun or Moon.
uniform SpotLight     u_spot_light;  // Camera flashlight.
uniform vec3          u_cam_pos;     // Camera (eye) position.
uniform bool u_use_flat_shading;     // Use flat shading.

out vec4 pipe_color;

vec3 calculateAmbientLighting(AmbientLight p_light, Material p_mat);
vec3 calculateDiffusePointLighting(Light p_light, Material p_mat, vec3 p_normal);
vec3 calculateSpecularPointLighting(Light p_light, Material p_mat, vec3 p_normal);
vec3 calculateDiffuseSpotLighting(SpotLight p_spot, Material p_mat, vec3 p_normal);
vec3 calculateSpecularSpotLighting(SpotLight p_spot, Material p_mat, vec3 p_normal);

void main()
{
  // Choose the right normal for flat or smooth shading.
  vec3 normal         = u_use_flat_shading ? normalize(pipe_flat_normal) : normalize(pipe_normal);

  vec3 ambient        = calculateAmbientLighting(u_ambient, u_material);
  vec3 point_diffuse  = calculateDiffusePointLighting(u_point_light, u_material, normal);
  vec3 point_specular = calculateSpecularPointLighting(u_point_light, u_material, normal);
  vec3 spot_diffuse   = calculateDiffuseSpotLighting(u_spot_light, u_material, normal);
  vec3 spot_specular  = calculateSpecularSpotLighting(u_spot_light, u_material, normal);

  vec3 final_color = ambient + point_diffuse + point_specular + spot_diffuse + spot_specular;

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
 * @param p_light The ambient lighting source.
 * @param p_mat The material object.
 *
 * @returns The final color of the ambient lighting.
 */
vec3 calculateAmbientLighting(AmbientLight p_light, Material p_mat)
{
  return p_light.m_ambient * p_mat.m_ambient * p_light.m_strength;
}


/**
 * @brief Calculate the diffuse lighting of a point light.
 *
 * @param p_light The point light source.
 * @param p_mat The material object.
 * @param p_normal The normal vector of the pixel.
 *
 * @returns The final color of the diffuse point lighting.
 */
vec3 calculateDiffusePointLighting(Light p_light, Material p_mat, vec3 p_normal)
{
  vec3 frag_light_v = p_light.m_position - pipe_frag_pos;
  float absorption  = calculateLightAbsorptionFactor(frag_light_v, p_normal);
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
 * @param p_normal The normal vector of the pixel.
 *
 * @returns The final color of the specular point lighting.
 */
vec3 calculateSpecularPointLighting(Light p_light, Material p_mat, vec3 p_normal)
{
  vec3 light_frag_v       = pipe_frag_pos - p_light.m_position;
  vec3 reflect_v          = reflect(light_frag_v, p_normal);
  vec3 frag_to_cam_v      = u_cam_pos - pipe_frag_pos;

  float absorption = calculateLightAbsorptionFactor(reflect_v, frag_to_cam_v);
  float specular   = 0.0;

  if (absorption > 0.0 && p_mat.m_shininess > 0.0)
  {
    specular = pow(absorption, p_mat.m_shininess);
  }

  float attenuation = calculateLightAttenuation(
      p_light.m_attenuation.m_constant,
      p_light.m_attenuation.m_linear,
      p_light.m_attenuation.m_quadratic,
      length(light_frag_v)
  );

  return p_light.m_specular * p_mat.m_specular * p_light.m_strength * specular * attenuation;
}




/**
 * @brief Calculate the diffuse lighting of a spot light.
 *
 * @param p_light The spot light source.
 * @param p_mat The material object.
 * @param p_normal The normal vector of the pixel.
 *
 * @returns The final color of the diffuse spot lighting.
 */
vec3 calculateDiffuseSpotLighting(SpotLight p_spot, Material p_mat, vec3 p_normal)
{
  vec3 light_frag_v = pipe_frag_pos - p_spot.m_light.m_position;

  float absorption = calculateLightAbsorptionFactor(-light_frag_v, p_normal);

  float attenuation = calculateLightAttenuation(
      p_spot.m_light.m_attenuation.m_constant,
      p_spot.m_light.m_attenuation.m_linear,
      p_spot.m_light.m_attenuation.m_quadratic,
      length(light_frag_v)
  );

  float theta                 = dot(normalize(light_frag_v), normalize(p_spot.m_direction));
  float outer_inner_distance  = p_spot.m_inner_cutoff_cos - p_spot.m_outer_cutoff_cos;
  float spot_itensity         = clamp((theta - p_spot.m_outer_cutoff_cos) / outer_inner_distance, 0.0, 1.0);

  return p_spot.m_light.m_diffuse * p_mat.m_diffuse * p_spot.m_light.m_strength * absorption * spot_itensity * attenuation;
}


/**
 * @brief Calculate the specular lighting of a spot light.
 *
 * @param p_light The spot light source.
 * @param p_mat The material object.
 * @param p_normal The normal vector of the pixel.
 *
 * @returns The final color of the specular spot lighting.
 */
vec3 calculateSpecularSpotLighting(SpotLight p_spot, Material p_mat, vec3 p_normal)
{
  vec3 light_frag_v   = pipe_frag_pos - p_spot.m_light.m_position;
  vec3 reflect_v      = reflect(light_frag_v, p_normal);
  vec3 frag_to_cam_v  = u_cam_pos - pipe_frag_pos;
  float absorption    = calculateLightAbsorptionFactor(reflect_v, frag_to_cam_v);
  float specular      = 0.0;

  if (absorption > 0.0 && p_mat.m_shininess > 0.0)
  {
    specular = pow(absorption, p_mat.m_shininess);
  }

  float attenuation = calculateLightAttenuation(
      p_spot.m_light.m_attenuation.m_constant,
      p_spot.m_light.m_attenuation.m_linear,
      p_spot.m_light.m_attenuation.m_quadratic,
      length(light_frag_v)
  );

  float theta                 = dot(normalize(light_frag_v), normalize(p_spot.m_direction));
  float outer_inner_distance  = p_spot.m_inner_cutoff_cos - p_spot.m_outer_cutoff_cos;
  float spot_itensity         = clamp((theta - p_spot.m_outer_cutoff_cos) / outer_inner_distance, 0.0, 1.0);

  return p_spot.m_light.m_specular * p_mat.m_specular * p_spot.m_light.m_strength * specular * spot_itensity * attenuation;
}
