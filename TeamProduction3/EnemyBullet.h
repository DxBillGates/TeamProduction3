#pragma once
#include "Dx12_CBuffer.h"
#include "Dx12_Mesh.h"
#include "Matrix4.h"

class EnemyBullet
{
private:
	struct CBData
	{
		Matrix4 mat;
		DirectX::XMFLOAT4 color;
	};
	static MeshData md;
	static Dx12_Mesh mesh;
	Dx12_CBuffer<CBData>* cb;
private:
	Vector3 spPos;
	Vector3 pos;
	Vector3 vel;
	Vector3 target;
	bool isUse;
	bool isOldUse;
public:
	EnemyBullet();
	~EnemyBullet();
	void LoadAsset(ID3D12Device* device, Dx12_CBVSRVUAVHeap* heap);
	static void StaticLoadAsset(ID3D12Device* device, LoadContents* loader);
	void Initialize();
	void Update();
	void Draw(ID3D12GraphicsCommandList* cmdList);
	void SetTarget(const Vector3& tp,float value = 1);
	void SetPosition(const Vector3& p);
	Vector3 GetPosition();
	void SetIsUse(bool b);
	bool GetIsUse();
	bool GetIsOldUse();
	Vector3 GetLostPosition();
	void SetSpawnPosition(const Vector3& p);
};