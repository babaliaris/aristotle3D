#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;

uniform mat4 u_mvp;

out vec3 p_normal;

void main()
{
  gl_Position = u_mvp * vec4(in_position, 1.0f);
  p_normal    = in_normal;
}

