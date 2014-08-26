#version 420

in vec2 texCoord;

layout(binding = 0) uniform sampler2D textureTest;

layout(location = 0) out vec4 colorOut;

layout(binding = 0) uniform SceneViewData
{
	mat4 projectionMatrix;
	mat4 projectionInvMatrix;
	mat4 compositeTransform;
};

void main()
{
	colorOut = texture2D(textureTest, texCoord);
}
