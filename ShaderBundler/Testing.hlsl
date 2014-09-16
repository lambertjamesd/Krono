#include "IncludeTest.hlsli"

Texture2D diffuse : register( t0 );
SamplerState pointSampler : register( s0 );

float4 main( float4 pos : POSITION ) : SV_POSITION
{
	return saturate(pos);
}