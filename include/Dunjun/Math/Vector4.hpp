#ifndef  DUNJUN_MATH_VECTOR4_HPP
#define  DUNJUN_MATH_VECTOR4_HPP

#include <Dunjun/Types.hpp>

namespace Dunjun
{
	struct Vector4
	{
		Vector4()
			: x(0)
			, y(0)
			, z(0)
			, w(0)
		{
		}
		explicit Vector4(f32 xyzw)
			: x(xyzw)
			, y(xyzw)
			, z(xyzw)
			, w(xyzw)
		{
		}
		Vector4(f32 x, f32 y, f32 z, f32 w) // explicit means you must write Vector2 to get the info
			: x(x)
			, y(y)
			, z(z)
			, w(w)
		{
		}
		Vector4(f32 xyzw[4])
			: x(xyzw[0])
			, y(xyzw[1])
			, z(xyzw[2])
			, w(xyzw[3])
		{
		}

		f32& operator[](size_t index) { return data[index]; }
		const f32& operator[](size_t index) const { return data[index]; } // this lets you call informatino in v[0] = 1; format

		union // call info as v.x = 1;
		{
			f32 data[4];
			struct
			{
				f32 x, y, z, w;
			};
			struct
			{
				f32 r, g, b, a;
			};
			struct
			{
				f32 s, t, p, q;
			};
		};
	};
}

#endif
#pragma once
