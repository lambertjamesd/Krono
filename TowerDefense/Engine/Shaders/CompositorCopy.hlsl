
#include "DataTypes.hlsli"

Texture2D inputTexture : register( t0 );
SamplerState samPoint : register( s0 );

cbuffer SceneViewDataPix : register( b0 )
{
	float4x4 projectionViewMatrixPix;
	float4x4 projectionViewInvMatrixPix;
	float4 screenSizePix;
};

float4 Main(StandardVertexInput shaderInput) : SV_TARGET
{
	float2 texCoord = shaderInput.position.xy * screenSizePix.zw;
	return inputTexture.Sample(samPoint, texCoord);
}
