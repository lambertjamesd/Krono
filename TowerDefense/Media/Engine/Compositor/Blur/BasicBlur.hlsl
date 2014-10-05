
#include "../../Shaders/DataTypes.hlsli"

Texture2D inputTexture : register( t0 );
SamplerState samPoint : register( s0 );

cbuffer SceneViewData : register( b0 )
{
	float4x4 projectionViewMatrix;
	float4x4 projectionViewInvMatrix;
	float4 screenSize;
};

float4 Main(PSCompositeInput shaderInput) : SV_TARGET
{
	float2 texCoord = shaderInput.position.xy * screenSize.zw;
	float4 result = inputTexture.SampleLevel(samPoint, texCoord, 0);
	result += inputTexture.SampleLevel(samPoint, texCoord - float2(screenSize.z, 0), 0);
	result += inputTexture.SampleLevel(samPoint, texCoord + float2(screenSize.z, 0), 0);
	result += inputTexture.SampleLevel(samPoint, texCoord - float2(0, screenSize.w), 0);
	result += inputTexture.SampleLevel(samPoint, texCoord + float2(0, screenSize.w), 0);
	
	return result / 5;
}
