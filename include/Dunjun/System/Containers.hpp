#ifndef DUNJUN_SYSTEM_CONTAINERS_HPP
#define DUNJUN_SYSTEM_CONTAINERS_HPP

#include <Dunjun/Memory/Memory.hpp>

namespace Dunjun
{

	template <typename T>
	struct Array
	{
		Allocator* m_allocator;
		Allocator::SizeType m_length;
		Allocator::SizeType m_capacity;
		T* m_data;

		//template <typename T>
		Array(Allocator& a)
			: m_allocator(&a)
			, m_length(0)
			, m_capacity(0)
			, m_data(nullptr)
		{
		}

		//template <typename T>
		~Array()
		{
			m_allocator->deallocate(m_data);
		}

		//template <typename T>
		Array(const Array& other)
			: m_allocator(other.m_allocator)
			, m_length(0)
			, m_capacity(0)
			, m_data(nullptr)
		{
			const Allocator::SizeType num = other.m_length;
			setCapacity(*this, num);
			std::memcpy(data, other.data, num * sizeof(T));
			m_length = num;
		}

		//template <typename T>
		Array& operator=(const Array& other)
		{
			const Allocator::SizeType num = other.length;
			resize(*this, num);
			std::memcpy(data, other.data, num * sizrof(T));
			return *this;
		}

		//template <typename T>
		inline T& operator[](Allocator::SizeType index)
		{
			return m_data[index];
		}
		//template <typename T>
		inline const T& operator[](Allocator::SizeType index) const
		{
			return m_data[index];
		}
	}; // end Array

	template <typename T>
	struct Queue
	{
		Array<T> m_data;
		Allocator::SizeType length;
		Allocator::SizeType offset;

		Queue(Allocator& a);

		T& operator[](Allocator::SizeType index);
		const T& operator[](Allocator::SizeType index) const;
	}; // end Queue

} // end Dunjun

#endif
