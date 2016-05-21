
#include <Dunjun/Math/Vector2.hpp>

namespace Dunjun
{
	Vector2::Vector2()
			: x(0)
			, y(0)
		{
		}
	Vector2::Vector2(f32 xy)
			: x(xy)
			, y(xy)
		{
		}
	Vector2::Vector2(f32 x, f32 y)
			: x(x)
			, y(y)
		{
		}
	Vector2::Vector2(f32 xy[2])
			: x(xy[0])
			, y(xy[1])
		{
		}

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
)				.
)					VECTOR 2 OPERATORS
)
)				.
)					.
)
)				.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

		bool Vector2::Vector2::operator==(const Vector2& other) const // comparison
		{
			for (size_t i = 0; i < 2; i++)
			{
				if (data[i] != other.data[i])
					return false;
			}
			return true;
		}

		bool Vector2::operator!=(const Vector2& other) const
		{
			return !operator==(other);
		}

		Vector2 Vector2::operator-() const
		{
			return Vector2(-x, -y);
		}

		Vector2 Vector2::operator+(const Vector2& other) const // addition
		{
			return Vector2(x + other.x, y + other.y);
		}

		Vector2 Vector2::operator-(const Vector2& other) const // subtraction
		{
			return Vector2(x - other.x, y - other.y);
		}

		Vector2 Vector2::operator*(f32 scaler) const // scaler
		{
			return Vector2(scaler * x, scaler * y);
		}

		// Hadamard Product
		Vector2 Vector2::operator*(const Vector2& other) const
		{
			Vector2 result;
			for (size_t i = 0; i < 2; i++)
				result[i] = data[i] * other.data[i];
			return result;
		}

		Vector2 Vector2::operator/(const Vector2& other) const
		{
			Vector2 result;
			for (size_t i = 0; i < 2; i++)
				result[i] = data[i] / other.data[i];
			return result;
		}

		Vector2 Vector2::operator/(f32 scaler) const // division scaler
		{
			return Vector2(x / scaler, y / scaler);
		}

		Vector2& Vector2::operator+=(const Vector2& other) // addition
		{
			x += other.x;
			y += other.y;

			return *this;
		}

		Vector2& Vector2::operator-=(const Vector2& other) // subtraction
		{
			x -= other.x;
			y -= other.y;

			return *this;
		}

		Vector2& Vector2::operator*=(f32 scaler) // scaler
		{
			x *= scaler;
			y *= scaler;

			return *this;
		}

		Vector2& Vector2::operator/=(f32 scaler) // scaler
		{
			x /= scaler;
			y /= scaler;

			return *this;
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
