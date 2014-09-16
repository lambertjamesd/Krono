#version 420

in vec3 attrPOSITION0;
in vec3 attrNORMAL0;
in vec2 attrTEXCOORD0;

out	vec2 varyingTexCoord;
out	vec3 varyingNormal;

layout(binding = 0) uniform SceneViewData
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
	mat4 projectionViewMatrix;
};

layout(binding = 1) uniform EntityData
{
	mat4 modelMatrix;
	mat4 viewModelMatrix;
	mat4 projectionViewModelMatrix;
	mat4 normalMatrix;
};

void main()
{
	gl_Position = projectionViewModelMatrix * vec4(attrPOSITION0, 1);
	varyingNormal = (normalMatrix * vec4(attrNORMAL0, 0.0)).xyz;
	varyingTexCoord = attrTEXCOORD0;
}
