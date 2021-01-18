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
	const float PLAYER_TO_FIREWALL_DIS = 32;     //プレイヤーと壁の判定距離
	const float PLAYER_TO_ENEMY_DIS = 16;		 //プレイヤーと敵の判定距離
	const float INCREASE_FIRE_VALUE = 0.1f;		 //プレイヤーの燃えてる値を増やす定数
	const float DECREASE_FIRE_VALUE = 0.2f;		 //プレイヤーの燃えてる値を減らす定数
	const float PLAYER_TO_PUDDLE_DIS = 128 * 128;//プレイヤーと水たまりの判定距離(蒸発距離)
	const float PUDDLE_TO_PLAYER_DIS = 128 * 32; //プレイヤーと水たまりの判定距離(ダメージ距離)
	const float DECREASE_PUDDLE_VALUE = 0.01f;	 //プレイヤーと水たまりが近い時の水たまりの減少値
	const float FIREWALL_TO_ENEMY_DIS = 16;		 //壁と敵の攻撃の判定距離

	//テスト書き
	std::vector<NormalEnemy*>* neList = enemyManager.GetNormalEnemyList();
	std::vector<FieldEffectPuddle>* feList = feManager.GetPuddleList();
	std::vector<FieldEffectFireWall>* feFirewallList = feManager.GetFireWallList();
	for (auto& ne : *neList)
	{
		if (Vector3::Distance(ne->GetEnemyBulletPointer()->GetPosition(), player.GetPosition()) <= PLAYER_TO_ENEMY_DIS)
		{
			player.SetFireValue(player.GetFireValue() - DECREASE_FIRE_VALUE);
			ne->GetEnemyBulletPointer()->Initialize();
		}
		for (auto& fe : *feFirewallList)
		{
			if (!fe.GetLiveFlag())continue;
			if (Vector3::Distance(ne->GetEnemyBulletPointer()->GetPosition(),fe.GetPosition()) < FIREWALL_TO_ENEMY_DIS)
			{
				ne->GetEnemyBulletPointer()->Initialize();
			}
		}
		if (ne->GetEnemyBulletPointer()->GetIsOldUse() && !ne->GetEnemyBulletPointer()->GetIsUse())
		{
			feManager.CreatePuddle(ne->GetEnemyBulletPointer()->GetLostPosition());
		}

	}
	for (auto& fe : *feList)
	{
		if (Vector3::Distance(fe.GetPosition(), player.GetPosition()) * Vector3::Distance(fe.GetPosition(), player.GetPosition()) <= PLAYER_TO_PUDDLE_DIS)
		{
			if (player.GetFireValue() >= 0.8f)
				fe.SetAlphaValue(fe.GetAlphaValue() - DECREASE_PUDDLE_VALUE);
			if (Vector3::Distance(fe.GetPosition(), player.GetPosition()) * Vector3::Distance(fe.GetPosition(), player.GetPosition()) <= fe.GetAlphaValue() * PUDDLE_TO_PLAYER_DIS)
			{
				player.SetFireValue(player.GetFireValue() - DECREASE_FIRE_VALUE);
			}
		}
	}
	for (auto& fe : *feFirewallList)
	{
		if (!fe.GetLiveFlag())continue;
		if (Vector3::Distance(fe.GetPosition(), player.GetPosition()) <= PLAYER_TO_FIREWALL_DIS)
		{
			player.SetFireValue(player.GetFireValue() + INCREASE_FIRE_VALUE);
		}
	}
	//テスト書き
	feManager.Update();
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
