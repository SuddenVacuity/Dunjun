#ifndef DUNJUN_MATH_FUNCTIONS_MATRIX_HPP
#define DUNJUN_MATH_FUNCTIONS_MATRIX_HPP

#include <Dunjun/Math/Vector3.hpp>
#include <Dunjun/Math/Matrix4.hpp>
#include <Dunjun/Math/Angle.hpp>

#include <cmath>

namespace Dunjun
{
	inline Matrix4 translate(const Vector3& v)
	{
		Matrix4 result;
		result[3] = Vector4(v, 1);

		return result;
	}

	inline Matrix4 rotate(const Radian& angle, const Vector3& v) // angle is in radians, v is the axis to rotate around
	{
		const f32 c = std::cos(static_cast<f32>(angle));
		const f32 s = std::sin(static_cast<f32>(angle));

		const Vector3 axis(normalize(v));
		const Vector3 t = (1.0f - c) * axis;

		Matrix4 rot;
		rot[0][0] = c + t[0] * axis[0]; // x component
		rot[0][1] = 0 + t[0] * axis[1] + s * axis[2];
		rot[0][2] = 0 + t[0] * axis[2] - s * axis[1];
		rot[0][3] = 0;

		rot[1][0] = 0 + t[1] * axis[0] - s * axis[2]; // y component
		rot[1][1] = c + t[1] * axis[1];
		rot[1][2] = 0 + t[1] * axis[2] + s * axis[0];
		rot[1][3] = 0;

		rot[2][0] = 0 + t[2] * axis[0] - s * axis[1]; // z component
		rot[2][1] = 0 + t[2] * axis[1] + s * axis[0];
		rot[2][2] = c + t[2] * axis[2];
		rot[2][3] = 0;

		//Matrix4 result;
		//Matrix4 i;

		//result[0] = i[0] * rot[0];
		//result[1] = i[1] * rot[1];
		//result[2] = i[2] * rot[2];
		//result[3] = i[3] * rot[3];

		return rot;
	}

	inline Matrix4 scale(const Vector3& v)
	{
		Matrix4 result({v.x, 0, 0, 0}, {0, v.y, 0, 0}, {0, 0, v.z, 0}, {0, 0, 0, 1});
		return result;
	}

	// view handling functions in Functions/Matrix.cpp
	Matrix4 ortho(f32 left, f32 right, f32 bottom, f32 top);
	Matrix4 ortho(f32 left, f32 right, f32 bottom, f32 top, f32 zNear, f32 zFar);
	
	// angle is in radians
	Matrix4 perspective(const Radian& fovy, f32 aspect, f32 zNear, f32 zFar);
	Matrix4 infinitePerspective(const Radian& fovy, f32 aspect, f32 zNear);

	Matrix4 lookAt(const Vector3& eye, const Vector3& center, const Vector3& up);
}
#endif
