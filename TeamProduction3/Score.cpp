#include "Score.h"

Dx12_Sprite Score::sprite = Dx12_Sprite();
Dx12_Texture2D* Score::tex = nullptr;

Score::Score():pos(Vector3()),size(Vector3()),score(0)
{
	scoreAnimetionDatas.resize(8);
}

Score::~Score()
{
	for (auto itr = scoreAnimetionDatas.begin(); itr != scoreAnimetionDatas.end(); ++itr)
	{
		delete (*itr);
	}
}

void Score::LoadAsset(ID3D12Device * device, Dx12_CBVSRVUAVHeap * heap, LoadContents * loader)
{
	for (auto itr = scoreAnimetionDatas.begin(); itr != scoreAnimetionDatas.end(); ++itr)
	{
		(*itr) = new Dx12_CBuffer<AnimetionData>(device, heap, 1);
	}
}

void Score::StaticLoadAsset(ID3D12Device * device, Dx12_CBVSRVUAVHeap * heap, LoadContents * loader)
{
	tex = loader->LoadTexture("Resources/Texture/", "blackNum.png");
	SpriteData spriteData = {};
	loader->CreateModelData_Plane(32, 32, spriteData);
	sprite.Create(device, &spriteData);
}

void Score::DestroyAsset()
{
	delete tex;
}

void Score::Initialize()
{
	pos = Vector3();
	size = Vector3(32,32,32);
	score = 0;

	int value = 0;
	Vector2 vec = { (float)tex->GetMetadata()->width,(float)tex->GetMetadata()->height };
	for (int i = (int)scoreAnimetionDatas.size() - 1, j = 10, k = 1; i >= 0; --i, j *= 10, k *= 10)
	{
		value = score % j / k;
		scoreAnimetionDatas[i]->Map({ {DirectX::XMMatrixScaling(1,1,1) * DirectX::XMMatrixTranslation(size.x * (float)i + pos.x,pos.y,0) }, { (float)value,0,64,64 }, { vec.x,vec.y,0,0 },{1,1,1,1} });
	}
}

void Score::Update()
{
	int value = 0;
	Vector2 vec = { (float)tex->GetMetadata()->width,(float)tex->GetMetadata()->height };
	for (int i = (int)scoreAnimetionDatas.size() - 1, j = 10, k = 1; i >= 0; --i, j *= 10, k *= 10)
	{
		value = score % j / k;
		scoreAnimetionDatas[i]->Map({ {DirectX::XMMatrixScaling(1,1,1) * DirectX::XMMatrixTranslation(size.x * (float)i + pos.x,pos.y,0) }, { (float)value,0,64,64 }, { vec.x,vec.y,0,0 },{1,1,1,1} });
	}
}

void Score::Draw(ID3D12GraphicsCommandList * cmdList)
{
	cmdList->SetGraphicsRootDescriptorTable(2, scoreAnimetionDatas[0]->GetHeap()->GetSRVHandleForGPU(tex->GetSRVNumber()));
	for (auto& s : scoreAnimetionDatas)
	{
		s->Set(cmdList);
		sprite.Draw(cmdList);
	}
}

void Score::SetScore(int value)
{
	score = value;
}

int Score::GetScore()
{
	return score;
}

void Score::SetSize(Vector3 size)
{
	this->size = size;
}

void Score::SetPosition(const Vector3 & p)
{
	pos = p;
}
