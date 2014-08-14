
#include "DataPassStructures.hlsli"

Texture2D textureTest : register( t0 );
SamplerState samLinear : register( s0 );

float4 main(TexCoordVertex input) : SV_TARGET
{
	return float4(textureTest.Sample(samLinear, input.texCoord).rgb, 1.0);
}