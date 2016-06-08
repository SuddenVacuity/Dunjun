#include <Dunjun/Math/Constants.hpp>

//#include <limits> // included in Common.hpp

namespace Dunjun
{
namespace Constants
{
	extern const f32 EPSILON = std::numeric_limits<f32>::epsilon();

	const f32  ZERO		  = 0.00000000f;
	const f32  ONE		  = 1.00000000f;
	const f32  THIRD	  = 0.33333333f;
	const f32  TWO_THIRDS = 0.66666667f;
		   
	const f32  E   = 2.71828183f;
	const f32  PI  = 3.14159265f;
	const f32  TAU = 6.28318531f;
		   
	const f32  SQRT_2 =	1.41421356f;
	const f32  SQRT_3 =	1.73205081f;
	const f32  PHI    = 1.61803399f;

}
}
/* 64 bit versions
namespace Constants
{
	const f64  ZERO		  = 0.000000000000000f;
	const f64  ONE		  = 1.000000000000000f;
	const f64  THIRD	  = 0.333333333333333f;
	const f64  TWO_THIRDS = 0.666666666666667f;
		   
	const f64  E   = 2.718281828459845f;
	const f64  PI  = 3.141592653589793f;
	const f64  TAU = 6.283185307179586f;
		   
	const f64  SQRT_2 = 1.414213562373895f;
	const f64  SQRT_3 = 1.732050807568877f;
	const f64  PHI    = 1.618033988749895f;

}*/
