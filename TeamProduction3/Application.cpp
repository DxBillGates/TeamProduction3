#include "Application.h"
#include "TitleScene.h"
#include "GamePlayScene.h"
#include "ResultScene.h"
#include "ScoreManager.h"
#include "Sound.h"
#include <stdio.h>
#pragma comment(lib,"winmm.lib")
Application::Application(const char * appName, int wndWidth, int wndHeight) :
	gameWnd(appName, wndWidth, wndHeight),
	device(gameWnd.GetHwnd(), gameWnd.GetWndWidth(), gameWnd.GetWndHeight()),
	cbvSrvHeap(device.GetDevice(), device.GetCmdList(), 2560, 128),
	keyboard(gameWnd.GetHwnd(), gameWnd.GetHinstance()),loader(&device,&cbvSrvHeap)
{
	gameWnd.PreviewWindow();
	QueryPerformanceFrequency(&timeFreq);
	QueryPerformanceCounter(&timeStart);

	ScoreManager::Create();

	//�V�[���̒ǉ��ƃZ�b�g
	Scene::SetStaticMenber(&gameWnd, &device, &cbvSrvHeap, &keyboard, &ctrler,&loader);
	sceneManager.Add(new TitleScene(), TITLE);
	sceneManager.Add(new GamePlayScene(), GAME_PLAY_SCENE);
	sceneManager.Add(new ResultScene(), RESULT);
	sceneManager.ChangeScene(GAME_PLAY_SCENE);

	Sound::SetAudioEngine();
}

Application::~Application()
{
	ScoreManager::Destroy();
}

void Application::LoadAsset()
{
	fillScreenTransition.LoadAsset(device.GetDevice(), &cbvSrvHeap, &loader);
	fourCornerBoxTransition.LoadAsset(device.GetDevice(), &cbvSrvHeap, &loader);
	sceneManager.LoadAsset();
	ScoreManager::GetInstance()->LoadAsset(device.GetDevice(), &cbvSrvHeap, &loader);
	ScoreManager::GetInstance()->StaticLoadAsset(device.GetDevice(), &cbvSrvHeap, &loader);
}

bool Application::Initialize()
{
	fillScreenTransition.Initialize();
	fourCornerBoxTransition.Initialize();
	//�V�[���̏�����
	sceneManager.Initialize();
	ScoreManager::GetInstance()->Initialize();
	return true;
}

bool Application::Run()
{
	//********************������Ȋ댯**********************//
	SoundData bgmData = SoundData("Resources/Music/daruma.wav");
	Sound bgm = Sound(&bgmData);

	LoadAsset();
	if (!Initialize())
	{
		return false;
	}
	while (true)
	{
		QueryPerformanceCounter(&timeEnd);
		frameTime = static_cast<double>(timeEnd.QuadPart - timeStart.QuadPart) / static_cast<double>(timeFreq.QuadPart);
		if (frameTime < FRAME_TIME)
		{
			//Sleep�̎��Ԃ��v�Z
			DWORD sleepTime = static_cast<DWORD>((FRAME_TIME - frameTime) * 1000);
			timeBeginPeriod(1);
			Sleep(sleepTime);
			timeEndPeriod(1);
			continue;
		}
		timeStart = timeEnd;
		fps = 1.0f / frameTime;
		//printf("%3.3f\n", fps);
		if (!Update())
		{
			break;
		}
		bgm.Start();
		Draw();
	}
	return true;
	//********************������Ȋ댯**********************//
}

bool Application::Update()
{
	//�C���v�b�g�f�o�C�X�̃A�b�v�f�[�g
	ctrler.Update();
	keyboard.Update();

	SceneTransition* pSceneTransition = &fillScreenTransition;
	//pSceneTransition = &fourCornerBoxTransition;
	//���݂̃V�[�����玟�̃V�[���ֈڂ�t���O������ꂽ�炻�̃V�[������
	//���̃V�[���̖��O���󂯎��V�[����J�ڂ��V�[��������������
	pSceneTransition->Update();
	if (pSceneTransition->GetEndFlag())
	{
		pSceneTransition->Initialize();
	}
	if (sceneManager.GetNowScene())
	{
		if (sceneManager.GetNowScene()->GetNextSceneFlag())
		{
			pSceneTransition->Play();
			if (pSceneTransition->GetReturnFlag())
			{
				sceneManager.ChangeScene(sceneManager.GetNowScene()->GetNextSceneName());
				sceneManager.Initialize();
			}
		}
	}

	//���݂̃V�[���̍X�V����
	sceneManager.Update();

	if (!gameWnd.ProcessMessage())return false;    //�E�B���h�E�̃v���Z�X���b�Z�[�W����
	if (keyboard.CheakHitKey(Key::ESCAPE))return false;    //ESCAPE�L�[�������ꂽ��I��
	if (ctrler.CheckHitKey(XinputPadKey::XINPUT_BACK))return false;    //Xinput�R���g���[���[��Back�L�[�������ꂽ��I��
	return true;
}

void Application::Draw()
{
	SceneTransition* pSceneTransition = &fillScreenTransition;
	//pSceneTransition = &fourCornerBoxTransition;
	//device.ClearScreen({ 0.2f,0.5f,1.0f,0 });
	device.ClearScreen({ 0.0f,0.0f,0.0f,0 });
	sceneManager.Draw();
	sceneManager.DrawSprite();
	pSceneTransition->Draw(device.GetCmdList());
	device.ScreecFlip();
}


