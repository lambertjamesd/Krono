
attribute vec3 attrPOSITION0;
attribute vec2 attrTEXCOORD0;

varying vec2 texCoord;

void main()
{
	gl_Position = vec4(attrPOSITION0, 1);
	texCoord = vec2(1.0, 0.0) - attrTEXCOORD0;
}