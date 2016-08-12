#ifndef DUNJUN_SYSTEM_STRING
#define DUNJUN_SYSTEM_STRING

#include <Dunjun/System/Containers/RingBuffer.hpp>

namespace Dunjun
{
	struct String
	{
		Array<char> data;

		String(Allocator& a = defaultAllocator());
		String(char c, Allocator& a = defaultAllocator());
		String(const char* s, Allocator& a = defaultAllocator());
		String(const char* s, size_t length, Allocator& a = defaultAllocator());

		String(const String& s);
		String& operator=(const String& s);

		String& operator=(const char c);
		String& operator=(const char* str);

		~String() = default;

		inline char& operator[](size_t index) { return data[index]; }
		inline const char& operator[](size_t index) const { return data[index]; }
	}; // end struct String

	size_t len(const String& s);
	size_t capacity(const String& s);

	size_t append(String& s, char c);
	size_t append(String& s, const char* cStr);
	size_t append(String& s, const String& other);

	void popBack(String& s);

	char* begin(String& s);
	const char* begin(const String& s);
	char* end(String& s);
	const char* end(const String& s);

	char& front(String& s);
	const char& front(const String& s);
	char& back(String& s);
	const char& back(const String& s);

	const char* cString(const String& str);

	void clear(String& s);

	void shrinkToFit(String& s);

	void resize(String& s, size_t length);

	void reserve(String& s, size_t capacity);

	// `substring` returns a copy of a part of the string in the range [begin, end)
	String substring(const String& s, size_t begin, size_t end);

	/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	)				
	)				OPERATORS
	)
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

	String operator+(const String& left, const String& right);

	String operator+(const String& left, const char* right);
	String operator+(const char* left, const String& right);

	String operator+(const String& left, char right);
	String operator+(char left, const String& right);

	bool operator==(const String& left, const String& right);
	bool operator==(const String& left, const char* right);
	bool operator==(const char* left, const String& right);

	bool operator!=(const String& left, const String& right);
	bool operator!=(const String& left, const char* right);
	bool operator!=(const char* left, const String& right);

	bool operator<(const String& left, const String& right);
	bool operator<(const String& left, const char* right);
	bool operator<(const char* left, const String& right);

	bool operator<=(const String& left, const String& right);
	bool operator<=(const String& left, const char* right);
	bool operator<=(const char* left, const String& right);

	bool operator>(const String& left, const String& right);
	bool operator>(const String& left, const char* right);
	bool operator>(const char* left, const String& right);

	bool operator>=(const String& left, const String& right);
	bool operator>=(const String& left, const char* right);
	bool operator>=(const char* left, const String& right);

	std::ostream& operator<<(std::ostream& os, const String& s);
	std::istream& operator>>(std::istream& is, String& s);

	std::istream& getline(std::istream& is, String& s);

} // end Dunjun

#endif
