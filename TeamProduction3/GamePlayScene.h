#pragma once
#include "Scene.h"
#include "Dx12_CBuffer.h"
#include "Dx12_Pipeline.h"
#include "Camera.h"
#include "Player.h"
#include "EnemyManager.h"
#include "Ground.h"
#include "FieldEffectPuddle.h"
#include "FieldEffectManager.h"
#include "Matrix4.h"
#include "Time.h"
#include "ScoreManager.h"
#include "SquareManager.h"
#include "Title.h"
#include "KeyOperation.h"
#include "Operation.h"
#include "Thermometer.h"
#include "PlayerParticleManager.h"
#include "Wall.h"
#include "Vector2.h"
#include "TutorialArrow.h"
#include"BackGround.h"
#include "TileInfo.h"


class GamePlayScene : public Scene
{
private:
	enum class SceneState
	{
		TITLE,TUTORIAL,PLAY,RESULT,
	};
private:
	SceneState sceneState;
	Camera mainCamera;
	Dx12_CBuffer<DirectX::XMMATRIX>* orthograph;
	Dx12_CBuffer<Perspective>* perspective;
	Dx12_Pipeline* animetionShader;
	Dx12_Pipeline* simpleShader;
	Dx12_Pipeline* spriteShader;
private:
	Player player;
	Thermometer thermometer;
	PlayerParticleManager playerParticleManager;

	Ground ground;
	EnemyManager enemyManager;
	FieldEffectManager feManager;
	ScoreManager* scoreManager;
	SquareManager squareManager;
	BackGround bg;
	NormalEnemy tutorialEnemy;
	TutorialArrow tutorialArrow;
	
	Title title;
	KeyOperation keyOperation;
	Operation operation;
	
	Vector3 worldSize;

	Time time;
	float timeValue;

	Sound* hitSE;
	SoundData* hitSEData;

	TileInfo tileInfo;
public:
	GamePlayScene();
	~GamePlayScene();
	void LoadAsset();
	void Initialize();
	void Update();
	void DrawSprite();
	void Draw();
	SceneName GetNextSceneName();
	Vector3 CheckHitWall(const Vector3& p,bool& b);
};
