#include "Vector2.h"
#include <math.h>
Vector2::Vector2() :x(0), y(0)
{
}

Vector2::Vector2(float x, float y) : x(x), y(y)
{
}

float Vector2::Length()
{
	return sqrtf(x * x + y * y);
}

Vector2 Vector2::Normalize()
{
	return *this / Length();
}

float Vector2::Distance(const Vector2 & v1, const Vector2 & v2)
{
	Vector2 v = v1 - v2;
	return v.Length();
}

Vector2 Vector2::Normalize(const Vector2 & v)
{
	Vector2 v1 = v;
	return v1 / v1.Length();
}

float Vector2::Cross(const Vector2 & v1, const Vector2 & v2)
{
	return v1.x * v2.y - v1.y * v2.x;
}

float Vector2::Dot(const Vector2 & v1, const Vector2 & v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}

Vector2 Vector2::Lerp(const Vector2 & s, const Vector2 & e, float t)
{
	return s * (1.0f - t) + e * t;
}

Vector2 Vector2::operator+()
{
	return *this;
}

Vector2 Vector2::operator-()
{
	return Vector2(-x, -y);
}

Vector2 & Vector2::operator+=(const Vector2 & v)
{
	x += v.x;
	y += v.y;
	return *this;
}

Vector2 & Vector2::operator-=(const Vector2 & v)
{
	x -= v.x;
	y -= v.y;
	return *this;
}

Vector2 & Vector2::operator*=(float s)
{
	x *= s;
	y *= s;
	return *this;
}

Vector2 & Vector2::operator/=(float s)
{
	x /= s;
	y /= s;
	return *this;
}

const Vector2 operator+(const Vector2 & v1, const Vector2 & v2)
{
	Vector2 a(v1);
	return a += v2;
}

const Vector2 operator-(const Vector2 & v1, const Vector2 & v2)
{
	Vector2 a(v1);
	return a -= v2;
}

const Vector2 operator*(const Vector2 & v, float s)
{
	Vector2 a(v);
	return a *= s;
}

const Vector2 operator*(float s, const Vector2 & v)
{
	return v * s;
}

const Vector2 operator/(const Vector2 & v, float s)
{
	Vector2 a(v);
	return a /= s;
}

const Vector2 operator/(float s, const Vector2 & v)
{
	return v / s;
}