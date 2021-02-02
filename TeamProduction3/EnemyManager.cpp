#include "EnemyManager.h"
#include "NormalEnemy.h"
#include "Camera.h"
#include "ScoreManager.h"

EnemyManager::EnemyManager()
{
	const int MAX_ENEMY = 20;
	for (int i = 0; i < MAX_ENEMY; ++i)
	{
		enemyList.push_back(new NormalEnemy());
	}
	hitParticleManager.resize(MAX_ENEMY);
}

EnemyManager::~EnemyManager()
{
	for (auto& e : enemyList)
	{
		delete e;
	}

	delete hitSE;
	delete hitSEData;
}

void EnemyManager::LoadAsset(ID3D12Device * device, Dx12_CBVSRVUAVHeap * heap, LoadContents * loader)
{
	NormalEnemy::StaticLoadAsset(device, loader);
	EnemyBullet::StaticLoadAsset(device, loader);
	for (auto& e : enemyList)
	{
		e->LoadAsset(device, heap);
	}
	for (auto& p : hitParticleManager)
	{
		p.LoadAsset(device, heap, loader);
	}

	hitSEData = new SoundData("Resources/Music/hit.wav");
	hitSE = new Sound(hitSEData);
}

void EnemyManager::Initialize()
{
	count = 0;
	for (auto& e : enemyList)
	{
		e->Initialize();
		e->GetEnemyBulletPointer()->Initialize();
	}

	for (auto& p : hitParticleManager)
	{
		p.Initialize();
	}
}

void EnemyManager::Update()
{
	const float REVIVE_RANGE = 2048;    //プレイヤーの周りにスポーンする範囲
	const float TARGET_RANGE = 128;	//プレイヤーが索敵範囲外の時用の自分の周辺に飛ばす値、大きければ範囲が広がる
	const int REVIVE_COUNT = 100;	    //何フレームに一度敵を出すか
	const float RANGE = 1024;		    //索敵範囲
	const float SHOT_RANGE = 256;	    //索敵範囲内の時にどれくらいの距離から弾を発射するか
	const float DISTANCE = 64;			//敵同士当たり判定
	const float PLAYER_DISTANCE = 100;	//player当たり判定

	bool revive = false;
	if (count % REVIVE_COUNT == 0)
	{
		revive = true;
	}
	int i = 0;
	for (auto& e : enemyList)
	{
		if (revive)
		{
			if (!e->GetLiveFlag())
			{
				Vector3 randomPos;
				randomPos.x = (float)std::rand() / RAND_MAX * REVIVE_RANGE - REVIVE_RANGE / 2.0f + pPlayer->GetPosition().x;
				randomPos.z = (float)std::rand() / RAND_MAX * REVIVE_RANGE - REVIVE_RANGE / 2.0f + pPlayer->GetPosition().z;
				if (randomPos.x < 0 || randomPos.x > 2048)continue;
				if (randomPos.z < 0 || randomPos.z > 2048)continue;
				e->Revive(randomPos);
				revive = false;
			}
		}
		float dis = Vector3::Distance(pPlayer->GetPosition(), e->GetPos());
		if (dis * dis <= RANGE * RANGE && pPlayer->GetRedValue() >= 0.6f)
		{
			e->SetTargetPos(pPlayer->GetPosition());
			e->SetMoveVector(pPlayer->GetPosition() - e->GetPos());
			if (dis * dis <= SHOT_RANGE * SHOT_RANGE)
			{
				e->SetMoveVector(e->GetPos() - pPlayer->GetPosition());
			}
		}
		else
		{
			if (e->GetCoolTime() <= 0)
			{
				Vector3 randomPos;
				randomPos.x = (float)std::rand() / RAND_MAX * TARGET_RANGE - TARGET_RANGE / 2.0f + e->GetPos().x;
				randomPos.z = (float)std::rand() / RAND_MAX * TARGET_RANGE - TARGET_RANGE / 2.0f + e->GetPos().z;
				e->SetTargetPos(randomPos);
			}
			e->SetMoveVector(Vector3());
		}
		e->Update();

		if (e->GetLiveFlag())
		{
			if (Vector3::Distance(pPlayer->GetPosition(), e->GetPos()) <= PLAYER_DISTANCE && pPlayer->GetFireValue() >= 1)
			{
				hitSE->Start();
				Camera::ScreenShake();
				hitParticleManager[i].Emitte(e->GetPos());
				e->Initialize();
			}
		}

		++i;
	}

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

	for (auto& p : hitParticleManager)
	{
		p.Update();
	}
}

void EnemyManager::Draw(ID3D12GraphicsCommandList * cmdList)
{
	for (auto& e : enemyList)
	{
		e->Draw(cmdList);
	}

	for (auto& p : hitParticleManager)
	{
		p.Draw(cmdList,nullptr);
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
