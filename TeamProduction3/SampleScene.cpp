#include "SampleScene.h"

SampleScene::SampleScene()
	: camera(Camera()),perspective(new Dx12_CBuffer<Perspective>(device->GetDevice(),heap,0))
{
	camera.SetKeyboard(keyboard);
}

SampleScene::~SampleScene()
{
}

void SampleScene::LoadAsset()
{
}

void SampleScene::Initialize()
{
	nextSceneFlag = false;
	camera.Initialize();
}

void SampleScene::Update()
{
	camera.Update();
}

void SampleScene::DrawSprite()
{
}

void SampleScene::Draw()
{
}

SceneName SampleScene::GetNextSceneName()
{
	return SceneName();
}
