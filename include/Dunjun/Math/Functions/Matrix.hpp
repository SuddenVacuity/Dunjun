#ifndef DUNJUN_MATH_FUNCTIONS_MATRIX_HPP
#define DUNJUN_MATH_FUNCTIONS_MATRIX_HPP

#include <Dunjun/Math/Quaternion.hpp>

namespace Dunjun
{
namespace Math
{
	// matrix transform handling functions
	Matrix4 translate(const Vector3& v);
	Matrix4 rotate(const Radian& angle, const Vector3& v); // angle is in radians, v is the axis to rotate around
	Matrix4 scale(const Vector3& v);

	// view handling functions in Functions/Matrix.cpp
	Matrix4 ortho(f32 left, f32 right, f32 bottom, f32 top);
	Matrix4 ortho(f32 left, f32 right, f32 bottom, f32 top, f32 zNear, f32 zFar);
	
	// angle is in radians
	Matrix4 perspective(const Radian& fovy, f32 aspect, f32 zNear, f32 zFar);
	Matrix4 infinitePerspective(const Radian& fovy, f32 aspect, f32 zNear);

	template <class T>
	T lookAt(const Vector3& eye, const Vector3& center, const Vector3& up);
	template <>
	Matrix4 lookAt<Matrix4>(const Vector3& eye, const Vector3& center, const Vector3& up);
	template <>
	Quaternion lookAt<Quaternion>(const Vector3& eye, const Vector3& center, const Vector3& up);

} // end Math
} // end Dunjun
#endif
