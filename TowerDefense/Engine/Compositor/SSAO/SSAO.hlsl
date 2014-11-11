
#include "../../Shaders/DataTypes.hlsli"

Texture2D depthTexture : register( t0 );
Texture2D normalTexture : register( t1 );

Texture2D noiseTexture : register( t2 );

SamplerState samPoint : register( s0 );

cbuffer SceneViewData : register( b0 )
{
	float4x4 projectionViewMatrix;
	float4x4 projectionViewInvMatrix;
	float4 screenSize;
};

cbuffer SSAOParameters : register( b1 )
{
	float maxRadius;
	int numberOfSamples;
};


float4 SampleNoise(float4 noiseInput)
{
	return noiseTexture.SampleLevel(samPoint, frac(noiseInput.xy * noiseInput.zw), 0);
}

float3 PointInSphere(float3 seed)
{
	float3 result = seed;
	
	for (int i = 0; i < 5; ++i)
	{
		result = SampleNoise(float4(result, 1.0)).xyz * 2 - 1;
		
		if (dot(result, result) < 1)
		{
			return result;
		}
	}
	
	return result;
}

float3 WorldPosition(float3 normalizedPos)
{
#ifdef OPENGL
	normalizedPos.z = normalizedPos.z * 2 - 1;
#endif
	float4 result = mul(projectionViewInvMatrix, float4(normalizedPos, 1.0));
	result /= result.w;
	return result.xyz;
}

float3 NormalizedPosition(float3 worldPosition)
{
	float4 result = mul(projectionViewMatrix, float4(worldPosition, 1.0));
	result /= result.w;
#ifdef OPENGL
	result.z = (result.z + 1) * 0.5;
#endif
	return result.xyz;
}

float2 TexToNorm(float2 texturePos)
{
#ifdef OPENGL
	return texturePos.xy * float2(2.0, 2.0) - float2(1.0, 1.0);
#else
	return texturePos.xy * float2(2.0, -2.0) - float2(1.0, -1.0);
#endif
}

float2 NormToTex(float2 normXY)
{
#ifdef OPENGL
	return (normXY + float2(1.0, 1.0)) * float2(0.5, 0.5);
#else
	return (normXY + float2(1.0, -1.0)) * float2(0.5, -0.5);
#endif
}

float Main(PSCompositeInput shaderInput) : SV_TARGET
{
	float2 texCoord = shaderInput.position.xy * screenSize.zw;

	float depth = depthTexture.SampleLevel(samPoint, texCoord, 0).r;
	
	float2 normalizedPosition = shaderInput.normalizedPosition.xy / shaderInput.normalizedPosition.w;
	float2 dPixdNorm = screenSize.zw / float2(ddx(normalizedPosition.x), ddy(normalizedPosition.y));
	
	if (depth == 1.0)
	{
		return 0.0;
	}
	
	float3 worldPosition = WorldPosition(float3(normalizedPosition, depth));
	float3 worldNormal = normalTexture.SampleLevel(samPoint, texCoord, 0).xyz;
	
	float oclussionFactor = 0.0;
	
	float3 hemisphere = float3(shaderInput.position.xy * depth, 1.0);
	
	for (int i = 0; i < numberOfSamples; ++i)
	{
		hemisphere = PointInSphere(hemisphere);
		
		float hemisphereSide = dot(hemisphere, worldNormal);
		if (hemisphereSide < 0)
		{
			hemisphere -= 2 * worldNormal * hemisphereSide;
		}
		
		float3 sampleWorldPosition = worldPosition + hemisphere * maxRadius;
		float3 sampleNormalizedPos = NormalizedPosition(sampleWorldPosition);
		
		if (any(float4(sampleNormalizedPos.xy, -sampleNormalizedPos.xy) < float4(-1, -1, -1, -1)))
		{
			oclussionFactor += 1.0;
			continue;
		}
		
		float2 sampleTexCoord = (sampleNormalizedPos.xy - normalizedPosition) * dPixdNorm;
		float sampleDepth = depthTexture.SampleLevel(samPoint, texCoord + sampleTexCoord, 0).r;
		
		float depthOffset = sampleDepth - sampleNormalizedPos.z;
		float sampleDistance = length(WorldPosition(float3(sampleNormalizedPos.xy, sampleDepth)) - sampleWorldPosition);
		
		if (depthOffset > 0 || sampleDistance > maxRadius)
		{
			oclussionFactor += 1.0;
		}
	}
	
	oclussionFactor /= numberOfSamples;
	
	return oclussionFactor;
}
