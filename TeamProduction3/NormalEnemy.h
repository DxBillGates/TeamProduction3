#pragma once
#include "Enemy.h"
#include "EnemyBullet.h"

class NormalEnemy : public Enemy
{
private:
	static Dx12_Mesh mesh;
	static MeshData md;
	EnemyBullet bullet;
public:
	NormalEnemy();
	~NormalEnemy();
	void LoadAsset(ID3D12Device* device, Dx12_CBVSRVUAVHeap* heap);
	static void StaticLoadAsset(ID3D12Device* device, LoadContents* loader);
	void Initialize();
	void Update();
	void Draw(ID3D12GraphicsCommandList* cmdList);
	EnemyBullet* GetEnemyBulletPointer();
};
