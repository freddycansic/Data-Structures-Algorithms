#pragma once

#include <array>
#include <initializer_list>
#include <iostream>
#include <functional>

template<size_t rows, size_t columns, typename T = float> requires std::is_arithmetic_v<T>
class Mat
{
	static_assert(rows > 0 && columns > 0, "Matrix must have positive dimensions.");

	using MATRIX_ROW = std::array<T, columns>;
	using MATRIX_DATA = std::array<MATRIX_ROW, rows>;
public:
	template<size_t newRows, size_t newCols>
	[[nodiscard]] Mat<newRows, newCols> setMatSize() const
	{
		static_assert(newRows > 0 && newCols > 0, "Matrix dimensions must be positive!");

		Mat<newRows, newCols> newMat;
		
		for (size_t row = 0; row < ro; ++row)
		{
			for (size_t col = 0; col < (newCols < columns) ? columns : newCols; ++col)
			{
				newMat[row][col] = m_Data[row][col];
			}
		}

		return newMat;
	}

public:
	Mat()
	{
		for (size_t row = 0; row < rows; ++row)
		{
			memset(&m_Data[row], 0, columns * sizeof(T));
		}
	}

	Mat(std::initializer_list<std::initializer_list<T>> list)
	{
		size_t rowCount = 0;
		for (const auto& row : list)
		{
			size_t colCount = 0;
			for (const auto& col : row)
			{
				m_Data[rowCount][colCount] = col;
				++colCount;
			}
			++rowCount;
		}
	}

	Mat& operator=(const Mat<rows, columns>& other)
	{
		for (size_t row = 0; row < rows; ++row)
		{
			std::copy(other[row].begin(), other[row].end(), m_Data[row].begin());
		}

		return *this;
	}

	[[nodiscard]] Mat<rows, columns> inverse() const
	{
		static_assert(rows == columns, "Can only compute inverse of square matrix!");

		auto result = this->template augment<columns>(Mat<rows, columns>::identity());

		for (size_t col = 0; col < columns; ++col)
		{
			for (size_t row = 0; row < rows; ++row)
			{
				if (col == row)
				{
					if (m_Data[row][col] == static_cast<T>(1)) continue;

					result = result.divideRow(row, 1 / m_Data[row][col]);
				}
			}
		}

		return result.template setMatSize<rows, columns>();
	}

	template<size_t newColumns, size_t newColumnsSize = columns + newColumns>
	[[nodiscard]] Mat<rows, newColumnsSize> augment(const Mat<rows, newColumns>& newMat) const
	{
		static_assert(newColumns > 0, "Number of new columns must be greater than 0!");

		// grow matrix while maintaining data
		auto augmentedMat = this->setMatSize<rows, newColumnsSize>(); 

		// append new matrix
		for (size_t row = 0; row < rows; ++row)
		{
			std::copy(newMat[row].begin(), newMat[row].end(), &augmentedMat[row][columns]);
		}

		return augmentedMat;
	}

	template<size_t newColumns, size_t newColumnsSize = columns + newColumns>
	[[nodiscard]] Mat<rows, newColumnsSize> augment() const
	{
		return augment(Mat<rows, newColumns>());
	}
	
	[[nodiscard]] Mat<rows, columns> divideRow(size_t rowToDivide, T divisor) const
	{
		auto result = *this;

		for (auto& el : result[rowToDivide])
		{
			el /= divisor;
		}

		return result;
	}

	template<size_t rowToDivide>
	[[nodiscard]] Mat<rows, columns> addRow(T divisor) const
	{
		//return this->template applyToRow<rowToDivide - 1>(std::divides(), divisor);
		return Mat<rows, columns>();
	}

	[[nodiscard]] static Mat identity()
	{
		static_assert(columns == rows, "Can only compute identity of square matrix!");

		Mat<columns, rows> result;
		for (size_t i = 0; i < rows; ++i)
		{
			result[i][i] = 1;
		}

		return result;
	}

	MATRIX_ROW& operator[](size_t index)
	{
		return m_Data.at(index);
	}

	const MATRIX_ROW& operator[](size_t index) const
	{
		return m_Data.at(index);
	}

	template<size_t rows, size_t columns, typename T>
	friend std::ostream& operator<<(std::ostream& os, const Mat<rows, columns>& matrix);

private:
	MATRIX_DATA m_Data;
};

template<size_t rows, size_t columns, typename T = float>
std::ostream& operator<<(std::ostream& os, const Mat<rows, columns>& matrix) {
	for (unsigned int row = 0; row < rows; row++) {
		os << "| ";
		for (unsigned int col = 0; col < columns; col++) {
			os << matrix[row][col] << "\t";
		}
		os << "|\n";
	}

	return os;
};