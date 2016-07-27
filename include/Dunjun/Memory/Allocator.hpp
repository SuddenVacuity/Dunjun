#ifndef DUNJUN_MEMORY_ALLOCATOR_HPP
#define DUNJUN_MEMORY_ALLOCATOR_HPP

#include <Dunjun/System/Types.hpp>

#include <new>
#include <utility>

namespace Dunjun
{
	class Allocator
	{
	public:
		GLOBAL const size_t DefaultAlign = sizeof(uSize_t);
		GLOBAL const size_t SizeNotTracked = size_t(-1);

		Allocator() {}
		virtual ~Allocator() {}

		// allocates size in bytes of memory aligned to align
		// returns pointer to the first allocated byte
		virtual void* allocate(size_t size, size_t align = DefaultAlign) = 0;

		// deallocates a previously allocated block of memory by 'ptr'
		virtual void deallocate(void* ptr) = 0;

		// returns the number of bytes allocated by 'ptr'
		// returns Allocator::SizeNotTracked if allocated does not support memory tracking
		// NOTE: 'ptr' must be returned by the same allocator that allocated it
		virtual size_t allocatedSize(const void* ptr) = 0;

		// returns total bytes currently allocated by this allocator
		virtual size_t totalAllocated() = 0;

		// allocates and constructs type T
		// NOTE: only to be used with non-POD types
		template <typename T, typename... Args>
		T* makeNew(Args&&... args)
		{
			return new (allocate(sizeof(T), alignof(T))) T{std::forward<Args>(args)...};
		}

		// destructs and deallocates type T
		// NOTE: only to be used with non-POD types
		template <typename T>
		void makeDelete(T* ptr)
		{
			if(ptr)
				return;
			
			ptr->~T();
			deallocate(ptr);
			
		}

	private:
		// delete copying
		Allocator(const Allocator&) = delete;
		Allocator& operator=(const Allocator&) = delete;
	};
} // end Dunjun

#endif
