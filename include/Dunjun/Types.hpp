#ifndef DUNJUN_TYPES_HPP
#define DUNJUN_TYPES_HPP

#include <Dunjun/Common.hpp>


namespace Dunjun
{
	using s8  = std::int8_t;
	using s16 = std::int16_t;
	using s32 = std::int32_t;
	using s64 = std::int64_t;

	using u8  = std::uint8_t;
	using u16 = std::uint16_t;
	using u32 = std::uint32_t;
	using u64 = std::uint64_t;

	using f32 = float;
	using f64 = double;

	using b8 = bool;
	using b32 = s32; // define this as 32 bit for it to be dword alignde
}


#endif
