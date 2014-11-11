#include "MeshMath.h"

#include "Math/Matrix.h"

namespace krono
{

void MeshMath::CalculateTangent(const Vector3f* positions, const Vector2f* texCoords, Vector3f& tangent, Vector3f& binormal)
{
	Vector3f rightPosition = positions[1] - positions[0];
	Vector3f upPosition = positions[2] - positions[0];
	upPosition -= upPosition.ProjectOnto(rightPosition);

	rightPosition.Normalize();
	upPosition.Normalize();

	Vector2f rightTex = texCoords[1] - texCoords[0];
	Vector2f upTex = texCoords[2] - texCoords[0];
	upTex -= upTex.ProjectOnto(rightTex);

	rightTex.Normalize();
	upTex.Normalize();

	tangent = rightPosition * rightTex.x + upPosition * upTex.x;
	binormal = rightPosition * rightTex.y + upPosition * upTex.y;
}

BoundingBoxf MeshMath::CalculateBoundingBox(const AttributeArray<Vector3f>& positions, size_t vertexCount)
{
	BoundingBoxf result(positions[0], positions[0]);

	for (size_t i = 1; i < vertexCount; ++i)
	{
		result.minimum = Min(result.minimum, positions[i]);
		result.maximum = Max(result.maximum, positions[i]);
	}

	return result;
}

void MeshMath::NormalizeTangents(AttributeArray<Vector4f>& tangents, const AttributeArray<Vector3f>& normals, size_t vertexCount)
{
	for (size_t vertexIndex = 0; vertexIndex < vertexCount; ++vertexIndex)
	{
		Vector3f tangent = tangents[vertexIndex].XYZ();
		tangent -= tangent.ProjectOnto(normals[vertexIndex]);
		tangents[vertexIndex] = Vector4f(tangent.Normalized(), Math<float>::Sign(tangents[vertexIndex].w));
	}
}

}