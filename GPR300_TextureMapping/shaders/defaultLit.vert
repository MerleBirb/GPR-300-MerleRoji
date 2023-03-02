#version 450                          
layout (location = 0) in vec3 vPos;  
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vUV;

uniform mat4 _Model;
uniform mat4 _View;
uniform mat4 _Projection;

out struct Vertex
{
    vec3 WorldNormal;
    vec3 WorldPosition;
}v_out;

out vec3 Normal;
out vec2 UV;

void main()
{  
    //v_out.WorldPosition = vec3(_Model * vec4(vPos, 1));
    //v_out.WorldNormal = transpose(inverse(mat3(_Model))) * vNormal;

    Normal = vNormal;
    UV = vUV;

    gl_Position = _Projection * _View * _Model * vec4(vPos, 1);
}
