#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 fsNormal;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;
uniform vec3 lightPosition;
uniform vec3 viewPos;

void main()
{    
    vec3 norm = normalize(fsNormal);
    vec3 lightDir = normalize(lightPosition - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 lightColor = vec3(1.0f,1.0f,1.0f);
    vec3 diffuse = diff * lightColor;
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    float specularStrength = 0.8;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;
    vec3 objectColor = vec3(texture(texture_diffuse1, TexCoords).xyz);
    //vec3 result = (ambient + diffuse) * objectColor;
    vec3 result = (ambient + diffuse + specular) * objectColor;
    //vec3 result = vec3(0.0, 0.0, 1.0);
    FragColor =  vec4(result, 1.0);
}