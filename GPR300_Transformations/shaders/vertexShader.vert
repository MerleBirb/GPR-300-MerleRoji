#version 450                          
layout (location = 0) in vec3 vPos;  
layout (location = 1) in vec3 vNormal;

out vec3 Normal;

void main(){ 
    Normal = vNormal;
    gl_Position = vec4(vPos,1);
}
