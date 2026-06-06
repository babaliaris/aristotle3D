#version 330 core
uniform vec3 u_light_color;

out vec4 pipe_color;

void main()
{
  pipe_color = vec4(u_light_color, 1.0);
}

