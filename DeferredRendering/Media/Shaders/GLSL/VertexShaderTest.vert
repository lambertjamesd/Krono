
attribute vec3 attrPOSITION0;
attribute vec2 attrTEXCOORD0;

varying vec4 color;

layout(binding = 0) uniform TestingBlock
{
	mat4 projection;
	mat4 modelView;
};

void main()
{
	gl_Position = projection * vec4(attrPOSITION0, 1);
	color = vec4(attrTEXCOORD0, 0.0, 1.0);
}