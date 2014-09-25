
#include "DataTypes.hlsli"

cbuffer SceneViewData : register(b0)
{
	float4x4 projectionViewMatrix;
	float4x4 projectionViewInvMatrix;
	float4x4 compositeTransform;
	float4x4 projectionMatrixRasterSpace;
};

PositionNormalTexture Main( float3 pos : POSITION0, float2 texCoord : TEXCOORD0)
{
	PositionNormalTexture result;
	result.position = mul(compositeTransform, float4(pos, 1));
	result.texCoord = texCoord;
#ifdef OPENGL
	result.texCoord.y = 1 - result.texCoord.y;
#endif
	return result;
}
