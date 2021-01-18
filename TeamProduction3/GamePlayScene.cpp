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
	//�J�����̏�����
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
	const float PLAYER_TO_FIREWALL_DIS = 32;     //�v���C���[�ƕǂ̔��苗��
	const float PLAYER_TO_ENEMY_DIS = 16;		 //�v���C���[�ƓG�̔��苗��
	const float INCREASE_FIRE_VALUE = 0.1f;		 //�v���C���[�̔R���Ă�l�𑝂₷�萔
	const float DECREASE_FIRE_VALUE = 0.2f;		 //�v���C���[�̔R���Ă�l�����炷�萔
	const float PLAYER_TO_PUDDLE_DIS = 128 * 128;//�v���C���[�Ɛ����܂�̔��苗��(��������)
	const float PUDDLE_TO_PLAYER_DIS = 128 * 32; //�v���C���[�Ɛ����܂�̔��苗��(�_���[�W����)
	const float DECREASE_PUDDLE_VALUE = 0.01f;	 //�v���C���[�Ɛ����܂肪�߂����̐����܂�̌����l
	const float FIREWALL_TO_ENEMY_DIS = 16;		 //�ǂƓG�̍U���̔��苗��

	//�e�X�g����
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
	//�e�X�g����
	feManager.Update();
	//�t�B�[���h�G�t�F�N�g(��)�̐���
	if (player.GetIsMove())
	{
		feManager.CreateFireWall(player.GetOldPos());
	}

	//�n��
	ground.Update();
	//EnemyManager�Ƀv���C���[�̃|�W�V�������Z�b�g
	enemyManager.SetTarget(&player);
	enemyManager.Update();
	//�v���C���[�̈ړ��������J��������擾
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
