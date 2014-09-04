#version 420

in vec3 attrPOSITION0;
in vec3 attrNORMAL0;
in vec2 attrTEXCOORD0;

out ScreenPositionVertex {
	vec4 rasterPosition;
};

layout(binding = 0) uniform SceneViewDataVert
{
	mat4 projectionMatrix;
	mat4 projectionInvMatrix;
	mat4 compositeTransform;
	mat4 projectionMatrixRasterSpace;
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
	rasterPosition = projectionMatrixRasterSpace * viewModelMatrix * vec4(attrPOSITION0, 1);
}
