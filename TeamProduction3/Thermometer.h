#pragma once
#include "Dx12_CBuffer.h"
#include "Dx12_Mesh.h"
#include "Vector3.h"
#include "Matrix4.h"
class Thermometer
{
private:
	struct CBData
	{
		Matrix4 mat;
		DirectX::XMFLOAT4 color;
	};
private:
	Dx12_Mesh mesh;
	MeshData md;
	Dx12_CBuffer<CBData>* cb;

	Vector3 billBoradPos;
	Vector3 direction;
	Vector3 color;
	Vector3 position;
	Vector3 scale;
	bool liveFlag;
public:
	Thermometer();
	~Thermometer();
	void LoadAsset(ID3D12Device* pDevice, Dx12_CBVSRVUAVHeap* heap, LoadContents* loader);
	void Initialize();
	void Update(Vector3 pos,float thermol,Vector3 cameraPos);
	void Draw(ID3D12GraphicsCommandList* pCmdList, Dx12_CBVSRVUAVHeap* heap);
	
	Vector3 GetPosition();
};

