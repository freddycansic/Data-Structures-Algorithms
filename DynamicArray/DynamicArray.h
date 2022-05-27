#pragma once

#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <stdexcept>

// if using msvc use debug breaks
#ifdef _MSC_VER
	#define ASSERT(x, msg) if (!(x)) { std::cout << (msg) << std::endl; __debugbreak(); }

// otherwise use C assert
#else
	#include <cassert>
	#define ASSERT(x, msg) if (!(x)) { std::cout << (msg) << std::endl; assert(x); }

#endif

// toggle comment to toggle debug messages
//#define DYNAMIC_ARRAY_DEBUG

#define DATA_START m_Data
#define DATA_END (m_Data + m_Count)

/**
 * @brief A dynamically sized array template class.
 * @author Freddy Cansick
 * @date 26/5/2022
 */
template <typename T>
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
	 * @brief Copy a dynamic array into another dynamic array.
	 * @param other The array to copy from.
	 * @returns A copy of the given array.
	*/
	DynamicArray<T>& operator=(const DynamicArray<T>& other);

	/**
	 * @brief Move a dynamic array into another dynamic array.
	 * @param other The array to move from.
	*/
	DynamicArray(DynamicArray<T>&& other) noexcept;

	/**
	 * @brief Move a dynamic array into another dynamic array.
	 */
	DynamicArray<T>& operator=(DynamicArray<T>&& other) noexcept;

	~DynamicArray();

	/**
	 * @brief Append an element to the end of the array.
	 * @param element Element to add.
	 */
	void append(const T& element);

	/**
	 * @brief Remove the first instance of an element in an array.
	 * @param element Element to remove.
	 */
	void remove(const T& element);

	/**
	 * @brief Insert an element at a given position.
	 * @param pos Position in array to insert element into.
	 * @param element Element to insert.
	 */
	void insert(size_t pos, const T& element);

	/**
	 * @brief Remove and return element at the end of the array.
	 * @returns Element at the end of the array.
	 */
	T pop();

	/**
	 * @brief Remove and return element at given position in the array.
	 * @param pos Position in array to remove and return.
	 * @returns Element at specified position.
	 */
	T pop(size_t pos);

	/**
	 * @brief Append an element to the end of the array.
	 * @param count Number of elements to allocate memory for.
	 */
	void reserve(size_t count);

	/**
	 * @brief Clear every element in the array.
	 */
	void clear();

	/**
	 * @brief Returns the number of times a given parameter occurs in the array.
	 * @element Element to count.
	 * @returns Count of element in array.
	 */
	[[nodiscard]] size_t count(const T& element) const;

	/**
	 * @brief Returns the number of elements in the array.
	 * @returns Number of elements in the array.
	 */
	[[nodiscard]] size_t len() const;

	/**
	 * @brief Returns the index of a given element in an array.
	 * @param element Element to get index of.
	 * @returns Index of given element.
	 */
	[[nodiscard]] size_t index(const T& element) const;

	/**
	 * @brief Returns whether the array is empty or not.
	 * @returns If the array is empty or not.
	 */
	[[nodiscard]] bool isEmpty() const;

	/**
	 * @brief Returns a reference to the element at the given position.
	 * @param pos Position of element to return.
	 * @return Reference to the element at the given position.
	 */
	[[nodiscard]] T& at(size_t pos);

	/**
	 * @brief Returns a constant reference to the element at the given position.
	 * @param pos Position of element to return.
	 * @return Constant reference to the element at the given position.
	 */
	[[nodiscard]] const T& at(size_t pos) const;

	/**
	 * @brief Returns a reference to the element at the given position.
	 * @param pos Position of element to return.
	 * @return Reference to the element at the given position.
	 */
	[[nodiscard]] T& operator[](size_t pos);

	/**
	 * @brief Returns a constant reference to the element at the given position.
	 * @param pos Position of element to return.
	 * @return Constant reference to the element at the given position.
	 */
	[[nodiscard]] const T& operator[](size_t pos) const;

	/**
	 * @brief Return a pointer to the internal data structure.
	 * @returns A pointer to the internal data structure.
	 */
	[[nodiscard]] T* data() const;

	template<typename T>
	friend std::ostream& operator<<(std::ostream& os, const DynamicArray<T>& arr);
};

template <typename T>
DynamicArray<T>::DynamicArray(const std::initializer_list<T>& elements) :
	m_Count(elements.size()), m_CountAlloced(static_cast<size_t>(pow(2, ceil(log2(elements.size())))))
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
	this->m_Data = allocNewArray(other.m_CountAlloced);
	std::copy(other.m_Data, other.m_Data + other.m_Count, this->m_Data);
}

template<typename T>
DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray<T>& other)
{
	this->m_Count = other.m_Count;
	this->m_CountAlloced = other.m_CountAlloced;

	this->m_Data = allocNewArray(other.m_CountAlloced);
	std::copy(other.m_Data, other.m_Data + other.m_Count, this->m_Data);

	return *this;
}

template<typename T>
DynamicArray<T>::DynamicArray(DynamicArray<T>&& other) noexcept :
	m_Count(other.m_Count), m_CountAlloced(other.m_CountAlloced), m_Data(other.m_Data)
{
	other.m_Data = nullptr;
}

template<typename T>
DynamicArray<T>& DynamicArray<T>::operator=(DynamicArray<T>&& other) noexcept
{
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

		m_CountAlloced *= 2;
		reserve(m_CountAlloced);
	}
	
	// add new element to array
	m_Data[m_Count++] = element;
}

template<typename T>
void DynamicArray<T>::remove(const T& element)
{
	if (const auto elementIt = std::find(DATA_START, DATA_END, element); elementIt == DATA_END) {
		throw std::range_error("Cannot remove an element which is not in array.");
	} else {
		std::copy(elementIt + 1, DATA_END, elementIt);
	}

	--m_Count;
}

template<typename T>
void DynamicArray<T>::clear()
{
	m_Count = 0;
}

template<typename T>
size_t DynamicArray<T>::count(const T& element) const
{
	if (m_Count == 0) return 0;

	return std::count(DATA_START, DATA_END, element);
}

template<typename T>
size_t DynamicArray<T>::len() const
{
	return m_Count;
}

template<typename T>
size_t DynamicArray<T>::index(const T& element) const
{
	return std::distance(DATA_START, std::find(DATA_START, DATA_END, element));
}

template<typename T>
bool DynamicArray<T>::isEmpty() const
{
	return m_Count == 0;
}

template<typename T>
void DynamicArray<T>::insert(size_t pos, const T& element)
{
	ASSERT(pos < m_Count, "Insert array index out of bounds!");

	if (m_CountAlloced <= m_Count) {

		m_CountAlloced *= 2;
		reserve(m_CountAlloced);
	}

	// shift data after given element 1 to the right
	std::copy(DATA_START + pos, DATA_END, DATA_START + pos + 1);
	m_Data[pos] = element;

	++m_Count;
}

template<typename T>
T DynamicArray<T>::pop()
{
	ASSERT(m_Count != 0, "Cannot pop value from empty array!");

	const auto& element = m_Data[m_Count - 1];
	remove(element);
	
	return element;
}
	
template<typename T>
T DynamicArray<T>::pop(size_t pos)
{
	ASSERT(m_Count != 0, "Cannot pop value from empty array!");
	ASSERT(pos < m_Count, "Array index out of bounds!");

	const auto& element = m_Data[pos];
	remove(element);
	
	return element;
}

template<typename T>
void DynamicArray<T>::reserve(size_t count)
{
	if (count == m_CountAlloced) return;

	T* tempArr = allocNewArray(count);

	std::copy(DATA_START, DATA_END, tempArr);

	free(m_Data);

	m_Data = tempArr;

	m_CountAlloced = count;
}

template<typename T>
T& DynamicArray<T>::at(size_t pos)
{
	ASSERT(pos < m_Count, "Array index out of bounds!");

	return m_Data[pos];
}

template<typename T>
const T& DynamicArray<T>::at(size_t pos) const
{
	ASSERT(pos < m_Count, "Array index out of bounds!");

	return m_Data[pos];
}

template<typename T>
T& DynamicArray<T>::operator[](size_t pos)
{
	return at(pos);
}

template<typename T>
const T& DynamicArray<T>::operator[](size_t pos) const
{
	return at(pos);
}

template<typename T>
T* DynamicArray<T>::data() const
{
	if (m_Count == 0) {
		return nullptr;
	}

	return &m_Data[0];
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
	os << "[";

	if (arr.m_Count != 0) {
		for (size_t i = 0; i < arr.m_Count - 1; ++i) {
			os << arr.m_Data[i] << ", ";
		}

		os << arr.m_Data[arr.m_Count - 1];
	}

	os << "]";

#ifdef DYNAMIC_ARRAY_DEBUG
	return os << " NUM ELEMENTS = " << arr.m_Count << " (" << arr.m_Count * sizeof(T) << " BYTES) NUM ALLOCATED ELEMENTS = " << arr.m_CountAlloced << " (" << arr.m_CountAlloced * sizeof(T) << " BYTES)";
#else
	return os;
#endif
}