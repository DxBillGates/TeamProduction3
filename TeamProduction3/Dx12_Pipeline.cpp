#include "Dx12_Pipeline.h"
#include "COMRelease.h"

Dx12_Pipeline::Dx12_Pipeline(ID3D12Device * device, Dx12_Shader * pShader, Dx12_RootSignature * pRootSignature, std::vector<InputLayout> inputLayout, BlendMode blendMode, D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType, bool alphaWrite, bool depthWrite)
	:shader(pShader), rootSignature(pRootSignature)
{
	D3D12_INPUT_ELEMENT_DESC* inputDesc = new D3D12_INPUT_ELEMENT_DESC[(int)inputLayout.size()];
	//引数のインプットレイアウトからPSOのインプットレイアウトを設定
	for (int i = 0; i < (int)inputLayout.size(); ++i)
	{
		if (inputLayout[i] == POSITION)
		{
			inputDesc[i] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
		}
		else if (inputLayout[i] == TEXCOORD)
		{
			inputDesc[i] = { "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
		}
		else if (inputLayout[i] == NORMAL)
		{
			inputDesc[i] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
		}
	}
	//ブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blendDesc = {};
	SetBlendMode(&blendDesc, BLENDMODE_ALPHA);
	//PSOの各種設定
	HRESULT result;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.VS.BytecodeLength = shader->GetVsBlob()->GetBufferSize();
	psoDesc.VS.pShaderBytecode = shader->GetVsBlob()->GetBufferPointer();
	psoDesc.PS.BytecodeLength = shader->GetPsBlob()->GetBufferSize();
	psoDesc.PS.pShaderBytecode = shader->GetPsBlob()->GetBufferPointer();
	if (shader->GetGsBlob())
	{
		psoDesc.GS.BytecodeLength = shader->GetGsBlob()->GetBufferSize();
		psoDesc.GS.pShaderBytecode = shader->GetGsBlob()->GetBufferPointer();
	}
	psoDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
	psoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	psoDesc.RasterizerState.DepthClipEnable = true;
	psoDesc.DepthStencilState.DepthEnable = true;
	if (depthWrite)
	{
		psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	}
	else
	{
		psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	}
	psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	psoDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	if (alphaWrite)
	{
		psoDesc.BlendState.AlphaToCoverageEnable = false;
	}
	else
	{
		psoDesc.BlendState.AlphaToCoverageEnable = true;
	}

	psoDesc.BlendState.RenderTarget[0] = blendDesc;
	psoDesc.InputLayout.pInputElementDescs = inputDesc;
	psoDesc.InputLayout.NumElements = (int)inputLayout.size();
	psoDesc.PrimitiveTopologyType = topologyType;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	psoDesc.SampleDesc.Count = 1;
	psoDesc.pRootSignature = rootSignature->Get();
	result = device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&solidPso));
	psoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	result = device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&wirePso));

	delete[] inputDesc;
}

Dx12_Pipeline::~Dx12_Pipeline()
{
	delete rootSignature;
	delete shader;
	COM_RELEASE(solidPso);
	COM_RELEASE(wirePso);
}

void Dx12_Pipeline::Set(ID3D12GraphicsCommandList * pCmdList, D3D12_FILL_MODE fillmode)
{
	//引数のFILLMODEによってワイヤーフレームか塗りつぶしか分岐
	if (fillmode == D3D12_FILL_MODE_SOLID)
	{
		pCmdList->SetPipelineState(solidPso);
	}
	else
	{
		pCmdList->SetPipelineState(wirePso);
	}
	//ルートシグネチャのセット
	pCmdList->SetGraphicsRootSignature(rootSignature->Get());
}

void Dx12_Pipeline::SetBlendMode(D3D12_RENDER_TARGET_BLEND_DESC * blendDesc, BlendMode mode)
{
	(*blendDesc).RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	(*blendDesc).BlendEnable = true;
	(*blendDesc).BlendOpAlpha = D3D12_BLEND_OP_ADD;
	(*blendDesc).SrcBlendAlpha = D3D12_BLEND_ONE;
	(*blendDesc).DestBlendAlpha = D3D12_BLEND_ZERO;
	switch (mode)
	{
	case BLENDMODE_ADD:
		/* 加算合成 */
		(*blendDesc).BlendOp = D3D12_BLEND_OP_ADD;
		(*blendDesc).SrcBlend = D3D12_BLEND_ONE;
		(*blendDesc).DestBlend = D3D12_BLEND_ONE;
		break;
	case BLENDMODE_SUB:
		/* 減算合成 */
		(*blendDesc).BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
		(*blendDesc).SrcBlend = D3D12_BLEND_ONE;
		(*blendDesc).DestBlend = D3D12_BLEND_ONE;
		break;
	case BLENDMODE_INV:
		/* 色反転 */
		(*blendDesc).BlendOp = D3D12_BLEND_OP_ADD;
		(*blendDesc).SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
		(*blendDesc).DestBlend = D3D12_BLEND_ZERO;
		break;
	case BLENDMODE_ALPHA:
		/* 半透明 */
		(*blendDesc).BlendOp = D3D12_BLEND_OP_ADD;
		(*blendDesc).SrcBlend = D3D12_BLEND_SRC_ALPHA;
		(*blendDesc).DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		break;
	}
}
