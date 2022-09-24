#include "Matrix.h"

int main()
{
	Mat<3, 3> mat32 = { {1, -2, 3},
						{2, 1, 1},
						{-3, 2, -2}
	};

	std::cout << mat32.augment<3>(Mat<3, 3>::identity()) << std::endl;

	std::cout << mat32.divideRow(2, 3) << std::endl;

	std::cout << mat32.inverse() << std::endl;

	return 0;
}