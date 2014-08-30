
#include "DataPassStructures.hlsli"

Texture2D textureTest : register( t0 );
SamplerState samPoint : register( s0 );

CompositeOutput main(NormalTexCoordVertex input)
{
	CompositeOutput result;
	result.color = float4(textureTest.Sample(samPoint, input.texCoord).rgb, 1.0);
	result.normal = float4(normalize(input.normal), 1);
	return result;
}