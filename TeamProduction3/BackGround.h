#pragma once
#include "Dx12_CBuffer.h"
#include "Dx12_Mesh.h"
#include "Vector3.h"
#include "Matrix4.h"
#include "Quaternion.h"
class BackGround
{
private:
	struct CBData
	{
		Matrix4 mat;
		DirectX::XMFLOAT4 color;
	};
private:
	static const int a = 16
		;
	Dx12_Mesh mesh1, mesh2, mesh3;
	MeshData md1, md2, md3;
	Dx12_CBuffer<CBData>* cb1, *cb2, *cb3[a];
	float roteY[a];

	Vector3 pos1, pos2, pos3[a];
public:
	BackGround();
	~BackGround();
	void LoadAsset(ID3D12Device* pDevice, Dx12_CBVSRVUAVHeap* heap, LoadContents* loader);
	void Initialize();
	void Update();
	void Draw(ID3D12GraphicsCommandList* pCmdList, Dx12_CBVSRVUAVHeap* heap);
	void TreeDraw(ID3D12GraphicsCommandList* pCmdList, Dx12_CBVSRVUAVHeap* heap);
	void SetOBJPosition(Vector3 p1, Vector3 p2, Vector3 p3);
};

