#ifndef  DUNJUN_MATH_VECTOR4_HPP
#define  DUNJUN_MATH_VECTOR4_HPP

#include <Dunjun/Types.hpp>
#include <cmath>

#include <Dunjun/Math/Vector3.hpp>


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

		explicit Vector4(const Vector2& other, f32 z, f32 w) // to add zw component to xy format
			:Vector4(other.x, other.y, z, w)

		{
		}

		explicit Vector4(const Vector3& other, f32 w) // to add w component to xyz format
			:Vector4(other.x, other.y, other.z, w)
		{
		}

		// operators
		f32& operator[](size_t index) { return data[index]; }
		const f32& operator[](size_t index) const { return data[index]; } // this lets you call informatino in v[0] = 1; format

		bool operator==(const Vector4& other) const // comparison
		{
			for (size_t i = 0; i < 4; i++)
			{
				if (data[i] != other.data[i])
					return false;
			}
			return true;
		}

		bool operator!=(const Vector4& other) const
		{
			return !operator==(other);
		}

		Vector4 operator+(const Vector4& other) const // addition
		{
			return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
		}

		Vector4 operator-(const Vector4& other) const // subtraction
		{
			return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
		}

		Vector4 operator*(f32 scaler) const // scaler
		{
			return Vector4(scaler * x, scaler * y, scaler * z, scaler * w);
		}

		// Hadamard Product
		Vector4 operator*(const Vector4& other) const
		{
			Vector4 result;
			for (size_t i = 0; i < 4; i++)
				result[i] = data[i] * other.data[i];
			return result;
		}

		Vector4 operator/(f32 scaler) const // division scaler
		{
			return Vector4(x / scaler, y / scaler, z / scaler, w / scaler);
		}

		Vector4& operator+=(const Vector4& other) // addition
		{
			x += other.x;
			y += other.y;
			z += other.z;
			w += other.w;

			return *this;
		}

		Vector4& operator-=(const Vector4& other) // subtraction
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
			w -= other.w;

			return *this;
		}

		Vector4& operator*=(f32 scaler) // scaler
		{
			x *= scaler;
			y *= scaler;
			z *= scaler;
			w *= scaler;

			return *this;
		}

		Vector4& operator/=(f32 scaler) // scaler
		{
			x /= scaler;
			y /= scaler;
			z /= scaler;
			w /= scaler;

			return *this;
		}

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

	inline Vector4& operator*(f32 scaler, const Vector4& vector) // scaler for the other side
	{
		return vector * scaler;
	}

	inline f32 dot(const Vector4& a, const Vector4& b) // dot product
	{
		return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	}

	/* no cross product on vector4
	inline Vector4 cross(const Vector4& a, const Vector4& b) // 
	{
		return Vector4(a.y * b.z - b.y * a.z,  // x
					   a.z * b.x - b.x * a.y,  // y
					   a.x * b.y - b.x * a.y); // z
	}
	*/

	inline f32 lengthSquared(const Vector4& a)
	{
		return dot(a, a);
	}

	inline f32 length(const Vector4& a)
	{
		return std::sqrtf(lengthSquared(a));
	}

	inline Vector4 normalized(const Vector4& a) // normalize
	{
		return a * (1.0f / length(a));
	}

	inline std::ostream& operator<<(std::ostream& os, const Vector4& v)
	{
		return os << "Vector4(" << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3] << ")";
	}
}

#endif
