#version 330 core

struct Light {
vec4 ambient, diffuse, specular, direction;
};

struct Material {
vec4 ambient, diffuse, specular;
float shininess;
};

#define LIGHTS_COUNT 2

in vec3 normal, position;
in vec2 texCoord;

uniform vec3 camera;
uniform Light lights[LIGHTS_COUNT];
uniform Material material;
uniform sampler2D tex;

vec4 CalcLight(Light light, vec3 normal, vec3 viewDir);

uniform vec4 color;

void main()
{

    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(camera - position);

    vec4 result;

    for(int i = 0; i < LIGHTS_COUNT; i++) 
    {
        result += CalcLight(lights[i], norm, viewDir);
    }

    gl_FragColor = color + result;
}

vec4 CalcLight(Light light, vec3 normal, vec3 viewDir) 
{
    vec4 ambient = light.ambient * material.ambient * texture(tex, texCoord);

    vec3 lightDir = normalize(- light.direction.xyz);

    float Kd = max(dot(normal, lightDir), 0.0);
    vec4 diffuse = Kd * light.diffuse * material.diffuse * texture(tex, texCoord);

    vec3 reflectDir = reflect(-lightDir, normal);
    float Ks = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec4 specular = Ks * light.specular * material.specular * texture(tex, texCoord);

    return ambient + diffuse + specular;
}