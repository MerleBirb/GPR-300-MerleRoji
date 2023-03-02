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

uniform vec3 cameraPos;

// const int MAX_LIGHTS 8
#define MAX_LIGHTS 8
uniform Light _Lights[MAX_LIGHTS];

in vec3 Normal;
in vec2 UV;

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
    vec3 viewerDir = normalize(cameraPos - v_out.WorldPosition);
    vec3 reflectDir = reflect(-toLightDir, surfaceNormal);

    specularLight = specCoefficient * pow(max(dot(reflectDir, viewerDir), 0), shininess) * color;

    return specularLight;
}

void main()
{
    //vec3 normal = normalize(v_out.WorldNormal);
    //
    //vec3 color = vec3(0);
    //vec3 toLightDir = normalize(_Lights[0].position - v_out.WorldPosition);
    //vec3 ambientLight = ambient(0.2f, _Lights[0].color);
    //vec3 diffuseLight = diffuse(0.8f, toLightDir, normal, _Lights[0].color);
    //vec3 specularLight = specular(0.5f, toLightDir, normal, 64f, _Lights[0].color);
    //
    //color = ambientLight + diffuseLight + specularLight;

    FragColor = vec4(UV.x, UV.y, 0.0f,1.0f);
}
