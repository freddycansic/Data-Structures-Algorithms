#pragma once

#include <array>
#include <initializer_list>
#include <iostream>
#include <functional>
#include <cassert>

template<size_t rows, size_t columns, typename T = float> requires std::is_arithmetic_v<T>
class Mat
{
	static_assert(rows > 0 && columns > 0, "Matrix must have positive dimensions.");

	using MATRIX_ROW = std::array<T, columns>;
	using MATRIX_DATA = std::array<MATRIX_ROW, rows>;

public: // idk why this has to be public
	template<size_t newRows, size_t newCols>
	[[nodiscard]] Mat<newRows, newCols> setMatSize() const
	{
		Mat<newRows, newCols> newMat;

		for (size_t row = 0; row < (newRows < rows ? newRows : rows); ++row)
		{
			for (size_t col = 0; col < (newCols < columns ? newCols : columns); ++col)
			{
				newMat[row][col] = m_Data[row][col];
			}
		}

		return newMat;
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

	[[nodiscard]] Mat<rows, columns> addRowToRow(size_t rowToAddTo, T multiplier, size_t rowToAddFrom) const
	{
		auto result = *this;

		for (size_t col = 0; col < columns; ++col)
		{
			result[rowToAddTo][col] += multiplier * result[rowToAddFrom][col];
		}

		return result;
	}

	template<size_t newColumns>
	[[nodiscard]] Mat<rows, columns + newColumns> augment(const Mat<rows, newColumns>& newMat) const
	{
		static_assert(newColumns > 0, "Number of new columns must be greater than 0!");

		// grow matrix while maintaining data
		auto augmentedMat = this->setMatSize<rows, columns + newColumns>();

		// append new matrix
		for (size_t row = 0; row < rows; ++row)
		{
			std::copy(newMat[row].begin(), newMat[row].end(), &augmentedMat[row][columns]);
		}

		return augmentedMat;
	}

	template<size_t newColumns>
	[[nodiscard]] Mat<rows, columns + newColumns> augment() const
	{
		return augment(Mat<rows, newColumns>());
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

	Mat(const Mat& other)
	{
		for (size_t row = 0; row < rows; ++row)
		{
			std::copy(other[row].begin(), other[row].end(), m_Data[row].begin());
		}
	}

	Mat(Mat&& other) noexcept
		: m_Data(std::move(other.m_Data))
	{
	}

	Mat& operator=(Mat&& other) noexcept
	{
		m_Data = std::move(other.m_Data);

		return *this;
	}

	~Mat() = default;

	[[nodiscard]] Mat<rows - 1, columns - 1> minor(size_t excludeRowIndex, size_t excludeColumnIndex) const
	{
		Mat<rows - 1, columns - 1> minor;

		size_t minorRow = 0, minorCol = 0;
		for (size_t majorRow = 0; majorRow < rows; ++majorRow)
		{
			if (majorRow == excludeRowIndex) continue;

			for (size_t majorCol = 0; majorCol < columns; ++majorCol)
			{
				if (majorCol == excludeColumnIndex) continue;

				minor[minorRow][minorCol++] = m_Data[majorRow][majorCol];

				if (minorCol == columns - 1) // TODO is there a better way to do this?
				{
					minorCol = 0;
					++minorRow;
				}
			
			}
		}

		return minor;
	}

	[[nodiscard]] T determinant() const
	{
		static_assert(rows == columns, "Can only compute determinant of square matrix!");

		if constexpr (rows == 1) return m_Data[0][0];
		if constexpr (rows == 2) return m_Data[0][0] * m_Data[1][1] - m_Data[1][0] * m_Data[0][1];

		T result = static_cast<T>(0);

		// for each column make a minor
		for (size_t col = 0; col < columns; ++col)
		{
			if constexpr (rows > 1) // TODO should compile without this because of the guard clauses at the start of func i think
			{
				// generate minor
				auto minor = this->minor(0, col);

				result += (col % 2 == 0 ? 1 : -1) * m_Data[0][col] * minor.determinant();
			}
		}

		return result;
	}

	[[nodiscard]] Mat<rows, columns> inverse() const
	{
		static_assert(rows == columns, "Can only compute inverse of square matrix!");

		assert(this->determinant() != static_cast<T>(0));

		// augment matrix with identity
		auto result = this->template augment<columns>(Mat<rows, columns>::identity());

		// iterate over every col
		for (size_t col = 0; col < columns; ++col)
		{
			// make diagonal = 1
			if (result[col][col] != static_cast<T>(1))
			{
				result = result.divideRow(col, result[col][col]);
			}

			// go to row under diagonal
			for (size_t row = col+1; row < rows; ++row)
			{
				const auto& element = result[row][col];

				// make it a 0
				if (element != static_cast<T>(0))
				{
					result = result.addRowToRow(row, -element, col);
				}
			}
		}
		
		// iteratr over every col
		for (size_t col = 1; col < columns; ++col)
		{
			// start at diagonal first
			// go to row above
			for (size_t row = 0; row < col; ++row)
			{
				const auto& element = result[row][col];

				// make it a 0
				if (element != static_cast<T>(0))
				{
					result = result.addRowToRow(row, -element, col);
				}
			}
		}

		// return augmented section
		Mat<rows, columns> end;

		for (size_t row = 0; row < rows; ++row)
		{
			for (size_t col = 0, augmentedCol = columns; col < columns; ++col, ++augmentedCol)
			{
				end[row][col] = result[row][augmentedCol];
			}
		}

		return end;
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
	for (size_t row = 0; row < rows; row++) {
		os << "| ";
		for (size_t col = 0; col < columns; col++) {
			os << matrix[row][col] << "\t";
		}
		os << "|\n";
	}

	return os;
};