#include "ResultScene.h"
#include "ScoreManager.h"
#include"ResultParticle.h"

ResultScene::ResultScene():
orthograph(new Dx12_CBuffer<DirectX::XMMATRIX>(device->GetDevice(), heap, 0))
{
	orthograph->Map(DirectX::XMMatrixOrthographicOffCenterLH(0, 1280, 720, 0, 0, 1));
}

ResultScene::~ResultScene()
{
	delete orthograph;
	delete animetionShader;
	delete spriteShader;
	ResultParticle::StaticDelete();
}

void ResultScene::LoadAsset()
{
	ID3D12Device* pDevice = device->GetDevice();
	animetionShader = new Dx12_Pipeline(device->GetDevice(), new Dx12_Shader(L"AnimetionVS.hlsl", L"AnimetionPS.hlsl"), new Dx12_RootSignature(device->GetDevice(), { CBV,CBV,SRV }), { POSITION,TEXCOORD, }, BLENDMODE_ALPHA, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, true, false);
	spriteShader = new Dx12_Pipeline(pDevice, new Dx12_Shader(L"SpriteVS.hlsl", L"SpritePS.hlsl"), new Dx12_RootSignature(pDevice, { CBV,CBV,SRV }), { POSITION,TEXCOORD }, BLENDMODE_ALPHA, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, true, false);
	ResultParticle::StaticLoadAsset(pDevice, heap, loader);
	particle.LoadAsset(pDevice, heap, loader);
}

void ResultScene::Initialize()
{
	ScoreManager::GetInstance()->UpdateScore();
	nextSceneFlag = false;
	particle.Initialize();
	if (ScoreManager::GetInstance()->GetFirstFlag())
	{
		particle.Emitte(Vector3(640, 360, 0));
	}
}

void ResultScene::Update()
{
	ScoreManager::GetInstance()->Update();
	if (keyboard->CheakHitKey(Key::SPACE))nextSceneFlag = true;

	particle.Update();
}

void ResultScene::DrawSprite()
{
	spriteShader->Set(device->GetCmdList());

	particle.Draw(device->GetCmdList());

	animetionShader->Set(device->GetCmdList());
	orthograph->Set(device->GetCmdList());
	ScoreManager::GetInstance()->Draw(device->GetCmdList());

}

void ResultScene::Draw()
{
}

SceneName ResultScene::GetNextSceneName()
{
	return GAME_PLAY_SCENE;
}
