
#include <Dunjun/System/StringFunctions.hpp>

namespace Dunjun
{
	namespace Strings
	{
		// TODO: support UTF-8 and unicode

		namespace
		{
			const u32 PrimeRK = 16777619;
		}

		// `hashStr` returns the hash and the appropriate multiplicative factor
		// for use in the Rabin-Karp algorithm
		INTERNAL void hashStr(const String& sep, u32& hash, u32& power)
		{
			hash = 0;

			const uSize_t lsep = len(sep);

			for(uSize_t i = 0; i < lsep; i++)
				hash = hash * PrimeRK + (u32)sep[i];

			power = 1;

			u32 sq = PrimeRK;
			for(uSize_t i = lsep; i > 0; i >>= 1)
			{
				if((i & 1) != 0)
					power *= sq;
				sq *= sq;
			}
		}

		// `hashStrRev` returns the hash of the reverse of sep and the appropriate
		// multiplicative factor for use in Rabin-Karp algorithm.
		INTERNAL void hashStrRev(const String& sep, u32& hash, u32& power)
		{
			hash = 0;

			const uSize_t lsep = len(sep);

			for(uSize_t i = lsep - 1; i >= 0; i--)
				hash = hash * PrimeRK + (u32)sep[i];

			power = 1;

			u32 sq = PrimeRK;

			for(uSize_t i = lsep; i > 0; i >>= 1)
			{
				if((i & 1) != 0)
					power *= sq;
				sq *= sq;
			}
		}

		// returns true if substr is within s
		bool contains(const String& s, const String& substr)
		{
			return index(s, substr) >= 0;
		}
		// returns true if any characters in chars are contained in s
		bool containsAny(const String& s, const String& chars)
		{
			return indexAny(s, chars) >= 0;
		}

		// returns true if string begins with prefix
		bool hasPrefix(const String& s, const String& prefix)
		{
			return len(s) >= len(prefix) && substring(s, 0, len(prefix)) == prefix;
		}
		// returns true if string ends with suffix
		bool hasSuffix(const String& s, const String& suffix)
		{
			return len(s) >= len(suffix) && substring(s, len(s) - len(suffix), len(s)) == suffix;
		}

		INTERNAL char* copyStringToCString(const String& str, Allocator& a)
		{
			const char* cStr = cString(str);
			const uSize_t l = len(str) + 1;
			char* out = (char*)a.allocate(l);
			memcpy(out, cStr, l);
			return out;
		}

		// concatenates the elements of array to create a single string
		// the seperator sep is placed between elements in the resulting string
		String join(const Array<char*>& array, const String& sep)
		{
			const sSize_t aLen = len(array);
			if(aLen == 0)
				return "";

			if(aLen == 1)
				return array[0];

			sSize_t n = len(sep) * (aLen - 1);
			for(sSize_t i = 0; i < aLen; i++)
				n += len(array[i]);

			String out = array[0];

			for(sSize_t i = 1; i < aLen; i++)
			{
				append(out, sep);
				append(out, array[i]);
			}

			return out;
		}

		// slices s into all sbustrings seperated by sep and returns an array
		// of the bubstrings between those seprators
		void split(const String& s, const String& sep, Array<char*>& out)
		{
			Allocator& a = out.m_allocator;

			if (sep == "")
			{
				const uSize_t n = len(s);

				resize(out, n);

				for (uSize_t i = 0; i < n; i++)
					out[i] = copyStringToCString(s[i], a);

				return;
			}

			const uSize_t n = count(s, sep) + 1;
			const char c = sep[0];

			uSize_t start = 0;
			uSize_t na = 0;
			resize(out, n);

			const uSize_t lsep = len(sep);
			const uSize_t ls = len(s);

			for (uSize_t i = 0; i + lsep <= ls && na + 1 < n; i++)
			{
				if (s[i] == c && (lsep == 1 || substring(s, i, i + lsep) == sep))
				{
					out[na] = copyStringToCString(substring(s, start, i), a);
					na++;
					start = i + lsep;
					i += lsep - 1;
				}
			}

			out[na] = copyStringToCString(substring(s, start, ls), a);

			resize(out, na + 1);
		}

		void deallocateCStringArray(Array<char*>& a)
		{
			Allocator& _a = a.m_allocator;
			for(char* s : a)
				_a.deallocate(s);
		}

		// Generic split: splits after each instance of sep, including sepSave bytes
		// of sep in the subarrays.
		//INTERNAL Array<String> genSplit(const String& s, const String& sep, sSize_t sepSave, sSize_t n)
		//{
		//	Array<String> a = defaultAllocator();
		//
		//	if(n == 0)
		//		return a;
		//
		//	if(sep == "")
		//		return explode(s, n);
		//
		//	if(n < 0)
		//		n = count(s, sep) + 1;
		//
		//	char c = sep[0];
		//	uSize_t start = 0;
		//	sSize_t na = 0;
		//
		//	reserve(a, n);
		//
		//	const uSize_t lsep = len(sep);
		//	const uSize_t ls = len(s);
		//
		//	for(uSize_t i = 0; i + lsep <= ls && na + 1 < n; i++)
		//	{
		//		if(s[i] == c && (lsep == 1 || substring(s, i, i + lsep) == sep))
		//		{
		//			a[na] = substring(s, start, i + sepSave);
		//			na++;
		//			start = i + lsep;
		//			i += lsep - 1;
		//		}
		//	}
		//
		//	a[na] = substring(s, start, ls);
		//
		//	resize(a, na + 1);
		//
		//	return a;
		//}
		//

		// returns lowercase version of s
		String toLowerCase(const String& s)
		{
			String out = s;

			const uSize_t ls = len(s);

			for(uSize_t i = 0; i < ls; i++)
				out[i] = (char)tolower(s[i]);

			return out;
		}
		char toLowerCase(const char c)
		{
			if(c >= 'A' && c <= 'Z')
				return c + 'a' - 'A';

			return c;
		}

		// returns a titlized version of s
		// TODO: implement String toTitleCase(const String& s);
		String toTitleCase(const String& s)
		{
			const uSize_t ls = len(s);

			if(ls == 0)
				return s;

			String out = s;

			uSize_t i = 0;
			while(isSpace(out[i]))
				i++;

			// TODO: Only upper the first of words that are a certain length (3-4)
			// TODO: Always captialize the first and last words
			for(/**/; i < ls; i++)
			{
				if(isSpace(out[i - 1]))
					out[i] = toUpperCase(out[i]);
			}

			return out;
		}
		// returns uppercase version of s
		String toUpperCase(const String& s)
		{
			String out = s;

			const uSize_t ls = len(s);

			for (uSize_t i = 0; i < ls; i++)
				out[i] = (char)toupper(s[i]);

			return out;
		}
		char toUpperCase(const char c)
		{
			if( c >= 'a' && c <= 'z')
				return c + 'A' - 'a';

			return c;
		}


		// returns true if character is whitespace
		bool isSpace(const char c)
		{
			const char* whiteSpace = " \t\n\v\f\r";
			for(uSize_t i = 0; i < 6; i++)
				if(c == whiteSpace[i])
					return true;

			return false;
		}
		// returns true if character is a-z A-Z
		bool isAlpha(const char c)
		{
			return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
		}
		// returns true if chracter is 0-9
		bool isNumeric(const char c)
		{
			return (c >= '0' && c <= '9');
		}
		// returns true if character is a-z A-Z 0-9
		bool isAlphaNumeric(const char c)
		{
			return isAlpha(c) || isNumeric(c);
		}


		//
		String trimLeft(const String& s, const String& cutset)
		{
			// TODO: make more efficient
			const sSize_t slen = len(s);
			const uSize_t clen = len(cutset);

			if(slen == 0)
				return s;

			sSize_t pos = -1;
			bool t = false;

			for(sSize_t i = 0; i < slen; i++)
			{
				for(uSize_t j = 0; j < clen; j++)
				{
					if(s[i] == cutset[j])
					{
						t = true;
						pos = i;
					}
				}
				if(!t)
					break;
				t = false;
			}

			if(pos != -1)
				return substring(s, pos+1, slen);
			return s;
		}
		//
		String trimRight(const String& s, const String& cutset)
		{
			// TODO: make more effiecient
			const sSize_t slen = len(s);
			const uSize_t clen = len(cutset);

			if (slen == 0)
				return s;

			sSize_t pos = -1;
			bool t = false;

			for (sSize_t i = slen - 1; i >= 0; i--)
			{
				for (uSize_t j = 0; j < clen; j++)
				{
					if (s[i] == cutset[j])
					{
						t = true;
						pos = i;
					}
				}
				if (!t)
					break;
				t = false;
			}

			if (pos != -1)
				return substring(s, 0, pos);
			return s;
		}
		// trim(s, cutset)` is equivalent to `trimLeft(trimRight(s, cutset), cutset)
		String trim(const String& s, const String& cutset)
		{
			return trimLeft(trimRight(s, cutset), cutset);
		}
		//removes any special characters (space, \n, \t, etc)
		String trimSpace(const String& s)
		{
			return trim(s, " \t\n\v\f\r");
		}

		// counts the number of non overlapping instances od sep
		size_t count(const String& s, const String& sep)
		{
			uSize_t n = 0;

			const uSize_t ls = len(s);
			const uSize_t lsep = len(sep);

			if(lsep == 0)
				return ls + 1;

			if(lsep == 1)
			{
				char c = sep[0];
				for(uSize_t i = 0; i < ls; i++)
				{
					if(s[i] == c)
						n++;
				}
				return n;
			}

			if(lsep > ls)
				return 0;

			if(lsep == ls)
			{
				if(sep == s)
					return 1;
				return 0;
			}

			// Rabin-Karp search
			u32 hashsep, power;
			hashStr(sep, hashsep, power);

			u32 h = 0;
			for(uSize_t i = 0; i < lsep; i++)
				h = h * PrimeRK + (u32)s[i];

			uSize_t lastMatch = 0;
			if(h == hashsep && substring(sep, 0, lsep) == sep)
			{
				n++;
				lastMatch = lsep;
			}

			for(uSize_t i = lsep; i < ls; /**/)
			{
				h *= PrimeRK;
				h += (u32)s[i];
				h -= power * (u32)s[i - lsep];
				i++;
				if(h == hashsep &&
					lastMatch <= i - lsep && 
					substring(s, i - lsep, i) == sep)
				{
					n++;
					lastMatch = i;
				}
			}
			return n;
		}

		// returns the index of first instance of sep
		// returns -1 if sep is no present
		sSize_t index(const String& s, const String& sep)
		{
			const uSize_t n = len(sep);
			const uSize_t sl = len(s);

			if(n == 0)
				return 0;

			if(n == 1)
			{
				const char c = sep[0];
				for(uSize_t i = 0; i < sl; i++)
				{
					if(s[i] == c)
						return (sSize_t)i;
					return -1;
				}
			}

			if(n == sl)
			{
				if(sep == s)
					return 0;
				return -1;
			}

			if(n > sl)
				return -1;

			// Rabin-Karp search
			u32 hashsep, power;
			hashStr(sep, hashsep, power);
			u32 h = 0;

			for(uSize_t i = 0; i < n; i++)
				h = h * PrimeRK + (u32)s[i];

			if(h == hashsep && substring(s, 0, n) == sep)
				return 0;

			for(uSize_t i = n; i < sl; /**/)
			{
				h *= PrimeRK;
				h += (u32)s[i];
				h -= power * (u32)s[i - n];
				i++;
				if(h == hashsep && substring(s, i - n, i) == sep)
					return i - n;
			}

			return -1;
		}
		// returns the index of the first of any character in chars
		// returns -1 if no characters in chars are present
		sSize_t indexAny(const String& s, const String& chars)
		{
			const uSize_t cLen = len(chars);
			const uSize_t sLen = len(s);

			if(cLen > 0)
			{
				for(uSize_t i = 0; i < sLen; i++)
				{
					for(uSize_t j = 0; j < cLen; j++)
					{
						if(s[i] == chars[j])
							return (sSize_t)i;
					}
				}
			}

			return -1;
		}
		// returns the index of last instance of sep
		// returns -1 if sep is no present
		sSize_t lastIndex(const String& s, const String& sep)
		{
			uSize_t n = len(sep);

			if(n == 0)
				return len(sep);

			if(n == 1)
			{
				char c = sep[0];
				for(uSize_t i = len(s) - 1; i >= 0; i--)
				{
					if(s[i] == c)
						return i;
				}
				return -1;
			}

			if(n == len(s))
			{
				if(sep == s)
					return 0;
				return -1;
			}

			if(n > len(s))
				return -1;

			// Rabin-Karp search
			u32 hashsep, power;
			hashStrRev(sep, hashsep, power);
			uSize_t last = len(s) - n;

			u32 h = 0;
			for(uSize_t i = last - 1; i >= 0; i--)
			{
				h *= PrimeRK;
				h += (u32)s[i];
				h -= power * (u32)s[i + n];
				if(h == hashsep && substring(s, i, i + n) == sep)
					return i;
			}

			return -1;
		}
		// returns the index of the last of any character in chars
		// returns -1 if no characters in chars are present
		sSize_t lastIndexAny(const String& s, const String& chars)
		{
			if(len(chars) > 0)
			{
				for(uSize_t i = len(s) - 1; i >= 0; i--)
				{
					for(uSize_t j = 0; j < len(chars); j++)
					{
						if(s[i] == chars[i])
							return i;
					}
				}
			}

			return -1;
		}

		// returns sting s * count
		String repeat(const String& s, uSize_t count)
		{
			String out;
			reserve(out, count * len(s));

			for(uSize_t i = 0; i < count; i++)
				append(out, s);

			return out;
		}

		String replace(const String& oldStr, const String& newStr, sSize_t n)
		{
			// TODO: Strings::replace()
			return oldStr;
		}


	} // end Strings
} // end Dunjun

