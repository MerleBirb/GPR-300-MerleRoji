#version 450                          
out vec4 FragColor;

in struct Vertex
{
    vec3 WorldNormal;
    vec3 WorldPosition;
}v_out;

struct Light
{
    vec3 position;
    vec3 color;
    float intensity;
};

struct Material
{
    float ambientCoefficient;
    float diffuseCoefficient;
    float specularCoefficient;
    float shininess;
    vec3 objColor;
};

uniform vec3 _CameraPos;
uniform Material _Material;

// const int MAX_LIGHTS 8
#define MAX_LIGHTS 8
uniform Light _Lights[MAX_LIGHTS];

vec3 ambient(float coefficient, vec3 color)
{
    vec3 ambientLight;

    ambientLight = coefficient * color;

    return ambientLight;
}

vec3 diffuse(float difCoefficient, vec3 toLightDir, vec3 surfaceNormal, vec3 color)
{
    vec3 diffuseLight;

    diffuseLight = difCoefficient * max(dot(toLightDir, surfaceNormal), 0) * color; // max range 0 - 1

    return diffuseLight;
}

vec3 specular(float specCoefficient, vec3 toLightDir, vec3 surfaceNormal, float shininess, vec3 color)
{
    vec3 specularLight;
    vec3 viewerDir = normalize(_CameraPos - v_out.WorldPosition);
    //vec3 reflectDir = reflect(-toLightDir, surfaceNormal);

    vec3 halfVector = normalize(viewerDir + toLightDir); // blinn phong

    specularLight = specCoefficient * pow(max(dot(surfaceNormal, halfVector), 0), shininess) * color;

    return specularLight;
}

vec3 calculateLight(Light light)
{
    vec3 lightColor = vec3(0);

    vec3 normal = normalize(v_out.WorldNormal);
    vec3 toLightDir = normalize(light.position - v_out.WorldPosition);

    vec3 ambientLight = ambient(_Material.ambientCoefficient, light.color);
    vec3 diffuseLight = diffuse(_Material.diffuseCoefficient, toLightDir, normal, light.color);
    vec3 specularLight = specular(_Material.specularCoefficient, toLightDir, normal, _Material.shininess, light.color);

    lightColor = ambientLight + diffuseLight + specularLight;
    return lightColor;
}

void main()
{
    vec3 color = vec3(0);
    
    for(int i = 0; i < MAX_LIGHTS; i++)
    {
        color += calculateLight(_Lights[i]);
    }

    FragColor = vec4(_Material.objColor * color,1.0f);
}
