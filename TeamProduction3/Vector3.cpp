#include "Vector3.h"
#include <math.h>
Vector3::Vector3() :x(0), y(0), z(0)
{
}

Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z)
{
}

float Vector3::Length()
{
	return sqrtf(x * x + y * y + z * z);
}

Vector3 Vector3::Normalize()
{
	float length = Length();
	if (length == 0)return Vector3();
	return *this / Length();
}

float Vector3::Distance(const Vector3 & v1, const Vector3 & v2)
{
	Vector3 v = v1 - v2;
	return v.Length();;
}

Vector3 Vector3::Normalize(const Vector3 & v)
{
	Vector3 v1 = v;
	if (v1.Length() == 0)return Vector3();
	return v1 / v1.Length();
}

Vector3 Vector3::Cross(const Vector3 & v1, const Vector3 & v2)
{
	Vector3 v = Vector3(v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x);
	return v;
}

float Vector3::Dot(const Vector3 & v1, const Vector3 & v2)
{
	float value = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	return value;
}

Vector3 Vector3::Lerp(const Vector3 & s, const Vector3 & e, float t)
{
	return s * (1.0f - t) + e * t;
}

Vector3 Vector3::operator+()
{
	return *this;
}

Vector3 Vector3::operator-()
{
	return Vector3(-x, -y, -z);
}

Vector3 & Vector3::operator+=(const Vector3 & v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vector3 & Vector3::operator-=(const Vector3 & v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Vector3 & Vector3::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

Vector3 & Vector3::operator/=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

bool Vector3::operator==(const Vector3 & v)
{
	if (x == v.x&&y == v.y && z == v.z)return true;
	return false;
}

const Vector3 operator+(const Vector3 & v1, const Vector3 & v2)
{
	Vector3 a(v1);
	return a += v2;
}

const Vector3 operator-(const Vector3 & v1, const Vector3 & v2)
{
	Vector3 a(v1);
	return a -= v2;
}

const Vector3 operator*(const Vector3& v, float s)
{
	Vector3 a(v);
	return a *= s;
}

const Vector3 operator*(float s, const Vector3& v)
{
	return v * s;
}

const Vector3 operator/(const Vector3& v, float s)
{
	Vector3 a(v);
	return a /= s;
}

const Vector3 operator/(float s, const Vector3& v)
{

	return v / s;
}