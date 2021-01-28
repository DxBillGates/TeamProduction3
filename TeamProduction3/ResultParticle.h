#pragma once
#include "Dx12_Sprite.h"
#include "Dx12_Texture2D.h"
#include "Dx12_CBuffer.h"
#include "Matrix4.h"
class ResultParticle
{
private:
	struct CBData
	{
		Matrix4 mat;
		DirectX::XMFLOAT4 color;
	};
private:
	static Dx12_Sprite sprite;
	static Dx12_Texture2D* tex;
	Dx12_CBuffer<CBData>* cb;
	Vector3 pos;
	Vector3 vel;
	Vector3 scale;
	Vector3 color;
	bool liveFlag;
public:
	ResultParticle();
	~ResultParticle();
	void LoadAsset(ID3D12Device* device, Dx12_CBVSRVUAVHeap* heap, LoadContents* loader);
	static void StaticLoadAsset(ID3D12Device* device, Dx12_CBVSRVUAVHeap* heap, LoadContents* loader);
	void Initialize();
	void Emitte(Vector3 pos);
	void Update();
	void Draw(ID3D12GraphicsCommandList* cmdList);
	void SetPosition(const Vector3& p);
	Vector3 GetPosition();
	static void StaticDelete();
};

