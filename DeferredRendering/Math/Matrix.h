#pragma once

template <size_t Rows, size_t Columns, typename T = float> 
class Matrix
{
public:
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
					value += (*this)[row][i] * right[i][col];
				}

				result[row][col] = value;
			}
		}

		return result;
	}
	
	T* operator[](size_t index)
	{
		return mElements[index];
	}
	
	const T* operator[](size_t index) const
	{
		return mElements[index];
	}

	template <size_t NewRows, size_t NewColumns>
	operator Matrix<NewRows, NewColumns, T> () const
	{
		Matrix<NewRows, NewColumns, T> result = Matrix<NewRows, NewColumns, T>::Identity();

		for (size_t row = 0; row < min(Rows, NewRows); ++row)
		{
			for (size_t col = 0; col < min(Columns, NewColumns); ++col)
			{
				result[row][col] = mElements[row][col];
			}
		}

		return result;
	}

	static Matrix Identity()
	{
		Matrix result;

		for (size_t row = 0; row < Rows; ++row)
		{
			for (size_t col = 0; col < Columns; ++col)
			{
				result.mElements[row][col] = (row == col) ? 1 : 0;
			}
		}

		return result;
	}
protected:
	T mElements[Rows][Columns];
};

typedef Matrix<4, 4, float> Matrix4f;
typedef Matrix<3, 3, float> Matrix3f;
typedef Matrix<2, 2, float> Matrix2f;