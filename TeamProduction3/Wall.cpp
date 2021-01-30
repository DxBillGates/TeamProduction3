#include "Wall.h"

MeshData Wall::md = MeshData();
Dx12_Mesh Wall::mesh = Dx12_Mesh();

Wall::Wall()
{
}

Wall::~Wall()
{
	delete cb;
}

void Wall::StaticLoadAsset(ID3D12Device * device, Dx12_CBVSRVUAVHeap * heap, LoadContents * loader)
{
	loader->LoadMeshData("Resources/Model/", "cube", md);
	mesh.Create(device, &md);
}

void Wall::LoadAsset(ID3D12Device * device, Dx12_CBVSRVUAVHeap * heap, LoadContents * loader)
{
	cb = new Dx12_CBuffer<CBData>(device, heap, 1);
}

void Wall::Initialize()
{
	pos = Vector3();
	size = Vector3(32,32,32);
}

void Wall::Update()
{
	cb->Map({ Matrix4::Scale(size) * Matrix4::Translate(pos),{1,1,1,1} });
}

void Wall::Draw(ID3D12GraphicsCommandList* cmdList)
{
	cb->Set(cmdList);
	mesh.Draw(cmdList);
}

void Wall::SetSize(const Vector3 & s)
{
	size = s;
}

Vector3 Wall::GetSize()
{
	return size;
}

void Wall::SetPosition(const Vector3 & p)
{
	pos = p;
}

Vector3 Wall::GetPosition()
{
	return Vector3();
}
