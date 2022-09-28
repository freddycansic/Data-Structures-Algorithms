#include "Matrix.h"

#include <chrono>
#include <future>

#include "glm/mat4x4.hpp"

template<typename DurationMetric = std::chrono::milliseconds, typename F, typename... Args> 
auto benchmark(F f, Args... args)
{
	const auto start = std::chrono::high_resolution_clock::now();

	std::invoke(f, args...);

	return std::chrono::duration_cast<DurationMetric>(std::chrono::high_resolution_clock::now() - start);
}

template <size_t rows, size_t columns, typename T>
void runAsyncBenchmark(const Mat<rows, columns, T>& mat, const glm::mat<columns, rows, T, glm::qualifier::defaultp>& glmMat, size_t iterations)
{
	auto gaussianTimeFuture = std::async([&mat, iterations]()
		{
			return benchmark([&mat, iterations]()
				{
					for (size_t i = 0; i < iterations; i++)
					{
						mat.gaussianInverse();
					}
				});
		});

	auto adjugateTimeFuture = std::async([&mat, iterations]()
		{
			return benchmark([&mat, iterations]()
				{
					for (size_t i = 0; i < iterations; i++)
					{
						mat.adjugateInverse();
					}
				});
		});

	auto glmTimeFuture = std::async([&glmMat, iterations]()
		{
			return benchmark([&glmMat, iterations]()
				{
					for (size_t i = 0; i < iterations; i++)
					{
						glm::inverse(glmMat);
					}
				});
		});

	std::cout << "Time taken to invert " << iterations << " matrices:" << std::endl;
	std::cout << "Gaussian: " << gaussianTimeFuture.get() << std::endl;
	std::cout << "Adjugate: " << adjugateTimeFuture.get() << std::endl;
	std::cout << "GLM (Adjugate): " << glmTimeFuture.get() << std::endl;
}

int main()
{
	const Mat<4, 4> mat = {

		{-5, 4, 1, 7},
		{-9, 3, 2, -5},
		{-2, 0, -1, 1},
		{1, 14, 0, 3}

	};
	
	const glm::mat4 glmMat(
		-5, 4, 1, 7,
		-9, 3, 2, -5,
		-2, 0, -1, 1,
		1, 14, 0, 3
	);

	constexpr size_t iterations = 1000000;

	//std::cout << mat.gaussianInverse() << std::endl;
	//std::cout << mat.adjugateInverse() << std::endl;

	//runAsyncBenchmark(mat, glmMat, iterations);

	std::cout << mat.determinant() << std::endl;
	std::cout << mat.determinantOld() << std::endl;

	std::cout << "Determinant old " << benchmark([iterations, &mat]()
	{
		for (size_t i = 0; i < iterations; ++i)
		{
			mat.determinantOld();
		}
	}) << std::endl;

	std::cout << "Determinant new " << benchmark([iterations, &mat]()
		{
			for (size_t i = 0; i < iterations; ++i)
			{
				mat.determinant();
			}
		}) << std::endl;

	std::cout << "Determinant glm " << benchmark([iterations, &glmMat]()
		{
			for (size_t i = 0; i < iterations; ++i)
			{
				glm::determinant(glmMat);
			}
		}) << std::endl;

	return 0;
}