
#include <Dunjun/Math/Vector2.hpp>

namespace Dunjun
{
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
)				.
)					VECTOR 2 OPERATORS
)
)				.
)					.
)
)				.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

		bool operator==(const Vector2& a, const Vector2& b) // comparison
		{
			for (size_t i = 0; i < 2; i++)
			{
				if (a.data[i] != b.data[i])
					return false;
			}
			return true;
		}

		bool operator!=(const Vector2& a, const Vector2& b)
		{
			return !operator==(a, b);
		}

		Vector2 operator-(const Vector2& a)
		{
			return {-a.x, -a.y};
		}

		Vector2 operator+(const Vector2& a, const Vector2& b) // addition
		{
			return {a.x + b.x, a.y + b.y};
		}

		Vector2 operator-(const Vector2& a, const Vector2& b) // subtraction
		{
			return {a.x - b.x, a.y - b.y};
		}

		Vector2 operator*(const Vector2& a, f32 scaler) // scaler
		{
			return {scaler * a.x, scaler * a.y};
		}

		// Hadamard Product
		Vector2 operator*(const Vector2& a, const Vector2& b)
		{
			Vector2 result;
			for (size_t i = 0; i < 2; i++)
				result.data[i] = a.data[i] * b.data[i];
			return result;
		}

		Vector2 operator/(const Vector2& a, const Vector2& b)
		{
			Vector2 result;
			for (size_t i = 0; i < 2; i++)
				result.data[i] = a.data[i] / b.data[i];
			return result;
		}

		Vector2 operator/(const Vector2& a, f32 scaler) // division scaler
		{
			return {a.x / scaler, a.y / scaler};
		}

		Vector2& operator+=(Vector2& a, const Vector2& b) // addition
		{
			a.x += b.x;
			a.y += b.y;

			//return *this;
			return a;
		}

		Vector2& operator-=(Vector2& a, const Vector2& b) // subtraction
		{
			a.x -= b.x;
			a.y -= b.y;

			//return *this;
			return a;
		}

		Vector2& operator*=(Vector2& a, f32 scaler) // scaler
		{
			a.x *= scaler;
			a.y *= scaler;
			
			//return *this;
			return a;
		}

		Vector2& operator/=(Vector2& a, f32 scaler) // scaler
		{
			a.x /= scaler;
			a.y /= scaler;
			    
			//return *this;
			return a;
		}

	/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	)				.
	)					FUNCTIONS OUTSIDE VECTOR 2 STRUCT
	)
	)				.
	)					.
	)
	)				.
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

	f32 dot(const Vector2& a, const Vector2& b) // dot product
	{
		return a.x * b.x + a.y * b.y;
	}

	f32 cross(const Vector2& a, const Vector2& b) // 
	{
		return a.x * b.y - b.x * a.y;
	}

	f32 lengthSquared(const Vector2& a)
	{
		return dot(a, a);
	}

	f32 length(const Vector2& a)
	{
		return Math::sqrt(lengthSquared(a));
	}

	Vector2 normalize(const Vector2& a) // normalize
	{
		return a * (1.0f / length(a));
	}

}
