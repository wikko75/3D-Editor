#version 330 core

in vec3 frag_pos;
out vec4 color_out;

uniform vec4 in_color = vec4(1.0, 1.0, 1.0, 1.0);

void main() {
    
    color_out = in_color;
}