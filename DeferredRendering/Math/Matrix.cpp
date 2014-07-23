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

Vector4f operator*(const Matrix4f& lhs, const Vector4f& rhs)
{
	return Vector4f(
		lhs.At(0, 0) * rhs.x + lhs.At(0, 1) * rhs.y + lhs.At(0, 2) * rhs.z + lhs.At(0, 3) * rhs.w,
		lhs.At(1, 0) * rhs.x + lhs.At(1, 1) * rhs.y + lhs.At(1, 2) * rhs.z + lhs.At(1, 3) * rhs.w,
		lhs.At(2, 0) * rhs.x + lhs.At(2, 1) * rhs.y + lhs.At(2, 2) * rhs.z + lhs.At(2, 3) * rhs.w,
		lhs.At(3, 0) * rhs.x + lhs.At(3, 1) * rhs.y + lhs.At(3, 2) * rhs.z + lhs.At(3, 3) * rhs.w
		);
}

Vector4f operator*(const Vector4f& lhs, const Matrix4f& rhs)
{
	return Vector4f(
		lhs.x * rhs.At(0, 0) + lhs.y * rhs.At(1, 0) + lhs.z * rhs.At(2, 0) + lhs.w * rhs.At(3, 0),
		lhs.x * rhs.At(0, 1) + lhs.y * rhs.At(1, 1) + lhs.z * rhs.At(2, 1) + lhs.w * rhs.At(3, 1),
		lhs.x * rhs.At(0, 2) + lhs.y * rhs.At(1, 2) + lhs.z * rhs.At(2, 2) + lhs.w * rhs.At(3, 2),
		lhs.x * rhs.At(0, 3) + lhs.y * rhs.At(1, 3) + lhs.z * rhs.At(2, 3) + lhs.w * rhs.At(3, 3)
		);
}