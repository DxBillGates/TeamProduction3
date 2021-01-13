#include "Easing.h"

Vector2 Easing::EaseIn(const Vector2 & s, const Vector2 & e, float & t)
{
	if (t < 0)t = 0;
	else if (t > 1)t = 1;
	return s * (1.0f - t * t) + e * t * t;
}

Vector3 Easing::EaseIn(const Vector3 & s, const Vector3 & e, float & t)
{
	if (t < 0)t = 0;
	else if (t > 1)t = 1;
	return s * (1.0f - t * t) + e * t * t;
}

Vector2 Easing::EaseOut(const Vector2 & s, const Vector2 & e, float & t)
{
	if (t < 0)t = 0;
	else if (t > 1)t = 1;
	return s * (1.0f - t * (2.0f - t)) + e * t * (2.0f - t);
}

Vector3 Easing::EaseOut(const Vector3 & s, const Vector3 & e, float & t)
{
	if (t < 0)t = 0;
	else if (t > 1)t = 1;
	return s * (1.0f - t * (2.0f - t)) + e * t * (2.0f - t);
}

Vector2 Easing::EaseInOut(const Vector2 & s, const Vector2 & e, float & t)
{
	if (t < 0)t = 0;
	else if (t > 1)t = 1;
	return s * (1.0f - (t * t) * (3.0f - 2.0f * t)) + e * (t * t) * (3.0f - 2.0f * t);
}

Vector3 Easing::EaseInOut(const Vector3 & s, const Vector3 & e, float & t)
{
	if (t < 0)t = 0;
	else if (t > 1)t = 1;
	return s * (1.0f - (t * t) * (3.0f - 2.0f * t)) + e * (t * t) * (3.0f - 2.0f * t);
}
