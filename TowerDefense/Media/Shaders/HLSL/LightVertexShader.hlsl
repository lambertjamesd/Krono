
#include "DataPassStructures.hlsli"

cbuffer SceneViewData : register(b0)
{
	float4x4 projectionMatrix;
	float4x4 projectionInvMatrix;
	float4x4 compositeTransform;
	float4x4 projectionMatrixRasterSpace;
};

cbuffer EntityData : register(b1)
{
	float4x4 modelMatrix;
	float4x4 viewModelMatrix;
	float4x4 projectionViewModelMatrix;
	float4x4 normalMatrix;
};

ScreenPositionVertex Main( float3 pos : POSITION0, float3 normal : NORMAL0, float2 texCoord : TEXCOORD0)
{
	ScreenPositionVertex result;
	result.position = mul(projectionViewModelMatrix, float4(pos, 1));
	result.rasterPosition = mul(projectionMatrixRasterSpace, mul(viewModelMatrix, float4(pos, 1)));
	return result;
}