
varying vec2 texCoord;

layout(binding = 0) uniform sampler2D textureTest;
layout(binding = 0) uniform sampler2D textureTest_s1;

void main()
{
	if (texCoord.x > 0.5)
	{
		gl_FragColor = texture2D(textureTest, texCoord);
	}
	else
	{
		gl_FragColor = texture2D(textureTest_s1, texCoord);
	}
}