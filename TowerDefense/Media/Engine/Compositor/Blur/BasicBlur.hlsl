
#include "../../Shaders/DataTypes.hlsli"

Texture2D inputTexture : register( t0 );
SamplerState samPoint : register( s0 );

cbuffer SceneViewDataPix : register( b0 )
{
	float4x4 projectionViewMatrixPix;
	float4x4 projectionViewInvMatrixPix;
	float4 screenSizePix;
};

float4 Main(PositionNormalTexture shaderInput) : SV_TARGET
{
	float2 texCoord = shaderInput.position.xy * screenSizePix.zw;
	float4 result = inputTexture.SampleLevel(samPoint, texCoord, 0);
	result += inputTexture.SampleLevel(samPoint, texCoord - float2(screenSizePix.z, 0), 0);
	result += inputTexture.SampleLevel(samPoint, texCoord + float2(screenSizePix.z, 0), 0);
	result += inputTexture.SampleLevel(samPoint, texCoord - float2(0, screenSizePix.w), 0);
	result += inputTexture.SampleLevel(samPoint, texCoord + float2(0, screenSizePix.w), 0);
	
	return result / 5;
}
