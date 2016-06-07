#ifndef DUNJUN_TYPES_HPP
#define DUNJUN_TYPES_HPP

#include <Dunjun/Config.hpp>


namespace Dunjun
{
	using s8  = signed char;
	using u8  = unsigned char;
	using s16 = signed short;
	using u16 = unsigned short;
	using s32 = signed int;
	using u32 = unsigned int;

#if defined(_MSC_VER)
	using s64 = signed __int64;
	using u64 = unsigned __int64;
#else
	using s64 = signed long long;
	using u64 = unsigned long long;
#endif

	using f32 = float;
	using f64 = double;

	using b8 = bool;
	using b32 = s32; // define this as 32 bit for it to be dword alignde
}


#endif
