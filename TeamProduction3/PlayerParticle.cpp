#include "PlayerParticle.h"

MeshData PlayerParticle::md = MeshData();
Dx12_Mesh PlayerParticle::mesh = Dx12_Mesh();

PlayerParticle::PlayerParticle()
	:cb(nullptr), pos(Vector3())
{
}

PlayerParticle::~PlayerParticle()
{
	delete cb;
}

void PlayerParticle::LoadAsset(ID3D12Device * pDevice, Dx12_CBVSRVUAVHeap * heap)
{
	cb = new Dx12_CBuffer<CBData>(pDevice, heap, 1);
}

void PlayerParticle::StaticLoadAsset(ID3D12Device * device, LoadContents * loader)
{
	loader->LoadMeshData("Resources/Model/", "cube", md);
	mesh.Create(device, &md);
}

void PlayerParticle::Initialize()
{
	const int SPAWNVEC = 80;
	setPos = Vector3(0, 0, 0);
	spawnPos = Vector3((float)(rand() % SPAWNVEC) - SPAWNVEC/2, 0, (float)(rand() % SPAWNVEC) - SPAWNVEC/2);
	Init();
}
void PlayerParticle::Init()
{
	vel = Vector3(0, 0, 0);
	float sRand = (float)(rand() % 45);
	scale = Vector3((float)sRand, (float)sRand, (float)sRand);
	pos = setPos + spawnPos;
}

void PlayerParticle::Update()
{
	const float scaleFade = 0.45f;
	if (scale.x > 0)
	{
		vel.y += 0.2f;
	}
	if (scale.x < 0)
	{
		Init();
	}
	pos += vel;
	scale -= Vector3(scaleFade, scaleFade, scaleFade);
	cb->Map({ Matrix4::Scale(scale) *Matrix4::RotationY(0)* Matrix4::Translate(pos),{(float)250/255,(float)10/255,0,1} });
}

void PlayerParticle::Draw(ID3D12GraphicsCommandList * pCmdList, Dx12_CBVSRVUAVHeap* heap)
{
	cb->Set(pCmdList);
	pCmdList->SetGraphicsRootDescriptorTable(2, heap->GetSRVHandleForGPU(md.materialData.texture->GetSRVNumber()));
	if(liveFlag)
	mesh.Draw(pCmdList);
}

void PlayerParticle::SetPosition(Vector3  p)
{
	setPos = p;
}

