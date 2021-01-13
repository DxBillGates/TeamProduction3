#pragma once
#include"Scene.h"
#include<vector>
class SceneManager
{
private:
	std::vector<Scene*> scenes;
	Scene* nowScene;
public:
	SceneManager();
	~SceneManager();
	void Add(Scene* newScene, SceneName sceneName);
	void ChangeScene(SceneName sceneName);
	Scene* GetNowScene();
	void LoadAsset();
	void Initialize();
	void Update();
	void DrawSprite();
	void Draw();
};

