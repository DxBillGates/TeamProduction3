#pragma once
#include "Dx12_Mesh.h"
#include "Dx12_CBuffer.h"
#include "Matrix4.h"
#include "Vector3.h"

class Wall
{
private:
	struct CBData
	{
		Matrix4 mat;
		DirectX::XMFLOAT4 color;
	};
private:
	static MeshData md;
	static Dx12_Mesh mesh;
	Dx12_CBuffer<CBData>* cb;
	Vector3 pos;
	Vector3 size;
public:
	Wall();
	~Wall();
	static void StaticLoadAsset(ID3D12Device* device, Dx12_CBVSRVUAVHeap* heap, LoadContents* loader);
	void LoadAsset(ID3D12Device* device, Dx12_CBVSRVUAVHeap* heap, LoadContents* loader);
	void Initialize();
	void Update();
	void Draw(ID3D12GraphicsCommandList* cmdList);
	void SetSize(const Vector3& s);
	Vector3 GetSize();
	void SetPosition(const Vector3& p);
	Vector3 GetPosition();
};