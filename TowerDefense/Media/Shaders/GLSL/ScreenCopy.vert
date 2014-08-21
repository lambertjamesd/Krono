#version 420

attribute vec3 attrPOSITION0;
attribute vec2 attrTEXCOORD0;

varying vec2 texCoord;

void main()
{
	gl_Position = vec4(-attrPOSITION0.x, attrPOSITION0.yz, 1);
	texCoord = attrTEXCOORD0;
}