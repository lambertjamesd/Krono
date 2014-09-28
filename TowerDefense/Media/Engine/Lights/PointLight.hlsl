
#include "../Shaders/DataTypes.hlsli"
#include "../Shaders/ScreenSpace.hlsli"

Texture2D color : register( t0 );
Texture2D normal : register( t1 );
Texture2D depth : register( t2 );
Texture2D specular : register( t3 );

SamplerState samPoint : register( s0 );

cbuffer SceneViewData : register( b0 )
{
	float4x4 projectionViewMatrix;
	float4x4 projectionViewInvMatrix;
	float4 screenSizePix;
	float3 viewDirection;
	float3 viewPosition;
};

cbuffer PointLight : register( b1 )
{
	float4 lightPosition;
	float4 lightColor;
	float lightRange;
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
		float4 diffuseColor = color.Sample(samPoint, textureCoord.xy);
		float3 pointNormal = normal.Sample(samPoint, textureCoord.xy).xyz;
		float4 specularColor = specular.Sample(samPoint, textureCoord.xy);
		
		float3 normalizedSpace = float3(TexToNorm(textureCoord.xy), screenZ);
		
		float4 worldPosiiton = mul(projectionViewInvMatrix, float4(normalizedSpace, 1.0));
		worldPosiiton /= worldPosiiton.w;
		
		float3 offset = lightPosition.xyz - worldPosiiton.xyz;
		float lightDistance = length(offset);

		if (lightDistance > lightRange)
		{
			discard;
		}

		float falloff = 1 - lightDistance / lightRange;
	

		float3 lightDirection = offset / lightDistance;

		float diffuseFactor = saturate(dot(pointNormal, lightDirection));
		float specularFactor = 0;
		
		if (diffuseFactor > 0)
		{
			float3 halfVector = normalize(lightDirection + normalize(viewPosition - worldPosiiton));
			specularFactor = pow(saturate(dot(pointNormal, halfVector)), specularColor.a * 256);
		}
		
		float attenuationFactor = 1.0 / (lightDistance * lightDistance) - 1.0 / (lightRange * lightRange);

		return float4(
			lightColor.rgb * attenuationFactor * (
				diffuseColor.rgb * diffuseFactor + 
				specularColor.rgb * specularFactor), 1.0);
	}
	else
	{
		discard;
		return float4(0, 0, 0, 1);
	}
}
