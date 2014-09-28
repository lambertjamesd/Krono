
#include "DataPassStructures.hlsli"

Texture2D diffuseColor : register( t0 );
Texture2D specularColor : register( t1 );
SamplerState samPoint : register( s0 );

CompositeOutput Main(NormalTexCoordVertex psInput)
{
	CompositeOutput result;
	result.color = float4(diffuseColor.Sample(samPoint, psInput.texCoord).rgb, 1.0);
	result.normal = float4(normalize(psInput.normal), 1);
	result.specular = specularColor.Sample(samPoint, psInput.texCoord);
	result.emmisive = float4(0, 0, 0, 1);
	return result;
}