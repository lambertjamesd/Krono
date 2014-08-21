
#include "FolderTest/IncludeMe.hlsli"

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