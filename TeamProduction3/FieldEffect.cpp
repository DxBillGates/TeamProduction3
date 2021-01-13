#include "FieldEffect.h"

void FieldEffect::Create(const Vector3& p)
{
	if (!liveFlag && !reviveFlag)
	{
		reviveFlag = true;
		liveFlag = true;
		pos = p;
	}
}

Vector3 FieldEffect::GetPosition()
{
	return pos;
}

Vector3 FieldEffect::GetSize()
{
	return size;
}

float FieldEffect::GetAlphaValue()
{
	return alpha;
}

void FieldEffect::SetAlphaValue(float v)
{
	alpha = v;
}

bool FieldEffect::GetLiveFlag()
{
	return liveFlag;
}

bool FieldEffect::GetReviveFlag()
{
	return reviveFlag;
}

void FieldEffect::SetLiveFlag(bool f)
{
	liveFlag = f;
}
