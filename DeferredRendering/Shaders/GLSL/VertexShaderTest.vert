
attribute vec3 attrPOSITION0;
attribute vec4 attrCOLOR0;

varying vec4 color;

void main()
{
	gl_Position = vec4(attrPOSITION0, 1);
	color = attrCOLOR0;
}