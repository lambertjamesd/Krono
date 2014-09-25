
#include "../../Shaders/DataTypes.hlsli"

Texture2D depthTexture : register( t0 );
Texture2D normalTexture : register( t1 );

Texture2D noiseTexture : register( t2 );

SamplerState samPoint : register( s0 );

cbuffer SceneViewDataPix : register( b0 )
{
	float4x4 projectionViewMatrixPix;
	float4x4 projectionViewInvMatrixPix;
	float4 screenSizePix;
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
	float4 result = mul(projectionViewInvMatrixPix, float4(normalizedPos, 1.0));
	result /= result.w;
	return result.xyz;
}

float3 NormalizedPosition(float3 worldPosition)
{
	float4 result = mul(projectionViewMatrixPix, float4(worldPosition, 1.0));
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

float Main(PositionNormalTexture shaderInput) : SV_TARGET
{
	float2 texCoord = shaderInput.position.xy * screenSizePix.zw;

	float depth = depthTexture.Sample(samPoint, texCoord).r;
	
	if (depth == 1.0)
	{
		return 1.0;
	}
	
	float3 worldPosition = WorldPosition(float3(TexToNorm(texCoord), depth));
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
		
		float3 sampleWorldPos = NormalizedPosition(worldPosition + hemisphere * maxRadius);
		float2 sampleTexCoord = NormToTex(sampleWorldPos.xy);
		float depthOffset = depthTexture.SampleLevel(samPoint, sampleTexCoord, 0).r - sampleWorldPos.z;
		
		if (depthOffset > 0 || depthOffset < -maxRadius)
		{
			oclussionFactor += 1.0;
		}
	}
	
	oclussionFactor /= numberOfSamples;
	
	return oclussionFactor;
}
