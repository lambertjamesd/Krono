
#include "DataPassStructures.hlsli"

Texture2D textureTest : register( t0 );
SamplerState samPoint : register( s0 );

cbuffer SceneViewData : register( b0 )
{
	float4x4 projectionViewMatrix;
	float4x4 projectionViewInvMatrix;
	float4 screenSize;
};

float4 Main(TexCoordVertex shaderInput) : SV_TARGET
{
	float2 texCoord = shaderInput.position.xy * screenSize.zw;
	
	float3 inputColor = textureTest.Sample(samPoint, shaderInput.texCoord).rgb;
	float gray = dot(inputColor, float3(0.299, 0.587, 0.114));
	return float4(inputColor / (1.0 + gray), 1.0);
}
