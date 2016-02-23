#ifndef DUNJUN_TYPES_HPP
#define DUNJUN_TYPES_HPP

#include <cstdint>
#include <cstdlib>

namespace Dunjun
{
	using i8  = std::int8_t;
	using i16 = std::int16_t;
	using i32 = std::int32_t;
	using i64 = std::int64_t;

	using u8  = std::uint8_t;
	using u16 = std::uint16_t;
	using u32 = std::uint32_t;
	using u64 = std::uint64_t;

	using f32 = float;
	using f64 = double;

	using b8 = bool;
	using b32 = i32; // define this as 32 bit for it to be dword alignde
}


#endif