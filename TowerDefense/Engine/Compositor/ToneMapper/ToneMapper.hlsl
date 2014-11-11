
#include "../../Shaders/DataTypes.hlsli"

Texture2D textureTest : register( t0 );

cbuffer SceneViewData : register( b0 )
{
	float4x4 projectionViewMatrix;
	float4x4 projectionViewInvMatrix;
	float4 screenSize;
};

float4 Main(PSCompositeInput shaderInput) : SV_TARGET
{	
	float3 inputColor = textureTest.Load(int3(shaderInput.position.xy, 0)).rgb;
	float gray = dot(inputColor, float3(0.299, 0.587, 0.114));
	return float4(inputColor / (1.0 + gray), 1.0);
}
