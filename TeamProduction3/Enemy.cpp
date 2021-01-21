#include "Enemy.h"

Enemy::Enemy()
	:cb(nullptr), pos(Vector3()), targetPos(Vector3()), liveFlag(false), reviveFlag(false), reviveT(0)
{
}

Enemy::~Enemy()
{
}

void Enemy::Revive(const Vector3& p)
{
	if (!liveFlag && !reviveFlag)
		reviveFlag = true; liveFlag = true; pos = p;
}

Vector3 Enemy::GetPos()
{
	return pos;
}

void Enemy::SetPos(const Vector3 & p)
{
	pos = p;
}

bool Enemy::GetLiveFlag()
{
	return liveFlag;
}

void Enemy::SetLiveFlag(bool f)
{
	liveFlag = f;
}

bool Enemy::GetReviveFlag()
{
	return reviveFlag;
}

void Enemy::SetTargetPos(const Vector3 & tp)
{
	targetPos = tp;
}

Vector3 Enemy::GetVel()
{
	return vel;
}

void Enemy::SetVel(const Vector3 & v)
{
	vel = v;
}
