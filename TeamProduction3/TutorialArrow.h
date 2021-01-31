#pragma once
#include "Dx12_CBuffer.h"
#include "Dx12_Mesh.h"
#include "Matrix4.h"

class TutorialArrow
{
private:
	struct CBData
	{
		Matrix4 mat;
		DirectX::XMFLOAT4 color;
	};
private:
	Dx12_Mesh mesh;
	Dx12_Texture2D* tex;
	Dx12_CBuffer<CBData>* cb;
	Vector3 pos;
	Vector3 size;
	Vector3 angle;
	int t;
	bool isDraw;
public:
	TutorialArrow();
	~TutorialArrow();
	void Initialize();
	void LoadAsset(ID3D12Device* device, Dx12_CBVSRVUAVHeap* heap, LoadContents* loader);
	void Update(const Vector3& p = Vector3(), const Vector3& forward = Vector3());
	void Draw(ID3D12GraphicsCommandList* cmdList);
};

