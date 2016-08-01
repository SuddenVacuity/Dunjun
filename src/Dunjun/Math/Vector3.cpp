
#include <Dunjun/Math/Vector3.hpp>

namespace Dunjun
{
	const Vector3 Vector3::Zero = {0, 0, 0};
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
)				.
)					VECTOR 3 OPERATORS
)
)				.
)					.
)
)				.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

	bool operator==(const Vector3& a, const Vector3& b) // comparison
	{
		for (size_t i = 0; i < 3; i++)
		{
			if (a.data[i] != b.data[i])
				return false;
		}
		return true;
	}

	bool operator!=(const Vector3& a, const Vector3& b)
	{
		return !operator==(a, b);
	}

	Vector3 operator-(const Vector3& a) { return {-a.x, -a.y, -a.z}; }

	Vector3 operator+(const Vector3& a, const Vector3& b) // addition
	{
		return {a.x + b.x, a.y + b.y, a.z + b.z};
	}

	Vector3 operator-(const Vector3& a, const Vector3& b) // subtraction
	{
		return {a.x - b.x, a.y - b.y, a.z - b.z};
	}

	Vector3 operator*(const Vector3& a, f32 scaler) // scaler
	{
		return {scaler * a.x, scaler * a.y, scaler * a.z};
	}

	// Hadamard Product
	Vector3 operator*(const Vector3& a, const Vector3& b)
	{
		Vector3 result;
		for (size_t i = 0; i < 3; i++)
			result.data[i] = a.data[i] * b.data[i];
		return result;
	}

	Vector3 operator/(const Vector3& a, const Vector3& b)
	{
		Vector3 result;
		for (size_t i = 0; i < 3; i++)
			result.data[i] = a.data[i] / b.data[i];
		return result;
	}


	Vector3 operator/(const Vector3& a, f32 scaler) // division scaler
	{
		return {a.x / scaler, a.y / scaler, a.z / scaler};
	}

	Vector3& operator+=(Vector3& a, const Vector3& b) // addition
	{
		a.x += b.x;
		a.y += b.y;
		a.z += b.z;

		return a;
	}

	Vector3& operator-=(Vector3& a, const Vector3& b) // subtraction
	{
		a.x -= b.x;
		a.y -= b.y;
		a.z -= b.z;

		return a;
	}

	Vector3& operator*=(Vector3& a, f32 scaler) // scaler
	{
		a.x *= scaler;
		a.y *= scaler;
		a.z *= scaler;

		return a;
	}

	Vector3& operator/=(Vector3& a, f32 scaler) // scaler
	{
		a.x /= scaler;
		a.y /= scaler;
		a.z /= scaler;

		return a;
	}

	/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	)				.
	)					FUNCTIONS
	)
	)				.
	)					.
	)
	)				.
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

	f32 dot(const Vector3& a, const Vector3& b) // dot product
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}
	
	Vector3 cross(const Vector3& a, const Vector3& b) // 
	{
		return {a.y * b.z - b.y * a.z,  // x
				a.z * b.x - b.z * a.x,  // y
				a.x * b.y - b.x * a.y}; // z
	}
	
	f32 lengthSquared(const Vector3& a)
	{
		return dot(a, a);
	}
	
	f32 length(const Vector3& a)
	{
		return Math::sqrt(lengthSquared(a));
	}
	
	Vector3 normalize(const Vector3& a) // normalize
	{
		return a * (1.0f / length(a));
	}
} // end Dunjun
