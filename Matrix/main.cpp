#include "Matrix.h"

int main()
{
	const Mat<4, 4> mat = {

		{-5, 4, 1, 7},
		{-9, 3, 2, -5},
		{-2, 0, -1, 1},
		{1, 14, 0, 3}

	};

	std::cout << mat.determinant() << std::endl;
	std::cout << mat.inverse() << std::endl;

	return 0;
}