#pragma once
#include "Dx12_CBuffer.h"
#include "Dx12_Mesh.h"
#include "Matrix4.h"

class FieldEffect
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
	Vector3 size;
	bool liveFlag;
	bool reviveFlag;
	float reviveT;
	float alpha;
public:
	FieldEffect():cb(nullptr),pos(Vector3()),size(Vector3()),liveFlag(false),reviveFlag(false),reviveT(0),alpha(0) {}
	virtual ~FieldEffect() {}
	virtual void LoadAsset(ID3D12Device* device, Dx12_CBVSRVUAVHeap* heap) {}
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw(ID3D12GraphicsCommandList* cmdList) = 0;
	void Create(const Vector3& p);
	Vector3 GetPosition();
	Vector3 GetSize();
	float GetAlphaValue();
	void SetAlphaValue(float v);
	bool GetLiveFlag();
	bool GetReviveFlag();
	void SetLiveFlag(bool f);
};
