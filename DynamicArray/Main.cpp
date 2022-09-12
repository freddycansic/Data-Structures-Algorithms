#include <iostream>

#include "DynamicArray.h"

int main()
{
	DynamicArray<int> arr = {1, 2, 3, 4, 5};
	std::cout << arr << std::endl;

	arr.append(6);
	std::cout << arr << std::endl;

	arr.remove(6);
	std::cout << arr << std::endl;

	arr.insert(3, 99);
	std::cout << arr << std::endl;

	std::cout << arr.pop() << std::endl;
	std::cout << arr << std::endl;

	std::cout << arr.pop(1) << std::endl;
	std::cout << arr << std::endl;

	arr.reserve(10);
	std::cout << "Array allocated 10 * sizeof(int) = 40 bytes" << std::endl;

	std::cout << arr.count(1) << std::endl;

	std::cout << arr.len() << std::endl;

	std::cout << arr.index(1) << std::endl;

	std::cout << arr.at(2) << std::endl;
	std::cout << arr[2] << std::endl;

	arr.clear();
	std::cout << arr << std::endl;

	std::cout << (arr.isEmpty() ? "True" : "False") << std::endl;

	return 0;
}