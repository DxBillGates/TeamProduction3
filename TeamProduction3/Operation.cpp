#include "Operation.h"

Operation::Operation()
{
}

Operation::~Operation()
{
	delete tex;
	delete tex2;
	delete cb;
}

void Operation::LoadAsset(ID3D12Device * device, Dx12_CBVSRVUAVHeap * heap, LoadContents * loader)
{
	SpriteData sd;
	loader->CreateModelData_Plane(398, 156, sd);
	sprite.Create(device, &sd);
	tex = loader->LoadTexture("Resources/Texture/", "rush.png");
	tex2 = loader->LoadTexture("Resources/Texture/", "moveAndHot.png");
	cb = new Dx12_CBuffer<CBData>(device, heap, 1);
	cb->Map({ Matrix4::Translate(Vector3(1280 - 398,0,0)),{1,1,1,1} });
}

void Operation::Initialize()
{
	currentTex = tex2;
	pos = Vector3(1280 - 398, 0, 0);
	isFade = false;
	t = 0;
}

void Operation::Update()
{
	if (isFade)
	{
		t += 0.016f;
		Vector3 target = Vector3(2480, 0, 0);
		Vector3 lerp = Vector3::Lerp(pos, target, t);
		cb->Map({ Matrix4::Translate(lerp),{1,1,1,1} });
	}
}

void Operation::Draw(ID3D12GraphicsCommandList * cmdList)
{
	cb->Set(cmdList);
	cmdList->SetGraphicsRootDescriptorTable(2, cb->GetHeap()->GetSRVHandleForGPU(currentTex->GetSRVNumber()));
	sprite.Draw(cmdList);
}

void Operation::TextureChange()
{
	currentTex = tex;
}

void Operation::SetIsFade(bool b)
{
	isFade = b;
}
