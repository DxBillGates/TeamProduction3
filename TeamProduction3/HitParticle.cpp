#include "HitParticle.h"

MeshData HitParticle::md = MeshData();
Dx12_Mesh HitParticle::mesh = Dx12_Mesh();

HitParticle::HitParticle()
	:cb(nullptr), pos(Vector3())
{
}

HitParticle::~HitParticle()
{
	delete cb;
}

void HitParticle::LoadAsset(ID3D12Device * pDevice, Dx12_CBVSRVUAVHeap * heap)
{
	cb = new Dx12_CBuffer<CBData>(pDevice, heap, 1);
}

void HitParticle::StaticLoadAsset(ID3D12Device * device, LoadContents * loader)
{
	loader->LoadMeshData("Resources/Model/", "cube", md);
	mesh.Create(device, &md);
}
void HitParticle::Initialize()
{
	pos = {};
	vel = {};
	scale = { 1,1,1 };
	liveFlag = false;
}
void HitParticle::Emitte(Vector3 pos)
{
	this->pos = pos;
	//
	const int length = 90;
	const int division = 10;
	vel = Vector3((float)(rand() % length - length / 2) / division, (float)(rand() % length) / division, (float)(rand() % length - length / 2) / division);
	scale = { 20.0f,20.0f,20.0f };
	liveFlag = true;;
}

void HitParticle::Update()
{
	const float scaleFade = 0.25f;

	if (scale.x < 0)
	{
		liveFlag = false;
	}
	vel.y -= 0.5f;
	pos += vel;
	scale -= Vector3(scaleFade, scaleFade, scaleFade);
	cb->Map({ Matrix4::Scale(scale) *Matrix4::RotationY(0)* Matrix4::Translate(pos),{0.0f,(float)250 / 255,(float)10 / 255,1} });
}

void HitParticle::Draw(ID3D12GraphicsCommandList * pCmdList, Dx12_CBVSRVUAVHeap* heap)
{
	cb->Set(pCmdList);
	pCmdList->SetGraphicsRootDescriptorTable(2, cb->GetHeap()->GetSRVHandleForGPU(md.materialData.texture->GetSRVNumber()));
	if (liveFlag)
		mesh.Draw(pCmdList);
}

void HitParticle::SetPosition(const Vector3 & p)
{
	spawnPos = p;
}

Vector3 HitParticle::GetPosition()
{
	return pos;
}
