#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormals;
layout(location = 2) in vec4 aColor;
layout(location = 3) in int aIsSelected;

uniform mat4 u_model_mtx;
uniform mat4 u_view_projection_mtx;

flat out int is_selected;

out vec4 color;

void main()
{
    // vertex mode
    gl_PointSize = 15.0;
    is_selected = aIsSelected;
    color = aColor;
    gl_Position = u_view_projection_mtx * u_model_mtx * vec4(aPos, 1.0);

}
