#include <iostream>

#include "DynamicArray.h"

int main() {

	DynamicArray<int> dynamicArr = {1, 2, 3};

	while (true) {
		dynamicArr.append(8);
		//std::cout << dynamicArr << "\n";
	}

	return 0;
}