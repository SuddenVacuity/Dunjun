#ifndef  DUNJUN_MATH_VECTOR2_HPP
#define  DUNJUN_MATH_VECTOR2_HPP

#include <Dunjun/Math/Functions/MathLib.hpp>

namespace Dunjun
{
	struct Vector2
	{
		//f32 x = 0;
		//f32 y = 0;

		union // call info as v.x = 1;
		{
			f32 data[2];
			struct
			{
				f32 x, y;
			};
			struct
			{
				f32 r, g;
			};
			struct
			{
				f32 s, t;
			};
		};
		
		Vector2();
		explicit Vector2(f32 xy);
		Vector2(f32 x, f32 y);
		Vector2(f32 xy[2]);
		
		Vector2(const Vector2& other) = default; // copy constructor

		//
		// operators
		inline f32& operator[](size_t index) { return data[index]; }
		inline const f32& operator[](size_t index) const { return data[index]; } // this lets you call informatino in v[0] = 1; format

		bool operator==(const Vector2& other) const;
		bool operator!=(const Vector2& other) const;

		Vector2 operator-() const;

		Vector2 operator+(const Vector2& other ) const;
		Vector2 operator-(const Vector2& other) const;

		Vector2 operator*(f32 scaler) const;
		Vector2 operator*(const Vector2& other) const;
		Vector2 operator/(const Vector2& other) const;
		Vector2 operator/(f32 scaler) const;

		Vector2& operator+=(const Vector2& other);
		Vector2& operator-=(const Vector2& other);
		Vector2& operator*=(f32 scaler);
		Vector2& operator/=(f32 scaler);

	}; // end Vector2
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
		return os << "Vector2(" << v[0] << ", " << v[1] << ")";
	}

} // end Dunjun

#endif
