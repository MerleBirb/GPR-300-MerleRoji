#version 450
                          
layout (location = 0) in vec3 vPos;
layout (location = 2) in vec2 vuv;

out vec2 uv;

void main()
{
    gl_Position = vec4(vPos, 1);
    uv = vuv;
}