
#include "DataPassStructures.hlsli"

cbuffer SceneViewData : register(b0)
{
	float4x4 projectionMatrix;
	float4x4 projectionInvMatrix;
	float4x4 compositeTransform;
};

TexCoordVertex main( float4 pos : POSITION, float2 texCoord : TEXCOORD)
{
	TexCoordVertex result;
	result.position = mul(compositeTransform, pos);
	result.texCoord = texCoord;
	return result;
}