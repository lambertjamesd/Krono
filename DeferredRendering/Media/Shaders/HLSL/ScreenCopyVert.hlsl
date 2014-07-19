
#include "DataPassStructures.hlsli"

TexCoordVertex main( float4 position : POSITION, float2 texCoord : TEXCOORD)
{
	TexCoordVertex result;

	result.position = position;
	result.texCoord = texCoord;

	return result;
}