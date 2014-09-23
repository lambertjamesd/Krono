
#include "DataTypes.hlsli"

PositionNormalTexture Main(StandardVertexInput vertexInput)
{
	PositionNormalTexture result;
	result.position = float4(vertexInput.position, 1.0);

#ifdef OPENGL
	result.position.y = -result.position.y;
	result.position.z = result.position.z * 2 - 1;
#endif

	result.normal = float4(vertexInput.normal, 0.0);
	result.texCoord = vertexInput.texCoord;
	return result;
}
