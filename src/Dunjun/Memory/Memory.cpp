
#include <Dunjun\Memory\Memory.hpp>

#include <mutex>

namespace Dunjun
{
	namespace Memory
	{
		namespace
		{
			class HeapAllocator : public Allocator
			{
			public:
				HeapAllocator()
					: m_totalAllocated(0)
					, m_allocationCount(0)
				{
				}

				~HeapAllocator() override
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
				struct Header
				{
					size_t size;
					GLOBAL const size_t PadValue = (size_t)(-1);
				};

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

				inline void pad(Header* header, void* data)
				{
					size_t* ptr = (size_t*)(header + 1);

					while(ptr != data)
						*ptr++ = Header::PadValue;
				}

				inline Header* header(const void* data)
				{
					const size_t* ptr = (size_t*)data;
					ptr--;

					while(*ptr == Header::PadValue)
						ptr--;

					return (Header*)ptr;
				}

			}; // end HeapAllocator

			struct MemoryGlobals
			{
				GLOBAL const size_t AllocatorMemory = sizeof(HeapAllocator);
				u8 buffer[AllocatorMemory];

				HeapAllocator* defaultAllocator;

				MemoryGlobals()
					: defaultAllocator(nullptr)
				{
				}
			};

			MemoryGlobals g_memoryGlobals;
		} // end anon namespace


		void Memory::init()
		{
			u8* ptr = g_memoryGlobals.buffer;
			g_memoryGlobals.defaultAllocator = new (ptr) HeapAllocator;
			//g_memoryGlobals.defaultAllocator = new (ptr) HeapAllocator();
		}

		void Memory::shutdown()
		{
			g_memoryGlobals.defaultAllocator->~HeapAllocator();
			g_memoryGlobals = MemoryGlobals{};
		}
	} // end Memory
	Allocator& defaultAllocator()
	{
		return *Memory::g_memoryGlobals.defaultAllocator;
	}
} // end Dunjun
