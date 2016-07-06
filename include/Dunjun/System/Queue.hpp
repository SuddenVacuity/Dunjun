#ifndef DUNJUN_SYSTEM_QUEUE_HPP
#define DUNJUN_SYSTEM_QUEUE_HPP

#include <Dunjun/System/Containers.hpp>

namespace Dunjun
{
	// Appends an item to the end of the queue and changes queue length to match
	// expands the queue if needed
	template <typename T>
	Allocator::SizeType pushBack(Queue<T>& q, const T& item);
	// Removes the last item in the queue
	template <typename T>
	void popBack(Queue<T>& q);

	// Appends an item to the front of the queue and changes queue length to match
	// expands the queue if needed
	template <typename T>
	Allocator::SizeType pushFront(Queue<T>& q, const T& item);
	// Removes the first item in the queue
	template <typename T>
	void popFront(Queue<T>& q);

	// Appends a set of items to the end of the queue and changes length to match
	// expands the queue if needed
	template <typename T>
	Allocator::SizeType push(Queue<T>& q, const T* items, Allocator::SizeType count);
	// Removes a set of items from the end of the queue and changes length to match
	// expands the queue if needed
	template <typename T>
	void pop(Queue<T>& q, Allocator::SizeType count);

	// Returns the number of elements in the queue
	template <typename T>
	Allocator::SizeType len(const Queue<T>& q);
	// Returns number of spaces remaining in queue
	template <typename T>
	Allocator::SizeType space(const Queue<T>& q);
	// Returns if the queue is empty
	template <typename T>
	bool isEmpty(const Queue<T>& q);

	// Iterator :: Returns a pointer to where the queue begins
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
	//void resize(Queue<T>& q, Allocator::SizeType length);
	// Sets queue capacity to capacity
	// Allocates or deallocates memory as needed
	// and sets capacity to match
	template <typename T>
	void setCapacity(Queue<T>& q, Allocator::SizeType capacity);
	// Allocates number of spaces equal to amount
	// and sets capacity to match
	template <typename T>
	void reserve(Queue<T>& q, Allocator::SizeType count);

	// increases queue capacity to (current * 2 + 2)
	template <typename T>
	void grow(Queue<T>& q, Allocator::SizeType minCapacity = 0);

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
	Allocator::SizeType pushBack(Queue<T>& q, const T& item)
	{
		if(space(q) == 0)
			grow(q);

		q[q.m_length] = item;

		q.m_length++;
		q.m_offset++;

		return q.m_length;
	}

	template <typename T>
	void popBack(Queue<T>& q)
	{
		assert(q.m_length > 0 && "Queue<T> Queue must be greater than 0");

		q.m_length--;
		q.m_offset--;
	}

	////////////////////////////////////
	template <typename T>
	Allocator::SizeType pushFront(Queue<T>& q, const T& item)
	{
		if(space(q) == 0)
			grow(q);

		q.m_offset = (q.m_offset - 1 + len(q.m_data)) % len(q.m_data);

		q[0] = item;

		q.m_length++;

		return q.m_length;
	}

	template <typename T>
	void popFront(Queue<T>& q)
	{
		assert(q.m_length > 0 && "Queue<T> Queue must be greater than 0");

		q.m_offset = (q.m_offset + 1) % len(q.m_data);
		q.m_length--;
	}

	////////////////////////////////////
	template <typename T>
	Allocator::SizeType push(Queue<T>& q, const T* items, Allocator::SizeType count)
	{
	
		if(space(q) < count)
			grow(q, q.m_length + count);

		const Allocator::SizeType length = len(q.m_data);

		// find current offset
		const Allocator::SizeType insert = (q.m_offset + q.m_length) % length;

		// count of items used to find end of data to be added
		Allocator::SizeType toInsert = count;

		if(insert + toInsert + 1 > length)
			toInsert = length - insert;

		std::memcpy(begin(q.m_data) + insert, items, toInsert * sizeof(T));

		q.m_length += toInsert;

		// wrap around to beginning
		items += toInsert;
		count -= toInsert;

		std::memcpy(begin(q.m_data), items, count * sizeof(T));

		q.m_length += count;

		return q.m_length;
		//get 1 4 9 1 2 3 //expect 4 5 6 7 8 9
	}
	template <typename T>
	void pop(Queue<T>& q, Allocator::SizeType count)
	{
		assert(q.m_length > count && "Queue<T>::pop() - Count cannot be greater than queue length." );

		q.m_offset = (q.m_offset + count) % len(q.m_data);
		q.m_length -= count;
	}

	template <typename T>
	inline Allocator::SizeType len(const Queue<T>& q)
	{
		return q.m_length;
	}

	template <typename T>
	Allocator::SizeType space(const Queue<T>& q)
	{
		return len(q.m_data) - q.m_length;
	}

	template <typename T>
	inline bool isEmpty(const Queue<T>& q)
	{
		return q.m_length == 0;
	}

	////////////////////////////////////
	template <typename T>
	inline T* begin(Queue<T>& q)
	{
		return begin(q.m_data) - q.m_length;
	}

	template <typename T>
	inline const T* begin(const Queue<T>& q)
	{
		return begin(q.m_data) - q.m_length;
	}

	template <typename T>
	T* end(Queue<T>& q)
	{
		const Allocator::SizeType end = q.m_offset + q.m_length;

		return !(end < len(q.m_data)) ? end(q.m_data)
									  : begin(q.m_data) + end;
	}

	template <typename T>
	const T* end(const Queue<T>& q)
	{
		const Allocator::SizeType end = q.m_offset + q.m_length;

		return !(end < len(q.m_data)) ? end(q.m_data)
									  : begin(q.m_data) + end;
	}


	////////////////////////////////////
	template <typename T>
	T& front(Queue<T>& q)
	{
		assert(q.m_length > 0 && "Queue<T> Queue[0] does not exist.");
		return q[0];
	}

	template <typename T>
	const T* front(const Queue<T>& q)
	{
		assert(q.m_length > 0 && "Queue<T> Queue[0] does not exist.");
		return q[0];
	}

	template <typename T>
	T& back(Queue<T>& q)
	{
		assert(q.m_length > 0 && "Queue<T> Queue has no members");
		return q[q.m_length - 1];
	}

	template <typename T>
	const T* back(const Queue<T>& q)
	{
		assert(q.m_length > 0 && "Queue<T> Queue has no members");
		return q[q.m_length - 1];
	}

	////////////////////////////////////
	template <typename T>
	void clear(Queue<T>& q)
	{
		q.m_offset = 0;
		q.m_length = 0;
	}

	////////////////////////////////////
	//template <typename T>
	//void resize(Queue<T>& q, Allocator::SizeType length)
	//{
	//
	//}

	template <typename T>
	void setCapacity(Queue<T>& q, Allocator::SizeType capacity)
	{
		const Allocator::SizeType oldLength = len(q.m_data);

		resize(q.m_data, capacity);

		if(oldLength < q.m_offset + q.m_length)
		{
			std::memmove(begin(q.m_data) + capacity - (oldLength - q.m_offset),
						 begin(q.m_data) + q.m_offset,
						 (oldLength - q.m_offset) * sizeof(T));

			q.m_offset += capacity - oldLength;
		}

	}

	template <typename T>
	void reserve(Queue<T>& q, Allocator::SizeType count)
	{
		assert(count > 0 && "Queue<T>::reserve - count must be greater than 0.");

		setCapacity(q, q.m_length + count);
	}

	template <typename T>
	void grow(Queue<T>& q, Allocator::SizeType minCapacity)
	{
		Allocator::SizeType newCapacity = 2 * len(q.m_data) + 2;

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
		, m_length(0)
		, m_offset(0)
	{
	}

	template <typename T>
	T& Queue<T>::operator[](Allocator::SizeType index)
	{
		return m_data[(m_offset + index) % len(m_data)];
	}

	template <typename T>
	const T& Queue<T>::operator[](Allocator::SizeType index) const
	{
		return m_data[(m_offset + index) % len(m_data)];
	}
} // end Dunjun

#endif
