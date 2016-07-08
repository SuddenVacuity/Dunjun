#ifndef DUNJUN_MEMORY_MEMORY_HPP
#define DUNJUN_MEMORY_MEMORY_HPP

#include <Dunjun/Memory/Allocator.hpp>

namespace Dunjun
{
	Allocator& defaultAllocator();

	namespace Memory
	{

		void init();
		void shutdown();

		inline void* alignForward(void* ptr, size_t align)
		{
			uintptr p = (uintptr)ptr;
			const size_t modulo = p % align;
			if(modulo)
				p += (uintptr)(align - modulo);

			return (void*)p;
		}
	} // end Memory
} // end Dunjun

#endif
