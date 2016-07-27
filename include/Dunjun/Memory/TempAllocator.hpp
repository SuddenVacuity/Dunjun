#ifndef DUNJUN_MEMORY_TEMPALLOCATOR_HPP
#define DUNJUN_MEMORY_TEMPALLOCATOR_HPP

#include <Dunjun/Memory/Memory.hpp>

namespace Dunjun
{
	template <size_t Size>
	class TempAllocator : public Allocator
	{
	public:
		TempAllocator(Allocator& backing = defaultScratchAllocator());
		~TempAllocator();

		void* allocate(size_t size, size_t align = DefaultAlign);
		void deallocate(void* /*ptr*/) {}
		size_t allocatedSize(const void* /*ptr*/) { return SizeNotTracked; }
		size_t totalAllocated() { return SizeNotTracked; }


	private:
		Allocator& m_backing;

		u8* m_begin;
		u8* m_end;
		u8* m_current;

		size_t m_chunkSize;

		u8 m_buffer[Size];

	}; // end TempAllocator

	// NOTE: Please use on the premade sizes for the TempAllocator<> to reduce
	// unneeded template instantiation. If you need more than 4K, do _not_ use a
	// TempAllocator! Try using the `defaultScratchAllocator()` or just the
	// `defaultAllocator()`.
	using TempAllocator64   = TempAllocator<64>;
	using TempAllocator128  = TempAllocator<128>;
	using TempAllocator256  = TempAllocator<256>;
	using TempAllocator512  = TempAllocator<512>;
	using TempAllocator1024 = TempAllocator<1024>;
	using TempAllocator2048 = TempAllocator<2048>;
	using TempAllocator4096 = TempAllocator<4096>;

	template <size_t Size>
	inline TempAllocator<Size>::TempAllocator(Allocator& backing)
		: m_backing(backing)
		, m_chunkSize(4096)
	{
		m_current = m_begin = &m_buffer[0];
		m_end = m_begin + Size;
		*(void**)m_begin = nullptr;
		m_current += sizeof(void*);
	}

	template <size_t Size>
	inline TempAllocator<Size>::~TempAllocator()
	{
		void* ptr = *(void**)m_buffer;
		while(ptr)
		{
			void* nextPtr = *(void**)ptr;
			m_backing.deallocate(ptr);
			ptr = nextPtr;
		}
	}


	template <size_t Size>
	inline void* TempAllocator<Size>::allocate(size_t size, size_t align)
	{
		m_current = (u8*)Memory::alignForward(m_current, align);

		if(size > (size_t)(m_end - m_current))
		{
			size_t toAllocate = size + align + sizeof(void*);

			if(toAllocate < m_chunkSize)
				toAllocate = m_chunkSize;

			m_chunkSize *= 2;

			void* ptr = m_backing.allocate(toAllocate);
			*(void**)m_begin = ptr;

			m_current = m_begin = (u8*)ptr;
			m_end = m_begin + toAllocate;

			*(void**)m_begin = nullptr;

			m_current += sizeof(void*);
			m_current = (u8*)Memory::alignForward(m_current, align);
		}

		void* result = m_current;
		m_current += size;

		return result;

	}














} // end Dunjun

#endif
