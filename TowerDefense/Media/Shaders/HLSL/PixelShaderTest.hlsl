
#include "DataPassStructures.hlsli"

Texture2D textureTest : register( t0 );
SamplerState samPoint : register( s0 );

CompositeOutput Main(NormalTexCoordVertex psInput)
{
	CompositeOutput result;
	result.color = float4(textureTest.Sample(samPoint, psInput.texCoord).rgb, 1.0);
	result.normal = float4(psInput.normal, 1);
	return result;
}