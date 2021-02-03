#include "ResultScene.h"
#include "ScoreManager.h"
#include"ResultParticle.h"

ResultScene::ResultScene() :
	perspective(new Dx12_CBuffer<Perspective>(device->GetDevice(), heap, 0)),
	orthograph(new Dx12_CBuffer<DirectX::XMMATRIX>(device->GetDevice(), heap, 0))
{
	orthograph->Map(DirectX::XMMatrixOrthographicOffCenterLH(0, 1280, 720, 0, 0, 1));
}

ResultScene::~ResultScene()
{
	delete orthograph;
	delete perspective;
	delete simpleShader;
	delete animetionShader;
	delete spriteShader;
	ResultParticle::StaticDelete();
}

void ResultScene::LoadAsset()
{
	ID3D12Device* pDevice = device->GetDevice();
	simpleShader = new Dx12_Pipeline(pDevice, new Dx12_Shader(L"SimpleVS.hlsl", L"SimplePS.hlsl"), new Dx12_RootSignature(pDevice, { CBV,CBV,SRV }), { POSITION,TEXCOORD,NORMAL });
	animetionShader = new Dx12_Pipeline(device->GetDevice(), new Dx12_Shader(L"AnimetionVS.hlsl", L"AnimetionPS.hlsl"), new Dx12_RootSignature(device->GetDevice(), { CBV,CBV,SRV }), { POSITION,TEXCOORD, }, BLENDMODE_ALPHA, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, true, false);
	spriteShader = new Dx12_Pipeline(pDevice, new Dx12_Shader(L"SpriteVS.hlsl", L"SpritePS.hlsl"), new Dx12_RootSignature(pDevice, { CBV,CBV,SRV }), { POSITION,TEXCOORD }, BLENDMODE_ALPHA, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, true, false);
	ResultParticle::StaticLoadAsset(pDevice, heap, loader);
	particle.LoadAsset(pDevice, heap, loader);
	bg.LoadAsset(pDevice, heap, loader);
}

void ResultScene::Initialize()
{
	ScoreManager::GetInstance()->UpdateScore();
	nextSceneFlag = false;
	particle.Initialize();
	bg.Initialize();
	if (ScoreManager::GetInstance()->GetFirstFlag())
	{
		particle.Emitte(Vector3(640, 360, 0));
	}
	time = 25; //s
	bg.SetOBJPosition(Vector3(0,0,100), Vector3(0,3700,0), Vector3(100, 100, 100));
}

void ResultScene::Update()
{
	mainCamera.SetPosition(Vector3(0, 128, 1280));
	mainCamera.SetTarget(Vector3(0, 96, 0));

	ScoreManager::GetInstance()->Update();
	if (keyboard->CheakHitKeyAll() || ctrler->CheckHitKeyAll())nextSceneFlag = true;
	if (time <= 0)nextSceneFlag = true;
	particle.Update();
	time -= 0.016f;
	bg.Update();
	perspective->Map({ mainCamera.GetViewMatrix(),mainCamera.GetProjectionMatrix(90,gameWnd->GetAspect()) });
}

void ResultScene::DrawSprite()
{
	spriteShader->Set(device->GetCmdList());
	orthograph->Set(device->GetCmdList());
	particle.Draw(device->GetCmdList());

	animetionShader->Set(device->GetCmdList());
	orthograph->Set(device->GetCmdList());
	ScoreManager::GetInstance()->Draw(device->GetCmdList());

}

void ResultScene::Draw()
{
	simpleShader->Set(device->GetCmdList());
	perspective->Set(device->GetCmdList());
	bg.Draw(device->GetCmdList(),heap);
}

SceneName ResultScene::GetNextSceneName()
{
	return GAME_PLAY_SCENE;
}
