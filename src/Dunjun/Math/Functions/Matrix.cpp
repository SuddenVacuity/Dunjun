#include <Dunjun/Math/Functions/Matrix.hpp>

#include <cassert>

namespace Dunjun
{
namespace Math
{

	// matrix transform handling functions
	Matrix4 translate(const Vector3& v)
	{
		Matrix4 result;
		result[3] = Vector4(v, 1);

		return result;
	}
	Matrix4 rotate(const Radian& angle, const Vector3& v) // angle is in radians, v is the axis to rotate around
	{
		const f32 c = Math::cos(angle);
		const f32 s = Math::sin(angle);

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
	Matrix4 scale(const Vector3& v)
	{
		Matrix4 result( { v.x, 0, 0, 0 }, 
						{ 0, v.y, 0, 0 }, 
						{ 0, 0, v.z, 0 }, 
						{ 0, 0, 0, 1.0f });
		return result;
	}

	// view handling functions
	Matrix4 ortho(f32 left, f32 right, f32 bottom, f32 top)
	{
		Matrix4 result;

		result[0][0] = 2.0f / (right - left);
		result[1][1] = 2.0f / (top - bottom);
		result[2][2] = -1.0f;
		result[3][0] = -((right + left) / (right - left));
		result[3][1] = -((top + bottom) / (top - bottom));
		result[3][3] = 1;

		return result;
	}
	Matrix4 ortho(f32 left, f32 right, f32 bottom, f32 top, f32 zNear, f32 zFar)
	{
		Matrix4 result;

		result[0][0] = 2.0f / (right - left);
		result[1][1] = 2.0f / (top - bottom);
		result[2][2] = -2.0f / (zFar - zNear);
		result[3][0] = -((right + left) / (right - left));
		result[3][1] = -((top + bottom) / (top - bottom));
		result[3][2] = -((zFar + zNear) / (zFar - zNear));
		result[3][3] = 1;

		return result;
	}

	// fov angle is in radians
	Matrix4 perspective(const Radian& fovy, f32 aspect, f32 zNear, f32 zFar)
	{
		assert(Math::abs(aspect - std::numeric_limits<f32>::epsilon()) > 0.0f
			   && "Math::perspective 'fovy' is 0/inf"); // make sure aspect ratio is greater than 0

		Matrix4 result(0.0f);

		const f32 range = Math::tan(fovy / 2.0f);

		result[0][0] = 1.0f / (aspect * range);
		result[1][1] = 1.0f / (range);
		result[2][2] = -((zFar + zNear) / (zFar - zNear));
		result[2][3] = -1.0f;
		result[3][2] = -2.0f * zFar * zNear / (zFar - zNear);

		//const f32 left = -(f32)fovy;
		//const f32 right = (f32)fovy;
		//const f32 bottom = -1.0f;
		//const f32 top = 1.0f;
		//
		//// from stack NOT WORKING (DEPTH DRAW ORDER IS BROKEN)
		//result[0][0] = 2.0f * (1.0f / (right - left));
		//result[1][1] = 2.0f * (1.0f / (top - bottom));
		//result[2][2] = 2.0f * (1.0f / (zFar - zNear));
		//result[3][3] = 1.0f;
		//
		//result[0][3] = -((right + left) * (1.0f / (right - left)));
		//result[1][3] = -((top + bottom) * (1.0f / (top - bottom)));
		//result[2][3] = -((zFar + zNear) * (1.0f / (zFar - zNear)));

		return result;
	}
	Matrix4 infinitePerspective(const Radian& fovy, f32 aspect, f32 zNear)
	{
		const f32 range = Math::tan(fovy / 2.0f) * zNear;
		const f32 left = -range * aspect;
		const f32 right  = range * aspect;
		const f32 bottom = -range;
		const f32 top = range;

		Matrix4 result(0.0f);
		result[0][0] = (2.0f * zNear) / (right - left);
		result[1][1] = (2.0f * zNear) / (top - bottom);
		result[2][2] = -1.0f;
		result[2][3] = -1.0f;
		result[3][2] = -2.0f * zNear;

		return result;
	}

	template<>
	Matrix4 Math::lookAt<Matrix4>(const Vector3& eye, const Vector3& center, const Vector3& up)
	{
		const Vector3 f(normalize(center - eye));
		const Vector3 s(normalize(cross(f, up)));
		const Vector3 u(cross(s, f));

		Matrix4 result;
		result[0][0] = +s.x;
		result[1][0] = +s.y;
		result[2][0] = +s.z;

		result[0][1] = +u.x;
		result[1][1] = +u.y;
		result[2][1] = +u.z;

		result[0][2] = -f.x;
		result[1][2] = -f.y;
		result[2][2] = -f.z;

		result[3][0] = -dot(s, eye);
		result[3][1] = -dot(u, eye);
		result[3][2] = +dot(f, eye);

		return result;
	}

	template <>
	Quaternion Math::lookAt<Quaternion>(const Vector3& eye, const Vector3& center, const Vector3& up)
	{
		//const Vector3& pos = transform.position;

		if (length(center - eye) < 0.001f)
			return Quaternion(); // ignore as you can't look at where you are

		// TODO: fix quaternion lookat so that it doesn't use matrix4
		return matrix4ToQuaternion(lookAt<Matrix4>(eye, center, up));

		//const Vector3 f(normalize(center - eye));
		//const Vector3 s(normalize(cross(f, up)));
		//const Vector3 u(cross(s, f));
		//const Vector3 refUp(normalize(up));
		//
		//const f32 m = Math::sqrt(2.0f + 2.0f * dot(u, refUp));
		//
		//Vector3 v = (1.0f / m) * cross(u, refUp);
		//
		//return Quaternion(v, 0.5f * m);
	}
} // end Math
} // end Dunjun
