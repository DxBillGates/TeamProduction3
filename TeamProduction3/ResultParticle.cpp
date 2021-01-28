#include "ResultParticle.h"

Dx12_Sprite ResultParticle::sprite = Dx12_Sprite();
Dx12_Texture2D* ResultParticle::tex = nullptr;

ResultParticle::ResultParticle()
{
}

ResultParticle::~ResultParticle()
{
	delete cb;
}

void ResultParticle::LoadAsset(ID3D12Device * device, Dx12_CBVSRVUAVHeap * heap, LoadContents * loader)
{
	cb = new Dx12_CBuffer<CBData>(device, heap, 1);
}
void ResultParticle::StaticLoadAsset(ID3D12Device * device, Dx12_CBVSRVUAVHeap * heap, LoadContents * loader)
{
	tex = loader->LoadTexture("Resources/Texture/", "cube.png");
	SpriteData sd;
	loader->CreateModelData_Plane(32, 32, sd);
	sprite.Create(device, &sd);
}

void ResultParticle::Initialize()
{
	pos = {500,500,0};
	vel = {};
	scale = { 1,1,1 };
	liveFlag = false;
	color = Vector3((float)(rand() % 10) / 10, (float)(rand() % 10) / 10, (float)(rand() % 10) / 10);
}
void ResultParticle::Emitte(Vector3 pos)
{
	this->pos = pos;
	const int length = 100;
	const int division = 7;
	vel = Vector3((float)(rand() % length - length / 2) / division, (float)(rand() % length - length / 2) / division, 0);
	liveFlag = true;
	
}

void ResultParticle::Update()
{
	const float scaleFade = 1.0f;

	if (scale.x < 0)
	{
		liveFlag = false;
	}
	vel.y += 0.05f;
	pos += vel;

	cb->Map({ Matrix4::Translate(pos)*Matrix4::Scale(scale),{ color.x, color.y, color.z,1} });

}

void ResultParticle::Draw(ID3D12GraphicsCommandList * cmdList)
{
	cb->Set(cmdList);
	cmdList->SetGraphicsRootDescriptorTable(2, cb->GetHeap()->GetSRVHandleForGPU(tex->GetSRVNumber()));
	if (liveFlag)
	{
		sprite.Draw(cmdList);
	}
}

void ResultParticle::SetPosition(const Vector3 & p)
{
	pos = p;
}

Vector3 ResultParticle::GetPosition()
{
	return pos;
}

void ResultParticle::StaticDelete()
{
	delete tex;
}


