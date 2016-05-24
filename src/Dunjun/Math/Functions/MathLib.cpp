
#include <Dunjun/Math/Functions/MathLib.hpp>

#include <cmath>
#include <cfenv>

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
)				.
)					MATH LIBRARY
)
)				This is partly a library and partly a math wrapper
)					.
)
)				.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/


namespace Dunjun
{
	namespace Math
	{
		// Trigonometric
		f32 sin(const Radian& theta) { return std::sin(static_cast<f32>(theta)); }
		f32 cos(const Radian& theta) { return std::cos(static_cast<f32>(theta)); }
		f32 tan(const Radian& theta) { return std::tan(static_cast<f32>(theta)); }

		Radian asin(f32 a) { return Radian(std::asin(a)); }
		Radian acos(f32 a) { return Radian(std::acos(a)); }
		Radian atan(f32 a) { return Radian(std::atan(a)); }
		Radian atan2(f32 y, f32 x) { return Radian(std::atan2(y, x)); }

		// hyperbolic
		f32 sinh(f32 x) { return std::sinh(x); }
		f32 cosh(f32 x) { return std::cosh(x); }
		f32 tanh(f32 x) { return std::tanh(x); }

		f32 asinh(f32 x) { return std::asinh(x); }
		f32 acosh(f32 x) { return std::acosh(x); }
		f32 atanh(f32 x) { return std::atanh(x); }

		// power
		f32 pow(f32 x, f32 y) { return std::pow(x, y); }
		f32 sqrt(f32 x) { return std::sqrt(x); }
		f32 cbrt(f32 x) { return std::cbrt(x); }
		f32 hypotenuse(f32 x, f32 y) { return std::hypot(x, y); }

		f32 fastInvSqrt(f32 x) 
		{
			u32 i;
			f32 x2, y;
			const f32 threeHalves = 1.5f;

			x2 = x * 0.5f;
			y = x;
			i = pseudo_cast<u32>(y);

			i = 0x5f375a86 - (i >> 1);
			y = pseudo_cast<f32>(i);

			y = y * (threeHalves - (x2 * y * y)); // 1st iteration

			return y;
		}

		// exponential and logarithmic
		f32 exp(f32 x) { return std::exp(x); }
		f32 exp2(f32 x) { return std::exp2(x); }
		f32 ln(f32 x) { return std::log(x); }
		f32 ln1p(f32 x) { return std::log1p(x); }
		f32 log2(f32 x) { return std::log2(x); }
		f32 ln10(f32 x) { return std::log10(x); }
		f32 logBase(f32 x, f32 base) { return ln(x) * (1.0f / ln(base)); }

		// rounding
		f32 ceil(f32 x) { return std::ceil(x); }
		f32 floor(f32 x) { return std::floor(x); }

		f32 mod(f32 x, f32 y)
		{
			f32 result = std::remainder(Math::abs(x), (y = Math::abs(y)));

			if(std::signbit(result))
				result += y;

			return std::copysign(result, x);
		}

		f32 truncate(f32 x)
		{
			u32 i = pseudo_cast<u32>(x);
			s32 exponent = ( i >> 23 ) & 0xFF;
			s32 fractionalBits = 0x7F + 23 - exponent;

			if(fractionalBits > 23)
				return 0.0f;
			if(fractionalBits > 0)
				i &= ~((1u << fractionalBits) - 1);

			return pseudo_cast<f32>(i);
		}

		f32 round(f32 x)
		{
			std::fenv_t saveEnv;
			std::feholdexcept(&saveEnv);
			f32 result = std::rint(x);

			if(std::fetestexcept(FE_INEXACT))
			{
				std::fesetround(FE_TOWARDZERO);
				result = std::rint(std::copysign(0.5f + Math::abs(x), x));
			}

			std::feupdateenv(&saveEnv);
			return result;
		}

		s32 sign(s32 x)
		{
			u32 i = reinterpret_cast<const u32&>(x);
			i &= 0x80000000ul;

			if(i)
				return -1;

			return 1;
		}

		s64 sign(s64 x)
		{
			u64 i = reinterpret_cast<const u64&>(x);
			i &= 0x8000000000000000ull;

			if (i)
				return -1;

			return 1;
		}

		f32 sign(f32 x)
		{
			u32 i = reinterpret_cast<const u32&>(x);
			i &= 0x80000000ul;

			if (i)
				return -1.0f;

			return 1.0f;
		}


		// other
		f32 abs(f32 x)
		{
			u32 i = reinterpret_cast<const u32&>(x);
			i &= 0x7FFFFFFFul;
			return reinterpret_cast<const f32&>(i);
		}

		s8  abs(s8 x)
		{
			u8 i = reinterpret_cast<const u8&>(x);
			i &= 0x7Fu;
			return reinterpret_cast<const s8&>(i);
		}

		s16 abs(s16 x)
		{
			u16 i = reinterpret_cast<const u16&>(x);
			i &= 0x7FFFu;
			return reinterpret_cast<const s16&>(i);
		}

		s32 abs(s32 x)
		{
			u32 i = reinterpret_cast<const u32&>(x);
			i &= 0x7FFFFFFFul;
			return reinterpret_cast<const s32&>(i);
		}

		s64 abs(s64 x)
		{
			u64 i = reinterpret_cast<const u64&>(x);
			i &= 0x7FFFFFFFFFFFFFFFull;
			return reinterpret_cast<const s64&>(i);
		}

		Radian abs(const Radian& x)
		{
			return Radian(abs(static_cast<f32>(x)));
		}

		Degree abs(const Degree& x)
		{
			return Degree(abs(static_cast<f32>(x)));
		}


	} // end Math
} // end Dunjun
