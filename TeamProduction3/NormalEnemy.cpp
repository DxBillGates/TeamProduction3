#include "NormalEnemy.h"

Dx12_Mesh NormalEnemy::mesh = Dx12_Mesh();
MeshData NormalEnemy::md = MeshData();
Dx12_Mesh NormalEnemy::coolTimeMesh = Dx12_Mesh();
MeshData NormalEnemy::coolTimeMd = MeshData();


NormalEnemy::NormalEnemy()
	:Enemy()
{
}

NormalEnemy::~NormalEnemy()
{
	delete cb;
	delete coolTimeCB;
}

void NormalEnemy::LoadAsset(ID3D12Device* device, Dx12_CBVSRVUAVHeap * heap)
{
	cb = new Dx12_CBuffer<CBData>(device, heap, 1);
	coolTimeCB = new Dx12_CBuffer<CBData>(device, heap, 1);
	bullet.LoadAsset(device, heap);
}

void NormalEnemy::StaticLoadAsset(ID3D12Device * device, LoadContents * loader)
{
	loader->LoadMeshData("Resources/Model/enemy/", "enemy", md);
	loader->LoadMeshData("Resources/Model/", "cube", coolTimeMd);
	mesh.Create(device, &md);
	coolTimeMesh.Create(device, &coolTimeMd);
}

void NormalEnemy::Initialize()
{
	pos = Vector3();
	targetPos = Vector3();
	liveFlag = false;
	reviveFlag = false;
	reviveT = 0;
	vel = Vector3();
	//bullet.Initialize();
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
			//target += (targetPos - pos);
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
		pos += Vector3::Normalize(moveVector) + vel;
	}

	bullet.Update();
	cb->Map({ Matrix4::Scale(Vector3(32,32,32)) * Matrix4::Translate(pos),{0,1,0,reviveT} });
	coolTimeCB->Map({ Matrix4::Scale(Vector3(8,32 * coolTime,8)) * Matrix4::Translate(pos+Vector3(0,32+16,0)),{0,0,1,reviveT} });

	if (vel.Length() > 0)
	{
		vel -= Vector3::Normalize(vel);
	}
}

void NormalEnemy::Draw(ID3D12GraphicsCommandList * cmdList)
{
	
	bullet.Draw(cmdList);
	if (liveFlag)
	{
		cb->Set(cmdList);
		cmdList->SetGraphicsRootDescriptorTable(2, cb->GetHeap()->GetSRVHandleForGPU(md.materialData.texture->GetSRVNumber()));
		mesh.Draw(cmdList);

		coolTimeCB->Set(cmdList);
		coolTimeMesh.Draw(cmdList);
	}
}

EnemyBullet * NormalEnemy::GetEnemyBulletPointer()
{
	return &bullet;
}
