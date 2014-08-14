
varying vec2 texCoord;

layout(binding = 0) uniform sampler2D textureTest;

void main()
{
	gl_FragColor = texture2D(textureTest, texCoord);
}