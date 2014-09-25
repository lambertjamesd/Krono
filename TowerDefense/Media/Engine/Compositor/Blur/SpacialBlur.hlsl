
#include "../../Shaders/DataTypes.hlsli"
#include "../../Shaders/ScreenSpace.hlsli"

Texture2D inputTexture : register( t0 );
Texture2D depthTexture : register( t1 );
SamplerState samPoint : register( s0 );

cbuffer SceneViewDataPix : register( b0 )
{
	float4x4 projectionViewMatrixPix;
	float4x4 projectionViewInvMatrixPix;
	float4 screenSizePix;
};

cbuffer BlurParameters : register( b1 )
{
	float2 direction;
	float maxDistance;
	int maxSampleCount;
};

float4 SampleDirection(float2 texCoord, float2 direction, float3 worldPos, inout float weight)
{
	int currentSample = 0;
	
	float4 result = float4(0, 0, 0, 0);
	
	while (currentSample < maxSampleCount)
	{
		float depth = depthTexture.SampleLevel(samPoint, texCoord, 0).r;
		float3 samplePos = WorldPosition(projectionViewInvMatrixPix, float3(TexToNorm(texCoord), depth));
		
		float distance = length(worldPos - samplePos);
		
		if (distance > maxDistance)
		{
			break;
		}
		
		float sampleWeight = 1 - distance / maxDistance;
		
		result += inputTexture.SampleLevel(samPoint, texCoord, 0) * sampleWeight;
		weight += sampleWeight;
		texCoord += direction;
		++currentSample;
	}
	
	return result;
}

float4 Main(PositionNormalTexture shaderInput) : SV_TARGET
{
	float2 texCoord = shaderInput.position.xy * screenSizePix.zw;
	float2 step = direction * screenSizePix.zw;
	float4 result = inputTexture.SampleLevel(samPoint, texCoord, 0);
	float depth = depthTexture.SampleLevel(samPoint, texCoord, 0).r;
	float3 worldPos = WorldPosition(projectionViewInvMatrixPix, float3(TexToNorm(texCoord), depth));
	float weight = 1.0;
	result += SampleDirection(texCoord + step, step, worldPos, weight);
	result += SampleDirection(texCoord - step, -step, worldPos, weight);
	
	return result / weight;
}
