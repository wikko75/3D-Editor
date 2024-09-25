#version 330 core

layout (location = 0) in vec3 pos_in;

uniform mat4 model_mtx;
uniform mat4 view_mtx;
uniform mat4 projection_mtx;

out vec3 frag_pos;

void main() {
    gl_Position =  projection_mtx * view_mtx * model_mtx * vec4(pos_in, 1.0);

    // light calculation in view space instead of world space
    frag_pos = vec3(view_mtx * model_mtx * vec4(pos_in, 1.0));
}