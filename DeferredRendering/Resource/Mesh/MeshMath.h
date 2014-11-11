#pragma once

#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Collide/BoundingBox.h"

#include "Core/Types.h"

namespace krono
{

template <typename T>
class AttributeArray
{
public:
	AttributeArray(void* dataPointer, size_t offset, size_t byteStride) :
		mDataPointer((T*)((char*)dataPointer + offset)),
		mByteStride(byteStride)
	{

	}

	AttributeArray(T* dataPointer, size_t byteStride) :
		mDataPointer(dataPointer),
		mByteStride(byteStride)
	{

	}

	const T& operator[](size_t index) const
	{
		return *(const T*)((const char*)mDataPointer + mByteStride * index);
	}
	
	T& operator[](size_t index)
	{
		return *(T*)((char*)mDataPointer + mByteStride * index);
	}
private:
	T* mDataPointer;
	size_t mByteStride;
};

class MeshMath
{
public:
	// position and texCoords should each have a length of 3
	static void CalculateTangent(const Vector3f* positions, const Vector2f* texCoords, Vector3f& tangent, Vector3f& binormal);

	static BoundingBoxf CalculateBoundingBox(const AttributeArray<Vector3f>& positions, size_t vertexCount);

	template <typename T>
	static void AppendTangents(AttributeArray<Vector4f>& tangents, const AttributeArray<Vector3f>& positions, const AttributeArray<Vector2f>& texCoords, const AttributeArray<Vector3f>& normals, const T* indexArray, size_t indexCount)
	{
		for (size_t index = 0; index + 2 < indexCount; index += 3)
		{
			Vector3f trianglePositions[3];
			Vector2f triangleTexCoords[3];
			Vector3f normal;


			for (size_t i = 0; i < 3; ++i)
			{
				trianglePositions[i] = positions[indexArray[index + i]];
				triangleTexCoords[i] = texCoords[indexArray[index + i]];
				normal += normals[indexArray[index + i]];
			}

			Vector3f tangent;
			Vector3f binormal;

			CalculateTangent(trianglePositions, triangleTexCoords, tangent, binormal);

			Vector4f combinedTangent = Vector4f(tangent, binormal.Dot(normal.Cross(tangent)) > 0.0f ? 1.0f : -1.0f);

			for (size_t i = 0; i < 3; ++i)
			{
				tangents[indexArray[index + i]] += combinedTangent;
			}
		}
	}

	static void NormalizeTangents(AttributeArray<Vector4f>& tangents, const AttributeArray<Vector3f>& normals, size_t vertexCount);
private:
};

}
