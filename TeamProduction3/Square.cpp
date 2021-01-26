#include "Square.h"

Dx12_Mesh Square::mesh = Dx12_Mesh();
MeshData Square::md = MeshData();
Vector3 Square::size = Vector3();

Square::Square()
{
}

Square::~Square()
{
	delete cb;
}

void Square::StaticLoadAsset(ID3D12Device * device, Dx12_CBVSRVUAVHeap * heap, LoadContents * loader)
{
	//loader->LoadMeshData("Resources/Model/", "cube", md);
	loader->CreateModelData_Floor(1, 1, md);
	mesh.Create(device, &md);
	md.materialData.texture = loader->LoadTexture("Resources/Texture/", "Square.png");
}

void Square::LoadAsset(ID3D12Device * device, Dx12_CBVSRVUAVHeap * heap, LoadContents * loader)
{
	cb = new Dx12_CBuffer<CBData>(device, heap, 1);
}

void Square::Initialize()
{
	color = Vector3(1, 1, 1);
	cb->Map({ Matrix4::Scale(size) * Matrix4::Translate(pos),{color.x,color.y,color.z,1} });
}

void Square::Update()
{
	Vector3 newpos = pos;
	newpos.y = -32;
	cb->Map({ Matrix4::Scale(size) * Matrix4::Translate(newpos),{color.x,color.y,color.z,1} });
}

void Square::Draw(ID3D12GraphicsCommandList * cmdList)
{
	cb->Set(cmdList);
	cmdList->SetGraphicsRootDescriptorTable(2, cb->GetHeap()->GetSRVHandleForGPU(md.materialData.texture->GetSRVNumber()));
	mesh.Draw(cmdList);

}

Vector3 Square::GetPosition()
{
	return pos;
}

void Square::SetPosition(const Vector3 & p)
{
	pos = p;
}

Vector3 Square::GetColor()
{
	return color;
}

void Square::SetColor(const Vector3 & c)
{
	color = c;
}

Vector3 Square::GetSize()
{
	return size;
}

void Square::SetSize(const Vector3 & s)
{
	size = s;
}
