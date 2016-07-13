#ifndef DUNJUN_MATH_MATRIX4_HPP
#define DUNJUN_MATH_MATRIX4_HPP

#include <Dunjun/Math/Vector4.hpp>

namespace Dunjun
{
	struct Matrix4
	{
		union
		{
			struct
			{
				Vector4 x, y, z, w;
			};
			Vector4 data[4];
		};

		// Matrix4 all values 0's with diagonal 1's
		GLOBAL const Matrix4 Identity;

		// Matrix4 all values 0
		GLOBAL const Matrix4 Identity0;

		inline Vector4& operator[](size_t index) { return data[index]; }
		inline const Vector4& operator[](size_t index) const { return data[index]; }

	}; // end Matrix4

	//Vector4& operator[](size_t index);
	//const Vector4& operator[](size_t index);

	bool operator==(const Matrix4& a, const Matrix4& b);
	bool operator!=(const Matrix4& a, const Matrix4& b);

	// Addition
	Matrix4 operator+(const Matrix4& a, const Matrix4& b);
	Matrix4 operator-(const Matrix4& a, const Matrix4& b);
	Matrix4 operator*(const Matrix4& a, const Matrix4& b);
	Vector4 operator*(const Matrix4& a, const Vector4& v);
	Matrix4 operator*(const Matrix4& a, f32 scaler);
	Matrix4 operator/(const Matrix4& a, f32 scaler);

	Matrix4& operator+=(Matrix4& a, const Matrix4& b);
	Matrix4& operator-=(Matrix4& a, const Matrix4& b);
	Matrix4& operator*=(Matrix4& a, const Matrix4& b);

	inline Matrix4 operator*(f32 scaler, const Matrix4& m) // scaler for the other side
	{
		//Matrix4 mat;
		//for (size_t i = 0; i < 4; i++)
		//	mat[i] = m[i] * scaler;
		//return mat;
		return m * scaler;
	}

	// use in transpose matrix
	Matrix4 transpose(const Matrix4& m);
	f32 determinant(const Matrix4& m);
	Matrix4 inverse(const Matrix4& m);
	Matrix4 hadamardProduct(const Matrix4& a, const Matrix4& b);

	inline std::ostream& operator<<(std::ostream& os, const Matrix4& m)
	{
		os << "Matrix4(";
		for(size_t i = 0; i < 4; i++)
			os << "\n\t" << m.data[i];
		os << "\n)";
		
		return os;
	}

}

#endif
