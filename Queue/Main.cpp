#include <iostream>

#include "Queue.h"

int main() {

	Queue<int, 5> queue1 = { 1, 2, 3 };

	std::cout << queue1 << std::endl;

	queue1.enQueue(3);

	std::cout << queue1 << std::endl;

	queue1.deQueue();
	queue1.deQueue();

	std::cout << queue1 << std::endl;

	queue1.enQueue(7);
	queue1.enQueue(8);
	queue1.enQueue(9);

	std::cout << queue1 << std::endl;

	return 0;
}