
struct StandardVertexInput
{
	float3 position : POSITION0;
	float3 normal : NORMAL0;
	float2 texCoord : TEXCOORD0;
};

struct TangentVertexInput
{
	float3 position : POSITION0;
	float3 normal : NORMAL0;
	float4 tangent : TANGENT0;
	float2 texCoord : TEXCOORD0;
};

struct PSCompositeInput
{
	float4 position : SV_Position;
	float4 normalizedPosition : POSITION;
};

struct PositionNormalTexture
{
	float4 position : SV_Position;
	float4 normal : NORMAL;
	float2 texCoord : TEXCOORD;
};

struct PositionNormalTangentTexture
{
	float4 position : SV_Position;
	float4 normal : NORMAL;
	float4 tangent : TANGENT;
	float2 texCoord : TEXCOORD;
};

struct CompositeOutput
{
	float4 color: SV_TARGET0;
	float4 normal: SV_TARGET1;
	float4 specular: SV_TARGET2;
	float4 emmisive: SV_TARGET3;
};