#include "TitleScene.h"

TitleScene::TitleScene()
	:orthograph(new Dx12_CBuffer<Matrix4>(device->GetDevice(), heap, 0))
{
	//•½s“Š‰es—ñ‚Ì¶¬
	Matrix4 a =
	{
		gameWnd->GetWndWidth() / 2.0f,0,0,0,
		0,gameWnd->GetWndHeight() / 2.0f,0,0,
		0,0,1,0,
		-1,1,0,1
	};
	orthograph->Map(a);
}

TitleScene::~TitleScene()
{
	delete orthograph;
}

void TitleScene::LoadAsset()
{
}

void TitleScene::Initialize()
{
	nextSceneFlag = false;
	printf("Title\n");
}

void TitleScene::Update()
{
	if (keyboard->KeyPressTrigger(Key::SPACE) || ctrler->CheckHitKey(XinputPadKey::XINPUT_A))nextSceneFlag = true;
}

void TitleScene::DrawSprite()
{
}

void TitleScene::Draw()
{
}

SceneName TitleScene::GetNextSceneName()
{
	return GAME_PLAY_SCENE;
}
