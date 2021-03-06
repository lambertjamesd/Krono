
#include "DataPassStructures.hlsli"

cbuffer SceneViewData : register(b0)
{
	float4x4 projectionMatrix;
	float4x4 viewMatrix;
	float4x4 projectionViewMatrix;
};

cbuffer EntityData : register(b1)
{
	float4x4 modelMatrix;
	float4x4 viewModelMatrix;
	float4x4 projectionViewModelMatrix;
	float4x4 normalMatrix;
};

NormalTexCoordVertex Main( float3 pos : POSITION0, float3 normal : NORMAL0, float2 texCoord : TEXCOORD0)
{
	NormalTexCoordVertex result;
	result.position = mul(projectionViewModelMatrix, float4(pos, 1));
	result.normal = (mul(normalMatrix, float4(normal, 0))).xyz;
	result.texCoord = texCoord;
	return result;
}