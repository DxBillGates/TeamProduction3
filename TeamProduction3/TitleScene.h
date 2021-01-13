#pragma once
#include "Scene.h"
#include "Dx12_CBuffer.h"
#include "Dx12_Pipeline.h"
#include "Matrix4.h"

class TitleScene : public Scene
{
private:
	Dx12_CBuffer<Matrix4>* orthograph;
public:
	TitleScene();
	~TitleScene();
	void LoadAsset();
	void Initialize();
	void Update();
	void DrawSprite();
	void Draw();
	SceneName GetNextSceneName();
};
