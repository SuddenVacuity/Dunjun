#ifndef DUNJUN_COMMON_HPP // ifndef checks if this macro has been defined
#define DUNJUN_COMMON_HPP // if not then define it

#include <Dunjun/System/Types.hpp>

// undef these for internal math library
#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

#include <algorithm>
#include <array> // SceneNode.hpp
#include <bitset> // SceneNode.hpp
#include <cassert>
#include <chrono>
//#include <cmath> replaced by internal math library
#include <cstdarg>
//#include <cstdint> no longer needed by Types.hpp
#include <cstdio>
#include <cstdlib>
#include <cstring> // Allocator Containers
#include <deque> // SceneNode.hpp
#include <functional>
#include <fstream>
#include <iostream>
#include <iterator> // Renderer.cpp
#include <limits>
#include <map>
#include <memory>
#include <ostream>
#include <random>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <thread>
//#include <typeindex> // SceneNode.hpp
//#include <typeinfo> // SceneNode.hpp
#include <unordered_map> // ResourceHolder.hpp
#include <vector>

//namespace
//{
//	template <typenameF>
//	struct Defer
//	{
//		Defer(F f)
//			:f(f)
//		{
//		}
//
//		~Defer() { f(); }
//
//		F f;
//	};
//
//	template <typename F>
//	inline Defer<F> makeDefer(F f)
//	{
//		return Defer<F>(f);
//	}
//} // end anon namespace
//
//#define STRING_JOIN2(arg1, arg2) DO_STRING_JOIN(arg1, arg2)
//#define DO_STRING_JOIN2(arg1, arg2) arg1 ## arg2
//#define DEFER(code) auto STRING_JOIN2(defer_, __LINE__) = makeDefer([=](){code;});

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
		std::memcpy(&to, &x, ((sizeof(T) < sizeof(U)) ? sizeof(T) : sizeof(U)));
		return to;
	}
	/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	)				.
	)					DEFER FUNCTIONS TO END OF SCOPE
	)
	)				.
	)					.
	)
	)				.
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

	namespace Impl
	{
		template <typename Fn>
		struct Defer
		{
			Defer(Fn&& fn)
				: fn(std::forward<Fn>(fn))
			{
			}

			~Defer() 
			{ 
				fn(); 
			}

			Fn fn;
		};

		template <typename Fn>
		Defer<Fn> deferFn(Fn&& fn)
		{
			return Defer<Fn>(std::forward<Fn>(fn)); 
		}

	} // end Impl

#define Defer_1(x, y) x##y
#define Defer_2(x, y) Defer_1(x, y)
#define Defer_3(x) Defer_2(c, __COUNTER__)
#define defer(code) auto Defer_3(_defer_) = Impl::deferFn([&](){code;});

/*/	Example for defer
 *	FILE* f{open("test.txt", "r")}
 *	if (f == nullptr)
 *		return;
 *	defer(fclose(f)); 
/*/



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

		return {s_buf, strlen(s_buf)};
	}

	//std::string resourcePath();
	//std::string getFileDirectory(const std::string& filepath);

	//inline void throwRuntimeError(const std::string& str)
	//{
	//	std::cerr << str.c_str() << std::endl;
	//	std::runtime_error(str.c_str());
	//}

	// len() currently unused
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

	bool showSimpleMessageBox(MessageBoxType type, 
							  const std::string& title, 
							  const std::string& message);

	namespace BaseDirectories
	{
		extern const std::string Shaders;
		extern const std::string Texture;
	} // end BaseDirectories
} // END namespace Dunjun

#endif
