#ifndef DUNJUN_SYSTEM_RINGBUFFER_HPP
#define DUNJUN_SYSTEM_RINGBUFFER_HPP

#include <Dunjun/System/Containers.hpp>

namespace Dunjun
{
	// Returns the number of elements in the queue
	template <typename T>
	size_t len(const RingBuffer<T>& q);
	// Returns number of spaces remaining in queue
	template <typename T>
	size_t space(const RingBuffer<T>& q);

	// returns current position in buffer then increases offset
	template <typename T>
	const T& cycle(RingBuffer<T>& q);

	// adds item to current position without changing capacity
	template <typename T>
	void insert(RingBuffer<T>& q,const T item);

	// adds item behind current offset
	// added item is last in queue
	template <typename T>
	size_t pushBack(RingBuffer<T>& q, const T& item);
	// moves offset forward by one
	// TODO: make this affect data
	template <typename T>
	void popBack(RingBuffer<T>& q);

	// adds item in front of current offset
	// added item first in queue
	template <typename T>
	size_t pushFront(RingBuffer<T>& q, const T& item);
	// moves offset back by one
	// TODO: make this affect data
	template <typename T>
	void popFront(RingBuffer<T>& q);

	// Appends a set of items to the end of the queue and changes length to match
	// expands the queue if needed
	template <typename T>
	size_t push(RingBuffer<T>& q, const T* items, size_t count);
	// Removes a set of items from the end of the queue and changes length to match
	// expands the queue if needed
	template <typename T>
	void pop(RingBuffer<T>& q, size_t count);

	// Iterator :: Returns a pointer to where to insert data
	template <typename T>
	inline T* begin(RingBuffer<T>& q);
	template <typename T>
	const T* begin(const RingBuffer<T>& q);
	// Iterator :: Returns a pointer to where the queue ends
	template <typename T>
	T* end(RingBuffer<T>& q);
	template <typename T>
	const T* end(const RingBuffer<T>& q);

	// Returns pointer to the first item in the queue
	template <typename T>
	T& front(RingBuffer<T>& q);
	template <typename T>
	const T* front(const RingBuffer<T>& q);
	// Returns a pointer to the last item in the queue
	template <typename T>
	T& back(RingBuffer<T>& q);
	template <typename T>
	const T* back(const RingBuffer<T>& q);

	// Changes queue length to 0
	// Does not change capacity or zero old values
	template <typename T>
	void clear(RingBuffer<T>& q);

	// Changes the size of the queue
	// sets length and increases capacity if legnth > capacity
	//template <typename T>
	//void resize(RingBuffer<T>& q, size_t length);
	// Sets queue capacity to capacity
	// Allocates or deallocates memory as needed
	// and sets capacity to match
	template <typename T>
	void setCapacity(RingBuffer<T>& q, size_t capacity);
	// Allocates number of spaces equal to amount
	// and sets capacity to match
	template <typename T>
	void reserve(RingBuffer<T>& q, size_t count);

	// increases queue capacity to (current * 2 + 2)
	template <typename T>
	void grow(RingBuffer<T>& q, size_t count = 0);

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
	inline size_t len(const RingBuffer<T>& q)
	{
		return q.m_capacity;
	}

	template <typename T>
	size_t space(const RingBuffer<T>& q)
	{
		return len(q.m_data) - q.m_offset;
	}
	/////////////////////////////////////////

	template <typename T>
	const T& cycle(RingBuffer<T>& q)
	{
		q.m_offset = q.m_offset % q.m_capacity;
		return q.m_data[q.m_offset++];
	}

	template <typename T>
	void insert(RingBuffer<T>& q, const T item)
	{
		q.m_offset = q.m_offset % q.m_capacity;
		q.m_data[q.m_offset] = item;
	}

	/////////////////////////////////////////

	template <typename T>
	size_t pushBack(RingBuffer<T>& q, const T& item)
	{
		// copy first item in array for later
		T temp = q.m_data[0];

		// shift everything past [0] to the left by one
		std::memmove(begin(q.m_data), begin(q.m_data) + 1, (q.m_capacity - 1) * sizeof(T));

		// make last item in array equal to the previous first item
		q.m_data[q.m_offset - 1] = temp;

		q[q.m_capacity - 1] = item;


		//if (q.m_offset == q.m_capacity)
		//	q.m_offset = 0;
		//else
		//	q.m_offset++;
		//
		return q.m_offset;
	}

	template <typename T>
	void popBack(RingBuffer<T>& q)
	{
		//assert(q.m_capacity > 0 && "RingBuffer<T> RingBuffer must be greater than 0");

		if (q.m_offset == 0)
			q.m_offset = q.m_capacity - 1;
		else
			q.m_offset--;
	}

	////////////////////////////////////
	template <typename T>
	size_t pushFront(RingBuffer<T>& q, const T& item)
	{
		// copy first item in array for later
		T temp = q.m_data[q.m_offset - 1];

		// shift everything past [0] to the left by one
		std::memmove(begin(q.m_data) + 1, begin(q.m_data), (q.m_capacity - 1) * sizeof(T));

		// make last item in array equal to the previous first item
		q.m_data[0] = temp;

		q[0] = item;

		return q.m_capacity;
	}

	template <typename T>
	void popFront(RingBuffer<T>& q)
	{
		//assert(q.m_capacity > 0 && "RingBuffer<T>::popFront() RingBuffer capacity must be greater than 0");

		if (q.m_offset == q.m_capacity - 1)
			q.m_offset = 0;
		else
			q.m_offset++;
	}

	////////////////////////////////////
	template <typename T>
	size_t push(RingBuffer<T>& q, const T* items, size_t count)
	{
		if(q.m_capacity < count)
			grow(q, count - q.m_capacity);

		//const size_t length = len(q.m_data);

		// find current offset
		//const size_t insert = (q.m_offset + q.m_capacity) % length;

		// count of items - also used to find begin and end of data to be added
		size_t insertPos = q.m_offset;
		size_t firstInsert = count;

		if (insertPos + firstInsert > q.m_capacity)
			firstInsert = q.m_capacity - insertPos;
		// push first part of the data from rb[0] to m_data[end]

		std::memcpy(begin(q), items, firstInsert * sizeof(T));

		if(firstInsert != count)
		{
			size_t remainingInsert = count - firstInsert;

			std::memcpy(begin(q.m_data), items + firstInsert, remainingInsert * sizeof(T));
		}
		else
			q.m_offset = q.m_offset + count;


		//{
		//if (insert + toInsert > length)
		//	toInsert = length - insert;
		//
		//std::memcpy(begin(q.m_data) + insert, items, toInsert * sizeof(T));
		//
		//// wrap around to beginning
		//items += toInsert;
		//toInsert = count - toInsert;
		//
		//std::memcpy(begin(q.m_data), items, toInsert * sizeof(T));
		//}

		return q.m_capacity;
		// TODO: Fix RingBuffer
		//get 1 4 9 1 2 3 //expect 4 5 6 7 8 9
	}
	template <typename T>
	void pop(RingBuffer<T>& q, size_t count)
	{
		//assert(q.m_capacity > count && "RingBuffer<T>::pop() - Count cannot be greater than queue length.");
		//
		//q.m_offset = (q.m_offset + count) % len(q.m_data);
		//q.m_capacity -= count;
	}

	////////////////////////////////////
	template <typename T>
	inline T* begin(RingBuffer<T>& q)
	{
		return begin(q.m_data) + q.m_offset;
	}

	template <typename T>
	inline const T* begin(const RingBuffer<T>& q)
	{
		return begin(q.m_data) + q.m_offset;
	}

	template <typename T>
	T* end(RingBuffer<T>& q)
	{
		assert(!(q.m_capacity < 0));
		return q.m_offset > 0 ? begin(q.m_data) + q.m_offset - 1
							  : begin(q.m_data) + q.m_capacity - 1;
	}

	template <typename T>
	const T* end(const RingBuffer<T>& q)
	{
		assert(!(q.m_capacity < 0));
		return q.m_offset > 0 ? begin(q.m_data) + q.m_offset - 1
							  : begin(q.m_data) + q.m_capacity - 1;
	}


	////////////////////////////////////
	template <typename T>
	T& front(RingBuffer<T>& q)
	{
		assert(q.m_capacity > 0 && "RingBuffer<T> RingBuffer has no members");
		return q[0];
	}

	template <typename T>
	const T* front(const RingBuffer<T>& q)
	{
		assert(q.m_capacity > 0 && "RingBuffer<T> RingBuffer has no members");
		return q[0];
	}

	template <typename T>
	T& back(RingBuffer<T>& q)
	{
		assert(q.m_capacity > 0 && "RingBuffer<T> RingBuffer has no members");
		return q[q.m_capacity - 1];
	}

	template <typename T>
	const T* back(const RingBuffer<T>& q)
	{
		assert(q.m_capacity > 0 && "RingBuffer<T> RingBuffer has no members");
		return q[q.m_capacity - 1];
	}

	////////////////////////////////////
	template <typename T>
	void clear(RingBuffer<T>& q)
	{
		q.m_offset = 0;
		q.m_capacity = 0;
	}

	////////////////////////////////////
	//template <typename T>
	//void resize(RingBuffer<T>& q, size_t length)
	//{
	//
	//}

	template <typename T>
	void setCapacity(RingBuffer<T>& q, size_t capacity)
	{
		const size_t oldLength = q.m_capacity;

		q.m_capacity = capacity;
		resize(q.m_data, capacity);

		//if (oldLength < q.m_offset + q.m_capacity)
		//{
		//	std::memmove(begin(q.m_data) + capacity - (oldLength - q.m_offset),
		//		begin(q.m_data) + q.m_offset,
		//		(oldLength - q.m_offset) * sizeof(T));
		//}
	}

	template <typename T>
	void reserve(RingBuffer<T>& q, size_t count)
	{
		assert(count > 0 && "RingBuffer<T>::reserve - count must be greater than 0.");

		setCapacity(q, q.m_capacity + count);
	}

	// TODO: remove this - ring buffer should only grow by explicit amounts
	template <typename T>
	void grow(RingBuffer<T>& q, size_t count)
	{
		size_t capacity = q.m_capacity + count;

		if(count == 0)
			capacity = 2 * len(q.m_data) + 2;

		setCapacity(q, capacity);
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
	RingBuffer<T>::RingBuffer(Allocator& a)
		: m_data(a)
		, m_capacity(0)
		, m_offset(0)
	{
	}

	// cancel offset overloaded operators for testing
#if 0
	template <typename T>
	T& RingBuffer<T>::operator[](size_t index)
	{
		return m_data[index % len(m_data)];
	}

	template <typename T>
	const T& RingBuffer<T>::operator[](size_t index) const
	{
		return m_data[index % len(m_data)];
	}
#else

	template <typename T>
	T& RingBuffer<T>::operator[](size_t index)
	{
		//const size_t total = m_offset + index;
		//
		//m_offset = total % m_capacity;
		//
		//return m_data[m_offset++];

		return m_data[index % m_capacity];
	}

	template <typename T>
	const T& RingBuffer<T>::operator[](size_t index) const
	{
		//const size_t total = m_offset + index;
		//
		//m_offset = total % m_capacity;
		//
		//return m_data[m_offset++];

		return m_data[index % m_capacity];
	}
#endif
} // end Dunjun

#endif
