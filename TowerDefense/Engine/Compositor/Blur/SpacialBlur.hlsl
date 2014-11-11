
#include "../../Shaders/DataTypes.hlsli"
#include "../../Shaders/ScreenSpace.hlsli"

Texture2D inputTexture : register( t0 );
Texture2D depthTexture : register( t1 );
SamplerState samPoint : register( s0 );

cbuffer SceneViewData : register( b0 )
{
	float4x4 projectionViewMatrix;
	float4x4 projectionViewInvMatrix;
	float4 screenSize;
};

cbuffer BlurParameters : register( b1 )
{
	float2 direction;
	float maxDistance;
	int maxSampleCount;
};

float4 SampleDirection(float2 texCoord, float2 direction, float3 normalizedPosition, float3 normalizedStep, inout float weight)
{
	int currentSample = 0;
	float3 worldPos = WorldPosition(projectionViewInvMatrix, normalizedPosition);
	
	float4 result = float4(0, 0, 0, 0);
	
	while (currentSample < maxSampleCount)
	{
		float depth = depthTexture.SampleLevel(samPoint, texCoord, 0).r;
		float3 samplePos = WorldPosition(projectionViewInvMatrix, float3(normalizedPosition.xy, depth));
		
		float distance = length(worldPos - samplePos);
		
		if (distance > maxDistance)
		{
			break;
		}
		
		float sampleWeight = 1 - distance / maxDistance;
		
		result += inputTexture.SampleLevel(samPoint, texCoord, 0) * sampleWeight;
		weight += sampleWeight;
		texCoord += direction;
		normalizedPosition += normalizedStep;
		++currentSample;
	}
	
	return result;
}

float4 Main(PSCompositeInput shaderInput) : SV_TARGET
{
	float2 texCoord = shaderInput.position.xy * screenSize.zw;
	float4 result = inputTexture.SampleLevel(samPoint, texCoord, 0);
	float depth = depthTexture.SampleLevel(samPoint, texCoord, 0).r;
	
	float2 step = direction * screenSize.zw;
	float3 normalizedPosition = float3(shaderInput.normalizedPosition.xy / shaderInput.normalizedPosition.w, depth);
	float3 normalizedStep = float3(direction * float2(ddx(normalizedPosition.x), ddy(normalizedPosition.y)), 0);
	float weight = 1.0;
	result += SampleDirection(texCoord + step, step, normalizedPosition + normalizedStep, normalizedStep, weight);
	result += SampleDirection(texCoord - step, -step, normalizedPosition - normalizedStep, -normalizedStep, weight);
	
	return result / weight;
}
