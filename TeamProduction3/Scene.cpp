#include "Scene.h"

Window* Scene::gameWnd = nullptr;
Dx12_Device* Scene::device = nullptr;
Dx12_CBVSRVUAVHeap* Scene::heap = nullptr;
Keyboard* Scene::keyboard = nullptr;
Xinput* Scene::ctrler = nullptr;
LoadContents* Scene::loader = nullptr;

Scene::Scene()
{
}

Scene::~Scene()
{
}

SceneName Scene::GetSceneName()
{
	return sceneName;
}

void Scene::SetSceneName(SceneName sceneName)
{
	this->sceneName = sceneName;
}

bool Scene::GetNextSceneFlag()
{
	return nextSceneFlag;
}

void Scene::SetStaticMenber(Window * pWnd, Dx12_Device * pDevice, Dx12_CBVSRVUAVHeap* pHeap, Keyboard * pKeyboard, Xinput * pCtrler, LoadContents* pLoader)
{
	gameWnd = pWnd;
	device = pDevice;
	heap = pHeap;
	keyboard = pKeyboard;
	ctrler = pCtrler;
	loader = pLoader;
}
