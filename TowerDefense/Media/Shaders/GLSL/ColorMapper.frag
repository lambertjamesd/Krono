#version 420

in vec2 texCoord;

layout(binding = 0) uniform PixelCompositeData
{
	mat4 projectionMatrixPix;
	mat4 projectionInvMatrixPix;
	vec4 screenSize;
};

layout(binding = 0) uniform sampler2D textureTest;


layout(location = 0) out vec4 colorOut;

void main()
{	
	colorOut = vec4(texture2D(textureTest, texCoord).rgb, 1.0);
}
