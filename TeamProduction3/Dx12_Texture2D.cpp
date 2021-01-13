#include "Dx12_Texture2D.h"
#include "COMRelease.h"

Dx12_Texture2D::Dx12_Texture2D(const wchar_t * filename, ID3D12Device * device, Dx12_CBVSRVUAVHeap * srvHeap)
{
	DirectX::LoadFromWICFile(filename, DirectX::WIC_FLAGS_NONE, &metadata, scratchImage);
	image = scratchImage.GetImage(0, 0, 0);
	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(metadata.dimension);
	resDesc.Format = metadata.format;
	resDesc.Width = (UINT64)metadata.width;
	resDesc.Height = (UINT)metadata.height;
	resDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	resDesc.MipLevels = (UINT16)metadata.mipLevels;
	resDesc.SampleDesc.Count = 1;
	device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&texBuff));
	texBuff->WriteToSubresource(0, nullptr, image->pixels, (UINT)image->rowPitch, (UINT)image->slicePitch);
	srvNumber = srvHeap->GetNextSrvDescriptorNumber();
	srvHeap->CreateSRV(texBuff);
}

Dx12_Texture2D::~Dx12_Texture2D()
{
	COM_RELEASE(texBuff);
}

DirectX::TexMetadata * Dx12_Texture2D::GetMetadata()
{
	return &metadata;
}

int Dx12_Texture2D::GetSRVNumber()
{
	return srvNumber;
}
