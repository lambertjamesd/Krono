
struct ColorVertex
{
	float4 position : SV_Position;
	float4 color : Color;
};

struct TexCoordVertex
{
	float4 position : SV_Position;
	float2 texCoord : TexCoord;
};

struct NormalTexCoordVertex
{
	float4 position : SV_Position;
	float3 normal : Normal;
	float2 texCoord : TexCoord;
};

struct CompositeOutput
{
	float4 color: SV_TARGET0;
	float4 normal: SV_TARGET1;
	float4 specular: SV_TARGET2;
	float4 emmisive: SV_TARGET3;
};

struct ScreenPositionVertex
{
	float4 position : SV_Position;
	float4 rasterPosition : RasterPosition;
};