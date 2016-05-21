#ifndef DUNJUN_COMMON_HPP // ifndef checks if this macro has been defined
#define DUNJUN_COMMON_HPP // if not then define it

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
#include <cstdint>
#include <cstdlib>
#include <deque> // SceneNode.hpp
#include <functional>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <ostream>
#include <random>
#include <sstream>
#include <stack>
#include <stdexcept>
//#include <typeindex> // SceneNode.hpp
//#include <typeinfo> // SceneNode.hpp
#include <vector>

#define GLOBAL static // set all these types to refer to static
#define INTERNAL static
#define LOCAL_PERSIST static

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
			static_asset(std::extent<T>::value == 0, "make_unique<T[N]>() iis forbidden, please use make_uniques<T[]>().");
	
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


} // END namespace Dunjun

#endif
