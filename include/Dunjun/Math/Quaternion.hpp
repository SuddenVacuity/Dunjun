#ifndef DUNJUN_MATH_QUATERNION_HPP
#define DUNJUN_MATH_QUATERNION_HPP

#include <Dunjun/Math/Matrix4.hpp>

namespace Dunjun
{
	struct Quaternion
	{
		union 
		{
			struct
			{
				f32 x, y, z, w;
			};
			Vector3 xyz;
			f32 data[4];
		};
		GLOBAL const Quaternion Identity;
	};


	//inline const f32 operator[](const Quaternion& a, size_t index) { return data[index]; }
	//inline f32& operator[](const Quaternion& a, size_t index) { return data[index]; }

	Quaternion operator-(const Quaternion& a);
	Quaternion operator+(const Quaternion& a, const Quaternion& b);
	Quaternion operator-(const Quaternion& a, const Quaternion& b);
	Quaternion operator*(const Quaternion& a, const Quaternion& b);
	Quaternion operator*(const Quaternion& a, f32 s);
	Quaternion operator/(const Quaternion& a, f32 s);

	bool operator==(const Quaternion& a, const Quaternion& b);
	bool operator!=(const Quaternion& a, const Quaternion& b);

	//const Vector3 vector(const Quaternion& a);
	//Vector3& vector(Quaternion& a);
	//
	//f32 scaler(const Quaternion& a);
	//f32& scaler(Quaternion& a);
//
// functions outside of Quaternion struct
Quaternion operator*(f32 s, const Quaternion& q);

f32 dot(const Quaternion& a, const Quaternion& b);
Quaternion cross(const Quaternion& a, const Quaternion& b);

f32 lengthSquared(const Quaternion& q);
f32 length(const Quaternion& q);

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
		os << q.data[i];
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

inline Radian roll(const Quaternion& q);
inline Radian pitch(const Quaternion& q);
inline Radian yaw(const Quaternion& q);

inline EulerAngles quaternionToEulerAngles(const Quaternion& q);
inline Quaternion eulerAnglesToQuaternion(const EulerAngles& e,
										  const Vector3& xAxis = { 1, 0, 0 },
										  const Vector3& yAxis = { 0, 1, 0 },
										  const Vector3& zAxis = { 0, 0, 1 });

inline Quaternion eulerAnglesToQuaternion(const Radian& pitch, const Radian& yaw, const Radian& roll,
										  const Vector3& xAxis = { 1, 0, 0 },
										  const Vector3& yAxis = { 0, 1, 0 },
										  const Vector3& zAxis = { 0, 0, 1 })
{
	return eulerAnglesToQuaternion({pitch, yaw, roll}, xAxis, yAxis, zAxis);
}

//
// Matrices
Matrix4 quaternionToMatrix4(const Quaternion& q);
// NOTE: assumes matrix is only a rotational matrix and has no shear applied (scale)
Quaternion matrix4ToQuaternion(const Matrix4& m);


// positive values go counterclock-wise
Quaternion offsetOrientation(const Quaternion& orientation, 
							 const Radian& yaw, const Radian& pitch);

// returns vector multiplied by orientation
Vector3 forwardVector(const Quaternion& orientation);
// returns vector multiplied by orientation
Vector3 backwardVector(const Quaternion& orientation);

// returns vector multiplied by orientation
Vector3 rightVector(const Quaternion& orientation);
// returns vector multiplied by orientation
Vector3 leftVector(const Quaternion& orientation);

// returns vector multiplied by orientation
Vector3 upVector(const Quaternion& orientation);
// returns vector multiplied by orientation
Vector3 downVector(const Quaternion& orientation);




} // end Dunjun

#endif
