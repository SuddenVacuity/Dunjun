
#include <Dunjun/Math/Vector4.hpp>

namespace Dunjun
{
	Vector4::Vector4()
		: x(0)
		, y(0)
		, z(0)
		, w(0)
	{
	}
	Vector4::Vector4(f32 xyzw)
		: x(xyzw)
		, y(xyzw)
		, z(xyzw)
		, w(xyzw)
	{
	}
	Vector4::Vector4(f32 x, f32 y, f32 z, f32 w) // explicit means you must write Vector2 to get the info
		: x(x)
		, y(y)
		, z(z)
		, w(w)
	{
	}
	Vector4::Vector4(f32 xyzw[4])
		: x(xyzw[0])
		, y(xyzw[1])
		, z(xyzw[2])
		, w(xyzw[3])
	{
	}

	Vector4::Vector4(const Vector2& other, f32 z, f32 w) // to add zw component to xy format
		:Vector4(other.x, other.y, z, w)

	{
	}

	Vector4::Vector4(const Vector3& other, f32 w) // to add w component to xyz format
		:Vector4(other.x, other.y, other.z, w)
	{
	}

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
)				.
)					VECTOR 4 OPERATORS
)
)				.
)					.
)
)				.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

	bool Vector4::operator==(const Vector4& other) const // comparison
	{
		for (size_t i = 0; i < 4; i++)
		{
			if (data[i] != other.data[i])
				return false;
		}
		return true;
	}

	bool Vector4::operator!=(const Vector4& other) const
	{
		return !operator==(other);
	}

	Vector4 Vector4::operator-() const { return{ -x, -y, -z, w }; }

	Vector4 Vector4::operator+(const Vector4& other) const // addition
	{
		return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
	}

	Vector4 Vector4::operator-(const Vector4& other) const // subtraction
	{
		return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
	}

	Vector4 Vector4::operator*(f32 scaler) const // scaler
	{
		return Vector4(scaler * x, scaler * y, scaler * z, scaler * w);
	}

	// Hadamard Product
	Vector4 Vector4::operator*(const Vector4& other) const
	{
		Vector4 result;
		for (size_t i = 0; i < 4; i++)
			result[i] = data[i] * other.data[i];
		return result;
	}

	Vector4 Vector4::operator/(const Vector4& other) const
	{
		Vector4 result;
		for (size_t i = 0; i < 4; i++)
			result[i] = data[i] / other.data[i];
		return result;
	}

	Vector4 Vector4::operator/(f32 scaler) const // division scaler
	{
		return Vector4(x / scaler, y / scaler, z / scaler, w / scaler);
	}

	Vector4& Vector4::operator+=(const Vector4& other) // addition
	{
		x += other.x;
		y += other.y;
		z += other.z;
		w += other.w;

		return *this;
	}

	Vector4& Vector4::operator-=(const Vector4& other) // subtraction
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		w -= other.w;

		return *this;
	}

	Vector4& Vector4::operator*=(f32 scaler) // scaler
	{
		x *= scaler;
		y *= scaler;
		z *= scaler;
		w *= scaler;

		return *this;
	}

	Vector4& Vector4::operator/=(f32 scaler) // scaler
	{
		x /= scaler;
		y /= scaler;
		z /= scaler;
		w /= scaler;

		return *this;
	}

	/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	)				.
	)					FUNCTIONS OUTSIDE VECTOR 4 STRUCT
	)
	)				.
	)					.
	)
	)				.
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

	f32 dot(const Vector4& a, const Vector4& b) // dot product
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

	f32 lengthSquared(const Vector4& a)
	{
		return dot(a, a);
	}
	
	f32 length(const Vector4& a)
	{
		return Math::sqrt(lengthSquared(a));
	}
	
	Vector4 normalize(const Vector4& a) // normalize
	{
		return a * (1.0f / length(a));
	}
} // end Dunjun
