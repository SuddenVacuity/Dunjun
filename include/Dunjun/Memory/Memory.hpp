#ifndef DUNJUN_MEMORY_MEMORY_HPP
#define DUNJUN_MEMORY_MEMORY_HPP

#include <Dunjun/Memory/Allocator.hpp>

namespace Dunjun
{
	Allocator& defaultAllocator();
	Allocator& defaultScatchAllocator();

	namespace Memory
	{
		// assigns pointer for memoryGlobals
		// and creates HeapAllocator for memory
		void init();

		// checks that all allocated memory has been deallocated
		// resets memoryGlobals
		void shutdown();

		// `alignForward` returns the pointer `ptr` aligned to the desited `align` byte
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
