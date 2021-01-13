#pragma once
#include"Dx12_CBVSRVUAVHeap.h"
#include"DirectXTex.h"
#include<d3d12.h>
class Dx12_Texture2D
{
private:
	ID3D12Resource* texBuff;
	int srvNumber;
	DirectX::TexMetadata metadata;
	DirectX::ScratchImage scratchImage;
	const DirectX::Image* image;
public:
	Dx12_Texture2D(const wchar_t* filename, ID3D12Device* device, Dx12_CBVSRVUAVHeap* srvHeap);
	~Dx12_Texture2D();
	DirectX::TexMetadata* GetMetadata();
	int GetSRVNumber();
};

