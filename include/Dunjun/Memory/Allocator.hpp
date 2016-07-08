#ifndef DUNJUN_MEMORY_ALLOCATOR_HPP
#define DUNJUN_MEMORY_ALLOCATOR_HPP

#include <Dunjun/Common.hpp>

#include <new>
#include <utility>

namespace Dunjun
{
	class Allocator
	{
	public:
		GLOBAL const size_t DefaultAlign = 4;
		GLOBAL const size_t MaxSize = size_t(-1);

		Allocator() {}
		virtual ~Allocator() {}

		virtual void* allocate(size_t size, size_t align = DefaultAlign) = 0;
		virtual void deallocate(void* ptr) = 0;
		virtual size_t allocatedSize(void* ptr) = 0;

		template <typename T, typename... Args>
		T* makeNew(Args&&... args)
		{
			return new (allocate(sizeof(T), alignof(T))) T{std::forward<Args>(args)...};
		}

		template <typename T>
		void makeDelete(T* ptr)
		{
			if(ptr)
			{
				ptr->~T();
				deallocate(ptr);
			}
		}

	private:
		Allocator(const Allocator&) = delete;
		Allocator& operator=(const Allocator&) = delete;
	};
} // end Dunjun

#endif
