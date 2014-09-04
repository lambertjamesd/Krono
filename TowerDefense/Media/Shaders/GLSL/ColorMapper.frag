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
	vec3 inputColor = texture2D(textureTest, texCoord).rgb;
	float gray = dot(inputColor, vec3(0.299, 0.587, 0.114));
	colorOut = vec4(inputColor / (1 + gray), 1.0);
}
