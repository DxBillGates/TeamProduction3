#pragma once
#include<d3d12.h>
#include<vector>
enum RangeType
{
	CBV,
	SRV,
	UAV
};
class Dx12_RootSignature
{
private:
	ID3DBlob* rootBlob;
	ID3D12RootSignature* rootSignature;
public:
	Dx12_RootSignature(ID3D12Device* device, std::vector<RangeType> rangeTypes);
	~Dx12_RootSignature();
	ID3D12RootSignature* Get();
	void CreateRange(D3D12_DESCRIPTOR_RANGE& range,RangeType type,int count);
	void CreateSampler(D3D12_STATIC_SAMPLER_DESC& samplerDesc);
};

