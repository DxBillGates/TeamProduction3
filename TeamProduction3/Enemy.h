#pragma once
#include "Dx12_CBuffer.h"
#include "Dx12_Mesh.h"
#include "Matrix4.h"
class Enemy
{
protected:
	struct CBData
	{
		Matrix4 mat;
		DirectX::XMFLOAT4 color;
	};
protected:
	Dx12_CBuffer<CBData>* cb;
	Vector3 pos;
	Vector3 targetPos;
	bool liveFlag;
	bool reviveFlag;
	float reviveT;
	Vector3 vel;
public:
	Enemy();
	virtual ~Enemy();
	virtual void LoadAsset(ID3D12Device* device, Dx12_CBVSRVUAVHeap* heap) = 0;
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw(ID3D12GraphicsCommandList* cmdList) = 0;
	void Revive(const Vector3& p);
	Vector3 GetPos();
	void SetPos(const Vector3& p);
	bool GetLiveFlag();
	void SetLiveFlag(bool f);
	bool GetReviveFlag();
	void SetTargetPos(const Vector3& tp);
	Vector3 GetVel();
	void SetVel(const Vector3& v);
};
