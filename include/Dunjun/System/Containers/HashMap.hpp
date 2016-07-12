#ifndef DUNJUN_SYSTEM_HASHMAP_HPP
#define DUNJUN_SYSTEM_HASHMAP_HPP

#include <Dunjun/System/Containers.hpp>
#include <Dunjun/System/Containers/Array.hpp>

namespace Dunjun
{
	// check if key exists in hash
	template <typename T>
	bool has(const HashMap<T> u, u64 key);

	// returns stored value for the key
	//template <typename T>
	//const T& get(const HashMap<T>& h, u64 key);
	// returns stored value for the key - return a default value if key does not exist
	template <typename T>
	const T& get(const HashMap<T>& h, u64 key, const T& defaultValue);

	// set value for the specified key
	template <typename T>
	void set(HashMap<T>& h, u64 key, const T& value);

	// removes the key from the hashmap if it exists
	template <typename T>
	void remove(HashMap<T>& h, u64 key);

	// reserve space for the hash lookup table
	template <typename T>
	void reserve(HashMap<T>& h, size_t capacity);

	// remove all entries and hashes from the hashmap
	template <typename T>
	void clear(HashMap<T>& h);

	// iterators
	template <typename T>
	const typename HashMap<T>::Entry* begin(const HashMap<T>& h);
	template <typename T>
	const typename HashMap<T>::Entry* end(const HashMap<T>& h);


	namespace MultiHash
	{
		// returns all entries in array with the corresponding key
		template <typename T>
		void get(const HashMap<T>& h, u64 key, Array<T>& items);

		// returns the number of entries using that key
		template <typename T>
		size_t count(const HashMap<T>& h, u64 key);

		// removes the entry from the hashmap if it exists
		template <typename T>
		void remove(HashMap<T>& h, const typename HashMap<T>::Entry* e);
		// removes the entries with the key
		template <typename T>
		void removeAll(HashMap<T>& h, u64 key);

		// inserts the value as another value for the set key
		template <typename T>
		void insert(HashMap<T>& h, u64 key, const T& value);

		// 
		template <typename T>
		const typename HashMap<T>::Entry* findFirst(const HashMap<T>& h, u64 key);

		// 
		template <typename T>
		const typename HashMap<T>::Entry* findNext(const HashMap<T>& h, const typename HashMap<T>::Entry* e);






	} // end MultiHashMap


	/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	)				.
	)					.
	)
	)				.
	)					.
	)
	)				.
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

	namespace Impl
	{
		GLOBAL const size_t EndOfList = (size_t)(-1);

		struct FindResult
		{
			size_t hashIndex;
			size_t dataPrev;
			size_t dataIndex;
		};

		template <typename T>
		size_t addEntry(HashMap<T>& h, u64 key);
		template <typename T>
		void erase(HashMap<T>& h, const FindResult fr);
		template <typename T>
		FindResult find(const HashMap<T>& h, u64 key);
		template <typename T>
		FindResult find(const HashMap<T>& h, const typename HashMap<T>::Entry* e);
		template <typename T>
		size_t make(HashMap<T>& h, u64 key);
		template <typename T>
		void findAndErase(HashMap<T>& h, u64 key);
		template <typename T>
		size_t findOrFail(const HashMap<T>& h, u64 key);
		template <typename T>
		size_t findOrMake(HashMap<T>& h, u64 key);
		template <typename T>
		void rehash(HashMap<T>& h, size_t capacity);
		template <typename T>
		void grow(HashMap<T>& h);
		template <typename T>
		bool full(HashMap<T>& h);

		//////////////////////

		template <typename T>
		size_t addEntry(HashMap<T>& h, u64 key)
		{
			typename HashMap<T>::Entry e;

			e.key = key;
			e.next = EndOfList;

			size_t eIndex = len(h.data);
			pushBack(h.data, e);

			return eIndex;
		}

		template <typename T>
		void erase(HashMap<T>& h, const FindResult fr)
		{
			if(fr.dataPrev == EndOfList)
				h.hashes[fr.hashIndex] = h.data[fr.dataIndex].next;
			else
				h.data[fr.dataPrev].next = h.data[fr.dataIndex].next;

			popBack(h.data);

			if(fr.dataIndex == len(h.data))
				return;

			h.data[fr.dataIndex] = h.data[len(h.data)];

			FindResult last = find(h, h.data[fr.dataIndex].key);

			if(last.dataPrev == EndOfList)
				h.hashes[last.hashIndex] = fr.dataIndex;
			else
				h.data[last.hashIndex].next = fr.dataIndex;
		}

		template <typename T>
		FindResult find(const HashMap<T>& h, u64 key)
		{
			FindResult result;

			result.hashIndex = EndOfList;
			result.dataPrev = EndOfList;
			result.dataIndex = EndOfList;

			if (len(h.hashes) == 0)
				return result;

			result.hashIndex = key % len(h.hashes);
			result.dataIndex = h.hashes[result.hashIndex];

			while (result.dataIndex != EndOfList)
			{
				if (h.data[result.dataIndex].key == key)
					return result;

				result.dataPrev = result.dataIndex;
				result.dataIndex = h.data[result.dataIndex].next;
			}

			return result;
		}

		template <typename T>
		FindResult find(const HashMap<T>& h, const typename HashMap<T>::Entry* e)
		{
			FindResult result;

			result.hashIndex = EndOfList;
			result.dataPrev = EndOfList;
			result.dataIndex = EndOfList;

			if (len(h.hashes) == 0 || !e)
				return result;

			result.hashIndex = e->key % len(h.hashes);
			result.dataIndex = h.hashes[result.hashIndex];

			while (result.dataIndex != EndOfList)
			{
				if (&h.data[result.dataIndex] == e)
					return result;

				result.dataPrev = result.dataIndex;
				result.dataIndex = h.data[result.dataIndex].next;
			}

			return result;
		}

		template <typename T>
		size_t make(HashMap<T>& h, u64 key)
		{
			const FindResult fr = find(h, key);
			const size_t index = addEntry(h, key);

			if(fr.dataPrev == EndOfList)
				h.hashes[fr.hashIndex] = index;
			else
				h.data[fr.dataPrev].next = index;

			h.data[index].next = fr.dataIndex;
			
			return index;
		}

		template <typename T>
		void findAndErase(HashMap<T>& h, u64 key)
		{
			const FindResult fr = find(h, key);

			if(fr.dataIndex != EndOfList)
				erase(h, fr);
		}


		template <typename T>
		size_t findOrFail(const HashMap<T>& h, u64 key)
		{
			return find(h, key).dataIndex;
		}

		template <typename T>
		size_t findOrMake(HashMap<T>& h, u64 key)
		{
			const FindResult result = find(h, key);
			if(result.dataIndex != EndOfList)
				return result.dataIndex;

			 size_t index = addEntry(h, key);

			 if(result.dataPrev == EndOfList)
				 h.hashes[result.hashIndex] = index;
			 else
				 h.data[result.dataPrev].next = index;

			return index;
		}

		template <typename T>
		void rehash(HashMap<T>& h, size_t capacity)
		{
			HashMap<T> newHash = *h.hashes.m_allocator;

			resize(newHash.hashes, capacity);
			const size_t oldLength = len(h.data);

			reserve(newHash.data, oldLength);

			for(size_t i = 0; i < capacity; i++)
				newHash.hashes[i] = EndOfList;

			for (size_t i = 0; i < oldLength; i++)
			{
				auto& e = h.data[i];
				MultiHash::insert(newHash, e.key, e.value);
			}

			HashMap<T> empty(*h.hashes.m_allocator);

			h.~HashMap<T>();

			std::memcpy(&h, &newHash, sizeof(HashMap<T>));
			std::memcpy(&newHash, &empty, sizeof(HashMap<T>));
		}

		template <typename T>
		void grow(HashMap<T>& h)
		{
			const size_t newCapacity = len(h.data) * 2 + 2;
			rehash(h, newCapacity);
		}

		template <typename T>
		bool full(HashMap<T>& h)
		{
			const f32 maximumLoadCoefficient = 0.75f;
			return len(h.data) >= maximumLoadCoefficient * len(h.hashes);
		}

		} // end Impl

		/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		)				.
		)					.
		)
		)				.
		)					.
		)
		)				.
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/


	template <typename T>
	inline bool has(const HashMap<T> h, u64 key)
	{
		return Impl::findOrFail(h, key) != Impl::EndOfList;
	}

	//template <typename T>
	//const T& get(const HashMap<T>& h, u64 key)
	//{
	//	const size_t index = Impl::findOrFail(h, key);
	//	assert(index != Impl::EndOfList && "HashMap::get() index does not exist and no default value was specified");
	//
	//	return h.data[index].value;
	//}

	template <typename T>
	inline const T& get(const HashMap<T>& h, u64 key, const T& defaultValue)
	{
		const size_t index = Impl::findOrFail(h, key);

		if(index == Impl::EndOfList)
			return defaultValue;

		return h.data[index].value;
	}

	template <typename T>
	inline void set(HashMap<T>& h, u64 key, const T& value)
	{
		if(len(h.hashes) == 0)
			Impl::grow(h);

		const size_t index = Impl::findOrMake(h, key);

		h.data[index].value = value;

		if(Impl::full(h))
			Impl::grow(h);

	}

	template <typename T>
	inline void remove(HashMap<T>& h, u64 key)
	{
		Impl::findAndErase(h, key);
	}

	template <typename T>
	inline void reserve(HashMap<T>& h, size_t capacity)
	{
		Impl::rehash(h, capacity);
	}

	template <typename T>
	inline void clear(HashMap<T>& h)
	{
		clear(h.hashes);
		clear(h.data);
	}

	template <typename T>
	inline const typename HashMap<T>::Entry* begin(const HashMap<T>& h)
	{
		return begin(h.data);
	}
	template <typename T>
	inline const typename HashMap<T>::Entry* end(const HashMap<T>& h)
	{
		return end(h.data);
	}

	/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	)				.
	)					.
	)
	)				.
	)					.
	)
	)				.
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

	namespace MultiHash
	{
		template <typename T>
		inline void get(const HashMap<T>& h, u64 key, Array<T>& items)
		{
			auto e = MultiHash::findFirst(h, key);

			while (e)
			{
				pushBack(items, e->value);
				e = MultiHash::findNext(h, e);
			}
		}

		template <typename T>
		inline size_t count(const HashMap<T>& h, u64 key)
		{
			size_t c = 0;
			auto e = MultiHash::findFirst(h, key);

			while (e)
			{
				c++;
				e = MultiHash::findNext(h, e);
			}

			return c;
		}

		template <typename T>
		void remove(HashMap<T>& h, const typename HashMap<T>::Entry* e)
		{
			const Impl::FindResult fr = Impl::find(h, e);

			if (fr.dataIndex != Impl::EndOfList)
				Impl::erase(h, fr);
		}

		template <typename T>
		inline void removeAll(HashMap<T>& h, u64 key)
		{
			while (has(h, key))
				remove(h, key);
		}

		template <typename T>
		inline void insert(HashMap<T>& h, u64 key, const T& value)
		{
			if (len(h.hashes) == 0)
				Impl::grow(h);

			const size_t next = Impl::make(h, key);
			h.data[next].value = value;

			if (Impl::full(h))
				Impl::grow(h);

		}

		template <typename T>
		inline const typename HashMap<T>::Entry* findFirst(const HashMap<T>& h, u64 key)
		{
			const size_t index = Impl::findOrFail(h, key);
			if (index == Impl::EndOfList)
				return nullptr;

			return &h.data[index];
		}

		template <typename T>
		inline const typename HashMap<T>::Entry* findNext(const HashMap<T>& h, const typename HashMap<T>::Entry* e)
		{
			if (!e)
				return nullptr;

			size_t index = e->next;

			while (index != Impl::EndOfList)
			{
				if (h.data[index].key == e->key)
					return &h.data[index];

				index = h.data[index].next;
			}

			return nullptr;
		}


	} // end MultiHashMap

	/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	)				.
	)					.
	)
	)				.
	)					.
	)
	)				.
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

	template <typename T>
	HashMap<T>::HashMap(Allocator& a)
		: hashes(a)
		, data(a)
	{
	}

} // end Dunjun

#endif
