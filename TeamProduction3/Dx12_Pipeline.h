#pragma once
#include"Dx12_RootSignature.h"
#include"Dx12_Shader.h"
#include<d3d12.h>
#include<vector>
enum BlendMode
{
	BLENDMODE_ADD,
	BLENDMODE_SUB,
	BLENDMODE_INV,
	BLENDMODE_ALPHA
};

enum InputLayout
{
	POSITION,
	TEXCOORD,
	NORMAL,
};

class Dx12_Pipeline
{
private:
	Dx12_RootSignature* rootSignature;
	Dx12_Shader* shader;
	ID3D12PipelineState* solidPso;
	ID3D12PipelineState* wirePso;
public:
	Dx12_Pipeline(ID3D12Device* device, Dx12_Shader* pShader, Dx12_RootSignature* pRootSignature, std::vector<InputLayout> inputLayout, BlendMode blendMode = BLENDMODE_ALPHA,D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,bool alphaWrite = true,bool depthWrite = true);
	~Dx12_Pipeline();
	void Set(ID3D12GraphicsCommandList* pCmdList,D3D12_FILL_MODE fillmode = D3D12_FILL_MODE_SOLID);
	void SetBlendMode(D3D12_RENDER_TARGET_BLEND_DESC * blendDesc, BlendMode mode);
};

