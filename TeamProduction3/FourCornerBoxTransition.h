#pragma once
#include "SceneTransition.h"
#include "Dx12_Pipeline.h"
#include "Dx12_CBuffer.h"
#include "Dx12_Sprite.h"
#include "Matrix4.h"

class FourCornerBoxTransition : public SceneTransition
{
private:
	struct CBData
	{
		Matrix4 modelMat;
		DirectX::XMFLOAT4 color;
	};
private:
	Dx12_Sprite sprite;
	Dx12_Pipeline* shader;
	Dx12_CBuffer<DirectX::XMMATRIX>* orthograph;
	std::vector<Dx12_CBuffer<CBData>*> fourCornerBoxCB;
public:
	FourCornerBoxTransition();
	~FourCornerBoxTransition();
	void LoadAsset(ID3D12Device* device, Dx12_CBVSRVUAVHeap* heap, LoadContents* loader);
	void Initialize();
	void Update();
	void Draw(ID3D12GraphicsCommandList* cmdList);
};
