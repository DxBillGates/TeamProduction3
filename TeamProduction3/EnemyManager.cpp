#include "EnemyManager.h"
#include "NormalEnemy.h"
#include "Camera.h"
#include "ScoreManager.h"

EnemyManager::EnemyManager()
{
	const int MAX_ENEMY = 10;
	for (int i = 0; i < MAX_ENEMY; ++i)
	{
		enemyList.push_back(new NormalEnemy());
	}
}

EnemyManager::~EnemyManager()
{
	for (auto& e : enemyList)
	{
		delete e;
	}
}

void EnemyManager::LoadAsset(ID3D12Device * device, Dx12_CBVSRVUAVHeap * heap, LoadContents * loader)
{
	NormalEnemy::StaticLoadAsset(device, loader);
	EnemyBullet::StaticLoadAsset(device, loader);
	for (auto& e : enemyList)
	{
		e->LoadAsset(device, heap);
	}
}

void EnemyManager::Initialize()
{
	count = 0;
	for (auto& e : enemyList)
	{
		e->Initialize();
		e->GetEnemyBulletPointer()->Initialize();
	}
}

void EnemyManager::Update()
{
	const float REVIVE_RANGE = 1280;
	const int REVIVE_COUNT = 60;
	bool revive = false;
	if (count % REVIVE_COUNT == 0)
	{
		revive = true;
	}
	for (auto& e : enemyList)
	{
		if (revive)
		{
			if (!e->GetLiveFlag())
			{
				Vector3 randomPos;
				randomPos.x = (float)std::rand() / RAND_MAX * REVIVE_RANGE - REVIVE_RANGE / 2.0f + pPlayer->GetPosition().x;
				randomPos.z = (float)std::rand() / RAND_MAX * REVIVE_RANGE - REVIVE_RANGE / 2.0f + pPlayer->GetPosition().z;
				e->Revive(randomPos);
				revive = false;
			}
		}
		e->SetTargetPos(pPlayer->GetPosition());
		e->Update();

		if (e->GetLiveFlag())
		{
			if (Vector3::Distance(pPlayer->GetPosition(), e->GetPos()) <= 32 && pPlayer->GetFireValue() >= 1)
			{
				Camera::ScreenShake();
				Score* s = ScoreManager::GetInstance()->GetCurrentScore();
				s->SetScore(s->GetScore() + 100);
				e->Initialize();
			}
		}
	}

	const float DISTANCE = 16;
	for (auto& e : enemyList)
	{
		//敵同士の当たり判定
		for (auto& e2 : enemyList)
		{
			//インスタンスが同じ、判定相手が生きていないなら当たり判定を実行しない
			if (e == e2)continue;
			if (!e->GetLiveFlag())continue;
			if (!e2->GetLiveFlag())continue;
			if (Vector3::Distance(e->GetPos(), e2->GetPos()) <= DISTANCE)
			{
				//当たっているなら自身と相手間のベクトルを求め、ポジションを補正する
				Vector3 vec = e->GetPos() - e2->GetPos();
				e->SetVel(e->GetVel() + Vector3::Normalize(vec) * 5);
			}
		}
	}
	++count;
}

void EnemyManager::Draw(ID3D12GraphicsCommandList * cmdList)
{
	for (auto& e : enemyList)
	{
		e->Draw(cmdList);
	}
}

void EnemyManager::SetTarget(Player* pp)
{
	pPlayer = pp;
}

std::vector<NormalEnemy*>* EnemyManager::GetNormalEnemyList()
{
	return &enemyList;
}
