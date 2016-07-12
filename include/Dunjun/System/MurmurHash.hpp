#ifndef DUNJUN_SYSTEM_MURMURHASH_HPP
#define DUNJUN_SYSTEM_MURMURHASH_HPP

#include <Dunjun/System/Types.hpp>

namespace Dunjun
{
	u64 murmurVoidHash64(const void* key, s32 length, u64 seed = 0x9747b28c);

	inline u64 murmurStringHash64(const char* str, u64 seed = 0x9747b28c)
	{
		return murmurVoidHash64(str, strlen(str), seed);
	}
} // end Dunjun

#endif
