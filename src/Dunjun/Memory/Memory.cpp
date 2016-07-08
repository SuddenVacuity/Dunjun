
#include <Dunjun\Memory\Memory.hpp>

namespace Dunjun
{

	struct Header
	{
		GLOBAL const size_t PadValue = Allocator::MaxSize;
		size_t size;
	};

	INTERNAL inline void fill(Header* header, void* data, size_t size)
	{
		header->size = size;
		size_t* ptr = (size_t*)(header + 1);

		while(ptr < data)
			*ptr++ = Header::PadValue;
	}

	INTERNAL inline Header* header(void* data)
	{
		size_t* s = (size_t*)data;

		// loop until s is 0
		while (*(s - 1) == Header::PadValue)
			s--;

		return (Header*)s - 1;
	}

	class HeapAllocator : public Allocator
	{
	public:
		HeapAllocator()
			: m_totalAllocated(0)
		{}

		~HeapAllocator() {}

		virtual void* allocate(size_t size, size_t align = DefaultAlign)
		{
			const size_t total = size + align + sizeof(Header); // total memory
			Header* header = (Header*)malloc(total);//  malloc total memroy
			void* ptr = header + 1; // get a pointer to the next point in memory
			ptr = Memory::alignForward(ptr, align); // make new pointer aligned with memory

			fill(header, ptr, total);

			return ptr;
		}

		virtual void deallocate(void* ptr)
		{
			if(!ptr)
				return;

			Header* h = header(ptr);
				
			m_totalAllocated  -= h->size;

			free(h);
		}

		virtual size_t allocatedSize(void* ptr)
		{
			return  header(ptr)->size;
		}

	private:
		size_t m_totalAllocated;

	}; // end HeapAllocator

	struct MemoryGlobals
	{
		GLOBAL const usize AllocatorMemroy = sizeof(HeapAllocator);
		u8 buffer[AllocatorMemroy];

		HeapAllocator* defaultAllocator;

		MemoryGlobals()
			: defaultAllocator(nullptr)
		{
		}
	};

	namespace
	{
		MemoryGlobals g_memoryGlobals;
	} // end anon namespace

	Allocator& defaultAllocator()
	{
		return *g_memoryGlobals.defaultAllocator;
	}

	void Memory::init()
	{
		u8* ptr = g_memoryGlobals.buffer;
		g_memoryGlobals.defaultAllocator = new (ptr) HeapAllocator();
	}

	void Memory::shutdown()
	{
		g_memoryGlobals.defaultAllocator->~HeapAllocator();
		g_memoryGlobals = MemoryGlobals();
	}

} // end Dunjun
