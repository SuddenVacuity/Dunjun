#ifndef DUNJUN_COMMON_HPP // ifndef checks if this macro has been defined
#define DUNJUN_COMMON_HPP // if not then define it

#include <Dunjun/ReadOnly.hpp>

#include <cassert>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <functional>
#include <fstream>
#include <iostream>
#include <limits>
#include <memory>
#include <ostream>
#include <random>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <vector>

#define GLOBAL static // set static to refer to all these types
#define INTERNAL static
#define LOCAL_PERSIST static

namespace Dunjun
{
namespace
{
	template <class T, class... Args>
	std::unique_ptr<T> make_unique_helper(std::false_type, Args&&... args)
	{
		return std::unique_ptr<T> (new T(std::forward<Args>(args)...));
	}

	template <class T, class... Args>
	std::unique_ptr<T> make_unique_helper(std::true_type, Args&&... args)
	{
		static_asset(std::extent<T>::value == 0, "make_unique<T[N]>() iis forbidden, please use make_uniques<T[]>().");

		typedef typename std::remove_extent<T>::type U;
		return std::unique_ptr<T>(new U[sizeof...(Args)]{std::forward<Args>(args)...});
	}
}// END namespace

// NOTE: std::make_unique is not available in C++11, only C++14
//		 MSVC does support std::make_unique but use this function instead.
template <class T, class... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
	return make_unique_helper<T>(std::is_array<T>(), std::forward<ARGS>(args)...);
}


} // END namespace Dunjun

#endif
