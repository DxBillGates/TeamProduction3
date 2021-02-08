#include "FireParticle.h"

MeshData FireParticle::md = MeshData();
Dx12_Mesh FireParticle::mesh = Dx12_Mesh();

FireParticle::FireParticle()
	:cb(nullptr), pos(Vector3())
{
}

FireParticle::~FireParticle()
{
	delete cb;
}

void FireParticle::LoadAsset(ID3D12Device * pDevice, Dx12_CBVSRVUAVHeap * heap)
{
	cb = new Dx12_CBuffer<CBData>(pDevice, heap, 1);
}

void FireParticle::StaticLoadAsset(ID3D12Device * device, LoadContents * loader)
{
	loader->LoadMeshData("Resources/Model/", "cube", md);
	mesh.Create(device, &md);
}



void FireParticle::Initialize()
{

	setPos = Vector3(0, 0, 0);
	spawnPos = Vector3((float)(rand() % 50)-25,0, (float)(rand() % 50)-25);
	Init();
}
void FireParticle::Init()
{
	vel = Vector3(0, 0, 0);
	float sRand = (float)(rand() % 60);
	scale = Vector3((float)sRand, (float)sRand, (float)sRand);
	pos = spawnPos;
}

void FireParticle::Update()
{
	const float scaleFade = 0.45f;
	if (scale.x > 0)
	{
		vel.y += 0.05f;
	}
	if (scale.x < 0)
	{
		Init();
	}
	pos += vel;
	scale -= Vector3(scaleFade, scaleFade, scaleFade);
	cb->Map({ Matrix4::Scale(scale) *Matrix4::RotationY(0)* Matrix4::Translate(setPos+pos),{1,0.12f,0,1} });
}

void FireParticle::Draw(ID3D12GraphicsCommandList * pCmdList, Dx12_CBVSRVUAVHeap* heap)
{
	cb->Set(pCmdList);
	//pCmdList->SetGraphicsRootDescriptorTable(2, heap->GetSRVHandleForGPU(md.materialData.texture->GetSRVNumber()));
	//if(liveFlag)
	mesh.Draw(pCmdList);
}

void FireParticle::SetPosition(Vector3  p)
{
	setPos = p;
}

