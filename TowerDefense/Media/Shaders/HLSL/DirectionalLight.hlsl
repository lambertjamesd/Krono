
#include "DataPassStructures.hlsli"

Texture2D color : register( t0 );
SamplerState samPoint : register( s0 );

Texture2D normal : register( t1 );

float4 Main(TexCoordVertex shaderInput) : SV_TARGET
{
	float diffuseAmount = saturate(dot(normal.Sample(samPoint, shaderInput.texCoord).xyz, float3(0.0, 0.7, -0.7)));
	return float4(diffuseAmount * color.Sample(samPoint, shaderInput.texCoord).rgb, 1.0);
}