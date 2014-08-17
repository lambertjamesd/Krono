
#include "DataPassStructures.hlsli"

Texture2D textureTest : register( t0 );
SamplerState samPoint : register( s0 );
SamplerState samLinear : register( s1 );

float4 main(TexCoordVertex input) : SV_TARGET
{
	if (input.texCoord.x > 0.5)
	{
		return float4(textureTest.Sample(samPoint, input.texCoord).rgb, 1.0);
	}
	else
	{
		return float4(textureTest.Sample(samLinear, input.texCoord).rgb, 1.0);
	}
}