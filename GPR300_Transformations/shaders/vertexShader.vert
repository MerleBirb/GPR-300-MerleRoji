#version 450                          
layout (location = 0) in vec3 vPos;  
layout (location = 1) in vec3 vNormal;

out vec3 Normal;
uniform mat4 _MVP;

void main()
{ 
    Normal = vNormal;
    gl_Position = _MVP * vec4(vPos,1);
}
