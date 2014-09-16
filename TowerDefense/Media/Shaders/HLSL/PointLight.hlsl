
#include "DataPassStructures.hlsli"

Texture2D color : register( t0 );
Texture2D normal : register( t1 );
Texture2D depth : register( t2 );

SamplerState samPoint : register( s0 );

cbuffer SceneViewDataPix : register(b0)
{
	float4x4 projectionMatrixPix;
	float4x4 projectionInvMatrixPix;
	float4 screenSizePix;
};

float4 Main(ScreenPositionVertex pixelInput) : SV_TARGET
{
	float3 textureCoord = pixelInput.rasterPosition.xyz / pixelInput.rasterPosition.w;	

#ifdef OPENGL
	textureCoord.y = 1 - textureCoord.y;
#endif

	float screenZ = depth.Sample(samPoint, textureCoord.xy).r;
	
#ifdef OPENGL
	screenZ = screenZ * 2 - 1;
#endif
	
	if (screenZ > textureCoord.z)
	{
		discard;
		return float4(0, 0, 0, 0);
	}
	else
	{
		float2 xyPos = textureCoord.xy * float2(2.0, -2.0) + float2(-1.0, 1.0);
		float4 viewPos = mul(projectionInvMatrixPix, float4(xyPos, screenZ, 1.0));
		viewPos /= viewPos.w;
		
		float3 lightDir = float3(1, 0, 1) - viewPos.xyz;
		
		float lightDistance = length(lightDir);
		float lightFactor = saturate(dot(lightDir, normal.Sample(samPoint, textureCoord.xy).rgb) / lightDistance);
	
		// simple attenuation
		lightFactor *= max(1 - lightDistance, 0) * 10;
	
		return float4(color.Sample(samPoint, textureCoord.xy).xyz * lightFactor , 1.0);	
	}
}
