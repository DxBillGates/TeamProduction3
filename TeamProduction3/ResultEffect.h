#pragma once
#include "Dx12_Sprite.h"
#include "Dx12_Texture2D.h"
#include "Dx12_CBuffer.h"
#include "Matrix4.h"
#include"ResultParticle.h"
class ResultEffect
{
private:
	struct CBData
	{
		Matrix4 mat;
		DirectX::XMFLOAT4 color;
	};
private:
	Dx12_Sprite sFontSprit,kingSprite,rfSprite,maiSprite;
	Dx12_Texture2D* sFontTex,*kingTex,*rfTex,*maiTex;
	Dx12_CBuffer<CBData>* sFontCB,*kingCB[3],*rfCB,*maiCB;

	static const int particleCount=30;
	ResultParticle particle[particleCount];
	Vector3 sFontPos,kingTexPos,rfPos,maiPos;

public:
	ResultEffect();
	~ResultEffect();
	void LoadAsset(ID3D12Device* device, Dx12_CBVSRVUAVHeap* heap, LoadContents* loader);
	void Initialize();
	void Emitte(Vector3 pos);
	void Update();
	void Draw(ID3D12GraphicsCommandList* cmdList);
	void SetPosition(const Vector3& p);
	Vector3 GetPosition();
};

