
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

Texture2D diffuseTexture : register( t0 );
SamplerState pointSampler : register( s0 );

#ifdef OPENGL

struct GLSLOnly
{
	float4 huzzah;
};

#endif

#ifndef OPENGL

struct HLSLOnly
{
	float4 nope;
};

#endif

SamplerState SelectSampler()
{
	return pointSampler;
}

float4 OtherFunction(float4 yay, SamplerState samplerParameter)
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

	SamplerState samplerState = SelectSampler();
	
	testing = OtherFunction((vector<float,4>)vector<int,4>(1, 2, 3, 4), SelectSampler());

	do
	{
		if (foo)
		{
		
		}
	}
	while (foo);
	
	if (foo)
	{
	
	}
	else
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

float4 Main(float4 position : POSITION0) : SV_Position
{
	return position;
}