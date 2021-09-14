#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;


struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};


struct DirLight {
    vec3 direction;

    vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    vec3 color;
    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define NR_POINT_LIGHTS 4

uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform DirLight dirLight;


uniform vec3 viewPos;
uniform Material material;


vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

vec3 textureRGB(sampler2D texture);
float band(float start, float end, float t);
float rect(vec2 uv, float start);


void main() {
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = calcDirLight(dirLight, norm, viewDir);

    for (int i = 0; i < NR_POINT_LIGHTS; i++) {
        result += calcPointLight(pointLights[i], norm, FragPos, viewDir);
    }

    FragColor = vec4(result, 1.0);
}

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // combine results
    vec3 diffuseTexture = textureRGB(material.diffuse);

    vec3 ambient = light.ambient * diffuseTexture;

    vec3 diffuse = light.diffuse * diff * diffuseTexture;

    vec3 specular = light.specular * spec * textureRGB(material.specular);

    return (ambient + diffuse + specular) * light.color;
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // combining results

    vec3 diffuseTexture = textureRGB(material.diffuse);

    vec3 ambient = light.ambient * diffuseTexture;

    vec3 diffuse = light.diffuse * diff * diffuseTexture;

    vec3 specular = light.specular * spec * textureRGB(material.specular);

    ambient *= attenuation;

    diffuse *= attenuation;

    specular *= attenuation;

    return (ambient + diffuse + specular) * light.color;
}

vec3 textureRGB(sampler2D textureId) {
    return texture(textureId, TexCoords).rgb;
}

float band(float start, float end, float t) {
    return step(start, t)*step(t, end);
}

float rect(vec2 uv, float start) {
    return band(start, 1.0f-start, TexCoords.x)*band(start, 1.0f-start, TexCoords.y);
}
