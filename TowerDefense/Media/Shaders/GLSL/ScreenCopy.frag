#version 420

varying vec2 texCoord;

layout(binding = 0) uniform sampler2D screen;
layout(binding = 1) uniform sampler2D depth;

void main()
{
	gl_FragColor = vec4(texture2D(screen, texCoord).xyz, 1.0);
}