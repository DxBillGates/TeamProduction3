#pragma once
#include "BaseApplication.h"
#include "Window.h"
#include "Dx12_Device.h"
#include "Dx12_CBVSRVUAVHeap.h"
#include "Keyboard.h"
#include "Xinput.h"
#include "LoadContents.h"
#include "SceneManager.h"
#include "FillScreenTransition.h"
#include "FourCornerBoxTransition.h"

class Application : public BaseApplication
{
private:
	Window gameWnd;
	Dx12_Device device;
	Dx12_CBVSRVUAVHeap cbvSrvHeap;
	Keyboard keyboard;
	Xinput ctrler;
	LoadContents loader;
private:
	SceneManager sceneManager;
private:
	double fps = 0;
	const float FRAME_TIME = 1.0f / 60;
	double frameTime = 0;
	LARGE_INTEGER timeStart;
	LARGE_INTEGER timeEnd;
	LARGE_INTEGER timeFreq;
private:
	FillScreenTransition fillScreenTransition;
	FourCornerBoxTransition fourCornerBoxTransition;
public:
	Application(const char* appName = "‚¾‚é‚Ü“]‚ª‚µ", int wndWidth = 1280, int wndHeight = 720);
	~Application();
	void LoadAsset();
	bool Initialize();
	bool Run();
	bool Update();
	void Draw();
};

