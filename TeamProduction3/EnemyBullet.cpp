#include "EnemyBullet.h"

MeshData EnemyBullet::md = MeshData();
Dx12_Mesh EnemyBullet::mesh = Dx12_Mesh();

EnemyBullet::EnemyBullet():cb(nullptr),pos(Vector3()),vel(Vector3()),target(Vector3()),isUse(false),isOldUse(false)
{
}

EnemyBullet::~EnemyBullet()
{
	delete cb;
}

void EnemyBullet::LoadAsset(ID3D12Device * device, Dx12_CBVSRVUAVHeap * heap)
{
	cb = new Dx12_CBuffer<CBData>(device, heap, 1);
}

void EnemyBullet::StaticLoadAsset(ID3D12Device * device, LoadContents * loader)
{
	loader->LoadMeshData("Resources/Model/", "cube", md);
	mesh.Create(device, &md);
}

void EnemyBullet::Initialize()
{
	spPos = Vector3();
	pos = Vector3();
	vel = Vector3();
	target = Vector3();
	isUse = false;
	isOldUse = false;
}

void EnemyBullet::Update()
{
	const float RANGE = 300;    //‘Å‚Á‚½êŠ‚©‚ç‚Ç‚Ì‚­‚ç‚¢‚Å…‚½‚Ü‚è‚É‚·‚é‚©A’l‚ª‘å‚«‚¯‚ê‚Î‰“‚­‚Ü‚Å”ò‚Î‚¹‚é
	isOldUse = isUse;
	if (isUse)
	{
		pos += vel;
		if (Vector3::Distance(spPos, pos) >= RANGE)
		{
			isUse = false;
		}
		//if (Vector3::Distance(pos, target) <= 16)
		//{
		//	isUse = false;
		//}
	}
	float a = (isUse) ? 1.0f : 0.0f;
	cb->Map({ Matrix4::Scale(Vector3(16,16,16)) * Matrix4::Translate(pos),{0,0,1,a} });
}

void EnemyBullet::Draw(ID3D12GraphicsCommandList * cmdList)
{
	cb->Set(cmdList);
	cmdList->SetGraphicsRootDescriptorTable(2, cb->GetHeap()->GetSRVHandleForGPU(md.materialData.texture->GetSRVNumber()));
	mesh.Draw(cmdList);
}

void EnemyBullet::SetTarget(const Vector3 & tp,float value)
{
	target = tp;
	vel = Vector3::Normalize(target - pos) * 6;
}

void EnemyBullet::SetPosition(const Vector3 & p)
{
	pos = p;
}

Vector3 EnemyBullet::GetPosition()
{
	return pos;
}

void EnemyBullet::SetIsUse(bool b)
{
	isUse = b;
}

bool EnemyBullet::GetIsUse()
{
	return isUse;
}

bool EnemyBullet::GetIsOldUse()
{
	return isOldUse;
}

Vector3 EnemyBullet::GetLostPosition()
{
	return Vector3(target.x,1,target.z);
}

void EnemyBullet::SetSpawnPosition(const Vector3 & p)
{
	spPos = p;
}
