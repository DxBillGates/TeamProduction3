#include "Time.h"
#include "Easing.h"

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
	tex = loader->LoadTexture("Resources/Texture/", "num2.png");
	SpriteData spriteData = {};
	loader->CreateModelData_Plane(64, 64, spriteData,true);
	sprite.Create(device, &spriteData);
}

void Time::Initialize()
{
	time = 0;
	int value = 0;
	scale = { 1,1,1 };
	color = { 1,1,1 };
	verge = false;
	a = 0;
	b = 1;
	Vector2 vec = { (float)tex->GetMetadata()->width,(float)tex->GetMetadata()->height };
	for (int i = (int)scoreAnimetionDatas.size() - 1, j = 10, k = 1; i >= 0; --i, j *= 10, k *= 10)
	{
		value = time % j / k;
		scoreAnimetionDatas[i]->Map({ {DirectX::XMMatrixScaling(b,b,scale.z) * DirectX::XMMatrixTranslation(b*size.x * (float)i + pos.x-b,pos.y,0) }, { (float)value,0,64,64 }, { vec.x,vec.y,0,0 },{1,1,1,1} });
	}
	pos = Vector3(0,64,0);
	t = 0;
}

void Time::Update()
{

#pragma region 制限時間間近になったら赤くなって知らせる
	const int vergrTime = 5;	//点滅を始める残り時間
	
	if (time <vergrTime)
	{
		if (t >= 1)t = 1;
		verge = true;
		pos = Vector3::Lerp(Vector3(608, 32, 0), Vector3(608, 240, 0), GatesEngine::Math::Easing::EaseInCubic(t));
		t += 0.016f;
	}
	else
	{
		verge = false;
	}
	if (verge)
	{
		 color = { 1,0,0 };
		b = scale.x + sin(a)*0.1f;
		a += 0.1f;
	}
	else
	{
		 color = { 1,1,1 };
		b = 1;
		a = 0;
	}
#pragma endregion
	
	int value = 0;
	Vector2 vec = { (float)tex->GetMetadata()->width,(float)tex->GetMetadata()->height };
	for (int i = (int)scoreAnimetionDatas.size() - 1, j = 10, k = 1; i >= 0; --i, j *= 10, k *= 10)
	{
		value = time % j / k;
		scoreAnimetionDatas[i]->Map({ {DirectX::XMMatrixScaling(b,b,scale.z) * DirectX::XMMatrixTranslation(b*size.x * (float)i + pos.x,pos.y,0) }, { (float)value,0,64,64 }, { vec.x,vec.y,0,0 },{color.x,color.y,color.z,1} });
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
