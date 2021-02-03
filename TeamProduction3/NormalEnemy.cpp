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

	delete shotSE;
	delete shotSEData;
}

void NormalEnemy::LoadAsset(ID3D12Device* device, Dx12_CBVSRVUAVHeap * heap)
{
	cb = new Dx12_CBuffer<CBData>(device, heap, 1);
	coolTimeCB = new Dx12_CBuffer<CBData>(device, heap, 1);
	bullet.LoadAsset(device, heap);

	shotSEData = new SoundData("Resources/Music/waterShot2.wav");
	shotSE = new Sound(shotSEData);
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
	moveVector = Vector3();
	coolTime = 2.2f;
}

void NormalEnemy::Update()
{
	const float REVIVE_TIME = 1;       //•œŠˆŠÔ(•b)
	const float RANGE = 256;	       //‚Ç‚ê‚­‚ç‚¢‚Ì‹——£‚Å‹…‚ğ‘Å‚Â‚©
	const float RANDOM_RANGE = 256;    //’e‚Ì¸“x(”ÍˆÍ)A’l‚ª‘å‚«‚¯‚ê‚Î‘å‚«‚¢‚¾‚¯¸“x‚ª—‚¿‚é
	const float COOL_TIME = 5;	       //’e‚ÌÄ‘•“UŠÔ
	const float SPEED = 1.4f;          //ˆÚ“®•ûŒü‚ÉSPEED”{‚·‚é’l
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
		if (!bullet.GetIsUse() /*&& Vector3::Distance(targetPos,pos) < RANGE*/ && coolTime == 0)
		{
			bullet.Initialize();
			bullet.SetPosition(pos);
			bullet.SetSpawnPosition(pos);
			Vector3 target;
			target.x = (float)std::rand() / RAND_MAX * RANDOM_RANGE - RANDOM_RANGE / 2.0f + targetPos.x;
			target.z = (float)std::rand() / RAND_MAX * RANDOM_RANGE - RANDOM_RANGE / 2.0f + targetPos.z;
			bullet.SetTarget(target);
			bullet.SetIsUse(true);
			coolTime = COOL_TIME;
			shotSE->Start();
		}
		if (coolTime > 0)
		{
			coolTime -= 0.016f;
			if (coolTime <= 0)
			{
				coolTime = 0;
			}
		}
		moveVector = moveVector.Normalize();
		pos += moveVector + vel + moveVector * SPEED;
	}

	bullet.Update();
	Vector3 v = targetPos - pos;
	v = v.Normalize();
	float angle = atan2f(v.x, v.z);
	cb->Map({ Matrix4::Scale(Vector3(32,32,32)) *Matrix4::RotationY(angle)* Matrix4::Translate(pos),{1,1,1,reviveT} });
	coolTimeCB->Map({ Matrix4::Scale(Vector3(32 * coolTime,8,8)) *Matrix4::RotationY(angle)* Matrix4::Translate(pos + Vector3(0,32 + 16,0)),{0,0,1,reviveT} });

	if (vel.Length() > 0)
	{
		vel -= Vector3::Normalize(vel);
	}
}

void NormalEnemy::Draw(ID3D12GraphicsCommandList * cmdList)
{
	if (bullet.GetIsUse())
		bullet.Draw(cmdList);
	if (liveFlag)
	{
		coolTimeCB->Set(cmdList);
		coolTimeMesh.Draw(cmdList);
		cb->Set(cmdList);
		cmdList->SetGraphicsRootDescriptorTable(2, cb->GetHeap()->GetSRVHandleForGPU(md.materialData.texture->GetSRVNumber()));
		mesh.Draw(cmdList);

	}
}

EnemyBullet * NormalEnemy::GetEnemyBulletPointer()
{
	return &bullet;
}

void NormalEnemy::SetMoveVector(const Vector3 & v)
{
	moveVector = v;
}

float NormalEnemy::GetCoolTime()
{
	return coolTime;
}
