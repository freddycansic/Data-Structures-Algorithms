#pragma once

#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <iterator>

// toggle comment to toggle debug messages
#define DYNAMIC_ARRAY_DEBUG

template<typename T>
class DynamicArray
{

private:
	size_t m_Count = 0, m_CountAlloced = 0;
	T* m_Data = nullptr;

	T* allocNewArray(size_t count);

public:
	/**
	 * @brief Construct a dynamic array with a list of elements.
	 * @param elements List of elements to construct array with.
	*/
	DynamicArray(const std::initializer_list<T>& elements);

	/**
	 * @brief Construct an empty dynamic array.
	*/
	DynamicArray() = default;

	/**
	 * @brief Copy a dynamic array into another dynamic array.
	 * @param other The array to copy from.
	*/
	DynamicArray(const DynamicArray<T>& other);

	/**
	 * @brief Move a dynamic array into another dynamic array.
	 * @param other The array to move from.
	*/
	DynamicArray(DynamicArray<T>&& other) noexcept;

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
#ifdef DYNAMIC_ARRAY_DEBUG
	std::cout << "FREEING " << m_CountAlloced << " ELEMENTS (" << m_CountAlloced * sizeof(T) << " BYTES)" << std::endl;
#endif

	free(m_Data);
}

template<typename T>
DynamicArray<T>::DynamicArray(const DynamicArray<T>& other) :
	m_Count(other.m_Count), m_CountAlloced(other.m_CountAlloced)
{
	std::cout << "COPY CTOR" << std::endl;

	this->m_Data = allocNewArray(other.m_CountAlloced);
	std::copy(other.m_Data, other.m_Data + other.m_Count, this->m_Data);
}

template<typename T>
DynamicArray<T>::DynamicArray(DynamicArray<T>&& other) noexcept :
	m_Count(std::move(other.m_Count)), m_CountAlloced(std::move(other.m_CountAlloced))
{
	std::cout << "MOVE CTOR" << std::endl;

	free(this->m_Data);
	m_Data = std::move(other.m_Data);
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
#ifdef DYNAMIC_ARRAY_DEBUG
	std::cout << "ALLOCATING " << count << " ELEMENTS (" << count * sizeof(T) << " BYTES)" << std::endl;
#endif

	return static_cast<T*>(malloc(count * sizeof(T)));
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const DynamicArray<T>& arr) {
	for (size_t i = 0; i < arr.m_Count - 1; ++i) {
		os << arr.m_Data[i] << ", ";
	}

	os << arr.m_Data[arr.m_Count - 1];

#ifdef DYNAMIC_ARRAY_DEBUG
	return os << " NUM ELEMENTS = " << arr.m_Count << " (" << arr.m_Count * sizeof(T) << " BYTES) NUM ALLOCATED ELEMENTS = " << arr.m_CountAlloced << " (" << arr.m_CountAlloced * sizeof(T) << " BYTES)";
#else
	return os;
#endif
}