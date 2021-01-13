#pragma once
#pragma once
#include"LoadContents.h"
#include"Dx12_VertexData.h"
#include<d3d12.h>
#include<DirectXMath.h>
#include<vector>
class Dx12_Sprite
{
private:
	ID3D12Resource* vBuff;
	ID3D12Resource* iBuff;
	D3D12_VERTEX_BUFFER_VIEW vbView;
	D3D12_INDEX_BUFFER_VIEW ibView;
public:
	Dx12_Sprite();
	~Dx12_Sprite();
	void Create(ID3D12Device* device, SpriteData* spriteData);
	void Create(ID3D12Device* device, std::vector<VertexUV>* vertices, std::vector<unsigned short>* indices);
	void Draw(ID3D12GraphicsCommandList* cmdList);
	ID3D12Resource* GetVBuff();
	ID3D12Resource* GetIBuff();
	D3D12_VERTEX_BUFFER_VIEW* GetVBView();
	D3D12_INDEX_BUFFER_VIEW* GetIBView();
	UINT GetIndicesCount();
};



