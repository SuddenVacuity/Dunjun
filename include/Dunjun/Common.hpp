#ifndef DUNJUN_COMMON_HPP // ifndef checks if this macro has been defined
#define DUNJUN_COMMON_HPP // if not then define it

namespace Dunjun
{
	//using Float32 = float; // sets Float32 to function as float
	//using Float64 = double; // sets Float64 to function as double

	#define GLOBAL static // set static to refer to all these types
	#define INTERNAL static
	#define LOCAL_PERSIST static
} // END namespace Dunjun


#endif