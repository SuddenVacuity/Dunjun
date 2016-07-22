#ifndef DUNJUN_SYSTEM_ARRAY_HPP
#define DUNJUN_SYSTEM_ARRAY_HPP

#include <Dunjun\System\Containers.hpp>

namespace Dunjun
{
	// Returns the number of elements in the array
	template <typename T>
	size_t len(const Array<T>& a);
	// Returns the max number of elements the array can hold
	template <typename T>
	size_t capacity(const Array<T>& a);

	// Appends an item to the array and returns the number of items in the array
	template <typename T>
	size_t append(Array<T>& a, const T& item);
	// Append items to the array and returns the number of items in the array
	template <typename T>
	size_t append(Array<T>& a, const T* items, size_t count);

	// Appends an item to the array and changes array length to match
	// expands the array if needed
	//template <typename T>
	//size_t pushBack(Array<T>& a, const T& item);
	// Removes the last item in the array
	template <typename T>
	void popBack(const Array<T>& a);
	// Appends a set of items to the array and changes length to match
	// expands the array if needed
	template <typename T>
	size_t push(Array<T>& a, const T* items, size_t count);

	// Iterator :: Returns a pointer to where the array begins
	template <typename T>
	T* begin(Array<T>& a);
	template <typename T>
	const T* begin(const Array<T>& a);
	// Iterator :: Returns a pointer to where the array ends
	template <typename T>
	T* end(Array<T>& a);
	template <typename T>
	const T* end(const Array<T>& a);

	// Returns pointer to the first item in the array
	template <typename T>
	T& front(Array<T>& a);
	template <typename T>
	const T* front(const Array<T>& a);
	// Returns a pointer to the last item in the array
	template <typename T>
	T& back(Array<T>& a);
	template <typename T>
	const T* back(const Array<T>& a);

	// Changes array length to 0
	// Does not change capacity or zero old values
	template <typename T>
	void trim(Array<T>& a);
	template <typename T>
	void clear(Array<T>& a);

	// Changes the size of the array
	// sets length and increases capacity if legnth > capacity
	template <typename T>
	void resize(Array<T>& a, size_t length);
	// Sets array capacity to capacity
	// Allocates or deallocates memory as needed
	// and sets capacity to match
	template <typename T>
	void setCapacity(Array<T>& a, size_t capacity);
	// Allocates number of spaces equal to amount
	// and sets capacity to match
	template <typename T>
	void reserve(Array<T>& a, size_t capacity);
	// increases array capacity to (current * 2 + 2)
	template <typename T>
	void grow(Array<T>& a, size_t minCapacity = 0);


	/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	)				.
	)					.
	)
	)				.
	)					.
	)
	)				.
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

	template <typename T>
	inline size_t len(const Array<T>& a)
	{
		return a.m_length;
	}
	template <typename T>
	inline size_t capacity(const Array<T>& a)
	{
		return a.m_capacity;
	}

	//////////////////////////////////

	template <typename T>
	size_t append(Array<T>& a, const T& item)
	{
		if (a.m_length + 1 > a.m_capacity)
			grow(a);

		a.m_data[a.m_length++] = item;

		return a.m_length;
	}

	template <typename T>
	size_t append(Array<T>& a, const T* items, size_t count)
	{
		if (a.m_capacity <= a.m_length + count)
			grow(a, a.m_length + count);

		memcpy(&a.m_data[a.m_length], items, count * sizeof(T));
		a.m_length += count;

		return a.m_length;
	}

	//template <typename T>
	//inline size_t pushBack(Array<T>& a, const T& item)
	//{
	//	if(a.m_length == a.m_capacity)
	//		grow(a);
	//
	//	a.m_data[a.m_length++] = item;
	//
	//	return a.m_length;
	//}
	template <typename T>
	inline void popBack(Array<T>& a)
	{
		assert(a.m_length > 0 && "Array<T> Array must be greater than 0");

		a.m_length--;
	}

	template <typename T>
	inline size_t push(Array<T>& a, const T* items, size_t count)
	{
		if(a.m_capacity <= a.m_length + count)
			grow(a, a.m_length + count);

		std::memcpy(&a.m_data[a.m_length], items, count * sizeof(T));
		a.m_length += count;

		return a.m_length;
	}

	/////////////////////////////////
	template <typename T>
	inline T* begin(Array<T>& a)
	{
		return a.m_data;
	}
	template <typename T>
	inline const T* begin(const Array<T>& a)
	{
		return a.m_data;
	}
	template <typename T>
	inline T* end(Array<T>& a)
	{
		return a.m_data + a.m_length;
	}
	template <typename T>
	inline const T* end(const Array<T>& a)
	{
		return a.m_data + a.m_length;
	}

	/////////////////////////////////
	template <typename T>
	inline T& front(Array<T>& a)
	{
		assert(a.m_length > 0 && "Array<T> Array[0] does not exist");
		return a.m_data[0];
	}
	template <typename T>
	inline const T* front(const Array<T>& a)
	{
		assert(a.m_length > 0 && "Array<T> Array[0] does not exist");
		return a.m_data[0];
	}
	template <typename T>
	inline T& back(Array<T>& a)
	{
		assert(a.m_length > 0 && "Array<T> Array has no members");
		return a.m_data[a.m_length - 1];
	}
	template <typename T>
	inline const T* back(const Array<T>& a)
	{
		assert(a.m_length > 0 && "Array<T> Array has no members");
		return a.m_data[a.m_length - 1];
	}
	/////////////////////////////////
	template <typename T>
	inline void trim(Array<T>& a)
	{
		setCapacity(a, a.m_length);
	}
	template <typename T>
	inline void clear(Array<T>& a)
	{
		resize(a, 0);
	}

	/////////////////////////////////
	template <typename T>
	inline void resize(Array<T>& a, size_t length)
	{
		if(length > a.m_capacity)
			grow(a, length);

		a.m_length = length;
	}
	template <typename T>
	inline void setCapacity(Array<T>& a, size_t capacity)
	{
		if(capacity == a.m_capacity)
			return;

		if(capacity < a.m_length)
			resize(a, capacity);

		T* data = nullptr;

		if(capacity > 0)
		{
			data = (T*)a.m_allocator->allocate(capacity * sizeof(T), alignof(T));

			std::memcpy(data, a.m_data, a.m_length * sizeof(T));
		}

		a.m_allocator->deallocate(a.m_data);
		a.m_data = data;
		a.m_capacity = capacity;
	}
	template <typename T>
	void reserve(Array<T>& a, size_t capacity)
	{
		//assert(capacity > 0 && "Array<T>::reserve - count must be greater than 0.");
		
		if(capacity > a.m_capacity)
			setCapacity(a, capacity);
	}
	template <typename T>
	inline void grow(Array<T>& a, size_t minCapacity)
	{
		size_t capacity = 2 * a.m_capacity + 2;

		if(capacity < minCapacity)
			capacity = minCapacity;

		setCapacity(a, capacity);
	}


	/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	)				.
	)					.
	)
	)				.
	)					.
	)
	)				.
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

	template <typename T>
	inline Array<T>::Array(Allocator& a)
		: m_allocator(&a)
		, m_length(0)
		, m_capacity(0)
		, m_data(nullptr)
	{
	}

	template <typename T>
	inline Array<T>::~Array()
	{

		m_allocator->deallocate(m_data);
	}

	template <typename T>
	Array<T>::Array(const Array& other)
		: m_allocator(other.m_allocator)
		, m_length(0)
		, m_capacity(0)
		, m_data(nullptr)
	{
		const size_t num = other.m_length;
		setCapacity(*this, num);
		std::memcpy(m_data, other.m_data, num * sizeof(T));
		m_length = num;
	}

	template <typename T>
	inline Array<T>& Array<T>::operator=(const Array& other)
	{
		const size_t num = other.m_length;
		resize(*this, num);
		std::memcpy(m_data, other.m_data, num * sizeof(T));
		//m_length = num;
		return *this;
	}

	template <typename T>
	inline T& Array<T>::operator[](size_t index)
	{
		return m_data[index];
	}
	template <typename T>
	inline const T& Array<T>::operator[](size_t index) const
	{
		return m_data[index];
	}

} // end Dunjun

#endif
