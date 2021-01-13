#include "Dx12_RootSignature.h"
#include "COMRelease.h"
Dx12_RootSignature::Dx12_RootSignature(ID3D12Device * device, std::vector<RangeType> rangeTypes)
{
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
	//コンストラクタで指定されたレンジの数だけルートパラメータ,レンジを生成
	D3D12_ROOT_PARAMETER* rootParam = new D3D12_ROOT_PARAMETER[(int)rangeTypes.size()];
	D3D12_DESCRIPTOR_RANGE* range = new D3D12_DESCRIPTOR_RANGE[(int)rangeTypes.size()];
	//SRVの数だけSamplerDescを生成
	int samplerCount = 0;
	for (int i = 0; i < (int)rangeTypes.size(); ++i)
	{
		if (rangeTypes[i] == SRV)
		{
			++samplerCount;
		}
	}
	D3D12_STATIC_SAMPLER_DESC* samplerDesc = new D3D12_STATIC_SAMPLER_DESC[samplerCount];
	//指定された順に設定
	int cbvCount = 0;
	samplerCount = 0;
	for (int i = 0; i < (int)rangeTypes.size(); ++i)
	{
		switch (rangeTypes[i])
		{
		case CBV:
			CreateRange(range[i], CBV, cbvCount);
			rootParam[i].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			rootParam[i].DescriptorTable.pDescriptorRanges = &range[i];
			rootParam[i].DescriptorTable.NumDescriptorRanges = 1;
			rootParam[i].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
			++cbvCount;
			break;
		case SRV:
			CreateRange(range[i], SRV, samplerCount);
			rootParam[i].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			rootParam[i].DescriptorTable.pDescriptorRanges = &range[i];
			rootParam[i].DescriptorTable.NumDescriptorRanges = 1;
			rootParam[i].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
			CreateSampler(samplerDesc[samplerCount]);
			++samplerCount;
			break;
		}
	}
	rootSignatureDesc.pStaticSamplers = samplerDesc;
	rootSignatureDesc.NumStaticSamplers = samplerCount;
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParam;
	rootSignatureDesc.NumParameters = (int)rangeTypes.size();

	ID3DBlob* errorBlob = nullptr;
	HRESULT result;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootBlob, &errorBlob);
	result = device->CreateRootSignature(0, rootBlob->GetBufferPointer(), rootBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	delete[] rootParam;
	delete[] range;
	delete[] samplerDesc;
}

Dx12_RootSignature::~Dx12_RootSignature()
{
	COM_RELEASE(rootSignature);
	COM_RELEASE(rootBlob);
}

ID3D12RootSignature * Dx12_RootSignature::Get()
{
	return rootSignature;
}

void Dx12_RootSignature::CreateRange(D3D12_DESCRIPTOR_RANGE& range, RangeType type, int count)
{
	switch (type)
	{
	case CBV:
		range = {};
		range.NumDescriptors = 1;
		range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		range.BaseShaderRegister = count;
		range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		break;
	case SRV:
	    range = {};
		range.NumDescriptors = 1;
		range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		range.BaseShaderRegister = count;
		range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		break;
	}
}

void Dx12_RootSignature::CreateSampler(D3D12_STATIC_SAMPLER_DESC& samplerDesc)
{
	samplerDesc = {};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	samplerDesc.Filter = D3D12_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
}
