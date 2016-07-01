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
		using SizeType = usize;

		GLOBAL const SizeType DefaultAlign = 4;
		GLOBAL const SizeType MaxSize = SizeType(-1);

		Allocator() {}
		virtual ~Allocator() {}

		virtual void* allocate(SizeType size, SizeType align = DefaultAlign) = 0;
		virtual void deallocate(void* ptr) = 0;
		virtual SizeType allocatedSize(void* ptr) = 0;

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
