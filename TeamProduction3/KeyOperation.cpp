#include "KeyOperation.h"

KeyOperation::KeyOperation()
{
}

KeyOperation::~KeyOperation()
{
	delete tex;
	delete cb;
}

void KeyOperation::LoadAsset(ID3D12Device * device, Dx12_CBVSRVUAVHeap * heap, LoadContents * loader)
{
	SpriteData sd;
	loader->CreateModelData_Plane(1280, 64, sd);
	sprite.Create(device, &sd);
	tex = loader->LoadTexture("Resources/Texture/", "keys.png");
	cb = new Dx12_CBuffer<CBData>(device, heap, 1);
	cb->Map({ Matrix4::Translate(Vector3(0,720 - 64,0)),{1,1,1,1} });
}

void KeyOperation::Initialize()
{
}

void KeyOperation::Update()
{
}

void KeyOperation::Draw(ID3D12GraphicsCommandList * cmdList)
{
	cb->Set(cmdList);
	cmdList->SetGraphicsRootDescriptorTable(2, cb->GetHeap()->GetSRVHandleForGPU(tex->GetSRVNumber()));
	sprite.Draw(cmdList);
}
