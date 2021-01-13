#include "FourCornerBoxTransition.h"

FourCornerBoxTransition::FourCornerBoxTransition():sprite(Dx12_Sprite()),shader(nullptr),orthograph(nullptr)
{
	fourCornerBoxCB.resize(4);
}

FourCornerBoxTransition::~FourCornerBoxTransition()
{
	delete shader;
	delete orthograph;
	for (auto& f : fourCornerBoxCB)
	{
		delete f;
	}
}

void FourCornerBoxTransition::LoadAsset(ID3D12Device * device, Dx12_CBVSRVUAVHeap * heap, LoadContents * loader)
{
	shader = new Dx12_Pipeline(device, new Dx12_Shader(L"FillVS.hlsl", L"FillPS.hlsl"), new Dx12_RootSignature(device, { CBV,CBV }), { POSITION,TEXCOORD });
	for (auto& f : fourCornerBoxCB)
	{
		f = new Dx12_CBuffer<CBData>(device, heap, 1);
	}
	SpriteData sd;
	loader->CreateModelData_Plane(1280.f/7.f, 720.f/7.f, sd,true);
	sprite.Create(device, &sd);

	//ïΩçsìäâeçsóÒÇÃê∂ê¨
	orthograph = new Dx12_CBuffer<DirectX::XMMATRIX>(device, heap, 0);
	orthograph->Map(DirectX::XMMatrixOrthographicOffCenterLH(0, 1280, 720, 0, 0, 1));
}

void FourCornerBoxTransition::Initialize()
{
	t = 0;
	playFlag = false;
	returnFlag = false;
	endFlag = false;
	increaseFlag = false;
}

void FourCornerBoxTransition::Update()
{
	const float TRANSITION_TIME = 2;
	returnFlag = false;
	std::vector<Vector3> fourCorner;
	fourCorner.push_back(Vector3(0,0,0));
	fourCorner.push_back(Vector3(1280,0,0));
	fourCorner.push_back(Vector3(1280,720,0));
	fourCorner.push_back(Vector3(0,720,0));
	if (playFlag)
	{
		float easing = 0 * (1.0f - t * t) + 10 * t * t;;
		float dt = (0.016f / TRANSITION_TIME);
		t += (increaseFlag) ? dt : -dt;
		if (t >= 1.0f)
		{
			returnFlag = true;
			increaseFlag = false;
			t = 1.0f;
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
		for (int i = 0; i < 4; ++i)
		{
			fourCornerBoxCB[i]->Map({ Matrix4::Scale(Vector3(easing,easing,easing)) * Matrix4::Translate(fourCorner[i]),{0,0,0,1} });
		}
	}
}

void FourCornerBoxTransition::Draw(ID3D12GraphicsCommandList * cmdList)
{
	shader->Set(cmdList);
	orthograph->Set(cmdList);
	for (auto& f : fourCornerBoxCB)
	{
		f->Set(cmdList);
		sprite.Draw(cmdList);
	}
}
