#version 330 core

layout (location = 0) in vec3 in_pos;
layout (location = 2) in vec4 in_color;

uniform mat4 u_model_mtx;
uniform mat4 u_view_projection_mtx;

out vec4 color;

void main() {
    gl_Position = u_view_projection_mtx * u_model_mtx * vec4(in_pos, 1.0);
    color = in_color;
}

