#include "Ground.h"

Ground::Ground():mesh(Dx12_Mesh()),cb(nullptr),md(MeshData()),size(Vector3(128,128,128))
{
}

Ground::~Ground()
{
	delete cb;
}

void Ground::LoadAsset(ID3D12Device * pDevice, Dx12_CBVSRVUAVHeap * heap, LoadContents * loader)
{
	cb = new Dx12_CBuffer<CBData>(pDevice, heap, 1);
	//loader->LoadMeshData("Resources/Model/", "cube", md);
	loader->CreateModelData_Plane(32, 32, md);
	mesh.Create(pDevice, &md);
}

void Ground::Initialize()
{
	pos = Vector3(0, 0, 1);
	size = Vector3(1,1,1);
	billBoard = Matrix4::Identity();
}

void Ground::Update()
{
	cb->Map({ (Matrix4::Scale(size) * Matrix4::Translate(pos))*billBoard,{1,1,1,1} });
}

void Ground::Draw(ID3D12GraphicsCommandList * pCmdList, Dx12_CBVSRVUAVHeap * heap)
{
	cb->Set(pCmdList);
	mesh.Draw(pCmdList);
}

void Ground::SetSize(const Vector3 & s)
{
	size = s;
}

void Ground::SetPosition(const Vector3 & p)
{
	pos = p;
}

Vector3 Ground::GetSize()
{
	return size;
}

void Ground::SetBillBoard(const Matrix4 & b)
{
	billBoard = b;
}
