#version 330 core

layout (location = 0) in vec3 in_pos = vec3(1.0, 0.0, 0.0);

uniform vec3 model_mtx;
uniform vec3 u_view_projection_mtx;


void main() {
    gl_Position = u_view_projection_mtx * model_mtx * vec4(in_pos, 1.0);
}

