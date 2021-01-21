#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Dx12_Sprite.h"
#include "Dx12_CBuffer.h"
class Time
{
private:
	struct AnimetionData
	{
		DirectX::XMMATRIX model;
		DirectX::XMFLOAT4 ancerAndSize;
		DirectX::XMFLOAT4 texSize;
	};
private:
	Vector3 pos;
	Vector3 size;
	int time;
	std::vector<Dx12_CBuffer<AnimetionData>*> scoreAnimetionDatas;
	Dx12_Sprite sprite;
	Dx12_Texture2D* tex;
public:
	Time();
	~Time();
	void LoadAsset(ID3D12Device* device, Dx12_CBVSRVUAVHeap* heap, LoadContents* loader);
	void Initialize();
	void Update();
	void Draw(ID3D12GraphicsCommandList* cmdList);
	void SetTime(int time = 0);
	void SetSize(Vector3 size);
	void SetPosition(const Vector3& p);
};

