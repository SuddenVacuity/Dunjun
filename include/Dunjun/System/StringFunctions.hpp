#ifndef DUNJUN_SYSTEM_STRINGFUNCITONS_HPP
#define DUNJUN_SYSTEM_STRINGFUNCITONS_HPP

#include <Dunjun/System/String.hpp>

#include <functional>

namespace Dunjun
{
namespace Strings
{
	// TODO: support UTF-8 and unicode

	// returns true if substr is within s
	bool contains(const String& s, const String& substr);
	// returns true if any characters in chars are contained in s
	bool containsAny(const String& s, const String& chars);

	// returns true if string begins with prefix
	bool hasPrefix(const String& s, const String& prefix);
	// returns true if string ends with suffix
	bool hasSuffix(const String& s, const String& suffix);

	// concatenates the elements of array to create a single string
	// the seperator sep is placed between elements in the resulting string
	String join(const Array<String>& array, const String& sep);

	// slices s into all sbustrings seperated by sep and returns an array
	// of the bubstrings between those seprators
	void split(const String& s, const String& sep, Array<String>& out);

	// returns lowercase version of s
	String toLowerCase(const String& s);
	char toLowerCase(const char c);
	// returns a titlized version of s
	// TODO: implement String toTitleCase(const String& s);
	String toTitleCase(const String& s);
	// returns uppercase version of s
	String toUpperCase(const String& s);
	char toUpperCase(const char c);

	// returns true if character is whitespace
	bool isSpace(const char c);
	// returns true if character is a-z A-Z
	bool isAlpha(const char c);
	// returns true if chracter is 0-9
	bool isNumeric(const char c);
	// returns true if character is a-z A-Z 0-9
	bool isAlphaNumeric(const char c);


	//
	String trimLeft(const String& s, const String& cutset);
	//
	String trimRight(const String& s, const String& cutset);
	// trim(s, cutset)` is equivalent to `trimLeft(trimRight(s, cutset), cutset)
	String trim(const String& s, const String& cutset);
	//removes any special characters (space, \n, \t, etc)
	String trimSpace(const String& s);

	// counts the number of non overlapping instances od sep
	size_t count(const String& s, const String& sep);

	// returns the index of first instance of sep
	// returns -1 if sep is no present
	sSize_t index(const String& s, const String& sep);
	// returns the index of the first of any character in chars
	// returns -1 if no characters in chars are present
	sSize_t indexAny(const String& s, const String& chars);
	// returns the index of last instance of sep
	// returns -1 if sep is no present
	sSize_t lastIndex(const String& s, const String& sep);
	// returns the index of the last of any character in chars
	// returns -1 if no characters in chars are present
	sSize_t lastIndexAny(const String& s, const String& chars);

	// returns sting s * count
	String repeat(const String& s, uSize_t count);

	String replace(const String& oldStr, const String& newStr, sSize_t n);


} // end Strings
} // end Dunjun

#endif
