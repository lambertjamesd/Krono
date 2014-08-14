#pragma once

#include "Vector3.h"
#include "Vector4.h"

template <size_t Rows, size_t Columns, typename T = float> 
class Matrix
{
public:
	Matrix(T *rowMajorData)
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

		for (size_t col = 0; col < min(Columns, NewColumns); ++col)
		{
			for (size_t row = 0; row < min(Rows, NewRows); ++row)
			{
				result.mElements[col][row] = mElements[col][row];
			}
		}

		return result;
	}

	T SubDeterminant(size_t rowSkip, size_t colSkip) const
	{
		static_assert(Rows == Columns, "Can only take determinat of a square matrix");

		T result = 0;
		
		for (size_t col = 0; col < Columns - 1; ++col)
		{
			T positiveSlant = 1;
			T negativeSlant = 1;

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
			T positiveSlant = 1;
			T negativeSlant = 1;

			for (size_t i = 0; i < Columns; ++i)
			{
				positiveSlant *= mElements[(col + i) % Columns][i];
				negativeSlant *= mElements[(col + i) % Columns][(Columns - 1) - i];
			}

			result += positiveSlant - negativeSlant;
		}

		return result;
	};

	Matrix Inverse() const
	{
		static_assert(Rows == Columns, "Can only take inverse of a square matrix");

		Matrix result;

		T determinantInv = 1 / Determinant();
		
		for (size_t col = 0; col < Columns; ++col)
		{
			for (size_t row = 0; row < Rows; ++row)
			{
				result.mElements[col][row] = SubDeterminant(col, row) * (((row ^ col) & 0x1) ? -determinantInv : determinantInv);
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
				result.mElements[col][row] = (row == col) ? (T)1 : (T)0;
			}
		}

		return result;
	}
protected:
	T mElements[Columns][Rows];
};

typedef Matrix<4, 4, float> Matrix4f;
typedef Matrix<3, 3, float> Matrix3f;
typedef Matrix<2, 2, float> Matrix2f;

Matrix4f ScaleMatrix(const Vector3f& vector);
Matrix4f TranslationMatrix(const Vector3f& vector);

Vector4f operator*(const Matrix4f& lhs, const Vector4f& rhs);
Vector4f operator*(const Vector4f& lhs, const Matrix4f& rhs);