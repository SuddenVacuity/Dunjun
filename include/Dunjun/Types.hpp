#ifndef DUNJUN_TYPES_HPP
#define DUNJUN_TYPES_HPP

#include <cstdint>
#include <cstdlib>

namespace Dunjun
{
	using Int8  = std::int8_t;
	using Int16 = std::int16_t;
	using Int32 = std::int32_t;
	using Int64 = std::int64_t;

	using uInt8  = std::uint8_t;
	using uInt16 = std::uint16_t;
	using uInt32 = std::uint32_t;
	using uInt64 = std::uint64_t;

	using Float32 = float;
	using Float64 = double;

	using Size = std::size_t;

	using uIntPtr = std::uintptr_t;
	using IntPtr = std::intptr_t;
}


#endif