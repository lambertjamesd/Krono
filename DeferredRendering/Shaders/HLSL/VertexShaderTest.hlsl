
#include "DataPassStructures.hlsli"

ColorVertex main( float4 pos : POSITION, float4 color : COLOR)
{
	ColorVertex result;
	result.position = pos;
	result.color = color;
	return result;
}