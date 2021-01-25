#pragma once
#include "Vector3.h"
struct Matrix4
{
	float	m[4][4];

	static Matrix4 Identity();
	static Matrix4 Scale(const Vector3& v);
	static Matrix4 RotationX(float angle);
	static Matrix4 RotationY(float angle);
	static Matrix4 RotationZ(float angle);
	static Matrix4 Translate(const Vector3& v);
	static Vector3 Transform(const Vector3& v, const Matrix4& m);
	static Matrix4 Transpose(const Matrix4& m);
	static Matrix4 Inverse(const Matrix4& m);
	static float Determinant(const Matrix4& m);
};
Matrix4& operator*=(Matrix4& m1, const Matrix4& m2);
Matrix4 operator*(const Matrix4& m1, const Matrix4& m2);
Vector3 operator*(const Vector3& v, const Matrix4& m);