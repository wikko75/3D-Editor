#version 330 core

uniform vec3 u_color = vec3(1.0, 1.0, 1.0);

out vec4 out_color;

void main() {
    out_color = vec4(u_color, 1.0);
}