#pragma once
#include "Scene.h"
#include "Camera.h"
#include "Dx12_CBuffer.h"

class SampleScene : public Scene
{
private:
	Camera camera;
	Dx12_CBuffer<Perspective>* perspective;
public:
	SampleScene();
	~SampleScene();
	void LoadAsset();
	void Initialize();
	void Update();
	void DrawSprite();
	void Draw();
	SceneName GetNextSceneName();
};

