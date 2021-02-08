#include "GamePlayScene.h"

GamePlayScene::GamePlayScene()
	:mainCamera(Camera()), perspective(new Dx12_CBuffer<Perspective>(device->GetDevice(), heap, 0)),
	orthograph(new Dx12_CBuffer<DirectX::XMMATRIX>(device->GetDevice(), heap, 0))
{
	orthograph->Map(DirectX::XMMatrixOrthographicOffCenterLH(0, 1280, 720, 0, 0, 1));
	Player::SetInputDevice(keyboard, ctrler);
	scoreManager = ScoreManager::GetInstance();
	sceneState = SceneState::TITLE;
}

GamePlayScene::~GamePlayScene()
{
	delete orthograph;
	delete perspective;
	delete simpleShader;
	delete animetionShader;
	delete spriteShader;

	delete hitSE;
	delete hitSEData;
}

void GamePlayScene::LoadAsset()
{
	ID3D12Device* pDevice = device->GetDevice();
	PlayerParticle::StaticLoadAsset(pDevice, loader);
	HitParticle::StaticLoadAsset(pDevice, loader);
	FireParticle::StaticLoadAsset(pDevice, loader);
	simpleShader = new Dx12_Pipeline(pDevice, new Dx12_Shader(L"SimpleVS.hlsl", L"SimplePS.hlsl"), new Dx12_RootSignature(pDevice, { CBV,CBV,SRV }), { POSITION,TEXCOORD,NORMAL });
	animetionShader = new Dx12_Pipeline(pDevice, new Dx12_Shader(L"AnimetionVS.hlsl", L"AnimetionPS.hlsl"), new Dx12_RootSignature(pDevice, { CBV,CBV,SRV }), { POSITION,TEXCOORD, }, BLENDMODE_ALPHA, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, true, false);
	spriteShader = new Dx12_Pipeline(pDevice, new Dx12_Shader(L"SpriteVS.hlsl", L"SpritePS.hlsl"), new Dx12_RootSignature(pDevice, { CBV,CBV,SRV }), { POSITION,TEXCOORD }, BLENDMODE_ALPHA, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, true, false);
	player.LoadAsset(pDevice, heap, loader);
	ground.LoadAsset(pDevice, heap, loader);
	enemyManager.LoadAsset(pDevice, heap, loader);
	feManager.LoadAsset(pDevice, heap, loader);
	time.LoadAsset(pDevice, heap, loader);
	squareManager.LoadAsset(pDevice, heap, loader);
	title.LoadAsset(pDevice, heap, loader);
	tutorialEnemy.LoadAsset(pDevice, heap);
	keyOperation.LoadAsset(pDevice, heap, loader);
	operation.LoadAsset(pDevice, heap, loader);
	thermometer.LoadAsset(pDevice, heap, loader);
	playerParticleManager.LoadAsset(pDevice, heap, loader);
	Wall::StaticLoadAsset(pDevice, heap, loader);
	tutorialArrow.LoadAsset(pDevice, heap, loader);
	bg.LoadAsset(pDevice, heap, loader);
	tileInfo.LoadAsset(pDevice, heap, loader);
	//bg.TreeLoadAsset(pDevice, heap, loader);

	hitSEData = new SoundData("Resources/Music/hit.wav");
	hitSE = new Sound(hitSEData);
}

void GamePlayScene::Initialize()
{
	//カメラの初期化
	mainCamera.Initialize();
	mainCamera.SetPosition(Vector3(0, 100, -512));
	mainCamera.SetTarget(Vector3(0, 0, 0));

	player.Initialize();
	ground.Initialize();
	enemyManager.Initialize();

	nextSceneFlag = false;
	printf("GamePlay\n");
	feManager.Initialize();
	time.Initialize();
	timeValue = 60;
	scoreManager->GetCurrentScore()->Initialize();
	squareManager.Initialize();
	sceneState = SceneState::TITLE;
	title.Initialize();
	tutorialEnemy.Initialize();
	tutorialEnemy.SetPos(Vector3(640, 0, 0));
	tutorialArrow.Initialize();

	operation.Initialize();
	thermometer.Initialize();
	playerParticleManager.Initialize();
	bg.Initialize();
	//壁のサイズとポジションの設定
	worldSize = squareManager.GetSize();
	tileInfo.Initialize();
}

void GamePlayScene::Update()
{
	const float PLAYER_TO_FIREWALL_DIS = 64;     //プレイヤーと壁の判定距離
	const float PLAYER_TO_ENEMY_DIS = 64;		 //プレイヤーと弾の判定距離
	const float INCREASE_FIRE_VALUE = 0.01f;		 //プレイヤーの燃えてる値を増やす定数
	const float DECREASE_FIRE_VALUE = 1.0f;		 //プレイヤーの燃えてる値を減らす定数
	const float PLAYER_TO_PUDDLE_DIS = 256 * 180;//プレイヤーと水たまりの判定距離(蒸発距離)
	const float PUDDLE_TO_PLAYER_DIS = 256 * 64; //プレイヤーと水たまりの判定距離(ダメージ距離)
	const float DECREASE_PUDDLE_VALUE = 0.0255f;	 //プレイヤーと水たまりが近い時の水たまりの減少値
	const float FIREWALL_TO_ENEMY_DIS = 64;		 //壁と敵の攻撃の判定距離

	const float PLAYER_SIZE_MAGNI = 2;           //プレイヤーのサイズを実際のサイズより大きく判定させるための値(床との当たり判定用)


	if (sceneState == SceneState::TITLE)
	{
		title.Update(player.GetRedValue() * 2);
		player.SetForward(mainCamera.GetForward());
		player.Update();
		thermometer.Update(player.GetPosition() + Vector3(0, 64, 0), player.GetRedValue(), mainCamera.GetPosition());
		if (player.GetRedValue() >= 1)title.SetIsFada(true);
		if (title.GetEndIsFade())
		{
			sceneState = SceneState::TUTORIAL;
			tutorialEnemy.Revive(Vector3());
			tutorialEnemy.SetPos(Vector3(worldSize.x / 2, 0, 0));
		}
	}
	if (sceneState == SceneState::TUTORIAL)
	{
		player.SetForward(mainCamera.GetForward());
		player.Update();
		thermometer.Update(player.GetPosition() + Vector3(0, 64, 0), player.GetRedValue(), mainCamera.GetPosition());
		tutorialEnemy.SetTargetPos(player.GetOldPos());
		tutorialEnemy.SetMoveVector(player.GetOldPos() - tutorialEnemy.GetPos());
		tutorialEnemy.Update();
		if (Vector3::Distance(player.GetPosition(), tutorialEnemy.GetPos()) <= 64)
		{
			hitSE->Start();
			mainCamera.ScreenShake();
			tutorialEnemy.Initialize();
			sceneState = SceneState::PLAY;
			operation.SetIsFade(true);
		}

		if (player.GetRedValue() >= 1)
		{
			operation.TextureChange();
		}

		Vector3 tutorialPos = tutorialEnemy.GetPos() - player.GetPosition();
		Vector3 forward = tutorialPos.Normalize();
		float length = tutorialPos.Length();
		tutorialPos = tutorialPos.Normalize();
		tutorialPos = player.GetPosition() + tutorialPos * 100 + Vector3(0, 16, 0);
		tutorialArrow.Update(tutorialPos, forward);
	}
#pragma region カメラ処理
	Vector3 pos = player.GetPosition();

	Vector3 oldCameraPos = mainCamera.GetPosition();
	Vector3 oldTargetPos = mainCamera.GetTarget();
	mainCamera.Update(keyboard, ctrler, player.GetPosition());
	Vector3 newCameraPos = mainCamera.GetPosition();
	Vector3 newTargetPos = mainCamera.GetTarget();

	Vector3 v = Vector3::Lerp(oldCameraPos, newCameraPos, 0.05f);
	Vector3 v1 = Vector3::Lerp(oldTargetPos, newTargetPos, 0.05f);

	mainCamera.SetPosition(v);
	mainCamera.SetTarget(v1);
#pragma endregion

	if (!mainCamera.IsShake() && sceneState == SceneState::PLAY)
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
					player.SetIsDamage(true);
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
				feManager.CreatePuddle(ne->GetEnemyBulletPointer()->GetPosition() - Vector3(0, 24, 0));
			}
			bool a = false;
			ne->SetPos(CheckHitWall(ne->GetPos(), a));
			if (ne->GetEnemyBulletPointer()->GetIsUse())
			{
				CheckHitWall(ne->GetEnemyBulletPointer()->GetPosition(), a);
				if (a)
				{
					feManager.CreatePuddle(ne->GetEnemyBulletPointer()->GetPosition() - Vector3(0, 24, 0));
					ne->GetEnemyBulletPointer()->Initialize();
				}
			}

		}
		for (auto& fe : *feList)
		{
			if (!fe.GetLiveFlag())continue;
			if (Vector3::Distance(fe.GetPosition(), player.GetPosition()) * Vector3::Distance(fe.GetPosition(), player.GetPosition()) <= PLAYER_TO_PUDDLE_DIS)
			{
				if (player.GetFireValue() >= 0.8f)
				{
					fe.GetSE()->Start();
					fe.SetAlphaValue(fe.GetAlphaValue() - DECREASE_PUDDLE_VALUE);
				}
				if (Vector3::Distance(fe.GetPosition(), player.GetPosition()) * Vector3::Distance(fe.GetPosition(), player.GetPosition()) <= fe.GetAlphaValue() * PUDDLE_TO_PLAYER_DIS)
				{
					player.SetFireValue(player.GetFireValue() - DECREASE_FIRE_VALUE);
					player.SetIsDamage(true);
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
			//int px = (int)player.GetPosition().x, pz = (int)player.GetPosition().z;
			//px /= (int)Square::GetSize().x;
			//pz /= (int)Square::GetSize().x;
			//if (px < (int)squares->size() && pz < (int)squares[0].size() && px >= 0 && pz >= 0)
			//{
			//	(*squares)[px][pz].SetColor(Vector3((float)245 / 255, (float)27 / 255, (float)27 / 255));
			//}

			int width = 2, depth = 2;
			Vector3 size = player.GetSize()*PLAYER_SIZE_MAGNI;
			Vector3 s = size / (float)width;
			Vector3 p = player.GetPosition() - s;
			Vector3 pp = player.GetPosition();
			Vector3 cp;
			for (int i = 0; i < width; ++i)
			{
				for (int j = 0;j < depth; ++j)
				{
					cp.x = p.x + size.x * i;
					cp.z = p.z + size.z * j;
					cp /= (*squares)[0][0].GetSize().x;
					if ((int)cp.x < (int)squares->size() && (int)cp.z < (int)squares[0].size() && cp.x >= 0 && cp.z >= 0)
					{
						if ((*squares)[(int)cp.x][(int)cp.z].GetColor() == Vector3((float)245 / 255, (float)27 / 255, (float)27 / 255))
						{
							continue;
						}
						(*squares)[(int)cp.x][(int)cp.z].SetColor(Vector3((float)245 / 255, (float)27 / 255, (float)27 / 255));
					}

				}
			}
		}
		for (auto& ne : *neList)
		{
			if (!ne->GetLiveFlag())continue;
			int ex = (int)ne->GetPos().x, ez = (int)ne->GetPos().z;
			ex /= (int)Square::GetSize().x;
			ez /= (int)Square::GetSize().x;
			if (ex < (int)squares->size() && ez < (int)squares[0].size() && ex >= 0 && ez >= 0)
			{
				(*squares)[ex][ez].SetColor(Vector3(0.25f, 0.25f, 0.25f));
			}
		}

		for (auto& fe : *feList)
		{
			if (fe.GetLiveFlag())
			{
				int width = 3, depth = 3;
				Vector3 size = fe.GetSize();
				//Vector3 s = size / 2;
				Vector3 p = fe.GetPosition() - size;
				Vector3 cp;
				for (int i = 0; i < width; ++i)
				{
					for (int j = 0; j < depth; ++j)
					{
						cp.x = p.x + size.x * i;
						cp.z = p.z + size.z * j;
						cp /= (*squares)[0][0].GetSize().x;
						if ((int)cp.x < (int)squares->size() && (int)cp.z < (int)squares[0].size() && cp.x >= 0 && cp.z >= 0)
						{
							if ((*squares)[(int)cp.x][(int)cp.z].GetColor() == Vector3(0, 0, 1))
							{
								continue;
							}
							(*squares)[(int)cp.x][(int)cp.z].SetColor(Vector3(0.25f, 0.25f, 0.25f));
						}
					}
				}
			}
		}

		Vector3 c = squareManager.GetTilesInfomation();
		if (keyboard->KeyPressTrigger(Key::SPACE))
			printf("赤:%d,青:%d,白:%d\n", (int)c.x, (int)c.y, (int)c.z);

		//テスト書き
#pragma endregion
		//フィールドエフェクト(炎)の生成
		if (player.GetIsMove())
		{
			if (player.GetRedValue() >= 1)
				feManager.CreateFireWall(player.GetOldPos() - Vector3(0, 32, 0));
		}

		//地面
		ground.SetBillBoard(Matrix4::Inverse(mainCamera.GetViewMatrix()));
		//ground.SetPosition(player.GetPosition());
		//ground.SetPosition(mainCamera.GetPosition() + mainCamera.GetForward() * 10);
		ground.Update();
		//EnemyManagerにプレイヤーのポジションをセット
		enemyManager.SetTarget(&player);
		enemyManager.Update();
		//プレイヤーの移動方向をカメラから取得
		player.SetForward(mainCamera.GetForward());
		player.Update();
		thermometer.Update(player.GetPosition() + Vector3(0, 64, 0), player.GetRedValue(), mainCamera.GetPosition());

#pragma region 制限時間
		time.SetSize(Vector3(32, 32, 32));
		time.SetPosition(Vector3(608, 32, 0));
		time.SetTime((int)timeValue);
		time.Update();
		timeValue -= 0.016f;
		if (timeValue < 0)
		{
			nextSceneFlag = true;
		}
#pragma endregion
	}

	scoreManager->GetCurrentScore()->SetScore((int)squareManager.GetTilesInfomation().x);
	scoreManager->GetCurrentScore()->SetPosition(Vector3(64 + 16, 32, 0));
	scoreManager->GetCurrentScore()->Update();
	if (scoreManager->GetCurrentScore()->GetScore() % 10 == 0)
	{
		scoreManager->GetCurrentScore()->SetEffectFlag();
	}

	tileInfo.SetPosition(Vector3(32, 32, 0));
	tileInfo.Update();

	feManager.Update();
	squareManager.Update();
	playerParticleManager.SetPosition(player.GetPosition());
	playerParticleManager.Update(player.GetRedValue());
	{
		bool a = false;
		player.SetPosition(CheckHitWall(player.GetPosition(), a));
	}
	bg.Update();

	operation.Update();
	perspective->Map({ mainCamera.GetViewMatrix(),mainCamera.GetProjectionMatrix(90,gameWnd->GetAspect()) });

	if (keyboard->KeyPressTrigger(Key::D1))nextSceneFlag = true;
}

void GamePlayScene::DrawSprite()
{
	spriteShader->Set(device->GetCmdList());
	orthograph->Set(device->GetCmdList());
	if (sceneState == SceneState::TITLE)
		title.Draw(device->GetCmdList());

	keyOperation.Draw(device->GetCmdList());

	if (sceneState != SceneState::TITLE)
		operation.Draw(device->GetCmdList());

	if (sceneState == SceneState::PLAY)
		tileInfo.Draw(device->GetCmdList());

	if (sceneState == SceneState::PLAY)
	{
		animetionShader->Set(device->GetCmdList());
		orthograph->Set(device->GetCmdList());
		time.Draw(device->GetCmdList());
		scoreManager->GetCurrentScore()->Draw(device->GetCmdList());
	}

}

void GamePlayScene::Draw()
{
	ID3D12GraphicsCommandList* pCmdList = device->GetCmdList();
	simpleShader->Set(pCmdList);
	perspective->Set(device->GetCmdList());
	squareManager.Draw(pCmdList);
	feManager.Draw(pCmdList);
	player.Draw(pCmdList, heap);
	thermometer.Draw(pCmdList, heap);

	enemyManager.Draw(pCmdList);
	if (sceneState == SceneState::TUTORIAL)
	{
		tutorialEnemy.Draw(pCmdList);
		tutorialArrow.Draw(pCmdList);
	}
	playerParticleManager.Draw(pCmdList, heap);
	bg.Draw(pCmdList, heap);
	bg.TreeDraw(pCmdList, heap);

	//device->ClearDepth();
	//ground.Draw(pCmdList, heap);
}

SceneName GamePlayScene::GetNextSceneName()
{
	return RESULT;
}

Vector3 GamePlayScene::CheckHitWall(const Vector3 & p, bool& b)
{
	Vector3 resultPos = p;
	if (p.x <= 0)resultPos.x = 0;
	if (p.z <= 0)resultPos.z = 0;
	if (p.x >= worldSize.x - 1)resultPos.x = worldSize.x - 1;
	if (p.z >= worldSize.z - 1)resultPos.z = worldSize.z - 1;
	if (!(resultPos == p))b = true;
	return resultPos;
}
