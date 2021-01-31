#pragma once
#include "FieldEffect.h"
#include "Sound.h"

//水たまりフィールドエフェクト
class FieldEffectPuddle : public FieldEffect
{
private:
	static MeshData md;
	static Dx12_Mesh mesh;
	Sound* se;
	SoundData* seData;
public:
	FieldEffectPuddle();
	~FieldEffectPuddle();
	void LoadAsset(ID3D12Device* device, Dx12_CBVSRVUAVHeap* heap);
	void Initialize();
	void Update();
	void Draw(ID3D12GraphicsCommandList* cmdList);
	static void StaticLoadAsset(ID3D12Device* device, LoadContents* loader);
	Sound* GetSE();
};