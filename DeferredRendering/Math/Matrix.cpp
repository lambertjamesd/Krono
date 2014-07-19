#include "stdafx.h"

#include "Matrix.h"

Matrix4f ScaleMatrix(const Vector3f& vector)
{
	Matrix4f result = Matrix4f::Identity();
	result.At(0, 0) = vector.x;
	result.At(1, 1) = vector.y;
	result.At(2, 2) = vector.z;
	return result;
}

Matrix4f TranslationMatrix(const Vector3f& vector)
{
	Matrix4f result = Matrix4f::Identity();
	result.At(0, 3) = vector.x;
	result.At(1, 3) = vector.y;
	result.At(2, 3) = vector.z;
	return result;
}