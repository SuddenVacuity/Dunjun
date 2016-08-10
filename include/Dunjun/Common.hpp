#ifndef DUNJUN_COMMON_HPP // ifndef checks if this macro has been defined
#define DUNJUN_COMMON_HPP // if not then define it

#include <Dunjun/System/MurmurHash.hpp>

// undef these for internal math library
#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif


namespace Dunjun
{
	namespace
	{
		template <class T, class... Args>
		INTERNAL std::unique_ptr<T> make_unique_helper(std::false_type, Args&&... args)
		{
			return std::unique_ptr<T> (new T(std::forward<Args>(args)...));
		}
	
		template <class T, class... Args>
		INTERNAL std::unique_ptr<T> make_unique_helper(std::true_type, Args&&... args)
		{
			static_assert(std::extent<T>::value == 0, "Dunjun::make_unique<T[N]>() is forbidden, please use make_uniques<T[]>().");
	
			typedef typename std::remove_extent<T>::type U;
			return std::unique_ptr<T>(new U[sizeof...(Args)]{std::forward<Args>(args)...});
		}
	}// END namespace
	
	// NOTE: std::make_unique is not available in C++11, only C++14
	//		 MSVC does support std::make_unique but use this function instead.
	template <class T, class... Args>
	inline std::unique_ptr<T> make_unique(Args&&... args)
	{
		return make_unique_helper<T>(std::is_array<T>(), std::forward<Args>(args)...);
	}

	// Very similar to doing: T y = *(T*)(&x);
	template <typename T, typename U>
	inline T pseudo_cast(const U& x)
	{
		T to = T(0);
		memcpy(&to, &x, ((sizeof(T) < sizeof(U)) ? sizeof(T) : sizeof(U)));
		return to;
	}

	// Cross-Platform version of sprintf that uses a local persist buffer
	// If more than 1024 characters are needed, a std::stringstream may be needed
	// instead
	inline std::string stringFormat(const char* fmt, ...)
	{
		LOCAL_PERSIST char s_buf[1024];
		va_list v;
		va_start(v, fmt);

#if defined(DUNJUN_COMPILER_MSVC)
		_vsnprintf_s(s_buf, 1024, fmt, v);
#else
		vsnprintf(s_buf, 1024, fmt, v);
#endif
		va_end(v);
		s_buf[1023] = '\0';

		return {s_buf, len(s_buf)};
	}

	//std::string resourcePath();
	//std::string getFileDirectory(const std::string& filepath);

	//inline void throwRuntimeError(const std::string& str)
	//{
	//	std::cerr << str.c_str() << std::endl;
	//	std::runtime_error(str.c_str());
	//}

	inline size_t len(const char* str)
	{
		if (str)
			return strlen(str);
		return 0;
	}
	inline size_t len(char* str)
	{
		if (str)
			return strlen(str);
		return 0;
	}
	template <typename T>
	inline size_t len(const T& t)
	{
		return t.size();
	}
	template <typename T, size_t N>
	inline size_t len(const T(&array)[N])
	{
		return N;
	}	
	template <typename T>
	inline bool empty(const T& t)
	{
		return t.empty();
	}
	template <typename T, size_t N>
	inline bool empty(const T(&array)[N])
	{
		return false;
	}


	inline void throwRuntimeError(const std::string& str)
	{
		std::cerr << str.c_str() << std::endl;
		std::runtime_error(str.c_str());
		std::exit(EXIT_FAILURE);
	}

	enum class MessageBoxType
	{
		Error,
		Warning,
		Information,
	};

	bool showSimpleMessageBox(const String& message = "An error occurred.",
							  const String& title = "Error",
							  MessageBoxType type = MessageBoxType::Error);

	namespace BaseDirectories
	{
		char* const Textures  = "data/textures/";
		char* const Shaders = "data/shaders/";
	} // end BaseDirectories

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
)				.
)					.
)
)				.
)					.
)
)				.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
	
		namespace OSVersion
		{
			const u16 WINDOWS_2000	= 0x0500; // _WIN32_WINNT_WIN2K
			const u16 WINDOWS_ME	= 0x0490; // unknown
			const u16 WINDOWS_XP	= 0x0501; // _WIN32_WINNT_XP
			const u16 WINDOWS_XP64	= 0x0502; // _WIN32_WINNT_WS03
			const u16 WINDOWS_VISTA = 0x0600; // _WIN32_WINNT_VISTA
			const u16 WINDOWS_7		= 0x0601; // _WIN32_WINNT_WIN7
			const u16 WINDOWS_8		= 0x0602; // _WIN32_WINNT_WIN8
			const u16 WINDOWS_8_1	= 0x0603; // _WIN32_WINNT_WINBLUE
			const u16 WINDOWS_10	= 0x1000; // unknown
		}

	const char* getSystemOS();

} // END namespace Dunjun

#endif
