#ifndef DUNJUN_MATH_QUATERNION_HPP
#define DUNJUN_MATH_QUATERNION_HPP

#include <Dunjun/Math/Functions/Matrix.hpp>

#include <ostream>

namespace Dunjun
{
	struct Quaternion
	{
		// GLS, vec4 = xyzw
		Vector3 xyz = Vector3(0, 0, 0);
		f32 w = 1.0f;

		Quaternion()
			: xyz(0, 0, 0)
			, w(1)
		{
		}

		Quaternion(const Quaternion& q) = default;

		/*explicit*/ Quaternion(const Vector3& v, f32 s)
			: xyz(v)
			, w(s)
		{
		}

		f32 operator[](size_t index) const
		{
			if (index < 3)
				return xyz[index];
			return w;
		}

		f32& operator[](size_t index)
		{
			if (index < 3)
				return xyz[index];
			return w;
		}

		Quaternion operator-() const
		{
			Quaternion c;

			c.w = -w;
			c.xyz = -xyz;

			return c;
		}

		Quaternion operator+(const Quaternion& b) const
		{
			Quaternion c;

			c.w = w + b.w;
			c.xyz = xyz + b.xyz;

			return c;
		}

		Quaternion operator-(const Quaternion& b) const
		{
			Quaternion c;

			c.w = w - b.w;
			c.xyz = xyz - b.xyz;

			return c;
		}

		Quaternion operator*(const Quaternion& b) const
		{
			const Quaternion& a = *this;
			Quaternion c;

			c.xyz = a.w * b.xyz + b.w * a.xyz + cross(a.xyz, b.xyz);
			c.w = a.w * b.w - dot(a.xyz, b.xyz);

			return c;
		}

		Quaternion operator*(f32 s) const
		{
			Quaternion c = *this;

			c.xyz *= s;
			c.w *= s;

			return c;
		}

		Quaternion operator/(f32 s) const
		{
			Quaternion c = *this;

			c.xyz /= s;
			c.w /= s;

			return c;
		}

		bool operator==(const Quaternion& b) const
		{
			for(size_t i = 0; i < 4; i++)
			{
				if(operator[](i) != b[i])
					return false;
			}
			return true;
		}

		bool operator!= (const Quaternion& b) const
		{
			return !operator==(b);
		}

		f32 lengthSquared() const
		{
			return dot(xyz , xyz) + w * w;
		}

		f32 length() const
		{
			return std::sqrt(lengthSquared());
		}



	};


	// functions

inline Quaternion operator*(f32 s, const Quaternion& q)
{
	return q * s;
}

inline f32 dot(const Quaternion& a, const Quaternion& b)
{
	return dot(a.xyz, b.xyz) + a.w * b.w;
}

inline Quaternion normalize(const Quaternion& q)
{
	return q * (1.0f / q.length());
}

inline Quaternion conjugate(const Quaternion& q)
{
	Quaternion c(-q.xyz, q.w);
	return c;
}

inline Quaternion inverse(const Quaternion& q)
{
	Quaternion c = conjugate(q) / dot(q, q);
	return c;
}

inline Vector3 operator*(const Quaternion& q, const Vector3& v)
{
	// return q * quaternion(v, 0) * conjugate(q) // More expensive
	Vector3 t = 2.0f * cross(q.xyz, v);
	return (v + q.w * t + cross(q.xyz, t));
}






inline std::ostream& operator<<(std::ostream& os, const Quaternion& q)
{
	os << "Quaternion(";
	for(size_t i = 0; i < 4; i++)
	{
		os << q[i];
		if (i < 3)
			os << ", ";
	}
	os << ")";

	return os;

}


inline Radian angle(const Quaternion& q)
{
	return Radian(2.0f * std::acos(q.w));
}

inline Vector3 axis(const Quaternion& q)
{
	f32 s2 = 1.0f - q.w * q.w; // 1 - sqrt(cos(theta)) = sqrt(sin(theta))
	if(s2 <- 0)
		return Vector3(0,0,1);

	f32 invs2 = 1.0f / std::sqrt(s2); 

	return q.xyz * invs2;
}

inline Quaternion angleAxis(const Radian& angle, const Vector3& axis)
{
	Quaternion q;

	const Vector3 a = normalize(axis);

	const f32 s = std::sin((f32)(0.5f * angle));

	q.xyz = a * s;
	q.w = std::cos((f32)(0.5f * angle));

	return q;
}

struct EulerAngles
{
	Radian pitch;
	Radian yaw;
	Radian roll;
};

inline Radian roll(const Quaternion& q)
{
	return Radian(std::atan2(2.0f * q[0] * q[1] + q[2] * q[3],
		q[0] * q[0] + q[3] * q[3] - q[1] * q[1] - q[2] * q[2]));
}

inline Radian pitch(const Quaternion& q)
{
	return Radian(std::atan2(2.0f * q[1] * q[2] + q[3] * q[0],
		q[3] * q[3] - q[0] * q[0] - q[1] * q[1] + q[2] * q[2]));
}

inline Radian yaw(const Quaternion& q)
{
	return Radian(std::asin(-2.0f * (q[0] * q[2] - q[3] * q[1])));
}

inline EulerAngles quaternionToEulerAngles(const Quaternion& q)
{
	return {pitch(q), yaw(q), roll(q)};
}

} // end Dunjun

#endif
