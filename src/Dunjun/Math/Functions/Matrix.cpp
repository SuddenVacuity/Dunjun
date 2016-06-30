#include <Dunjun/Math/Functions/Matrix.hpp>

#include <cassert>

namespace Dunjun
{
namespace Math
{
	// matrix transform handling functions
	Matrix4 translate(const Vector3& v)
	{
		Matrix4 result = Matrix4::Identity;

		result.data[3] = Vector4{v.x, v.y, v.z, 1};


		return result;
	}
	Matrix4 rotate(const Radian& angle, const Vector3& v) // angle is in radians, v is the axis to rotate around
	{
		const f32 c = Math::cos(angle);
		const f32 s = Math::sin(angle);

		const Vector3 axis(normalize(v));
		const Vector3 t = (1.0f - c) * axis;

		Matrix4 rot = 0.0f * Matrix4::Identity;
		rot.data[0].data[0] = c + t.data[0] * axis.data[0]; // x component
		rot.data[0].data[1] = 0 + t.data[0] * axis.data[1] + s * axis.data[2];
		rot.data[0].data[2] = 0 + t.data[0] * axis.data[2] - s * axis.data[1];
		rot.data[0].data[3] = 0;	 							
						 							
		rot.data[1].data[0] = 0 + t.data[1] * axis.data[0] - s * axis.data[2]; // y component
		rot.data[1].data[1] = c + t.data[1] * axis.data[1];		
		rot.data[1].data[2] = 0 + t.data[1] * axis.data[2] + s * axis.data[0];
		rot.data[1].data[3] = 0;	 								
						 								
		rot.data[2].data[0] = 0 + t.data[2] * axis.data[0] - s * axis.data[1]; // z component
		rot.data[2].data[1] = 0 + t.data[2] * axis.data[1] + s * axis.data[0];
		rot.data[2].data[2] = c + t.data[2] * axis.data[2];
		rot.data[2].data[3] = 0;

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
		Matrix4 result = Matrix4::Identity;

		result.data[0] = { v.x, 0, 0, 0 };
		result.data[1] = { 0, v.y, 0, 0 };
		result.data[2] = { 0, 0, v.z, 0 };
		result.data[3] = { 0, 0, 0, 1.0f };

		return result;
	}

	// view handling functions
	Matrix4 ortho(f32 left, f32 right, f32 bottom, f32 top)
	{
		Matrix4 result = Matrix4::Identity;

		result.data[0].data[0] = 2.0f / (right - left);
		result.data[1].data[1] = 2.0f / (top - bottom);
		result.data[2].data[2] = -1.0f;
		result.data[3].data[0] = -((right + left) / (right - left));
		result.data[3].data[1] = -((top + bottom) / (top - bottom));
		result.data[3].data[3] = 1;

		return result;
	}
	Matrix4 ortho(f32 left, f32 right, f32 bottom, f32 top, f32 zNear, f32 zFar)
	{
		Matrix4 result = Matrix4::Identity;

		result.data[0].data[0] = 2.0f / (right - left);
		result.data[1].data[1] = 2.0f / (top - bottom);
		result.data[2].data[2] = -2.0f / (zFar - zNear);
		result.data[3].data[0] = -((right + left) / (right - left));
		result.data[3].data[1] = -((top + bottom) / (top - bottom));
		result.data[3].data[2] = -((zFar + zNear) / (zFar - zNear));
		result.data[3].data[3] = 1;

		return result;
	}

	// fov angle is in radians
	Matrix4 perspective(const Radian& fovy, f32 aspect, f32 zNear, f32 zFar)
	{
		assert(Math::abs(aspect - std::numeric_limits<f32>::epsilon()) > 0.0f
			   && "Math::perspective 'fovy' is 0/inf"); // make sure aspect ratio is greater than 0

		Matrix4 result = Matrix4::Identity0;

		const f32 range = Math::tan(fovy / 2.0f);

		result.data[0].data[0] = 1.0f / (aspect * range);
		result.data[1].data[1] = 1.0f / (range);
		result.data[2].data[2] = -((zFar + zNear) / (zFar - zNear));
		result.data[2].data[3] = -1.0f;
		result.data[3].data[2] = -2.0f * zFar * zNear / (zFar - zNear);

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

		Matrix4 result = Matrix4::Identity0;

		result.data[0].data[0] = (2.0f * zNear) / (right - left);
		result.data[1].data[1] = (2.0f * zNear) / (top - bottom);
		result.data[2].data[2] = -1.0f;
		result.data[2].data[3] = -1.0f;
		result.data[3].data[2] = -2.0f * zNear;

		return result;
	}

	template<>
	Matrix4 Math::lookAt<Matrix4>(const Vector3& eye, const Vector3& center, const Vector3& up)
	{
		const Vector3 f(normalize(center - eye));
		const Vector3 s(normalize(cross(f, up)));
		const Vector3 u(cross(s, f));

		Matrix4 result = Matrix4::Identity;

		result.data[0].data[0] = +s.x;
		result.data[1].data[0] = +s.y;
		result.data[2].data[0] = +s.z;
		
		result.data[0].data[1] = +u.x;
		result.data[1].data[1] = +u.y;
		result.data[2].data[1] = +u.z;
		
		result.data[0].data[2] = -f.x;
		result.data[1].data[2] = -f.y;
		result.data[2].data[2] = -f.z;
			
		result.data[3].data[0] = -dot(s, eye);
		result.data[3].data[1] = -dot(u, eye);
		result.data[3].data[2] = +dot(f, eye);

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
