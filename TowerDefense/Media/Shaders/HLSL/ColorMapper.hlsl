
#include "DataPassStructures.hlsli"

Texture2D textureTest : register( t0 );
SamplerState samPoint : register( s0 );

float4 main(TexCoordVertex input) : SV_TARGET
{
	return float4(textureTest.Sample(samPoint, input.texCoord).rgb, 1.0);
}