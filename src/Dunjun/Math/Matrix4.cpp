#include <Dunjun/Math/Functions/Matrix.hpp>

#include <cassert>

namespace Dunjun
{
	// use in transpose matrix
	Matrix4 transpose(const Matrix4& m)
	{
		Matrix4 result;

		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 4; j++)
				result[i][j] = m[j][i];
		}

		return result;
	}

	f32 determinant(const Matrix4& m) // determinant for transpose matrices
	{
		f32 coef00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
		f32 coef02 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
		f32 coef03 = m[1][2] * m[2][3] - m[3][2] * m[1][3];

		f32 coef04 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
		f32 coef06 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
		f32 coef07 = m[1][1] * m[2][3] - m[2][1] * m[1][3];

		f32 coef08 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
		f32 coef10 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
		f32 coef11 = m[1][1] * m[2][2] - m[2][1] * m[1][2];

		f32 coef12 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
		f32 coef14 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
		f32 coef15 = m[1][0] * m[2][3] - m[2][0] * m[1][3];

		f32 coef16 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
		f32 coef18 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
		f32 coef19 = m[1][0] * m[2][2] - m[2][0] * m[1][2];

		f32 coef20 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
		f32 coef22 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
		f32 coef23 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

		Vector4 fac0(coef00, coef00, coef02, coef03);
		Vector4 fac1(coef04, coef04, coef06, coef07);
		Vector4 fac2(coef08, coef08, coef10, coef11);
		Vector4 fac3(coef12, coef12, coef14, coef15);
		Vector4 fac4(coef16, coef16, coef18, coef19);
		Vector4 fac5(coef20, coef20, coef22, coef23);

		Vector4 vec0(m[1][0], m[0][0], m[0][0], m[0][0]);
		Vector4 vec1(m[1][1], m[0][1], m[0][1], m[0][1]);
		Vector4 vec2(m[1][2], m[0][2], m[0][2], m[0][2]);
		Vector4 vec3(m[1][3], m[0][3], m[0][3], m[0][3]);

		Vector4 inv0(vec1 * fac0 - vec2 * fac1 + vec3 * fac2);
		Vector4 inv1(vec0 * fac0 - vec2 * fac3 + vec3 * fac4);
		Vector4 inv2(vec0 * fac1 - vec1 * fac3 + vec3 * fac5);
		Vector4 inv3(vec0 * fac2 - vec1 * fac4 + vec2 * fac5);

		Vector4 signA(+1, -1, +1, -1);
		Vector4 signB(-1, +1, -1, +1);
		Matrix4 inverse(inv0 * signA, inv1 * signB, inv2 * signA, inv3 * signB);

		Vector4 row0(inverse[0][0], inverse[1][0], inverse[2][0], inverse[3][0]);

		Vector4 dot0(m[0] * row0);
		f32 dot1 = (dot0.x + dot0.y) + (dot0.z + dot0.w);
		return dot1;
	}

	Matrix4 inverse(const Matrix4& m)
	{
		f32 coef00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
		f32 coef02 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
		f32 coef03 = m[1][2] * m[2][3] - m[3][2] * m[1][3];

		f32 coef04 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
		f32 coef06 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
		f32 coef07 = m[1][1] * m[2][3] - m[2][1] * m[1][3];

		f32 coef08 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
		f32 coef10 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
		f32 coef11 = m[1][1] * m[2][2] - m[2][1] * m[1][2];

		f32 coef12 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
		f32 coef14 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
		f32 coef15 = m[1][0] * m[2][3] - m[2][0] * m[1][3];

		f32 coef16 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
		f32 coef18 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
		f32 coef19 = m[1][0] * m[2][2] - m[2][0] * m[1][2];

		f32 coef20 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
		f32 coef22 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
		f32 coef23 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

		Vector4 fac0(coef00, coef00, coef02, coef03);
		Vector4 fac1(coef04, coef04, coef06, coef07);
		Vector4 fac2(coef08, coef08, coef10, coef11);
		Vector4 fac3(coef12, coef12, coef14, coef15);
		Vector4 fac4(coef16, coef16, coef18, coef19);
		Vector4 fac5(coef20, coef20, coef22, coef23);

		Vector4 vec0(m[1][0], m[0][0], m[0][0], m[0][0]);
		Vector4 vec1(m[1][1], m[0][1], m[0][1], m[0][1]);
		Vector4 vec2(m[1][2], m[0][2], m[0][2], m[0][2]);
		Vector4 vec3(m[1][3], m[0][3], m[0][3], m[0][3]);

		Vector4 inv0(vec1 * fac0 - vec2 * fac1 + vec3 * fac2);
		Vector4 inv1(vec0 * fac0 - vec2 * fac3 + vec3 * fac4);
		Vector4 inv2(vec0 * fac1 - vec1 * fac3 + vec3 * fac5);
		Vector4 inv3(vec0 * fac2 - vec1 * fac4 + vec2 * fac5);

		Vector4 signA(+1, -1, +1, -1);
		Vector4 signB(-1, +1, -1, +1);
		Matrix4 inverse(inv0 * signA, inv1 * signB, inv2 * signA, inv3 * signB);

		Vector4 row0(inverse[0][0], inverse[1][0], inverse[2][0], inverse[3][0]);

		Vector4 dot0(m[0] * row0);
		f32 dot1 = (dot0.x + dot0.y) + (dot0.z + dot0.w);

		f32 oneOverDeterminant = 1.0f / dot1;

		return inverse * oneOverDeterminant;
	}


	// view handling functions

	Matrix4 ortho(f32 left, f32 right, f32 bottom, f32 top)
	{
		Matrix4 result;

		result[0][0] = 2.0f / (right - left);
		result[1][1] = 2.0f / (top - bottom);
		result[2][2] = -1.0f;
		result[3][0] = -(right + left) / (right - left);
		result[3][1] = -(top + bottom) / (top - bottom);

		return result;
	}
	Matrix4 ortho(f32 left, f32 right, f32 bottom, f32 top, f32 zNear, f32 zFar)
	{
		Matrix4 result;

		result[0][0] = 2.0f * zNear / (right - left);
		result[1][1] = 2.0f * zNear / (top - bottom);
		result[2][2] = -2.0f / (zFar - zNear);
		result[2][0] = -(right + left) / (right - left);
		result[3][0] = -(top + bottom) / (top + bottom);
		result[3][2] = -(zFar + zNear) / (zFar - zNear);

		return result;
	}

	// fov angle is in radians
	Matrix4 perspective(const Radian& fovy, f32 aspect, f32 zNear, f32 zFar)
	{
		assert(std::fabs(aspect - std::numeric_limits<f32>::epsilon()) > 0.0f); // make sure aspect ratio is greater than 0
		
		const f32 tanHalfFovy = std::tan(static_cast<f32>(fovy) / 2.0f);

		Matrix4 result(0.0f);
		result[0][0] = 1.0f / (aspect * tanHalfFovy);
		result[1][1] = 1.0f / (tanHalfFovy);
		result[2][2] = -(zFar + zNear) / (zFar - zNear);
		result[2][3] = -1.0f;
		result[3][2] = -2.0f * zFar * zNear / (zFar - zNear);

		return result;
	}
	Matrix4 infinitePerspective(const Radian& fovy, f32 aspect, f32 zNear)
	{
		const f32 range = std::tan(static_cast<f32>(fovy) / 2.0f) * zNear;
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

	Matrix4 lookAt(const Vector3& eye, const Vector3& center, const Vector3& up)
	{
		const Vector3 f(normalize(center- eye));
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
}
