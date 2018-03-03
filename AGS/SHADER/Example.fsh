#version 460 core

uniform	vec4 Offset;
uniform	vec4 Color;

in float gradient;

void main (void)
{
	gl_FragColor = vec4(Color) * abs(gradient -0.5) * 2 +  vec4(0.0, 0.0, 0.0, 1.0) * abs(gradient -0.5) * 2;
}
