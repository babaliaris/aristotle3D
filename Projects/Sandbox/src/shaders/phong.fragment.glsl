#version 330 core

in vec3 p_normal;

out vec4 p_color;

void main()
{
  vec3 visualized_normal = p_normal * 0.5f + 0.5f;
  p_color = vec4(visualized_normal, 1.0);
}
