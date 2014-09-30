
#include "DataTypes.hlsli"

Texture2D diffuseColor : register( t0 );
Texture2D specularColor : register( t1 );
Texture2D normalMap : register( t2 );
Texture2D noiseTexture : register( t3 );

SamplerState anisoscopic : register( s0 );
SamplerState pointSampling : register( s1 );

CompositeOutput Main(PositionNormalTangentTexture psInput)
{
	CompositeOutput result;
	
	float2 textureXSlope = ddx(psInput.texCoord);
	float2 textureYSlope = ddy(psInput.texCoord);
	
	float2 noise = noiseTexture.SampleLevel(pointSampling, psInput.position.xy / 64, 0).xy - 0.5;
	
	float2 jitteredCoordinate = psInput.texCoord + textureXSlope * noise.x + textureYSlope * noise.y;
	
	result.color = float4(diffuseColor.Sample(anisoscopic, jitteredCoordinate).rgb, 1.0);
	result.specular = specularColor.Sample(anisoscopic, jitteredCoordinate);
	float3 sampleNormal = normalMap.Sample(anisoscopic, jitteredCoordinate).rgb * 2 - 1;
	
	float3 faceNormal = normalize(psInput.normal.xyz);
	float3 faceTangent = normalize(psInput.tangent.xyz);
	float3 faceBinormal = cross(faceNormal, faceTangent) * psInput.tangent.w;
	
	sampleNormal = mul(sampleNormal, float3x3(faceTangent, faceBinormal, faceNormal));
	
	result.normal = float4(sampleNormal, 1);
	result.emmisive = float4(0, 0, 0, 1);
	return result;
}