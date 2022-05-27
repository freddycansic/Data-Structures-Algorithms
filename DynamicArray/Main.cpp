#include <iostream>

#include "DynamicArray.h"

int main() {

	DynamicArray<int> dynamicArr1 = {1, 2, 3};

	std::cout << dynamicArr1 << std::endl;

	dynamicArr1.remove(2);

	std::cout << dynamicArr1 << std::endl;

	std::cout << "END OF PROGRAM" << std::endl;

	return 0;
}