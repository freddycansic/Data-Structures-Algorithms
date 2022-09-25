#include "Matrix.h"

#include <chrono>

#include "glm/mat4x4.hpp"

int main()
{
	const Mat<4, 4> mat = {

		{-5, 4, 1, 7},
		{-9, 3, 2, -5},
		{-2, 0, -1, 1},
		{1, 14, 0, 3}

	};

	glm::mat4 glmMat(
		-5, 4, 1, 7,
		-9, 3, 2, -5,
		-2, 0, -1, 1,
		1, 14, 0, 3
	);

	auto start = std::chrono::high_resolution_clock::now();

	for (size_t i = 0; i < 99999; i++)
	{
		mat.inverse();
	}

	std::cout << (std::chrono::high_resolution_clock::now()-start).count() << std::endl;

	start = std::chrono::high_resolution_clock::now();

	for (size_t i = 0; i < 99999; i++)
	{
		glm::inverse(glmMat);
	}

	std::cout << (std::chrono::high_resolution_clock::now() - start).count() << std::endl;


	return 0;
}