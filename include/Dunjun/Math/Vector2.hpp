#ifndef  DUNJUN_MATH_VECTOR2_HPP
#define  DUNJUN_MATH_VECTOR2_HPP

#include <Dunjun/Math/Functions/MathLib.hpp>

namespace Dunjun
{
	struct Vector2
	{
		union// call info as v.x = 1;
		{
			struct
			{
				f32 x, y;
			};
			f32 data[2];
		};
		GLOBAL const Vector2 Zero;

		inline f32& operator[](uSize_t index) { return data[index]; }
		inline const f32& operator[](uSize_t index) const { return data[index]; }
	};
	//
	// operators
	//inline f32& operator[](Vector2& v, size_t index) { return v.data[index]; }
	//inline const f32& operator[](Vector2& v, size_t index) { return v.data[index]; } // this lets you call informatino in v[0] = 1; format

	bool operator==(const Vector2& a, const Vector2& b);
	bool operator!=(const Vector2& a, const Vector2& b);

	Vector2 operator-(const Vector2& a);

	Vector2 operator+(const Vector2& a, const Vector2& b);
	Vector2 operator-(const Vector2& a, const Vector2& b);

	Vector2 operator*(const Vector2& a, f32 scaler);
	Vector2 operator*(const Vector2& a, const Vector2& b);
	Vector2 operator/(const Vector2& a, f32 scaler);
	Vector2 operator/(const Vector2& a, const Vector2& b);

	Vector2& operator+=(Vector2& a, const Vector2& b);
	Vector2& operator-=(Vector2& a, const Vector2& b);
	Vector2& operator*=(Vector2& a, f32 scaler);
	Vector2& operator/=(Vector2& a, f32 scaler);


	//
	// Functions outside vector 2 struct
	inline Vector2& operator*(f32 scaler, const Vector2& vector) // scaler for the other side
	{
		return vector * scaler;
	}

	f32 dot(const Vector2& a, const Vector2& b);
	f32 cross(const Vector2& a, const Vector2& b);
	f32 lengthSquared(const Vector2& a);
	f32 length(const Vector2& a);
	Vector2 normalize(const Vector2& a);

	inline std::ostream& operator<<(std::ostream& os, const Vector2& v)
	{
		return os << "Vector2(" << v.data[0] << ", " << v.data[1] << ")";
	}

} // end Dunjun

#endif
