#pragma once
#include "Dx12_Mesh.h"
#include "Dx12_CBuffer.h"
#include "Matrix4.h"

class Square
{
private:
	struct CBData
	{
		Matrix4 mat;
		DirectX::XMFLOAT4 color;
	};
private:
	static Dx12_Mesh mesh;
	static MeshData md;
	static Vector3 size;
	Dx12_CBuffer<CBData>* cb;
	Vector3 pos;
	Vector3 color;
public:
	Square();
	~Square();
	static void StaticLoadAsset(ID3D12Device* device,Dx12_CBVSRVUAVHeap* heap,LoadContents* loader);
	void LoadAsset(ID3D12Device* device, Dx12_CBVSRVUAVHeap* heap, LoadContents* loader);
	void Initialize();
	void Update();
	void Draw(ID3D12GraphicsCommandList* cmdList);
	Vector3 GetPosition();
	void SetPosition(const Vector3& p);
	Vector3 GetColor();
	void SetColor(const Vector3& c);
	static Vector3 GetSize();
	static void SetSize(const Vector3& s);
};