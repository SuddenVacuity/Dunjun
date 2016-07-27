
#include <Dunjun\Memory\Memory.hpp>

#include <mutex>

namespace Dunjun
{
	namespace Memory
	{
		namespace
		{
			struct Header
			{
				size_t size;
				GLOBAL const size_t PadValue = (size_t)(-1);
			};

			inline void pad(Header* header, void* data)
			{
				uSize_t* ptr = (size_t*)(header + 1);

				while (ptr != data)
					*ptr++ = Header::PadValue;
			}

			inline Header* header(const void* data)
			{
				const size_t* ptr = (size_t*)data;
				ptr--;

				while (*ptr == Header::PadValue)
					ptr--;

				return (Header*)ptr;
			}

			class HeapAllocator : public Allocator
			{
			public:
				HeapAllocator()
					: m_totalAllocated(0)
					, m_allocationCount(0)
				{
				}

				~HeapAllocator()
				{

					assert(m_totalAllocated == 0 && m_allocationCount == 0 && "~HeapAllocator() :: allocator not clearing correctly");
				}

				virtual void* allocate(size_t size, size_t align = DefaultAlign) override
				{
					m_mutex.lock();
					defer(m_mutex.unlock());

					const size_t total = size + align + sizeof(Header); // total memory

					Header* header = (Header*)malloc(total);//  malloc total memroy
					header->size = total;

					void* ptr = Memory::alignForward(header + 1, align); // get a pointer to the next point in memory

					pad(header, ptr);

					m_totalAllocated += total;
					m_allocationCount++;

					return ptr;
				}

				virtual void deallocate(void* ptr) override
				{
					m_mutex.lock();
					defer(m_mutex.unlock());

					if(!ptr)
						return;

					Header* h = header(ptr);
						
					m_totalAllocated  -= h->size;
					m_allocationCount--;

					free(h);
				}

				virtual size_t allocatedSize(const void* ptr) override
				{
					m_mutex.lock();
					defer(m_mutex.unlock());

					return  header(ptr)->size;
				}

				virtual size_t totalAllocated() override
				{
					m_mutex.lock();
					defer(m_mutex.unlock());

					return m_totalAllocated;
				}

			private:

				std::mutex m_mutex;
				size_t m_totalAllocated;
				size_t m_allocationCount;

				//inline void fill(Header* header, void* data, size_t size)
				//{
				//	header->size = size;
				//	size_t* ptr = (size_t*)(header + 1);
				//
				//	while(ptr < data)
				//		*ptr++ = Header::PadValue;
				//}
				//
				//inline Header* header(void* data)
				//{
				//	size_t* v = (size_t*)data;
				//
				//	// loop until s is 0
				//	while (*(v - 1) == Header::PadValue)
				//		v--;
				//
				//	return (Header*)v - 1;
				//}


			}; // end HeapAllocator

			// An allocator used to allocate temporary (AKA scratch) memory.
			// ScratchAllocator
			// uses a fixed size ring buffer.
			//
			// NOTE(bill): If the ring buffer is exhausted, the scratch allocator will use
			// its backing allocator to allocate memory instead.

			class ScratchAllocator : public Allocator
			{
			public:
				ScratchAllocator(Allocator& backing, uSize_t size)
					: m_backing(backing)
				{
					m_begin = (u8*)m_backing.allocate(size);
					m_end = m_begin + size;
					m_allocate = m_begin;
					m_free = m_begin;
				}

				~ScratchAllocator()
				{
					assert(m_free == m_allocate);
					m_backing.deallocate(m_begin);
				}

				bool inUse(const void* ptr)
				{
					if(m_allocate == m_allocate)
						return false;

					if(m_allocate > m_free)
						return ptr >= m_free && ptr < m_allocate;

					return ptr >= m_free || ptr < m_allocate;
				}

				virtual void* allocate(uSize_t size, uSize_t align = DefaultAlign)
				{
					assert(align % 4 == 0); // NOTE: Align should be 4 or 8
					size = ((size + 3) / 4) * 4; // Move size to next alignment block

					u8* ptr = m_allocate;
					Header* h = (Header*)ptr;
					u8* data = (u8*)alignForward(h + 1, align);
					ptr = data + size;

					// NOTE: Reached the end of the buffer, wrap around to the
					// beginning
					if(ptr > m_end)
					{
						h->size = (m_end - (u8*)h) | ((uSize_t)(-1));

						ptr = m_begin;
						h = (Header*)ptr;
						data = (u8*)alignForward(h + 1, align);
						ptr = data + size;
					}

					// NOTE: If the buffer is exhausted use the backing allocator
					if(inUse(ptr))
						return m_backing.allocate(size, align);

					h->size = ptr - (u8*)h;
					pad(h, data);

					m_allocate = ptr;

					return data;
				}

				virtual void deallocate(void* ptr)
				{
					if(!ptr)
						return;

					if(ptr < m_begin || ptr >= m_end)
					{
						m_backing.deallocate(ptr);
						return;
					}

					// slot to be free
					Header* h = header(ptr);
					assert((sSize_t)h->size >= 0);
					h->size = h->size | ((uSize_t)(-1));

					// Move the free pointer past all the free slots
					while(m_free != m_allocate)
					{
						Header* h = (Header*)m_free;
						if((sSize_t)(h->size) < 0)
							break;

						// loop back
						m_free += h->size;

						if(m_free == m_end)
							m_free = m_begin;
					}
				}

				virtual uSize_t allocatedSize(const void* ptr)
				{
					Header* h = header(ptr);
					return h->size - ((const u8*)ptr - (const u8*)h);
				}

				virtual uSize_t totalAllocated()
				{
					return m_end - m_begin;
				}

			private:
				Allocator& m_backing;

				u8* m_begin;
				u8* m_end;

				u8* m_allocate;
				u8* m_free;

			}; // end class ScratchAllocator

			struct MemoryGlobals
			{
				GLOBAL const size_t AllocatorMemory = sizeof(HeapAllocator) +
													  sizeof(ScratchAllocator);
				u8 buffer[AllocatorMemory];

				HeapAllocator* defaultAllocator = nullptr;
				ScratchAllocator* defaultScratchAllocator = nullptr;
			};

			MemoryGlobals g_memoryGlobals;
		} // end anon namespace


		void Memory::init()
		{
			u8* ptr = g_memoryGlobals.buffer;

			// set pointer to heap allocator buffer
			g_memoryGlobals.defaultAllocator = new (ptr) HeapAllocator;

			// move to scratch aloocator buffer
			ptr += sizeof(HeapAllocator);

			// set pointer and size of scratch allocator
			g_memoryGlobals.defaultScratchAllocator = new (ptr) 
				ScratchAllocator(*g_memoryGlobals.defaultAllocator, 16 * 1024);

		}

		void Memory::shutdown()
		{
			// destroy allocators in reverse order
			g_memoryGlobals.defaultScratchAllocator->~ScratchAllocator();
			g_memoryGlobals.defaultAllocator->~HeapAllocator();

			// reset pointers to null
			g_memoryGlobals = MemoryGlobals{};
		}
	} // end Memory

	Allocator& defaultAllocator()
	{
		return *Memory::g_memoryGlobals.defaultAllocator;
	}

	Allocator& defaultScratchAllocator()
	{
		return *Memory::g_memoryGlobals.defaultScratchAllocator;
	}
} // end Dunjun
