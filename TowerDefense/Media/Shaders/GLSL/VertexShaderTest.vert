#version 440

attribute vec3 attrPOSITION0;
attribute vec2 attrTEXCOORD0;

out vec2 texCoord;

layout(binding = 0) uniform SceneViewData
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
	mat4 projectionViewMatrix;
};

layout(binding = 1) uniform EntityData
{
	mat4 modelMatrix;
};

void main()
{
	gl_Position = projectionViewMatrix * modelMatrix * vec4(attrPOSITION0, 1);
	texCoord = attrTEXCOORD0;
}
