#version 450                          
layout (location = 0) in vec3 vPos;  
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vUv;
layout (location = 3) in vec3 vTangent;

out vec2 uv;
out mat3 worldTBN;

uniform mat4 _Model;
uniform mat4 _View;
uniform mat4 _Projection;

uniform float _NmapIntensity;

out struct Vertex
{
    vec3 WorldNormal;
    vec3 WorldPosition;
}v_out;

void main()
{  
    v_out.WorldPosition = vec3(_Model * vec4(vPos, 1));
    v_out.WorldNormal = transpose(inverse(mat3(_Model))) * vNormal;
    gl_Position = _Projection * _View * _Model * vec4(vPos, 1);

    vec3 bitangent = normalize(cross(vNormal, vTangent));
    mat3 localTBN = mat3(vTangent * _NmapIntensity, bitangent * _NmapIntensity, vNormal);
    worldTBN = transpose(inverse(mat3(_Model))) * localTBN;

    uv = vUv;
}
