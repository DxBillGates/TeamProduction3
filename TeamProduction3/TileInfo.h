#pragma once
#include "Dx12_Sprite.h"
#include "Dx12_CBuffer.h"
#include "Matrix4.h"
class TileInfo
{
private:
	struct CBData
	{
		Matrix4 mat;
		DirectX::XMFLOAT4 color;
	};
private:
	Dx12_Sprite sprite;
	Dx12_CBuffer<CBData>* cb;
	Dx12_Texture2D* tex;
	Vector3 pos;
public:
	TileInfo();
	~TileInfo();
	void LoadAsset(ID3D12Device* device, Dx12_CBVSRVUAVHeap* heap, LoadContents* loader);
	void Initialize();
	void Update();
	void Draw(ID3D12GraphicsCommandList* cmdList);
	void SetPosition(const Vector3& p);
};

