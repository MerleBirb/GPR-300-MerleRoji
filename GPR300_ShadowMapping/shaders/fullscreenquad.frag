#version 450

in vec2 uv;
out vec4 FragColor;

uniform sampler2D _FullscreenQuad;

uniform int _ScreenWidth;
uniform int _ScreenHeight;
uniform bool _IsBlurOn;
uniform bool _IsNegOn;

void main()
{
	/// SETTINGS
	float doublePi = 6.28318530718; // pi * 2
	float dir = 16.0; // blur directions, more is better but slower
	float quality = 3.0; // blur quality, more is better but slower
	float size = 8.0; // size for radius

	vec2 radius = size / vec2(_ScreenWidth, _ScreenHeight);
	
	vec4 color = texture(_FullscreenQuad, uv);

	if (_IsBlurOn)
	{
		// blur loop calc
		for (float d = 0.0; d < doublePi; d += doublePi/dir)
		{
			for (float i = 1.0/quality; i <= 1.0; i += 1.0/quality)
			{
				color += texture(_FullscreenQuad, uv + vec2(cos(d), sin(d)) * radius * i);
			}
		}
		color /= quality * dir - 15.0;
	}

	if(_IsNegOn)
	{
		FragColor = vec4(1.0 - color.rgb, 1.0);
	}
	else
	{
		FragColor = color;
	}
}
