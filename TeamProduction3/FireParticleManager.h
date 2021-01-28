#pragma once
#include "Dx12_CBuffer.h"
#include "Dx12_Mesh.h"
#include "Vector3.h"
#include "Matrix4.h"
#include"FireParticle.h"
class FireParticleManager
{
private:
	struct CBData
	{
		Matrix4 mat;
		DirectX::XMFLOAT4 color;
	};
private:
	static const int size = 5;
	FireParticle particle[size];


public:
	FireParticleManager();
	~FireParticleManager();
	void LoadAsset(ID3D12Device* pDevice, Dx12_CBVSRVUAVHeap* heap, LoadContents* loader);
	void Initialize();
	void Update();
	void Draw(ID3D12GraphicsCommandList* pCmdList, Dx12_CBVSRVUAVHeap* heap);
	void SetLiveFlag(bool b);
	void SetPosition(Vector3 p);

};

