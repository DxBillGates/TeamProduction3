#pragma once
#include "Dx12_CBuffer.h"
#include "Dx12_Mesh.h"
#include "Vector3.h"
#include "Matrix4.h"
#include"PlayerParticle.h"
class PlayerParticleManager
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
	static const int size = 30;
	PlayerParticle particle[size];


public:
	PlayerParticleManager();
	~PlayerParticleManager();
	void LoadAsset(ID3D12Device* pDevice, Dx12_CBVSRVUAVHeap* heap, LoadContents* loader);
	void Initialize();
	void Update(float thermo);
	void Draw(ID3D12GraphicsCommandList* pCmdList, Dx12_CBVSRVUAVHeap* heap);
	void SetLiveFlag(bool b);
	void SetPosition(Vector3 p);

};

