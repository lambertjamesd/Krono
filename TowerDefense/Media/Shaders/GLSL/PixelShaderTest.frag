#version 420

in vec2 texCoord;
in vec3 normal;

layout(binding = 0) uniform sampler2D textureTest;

layout(location = 0) out vec4 colorOut;
layout(location = 1) out vec4 normalOut;

void main()
{
	colorOut = texture2D(textureTest, texCoord);
	normalOut = vec4(normalize(normal), 1.0);
}
