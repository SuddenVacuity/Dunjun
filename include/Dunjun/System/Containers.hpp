#ifndef DUNJUN_SYSTEM_CONTAINERS_HPP
#define DUNJUN_SYSTEM_CONTAINERS_HPP

#include <Dunjun/Memory/Memory.hpp>

namespace Dunjun
{
	// Dynamic array for POD types
	// does not construct items
	template <typename T>
	struct Array
	{
		Allocator* m_allocator;
		size_t m_length;
		size_t m_capacity;
		T* m_data;

		Array(Allocator& a);
		~Array();

		Array(const Array& other);

		Array& operator=(const Array& other);

		inline T& operator[](size_t index);
		inline const T& operator[](size_t index) const;
	}; // end Array

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
)				.
)					QUEUE
)				 Ring buffer for POD types
)				 adds items to the front or back of list and icreases array size to match
)				 can accept array positions greater than the array 
)				 loops around to the front is position is bigger than array
)				.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

	template <typename T>
	struct Queue
	{
		Array<T> m_data;
		size_t m_capacity;
		size_t m_offset;
	
		Queue(Allocator& a);
		~Queue() = default;
	
		T& operator[](size_t index);
		const T& operator[](size_t index) const;
	}; // end Queue

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
)				.
)					RingBuffer
)				 Ring buffer for POD types
)				 adds items to the front or back of list fixed array size - adding more overwrites old values
)				 can accept array positions greater than the array 
)				 loops around to the front is position is bigger than array
)				.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

	template <typename T>
	struct RingBuffer
	{
		Array<T> m_data;
		size_t m_capacity;
		size_t m_offset;
	
		RingBuffer(Allocator& a);

		T& operator[](size_t index);
		const T& operator[](size_t index) const;
	}; // end RingBuffer

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
)				.
)					HASH MAP
)
)				.
)					.
)
)				.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

	template <typename T>
	struct HashMap
	{
		struct Entry
		{
			u64 key;
			size_t next;
			T value;
		};

		Array<size_t> hashes;
		Array<Entry> data;

		HashMap(Allocator& a);
		~HashMap() = default;
	};

} // end Dunjun

#endif
