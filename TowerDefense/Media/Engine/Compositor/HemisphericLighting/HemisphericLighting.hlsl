
#include "../../Shaders/DataTypes.hlsli"

Texture2D occlusionTexture : register( t0 );
Texture2D normalTexture : register( t1 );
Texture2D colorTexture : register( t2 );

SamplerState samPoint : register( s0 );

cbuffer SceneViewData : register( b0 )
{
	float4x4 projectionViewMatrix;
	float4x4 projectionViewInvMatrix;
	float4 screenSize;
};

cbuffer SSAOParameters : register( b1 )
{
	float4 skyColor;
	float4 groundColor;
	float3 up;
};

float4 Main(PositionNormalTexture shaderInput) : SV_TARGET
{
	float2 texCoord = shaderInput.position.xy * screenSize.zw;
	float occlusion = occlusionTexture.Sample(samPoint, texCoord).r;
	float3 worldNormal = normalTexture.SampleLevel(samPoint, texCoord, 0).xyz;
	float4 worldColor = colorTexture.SampleLevel(samPoint, texCoord, 0);
	float lerpFactor = dot(up, worldNormal) * 0.5 + 0.5;
	return worldColor * lerp(groundColor, skyColor, float4(lerpFactor, lerpFactor, lerpFactor, lerpFactor)) * occlusion;
}
