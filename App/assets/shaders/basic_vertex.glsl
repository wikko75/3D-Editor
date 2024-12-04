#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormals;
layout(location = 2) in vec4 aColor;
layout(location = 3) in int aIsSelected;

uniform mat4 u_model_mtx;
uniform mat4 u_view_mtx;
uniform mat4 u_projection_mtx;

uniform mat3 u_normal_mtx;
uniform vec3 u_light_pos;

flat out int is_selected;

out vec4 color;

out vec3 frag_pos;
out vec3 normal;
out vec3 light_src_pos;

void main()
{
    // vertex mode
    gl_PointSize = 15.0;
    is_selected = aIsSelected;
    color = aColor;

    gl_Position = u_projection_mtx * u_view_mtx * u_model_mtx * vec4(aPos, 1.0);

    // light stuff
    frag_pos = vec3(u_view_mtx * u_model_mtx * vec4(aPos, 1.0));
    light_src_pos = vec3(u_view_mtx * vec4(u_light_pos, 1.0));
    normal = u_normal_mtx * aNormals;

}
