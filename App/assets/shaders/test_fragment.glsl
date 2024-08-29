#version 330 core

in vec3 pos;
out vec4 color_out;

void main() {
    color_out = vec4(pos, 1.0);
}