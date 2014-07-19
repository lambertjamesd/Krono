
#include "DataPassStructures.hlsli"

cbuffer MatrixStructure : register(b0)
{
	float4x4 projection;
	float4x4 modelView;
};

ColorVertex main( float4 pos : POSITION, float2 texCoord : TEXCOORD)
{
	ColorVertex result;
	result.position = mul(projection, pos);
	result.color = float4(texCoord, 0.0, 1.0);
	return result;
}