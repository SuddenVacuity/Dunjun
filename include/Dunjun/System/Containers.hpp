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
		size_t m_capacity;
		size_t m_length;
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
)
)				 Ring buffer for POD types  
)				 does not construct items	
)
)				.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

	template <typename T>
	struct Queue
	{
		Array<T> m_data;
		size_t m_capacity;
		size_t m_offset;

		Queue(Allocator& a);

		T& operator[](size_t index);
		const T& operator[](size_t index) const;
	}; // end Queue

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
	};

} // end Dunjun

#endif