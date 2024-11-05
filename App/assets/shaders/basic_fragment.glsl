#version 330 core
out vec4 FragColor;

uniform vec3 u_color = vec3(8.0, 8.0, 8.0);

void main()
{
    FragColor = vec4(u_color, 1.0);
}
