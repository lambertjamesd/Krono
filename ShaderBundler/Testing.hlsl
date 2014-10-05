#include "IncludeTest.hlsli"

Texture2D diffuse : register( t0 );
SamplerState pointSampler : register( s0 );

struct NestedStructure
{
	float bar;
};

struct SturctureTest
{
	float4 foo;
	NestedStructure bar;
};

cbuffer BufferTest : register ( b0 )
{
	SturctureTest structure;
	bool4 yeah;
};

float4 main( float4 pos : POSITION ) : SV_POSITION
{
	return saturate(pos + diffuse.Load(int3(0, 0, 0)));
}