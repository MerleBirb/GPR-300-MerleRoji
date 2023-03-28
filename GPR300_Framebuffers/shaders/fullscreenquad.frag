#version 450

in vec2 uv;
out vec4 FragColor;

uniform sampler2D _FullscreenQuad;

void main()
{
	/// SETTINGS
	float doublePi = 6.28318530718; // pi * 2
	float dir = 16.0; // blur directions, more is better but slower
	float quality = 3.0; // blur quality, more is better but slower
	float size = 8.0; // size for radius

	vec2 radius = size / uv.xy;
	
	vec4 color = texture(_FullscreenQuad, uv);

	// blur loop calc

	FragColor = color;
}
