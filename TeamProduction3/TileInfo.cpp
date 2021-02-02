#include "TileInfo.h"

TileInfo::TileInfo()
{
}

TileInfo::~TileInfo()
{
	delete cb;
	delete tex;
}

void TileInfo::LoadAsset(ID3D12Device * device, Dx12_CBVSRVUAVHeap * heap, LoadContents * loader)
{
	SpriteData sd;
	loader->CreateModelData_Plane(32, 32, sd, true);
	sprite.Create(device, &sd);
	tex = loader->LoadTexture("Resources/Texture/", "Square.png");
	cb = new Dx12_CBuffer<CBData>(device, heap, 1);
}

void TileInfo::Initialize()
{
	pos = Vector3();
}

void TileInfo::Update()
{
	Vector3 color((float)245 / 255, (float)27 / 255, (float)27 / 255);
	cb->Map({ Matrix4::Translate(pos),{color.x,color.y,color.z,1} });
}

void TileInfo::Draw(ID3D12GraphicsCommandList * cmdList)
{
	cb->Set(cmdList);
	cmdList->SetGraphicsRootDescriptorTable(2, cb->GetHeap()->GetSRVHandleForGPU(tex->GetSRVNumber()));
	sprite.Draw(cmdList);
}

void TileInfo::SetPosition(const Vector3 & p)
{
	pos = p;
}
