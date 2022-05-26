#include <iostream>

#include "DynamicArray.h"

int main() {

	DynamicArray<int> dynamicArr1 = {1, 2, 3};

	std::cout << dynamicArr1 << std::endl;

	DynamicArray<int> dynamicArr2 = dynamicArr1;

	//dynamicArr1 = DynamicArray<int>{4, 5, 6, 7};

	//std::cout << dynamicArr1 << std::endl;

	std::cout << "END OF PROGRAM" << std::endl;

	return 0;
}