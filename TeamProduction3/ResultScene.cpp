#include "ResultScene.h"
#include "ScoreManager.h"

ResultScene::ResultScene():
orthograph(new Dx12_CBuffer<DirectX::XMMATRIX>(device->GetDevice(), heap, 0))
{
	orthograph->Map(DirectX::XMMatrixOrthographicOffCenterLH(0, 1280, 720, 0, 0, 1));
}

ResultScene::~ResultScene()
{
	delete orthograph;
	delete animetionShader;
}

void ResultScene::LoadAsset()
{
	animetionShader = new Dx12_Pipeline(device->GetDevice(), new Dx12_Shader(L"AnimetionVS.hlsl", L"AnimetionPS.hlsl"), new Dx12_RootSignature(device->GetDevice(), { CBV,CBV,SRV }), { POSITION,TEXCOORD, });
}

void ResultScene::Initialize()
{
	ScoreManager::GetInstance()->UpdateScore();
	nextSceneFlag = false;
}

void ResultScene::Update()
{
	ScoreManager::GetInstance()->Update();
	if (keyboard->CheakHitKey(Key::SPACE))nextSceneFlag = true;
}

void ResultScene::DrawSprite()
{
	animetionShader->Set(device->GetCmdList());
	orthograph->Set(device->GetCmdList());
	ScoreManager::GetInstance()->Draw(device->GetCmdList());
}

void ResultScene::Draw()
{
}

SceneName ResultScene::GetNextSceneName()
{
	return TITLE;
}
