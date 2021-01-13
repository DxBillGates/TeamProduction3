#pragma once
#include "Dx12_CBuffer.h"
#include "Dx12_Mesh.h"
#include "Vector3.h"
#include "Matrix4.h"
class Ground
{
private:
	struct CBData
	{
		Matrix4 mat;
		DirectX::XMFLOAT4 color;
	};
private:
	Dx12_Mesh mesh;
	Dx12_CBuffer<CBData>* cb;
	MeshData md;
	Vector3 size;
public:
	Ground();
	~Ground();
	void LoadAsset(ID3D12Device* pDevice, Dx12_CBVSRVUAVHeap* heap, LoadContents* loader);
	void Initialize();
	void Update();
	void Draw(ID3D12GraphicsCommandList* pCmdList, Dx12_CBVSRVUAVHeap* heap);
	void SetSize(const Vector3& s);
	Vector3 GetSize();
};

