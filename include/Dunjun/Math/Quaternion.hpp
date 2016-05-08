#ifndef DUNJUN_MATH_QUATERNION_HPP
#define DUNJUN_MATH_QUATERNION_HPP

#include <Dunjun/Math/Matrix4.hpp>

namespace Dunjun
{
	struct Quaternion
	{
		Quaternion();
		Quaternion(const Quaternion& q) = default;
		Quaternion(f32 x, f32 y, f32 z, f32 w);
		explicit Quaternion(const Vector3& v, f32 s);

		inline const f32 operator[](size_t index) const { return data[index]; }
		inline f32& operator[](size_t index) { return data[index]; }

		Quaternion operator-() const;
		Quaternion operator+(const Quaternion& b) const;
		Quaternion operator-(const Quaternion& b) const;
		Quaternion operator*(const Quaternion& b) const;
		Quaternion operator*(f32 s) const;
		Quaternion operator/(f32 s) const;

		bool operator==(const Quaternion& b) const;
		bool operator!= (const Quaternion& b) const;

		const Vector3 vector() const;
		Vector3& vector();

		f32 scaler() const;
		f32& scaler();

		f32 lengthSquared() const;
		f32 length() const;

		union
		{
			f32 data[4];
			struct
			{
				f32 x, y, z, w;
			};
			f32 xyz[3];
		};
	};
//
// functions outside of Quaternion struct
Quaternion operator*(f32 s, const Quaternion& q);

f32 dot(const Quaternion& a, const Quaternion& b);
Quaternion cross(const Quaternion& a, const Quaternion& b);
Quaternion normalize(const Quaternion& q);
Quaternion conjugate(const Quaternion& q);
Quaternion inverse(const Quaternion& q);

// rotate v by q
Vector3 operator*(const Quaternion& q, const Vector3& v);

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
//
// start working with angles
struct EulerAngles
{
	Radian pitch;
	Radian yaw;
	Radian roll;
};

Radian angle(const Quaternion& q);
Vector3 axis(const Quaternion& q);
Quaternion angleAxis(const Radian& angle, const Vector3& axis);

Radian roll(const Quaternion& q);
Radian pitch(const Quaternion& q);
Radian yaw(const Quaternion& q);

EulerAngles quaternionToEulerAngles(const Quaternion& q);
//
// Matrices
Matrix4 quaternionToMatrix4(const Quaternion& q);
// NOTE: assumes matrix is only a rotational matrix and has no skew applied (scale)
Quaternion matrix4ToQuaternion(const Matrix4& m);

} // end Dunjun

#endif
