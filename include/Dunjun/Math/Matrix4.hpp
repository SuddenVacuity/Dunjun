#ifndef DUNJUN_MATH_MATRIX4_HPP
#define DUNJUN_MATH_MATRIX4_HPP

#include <Dunjun/Math/Vector4.hpp>

#include <array>

namespace Dunjun
{
	class Matrix4
	{
	public:
		Matrix4()
			: data	{ Vector4(1, 0, 0, 0)
					, Vector4(0, 1, 0, 0)
					, Vector4(0, 0, 1, 0)
					, Vector4(0, 0, 0, 1)}
		{
		}

		Matrix4(f32 x)
			: data	{ Vector4(x, 0, 0, 0)
					, Vector4(0, x, 0, 0)
					, Vector4(0, 0, x, 0)
					, Vector4(0, 0, 0, x) }
		{
		}

		Matrix4(const Vector4& v0, const Vector4& v1, const Vector4& v2, const Vector4& v3)
			: data {v0, v1, v2, v3}
		{
		}
		
		Matrix4(const Matrix4& other) = default;

		Vector4& operator[](size_t index)
		{
			return data[index];
		}
		const Vector4& operator[](size_t index) const
		{
			return data[index];
		}

		bool operator==(const Matrix4& m2) const
		{
			const Matrix4& m1 = *this; // shorthand
			for (size_t i = 0; i < 4; i++)
			{
				if (m1[i] != m2[i])
					return false;
			}
			return true;
		}

		bool operator!=(const Matrix4& m2) const
		{
			return !operator==(m2);
		}

		Matrix4 operator+(const Matrix4& other) const
		{
			Matrix4 mat;
			for (size_t i = 0; i < 4; i++)
				mat[i] = data[i] + other.data[i];
			return mat;
		}

		Matrix4 operator-(const Matrix4& other) const
		{
			Matrix4 mat;
			for (size_t i = 0; i < 4; i++)
				mat[i] = data[i] - other.data[i];
			return mat;
		}

		Matrix4 operator*(const Matrix4& m2) const
		{
			const Matrix4& m1 = *this; // shorthand

			const Vector4 srcA0 = m1[0];
			const Vector4 srcA1 = m1[1];
			const Vector4 srcA2 = m1[2];
			const Vector4 srcA3 = m1[3];

			const Vector4 srcB0 = m2[0];
			const Vector4 srcB1 = m2[1];
			const Vector4 srcB2 = m2[2];
			const Vector4 srcB3 = m2[3];

			Matrix4 result; // multiply the matrices
			result[0] = srcA0 * srcB0[0] + srcA1 * srcB0[1] + srcA2 * srcB0[2] + srcA3 * srcB0[3];
			result[1] = srcA0 * srcB1[0] + srcA1 * srcB1[1] + srcA2 * srcB1[2] + srcA3 * srcB1[3];
			result[2] = srcA0 * srcB2[0] + srcA1 * srcB2[1] + srcA2 * srcB2[2] + srcA3 * srcB2[3];
			result[3] = srcA0 * srcB3[0] + srcA1 * srcB3[1] + srcA2 * srcB3[2] + srcA3 * srcB3[3];

			return result;
		}

		Vector4 operator*(const Vector4& v) const // multiply by vector
		{
			const Matrix4& m = *this;

			const Vector4 mul0 = m[0] * v[0];
			const Vector4 mul1 = m[1] * v[1];
			const Vector4 mul2 = m[2] * v[2];
			const Vector4 mul3 = m[3] * v[3];

			const Vector4 add0 = mul0 + mul1;
			const Vector4 add1 = mul2 + mul3;

			return add0 + add1;
		}

		Matrix4 operator*(f32 scaler) const // scaler
		{
			Matrix4 mat;
			for (size_t i = 0; i < 4; i++)
				mat[i] = data[i] * scaler;
			return mat;
		}

		Matrix4 operator/(f32 scaler) const // scaler
		{
			Matrix4 mat;
			for (size_t i = 0; i < 4; i++)
				mat[i] = data[i] / scaler;
			return mat;
		}

		Matrix4 operator+=(const Matrix4& other)
		{
			return (*this = (*this) + other);
		}

		Matrix4 operator-=(const Matrix4& other)
		{
			return (*this = (*this) - other);
		}

		Matrix4 operator*=(const Matrix4& other)
		{
			return (*this = (*this) * other);
		}

		

		Vector4 data[4];
	};

	// use in transpose matrix
	Matrix4 transpose(const Matrix4& m);
	f32 determinant(const Matrix4& m); // determinant for transpose matrices
	Matrix4 inverse(const Matrix4& m); // inverse for transpose

	inline Matrix4 operator*(f32 scaler, const Matrix4& m) // scaler for the other side
	{
		Matrix4 mat;
		for (size_t i = 0; i < 4; i++)
			mat[i] = m[i] * scaler;
		return mat;
		// return m * scaler;
	}

	inline Matrix4 hadamardProduct(const Matrix4& a, const Matrix4& b)
	{
		Matrix4 result;
		for (size_t i = 0; i < 4; i++)
			result[i] = a[i] * b[i];
		return result;
	}

	inline std::ostream& operator<<(std::ostream& os, const Matrix4& m)
	{
		os << "Matrix4(";
		for(size_t i = 0; i < 4; i++)
			os << "\n\t" << m[i];
		os << "\n)";
		
		return os;
	}

}

#endif