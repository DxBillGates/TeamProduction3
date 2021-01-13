#include "Application.h"
#include "SampleScene.h"
#include <stdio.h>
#pragma comment(lib,"winmm.lib")
Application::Application(const char * appName, int wndWidth, int wndHeight) :
	gameWnd(appName, wndWidth, wndHeight),
	device(gameWnd.GetHwnd(), gameWnd.GetWndWidth(), gameWnd.GetWndHeight()),
	cbvSrvHeap(device.GetDevice(), device.GetCmdList(), 1280, 128),
	keyboard(gameWnd.GetHwnd(), gameWnd.GetHinstance()),loader(&device,&cbvSrvHeap)
{
	gameWnd.PreviewWindow();
	QueryPerformanceFrequency(&timeFreq);
	QueryPerformanceCounter(&timeStart);

	//シーンの追加とセット
	Scene::SetStaticMenber(&gameWnd, &device, &cbvSrvHeap, &keyboard, &ctrler,&loader);
	sceneManager.Add(new SampleScene(), SAMPLE);
	sceneManager.ChangeScene(SAMPLE);
}

Application::~Application()
{
}

void Application::LoadAsset()
{
	sceneManager.LoadAsset();
}

bool Application::Initialize()
{
	//シーンの初期化
	sceneManager.Initialize();
	return true;
}

bool Application::Run()
{
	//********************いじるな危険**********************//
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
			//Sleepの時間を計算
			DWORD sleepTime = static_cast<DWORD>((FRAME_TIME - frameTime) * 1000);
			timeBeginPeriod(1);
			Sleep(sleepTime);
			timeEndPeriod(1);
			continue;
		}
		timeStart = timeEnd;
		fps = 1.0f / frameTime;
		printf("%3.3f\n", fps);
		if (!Update())
		{
			break;
		}
		Draw();
	}
	return true;
	//********************いじるな危険**********************//
}

bool Application::Update()
{
	//インプットデバイスのアップデート
	ctrler.Update();
	keyboard.Update();

	//現在のシーンから次のシーンへ移るフラグが送られたらそのシーンから
	//次のシーンの名前を受け取りシーンを遷移しシーンを初期化する
	if (sceneManager.GetNowScene())
	{
		if (sceneManager.GetNowScene()->GetNextSceneFlag())
		{
			sceneManager.ChangeScene(sceneManager.GetNowScene()->GetNextSceneName());
			sceneManager.Initialize();
		}
	}
	//現在のシーンの更新処理
	sceneManager.Update();

	if (!gameWnd.ProcessMessage())return false;    //ウィンドウのプロセスメッセージ処理
	if (keyboard.CheakHitKey(Key::ESCAPE))return false;    //ESCAPEキーが押されたら終了
	if (ctrler.CheckHitKey(XinputPadKey::XINPUT_BACK))return false;    //XinputコントローラーのBackキーが押されたら終了
	return true;
}

void Application::Draw()
{
	device.ClearScreen({ 0.2f,0.5f,1.0f,0 });
	sceneManager.Draw();
	sceneManager.DrawSprite();
	device.ScreecFlip();
}


