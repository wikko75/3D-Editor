#version 330 core
layout(location = 0) in vec3 aPos;

uniform mat4 u_model_mtx;
uniform mat4 u_view_projection_mtx;

void main()
{
    // vertex mode
    gl_PointSize = 15.0;

    gl_Position = u_view_projection_mtx * u_model_mtx * vec4(aPos, 1.0);
}
