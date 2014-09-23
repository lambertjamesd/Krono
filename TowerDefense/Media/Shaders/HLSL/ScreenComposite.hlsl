
#include "DataPassStructures.hlsli"

cbuffer SceneViewData : register(b0)
{
	float4x4 projectionMatrix;
	float4x4 projectionInvMatrix;
	float4x4 compositeTransform;
	float4x4 projectionMatrixRasterSpace;
};

TexCoordVertex Main( float3 pos : POSITION0, float2 texCoord : TEXCOORD0)
{
	TexCoordVertex result;
	result.position = mul(compositeTransform, float4(pos, 1));
	result.texCoord = texCoord;
#ifdef OPENGL
	result.texCoord.y = 1 - result.texCoord.y;
#endif
	return result;
}
