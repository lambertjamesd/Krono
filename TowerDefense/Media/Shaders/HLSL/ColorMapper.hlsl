
#include "DataPassStructures.hlsli"

Texture2D textureTest : register( t0 );
SamplerState samPoint : register( s0 );

float4 main(TexCoordVertex input) : SV_TARGET
{
	float3 inputColor = textureTest.Sample(samPoint, input.texCoord).rgb;
	float gray = dot(inputColor, float3(0.299, 0.587, 0.114));
	return float4(inputColor / (1 + gray), 1.0);
}