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
		for (size_t col = 0; col < Columns; ++col)
		{
			for (size_t row = 0; row < Rows; ++row)
			{
				mElements[col][row] = (row == col) ? Constant<T>::One : Constant<T>::Zero;
			}
		}
	}

	~Matrix(void)
	{

	}

	size_t RowCount() const
	{
		return Rows;
	}

	size_t ColumnCount() const
	{
		return Columns;
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
	
	Matrix<Rows, Columns, T> operator-(const Matrix<Rows, Columns, T>& right) const
	{
		Matrix<Rows, Columns, T> result;

		for (size_t row = 0; row < Rows; ++row)
		{
			for (size_t col = 0; col < Columns; ++col)
			{
				result.mElements[col][row] = mElements[col][row] - right.mElements[col][row];
			}
		}

		return result;
	}

	Matrix<Rows, Columns, T> operator+(const Matrix<Rows, Columns, T>& right) const
	{
		Matrix<Rows, Columns, T> result;

		for (size_t row = 0; row < Rows; ++row)
		{
			for (size_t col = 0; col < Columns; ++col)
			{
				result.mElements[col][row] = mElements[col][row] * right.mElements[col][row];
			}
		}

		return result;
	}

	Matrix<Rows, Columns, T> operator*(const T& right) const
	{
		Matrix<Rows, Columns, T> result;

		for (size_t row = 0; row < Rows; ++row)
		{
			for (size_t col = 0; col < Columns; ++col)
			{
				result.mElements[col][row] = mElements[col][row] * right;
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
		Matrix<NewRows, NewColumns, T> result;

		for (size_t col = 0; col < std::min(Columns, NewColumns); ++col)
		{
			for (size_t row = 0; row < std::min(Rows, NewRows); ++row)
			{
				result.At(row, col) = mElements[col][row];
			}
		}

		return result;
	}

	Matrix<Rows - 1, Columns - 1, T> SubMatrix(size_t rowSkip, size_t colSkip) const
	{
		static_assert(Rows == Columns, "Can only take determinat of a square matrix");
		static_assert(Rows > 1 && Columns > 1, "Taking sub matrix of a 1x1 doesn't make sense");

		Matrix<Rows - 1, Columns - 1, T> result;
		
		for (size_t col = 0; col < Columns - 1; ++col)
		{
			for (size_t row = 0; row < Rows - 1; ++row)
			{
				size_t inputColumn = col;
				size_t inputRow = row;

				if (inputColumn >= colSkip)
				{
					++inputColumn;
				}

				if (inputRow >= rowSkip)
				{
					++inputRow;
				}

				result.At(row, col) = mElements[inputColumn][inputRow];
			}
		}

		return result;
	}

	T Determinant() const
	{
		static_assert(Rows == Columns, "Can only take determinat of a square matrix");

		if (Rows == 2)
		{
			return mElements[0][0] * mElements[1][1] - mElements[0][1] * mElements[1][0];
		}

		T result = 0;

		for (size_t col = 0; col < Columns; ++col)
		{
			T subValue = (col & 0x1) ? -Constant<T>::One : Constant<T>::One;

			subValue *= mElements[col][0];
			subValue *= SubMatrix(0, col).Determinant();

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
				result.mElements[row][col] = SubMatrix(row, col).Determinant() * (((row ^ col) & 0x1) ? -determinantInv : determinantInv);
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
		return Matrix();
	}
protected:
	T mElements[Columns][Rows];
};

template <typename T>
class Matrix<1,1,T>
{
public:
	Matrix(const T *rowMajorData)
	{
		mElement = *rowMajorData;
	}
	
	Matrix(void)
	{
		mElement = Constant<T>::One;
	}

	T& At(size_t row, size_t column)
	{
		return mElement;
	}
	
	const T& At(size_t row, size_t column) const
	{
		return mElement;
	}

	size_t RowCount() const
	{
		return 1;
	}

	size_t ColumnCount() const
	{
		return 1;
	}

	T Determinant() const
	{
		return mElement;
	}

	Matrix Inverse() const
	{
		Matrix result;
		result = Constant<T>::One / mElement;
		return result;
	}
private:
	T mElement;
};

template <typename T = float>
class Matrix4 : public Matrix<4, 4, T>
{
public:
	Matrix4()
	{

	}

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

	Vector3<T> TransformPoint(const Vector3<T>& input) const
	{
		return (*this * Vector4<T>(input, Constant<T>::One)).XYZ();
	}

	Vector3<T> TransformDirection(const Vector3<T>& input) const
	{
		return (*this * Vector4<T>(input, Constant<T>::Zero)).XYZ();
	}

	// gets the rotation of an orthonormal matrix
	// WARNING: this will have undefined behavior for scaled or skewed matrices
	Quaternion<T> GetRotation() const
	{
		return Quaternion<T>(
			Vector3<T>(this->At(0, 0), this->At(1, 0), this->At(2, 0)),
			Vector3<T>(this->At(0, 1), this->At(1, 1), this->At(2, 1)),
			Vector3<T>(this->At(0, 2), this->At(1, 2), this->At(2, 2)));
	}

	Vector3<T> GetTranslation() const
	{
		return Vector3<T>(this->At(0, 3), this->At(1, 3), this->At(2, 3));
	}

	void Decompose(Vector3<T>& position, Quaternion<T>& rotation, Vector3<T>& scale) const
	{
		position = GetTranslation();

		Matrix<3, 3, T> rotationMatrix = (Matrix<3, 3, T>)(*this);
		int currentIteration = 0;
		T currentError;
		T faultTolerance = Constant<T>::One / DecomposeFaultRecep;

		do
		{
			Matrix<3, 3, T> nextIteration = rotationMatrix.Inverse().Transpose();
			
			currentError = Constant<T>::Zero;
			for (int col = 0; col < nextIteration.ColumnCount(); ++col)
			{
				for (int row = 0; row < nextIteration.RowCount(); ++row)
				{
					T lastValue = rotationMatrix.At(row, col);
					rotationMatrix.At(row, col) = (nextIteration.At(row, col) + rotationMatrix.At(row, col)) / 2;
					T difference = lastValue - rotationMatrix.At(row, col);
					currentError += difference * difference;
				}
			}
			++currentIteration;
		} while (currentIteration < MaxDecomposeIterations && currentError > faultTolerance * faultTolerance);

		Matrix<3, 3, T> scaleMatrix = rotationMatrix.Inverse() * (Matrix<3, 3, T>)(*this);

		rotation = Quaternion<T>(
			Vector3<T>(rotationMatrix.At(0, 0), rotationMatrix.At(1, 0), rotationMatrix.At(2, 0)),
			Vector3<T>(rotationMatrix.At(0, 1), rotationMatrix.At(1, 1), rotationMatrix.At(2, 1)),
			Vector3<T>(rotationMatrix.At(0, 2), rotationMatrix.At(1, 2), rotationMatrix.At(2, 2)));

		scale = Vector3<T>(scaleMatrix.At(0, 0), scaleMatrix.At(1, 1), scaleMatrix.At(2, 2));
	}

	static Matrix4 Scale(const Vector3<T>& vector)
	{
		Matrix4 result;
		result.At(0, 0) = vector.x;
		result.At(1, 1) = vector.y;
		result.At(2, 2) = vector.z;
		return result;
	}

	static Matrix4 Translation(const Vector3<T>& vector)
	{
		Matrix4 result;
		result.At(0, 3) = vector.x;
		result.At(1, 3) = vector.y;
		result.At(2, 3) = vector.z;
		return result;
	}

	static Matrix4 Rotation(const Quaternion<T>& quat)
	{
		Matrix4 result;

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

	static Matrix4 FullTransform(const Vector3<T>& position, const Quaternion<T>& rotation, const Vector3<T>& scale)
	{
		return Matrix4::Translation(position) * Matrix4::Rotation(rotation) * Matrix4::Scale(scale);
	}
private:

	static const int MaxDecomposeIterations = 20;
	static const int DecomposeFaultRecep = 10000;
};

typedef Matrix4<float> Matrix4f;
typedef Matrix<3, 3, float> Matrix3f;
typedef Matrix<2, 2, float> Matrix2f;

Vector4f operator*(const Matrix4f& lhs, const Vector4f& rhs);
Vector4f operator*(const Vector4f& lhs, const Matrix4f& rhs);

template <size_t Rows, size_t Columns, typename T>
Matrix<Rows, Columns, T> operator*(const T& left, const Matrix<Rows, Columns, T>& right)
{
	Matrix<Rows, Columns, T> result;

	for (size_t row = 0; row < Rows; ++row)
	{
		for (size_t col = 0; col < Columns; ++col)
		{
			result.At(row, col) = left * right.At(row, col);
		}
	}
	return result;
}

}
