#pragma once
#include "Dx12_CBuffer.h"
#include "Dx12_Mesh.h"
#include "Vector3.h"
#include "Matrix4.h"
#include"HitParticle.h"
class HitParticleManager
{
private:
	struct CBData
	{
		Matrix4 mat;
		DirectX::XMFLOAT4 color;
	};
private:
	Dx12_Mesh mesh;
	MeshData md;
	Dx12_CBuffer<CBData>* cb;
	static const int size = 10;
	HitParticle particle[size];


public:
	HitParticleManager();
	~HitParticleManager();
	void LoadAsset(ID3D12Device* pDevice, Dx12_CBVSRVUAVHeap* heap, LoadContents* loader);
	void Initialize();
	void Emitte(Vector3 pos);
	void Update();
	void Draw(ID3D12GraphicsCommandList* pCmdList, Dx12_CBVSRVUAVHeap* heap);
};

