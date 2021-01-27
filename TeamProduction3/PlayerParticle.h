#pragma once
#include "Dx12_CBuffer.h"
#include "Dx12_Mesh.h"
#include "Vector3.h"
#include "Matrix4.h"
class PlayerParticle
{
private:
	struct CBData
	{
		Matrix4 mat;
		DirectX::XMFLOAT4 color;
	};
private:
	static Dx12_Mesh mesh;
	static MeshData md;
	Dx12_CBuffer<CBData>* cb;

	Vector3 spawnPos;
	Vector3 setPos;
	Vector3 pos;
	Vector3 vel;
	Vector3 scale;
	Vector3 a = Vector3(0, 0, 0);
	bool liveFlag;
public:
	PlayerParticle();
	~PlayerParticle();
	void LoadAsset(ID3D12Device* device, Dx12_CBVSRVUAVHeap* heap);
	static void StaticLoadAsset(ID3D12Device* device, LoadContents* loader);
	void Initialize();
	void Init();
	void Update();
	void Draw(ID3D12GraphicsCommandList* pCmdList, Dx12_CBVSRVUAVHeap* heap);
	void SetPosition(Vector3 p);
	void SetLiveFlag(bool b)
	{
		liveFlag = b;
	}
};

