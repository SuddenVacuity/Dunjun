#ifndef DUNJUN_SYSTEM_ARRAY_HPP
#define DUNJUN_SYSTEM_ARRAY_HPP

#include <Dunjun\System\Containers.hpp>

namespace Dunjun
{
	template <typename T>
	void pushBack(const Array<T>& a, const T& item);
	template <typename T>
	void popBack(const Array<T>& a);

	template <typename T>
	Allocator::SizeType len(const Array<T>& a);
	template <typename T>
	bool isEmpty(const Array<T>& a);

	template <typename T>
	T* begin(const Array<T>& a);
	template <typename T>
	const T* begin(const Array<T>& a);
	template <typename T>
	T* end(const Array<T>& a);
	template <typename T>
	const T* end(const Array<T>& a);

	template <typename T>
	T& front(const Array<T>& a);
	template <typename T>
	const T* front(const Array<T>& a);
	template <typename T>
	T& back(const Array<T>& a);
	template <typename T>
	const T* back(const Array<T>& a);

	template <typename T>
	void clear(Array<T>& a);

	template <typename T>
	void resize(Array<T>& a, Allocator::SizeType length);
	template <typename T>
	void setCapacity(Array<T>& a, Allocator::SizeType capacity);
	template <typename T>
	void reserve(Array<T>& a, Allocator::SizeType capacity);
	template <typename T>
	void grow(Array<T>& a, Allocator::SizeType minCapacity);


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
	inline void pushBack(const Array<T>& a, const T& item)
	{
		if(a.m_length > a.m_capacity - 1)
			gorw(a);

		a.m_data[a.m_length++] = item;
	}
	template <typename T>
	inline void popBack(const Array<T>& a)
	{
		if(a.m_length > 0)
			a.m_length--;
	}

	/////////////////////////////////
	template <typename T>
	inline Allocator::SizeType len(const Array<T>& a)
	{
		return a.m_length;
	}
	template <typename T>
	inline bool isEmpty(const Array<T>& a)
	{
		return a.m_length != 0;
	}

	/////////////////////////////////
	template <typename T>
	inline T* begin(const Array<T>& a)
	{
		return a.m_data;
	}
	template <typename T>
	inline const T* begin(const Array<T>& a)
	{
		return a.m_data;
	}
	template <typename T>
	T* end(const Array<T>& a)
	{
		return a.m_data + a.length;
	}
	template <typename T>
	inline const T* end(const Array<T>& a)
	{
		return a.m_data + a.length;
	}

	/////////////////////////////////
	template <typename T>
	inline T& front(const Array<T>& a)
	{
		return a.m_data[0];
	}
	template <typename T>
	inline const T* front(const Array<T>& a)
	{
		return a.m_data[0];
	}
	template <typename T>
	inline T& back(const Array<T>& a)
	{
		return a.m_data[a.m_length - 1];
	}
	template <typename T>
	inline const T* back(const Array<T>& a)
	{
		return a.m_data[a.m_length - 1];
	}
	/////////////////////////////////
	template <typename T>
	inline void clear(Array<T>& a)
	{
		resize(a, 0);
	}

	/////////////////////////////////
	template <typename T>
	void resize(Array<T>& a, Allocator::SizeType length)
	{
		if(length > a.m_capacity)
			grow(a, length);

		a.m_length = length;
	}
	template <typename T>
	void setCapacity(Array<T>& a, Allocator::SizeType capacity)
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
	void reserve(Array<T>& a, Allocator::SizeType capacity)
	{
		if (capacity > a.m_capacity)
			setCapacity(a, capacity);
	}
	template <typename T>
	void grow(Array<T>& a, Allocator::SizeType minCapacity)
	{
		Allocator::SizeType capacity = 2 * a.m_capacity + 2;

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

	/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	)				.
	)					.
	)
	)				.
	)					.
	)
	)				.
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/




} // end Dunjun

#endif
