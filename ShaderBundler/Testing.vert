#version 420

in vec3 attrPOSITION0;

void main()
{
	gl_Position = vec4(attrPOSITION0, 1.0);
}