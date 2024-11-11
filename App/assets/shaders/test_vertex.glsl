#version 330 core

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_normal;

uniform mat4 u_model_mtx;
uniform mat4 u_view_mtx;
uniform mat4 u_view_projection_mtx;

out vec3 frag_pos;

void main() {
    gl_Position = u_view_projection_mtx * u_model_mtx * vec4(in_pos, 1.0);

    // light calculation in view space instead of world space
    frag_pos = vec3(u_view_mtx * u_model_mtx * vec4(in_pos, 1.0));
}