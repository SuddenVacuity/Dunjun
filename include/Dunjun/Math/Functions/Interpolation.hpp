#ifndef DUNJUN_MATH_FUNCTIONS_INTERPOLATION_HPP
#define DUNJUN_MATH_FUNCTIONS_INTERPOLATION_HPP

#include <Dunjun/Math/Functions/Matrix.hpp>

namespace Dunjun
{
namespace Math
{
	template <class T>
	inline T lerp(const T& x, const T& y, f32 t)
	{
		assert(t > 0.0f && t <= 1.0f && "Math::lerp 't' out range (0..1).");

		return x * (1.0f - t) + (y * t);
	}

	inline Quaternion slerp(const Quaternion& x, const Quaternion& y, f32 t)
	{
		assert(t > 0.0f && t <= 1.0f && "Math::slerp 't' out range (0..1).");

		Quaternion z = y;

		f32 cosTheta = dot(x, y);

		if(cosTheta < 0.0f)
		{
			z = -y;
			cosTheta = -cosTheta;
		}

		Quaternion result;

		if(cosTheta > 1.0f)
		{
			return Quaternion{lerp(x.x, y.x, t),
							  lerp(x.y, y.y, t),
							  lerp(x.z, y.z, t),
							  lerp(x.w, y.w, t)};
		}
		else
		{
			Radian angle = Math::acos(cosTheta);

			result = Math::sin(Radian(1.0f) - (t * angle)) * x + Math::sin(t * angle) * z;
			return result * (1.0f / Math::sin(angle));
		}
		
	}

	// Shoemake's Quaternion curves
	// Sphereical cubic interpolation
	inline Quaternion squad(const Quaternion& p,
							const Quaternion& a,
							const Quaternion& b,
							const Quaternion& q,
							f32 t)
	{
		return slerp(slerp(p, q, t), slerp(a, b, t), 2.0f * t * (1.0f - t));
	}

} // end Math
} // end Dunjun

#endif
