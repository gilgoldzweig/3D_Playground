#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    sampler2D texture_diffuse3;
    sampler2D texture_specular1;
    sampler2D texture_specular2;
    sampler2D texture_specular3;
    sampler2D texture_normal1;
    sampler2D texture_normal2;
    sampler2D texture_height1;
    sampler2D texture_height2;
    sampler2D emission;
    float shininess;
};

struct Light {
    vec3 position;

    vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};


in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;


uniform vec3 viewPos;
uniform Material material;
uniform Light light;


float band(float start, float end, float t) {
    return step(start, t)*step(t, end);
}

float rect(vec2 uv, float start) {
    return band(start, 1.0f-start, TexCoords.x)*band(start, 1.0f-start, TexCoords.y);
}
vec3 textureRGB(sampler2D texture);

vec3 calcDirLight(Light light, vec3 normal, vec3 viewDir);

void main() {


//    vec3 diffuseTexture =  texture(material.texture_diffuse1, TexCoords).rgb;
//    vec3 specularTexture = texture(material.texture_specular1, TexCoords).rgb;
//    //    vec3 emissionTexture =  texture(material.emission, TexCoords).rgb;
//
//    vec3 norm = normalize(Normal);
//
//    vec3 ambient = light.color * light.ambient * diffuseTexture;
//
//
//    vec3 lightDir = normalize(light.position - FragPos);
//
//    float diff = max(dot(norm, lightDir), 0.0);
//    vec3 diffuse =  light.diffuse * diff * diffuseTexture * light.color;
//
//
//    vec3 viewDir = normalize(viewPos - FragPos);
//    vec3 reflectDir = reflect(-lightDir, norm);
//
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//
//    vec3 specular = light.specular * spec * specularTexture * light.color;

    //    float emissionMask = rect(TexCoords, specularTexture.r);
    //    vec3 emission = emissionTexture * emissionMask;


    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = calcDirLight(light, norm, viewDir);

    FragColor = vec4(result, 1.0);
}

vec3 calcDirLight(Light light, vec3 normal, vec3 viewDir) {

    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 lightDir = normalize(light.position);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // combine results
    vec3 diffuseTexture = textureRGB(material.texture_diffuse1);

    vec3 ambient = light.ambient * diffuseTexture;

    vec3 diffuse = light.diffuse * diff * diffuseTexture;

    vec3 specular = light.specular * spec * textureRGB(material.texture_specular1);

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;


    return (ambient + diffuse + specular) * light.color;
}

vec3 textureRGB(sampler2D textureId) {
    return texture(textureId, TexCoords).rgb;
}