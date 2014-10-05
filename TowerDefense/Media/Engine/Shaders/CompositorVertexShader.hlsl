
#include "DataTypes.hlsli"

cbuffer SceneViewData : register(b0)
{
	float4x4 projectionViewMatrix;
	float4x4 projectionViewInvMatrix;
	float4x4 compositeTransform;
	float4x4 projectionMatrixRasterSpace;
};

PSCompositeInput Main( float3 pos : POSITION0, float2 texCoord : TEXCOORD0)
{
	PSCompositeInput result;
	result.position = mul(compositeTransform, float4(pos, 1));
	result.normalizedPosition = result.position;
	return result;
}
