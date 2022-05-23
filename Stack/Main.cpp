#include <iostream>

#include "Stack.h"

int main() {

	Stack<int, 5> stack1 = { 1, 2, 3, 4, 5 };

	std::cout << stack1 << std::endl;

	stack1.pop();

	std::cout << stack1 << std::endl;

	stack1.pop();

	std::cout << stack1 << std::endl;

	std::cout << stack1.peek() << std::endl;

	stack1.push(8);
	stack1.push(9);

	std::cout << stack1 << std::endl;

	return 0;
}