#ifndef DUNJUN_SYSTEM_STRINGCONVERSIONS_HPP
#define DUNJUN_SYSTEM_STRINGCONVERSIONS_HPP

#include <Dunjun/System/StringFunctions.hpp>

namespace Dunjun
{
	namespace StrConv
	{
		// formatFloat converts floating point number f to a string
		// format types: 'b' == (-ddddp+-ddd) binary exponent
		//				 'e' == (-d.dddde+-dd) decinmal exponent
		//				 'f' == (-ddd.ddd) no exponent
		//				 'g' == '' for large exponents
		String formatFloat(f64 f, char format,  u32 precision, u32 bitsize);

		String formatBool(bool b);

		String formatSint(s64 s, u32 base);
		String formatUint(u64 u, u32 base);

		bool parseFloat(String str, f32& f);
		bool parseFloat(String str, f64& f);

		bool parseBool(String str, bool& b);

		bool parseSint(String str, u32 base, s8& s);
		bool parseSint(String str, u32 base, s16& s);
		bool parseSint(String str, u32 base, s32& s);
		bool parseSint(String str, u32 base, s64& s);

		bool parseUint(String str, u32 base, u8& u);
		bool parseUint(String str, u32 base, u16& u);
		bool parseUint(String str, u32 base, u32& u);
		bool parseUint(String str, u32 base, u64& u);

	} // end StrConv
} // end Dunjun

#endif
