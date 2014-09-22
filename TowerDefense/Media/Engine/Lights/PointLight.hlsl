
#include "../Shaders/DataTypes.hlsli"

Texture2D color : register( t0 );
Texture2D normal : register( t1 );
Texture2D depth : register( t2 );

SamplerState samPoint : register( s0 );

cbuffer SceneViewDataPix : register( b0 )
{
	float4x4 projectionMatrixPix;
	float4x4 projectionViewInvMatrixPix;
	float4 screenSizePix;
};

cbuffer PointLight : register( b1 )
{
	float4 lightPosition;
	float4 lightColor;
	float lightRadius;
};

float4 Main(PositionNormalTexture pixelInput) : SV_TARGET
{
	float3 textureCoord = float3(pixelInput.position.xy * screenSizePix.zw, pixelInput.position.z);	
	
	float screenZ = depth.Sample(samPoint, textureCoord.xy).r;
	
#ifdef OPENGL
	screenZ = screenZ * 2 - 1;
#endif

	if (screenZ < textureCoord.z)
	{
		float3 normalizedSpace = float3(textureCoord.xy * 2 - 1, screenZ);
		
		float4 worldPosiiton = mul(projectionViewInvMatrixPix, float4(normalizedSpace, 1.0));
		worldPosiiton /= worldPosiiton.w;
		
		float3 offset = lightPosition.xyz - worldPosiiton.xyz;
		float lightDistance = length(offset);

		if (lightDistance > lightRadius)
		{
			discard;
		}

		float falloff = 1 - lightDistance / lightRadius;
	
		float4 diffuseColor = color.Sample(samPoint, textureCoord.xy);

		float3 pointNormal = normal.Sample(samPoint, textureCoord.xy).xyz;
		float3 lightDirection = offset / lightDistance;

		float diffuseFactor = saturate(dot(pointNormal, lightDirection));

		return float4(diffuseColor.xyz * diffuseFactor, 1.0);
	}
	else
	{
		discard;
		return float4(0, 0, 0, 1);
	}
}
