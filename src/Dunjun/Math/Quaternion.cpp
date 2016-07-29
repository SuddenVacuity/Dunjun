
#include <Dunjun/Math/Quaternion.hpp>

namespace Dunjun
{
	const Quaternion Quaternion::Identity = {0, 0, 0, 1};

	Quaternion operator-(const Quaternion& a)
	{
		Quaternion c = a;

		c.w = -a.w;
		c.x = -a.x;
		c.y = -a.y;
		c.z = -a.z;

		return c;
	}

	Quaternion operator+(const Quaternion& a, const Quaternion& b)
	{
		Quaternion c = a;

		c.w = a.w + b.w;
		c.x = a.x + b.x;
		c.y = a.y + b.y;
		c.z = a.z + b.z;

		return c;
	}

	Quaternion operator-(const Quaternion& a, const Quaternion& b)
	{
		Quaternion c = a;

		c.w = a.w - b.w;
		c.x = a.x - b.x;
		c.y = a.y - b.y;
		c.z = a.z - b.z;

		return c;
	}

	Quaternion operator*(const Quaternion& a, const Quaternion& b)
	{
		Quaternion c = a;

		c.x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y;
		c.y = a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x;
		c.z = a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w;
		c.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;

		return c;
	}

	Quaternion operator*(const Quaternion& a, f32 s)
	{
		Quaternion c = a;

		c.x *= s;
		c.y *= s;
		c.z *= s;
		c.w *= s;

		return c;
	}

	Quaternion operator/(const Quaternion& a, f32 s)
	{
		Quaternion c = a;

		c.x /= s;
		c.y /= s;
		c.z /= s;
		c.w /= s;

		return c;
	}

	bool operator==(const Quaternion& a, const Quaternion& b)
	{
		for (size_t i = 0; i < 4; i++)
		{
			if (a.data[i] != b.data[i])
				return false;
		}
		return true;
	}

	bool operator!= (const Quaternion& a, const Quaternion& b)
	{
		return !operator==(a, b);
	}

	//const Vector3 vector(const Quaternion& a) { return reinterpret_cast<const Vector3&>(a.data); }
	//Vector3& vector(Quaternion& a) { return reinterpret_cast<Vector3&>(a.data); }
	//
	//const f32 scaler(const Quaternion& a) { return a.w; }
	//f32& scaler(Quaternion& a) { return a.w; }

	f32 lengthSquared(const Quaternion& q)
	{
		return dot(q, q);
	}

	f32 length(const Quaternion& q)
	{
		return Math::sqrt(lengthSquared(q));
	}
	/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	)				.
	)					FUNCTIONS OUTSIDE OF QUATERNION STRUCT
	)
	)				.
	)					.
	)
	)				.
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/


	Quaternion operator*(f32 s, const Quaternion& q)
	{
		return q * s;
	}

	f32 dot(const Quaternion& a, const Quaternion& b)
	{
		return dot(Vector3{a.x, a.y, a.z}, 
				   Vector3{b.x, b.y, b.z}) +
				   a.w * b.w;
	}

	Quaternion cross(const Quaternion& a, const Quaternion& b)
	{
		return Quaternion{
			a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
			a.w * b.y + a.y * b.w + a.z * b.x - a.x * b.z,
			a.w * b.z + a.z * b.w + a.x * b.y - a.y * b.x,
			a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z
		};

	}

	Quaternion normalize(const Quaternion& q)
	{
		return q * (1.0f / length(q));
	}

	Quaternion conjugate(const Quaternion& q)
	{
		Quaternion c{-q.x, -q.y, -q.z, q.w};
		return c;
	}

	Quaternion inverse(const Quaternion& q)
	{
		Quaternion c = conjugate(q) / dot(q, q);
		return c;
	}

	Vector3 operator*(const Quaternion& q, const Vector3& v)
	{
		// return q * quaternion(v, 0) * conjugate(q) // More expensive
		Vector3 t = 2.0f * cross(Vector3{q.x, q.y, q.z}, v);
		return (v + q.w * t + cross(Vector3{q.x, q.y, q.z}, t));
	}

	/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	)				.
	)					START WORKING WITH ANGLES
	)
	)				.
	)					.
	)
	)				.
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

	Radian angle(const Quaternion& q)
	{
		return Radian(2.0f * Math::acos(q.w));
	}

	Vector3 axis(const Quaternion& q)
	{
		const f32 s2 = 1.0f - q.w * q.w; // 1 - sqrt(cos(theta)) = sqrt(sin(theta))
		if (s2 <= 0.0f)
			return Vector3{0, 0, 1};

		const f32 invs2 = 1.0f / Math::sqrt(s2);

		return Vector3{q.x, q.y, q.z} * invs2;
	}

	Quaternion angleAxis(const Radian& angle, const Vector3& axis)
	{
		Quaternion q;

		Vector3 a = normalize(axis);

		const f32 s = Math::sin(0.5f * angle);

		q.xyz = a * s;
		q.w = Math::cos(0.5f * angle);

		return q;
	}

	Radian roll(const Quaternion& q)
	{
		return Radian(Math::atan2(2.0f * q.data[0] * q.data[1] + q.data[2] * q.data[3],
										 q.data[0] * q.data[0] + q.data[3] * q.data[3] -
										 q.data[1] * q.data[1] - q.data[2] * q.data[2]));
	}

	Radian pitch(const Quaternion& q)
	{
		return Radian(Math::atan2(2.0f * q.data[1] * q.data[2] + q.data[3] * q.data[0],
										 q.data[3] * q.data[3] - q.data[0] * q.data[0] -
										 q.data[1] * q.data[1] + q.data[2] * q.data[2]));
	}

	Radian yaw(const Quaternion& q)
	{
		return Radian(Math::asin(-2.0f * (q.data[0] * q.data[2] - q.data[3] * q.data[1])));
	}

	EulerAngles quaternionToEulerAngles(const Quaternion& q)
	{
		return{ pitch(q), yaw(q), roll(q) };
	}

	/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	)				.
	)					MATRICES
	)
	)				.
	)					.
	)
	)				.
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

	Matrix4 quaternionToMatrix4(const Quaternion& q)
	{
		Matrix4 mat = Matrix4::Identity;

		Quaternion a = normalize(q);

		const f32 xx = a.x * a.x;
		const f32 yy = a.y * a.y;
		const f32 zz = a.z * a.z;
		const f32 xy = a.x * a.y;
		const f32 xz = a.x * a.z;
		const f32 yz = a.y * a.z;
		const f32 wx = a.w * a.x;
		const f32 wy = a.w * a.y;
		const f32 wz = a.w * a.z;

		mat.data[0].data[0] = 1.0f - 2.0f * (yy + zz);
		mat.data[0].data[1] = 2.0f * (xy + wz);
		mat.data[0].data[2] = 2.0f * (xz - wy);

		mat.data[1].data[0] = 2.0f * (xy - wz);
		mat.data[1].data[1] = 1.0f - 2.0f * (xx + zz);
		mat.data[1].data[2] = 2.0f * (yz + wx);

		mat.data[2].data[0] = 2.0f * (xz + wy);
		mat.data[2].data[1] = 2.0f * (yz - wx);
		mat.data[2].data[2] = 1.0f - 2.0f * (xx + yy);

		return mat;
	}

	// NOTE: assumes matrix is only a rotational matrix and has no skew applied (scale)
	Quaternion matrix4ToQuaternion(const Matrix4& m)
	{
		const f32 fourXSquaredMinus1 = m.data[0].data[0] - m.data[1].data[1] - m.data[2].data[2];
		const f32 fourYSquaredMinus1 = m.data[1].data[1] - m.data[0].data[0] - m.data[2].data[2];
		const f32 fourZSquaredMinus1 = m.data[2].data[2] - m.data[0].data[0] - m.data[1].data[1];
		const f32 fourWSquaredMinus1 = m.data[0].data[0] + m.data[1].data[1] + m.data[2].data[2];

		int biggestIndex = 0;
		f32 fourBiggestSquaredMinus1 = fourWSquaredMinus1;

		if (fourXSquaredMinus1 > fourBiggestSquaredMinus1)
		{
			fourBiggestSquaredMinus1 = fourXSquaredMinus1;
			biggestIndex = 1;
		}

		if (fourYSquaredMinus1 > fourBiggestSquaredMinus1)
		{
			fourBiggestSquaredMinus1 = fourYSquaredMinus1;
			biggestIndex = 2;
		}

		if (fourZSquaredMinus1 > fourBiggestSquaredMinus1)
		{
			fourBiggestSquaredMinus1 = fourZSquaredMinus1;
			biggestIndex = 3;
		}

		const f32 biggestVal = Math::sqrt(fourBiggestSquaredMinus1 + 1.0f) * 0.5f;
		const f32 mult = 0.25f / biggestVal;

		Quaternion q;

		switch (biggestIndex)
		{
		case 0:
		{
			q.w = biggestVal;
			q.x = (m.data[1].data[2] - m.data[2].data[1]) * mult;
			q.y = (m.data[2].data[0] - m.data[0].data[2]) * mult;
			q.z = (m.data[0].data[1] - m.data[1].data[0]) * mult;
		}
		break;
		case 1:
		{
			q.w = (m.data[1].data[2] - m.data[2].data[1]) * mult;
			q.x = biggestVal;
			q.y = (m.data[0].data[1] + m.data[1].data[0]) * mult;
			q.z = (m.data[2].data[0] + m.data[0].data[2]) * mult;

		}
		break;
		case 2:
		{
			q.w = (m.data[2].data[0] - m.data[0].data[2]) * mult;
			q.x = (m.data[0].data[1] + m.data[1].data[0]) * mult;
			q.y = biggestVal;
			q.z = (m.data[1].data[2] + m.data[2].data[1]) * mult;
		}
		break;
		case 3:
		{
			q.w = (m.data[0].data[1] - m.data[1].data[0]) * mult;
			q.x = (m.data[2].data[0] + m.data[0].data[2]) * mult;
			q.y = (m.data[1].data[2] + m.data[2].data[1]) * mult;
			q.z = biggestVal;
		}
		break;
		default: // should never get here
		{
			assert(false && 
				   "Quaternion::matrix4ToQuaternion This should never happen");
		}
		break;
		}


		return q;
	}

	Quaternion eulerAnglesToQuaternion(const EulerAngles& e,
									   const Vector3& xAxis,
									   const Vector3& yAxis,
									   const Vector3& zAxis)
	{
		Quaternion p = angleAxis(e.pitch, xAxis);
		Quaternion y = angleAxis(e.yaw, yAxis);
		Quaternion r = angleAxis(e.roll, zAxis);

		return y * p * r;
	}

	Quaternion offsetOrientation(const Quaternion& orientation, const Radian& yaw, const Radian& pitch)
	{
		const Quaternion yawRot = angleAxis(yaw, { 0, 1, 0 }); // absolute up
		const Quaternion pitchRot = angleAxis(pitch, rightVector(orientation)); // relative right

		return yawRot * pitchRot * orientation;
	}


	// must be normalized
	// returns vector multiplied by orientation
	// while moving in 3d space this will return a vector3
	// that will cause you to move forward relative to current orientation
	Vector3 forwardVector(const Quaternion& orientation)
	{
		return orientation * Vector3{ 0, 0, -1 };
	}
	// must be normalized
	// returns vector multiplied by orientation
	// while moving in 3d space this will return a vector3
	// that will cause you to move backward relative to current orientation
	Vector3 backwardVector(const Quaternion& orientation)
	{
		return orientation * Vector3{ 0, 0, 1 };
	}

	// must be normalized
	// returns vector multiplied by orientation
	// while moving in 3d space this will return a vector3
	// that will cause you to move right relative to current orientation
	Vector3 rightVector(const Quaternion& orientation)
	{
		return orientation * Vector3{ 1, 0, 0 };
	}
	// must be normalized
	// returns vector multiplied by orientation
	// while moving in 3d space this will return a vector3
	// that will cause you to move left relative to current orientation
	Vector3 leftVector(const Quaternion& orientation)
	{
		return orientation * Vector3{ -1, 0, 0 };
	}

	// must be normalized
	// returns vector multiplied by orientation
	// while moving in 3d space this will return a vector3
	// that will cause you to move up relative to current orientation
	Vector3 upVector(const Quaternion& orientation)
	{
		return orientation * Vector3{ 0, 1, 0 };
	}
	// must be normalized
	// returns vector multiplied by orientation
	// while moving in 3d space this will return a vector3
	// that will cause you to move down relative to current orientation
	Vector3 downVector(const Quaternion& orientation)
	{
		return orientation * Vector3{ 0, -1, 0 };
	}











} // end Dunjun
