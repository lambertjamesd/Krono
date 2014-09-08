
typedef float4 Color;

typedef Color ColorFoo;

#define ARRAY_SIZE	4

typedef float ArrayTest[ARRAY_SIZE];

struct Foo
{
	float4 bar;
	float4x4 squareMatrix;
	bool4x3 nonSquare;
	
	Color diffuse;
	ColorFoo diffuseFoo;
	
	int intArray[4];
};

cbuffer ConstantBuffer : register( b0 )
{
	float4x4 projectionMatrix;
	float4 color;
	ArrayTest arrayTypedef;
	float arrayTestConstant[2];
};

#ifdef GLSL

struct GLSLOnly
{
	float4 huzzah;
};

#endif

#ifndef GLSL

struct HLSLOnly
{
	float4 nope;
};

#endif

float4 OtherFunction(float4 yay)
{
	return 1 - yay;
}

void Test(int foo, in float bar[4], out float3x3 output, inout float2x2 inOutput)
{
	float4 testing;
	
	testing.x = testing.x + 1;
	
	float arrayTest[4];
	int fooBar = 10;
	
	arrayTest[0] = 1.0;
	arrayTest[1] = (arrayTest[0]++) + 1;
	arrayTest[2] = -arrayTest[1];
	arrayTest[3] = (float)fooBar;
	
	testing = OtherFunction((vector<float,4>)vector<int,4>(1, 2, 3, 4));

	do
	{
		if (foo)
		{
		
		}
	}
	while (identifier);
	
	if (bar)
	{
	
	}
	else
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