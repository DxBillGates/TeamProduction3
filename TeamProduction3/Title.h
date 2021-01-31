#pragma once
#include "Dx12_Sprite.h"
#include "Dx12_Texture2D.h"
#include "Dx12_CBuffer.h"
#include "Matrix4.h"
class Title
{
private:
	struct CBData
	{
		Matrix4 mat;
		DirectX::XMFLOAT4 color;
	};
private:
	Dx12_Sprite sprite;
	Dx12_Sprite sprite2;
	Dx12_Texture2D* tex;
	Dx12_Texture2D* tex2;
	Dx12_CBuffer<CBData>* cb;
	Dx12_CBuffer<CBData>* cb2;
	Vector3 pos;
	bool isFade;
	bool endFade;
	float t;
public:
	Title();
	~Title();
	void LoadAsset(ID3D12Device* device, Dx12_CBVSRVUAVHeap* heap, LoadContents* loader);
	void Initialize();
	void Update(float size = 1);
	void Draw(ID3D12GraphicsCommandList* cmdList);
	void SetPosition(const Vector3& p);
	Vector3 GetPosition();
	void SetIsFada(bool b);
	bool GetIsFade();
	bool GetEndIsFade();
};

