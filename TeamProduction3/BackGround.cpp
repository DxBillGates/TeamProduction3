#include "BackGround.h"

BackGround::BackGround()
	:cb1(nullptr), cb2(nullptr), pos1(Vector3()), pos2(Vector3())
{
	
}

BackGround::~BackGround()
{
	delete cb1;
	delete cb2;
	for (int i = 0; i < a; i++)
	{
		delete cb3[i];
	}
}

void BackGround::LoadAsset(ID3D12Device * pDevice, Dx12_CBVSRVUAVHeap * heap, LoadContents * loader)
{
	cb1 = new Dx12_CBuffer<CBData>(pDevice, heap, 1);
	loader->LoadMeshData("Resources/Model/haikei/", "haikei", md1);
	mesh1.Create(pDevice, &md1);

	cb2 = new Dx12_CBuffer<CBData>(pDevice, heap, 1);
	loader->LoadMeshData("Resources/Model/skydome/", "skydome", md2);
	mesh2.Create(pDevice, &md2);
	for (int i = 0; i < a; i++)
	{
		cb3[i] = new Dx12_CBuffer<CBData>(pDevice, heap, 1);
	}
	loader->LoadMeshData("Resources/Model/tree1/", "tree1", md3);
	mesh3.Create(pDevice, &md3);
}


void BackGround::Initialize()
{
	Vector3 mapLength = { 128 * 16,0,128 * 16 };
	pos1 = Vector3(-1024, 0, 1000);

	pos2 = Vector3(0, 0, 0);
	for (int y = 0; y < a; y++)
	{
		if (y < 8)
		{
			pos3[y] = Vector3(0.0f, 130.0f, 300.0f * y);
			roteY[y] = 0;
			pos3[8+y] = Vector3(mapLength.x,130.0f, mapLength.z- 290.0f * y);
			roteY[8 + y] = -1.57f;
		}
	}
}

void BackGround::Update()
{
	for (int y = 0; y < a; y++)
	{
		cb3[y]->Map({ Matrix4::Scale(Vector3(320,320,320))* Matrix4::RotationY(roteY[y]) * Matrix4::Translate(pos3[y]),{1,1,1,1} });
	}
	cb1->Map({ Matrix4::Scale(Vector3(64,64,64)) * Matrix4::Translate(pos1)* Matrix4::RotationY(3.14f),{1,1,1,1} });
	cb2->Map({ Matrix4::Scale(Vector3(50,50,50)) * Matrix4::Translate(pos2),{0.1f,0.1f,0.1f,1} });
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
void BackGround::TreeDraw(ID3D12GraphicsCommandList * pCmdList, Dx12_CBVSRVUAVHeap* heap)
{
	for (int i = 0; i < a; i++)
	{
		cb3[i]->Set(pCmdList);
		pCmdList->SetGraphicsRootDescriptorTable(2, heap->GetSRVHandleForGPU(md3.materialData.texture->GetSRVNumber()));
		mesh3.Draw(pCmdList);
	}
}
void BackGround::SetOBJPosition(Vector3 p1, Vector3 p2, Vector3 p3)
{
	pos1 = p1;
	pos2 = p2;
	
}

