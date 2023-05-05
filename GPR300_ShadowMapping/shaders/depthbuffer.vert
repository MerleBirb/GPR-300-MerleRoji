#version 450

layout (location = 0) in vec3 vPos;

uniform mat4 _MVP; // model view projection

void main()
{
	gl_Position = _MVP * vec4(vPos, 1);
}