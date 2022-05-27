#include <iostream>

#include "DynamicArray.h"

int main() {

	DynamicArray<int> dynamicArr1 = {1, 2, 3};

	std::cout << dynamicArr1 << std::endl;

	std::cout << "Remove 2" << std::endl;

	//dynamicArr1.remove(2);

	std::cout << dynamicArr1 << std::endl;

	std::cout << "Number of 1s in array = " << dynamicArr1.count(7) << std::endl;
	std::cout << "Number of 7s in array = " << dynamicArr1.count(7) << std::endl;

	std::cout << "Index of value 3 = " << dynamicArr1.index(3) << std::endl;

	dynamicArr1.insert(1, 7);

	std::cout << "Insert 7 at pos 1" << std::endl;

	std::cout << dynamicArr1 << std::endl;

	std::cout << "END OF PROGRAM" << std::endl;

	return 0;
}