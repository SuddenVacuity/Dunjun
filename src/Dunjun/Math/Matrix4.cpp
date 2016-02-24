#include <Dunjun/Math/Matrix4.hpp>

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
}