#pragma once

#include "Vector3.h"
#include "Vector4.h"
#include "Math.h"
#include "Quaternion.h"
#include <cstring>
#include <algorithm>

namespace krono
{

template <size_t Rows, size_t Columns, typename T = float> 
class Matrix
{
public:
	Matrix(const T *rowMajorData)
	{
		memcpy(mElements, rowMajorData, sizeof(T) * Rows * Columns);
	}
	
	Matrix(void)
	{

	}

	~Matrix(void)
	{

	}
	
	template <size_t ResultColumns>
	Matrix<Rows, ResultColumns, T> operator*(const Matrix<Columns, ResultColumns, T>& right) const
	{
		Matrix<Rows, ResultColumns, T> result;

		for (size_t row = 0; row < Rows; ++row)
		{
			for (size_t col = 0; col < ResultColumns; ++col)
			{
				T value = T();

				for (size_t i = 0; i < Columns; ++i)
				{
					value += mElements[i][row] * right.mElements[col][i];
				}

				result.mElements[col][row] = value;
			}
		}

		return result;
	}

	T& At(size_t row, size_t column)
	{
		return mElements[column][row];
	}
	
	const T& At(size_t row, size_t column) const
	{
		return mElements[column][row];
	}

	template <size_t NewRows, size_t NewColumns>
	operator Matrix<NewRows, NewColumns, T> () const
	{
		Matrix<NewRows, NewColumns, T> result = Matrix<NewRows, NewColumns, T>::Identity();

		for (size_t col = 0; col < std::min(Columns, NewColumns); ++col)
		{
			for (size_t row = 0; row < std::min(Rows, NewRows); ++row)
			{
				result.At(row, col) = mElements[col][row];
			}
		}

		return result;
	}

	T SubDeterminant(size_t rowSkip, size_t colSkip) const
	{
		static_assert(Rows == Columns, "Can only take determinat of a square matrix");

		if (Rows == 2)
		{
			return mElements[1 - colSkip][1 - rowSkip];
		}

		T result = Constant<T>::Zero;
		
		for (size_t col = 0; col < Columns - 1; ++col)
		{
			T positiveSlant = Constant<T>::One;
			T negativeSlant = Constant<T>::One;

			for (size_t i = 0; i < Columns - 1; ++i)
			{
				size_t actualColumn = (col + i) % (Columns - 1);
				size_t actualRow = i;
				size_t negativeRow = Columns - 2 - i;

				if (actualColumn >= colSkip)
				{
					++actualColumn;
				}

				if (actualRow >= rowSkip)
				{
					++actualRow;
				}

				if (negativeRow >= rowSkip)
				{
					++negativeRow;
				}

				positiveSlant *= mElements[actualColumn][actualRow];
				negativeSlant *= mElements[actualColumn][negativeRow];
			}

			result += positiveSlant - negativeSlant;
		}

		return result;
	}

	T Determinant() const
	{
		static_assert(Rows == Columns, "Can only take determinat of a square matrix");

		T result = 0;

		for (size_t col = 0; col < Columns; ++col)
		{
			T subValue = (col & 0x1) ? -Constant<T>::One : Constant<T>::One;

			subValue *= mElements[col][0];
			subValue *= SubDeterminant(0, col);

			result += subValue;
		}

		return result;
	};

	Matrix Inverse() const
	{
		static_assert(Rows == Columns, "Can only take inverse of a square matrix");

		Matrix result;

		T determinantInv = Constant<T>::One / Determinant();
		
		for (size_t col = 0; col < Columns; ++col)
		{
			for (size_t row = 0; row < Rows; ++row)
			{
				result.mElements[row][col] = SubDeterminant(row, col) * (((row ^ col) & 0x1) ? -determinantInv : determinantInv);
			}
		}

		return result;
	}

	Matrix<Columns, Rows, T> Transpose() const
	{
		Matrix<Columns, Rows, T> result;

		for (size_t col = 0; col < Columns; ++col)
		{
			for (size_t row = 0; row < Rows; ++row)
			{
				result.mElements[row][col] = mElements[col][row];
			}
		}

		return result;
	}

	static Matrix Identity()
	{
		Matrix result;
		
		for (size_t col = 0; col < Columns; ++col)
		{
			for (size_t row = 0; row < Rows; ++row)
			{
				result.mElements[col][row] = (row == col) ? Constant<T>::One : Constant<T>::Zero;
			}
		}

		return result;
	}
protected:
	T mElements[Columns][Rows];
};

template <typename T = float>
class Matrix4 : public Matrix<4, 4, T>
{
public:
	Matrix4(const Matrix<4, 4, T>& other) :
		Matrix<4, 4, T>(&other.At(0, 0))
	{

	}

	Matrix4& operator=(const Matrix<4, 4, T>& other)
	{
		memcpy(this, &other, sizeof(Matrix4));
		return *this;
	}
	
	Matrix4& operator=(const Matrix4<T>& other)
	{
		memcpy(this, &other, sizeof(Matrix4));
		return *this;
	}

	static Matrix4 Scale(const Vector3<T>& vector)
	{
		Matrix4 result = Matrix4::Identity();
		result.At(0, 0) = vector.x;
		result.At(1, 1) = vector.y;
		result.At(2, 2) = vector.z;
		return result;
	}

	static Matrix4 Translation(const Vector3<T>& vector)
	{
		Matrix4 result = Matrix4::Identity();
		result.At(0, 3) = vector.x;
		result.At(1, 3) = vector.y;
		result.At(2, 3) = vector.z;
		return result;
	}

	static Matrix4 Rotation(const Quaternion<T>& quat)
	{
		Matrix4 result = Matrix4::Identity();

		result.At(0, 0) = 1 - 2 * (quat.y * quat.y + quat.z * quat.z);
		result.At(0, 1) = 2 * (quat.x * quat.y - quat.z * quat.w);
		result.At(0, 2) = 2 * (quat.x * quat.z + quat.y * quat.w);
		
		result.At(1, 0) = 2 * (quat.x * quat.y + quat.z * quat.w);
		result.At(1, 1) = 1 - 2 * (quat.x * quat.x + quat.z * quat.z);
		result.At(1, 2) = 2 * (quat.y * quat.z - quat.x * quat.w);

		result.At(2, 0) = 2 * (quat.x * quat.z - quat.y * quat.w);
		result.At(2, 1) = 2 * (quat.y * quat.z + quat.x * quat.w);
		result.At(2, 2) = 1 - 2 * (quat.x * quat.x + quat.y * quat.y);

		return result;
	}
private:
};

typedef Matrix4<float> Matrix4f;
typedef Matrix<3, 3, float> Matrix3f;
typedef Matrix<2, 2, float> Matrix2f;

Vector4f operator*(const Matrix4f& lhs, const Vector4f& rhs);
Vector4f operator*(const Vector4f& lhs, const Matrix4f& rhs);

}
