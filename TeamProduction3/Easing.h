#pragma once
#include "Vector2.h"
#include "Vector3.h"
class Easing
{
public:
	static Vector2 EaseIn(const Vector2& s, const Vector2& e, float& t);
	static Vector3 EaseIn(const Vector3& s, const Vector3& e, float& t);
	static Vector2 EaseOut(const Vector2& s, const Vector2& e, float& t);
	static Vector3 EaseOut(const Vector3& s, const Vector3& e, float& t);
	static Vector2 EaseInOut(const Vector2& s, const Vector2& e, float& t);
	static Vector3 EaseInOut(const Vector3& s, const Vector3& e, float& t);
};

