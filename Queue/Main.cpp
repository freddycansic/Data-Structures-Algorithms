#include <iostream>

#include "Queue.h"

int main() {

	Queue<int, 5> queue1 = { 1, 2, 3 };

	std::cout << "Empty? " << queue1.isEmpty() << std::endl;

	std::cout << queue1 << std::endl;

	queue1.enQueue(3);

	std::cout << queue1 << std::endl;

	std::cout << "Front value = " << queue1.deQueue() << std::endl;

	queue1.deQueue();

	std::cout << queue1 << std::endl;

	queue1.enQueue(7);
	queue1.enQueue(8);

	std::cout << "Current size = " << queue1.currentSize() << std::endl;

	queue1.enQueue(9);

	std::cout << queue1 << std::endl;

	std::cout << "Full? " << queue1.isFull() << std::endl;

	return 0;
}