#pragma once
#include "FieldEffect.h"

class FieldEffectFireWall : public FieldEffect
{
private:
	static Dx12_Mesh mesh;
	float remainingTime;
public:
	FieldEffectFireWall();
	~FieldEffectFireWall();
	void LoadAsset(ID3D12Device* device, Dx12_CBVSRVUAVHeap* heap);
	void Initialize();
	void Update();
	void Draw(ID3D12GraphicsCommandList* cmdList);
	static void StaticLoadAsset(ID3D12Device* device, LoadContents* loader);
};