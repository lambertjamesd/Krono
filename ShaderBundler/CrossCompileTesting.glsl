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
	uniform mat4 projectionMatrix;
	uniform vec4 color;
	uniform float arrayTypedef[4];
	uniform float arrayTestConstant[2];
};
layout(binding = 0) uniform sampler2D diffuseTexture;
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
	}	 while (foo);
	if (foo)
	{
	}	else

	{
	}
	for (float meh = 1.0; meh; ++meh)
	{
		continue;
	}
	switch (foo)
	{
		
	case 0:

		break;
		
	case 1:

		break;
		
	default:

		break;
	}
	while (foo)
	{
	}
	discard;
}
struct FooBar
{
	vec4 meh;
};
struct VSOutput
{
	vec4 position;
	vec2 texCoord;
	FooBar meh;
};
struct PSOutput
{
	vec4 position;
	vec2 texCoord;
};
PSOutput Main(VSOutput input)
{
	PSOutput result;
	result.position = input.position;
	result.texCoord = input.texCoord;
	return result;
}
in vec2 varyingTexCoord0;
in vec4 varyingTexCoord1;

layout(binding = 0) out vec4 targetSV_Target0;
layout(binding = 1) out vec2 targetSV_Target1;
void main()
{
	VSOutput input;
	input.position = gl_FragCoord;
	input.texCoord = varyingTexCoord0;
	input.meh.meh = varyingTexCoord1;
	PSOutput result = Main(input);
	targetSV_Target0 = result.position;
	targetSV_Target1 = result.texCoord;
}