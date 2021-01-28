#include "Time.h"

Time::Time()
{
	scoreAnimetionDatas.resize(3);
}

Time::~Time()
{
	for (auto& s : scoreAnimetionDatas)
	{
		delete s;
	}
	delete tex;
}

void Time::LoadAsset(ID3D12Device * device, Dx12_CBVSRVUAVHeap * heap, LoadContents * loader)
{
	for (auto& s : scoreAnimetionDatas)
	{
		s = new Dx12_CBuffer<AnimetionData>(device, heap, 1);
	}
	tex = loader->LoadTexture("Resources/Texture/", "whiteNum.png");
	SpriteData spriteData = {};
	loader->CreateModelData_Plane(32, 32, spriteData);
	sprite.Create(device, &spriteData);
}

void Time::Initialize()
{
	time = 0;
	int value = 0;
	Vector2 vec = { (float)tex->GetMetadata()->width,(float)tex->GetMetadata()->height };
	for (int i = (int)scoreAnimetionDatas.size() - 1, j = 10, k = 1; i >= 0; --i, j *= 10, k *= 10)
	{
		value = time % j / k;
		scoreAnimetionDatas[i]->Map({ {DirectX::XMMatrixScaling(1,1,1) * DirectX::XMMatrixTranslation(size.x * (float)i + pos.x,pos.y,0) }, { (float)value,0,128,128 }, { vec.x,vec.y,0,0 },{1,1,1,1} });
	}
}

void Time::Update()
{
	int value = 0;
	Vector2 vec = { (float)tex->GetMetadata()->width,(float)tex->GetMetadata()->height };
	for (int i = (int)scoreAnimetionDatas.size() - 1, j = 10, k = 1; i >= 0; --i, j *= 10, k *= 10)
	{
		value = time % j / k;
		scoreAnimetionDatas[i]->Map({ {DirectX::XMMatrixScaling(1,1,1) * DirectX::XMMatrixTranslation(size.x * (float)i + pos.x,pos.y,0) }, { (float)value,0,128,128 }, { vec.x,vec.y,0,0 },{1,1,1,1} });
	}
}

void Time::Draw(ID3D12GraphicsCommandList * cmdList)
{
	cmdList->SetGraphicsRootDescriptorTable(2, scoreAnimetionDatas[0]->GetHeap()->GetSRVHandleForGPU(tex->GetSRVNumber()));
	for (auto& s : scoreAnimetionDatas)
	{
		s->Set(cmdList);
		sprite.Draw(cmdList);
	}
}

void Time::SetTime(int time)
{
	this->time = time;
}

void Time::SetSize(Vector3 size)
{
	this->size = size;
}

void Time::SetPosition(const Vector3 & p)
{
	pos = p;
}
