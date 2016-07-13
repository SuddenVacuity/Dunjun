#ifndef  DUNJUN_MATH_VECTOR3_HPP
#define  DUNJUN_MATH_VECTOR3_HPP

#include <Dunjun/Math/Vector2.hpp>

namespace Dunjun
{
	struct Vector3
	{
		union // call info as v.x = 1;
		{
			struct
			{
				f32 x, y, z;
			};
			struct
			{
				f32 r, g, b;
			};
			Vector2 xy;
			f32 data[3];
		};
		GLOBAL const Vector3 Zero;

		inline f32& operator[](usize index) { return data[index]; }
		inline const f32& operator[](usize index) const { return data[index]; }
	};

	//
	// operators
	//f32& operator[](size_t index) { return data[index]; }
	//const f32& operator[](size_t index) { return data[index]; } // this lets you call informatino in v[0] = 1; format

	bool operator==(const Vector3& a, const Vector3& b);
	bool operator!=(const Vector3& a, const Vector3& b);

	Vector3 operator-(const Vector3& a);

	Vector3 operator+(const Vector3& a, const Vector3& b);
	Vector3 operator-(const Vector3& a, const Vector3& b);

	Vector3 operator*(const Vector3& a, f32 scaler);
	Vector3 operator*(const Vector3& a, const Vector3& b);
	Vector3 operator/(const Vector3& a, const Vector3& b);
	Vector3 operator/(const Vector3& a, f32 scaler);

	Vector3& operator+=(Vector3& a, const Vector3& b);
	Vector3& operator-=(Vector3& a, const Vector3& b);
	Vector3& operator*=(Vector3& a, f32 scaler);
	Vector3& operator/=(Vector3& a, f32 scaler);

	//
	// Functions outside Vector3 struct
	inline Vector3 operator*(f32 scaler, const Vector3& vector) // scaler for the other side
	{
		return vector * scaler;
	}

	f32 dot(const Vector3& a, const Vector3& b);
	Vector3 cross(const Vector3& a, const Vector3& b);
	f32 lengthSquared(const Vector3& a);
	f32 length(const Vector3& a);
	Vector3 normalize(const Vector3& a);

	inline std::ostream& operator<<(std::ostream& os, const Vector3& v)
	{
		return os << "Vector3(" << v.data[0] << ", " << v.data[1] << ", " << v.data[2] << ")";
	}
} // end Dunjun

#endif
