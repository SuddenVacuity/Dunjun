#ifndef DUNJUN_MATH_MATRIX4_HPP
#define DUNJUN_MATH_MATRIX4_HPP

#include <Dunjun/Math/Vector4.hpp>

namespace Dunjun
{
	class Matrix4
	{
	public:
		Matrix4(); // Identity

		explicit Matrix4(f32 x); // x * Identity

		explicit Matrix4(const Vector4& v0, 
						 const Vector4& v1, 
						 const Vector4& v2, 
						 const Vector4& v3);
		
		Matrix4(const Matrix4& other) = default;

		Vector4& operator[](size_t index);
		const Vector4& operator[](size_t index) const;

		bool operator==(const Matrix4& m2) const;
		bool operator!=(const Matrix4& m2) const;

		// Addition
		Matrix4 operator+(const Matrix4& other) const;
		Matrix4 operator-(const Matrix4& other) const;
		Matrix4 operator*(const Matrix4& m2) const;
		Vector4 operator*(const Vector4& v) const;
		Matrix4 operator*(f32 scaler) const;
		Matrix4 operator/(f32 scaler) const;

		Matrix4& operator+=(const Matrix4& other);
		Matrix4& operator-=(const Matrix4& other);
		Matrix4& operator*=(const Matrix4& other);

		Vector4 data[4];
	};

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
			os << "\n\t" << m[i];
		os << "\n)";
		
		return os;
	}

}

#endif
