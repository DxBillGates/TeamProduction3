#pragma once
#include "Scene.h"
#include "Dx12_CBuffer.h"
#include "Dx12_Pipeline.h"
#include "Camera.h"
#include "ResultEffect.h"
#include"BackGround.h"

class ResultScene : public Scene
{
private:
	Dx12_CBuffer<Perspective>* perspective;
	Dx12_CBuffer<DirectX::XMMATRIX>* orthograph;
	Dx12_Pipeline* simpleShader;
	Dx12_Pipeline* animetionShader;
	Dx12_Pipeline* spriteShader;
	float time;
	Camera mainCamera;
	ResultEffect particle;
	BackGround bg;
public:
	ResultScene();
	~ResultScene();
	void LoadAsset();
	void Initialize();
	void Update();
	void DrawSprite();
	void Draw();
	SceneName GetNextSceneName();

	
};