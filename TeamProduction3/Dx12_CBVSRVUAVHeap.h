#pragma once
#pragma once
#include<d3d12.h>
class Dx12_CBVSRVUAVHeap
{
private:
	ID3D12Device* pDevice;
	ID3D12GraphicsCommandList* pCmdList;
	ID3D12DescriptorHeap* mHeap;
	int srvStartOffset;
	int incrementSize;
	int nextCbvDescriptorNum;
	int nextSrvDescriptorNum;
public:
	Dx12_CBVSRVUAVHeap(ID3D12Device* device,ID3D12GraphicsCommandList* cmdList, int cbvNum, int srvNum, int uavNum = 0);
	~Dx12_CBVSRVUAVHeap();
	void CreateCBV(ID3D12Resource* cBuff);
	void CreateSRV(ID3D12Resource* texBuff);
	void Set();
	ID3D12DescriptorHeap* Get();
	D3D12_CPU_DESCRIPTOR_HANDLE GetCBVHandleForCPU(int number);
	D3D12_GPU_DESCRIPTOR_HANDLE GetCBVHandleForGPU(int number);
	D3D12_CPU_DESCRIPTOR_HANDLE GetSRVHandleForCPU(int number);
	D3D12_GPU_DESCRIPTOR_HANDLE GetSRVHandleForGPU(int number);
	int GetNextCbvDescriptorNumber();
	int GetNextSrvDescriptorNumber();
};

