#version 420

in vec3 attrPOSITION0;
in vec2 attrTEXCOORD0;

out vec2 texCoord;

layout(binding = 0) uniform SceneViewData
{
	mat4 projectionMatrix;
	mat4 projectionInvMatrix;
	mat4 compositeTransform;
};

void main()
{
	gl_Position = compositeTransform * vec4(attrPOSITION0, 1);
	texCoord = attrTEXCOORD0;
}
