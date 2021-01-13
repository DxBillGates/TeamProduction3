#include "EnemyManager.h"
#include "NormalEnemy.h"

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
	}
}

void EnemyManager::Update()
{
	bool revive = false;
	if (count % 60 == 0)
	{
		revive = true;
	}
	for (auto& e : enemyList)
	{
		if (revive)
		{
			if (!e->GetLiveFlag())
			{
				e->Revive();
				revive = false;
			}
		}
		e->SetTargetPos(pPlayer->GetPosition());
		e->Update();

		if (Vector3::Distance(pPlayer->GetPosition(), e->GetPos()) <= 32 && pPlayer->GetFireValue() >= 1)
		{
			e->Initialize();
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
