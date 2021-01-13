#pragma once
#include "FieldEffect.h"
#include "FieldEffectPuddle.h"
#include "FieldEffectFireWall.h"

class FieldEffectManager
{
private:
	enum class FieldEffects
	{
		PUDDLE,FIRE_WALL,
	};
private:
	std::vector<FieldEffectPuddle> fePuddleList;
	std::vector<FieldEffectFireWall> feFireWallList;
public:
	FieldEffectManager();
	~FieldEffectManager();
	void LoadAsset(ID3D12Device* device, Dx12_CBVSRVUAVHeap* heap, LoadContents* loader);
	void Initialize();
	void Update();
	void Draw(ID3D12GraphicsCommandList* cmdList);
	void CreatePuddle(const Vector3& pos);
	void CreateFireWall(const Vector3& pos);
	std::vector<FieldEffectPuddle>* GetPuddleList();
	std::vector<FieldEffectFireWall>* GetFireWallList();
};