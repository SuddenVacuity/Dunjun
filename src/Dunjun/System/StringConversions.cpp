
#include <Dunjun/System/StringConversions.hpp>

namespace Dunjun
{
	namespace StrConv
	{

		// formatFloat converts floating point number f to a string
		// format types: 'b' == (-ddddp+-ddd) binary exponent
		//				 'e' == (-d.dddde+-dd) decinmal exponent
		//				 'f' == (-ddd.ddd) no exponent
		//				 'g' == '' for large exponents
		String formatFloat(f64 f, char format, u32 precision, u32 bitsize)
		{
			// TODO: stringFormats
			return "";
		}

		String formatBool(bool b)
		{
			if(b)
				return "true";
			return "false";
		}

		String formatSint(s64 s, u32 base)
		{
			// TODO: stringFormats
			return "";
		}
		String formatUint(u64 u, u32 base)
		{
			// TODO: stringFormats
			return "";
		}

		/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		)				.
		)					PARSE FUNCTIONS
		)
		)				returns true if successful
		)				returns false if fails
		)
		)				.
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

		bool parseFloat(String str, f32& f)
		{ 
			f32 out;
			if(sscanf_s(cString(str), "%f", out) != 1)
				return false;

			f = out;
			return true;
		}
		bool parseFloat(String str, f64& f)
		{
			f64 out;
			if (sscanf_s(cString(str), "%g", out) != 1)
				return false;

			f = out;
			return true;
		}

		bool parseBool(String str, bool& b)
		{
			if (Strings::toLowerCase(str) == "true" || str == "1")
			{
				b = true;
				return true;
			}
			else if (Strings::toLowerCase(str) == "false" || str == "0")
			{
				b = false;
				return true;
			}

			std::cerr << "parseBool(): " + str + "is not a boolean value\n";
			return false;
		}

		/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		)				
		)			PARSE INTEGERS
		)					
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

		bool parseSint(String str, u32 base, s8& s)
		{
			// TODO: handle Base
			s8 out;
			if (sscanf_s(cString(str), "%d", out) != 1)
				return false;

			s = out;
			return true;
		}
		bool parseSint(String str, u32 base, s16& s)
		{
			// TODO: handle Base
			s16 out;
			if (sscanf_s(cString(str), "%d", out) != 1)
				return false;

			s = out;
			return true;
		}
		bool parseSint(String str, u32 base, s32& s)
		{
			// TODO: handle Base
			s32 out;
			if (sscanf_s(cString(str), "%d", out) != 1)
				return false;

			s = out;
			return true;
		}
		bool parseSint(String str, u32 base, s64& s)
		{
			// TODO: handle Base
			s64 out;
			if (sscanf_s(cString(str), "%lld", out) != 1)
				return false;

			s = out;
			return true;
		}

		bool parseUint(String str, u32 base, u8& u)
		{
			// TODO: handle Base
			u8 out;
			if (sscanf_s(cString(str), "%u", out) != 1)
				return false;

			u = out;
			return true;
		}
		bool parseUint(String str, u32 base, u16& u)
		{
			// TODO: handle Base
			u16 out;
			if (sscanf_s(cString(str), "%u", out) != 1)
				return false;

			u = out;
			return true;
		}
		bool parseUint(String str, u32 base, u32& u)
		{
			// TODO: handle Base
			u32 out;
			if (sscanf_s(cString(str), "%u", out) != 1)
				return false;

			u = out;
			return true;
		}
		bool parseUint(String str, u32 base, u64& u)
		{
			// TODO: handle Base
			u64 out;
			if (sscanf_s(cString(str), "%llu", out) != 1)
				return false;

			u = out;
			return true;
		}




	} // end StrConv
} // end Dunjun
