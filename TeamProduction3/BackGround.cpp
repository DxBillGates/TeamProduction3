#include "BackGround.h"

BackGround::BackGround()
	:cb1(nullptr), cb2(nullptr), pos1(Vector3()), pos2(Vector3())
{
}

BackGround::~BackGround()
{
	delete cb1;
	delete cb2;
}

void BackGround::LoadAsset(ID3D12Device * pDevice, Dx12_CBVSRVUAVHeap * heap, LoadContents * loader)
{
	cb1 = new Dx12_CBuffer<CBData>(pDevice, heap, 1);
	loader->LoadMeshData("Resources/Model/haikei/", "haikei", md1);
	mesh1.Create(pDevice, &md1);

	cb2 = new Dx12_CBuffer<CBData>(pDevice, heap, 1);
	loader->LoadMeshData("Resources/Model/skydome/", "skydome", md2);
	mesh2.Create(pDevice, &md2);
}

void BackGround::Initialize()
{
	pos1 = Vector3(-1024, 0, 700);
	vel1 = Vector3();
	pos2 = Vector3(0, 0, 0);
	vel2 = Vector3();

	cb1->Map({ Matrix4::Scale(Vector3(32,32,32)) * Matrix4::Translate(pos1)* Matrix4::RotationY(3.14f),{0.8f,0.8f,0.8f,1} });
	cb2->Map({ Matrix4::Scale(Vector3(80,80,80)) * Matrix4::Translate(pos2),{0.1f,0.1f,0.1f,1} });
}

void BackGround::Update()
{
}

void BackGround::Draw(ID3D12GraphicsCommandList * pCmdList, Dx12_CBVSRVUAVHeap* heap)
{
	cb1->Set(pCmdList);
	pCmdList->SetGraphicsRootDescriptorTable(2, heap->GetSRVHandleForGPU(md1.materialData.texture->GetSRVNumber()));
	mesh1.Draw(pCmdList);

	cb2->Set(pCmdList);
	pCmdList->SetGraphicsRootDescriptorTable(2, heap->GetSRVHandleForGPU(md2.materialData.texture->GetSRVNumber()));
	mesh2.Draw(pCmdList);
}

