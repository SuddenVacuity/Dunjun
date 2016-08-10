
#include <Dunjun/System/String.hpp>

namespace Dunjun
{

	String::String(Allocator& a)
		: data(a)
	{

	}

	String::String(char c, Allocator& a)
		: data(a)
	{
		append(data, c);
	}
	String::String(const char* s, Allocator& a)
		: data(a)
	{
		uSize_t len = 0;
		if(s)
			len = strlen(s);

		append(data, s, len);
	}
	String::String(const char* s, size_t length, Allocator& a)
		: data(a)
	{
		append(data, s, length);
	}

	String::String(const String& s)
		: data(s.data)
	{
	}
	String& String::operator=(const String& s)
	{
		data = s.data;
		return *this;
	}

	String& String::operator=(const char c)
	{
		clear(data);
		append(data, c);
		return *this;
	}
	String& String::operator=(const char* str)
	{
		uSize_t len = 0;
		if (str)
			len = strlen(str);

		clear(data);
		append(data, str, len);
		return *this;
	}

	//String::~String() = default;

	//inline char& String::operator[](size_t index) { return data[index]; }
	//inline const char& String::operator[](size_t index) const { return data[index]; }


	size_t len(const String& s)
	{
		return len(s.data);
	}
	size_t capacity(const String& s)
	{
		return capacity(s.data);
	}
	
	size_t append(String& s, char c)
	{
		return append(s.data, c);
	}
	size_t append(String& s, const char* cStr)
	{
		return append(s.data, cStr, len(cStr));
	}
	size_t append(String& s, const String& other)
	{
		return append(s.data, &other.data[0], len(other.data));
	}
	
	char* begin(String& s)
	{
		return begin(s.data);
	}
	const char* begin(const String& s)
	{
		return begin(s.data);
	}
	char* end(String& s)
	{
		return end(s.data);
	}
	const char* end(const String& s)
	{
		return end(s.data);
	}
	
	char& front(String& s)
	{
		return front(s.data);
	}
	const char& front(const String& s)
	{
		return front(s.data);
	}
	char& back(String& s)
	{
		return back(s.data);
	}
	const char& back(const String& s)
	{
		return back(s.data);
	}
	
	const char* cString(const String& str)
	{
		// use const cast here
		// string must be \0 terminated
		append(const_cast<Array<char>&>(str.data), '\0');
		popBack(const_cast<Array<char>&>(str.data));
		return begin(str);
	}
	
	void clear(String& s)
	{
		clear(s.data);
	}
	
	void shrinkToFit(String& s)
	{
		shrinkToFit(s.data);
	}
	
	void resize(String& s, size_t length)
	{
		resize(s.data, length);
	}

	void reserve(String& s, size_t capacity)
	{
		reserve(s.data, capacity);
	}
	
	// `substring` returns a copy of a part of the string in the range [begin, end)
	String substring(const String& s, size_t begin, size_t end)
	{
		if(end > len(s))
			end = len(s);

		String out = s.data.m_allocator;
		uSize_t l = end - begin;
		reserve(out, l);

		for(uSize_t i = begin; i < end; i++)
			append(out, s[i]);

		return out;
	}
	
	/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	)
	)				OPERATORS
	)
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
	
	String operator+(const String& left, const String& right)
	{
		String out = left;
		append(out, right);
		return out;
	}
	
	String operator+(const String& left, const char* right)
	{
		String out = left;
		append(out, right);
		return out;
	}
	String operator+(const char* left, const String& right)
	{
		String out = left;
		append(out, right);
		return out;
	}
	
	String operator+(const String& left, char right)
	{
		String out = left;
		append(out, right);
		return out;
	}
	String operator+(char left, const String& right)
	{
		String out = left;
		append(out, right);
		return out;
	}
	
	INTERNAL inline s32 stringCompare(const String& left, const String& right)
	{
		const char* s1 = cString(left);
		const char* s2 = cString(right);

		while(*s1 && (*s1 == *s2))
		{
			s1++;
			s2++;
		}

		return *(const u8*)(s1) - *(const u8*)(s2);
	}

	bool operator==(const String& left, const String& right)
	{
		return stringCompare(left, right) == 0;
	}
	bool operator==(const String& left, const char* right)
	{
		return stringCompare(left, cString(right)) == 0;
	}
	bool operator==(const char* left, const String& right)
	{
		return stringCompare(cString(left), right) == 0;
	}
	
	bool operator!=(const String& left, const String& right)
	{
		return !operator==(left, right);
	}
	bool operator!=(const String& left, const char* right)
	{
		return !operator==(left, right);
	}
	bool operator!=(const char* left, const String& right)
	{
		return !operator==(left, right);
	}
	
	bool operator<(const String& left, const String& right)
	{
		return stringCompare(left, right) < 0;
	}
	bool operator<(const String& left, const char* right)
	{
		return stringCompare(left, cString(right)) < 0;
	}
	bool operator<(const char* left, const String& right)
	{
		return stringCompare(cString(left), right) < 0;
	}
	
	bool operator<=(const String& left, const String& right)
	{
		return stringCompare(left, right) <= 0;
	}
	bool operator<=(const String& left, const char* right)
	{
		return stringCompare(left, cString(right)) <= 0;
	}
	bool operator<=(const char* left, const String& right)
	{
		return stringCompare(cString(left), right) <= 0;
	}
	
	bool operator>(const String& left, const String& right)
	{
		return stringCompare(left, right) > 0;
	}
	bool operator>(const String& left, const char* right)
	{
		return stringCompare(left, cString(right)) > 0;
	}
	bool operator>(const char* left, const String& right)
	{
		return stringCompare(cString(left), right) > 0;
	}
	
	bool operator>=(const String& left, const String& right)
	{
		return stringCompare(left, right) >= 0;
	}
	bool operator>=(const String& left, const char* right)
	{
		return stringCompare(left, cString(right)) >= 0;
	}
	bool operator>=(const char* left, const String& right)
	{
		return stringCompare(cString(left), right) >= 0;
	}
	
	std::ostream& operator<<(std::ostream& os, const String& s)
	{
		os << cString(s);
		return os;
	}
	std::istream& operator>>(std::istream& is, String& s)
	{
		char c = '\0';

		while(true)
		{
			if(is.eof())
				break;
			is >> c;
			if(c != '\0')
				break;

			append(s, c);
		}
		return is;
	}

	std::istream& getline(std::istream& is, String& s)
	{
		char c;
		clear(s);
		while(is.get(c) && c != '\n')
			append(s, c);
		return is;
	}

	
} // end Dunjun
