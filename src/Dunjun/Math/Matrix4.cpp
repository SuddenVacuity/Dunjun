#include <Dunjun/Math/Matrix4.hpp>

namespace Dunjun
{
	//Vector4& Matrix4::operator[](size_t index) { return data[index]; }
	//const Vector4& Matrix4::operator[](size_t index) const { return data[index]; }

	// Matrix4 all values 0's with diagonal 1's
	const Matrix4 Matrix4::Identity = {Vector4{1, 0, 0, 0},
									   Vector4{0, 1, 0, 0},
									   Vector4{0, 0, 1, 0},
									   Vector4{0, 0, 0, 1}};

	bool operator==(const Matrix4& a, const Matrix4& b)
			{
				const Matrix4& m1 = a; // shorthand
				for (size_t i = 0; i < 4; i++)
				{
					if (m1.data[i] != b.data[i])
						return false;
				}
				return true;
			}
	bool operator!=(const Matrix4& a, const Matrix4& b)
			{
				return !operator==(a, b);
			}

	Matrix4 operator+(const Matrix4& a, const Matrix4& b)
			{
				Matrix4 mat = Matrix4::Identity; // temp
				for (size_t i = 0; i < 4; i++)
					mat.data[i] = a.data[i] + b.data[i];
				return mat;
			}
	Matrix4 operator-(const Matrix4& a, const Matrix4& b)
			{
				Matrix4 mat = Matrix4::Identity;
				for (size_t i = 0; i < 4; i++)
					mat.data[i] = a.data[i] - b.data[i];
				return mat;
			}

	Matrix4 operator*(const Matrix4& a, const Matrix4& b)
			{
				const Matrix4& m1 = a; // shorthand

				const Vector4 srcA0 = m1.data[0];
				const Vector4 srcA1 = m1.data[1];
				const Vector4 srcA2 = m1.data[2];
				const Vector4 srcA3 = m1.data[3];

				const Vector4 srcB0 = b.data[0];
				const Vector4 srcB1 = b.data[1];
				const Vector4 srcB2 = b.data[2];
				const Vector4 srcB3 = b.data[3];

				Matrix4 result = Matrix4::Identity; // multiply the matrices
				result.data[0] = srcA0 * srcB0.data[0] + srcA1 * srcB0.data[1] + srcA2 * srcB0.data[2] + srcA3 * srcB0.data[3];
				result.data[1] = srcA0 * srcB1.data[0] + srcA1 * srcB1.data[1] + srcA2 * srcB1.data[2] + srcA3 * srcB1.data[3];
				result.data[2] = srcA0 * srcB2.data[0] + srcA1 * srcB2.data[1] + srcA2 * srcB2.data[2] + srcA3 * srcB2.data[3];
				result.data[3] = srcA0 * srcB3.data[0] + srcA1 * srcB3.data[1] + srcA2 * srcB3.data[2] + srcA3 * srcB3.data[3];

				return result;
			}
	Vector4 operator*(const Matrix4& a, const Vector4& v) // multiply by vector
			{
				const Matrix4& m = a;

				const Vector4 mul0 = m.data[0] * v.data[0];
				const Vector4 mul1 = m.data[1] * v.data[1];
				const Vector4 mul2 = m.data[2] * v.data[2];
				const Vector4 mul3 = m.data[3] * v.data[3];

				const Vector4 add0 = mul0 + mul1;
				const Vector4 add1 = mul2 + mul3;

				return add0 + add1;
			}
	Matrix4 operator*(const Matrix4& a, f32 scaler) // scaler
			{
				Matrix4 mat = Matrix4::Identity;
				for (size_t i = 0; i < 4; i++)
					mat.data[i] = a.data[i] * scaler;
				return mat;
			}
	Matrix4 operator/(const Matrix4& a, f32 scaler) // scaler
			{
				Matrix4 mat = Matrix4::Identity;
				for (size_t i = 0; i < 4; i++)
					mat.data[i] = a.data[i] / scaler;
				return mat;
			}

	Matrix4& operator+=(Matrix4& a, const Matrix4& b)
			{
				a = a + b;
				return a;
			}
	Matrix4& operator-=(Matrix4& a, const Matrix4& b)
			{
				a = a - b;
				return a;
			}
	Matrix4& operator*=(Matrix4& a, const Matrix4& b)
			{
				a = a * b;
				return a;
			}

	// use in transpose matrix
	Matrix4 transpose(const Matrix4& m)
	{
		Matrix4 result = Matrix4::Identity;

		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 4; j++)
				result.data[i].data[j] = m.data[j].data[i];
		}

		return result;
	}

	f32 determinant(const Matrix4& m)// determinant for transpose matrices
	{
		f32 coef00 = m.data[2].data[2] * m.data[3].data[3] - m.data[3].data[2] * m.data[2].data[3];
		f32 coef02 = m.data[1].data[2] * m.data[3].data[3] - m.data[3].data[2] * m.data[1].data[3];
		f32 coef03 = m.data[1].data[2] * m.data[2].data[3] - m.data[2].data[2] * m.data[1].data[3];

		f32 coef04 = m.data[2].data[1] * m.data[3].data[3] - m.data[3].data[1] * m.data[2].data[3];
		f32 coef06 = m.data[1].data[1] * m.data[3].data[3] - m.data[3].data[1] * m.data[1].data[3];
		f32 coef07 = m.data[1].data[1] * m.data[2].data[3] - m.data[2].data[1] * m.data[1].data[3];

		f32 coef08 = m.data[2].data[1] * m.data[3].data[2] - m.data[3].data[1] * m.data[2].data[2];
		f32 coef10 = m.data[1].data[1] * m.data[3].data[2] - m.data[3].data[1] * m.data[1].data[2];
		f32 coef11 = m.data[1].data[1] * m.data[2].data[2] - m.data[2].data[1] * m.data[1].data[2];

		f32 coef12 = m.data[2].data[0] * m.data[3].data[3] - m.data[3].data[0] * m.data[2].data[3];
		f32 coef14 = m.data[1].data[0] * m.data[3].data[3] - m.data[3].data[0] * m.data[1].data[3];
		f32 coef15 = m.data[1].data[0] * m.data[2].data[3] - m.data[2].data[0] * m.data[1].data[3];

		f32 coef16 = m.data[2].data[0] * m.data[3].data[2] - m.data[3].data[0] * m.data[2].data[2];
		f32 coef18 = m.data[1].data[0] * m.data[3].data[2] - m.data[3].data[0] * m.data[1].data[2];
		f32 coef19 = m.data[1].data[0] * m.data[2].data[2] - m.data[2].data[0] * m.data[1].data[2];

		f32 coef20 = m.data[2].data[0] * m.data[3].data[1] - m.data[3].data[0] * m.data[2].data[1];
		f32 coef22 = m.data[1].data[0] * m.data[3].data[1] - m.data[3].data[0] * m.data[1].data[1];
		f32 coef23 = m.data[1].data[0] * m.data[2].data[1] - m.data[2].data[0] * m.data[1].data[1];

		Vector4 fac0{coef00, coef00, coef02, coef03};
		Vector4 fac1{coef04, coef04, coef06, coef07};
		Vector4 fac2{coef08, coef08, coef10, coef11};
		Vector4 fac3{coef12, coef12, coef14, coef15};
		Vector4 fac4{coef16, coef16, coef18, coef19};
		Vector4 fac5{coef20, coef20, coef22, coef23};

		Vector4 vec0{m.data[1].data[0], m.data[0].data[0], m.data[0].data[0], m.data[0].data[0]};
		Vector4 vec1{m.data[1].data[1], m.data[0].data[1], m.data[0].data[1], m.data[0].data[1]};
		Vector4 vec2{m.data[1].data[2], m.data[0].data[2], m.data[0].data[2], m.data[0].data[2]};
		Vector4 vec3{m.data[1].data[3], m.data[0].data[3], m.data[0].data[3], m.data[0].data[3]};

		Vector4 inv0(vec1 * fac0 - vec2 * fac1 + vec3 * fac2);
		Vector4 inv1(vec0 * fac0 - vec2 * fac3 + vec3 * fac4);
		Vector4 inv2(vec0 * fac1 - vec1 * fac3 + vec3 * fac5);
		Vector4 inv3(vec0 * fac2 - vec1 * fac4 + vec2 * fac5);

		Vector4 signA{+1, -1, +1, -1};
		Vector4 signB{-1, +1, -1, +1};
		Matrix4 inverse{inv0 * signA, inv1 * signB, inv2 * signA, inv3 * signB};

		Vector4 row0{inverse.data[0].data[0], 
					 inverse.data[1].data[0], 
					 inverse.data[2].data[0], 
					 inverse.data[3].data[0]};

		Vector4 dot0(m.data[0] * row0);
		f32 dot1 = (dot0.x + dot0.y) + (dot0.z + dot0.w);
		return dot1;
	}

	Matrix4 inverse(const Matrix4& m)
	{
		f32 coef00 = m.data[2].data[2] * m.data[3].data[3] - m.data[3].data[2] * m.data[2].data[3];
		f32 coef02 = m.data[1].data[2] * m.data[3].data[3] - m.data[3].data[2] * m.data[1].data[3];
		f32 coef03 = m.data[1].data[2] * m.data[2].data[3] - m.data[2].data[2] * m.data[1].data[3];

		f32 coef04 = m.data[2].data[1] * m.data[3].data[3] - m.data[3].data[1] * m.data[2].data[3];
		f32 coef06 = m.data[1].data[1] * m.data[3].data[3] - m.data[3].data[1] * m.data[1].data[3];
		f32 coef07 = m.data[1].data[1] * m.data[2].data[3] - m.data[2].data[1] * m.data[1].data[3];

		f32 coef08 = m.data[2].data[1] * m.data[3].data[2] - m.data[3].data[1] * m.data[2].data[2];
		f32 coef10 = m.data[1].data[1] * m.data[3].data[2] - m.data[3].data[1] * m.data[1].data[2];
		f32 coef11 = m.data[1].data[1] * m.data[2].data[2] - m.data[2].data[1] * m.data[1].data[2];

		f32 coef12 = m.data[2].data[0] * m.data[3].data[3] - m.data[3].data[0] * m.data[2].data[3];
		f32 coef14 = m.data[1].data[0] * m.data[3].data[3] - m.data[3].data[0] * m.data[1].data[3];
		f32 coef15 = m.data[1].data[0] * m.data[2].data[3] - m.data[2].data[0] * m.data[1].data[3];

		f32 coef16 = m.data[2].data[0] * m.data[3].data[2] - m.data[3].data[0] * m.data[2].data[2];
		f32 coef18 = m.data[1].data[0] * m.data[3].data[2] - m.data[3].data[0] * m.data[1].data[2];
		f32 coef19 = m.data[1].data[0] * m.data[2].data[2] - m.data[2].data[0] * m.data[1].data[2];

		f32 coef20 = m.data[2].data[0] * m.data[3].data[1] - m.data[3].data[0] * m.data[2].data[1];
		f32 coef22 = m.data[1].data[0] * m.data[3].data[1] - m.data[3].data[0] * m.data[1].data[1];
		f32 coef23 = m.data[1].data[0] * m.data[2].data[1] - m.data[2].data[0] * m.data[1].data[1];

		Vector4 fac0{coef00, coef00, coef02, coef03};
		Vector4 fac1{coef04, coef04, coef06, coef07};
		Vector4 fac2{coef08, coef08, coef10, coef11};
		Vector4 fac3{coef12, coef12, coef14, coef15};
		Vector4 fac4{coef16, coef16, coef18, coef19};
		Vector4 fac5{coef20, coef20, coef22, coef23};

		Vector4 vec0{m.data[1].data[0], m.data[0].data[0], m.data[0].data[0], m.data[0].data[0]};
		Vector4 vec1{m.data[1].data[1], m.data[0].data[1], m.data[0].data[1], m.data[0].data[1]};
		Vector4 vec2{m.data[1].data[2], m.data[0].data[2], m.data[0].data[2], m.data[0].data[2]};
		Vector4 vec3{m.data[1].data[3], m.data[0].data[3], m.data[0].data[3], m.data[0].data[3]};

		Vector4 inv0(vec1 * fac0 - vec2 * fac1 + vec3 * fac2);
		Vector4 inv1(vec0 * fac0 - vec2 * fac3 + vec3 * fac4);
		Vector4 inv2(vec0 * fac1 - vec1 * fac3 + vec3 * fac5);
		Vector4 inv3(vec0 * fac2 - vec1 * fac4 + vec2 * fac5);

		Vector4 signA{+1, -1, +1, -1};
		Vector4 signB{-1, +1, -1, +1};
		Matrix4 inverse{inv0 * signA, inv1 * signB, inv2 * signA, inv3 * signB};

		Vector4 row0{inverse.data[0].data[0], 
					 inverse.data[1].data[0], 
					 inverse.data[2].data[0],
					 inverse.data[3].data[0]};

		Vector4 dot0(m.data[0] * row0);
		f32 dot1 = (dot0.x + dot0.y) + (dot0.z + dot0.w);

		f32 oneOverDeterminant = 1.0f / dot1;

		return inverse * oneOverDeterminant;
	}

	Matrix4 hadamardProduct(const Matrix4& a, const Matrix4& b)
	{
		Matrix4 result = Matrix4::Identity;
		for (size_t i = 0; i < 4; i++)
			result.data[i] = a.data[i] * b.data[i];
		return result;
	}


}
