#pragma once

#include <array>
#include <initializer_list>
#include <iostream>

template<size_t rows, size_t columns, typename T = float>
class Mat
{
	using MATRIX_ROW = std::array<T, columns>;
	using MATRIX_DATA = std::array<MATRIX_ROW, rows>;

public:

	Mat(const std::initializer_list<std::initializer_list<T>>& list)
	{
		//static_assert(list.size() == (rows * columns), "Initialiser list does not contain the right amount of elements to satisfy this matrix.");
		static_assert(rows > 0 && columns > 0, "Matrix must have positive dimensions.");
		memcpy(m_Data.at(0).data(), std::data(list.begin()), list.size() * sizeof(T));
	}

	MATRIX_ROW& operator[](size_t index)
	{
		return m_Data.at(index);
	}

	const MATRIX_ROW& operator[](size_t index) const
	{
		return m_Data.at(index);
	}

	template<size_t rows, size_t columns, typename T = float>
	friend std::ostream& operator<<(std::ostream& os, const Mat<rows, columns>& matrix);

private:
	MATRIX_DATA m_Data;
};

template<size_t rows, size_t columns, typename T = float>
std::ostream& operator<<(std::ostream& os, const Mat<rows, columns>& matrix) {
	for (unsigned int col = 0; col < columns; col++) {
		os << "| ";
		for (unsigned int row = 0; row < rows; row++) {
			os << matrix[row][col] << "\t";
		}
		os << "|\n";
	}

	return os;
};