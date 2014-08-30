
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

NormalTexCoordVertex main( float3 pos : POSITION, float3 normal : NORMAL, float2 texCoord : TEXCOORD)
{
	NormalTexCoordVertex result;
	result.position = mul(projectionViewModelMatrix, float4(pos, 1));
	result.normal = (mul(normalMatrix, float4(normal, 0))).xyz;
	result.texCoord = texCoord;
	return result;
}