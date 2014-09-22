
#include "DataTypes.hlsli"

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

PositionNormalTexture Main(StandardVertexInput vertexInput)
{
	PositionNormalTexture result;
	result.position = mul(projectionViewModelMatrix, float4(vertexInput.position, 1.0));
	result.normal - mul(normalMatrix, float4(vertexInput.normal, 0.0));
	result.texCoord = vertexInput.texCoord;
	return result;
}