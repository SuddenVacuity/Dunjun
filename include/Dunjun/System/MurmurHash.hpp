#ifndef DUNJUN_SYSTEM_MURMURHASH_HPP
#define DUNJUN_SYSTEM_MURMURHASH_HPP

#include <Dunjun/System/StringFunctions.hpp>

namespace Dunjun
{
	// A hashing function that can be used with a HashMap<T>
	// Source: https://code.google.com/p/smhasher/
	u64 murmurHash64(const void* key, s32 length, u64 seed = 0x9747b28c);

	inline u64 murmurStringHash64(const char* str, u64 seed = 0x9747b28c)
	{
		return murmurHash64(str, strlen(str), seed);
	}

	inline u64 murmurStringHash64(const String& str, u64 seed = 0x9747b28c)
	{
		return murmurHash64(cString(str), (s32)len(str), seed);
	}
} // end Dunjun

#endif
