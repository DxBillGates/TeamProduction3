#include "FillScreenTransition.h"

FillScreenTransition::FillScreenTransition():alpha(0)
{
}

FillScreenTransition::~FillScreenTransition()
{
	delete shader;
	delete cb;
	delete orthograph;
}

void FillScreenTransition::LoadAsset(ID3D12Device * device, Dx12_CBVSRVUAVHeap * heap, LoadContents* loader)
{
	shader = new Dx12_Pipeline(device, new Dx12_Shader(L"FillVS.hlsl", L"FillPS.hlsl"), new Dx12_RootSignature(device, { CBV,CBV }), {POSITION,TEXCOORD},BLENDMODE_ALPHA,D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,true,false);
	cb = new Dx12_CBuffer<CBData>(device, heap, 1);
	SpriteData sd;
	loader->CreateModelData_Plane(1280, 720, sd);
	sprite.Create(device,&sd);

	orthograph = new Dx12_CBuffer<DirectX::XMMATRIX>(device, heap, 0);
	//•½s“Š‰es—ñ‚Ì¶¬
	orthograph->Map(DirectX::XMMatrixOrthographicOffCenterLH(0, 1280, 720, 0, 0, 1));
}

void FillScreenTransition::Initialize()
{
	t = 0;
	playFlag = false;
	returnFlag = false;
	endFlag = false;
	increaseFlag = false;
	alpha = 0;
}

void FillScreenTransition::Update()
{
	const float TRANSITION_TIME = 1;
	returnFlag = false;

	if (playFlag)
	{
		float easing = 0 * (1.0f - t * t) + 1 * t * t;;
		float dt = (0.016f / TRANSITION_TIME);
		t += (increaseFlag) ? dt : -dt;
		if (t >= 1.2f)
		{
			returnFlag = true;
			increaseFlag = false;
			t = 1.2f;
		}

		if (!increaseFlag)
		{ 
			if (t <= 0)
			{
				t = 0;
				endFlag = true;
				playFlag = false;
				returnFlag = false;
			}
		}
		cb->Map({ Matrix4::Identity(),{0,0,0,t} });
	}
}

void FillScreenTransition::Draw(ID3D12GraphicsCommandList* cmdList)
{
	if (playFlag)
	{
		shader->Set(cmdList);
		orthograph->Set(cmdList);
		cb->Set(cmdList);
		sprite.Draw(cmdList);
	}
}
