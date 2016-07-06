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
		Allocator::SizeType m_length;
		Allocator::SizeType m_capacity;
		T* m_data;

		Array(Allocator& a);
		~Array();

		Array(const Array& other);

		Array& operator=(const Array& other);

		inline T& operator[](Allocator::SizeType index);
		inline const T& operator[](Allocator::SizeType index) const;
	}; // end Array

	template <typename T>
	struct Queue
	{
		Array<T> m_data;
		Allocator::SizeType m_length;
		Allocator::SizeType m_offset;

		Queue(Allocator& a);

		T& operator[](Allocator::SizeType index);
		const T& operator[](Allocator::SizeType index) const;
	}; // end Queue

} // end Dunjun

#endif
