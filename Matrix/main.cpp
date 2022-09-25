#include "Matrix.h"

int main()
{
	Mat<4, 4> mat = {
		{1, 2, 3, 4},
		{5, 6, 7, 8},
		{9, 10, 11, 12},
		{13, 14, 15, 16}
	};

	std::cout << mat.determinant() << std::endl;



	return 0;
}