#shader vertex
#version 330 core
// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

void main() {

    FragPos = vec3(model * vec4(aPos, 1.0f));
    Normal = aNormal;
    gl_Position = projection * view * model * vec4(FragPos, 1.0);
    TexCoords = aTexCoords;
}

#shader fragment
#version 330 core

struct Material {
    sampler2D diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 color;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;


uniform vec3 viewPos;
uniform vec3 objectColor;
uniform Material material;
uniform Light light;


void main() {


    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.color * (light.diffuse * (diff * material.diffuse)) * vec3(texture(material.diffuse, TexCoords));

    vec3 ambient = light.color * light.ambient * vec3(texture(material.diffuse, TexCoords));

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular =  light.color * (light.specular * (spec * material.specular));


    vec3 result = (ambient + diffuse + specular) * objectColor;
    color = vec4(result, 1.0);
}