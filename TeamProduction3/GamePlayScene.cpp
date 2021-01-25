#include "GamePlayScene.h"

GamePlayScene::GamePlayScene()
	:mainCamera(Camera()), perspective(new Dx12_CBuffer<Perspective>(device->GetDevice(), heap, 0)),
	orthograph(new Dx12_CBuffer<DirectX::XMMATRIX>(device->GetDevice(), heap, 0))
{
	orthograph->Map(DirectX::XMMatrixOrthographicOffCenterLH(0, 1280, 720, 0, 0, 1));
	Player::SetInputDevice(keyboard, ctrler);
	scoreManager = ScoreManager::GetInstance();
}

GamePlayScene::~GamePlayScene()
{
	delete orthograph;
	delete perspective;
	delete simpleShader;
	delete animetionShader;
}

void GamePlayScene::LoadAsset()
{
	ID3D12Device* pDevice = device->GetDevice();
	simpleShader = new Dx12_Pipeline(pDevice, new Dx12_Shader(L"SimpleVS.hlsl", L"SimplePS.hlsl"), new Dx12_RootSignature(pDevice, { CBV,CBV,SRV }), { POSITION,TEXCOORD,NORMAL });
	animetionShader = new Dx12_Pipeline(pDevice, new Dx12_Shader(L"AnimetionVS.hlsl", L"AnimetionPS.hlsl"), new Dx12_RootSignature(pDevice, { CBV,CBV,SRV }), { POSITION,TEXCOORD, });
	player.LoadAsset(pDevice, heap, loader);
	ground.LoadAsset(pDevice, heap, loader);
	enemyManager.LoadAsset(pDevice, heap, loader);
	feManager.LoadAsset(pDevice, heap, loader);
	time.LoadAsset(pDevice, heap, loader);
	squareManager.LoadAsset(pDevice, heap, loader);
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
	time.Initialize();
	timeValue = 999;
	scoreManager->GetCurrentScore()->Initialize();
	squareManager.Initialize();
}

void GamePlayScene::Update()
{
	const float PLAYER_TO_FIREWALL_DIS = 32;     //プレイヤーと壁の判定距離
	const float PLAYER_TO_ENEMY_DIS = 16;		 //プレイヤーと敵の判定距離
	const float INCREASE_FIRE_VALUE = 0.1f;		 //プレイヤーの燃えてる値を増やす定数
	const float DECREASE_FIRE_VALUE = 0.2f;		 //プレイヤーの燃えてる値を減らす定数
	const float PLAYER_TO_PUDDLE_DIS = 128 * 256;//プレイヤーと水たまりの判定距離(蒸発距離)
	const float PUDDLE_TO_PLAYER_DIS = 128 * 32; //プレイヤーと水たまりの判定距離(ダメージ距離)
	const float DECREASE_PUDDLE_VALUE = 0.01f;	 //プレイヤーと水たまりが近い時の水たまりの減少値
	const float FIREWALL_TO_ENEMY_DIS = 16;		 //壁と敵の攻撃の判定距離

	Vector3 pos = player.GetPosition();

	if (!mainCamera.IsShake())
	{
#pragma region 当たり判定

		//テスト書き
		std::vector<NormalEnemy*>* neList = enemyManager.GetNormalEnemyList();
		std::vector<FieldEffectPuddle>* feList = feManager.GetPuddleList();
		std::vector<FieldEffectFireWall>* feFirewallList = feManager.GetFireWallList();
		for (auto& ne : *neList)
		{
			if (ne->GetEnemyBulletPointer()->GetIsUse())
				if (Vector3::Distance(ne->GetEnemyBulletPointer()->GetPosition(), player.GetPosition()) <= PLAYER_TO_ENEMY_DIS)
				{
					player.SetFireValue(player.GetFireValue() - DECREASE_FIRE_VALUE);
					ne->GetEnemyBulletPointer()->Initialize();
				}
			for (auto& fe : *feFirewallList)
			{
				if (!fe.GetLiveFlag())continue;
				if (Vector3::Distance(ne->GetEnemyBulletPointer()->GetPosition(), fe.GetPosition()) < FIREWALL_TO_ENEMY_DIS)
				{
					fe.Initialize();
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
			if (!fe.GetLiveFlag())continue;
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
		std::vector<std::vector<Square>>* squares = squareManager.GetSquares();
		{
			int px = (int)player.GetPosition().x, pz = (int)player.GetPosition().z;
			px /= 32;
			pz /= 32;
			if (px < (int)squares->size() && pz < (int)squares[0].size() && px >= 0 && pz >= 0)
			{
				(*squares)[px][pz].SetColor(Vector3(1, 0, 0));
			}
		}
		for (auto& ne : *neList)
		{
			int ex = (int)ne->GetPos().x, ez = (int)ne->GetPos().z;
			ex /= 32;
			ez /= 32;
			if (ex < (int)squares->size() && ez < (int)squares[0].size() && ex >= 0 && ez >= 0)
			{
				(*squares)[ex][ez].SetColor(Vector3(0, 0, 1));
			}
		}

		Vector3 c = squareManager.GetTilesInfomation();
		if (keyboard->KeyPressTrigger(Key::SPACE))
			printf("赤:%d,青:%d,白:%d\n", (int)c.x, (int)c.y, (int)c.z);

#pragma endregion
		//テスト書き
		feManager.Update();
		//フィールドエフェクト(炎)の生成
		if (player.GetIsMove())
		{
			if (player.GetRedValue() >= 1)
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

		scoreManager->GetCurrentScore()->SetPosition(Vector3());
		scoreManager->GetCurrentScore()->Update();

		time.SetSize(Vector3(32, 32, 32));
		time.SetPosition(Vector3(640, 0, 0));
		time.SetTime((int)timeValue);
		time.Update();
		timeValue -= 0.016f;
		if (timeValue < 0)
		{
			nextSceneFlag = true;
		}
	}

	squareManager.Update();

	Vector3 oldCameraPos = mainCamera.GetPosition();
	Vector3 oldTargetPos = mainCamera.GetTarget();
	mainCamera.Update(keyboard, ctrler, player.GetPosition());
	Vector3 newCameraPos = mainCamera.GetPosition();
	Vector3 newTargetPos = mainCamera.GetTarget();

	Vector3 v = Vector3::Lerp(oldCameraPos, newCameraPos, 0.05f);
	Vector3 v1 = Vector3::Lerp(oldTargetPos, newTargetPos, 0.05f);

	mainCamera.SetPosition(v);
	mainCamera.SetTarget(v1);

	perspective->Map({ mainCamera.GetViewMatrix(),mainCamera.GetProjectionMatrix(90,gameWnd->GetAspect()) });

	if (keyboard->KeyPressTrigger(Key::D1))nextSceneFlag = true;
}

void GamePlayScene::DrawSprite()
{
	animetionShader->Set(device->GetCmdList());
	orthograph->Set(device->GetCmdList());
	time.Draw(device->GetCmdList());
	scoreManager->GetCurrentScore()->Draw(device->GetCmdList());
}

void GamePlayScene::Draw()
{
	ID3D12GraphicsCommandList* pCmdList = device->GetCmdList();
	simpleShader->Set(pCmdList);
	perspective->Set(device->GetCmdList());
	squareManager.Draw(pCmdList);
	player.Draw(pCmdList, heap);
	enemyManager.Draw(pCmdList);
	feManager.Draw(pCmdList);
	device->ClearDepth();
	//ground.Draw(pCmdList, heap);
}

SceneName GamePlayScene::GetNextSceneName()
{
	return RESULT;
}
