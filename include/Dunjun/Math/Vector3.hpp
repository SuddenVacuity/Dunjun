#ifndef  DUNJUN_MATH_VECTOR3_HPP
#define  DUNJUN_MATH_VECTOR3_HPP

#include <Dunjun/Math/Vector2.hpp>

namespace Dunjun
{
	struct Vector3
	{
		//f32 x = 0;
		//f32 y = 0;
		//f32 z = 0;

		union // call info as v.x = 1;
		{
			f32 data[3];
			struct
			{
				f32 x, y, z;
			};
			struct
			{
				f32 r, g, b;
			};
			struct
			{
				f32 s, t, p;
			};
		};
		
		Vector3();
		explicit Vector3(f32 xyz);
		Vector3(f32 x, f32 y, f32 z);
		Vector3(f32 xyz[3]);
		
		explicit Vector3(const Vector2& other, f32 z);
		
		Vector3(const Vector3& other) = default; // copy constructor

		//
		// operators
		f32& operator[](size_t index) { return data[index]; }
		const f32& operator[](size_t index) const { return data[index]; } // this lets you call informatino in v[0] = 1; format

		bool operator==(const Vector3& other) const;
		bool operator!=(const Vector3& other) const;

		Vector3 operator-() const;

		Vector3 operator+(const Vector3& other) const;
		Vector3 operator-(const Vector3& other) const;

		Vector3 operator*(f32 scaler) const;
		Vector3 operator*(const Vector3& other) const;
		Vector3 operator/(const Vector3& other) const;
		Vector3 operator/(f32 scaler) const;

		Vector3& operator+=(const Vector3& other);
		Vector3& operator-=(const Vector3& other);
		Vector3& operator*=(f32 scaler);
		Vector3& operator/=(f32 scaler);

	}; // end Vector3
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
		return os << "Vector3(" << v[0] << ", " << v[1] << ", " << v[2] << ")";
	}
} // end Dunjun

#endif
