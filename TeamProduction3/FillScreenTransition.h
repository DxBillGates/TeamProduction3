#pragma once
#include "SceneTransition.h"
#include "Dx12_Pipeline.h"
#include "Dx12_CBuffer.h"
#include "Dx12_Sprite.h"
#include "Matrix4.h"

class FillScreenTransition : public SceneTransition
{
private:
	struct CBData
	{
		Matrix4 modelMat;
		DirectX::XMFLOAT4 color;
	};
private:
	float alpha;
	Dx12_CBuffer<DirectX::XMMATRIX>* orthograph;
	Dx12_Pipeline* shader;
	Dx12_CBuffer<CBData>* cb;
	Dx12_Sprite sprite;
public:
	FillScreenTransition();
	~FillScreenTransition();
	void LoadAsset(ID3D12Device* device, Dx12_CBVSRVUAVHeap* heap,LoadContents* loader);
	void Initialize();
	void Update();
	void Draw(ID3D12GraphicsCommandList* cmdList);
};
