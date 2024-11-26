#version 330 core

flat in int is_selected;
in vec4 color;

out vec4 FragColor;

uniform vec3 u_color = vec3(8.0, 8.0, 8.0);
uniform int  u_primitive_type = 0; // 0 triangle  1 point

void main()
{
    // GL_TRIANGLES -> just color
    if (u_primitive_type == 0) 
    {
        FragColor = color;
        return;
    }

    // GL_POINTS
    if (u_primitive_type == 1)
    {
        if (is_selected == 0) 
        {
            FragColor = vec4(1.0, 1.0, 1.0, 1.0);  
        }
         else 
        {
            FragColor = vec4(0.0, 0.9, 0.0, 1.0);
        }
    }

}
