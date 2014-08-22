#version 420

in vec2 texCoord;

layout(binding = 0) uniform sampler2D textureTest;

layout(location = 0) out vec4 colorOut;

void main()
{
	if (texCoord.x > 0.5)
	{
		colorOut = texture2D(textureTest, texCoord);
	}
	else
	{
		colorOut = texture2D(textureTest, texCoord);
	}
}
