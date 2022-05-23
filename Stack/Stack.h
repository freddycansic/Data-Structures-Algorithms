#pragma once

#include <initializer_list>
#include <stdexcept>
#include <iostream>
#include <array>
#include <algorithm>

template<typename T, size_t size>
class Stack {

private:
	std::array<T, size> m_Data = {};
	size_t m_StackPointer = 0, m_CurrentSize = 0;

public:
	const size_t MAX_SIZE = size;

	Stack<T, size>(const std::initializer_list<T>& elements);
	Stack<T, size>() = default;

	void push(const T& item);
	T pop();
	[[nodiscard]] const T& peek() const;

	[[nodiscard]] size_t currentSize() const;
	[[nodiscard]] bool isEmpty() const;
	[[nodiscard]] bool isFull() const;

	template<typename T, size_t size>
	friend std::ostream& operator<<(std::ostream& os, const Stack<T, size>& stack);
};

template<typename T, size_t size>
Stack<T, size>::Stack(const std::initializer_list<T>& elements) :
	m_StackPointer(elements.size() - 1), m_CurrentSize(elements.size())
{
	if (elements.size() > MAX_SIZE)
		throw std::range_error("Too many elements given for maximum stack size.");

	std::copy(begin(elements), end(elements), begin(m_Data));
}

template<typename T, size_t size>
void Stack<T, size>::push(const T& item)
{
	if (isFull())
		throw std::range_error("Cannot add item to a full stack. Stack overflow.");

	++m_CurrentSize;

	++m_StackPointer;
	m_Data[m_StackPointer] = item;
}

template<typename T, size_t size>
T Stack<T, size>::pop()
{
	if (isEmpty())
		throw std::range_error("Cannot pop item from empty stack. Stack underflow.");

	--m_CurrentSize;

	const T& value = m_Data.at(m_StackPointer);
	--m_StackPointer;
	return value;
}

template<typename T, size_t size>
const T& Stack<T, size>::peek() const
{
	return m_Data.at(m_StackPointer);
}

template<typename T, size_t size>
size_t Stack<T, size>::currentSize() const
{
	return m_CurrentSize;
}

template<typename T, size_t size>
bool Stack<T, size>::isEmpty() const
{
	return m_CurrentSize == 0;
}

template<typename T, size_t size>
bool Stack<T, size>::isFull() const
{
	return m_CurrentSize == MAX_SIZE;
}

template<typename T, size_t size>
std::ostream& operator<<(std::ostream& os, const Stack<T, size>& stack) {
	os << "Stack = [";

	for (size_t i = 0; i < stack.MAX_SIZE - 1; ++i) {
		os << stack.m_Data.at(i) << ", ";
	}

	return os << stack.m_Data.at(stack.MAX_SIZE - 1) << "] Stack pointer = " << stack.m_StackPointer << "(" << stack.m_Data.at(stack.m_StackPointer) << ")";

}