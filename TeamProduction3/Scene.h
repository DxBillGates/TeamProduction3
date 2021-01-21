#pragma once
#include"Window.h"
#include"Dx12_Device.h"
#include"Dx12_CBVSRVUAVHeap.h"
#include"Keyboard.h"
#include"Xinput.h"
#include"LoadContents.h"

enum SceneName
{
	TITLE,
	GAME_PLAY_SCENE,
	RESULT
};

class Scene
{
protected:
	static Window* gameWnd;
	static Dx12_Device* device;
	static Dx12_CBVSRVUAVHeap* heap;
	static Keyboard* keyboard;
	static Xinput* ctrler;
	static LoadContents* loader;
protected:
	SceneName sceneName;
	bool nextSceneFlag;
public:
	Scene();
	virtual ~Scene();
	SceneName GetSceneName();
	void SetSceneName(SceneName sceneName);
	virtual void LoadAsset() = 0;
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void DrawSprite() = 0;
	virtual void Draw() = 0;
	bool GetNextSceneFlag();
	virtual SceneName GetNextSceneName() = 0;
	static void SetStaticMenber(Window* pWnd, Dx12_Device* pDevice, Dx12_CBVSRVUAVHeap* pHeap,Keyboard* pKeyboard, Xinput* pCtrler, LoadContents* pLoader);
};

