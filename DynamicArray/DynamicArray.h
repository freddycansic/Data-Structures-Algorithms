#pragma once

#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <stdexcept>

// toggle comment to toggle debug messages
#define DYNAMIC_ARRAY_DEBUG

template<typename T>
class DynamicArray
{
private:
	size_t m_Count = 0, m_CountAlloced = 0;
	T* m_Data = nullptr;

	T* allocNewArray(size_t count);
	void doubleArraySize();

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

	DynamicArray<T>& operator=(DynamicArray<T>&& other) noexcept;

	~DynamicArray();

	void append(const T& element);
	void remove(const T& element);
	[[nodiscard]] size_t count(const T& element) const;
	[[nodiscard]] size_t len() const;
	[[nodiscard]] size_t index(const T& element) const;
	void insert(size_t pos, const T& element);
	T pop();
	T pop(size_t pos);
	void reserve(size_t count);

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
	if (m_Data != nullptr) {
		std::cout << "FREEING " << m_CountAlloced << " ELEMENTS (" << m_CountAlloced * sizeof(T) << " BYTES)" << std::endl;
	}
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
	m_Count(other.m_Count), m_CountAlloced(other.m_CountAlloced), m_Data(other.m_Data)
{
	std::cout << "MOVE CTOR" << std::endl;

	other.m_Data = nullptr;
}

template<typename T>
DynamicArray<T>& DynamicArray<T>::operator=(DynamicArray<T>&& other) noexcept
{
	std::cout << "MOVE ASSIGNMENT" << std::endl;

	m_Count = other.m_Count;
	m_CountAlloced = other.m_CountAlloced;
	m_Data = other.m_Data;

	other.m_Data = nullptr;

	return *this;
}


template <typename T>
void DynamicArray<T>::append(const T& element)
{
	// if there is not enough memory allocated for new element
	if (m_CountAlloced <= m_Count) {
		doubleArraySize();
	}
	
	// add new element to array
	m_Data[m_Count++] = element;
}

template<typename T>
void DynamicArray<T>::remove(const T& element)
{
	if (const auto elementIt = std::find(m_Data, m_Data + m_Count, element); elementIt == m_Data + m_Count) {
		throw std::range_error("Cannot remove an element which is not in array.");
	} else {
		std::copy(elementIt + 1, m_Data + m_Count, elementIt);
	}

	--m_Count;
}

template<typename T>
size_t DynamicArray<T>::count(const T& element) const
{
	return std::count(m_Data, m_Data + m_Count, element);
}

template<typename T>
size_t DynamicArray<T>::len() const
{
	return m_Count;
}

template<typename T>
size_t DynamicArray<T>::index(const T& element) const
{
	return std::distance(m_Data, std::find(m_Data, m_Data + m_Count, element));
}

template<typename T>
void DynamicArray<T>::insert(size_t pos, const T& element)
{
	if (m_CountAlloced <= m_Count) {
		doubleArraySize();
	}

	// 1, 2, 3 (1, '7'
	// 1, 7, 2, 3

	std::copy(m_Data + pos, m_Data + m_Count, m_Data + pos + 1);
	m_Data[pos] = element;

	++m_Count;
}

template<typename T>
T DynamicArray<T>::pop()
{
	return m_Data[--m_Count];
}
	
template<typename T>
T DynamicArray<T>::pop(size_t pos)
{
	
}

template<typename T>
void DynamicArray<T>::reserve(size_t count)
{
}

template <typename T>
T* DynamicArray<T>::allocNewArray(size_t count)
{
#ifdef DYNAMIC_ARRAY_DEBUG
	std::cout << "ALLOCATING " << count << " ELEMENTS (" << count * sizeof(T) << " BYTES)" << std::endl;
#endif

	return static_cast<T*>(malloc(count * sizeof(T)));
}

template <typename T>
void DynamicArray<T>::doubleArraySize()
{
	m_CountAlloced *= 2;

	// create new array with doubled size
	T* tempArr = allocNewArray(m_CountAlloced);

	std::copy(m_Data, m_Data + m_Count, tempArr);

	free(m_Data);

	m_Data = tempArr;
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