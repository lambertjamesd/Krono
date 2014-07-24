
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
};

ColorVertex main( float4 pos : POSITION, float2 texCoord : TEXCOORD)
{
	ColorVertex result;
	result.position = mul(projectionViewMatrix, mul(modelMatrix, pos));
	result.color = float4(texCoord, 0.0, 1.0);
	return result;
}