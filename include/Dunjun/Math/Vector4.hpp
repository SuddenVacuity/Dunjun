#ifndef  DUNJUN_MATH_VECTOR4_HPP
#define  DUNJUN_MATH_VECTOR4_HPP


#include <Dunjun/Math/Vector3.hpp>


namespace Dunjun
{
	struct Vector4
	{
		union // call info as v.x = 1;
		{
			struct
			{
				f32 x, y, z, w;
			};
			struct
			{
				f32 r, g, b, a;
			};
			f32 data[4];
	
			struct
			{
				Vector2 xy;
				Vector2 zw;
			};
	
			struct
			{
				Vector2 rg;
				Vector2 ba;
			};
	
			Vector3 xyz;
			Vector3 rgb;
		};
		GLOBAL const Vector4 Zero;

		inline f32& operator[](uSize_t index) { return data[index]; }
		inline const f32& operator[](uSize_t index) const { return data[index]; }
	};

	//inline f32& operator[](size_t index) { return data[index]; }
	//inline const f32& operator[](size_t index) const { return data[index]; } // this lets you call informatino in v[0] = 1; format

	bool operator==(const Vector4& a, const Vector4& b);
	bool operator!=(const Vector4& a, const Vector4& b);

	Vector4 operator-(const Vector4& a);

	Vector4 operator+(const Vector4& a, const Vector4& b);
	Vector4 operator-(const Vector4& a, const Vector4& b);

	Vector4 operator*(const Vector4& a, f32 scaler);
	Vector4 operator*(const Vector4& a, const Vector4& b);
	Vector4 operator/(const Vector4& a, const Vector4& b);
	Vector4 operator/(const Vector4& a, f32 scaler);

	Vector4& operator+=(Vector4& a, const Vector4& b);
	Vector4& operator-=(Vector4& a, const Vector4& b);
	Vector4& operator*=(Vector4& a, f32 scaler);
	Vector4& operator/=(Vector4& a, f32 scaler);

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
		return os << "Vector4(" << v.data[0] << ", " << v.data[1] << ", " << v.data[2] << ", " << v.data[3] << ")";
	}
} // end Dunjun

#endif
