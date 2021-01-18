#include "NormalEnemy.h"

Dx12_Mesh NormalEnemy::mesh = Dx12_Mesh();
MeshData NormalEnemy::md = MeshData();

NormalEnemy::NormalEnemy()
	:Enemy()
{
}

NormalEnemy::~NormalEnemy()
{
	delete cb;
}

void NormalEnemy::LoadAsset(ID3D12Device* device, Dx12_CBVSRVUAVHeap * heap)
{
	cb = new Dx12_CBuffer<CBData>(device, heap, 1);
	bullet.LoadAsset(device, heap);
}

void NormalEnemy::StaticLoadAsset(ID3D12Device * device, LoadContents * loader)
{
	loader->LoadMeshData("Resources/Model/", "Player", md);
	mesh.Create(device, &md);
}

void NormalEnemy::Initialize()
{
	pos = Vector3();
	targetPos = Vector3();
	liveFlag = false;
	reviveFlag = false;
	reviveT = 0;
	bullet.Initialize();
}

void NormalEnemy::Update()
{
	const float REVIVE_TIME = 1;
	const float RANGE = 320;
	const float RANDOM_RANGE = 128;
	const float COOL_TIME = 2;
	if (reviveFlag)
	{
		if (reviveT < 1)
		{
			reviveT += (0.016f / REVIVE_TIME);
		}
		if (reviveT >= 1)
		{
			reviveT = 1;
			reviveFlag = false;
			liveFlag = true;
		}
	}
	if (liveFlag)
	{
		if (!bullet.GetIsUse() && Vector3::Distance(targetPos,pos) < RANGE && coolTime == 0)
		{
			bullet.Initialize();
			bullet.SetPosition(pos);
			Vector3 target;
			target.x = (float)std::rand() / RAND_MAX * RANDOM_RANGE - RANDOM_RANGE / 2.0f + targetPos.x;
			target.z = (float)std::rand() / RAND_MAX * RANDOM_RANGE - RANDOM_RANGE / 2.0f + targetPos.z;
			target += (targetPos - pos) * ((float)(std::rand() % 3) + 1);
			bullet.SetTarget(target);
			bullet.SetIsUse(true);
			coolTime = COOL_TIME;
		}
		if (coolTime > 0)
		{
			coolTime -= 0.016f;
			if (coolTime <= 0)
			{
				coolTime = 0;
			}
		}
		Vector3 moveVector;
		moveVector = targetPos - pos;
		pos += Vector3::Normalize(moveVector);
	}

	bullet.Update();
	cb->Map({ Matrix4::Scale(Vector3(32,32,32)) * Matrix4::Translate(pos),{0,1,0,reviveT} });
}

void NormalEnemy::Draw(ID3D12GraphicsCommandList * cmdList)
{
	bullet.Draw(cmdList);
	cb->Set(cmdList);
	mesh.Draw(cmdList);
}

EnemyBullet * NormalEnemy::GetEnemyBulletPointer()
{
	return &bullet;
}
