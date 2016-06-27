#ifndef  DUNJUN_MATH_VECTOR4_HPP
#define  DUNJUN_MATH_VECTOR4_HPP


#include <Dunjun/Math/Vector3.hpp>


namespace Dunjun
{
	struct Vector4
	{
		//f32 x = 0;
		//f32 y = 0;
		//f32 z = 0;
		//f32 w = 0;

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
		
		Vector4();
		explicit Vector4(f32 xyzw);
		Vector4(f32 x, f32 y, f32 z, f32 w);
		Vector4(f32 xyzw[4]);
		
		explicit Vector4(const Vector2& other, f32 z, f32 w);
		explicit Vector4(const Vector3& other, f32 w);
		
		Vector4(const Vector4& other) = default; // copy constructor

		//
		// operators
		inline f32& operator[](size_t index) { return data[index]; }
		inline const f32& operator[](size_t index) const { return data[index]; } // this lets you call informatino in v[0] = 1; format

		bool operator==(const Vector4& other) const;
		bool operator!=(const Vector4& other) const;

		Vector4 operator-() const;

		Vector4 operator+(const Vector4& other) const;
		Vector4 operator-(const Vector4& other) const;

		Vector4 operator*(f32 scaler) const;
		Vector4 operator*(const Vector4& other) const;
		Vector4 operator/(const Vector4& other) const;
		Vector4 operator/(f32 scaler) const;

		Vector4& operator+=(const Vector4& other);
		Vector4& operator-=(const Vector4& other);
		Vector4& operator*=(f32 scaler);
		Vector4& operator/=(f32 scaler);

	}; // end Vector4
	//
	// Functions outside Vector4 struct
	inline Vector4& operator*(f32 scaler, const Vector4& vector) // scaler for the other side
	{
		return vector * scaler;
	}

	f32 dot(const Vector4& a, const Vector4& b);

	/* no cross product on vector4
	inline Vector4 cross(const Vector4& a, const Vector4& b) // 
	{
		return Vector4(a.y * b.z - b.y * a.z,  // x
					   a.z * b.x - b.x * a.y,  // y
					   a.x * b.y - b.x * a.y); // z
	}
	*/

	f32 lengthSquared(const Vector4& a);
	f32 length(const Vector4& a);
	Vector4 normalize(const Vector4& a);

	inline std::ostream& operator<<(std::ostream& os, const Vector4& v)
	{
		return os << "Vector4(" << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3] << ")";
	}
} // end Dunjun

#endif
