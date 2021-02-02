#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Dx12_Sprite.h"
#include "Dx12_CBuffer.h"
class Score
{
private:
	struct AnimetionData
	{
		DirectX::XMMATRIX model;
		DirectX::XMFLOAT4 ancerAndSize;
		DirectX::XMFLOAT4 texSize;
		DirectX::XMFLOAT4 color;
	};

private:
	Vector3 pos;
	Vector3 size;
	Vector3 scale;
	Vector3 color;
	bool effectFlag;
	float a, b;
	int score;
	std::vector<Dx12_CBuffer<AnimetionData>*> scoreAnimetionDatas;
	static Dx12_Sprite sprite;
	static Dx12_Texture2D* tex;


public:
	Score();
	~Score();
	void LoadAsset(ID3D12Device* device, Dx12_CBVSRVUAVHeap* heap, LoadContents* loader);
	static void StaticLoadAsset(ID3D12Device* device, Dx12_CBVSRVUAVHeap* heap, LoadContents* loader);
	static void DestroyAsset();
	void Initialize();
	void Update();
	void Draw(ID3D12GraphicsCommandList* cmdList);
	void SetEffectFlag();
	void SetScore(int value = 0);
	int GetScore();
	void SetSize(Vector3 size);
	void SetPosition(const Vector3& p);
	void SetScale(Vector3 scale);
	void SetColor(Vector3 color);
};

