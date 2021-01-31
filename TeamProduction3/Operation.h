#pragma once
#include "Dx12_Sprite.h"
#include "Dx12_Texture2D.h"
#include "Dx12_CBuffer.h"
#include "Matrix4.h"

class Operation
{
private:
	struct CBData
	{
		Matrix4 mat;
		DirectX::XMFLOAT4 color;
	};
private:
	Dx12_Sprite sprite;
	Dx12_Texture2D* tex;
	Dx12_Texture2D* tex2;
	Dx12_Texture2D* currentTex;
	Dx12_CBuffer<CBData>* cb;
	Vector3 pos;
	float t;
	bool isFade;
public:
	Operation();
	~Operation();
	void LoadAsset(ID3D12Device* device, Dx12_CBVSRVUAVHeap* heap, LoadContents* loader);
	void Initialize();
	void Update();
	void Draw(ID3D12GraphicsCommandList* cmdList);
	void TextureChange();
	void SetIsFade(bool b);
};



