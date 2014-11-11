
#include "../Shaders/DataTypes.hlsli"

Texture2D color : register( t0 );
Texture2D normal : register( t1 );

SamplerState samPoint : register( s0 );

cbuffer SceneViewDataPix : register( b0 )
{
	float4x4 projectionMatrixPix;
	float4x4 projectionViewInvMatrixPix;
	float4 screenSizePix;
};

cbuffer DirectionalLight : register( b1 )
{
	float4 lightDirection;
	float4 lightColor;
};

float4 Main(PositionNormalTexture pixelInput) : SV_TARGET
{
	float2 textureCoord = float2(pixelInput.position.xy * screenSizePix.zw);
	float3 worldNormal = normal.Sample(samPoint, textureCoord).xyz;
	float diffuse = saturate(dot(worldNormal, -lightDirection.xyz));
	return float4(color.Sample(samPoint, textureCoord).xyz * diffuse, 1.0);
}
