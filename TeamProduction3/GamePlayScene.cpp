#include "GamePlayScene.h"

GamePlayScene::GamePlayScene()
	:mainCamera(Camera()), perspective(new Dx12_CBuffer<Perspective>(device->GetDevice(), heap, 0)),
	orthograph(new Dx12_CBuffer<DirectX::XMMATRIX>(device->GetDevice(), heap, 0))
{
	orthograph->Map(DirectX::XMMatrixOrthographicOffCenterLH(0, 1280, 720, 0, 0, 1));
	Player::SetInputDevice(keyboard, ctrler);
}

GamePlayScene::~GamePlayScene()
{
	delete orthograph;
	delete perspective;
	delete simpleShader;
}

void GamePlayScene::LoadAsset()
{
	ID3D12Device* pDevice = device->GetDevice();
	simpleShader = new Dx12_Pipeline(pDevice, new Dx12_Shader(L"SimpleVS.hlsl", L"SimplePS.hlsl"), new Dx12_RootSignature(pDevice, { CBV,CBV }), { POSITION,TEXCOORD,NORMAL });
	player.LoadAsset(pDevice, heap, loader);
	ground.LoadAsset(pDevice, heap, loader);
	enemyManager.LoadAsset(pDevice, heap, loader);
	feManager.LoadAsset(pDevice, heap, loader);
}

void GamePlayScene::Initialize()
{
	//カメラの初期化
	mainCamera.Initialize();
	mainCamera.SetPosition(Vector3(0, 128, -512));
	mainCamera.SetTarget(Vector3(0, 0, 0));

	player.Initialize();
	ground.Initialize();
	enemyManager.Initialize();

	nextSceneFlag = false;
	printf("GamePlay\n");
	feManager.Initialize();
}

void GamePlayScene::Update()
{
	//テスト書き
	std::vector<NormalEnemy*>* neList = enemyManager.GetNormalEnemyList();
	std::vector<FieldEffectPuddle>* feList = feManager.GetPuddleList();
	for (auto& ne : *neList)
	{
		if (Vector3::Distance(ne->GetEnemyBulletPointer()->GetPosition(), player.GetPosition()) <= 16)
		{
			player.SetFireValue(player.GetFireValue() - 0.2f);
		}
		if (ne->GetEnemyBulletPointer()->GetIsOldUse() && !ne->GetEnemyBulletPointer()->GetIsUse())
		{
			feManager.CreatePuddle(ne->GetEnemyBulletPointer()->GetLostPosition());
		}
	}
	for (auto& fe : *feList)
	{
		if (Vector3::Distance(fe.GetPosition(), player.GetPosition()) * Vector3::Distance(fe.GetPosition(), player.GetPosition()) <= 128 * 128)
		{
			if (player.GetFireValue() >= 0.8f)
				fe.SetAlphaValue(fe.GetAlphaValue() - 0.01f);
			if (Vector3::Distance(fe.GetPosition(), player.GetPosition()) * Vector3::Distance(fe.GetPosition(), player.GetPosition()) <= fe.GetAlphaValue() * 128 * 32)
			{
				player.SetFireValue(player.GetFireValue() - 0.2f);
			}
		}
	}
	feManager.Update();
	//テスト書き
	//フィールドエフェクト(炎)の生成
	if (player.GetIsMove())
	{
		feManager.CreateFireWall(player.GetOldPos());
	}

	//地面
	ground.Update();
	//EnemyManagerにプレイヤーのポジションをセット
	enemyManager.SetTarget(&player);
	enemyManager.Update();
	//プレイヤーの移動方向をカメラから取得
	player.SetForward(mainCamera.GetForward());
	player.Update();

	mainCamera.Update(keyboard, ctrler, player.GetPosition());
	perspective->Map({ mainCamera.GetViewMatrix(),mainCamera.GetProjectionMatrix(90,gameWnd->GetAspect()) });

	if (keyboard->KeyPressTrigger(Key::D1))nextSceneFlag = true;
}

void GamePlayScene::DrawSprite()
{
	//orthograph->Set(device->GetCmdList());
}

void GamePlayScene::Draw()
{
	ID3D12GraphicsCommandList* pCmdList = device->GetCmdList();
	simpleShader->Set(pCmdList);
	perspective->Set(device->GetCmdList());
	player.Draw(pCmdList, heap);
	ground.Draw(pCmdList, heap);
	enemyManager.Draw(pCmdList);
	feManager.Draw(pCmdList);
}

SceneName GamePlayScene::GetNextSceneName()
{
	return TITLE;
}
