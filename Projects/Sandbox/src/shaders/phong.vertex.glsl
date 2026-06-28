#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;

uniform mat4 u_mvp;
uniform mat4 u_model;
uniform mat4 u_normal_mat;

out vec3 pipe_normal;
out vec3 pipe_frag_pos;
flat out vec3 pipe_flat_normal;

void main()
{
  gl_Position       = u_mvp * vec4(in_position, 1.0f);
  pipe_normal       = mat3(u_normal_mat) * in_normal;
  pipe_flat_normal  = pipe_normal;
  pipe_frag_pos     = vec3(u_model * vec4(in_position, 1.0f));
}

