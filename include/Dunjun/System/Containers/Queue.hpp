#ifndef DUNJUN_SYSTEM_QUEUE_HPP
#define DUNJUN_SYSTEM_QUEUE_HPP

#include <Dunjun/System/Containers.hpp>

namespace Dunjun
{
	// Returns the number of elements in the queue
	template <typename T>
	size_t len(const Queue<T>& q);
	// Returns number of spaces remaining in queue
	template <typename T>
	size_t space(const Queue<T>& q);

	// adds item behind current offset
	// added item is last in queue
	template <typename T>
	size_t pushBack(Queue<T>& q, const T& item);
	// moves offset forward by one
	// TODO: make this affect data
	template <typename T>
	void popBack(Queue<T>& q);

	// adds item in front of current offset
	// added item first in queue
	template <typename T>
	size_t pushFront(Queue<T>& q, const T& item);
	// moves offset back by one
	// TODO: make this affect data
	template <typename T>
	void popFront(Queue<T>& q);

	// Appends a set of items to the end of the queue and changes length to match
	// expands the queue if needed
	template <typename T>
	size_t push(Queue<T>& q, const T* items, size_t count);
	// Removes a set of items from the end of the queue and changes length to match
	// expands the queue if needed
	template <typename T>
	void pop(Queue<T>& q, size_t count);

	// Iterator :: Returns a pointer to where to insert data
	template <typename T>
	inline T* begin(Queue<T>& q);
	template <typename T>
	const T* begin(const Queue<T>& q);
	// Iterator :: Returns a pointer to where the queue ends
	template <typename T>
	T* end(Queue<T>& q);
	template <typename T>
	const T* end(const Queue<T>& q);

	// Returns pointer to the first item in the queue
	template <typename T>
	T& front(Queue<T>& q);
	template <typename T>
	const T* front(const Queue<T>& q);
	// Returns a pointer to the last item in the queue
	template <typename T>
	T& back(Queue<T>& q);
	template <typename T>
	const T* back(const Queue<T>& q);

	// Changes queue length to 0
	// Does not change capacity or zero old values
	template <typename T>
	void clear(Queue<T>& q);

	// Changes the size of the queue
	// sets length and increases capacity if legnth > capacity
	//template <typename T>
	//void resize(Queue<T>& q, size_t length);
	// Sets queue capacity to capacity
	// Allocates or deallocates memory as needed
	// and sets capacity to match
	template <typename T>
	void setCapacity(Queue<T>& q, size_t capacity);
	// Allocates number of spaces equal to amount
	// and sets capacity to match
	template <typename T>
	void reserve(Queue<T>& q, size_t count);

	// increases queue capacity to (current * 2 + 2)
	template <typename T>
	void grow(Queue<T>& q, size_t minCapacity = 0);

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
	inline size_t len(const Queue<T>& q)
	{
		return q.m_capacity;
	}

	template <typename T>
	size_t space(const Queue<T>& q)
	{
		return len(q.m_data) - q.m_capacity;
	}

	/////////////////////////////////////////

	template <typename T>
	size_t pushBack(Queue<T>& q, const T& item)
	{
		if (space(q) == 0)
			grow(q);

		q[q.m_capacity++] = item;

		return q.m_capacity;
	}

	template <typename T>
	void popBack(Queue<T>& q)
	{
		assert(q.m_capacity > 0);

		q.m_capacity--;
	}

	////////////////////////////////////
	template <typename T>
	size_t pushFront(Queue<T>& q, const T& item)
	{
		if(space(q) == 0)
			grow(q);

		q.m_offset = (q.m_offset - 1 + len(q.m_data)) % len(q.m_data);

		q[0] = item;

		q.m_capacity++;

		return q.m_capacity;
	}

	template <typename T>
	void popFront(Queue<T>& q)
	{
		assert(q.m_capacity > 0);

		q.m_offset = (q.m_offset + 1) % len(q.m_data);
		q.m_capacity--;
	}

	////////////////////////////////////
	template <typename T>
	size_t push(Queue<T>& q, const T* items, size_t count)
	{
		if(space(q) < count)
			grow(q, q.m_capacity + count);

		const size_t length = len(q.m_data);
		const size_t insert = (q.m_offset + q.m_capacity) % length;

		size_t toInsert = count;

		if(insert + toInsert > length)
			toInsert = length - insert;

		std::memcpy(begin(q.m_data) + insert, items, toInsert * sizeof(T));

		items += toInsert;

		toInsert = count - toInsert;

		std::memcpy(begin(q.m_data), items, toInsert * sizeof(T));

		q.m_capacity += count;

		return q.m_capacity;
	}
	template <typename T>
	void pop(Queue<T>& q, size_t count)
	{
		assert(q.m_capacity > count && "Queue<T>::pop() - Count cannot be greater than queue length." );

		q.m_offset = (q.m_offset + count) % len(q.m_data);
		q.m_capacity -= count;
	}

	////////////////////////////////////
	template <typename T>
	inline T* begin(Queue<T>& q)
	{
		return begin(q.m_data) + q.m_offset;
	}

	template <typename T>
	inline const T* begin(const Queue<T>& q)
	{
		return begin(q.m_data) + q.m_offset;
	}

	template <typename T>
	T* end(Queue<T>& q)
	{
		const size_t endPos = (q.m_offset + q.m_capacity) % len(q.m_data);

		//return !(endPos < len(q.m_data)) ? end(q.m_data)
		//								   : begin(q.m_data) + endPos;
		return !(endPos < len(q.m_data)) ? end(q.m_data)
										 : begin(q.m_data) + endPos;
	}

	template <typename T>
	const T* end(const Queue<T>& q)
	{
		const size_t endPos = q.m_offset + q.m_capacity % len(q.m_data);

		//return !(endPos < len(q.m_data)) ? end(q.m_data)
		//								   : begin(q.m_data) + endPos;
		return !(endPos < len(q.m_data)) ? end(q.m_data)
										 : begin(q.m_data) + endPos;
	}


	////////////////////////////////////
	template <typename T>
	T& front(Queue<T>& q)
	{
		assert(q.m_capacity > 0 && "Queue<T> Queue has no members");
		return q[0];
	}

	template <typename T>
	const T* front(const Queue<T>& q)
	{
		assert(q.m_capacity > 0 && "Queue<T> Queue has no members");
		return q[0];
	}

	template <typename T>
	T& back(Queue<T>& q)
	{
		assert(q.m_capacity > 0 && "Queue<T> Queue has no members");
		return q[q.m_capacity - 1];
	}

	template <typename T>
	const T* back(const Queue<T>& q)
	{
		assert(q.m_capacity > 0 && "Queue<T> Queue has no members");
		return q[q.m_capacity - 1];
	}

	////////////////////////////////////
	template <typename T>
	void clear(Queue<T>& q)
	{
		q.m_offset = 0;
		q.m_capacity = 0;
	}

	////////////////////////////////////
	//template <typename T>
	//void resize(Queue<T>& q, size_t length)
	//{
	//
	//}

	template <typename T>
	void setCapacity(Queue<T>& q, size_t capacity)
	{
		const size_t oldLength = len(q.m_data);

		//q.m_capacity = capacity;
		resize(q.m_data, capacity);

		if(oldLength < q.m_offset + q.m_capacity)
		{
			std::memmove(begin(q.m_data) + capacity - (oldLength - q.m_offset),
						 begin(q.m_data) + q.m_offset,
						 (oldLength - q.m_offset) * sizeof(T));

			q.m_offset += capacity - oldLength;
		}

	}

	template <typename T>
	void reserve(Queue<T>& q, size_t count)
	{
		assert(count > 0 && "Queue<T>::reserve - count must be greater than 0.");

		setCapacity(q, q.m_capacity + count);
	}

	// TODO: remove this - ring buffer should only grow by explicit amounts
	template <typename T>
	void grow(Queue<T>& q, size_t minCapacity)
	{
		size_t newCapacity = 2 * len(q.m_data) + 2;

		if(newCapacity < minCapacity)
			newCapacity = minCapacity;

		setCapacity(q, newCapacity);
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
	Queue<T>::Queue(Allocator& a)
		: m_data(a)
		, m_capacity(0)
		, m_offset(0)
	{
	}

// cancel offset overloaded operators for testing
#if 0
	template <typename T>
	T& Queue<T>::operator[](size_t index)
	{
		return m_data[index % len(m_data)];
	}

	template <typename T>
	const T& Queue<T>::operator[](size_t index) const
	{
		return m_data[index % len(m_data)];
	}
#else
	template <typename T>
	T& Queue<T>::operator[](size_t index)
	{
		return m_data[(m_offset + index) % len(m_data)];
	}

	template <typename T>
	const T& Queue<T>::operator[](size_t index) const
	{
		return m_data[(m_offset + index) % len(m_data)];
	}
#endif
} // end Dunjun

#endif
