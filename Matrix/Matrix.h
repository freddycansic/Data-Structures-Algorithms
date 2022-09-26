#pragma once

#include <array>
#include <initializer_list>
#include <iostream>
#include <functional>
#include <cassert>
#include <xmmintrin.h>
#include <intrin.h>

template<size_t rows, size_t columns, typename T = float> requires std::is_arithmetic_v<T>
class Mat
{
	static_assert(rows > 0 && columns > 0, "Matrix must have positive dimensions.");

	using MATRIX_ROW = std::array<T, columns>;
	using MATRIX_DATA = std::array<MATRIX_ROW, rows>;
	
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

	Mat& operator=(const Mat& other)
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

	template<size_t otherRows, size_t otherCols>
	Mat<rows, otherCols> operator*(const Mat<otherRows, otherCols>& other) const
	{
		static_assert(columns == otherRows, "Cannot multiply matrices.");

		Mat<rows, otherCols> result;

		if constexpr (rows <= 4)
		{
			//const auto r0 = _mm_load_ps(reinterpret_cast<const float*>(&m_Data[0]));
			//const auto r1 = _mm_load_ps(reinterpret_cast<const float*>(&m_Data[1]));

			//const auto res = _mm_add_ps(r0, r1);

			//T result[4];

			//_mm_store_ps(reinterpret_cast<float*>(result), res);

		}

		for (size_t row = 0; row < rows; ++row) {
			// for every column in matrix B
			for (size_t otherCol = 0; otherCol < otherCols; ++otherCol) {
				// for every piece of data in the column and row
				for (size_t element = 0; element < rows; ++element) {
					// sum of them
					result[row][otherCol] += m_Data[row][element] * other[element][otherCol];
				}

			}
		}

		return result;
	}

	Mat operator*(T multilpier) const
	{
		Mat result;

		for (size_t row = 0; row < rows; ++row)
		{
			for (size_t col = 0; col < columns; ++col)
			{
				result[row][col] = multilpier * m_Data[row][col];
			}
		}

		return result;
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

	[[nodiscard]] Mat<rows, columns> gaussianInverse() const
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
			//std::copy(result[row].end() - columns, result[row].end(), end[row].begin());
		}

		return end;
	}

	[[nodiscard]] Mat adjugateInverse() const
	{
		return this->adjugate() * (1 / this->determinant());
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

	[[nodiscard]] Mat<rows - 1, columns - 1> minor(size_t elementRow, size_t elementColumn) const
	{
		static_assert(rows > 1 && columns > 1, "Can only compute minor of a matrix with order > 1!");
		static_assert(rows == columns, "Can only compute minor of a square matrix!");

		Mat<rows - 1, columns - 1> minor;

		size_t minorRow = 0, minorCol = 0;
		for (size_t majorRow = 0; majorRow < rows; ++majorRow)
		{
			if (majorRow == elementRow) continue;

			for (size_t majorCol = 0; majorCol < columns; ++majorCol)
			{
				if (majorCol == elementColumn) continue;

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

	[[nodiscard]] T cofactor(size_t elementRow, size_t elementColumn) const
	{
		// expecting arguments with indexes starting at 0
		auto minor = this->minor(elementRow, elementColumn);
		return static_cast<T>(std::pow(-1, elementRow + elementColumn)) * minor.determinant();
	}

	[[nodiscard]] Mat cofactorMatrix() const
	{
		static_assert(rows == columns, "Can only compute cofactor matrix from square matrix!");

		Mat result;

		for (size_t row = 0; row < rows; ++row)
		{
			for (size_t col = 0; col < columns; ++col)
			{
				result[row][col] = this->cofactor(row, col);
			}
		}

		return result;
	}

	[[nodiscard]] Mat transpose() const
	{
		Mat<columns, rows> result;

		for (size_t row = 0; row < rows; ++row)
		{
			for (size_t col = 0; col < columns; ++col)
			{
				result[col][row] = m_Data[row][col];
			}
		}

		return result;
	}

	[[nodiscard]] Mat adjugate() const
	{
		return this->cofactorMatrix().transpose();
	}

	[[nodiscard]] Mat adjoint() const
	{
		return this->adjugate();
	}

	// TODO make all this stuff private
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

	[[nodiscard]] Mat<rows, columns> divideRow(size_t rowToDivide, T divisor) const
	{
		auto result = *this;

		for (auto& el : result[rowToDivide])
		{
			el /= divisor;
		}

		return result;
	}

	[[nodiscard]] Mat<rows, columns> addRowToRow(size_t rowToAddToIndex, T multiplier, size_t rowToAddFromIndex) const
	{
		auto result = *this;

		if constexpr (columns <= 8)
		{
			const auto rowToAddFrom = _mm256_load_ps(reinterpret_cast<const float*>(&m_Data[rowToAddFromIndex]));
			const auto rowToAddTo = _mm256_load_ps(reinterpret_cast<const float*>(&m_Data[rowToAddToIndex]));

			std::array<T, columns> multipliersArray;
			multipliersArray.fill(multiplier);

			const auto multipliers = _mm256_load_ps(reinterpret_cast<const float*>(multipliersArray.data()));

			const auto multipliedRowToAddFrom = _mm256_mul_ps(rowToAddFrom, multipliers);

			const auto resultingRow = _mm256_add_ps(multipliedRowToAddFrom, rowToAddTo);

			_mm256_store_ps(reinterpret_cast<float*>(&result[rowToAddToIndex]), resultingRow);

			return result;
		}

		for (size_t col = 0; col < columns; ++col)
		{
			result[rowToAddToIndex][col] += multiplier * result[rowToAddFromIndex][col];
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