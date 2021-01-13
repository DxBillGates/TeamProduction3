#pragma once
#include "FieldEffect.h"

//�����܂�t�B�[���h�G�t�F�N�g
class FieldEffectPuddle : public FieldEffect
{
private:
	static Dx12_Mesh mesh;
public:
	FieldEffectPuddle();
	~FieldEffectPuddle();
	void LoadAsset(ID3D12Device* device, Dx12_CBVSRVUAVHeap* heap);
	void Initialize();
	void Update();
	void Draw(ID3D12GraphicsCommandList* cmdList);
	static void StaticLoadAsset(ID3D12Device* device, LoadContents* loader);
};