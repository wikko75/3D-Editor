#version 330 core

flat in int is_selected;
in vec4 color;

in vec3 frag_pos;
in vec3 normal;
in vec3 light_src_pos;

out vec4 FragColor;

uniform vec3 u_color = vec3(8.0, 8.0, 8.0);
uniform int  u_primitive_type = 0; // 0 triangle  1 point

void main()
{
    // GL_TRIANGLES -> just color
    if (u_primitive_type == 0) 
    {

        float ambientStrength = .1;
        float specularStrength = 0.5;
        vec3 lightSrcColor = vec3(1.0, 1.0, 1.0);  //move to uniform

        vec4 ambient = vec4(ambientStrength * lightSrcColor, 1.0);

        vec3 normalVec = normalize(normal);
        vec3 lightDir = normalize(light_src_pos - frag_pos);
        vec4 diffuse = vec4( max( dot(lightDir, normalVec), 0.0  ) * lightSrcColor, 1.0);

        vec3 lightRef = reflect(-lightDir, normalVec);
        vec4 specular = pow( max( dot(normalize(-frag_pos), lightRef), 0.0 ), 200 ) * vec4(lightSrcColor, 1.0) * specularStrength;
        
        FragColor =  (ambient + diffuse + specular) * color;
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
