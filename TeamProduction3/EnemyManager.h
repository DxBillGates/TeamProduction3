#pragma once
#include "NormalEnemy.h"
#include "Player.h"
#include "HitParticleManager.h"
#include "Sound.h"
class EnemyManager
{
private:
	std::vector<NormalEnemy*> enemyList;
	std::vector<HitParticleManager> hitParticleManager;
	int count;
	Player* pPlayer;

	Sound* hitSE;
	SoundData* hitSEData;
public:
	EnemyManager();
	~EnemyManager();
	void LoadAsset(ID3D12Device* device, Dx12_CBVSRVUAVHeap* heap, LoadContents* loader);
	void Initialize();
	void Update();
	void Draw(ID3D12GraphicsCommandList* cmdList);
	void SetTarget(Player* pp);
	std::vector<NormalEnemy*>* GetNormalEnemyList();
};
