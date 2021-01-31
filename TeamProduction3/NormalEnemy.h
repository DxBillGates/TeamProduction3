#pragma once
#include "Enemy.h"
#include "EnemyBullet.h"
#include "Sound.h"

class NormalEnemy : public Enemy
{
private:
	static Dx12_Mesh mesh;
	static MeshData md;
	static Dx12_Mesh coolTimeMesh;
	static MeshData coolTimeMd;
	Dx12_CBuffer<CBData>*coolTimeCB;
	EnemyBullet bullet;
	float coolTime;
	Vector3 moveVector;

	Sound* shotSE;
	SoundData* shotSEData;
public:
	NormalEnemy();
	~NormalEnemy();
	void LoadAsset(ID3D12Device* device, Dx12_CBVSRVUAVHeap* heap);
	static void StaticLoadAsset(ID3D12Device* device, LoadContents* loader);
	void Initialize();
	void Update();
	void Draw(ID3D12GraphicsCommandList* cmdList);
	EnemyBullet* GetEnemyBulletPointer();
	void SetMoveVector(const Vector3& v);
	float GetCoolTime();
};
