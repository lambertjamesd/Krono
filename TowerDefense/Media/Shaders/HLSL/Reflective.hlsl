
#include "DataPassStructures.hlsli"

cbuffer ReflectiveColor : register( b0 )
{
	float4 diffuseColor;
	float4 specularColor;
	float4 emmisiveColor;
};

CompositeOutput Main(NormalTexCoordVertex psInput)
{
	CompositeOutput result;
	result.color = diffuseColor;
	result.normal = float4(normalize(psInput.normal), 1);
	result.specular = specularColor;
	result.emmisive = emmisiveColor;
	return result;
}