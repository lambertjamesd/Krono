
#include "DataPassStructures.hlsli"

Texture2D color : register( t0 );
Texture2D normal : register( t1 );
Texture2D depth : register( t2 );

SamplerState samPoint : register( s0 );

cbuffer SceneViewData : register(b0)
{
	float4x4 projectionMatrix;
	float4x4 projectionInvMatrix;
	float4 screenSize;
};

float4 main(ScreenPositionVertex input) : SV_TARGET
{
	float3 textureCoord = input.rasterPosition.xyz / input.rasterPosition.w;
	float screenZ = depth.Sample(samPoint, textureCoord.xy).r;
	
	if (screenZ > textureCoord.z)
	{
		discard;
		return float4(0.0, 0.0, 0.0, 0.0);
	}
	else
	{
		float2 xyPos = textureCoord * float2(2.0, -2.0) + float2(-1.0, 1.0);
		float4 viewPos = mul(projectionInvMatrix, float4(xyPos, screenZ, 1.0));
		viewPos /= viewPos.w;
		
		float3 lightDir = float3(1, 0, 1) - viewPos.xyz;
		
		float lightDistance = length(lightDir);
		float lightFactor = saturate(dot(lightDir, normal.Sample(samPoint, textureCoord)) / lightDistance);
	
		// simple attenuation
		lightFactor *= max(1 - lightDistance, 0);
	
		return float4(color.Sample(samPoint, textureCoord) * lightFactor , 1.0);	
	}
}
