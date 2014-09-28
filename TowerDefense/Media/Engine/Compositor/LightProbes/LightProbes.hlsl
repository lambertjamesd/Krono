
#include "../../Shaders/DataTypes.hlsli"

Texture2D occlusionTexture : register( t0 );
Texture2D normalTexture : register( t1 );
Texture2D colorTexture : register( t2 );

SamplerState samPoint : register( s0 );

#define MAX_LIGHT_PROBES	256
#define MAX_TETRAHEDRONS	256

cbuffer SceneViewData : register( b0 )
{
	float4x4 projectionViewMatrix;
	float4x4 projectionViewInvMatrix;
	float4 screenSize;
};

struct LightProbe
{
	float3 position;
	float lightData[9];
};

struct Tetrahedron
{
	int lightProbes[4];
	int adjacentTetrahedrons[4];
	float4 sidePlane[4];
};

cbuffer LightProbeData : register( b1 )
{
	int lightProbeCount;
	LightProbe lightProbes[MAX_LIGHT_PROBES];
};

cbuffer TetrahedronData : register( b1 )
{
	int tetrahedronCount;
	Tetrahedron tetrahedrons[MAX_TETRAHEDRONS];
};

float4 Main(PositionNormalTexture shaderInput) : SV_TARGET
{
	return float4(1, 1, 1, 1);
}
