
#include <Dunjun/Math/Vector4.hpp>

namespace Dunjun
{
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
)				.
)					VECTOR 4 OPERATORS
)
)				.
)					.
)
)				.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

	bool operator==(const Vector4& a, const Vector4& b) // comparison
	{
		for (size_t i = 0; i < 4; i++)
		{
			if (a.data[i] != b.data[i])
				return false;
		}
		return true;
	}

	bool operator!=(const Vector4& a, const Vector4& b)
	{
		return !operator==(a, b);
	}

	Vector4 operator-(const Vector4& a) { return{ -a.x, -a.y, -a.z, a.w }; }

	Vector4 operator+(const Vector4& a, const Vector4& b) // addition
	{
		return {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
	}

	Vector4 operator-(const Vector4& a, const Vector4& b) // subtraction
	{
		return {a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
	}

	Vector4 operator*(const Vector4& a, f32 scaler) // scaler
	{
		return {scaler * a.x, scaler * a.y, scaler * a.z, scaler * a.w};
	}

	// Hadamard Product
	Vector4 operator*(const Vector4& a, const Vector4& b)
	{
		Vector4 result;
		for (size_t i = 0; i < 4; i++)
			result.data[i] = a.data[i] * b.data[i];
		return result;
	}

	Vector4 operator/(const Vector4& a, const Vector4& b)
	{
		Vector4 result;
		for (size_t i = 0; i < 4; i++)
			result.data[i] = a.data[i] / b.data[i];
		return result;
	}

	Vector4 operator/(const Vector4& a, f32 scaler) // division scaler
	{
		return {a.x / scaler, a.y / scaler, a.z / scaler, a.w / scaler};
	}

	Vector4& operator+=(Vector4& a, const Vector4& b) // addition
	{
		a.x += b.x;
		a.y += b.y;
		a.z += b.z;
		a.w += b.w;

		return a;
	}

	Vector4& operator-=(Vector4& a, const Vector4& b) // subtraction
	{
		a.x -= b.x;
		a.y -= b.y;
		a.z -= b.z;
		a.w -= b.w;

		return a;
	}

	Vector4& operator*=(Vector4& a, f32 scaler) // scaler
	{
		a.x *= scaler;
		a.y *= scaler;
		a.z *= scaler;
		a.w *= scaler;

		return a;
	}

	Vector4& operator/=(Vector4& a, f32 scaler) // scaler
	{
		a.x /= scaler;
		a.y /= scaler;
		a.z /= scaler;
		a.w /= scaler;

		return a;
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
