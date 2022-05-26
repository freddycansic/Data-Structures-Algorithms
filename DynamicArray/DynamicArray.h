#pragma once

#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <iterator>

template<typename T>
class DynamicArray
{

private:
	size_t m_Count = 0, m_CountAlloced = 0;
	T* m_Data = nullptr;

	T* allocNewArray(size_t count);

public:
	DynamicArray(const std::initializer_list<T>& elements);
	~DynamicArray();

	void append(const T& element);

	template<typename T>
	friend std::ostream& operator<<(std::ostream& os, const DynamicArray<T>& arr);
};

template <typename T>
DynamicArray<T>::DynamicArray(const std::initializer_list<T>& elements) :
	m_Count(elements.size()), m_CountAlloced(static_cast<size_t>(pow( ceil( log2(elements.size()) ), 2) ))
{
	m_Data = allocNewArray(m_CountAlloced);

	std::copy(elements.begin(), elements.end(), m_Data);
}

template <typename T>
DynamicArray<T>::~DynamicArray()
{
	std::cout << "FREEING " << m_CountAlloced << " (" << m_CountAlloced * sizeof(T) << " BYTES)" << std::endl;
	free(m_Data);
}

template <typename T>
void DynamicArray<T>::append(const T& element)
{
	// if there is not enough memory allocated for new element
	if (m_CountAlloced <= m_Count) {

		m_CountAlloced *= 2;

		// create new array with doubled size
		T* tempArr = allocNewArray(m_CountAlloced);

		std::copy(m_Data, m_Data + m_Count, tempArr);

		free(m_Data);

		m_Data = tempArr;
	}
	
	// add new element to array
	m_Data[m_Count++] = element;
}

template <typename T>
T* DynamicArray<T>::allocNewArray(size_t count)
{
	std::cout << "ALLOCATING " << count << " ELEMENTS (" << count * sizeof(T) << " BYTES)" << std::endl;
	return static_cast<T*>(malloc(count * sizeof(T)));
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const DynamicArray<T>& arr) {
	os << "DynamicArray = [";

	for (size_t i = 0; i < arr.m_Count - 1; ++i) {
		os << arr.m_Data[i] << ", ";
	}

	return os << arr.m_Data[arr.m_Count - 1] << "]";
	
}