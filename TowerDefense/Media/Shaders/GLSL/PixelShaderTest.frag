#version 420

in TextureNormal {
	vec2 texCoord;
	vec3 normal;
};

layout(binding = 0) uniform sampler2D textureTest;

layout(location = 0) out vec4 colorOut;
layout(location = 1) out vec4 normalOut;

void main()
{
	colorOut = vec4(texture2D(textureTest, texCoord).rgb, 1);
	normalOut = vec4(normal, 1);
}
