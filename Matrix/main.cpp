#include "Matrix.h"

int main()
{
	//Mat<1, 2> mat12 = { 1, 2};

	Mat<3, 2> mat32 = { {1, 2},
						{3, 4},
						{5, 6}
	};

	std::cout << mat32 << std::endl;

	return 0;
}