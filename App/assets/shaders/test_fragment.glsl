#version 330 core

in vec3 frag_pos;
out vec4 color_out;

void main() {
    color_out = vec4(frag_pos, 1.0);
}