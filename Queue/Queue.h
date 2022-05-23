#pragma once

#include <initializer_list>
#include <stdexcept>
#include <iostream>
#include <array>
#include <algorithm>

/**
 * @brief Queue data structure.
 * @tparam T Datatype of queue.
 * @tparam size Maximum number of elements in the queue.
*/
template <typename T, size_t size>
class Queue
{
private:
	std::array<T, size> m_Data = {};
	size_t m_Front = 0, m_Rear = 0, m_CurrentSize = 0;

public:
	/**
	 * @brief The maximum size of the queue, initialised at runtime.
	*/
	const size_t MAX_SIZE = size;

	/**
	 * @brief Constructs a queue with given elements.
	 * Iterates through each element and enqueues it.
	 * @param elements Initializer list of elements.
	*/
	Queue<T, size>(const std::initializer_list<T>& elements);

	/**
	 * @brief Constructs and empty queue.
	*/
	Queue<T, size>() = default;

	/**
	 * @brief Enqueues an item at the end of the queue.
	 * @param item Item to queue.
	*/
	void enQueue(const T& item);

	/**
	 * @brief Dequeues the item at the front of the queue.
	 * @return The dequeued item.
	*/
	T deQueue();

	/**
	 * @brief Returns the current number of elements in the queue.
	 * @return The number of elements in the queue.
	 */
	[[nodiscard]] size_t currentSize() const;

	/**
	 * @brief Checks if the queue is empty.
	 * @return True if the queue is empty else false.
	*/
	[[nodiscard]] bool isEmpty() const;

	/**
	 * @brief Checks if the queue is full.
	 * @return True if the queue is full else false.
	*/
	[[nodiscard]] bool isFull() const;
	
	template<typename T, size_t size>
	friend std::ostream& operator<<(std::ostream& os, const Queue<T, size>& queue);
};

template <typename T, size_t size>
Queue<T, size>::Queue(const std::initializer_list<T>& elements) :
	m_Data{T()}, m_Rear(elements.size() - 1), m_CurrentSize(elements.size())
{
	if (elements.size() > MAX_SIZE)
		throw std::runtime_error("Too many elements given for maximum queue size.");

	std::copy(begin(elements), end(elements), begin(m_Data));
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

	for (size_t i = 0; i < queue.MAX_SIZE - 1; ++i) {
		os << queue.m_Data[i] << ", ";
	}

	return os << queue.m_Data[queue.MAX_SIZE - 1] << "] Front = " << queue.m_Front << "(" << queue.m_Data[queue.m_Front] << ") Rear = " << queue.m_Rear << "(" << queue.m_Data[queue.m_Rear] << ")";
}