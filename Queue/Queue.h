#pragma once

#include <initializer_list>
#include <stdexcept>
#include <iostream>
#include <array>
#include <algorithm>

template <typename T, size_t size>
class Queue
{
private:
	const size_t MAX_SIZE = size;

	std::array<T, size> m_Data;
	size_t m_Front, m_Rear, m_CurrentSize;

public:

	Queue<T, size>(const std::initializer_list<T>& elements);

	void enQueue(const T& item);
	T deQueue();

	[[nodiscard]] constexpr size_t maxSize() const;
	[[nodiscard]] size_t currentSize() const;
	[[nodiscard]] bool isEmpty() const;
	[[nodiscard]] bool isFull() const;

	template<typename T, size_t size>
	friend std::ostream& operator<<(std::ostream& os, const Queue<T, size>& queue);
};

template <typename T, size_t size>
Queue<T, size>::Queue(const std::initializer_list<T>& elements) :
	m_Data{0}, m_Front(0), m_Rear(elements.size() - 1), m_CurrentSize(elements.size())
{
	if (elements.size() > MAX_SIZE)
		throw std::runtime_error("Too many elements given for maximum queue size.");

	std::copy(elements.begin(), elements.end(), &m_Data[0]);
}

template<typename T, size_t size>
void Queue<T, size>::enQueue(const T& item)
{
	if (isFull())
		throw std::range_error("Cannot add item to a full queue.");

	m_Rear = (m_Rear + 1) % MAX_SIZE;
	m_Data[m_Rear] = item;

	++m_CurrentSize;
}

template<typename T, size_t size>
T Queue<T, size>::deQueue()
{
	if (isEmpty())
		throw std::range_error("Cannot remove item from empty queue.");

	const T& value = m_Data[m_Front];

	m_Front = (m_Front + 1) % MAX_SIZE;
	--m_CurrentSize;

	return value;
}

template<typename T, size_t size>
constexpr size_t Queue<T, size>::maxSize() const
{
	return MAX_SIZE;
}

template<typename T, size_t size>
size_t Queue<T, size>::currentSize() const
{
	return m_CurrentSize;
}

template<typename T, size_t size>
bool Queue<T, size>::isEmpty() const
{
	return m_CurrentSize == 0;
}

template<typename T, size_t size>
bool Queue<T, size>::isFull() const
{
	return m_CurrentSize == MAX_SIZE;
}

template<typename T, size_t size>
std::ostream& operator<<(std::ostream& os, const Queue<T, size>& queue) {
	os << "Queue = [";

	if (queue.m_Front == queue.m_Rear) {

		for (size_t i = 0; i < queue.m_Front; ++i) {
			os << "0, ";
		}

		os << queue.m_Data[queue.m_Front] << ", ";

		for (size_t i = queue.m_Rear; i < queue.maxSize() - 1; ++i) {
			os << "0, ";
		}

		os << "0]";

	} else if (queue.m_Front < queue.m_Rear) {

		for (size_t i = 0; i < queue.m_Front; ++i) {
			os << "0, ";
		}

		for (size_t i = queue.m_Front; i < queue.m_Rear + 1; ++i) {
			os << queue.m_Data[i] << ", ";
		}

		for (size_t i = queue.m_Rear + 1; i < queue.maxSize() - 1; ++i) {
			os << "0, ";
		}

		os << "0]";

	} else {

		for (size_t i = 0; i < queue.m_Rear + 1; ++i) {
			os << queue.m_Data[i] << ", ";
		}

		for (size_t i = queue.m_Rear + 1; i < queue.m_Front; ++i) {
			os << "0, ";
		}

		for (size_t i = queue.m_Front; i < queue.maxSize() - 1; ++i) {
			os << queue.m_Data[i] << ", ";
		}

		os << queue.m_Data[queue.maxSize() - 1] << "]";

	}

	os << " Front = " << queue.m_Front << " Rear = " << queue.m_Rear;

	return os;
}