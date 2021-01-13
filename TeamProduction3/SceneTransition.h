#pragma once
#include <d3d12.h>
class SceneTransition
{
protected:
	float t;
	bool playFlag;
	bool returnFlag;
	bool endFlag;
	bool increaseFlag;
public:
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw(ID3D12GraphicsCommandList* cmdList) = 0;
	void Play();
	float GetT();
	bool GetPlayFlag();
	bool GetReturnFlag();
	bool GetEndFlag();
	bool GetIncreaseFlag();
};
