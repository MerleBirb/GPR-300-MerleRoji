#version 450                          
out vec4 FragColor;

in vec3 WorldNormal;
in vec3 WorldPosition;

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

// const int MAX_LIGHTS 8
#define MAX_LIGHTS 8
uniform Light _Lights[MAX_LIGHTS];

void main()
{
    vec3 normal = normalize(v_out.WorldNormal);
    FragColor = vec4(abs(normal),1.0f);
}
