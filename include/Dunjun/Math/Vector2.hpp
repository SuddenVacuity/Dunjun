#ifndef  DUNJUN_MATH_VECTOR2_HPP
#define  DUNJUN_MATH_VECTOR2_HPP

#include <Dunjun/Math/Angle.hpp>

namespace Dunjun
{
	struct Vector2
	{
		Vector2()
			: x(0)
			, y(0)
		{
		}
		explicit Vector2(f32 xy)
			: x(xy)
			, y(xy)
		{
		}
		Vector2(f32 x, f32 y)
			: x(x)
			, y(y)
			{
			}
		Vector2(f32 xy[2])
			: x(xy[0])
			, y(xy[1])
		{
		}

		Vector2(const Vector2& other) = default; // copy constructor

		// operators
		inline f32& operator[](size_t index) { return data[index]; }
		inline const f32& operator[](size_t index) const { return data[index]; } // this lets you call informatino in v[0] = 1; format

		inline bool operator==(const Vector2& other) const // comparison
		{
			for (size_t i = 0; i < 2; i++)
			{
				if (data[i] != other.data[i])
					return false;
			}
			return true;
		}

		inline bool operator!=(const Vector2& other) const
		{
			return !operator==(other);
		}

		inline Vector2 operator-() const 
		{ return { -x, -y}; }

		inline Vector2 operator+(const Vector2& other ) const // addition
		{ return Vector2(x + other.x, y + other.y); }

		inline Vector2 operator-(const Vector2& other) const // subtraction
		{return Vector2( x - other.x, y - other.y );}

		inline Vector2 operator*(f32 scaler) const // scaler
		{
			return Vector2( scaler * x, scaler * y);
		}

		// Hadamard Product
		inline Vector2 operator*(const Vector2& other) const
		{
			Vector2 result;
			for (size_t i = 0; i < 2; i++)
				result[i] = data[i] * other.data[i];
			return result;
		}

		inline Vector2 operator/(const Vector2& other) const
		{
			Vector2 result;
			for (size_t i = 0; i < 2; i++)
				result[i] = data[i] / other.data[i];
			return result;
		}

		inline Vector2 operator/(f32 scaler) const // division scaler
		{
			return Vector2(x / scaler, y / scaler);
		}

		inline Vector2& operator+=(const Vector2& other) // addition
		{
			x += other.x;
			y += other.y;

			return *this;
		}

		inline Vector2& operator-=(const Vector2& other) // subtraction
		{
			x -= other.x;
			y -= other.y;

			return *this;
		}

		inline Vector2& operator*=(f32 scaler) // scaler
		{
			x *= scaler;
			y *= scaler;

			return *this;
		}

		inline Vector2& operator/=(f32 scaler) // scaler
		{
			x /= scaler;
			y /= scaler;

			return *this;
		}

		// =====================================================
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
	};

	inline Vector2& operator*(f32 scaler, const Vector2& vector) // scaler for the other side
	{
		return vector * scaler;
	}

	inline f32 dot(const Vector2& a, const Vector2& b) // dot product
	{
		return a.x * b.x + a.y * b.y;
	}

	inline f32 cross(const Vector2& a, const Vector2& b) // 
	{
		return a.x * b.y - b.x * a.y;
	}

	inline f32 lengthSquared(const Vector2& a)
	{
		return dot(a, a);
	}

	inline f32 length(const Vector2& a)
	{
		return std::sqrtf(lengthSquared(a));
	}

	inline Vector2 normalize(const Vector2& a) // normalize
	{
		return a * (1.0f / length(a));
	}

	inline std::ostream& operator<<(std::ostream& os, const Vector2& v)
	{
		return os << "Vector2(" << v[0] << ", " << v[1] << ")";
	}

}

#endif
