#version 330 core

layout (location = 0) in vec3 pos_in;
out vec3 pos;

void main() {
    gl_Position = vec4(pos_in, 1.0);
    pos = pos_in;
}