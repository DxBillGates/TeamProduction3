#include "Dx12_CBVSRVUAVHeap.h"
#include "COMRelease.h"

Dx12_CBVSRVUAVHeap::Dx12_CBVSRVUAVHeap(ID3D12Device * device, ID3D12GraphicsCommandList * cmdList, int cbvNum, int srvNum, int uavNum)
{
	pDevice = device;
	pCmdList = cmdList;
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.NodeMask = 0;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapDesc.NumDescriptors = cbvNum + srvNum + uavNum;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	pDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&mHeap));
	srvStartOffset = cbvNum;
	incrementSize = pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	nextCbvDescriptorNum = 0;
	nextSrvDescriptorNum = 0;
}

Dx12_CBVSRVUAVHeap::~Dx12_CBVSRVUAVHeap()
{
	COM_RELEASE(mHeap);
}

void Dx12_CBVSRVUAVHeap::CreateCBV(ID3D12Resource * cBuff)
{
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
	cbvDesc.BufferLocation = cBuff->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = (UINT)cBuff->GetDesc().Width;
	D3D12_CPU_DESCRIPTOR_HANDLE handle = mHeap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += nextCbvDescriptorNum * incrementSize;
	pDevice->CreateConstantBufferView(&cbvDesc, handle);
	++nextCbvDescriptorNum;
}

void Dx12_CBVSRVUAVHeap::CreateSRV(ID3D12Resource * texBuff)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handle = mHeap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += srvStartOffset * incrementSize + nextSrvDescriptorNum * incrementSize;
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = texBuff->GetDesc().Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	pDevice->CreateShaderResourceView(texBuff, &srvDesc, handle);
	++nextSrvDescriptorNum;
}

void Dx12_CBVSRVUAVHeap::Set()
{
	pCmdList->SetDescriptorHeaps(1, &mHeap);
}

ID3D12DescriptorHeap * Dx12_CBVSRVUAVHeap::Get()
{
	return mHeap;
}

D3D12_CPU_DESCRIPTOR_HANDLE Dx12_CBVSRVUAVHeap::GetCBVHandleForCPU(int number)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handle = mHeap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += number * incrementSize;
	return handle;
}

D3D12_GPU_DESCRIPTOR_HANDLE Dx12_CBVSRVUAVHeap::GetCBVHandleForGPU(int number)
{
	D3D12_GPU_DESCRIPTOR_HANDLE handle = mHeap->GetGPUDescriptorHandleForHeapStart();
	handle.ptr += number * incrementSize;
	return handle;
}

D3D12_CPU_DESCRIPTOR_HANDLE Dx12_CBVSRVUAVHeap::GetSRVHandleForCPU(int number)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handle = mHeap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += incrementSize * srvStartOffset;
	handle.ptr += incrementSize * number;
	return handle;;
}

D3D12_GPU_DESCRIPTOR_HANDLE Dx12_CBVSRVUAVHeap::GetSRVHandleForGPU(int number)
{
	D3D12_GPU_DESCRIPTOR_HANDLE handle = mHeap->GetGPUDescriptorHandleForHeapStart();
	handle.ptr += incrementSize * srvStartOffset;
	handle.ptr += incrementSize * number;
	return handle;
}

int Dx12_CBVSRVUAVHeap::GetNextCbvDescriptorNumber()
{
	return nextCbvDescriptorNum;
}

int Dx12_CBVSRVUAVHeap::GetNextSrvDescriptorNumber()
{
	return nextSrvDescriptorNum;
}
