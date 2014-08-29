#version 420

in vec2 texCoord;

layout(binding = 0) uniform sampler2D color;
layout(binding = 1) uniform sampler2D normal;

layout(location = 0) out vec4 colorOut;

void main()
{
	float diffuseFactor = dot(texture2D(normal, texCoord).rgb, vec3(0, 0.7, -0.7));
	vec3 diffuseColor = texture2D(color, texCoord).rgb * diffuseFactor;

	colorOut = vec4(diffuseColor, 1.0);
}
