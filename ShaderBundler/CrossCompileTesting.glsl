#version 440



struct Foo
{
	vec4 bar;
	mat4 squareMatrix;
	bmat4x3 nonSquare;
	vec4 diffuse;
	vec4 diffuseFoo;
	int intArray[4];
};
layout(binding = 0) uniform ConstantBuffer
{
	mat4 projectionMatrix;
	vec4 color;
	float arrayTypedef[4];
	float arrayTestConstant[2];
};
struct GLSLOnly
{
	vec4 huzzah;
};
vec4 OtherFunction(vec4 yay)
{
	return 1 - yay;
}
void Test(int foo, in float bar[4], out mat3 output, inout mat2 inOutput)
{
	vec4 testing;
	testing.x = testing.x + 1;
	float arrayTest[4];
	int fooBar = 10;
	arrayTest[0] = 1.0;
	arrayTest[1] = (arrayTest[0]++) + 1;
	arrayTest[2] = -arrayTest[1];
	arrayTest[3] = (float)fooBar;
	testing = OtherFunction((vec4)ivec4(1, 2, 3, 4));
	do
	{
		if (foo)
		{
		}
	}	 while (identifier);
	if (bar)
	{
	}
	for (float meh; meh; meh)
	{
		continue;
	}
	switch (monkey)
	{
		
	case 0:

		break;
		
	case 1:

		break;
		
	default:

		break;
	}
	while (yes)
	{
	}
	discard;
}
