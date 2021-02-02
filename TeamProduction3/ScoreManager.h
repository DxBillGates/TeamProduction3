#pragma once
#include "Score.h"
#include <vector>

class Score;
class ScoreManager
{
private:

	static ScoreManager* instance;
	std::vector<Score*> scores;
	Score* myScore;
	Score* currentScore;
	bool isUpdateScore;
	int updateIndex;
	bool firstFlag;
	float a, b;
public:
	static ScoreManager* GetInstance();
	static void Create();
	static void Destroy();
	void LoadAsset(ID3D12Device* device, Dx12_CBVSRVUAVHeap* heap, LoadContents* loader);
	static void StaticLoadAsset(ID3D12Device* device, Dx12_CBVSRVUAVHeap* heap, LoadContents* loader);
	void Initialize();
	void Update();
	void UpdateScore();
	void Draw(ID3D12GraphicsCommandList* cmdList);
	bool GetFirstFlag()
	{
		return firstFlag;
	}
	Score* GetCurrentScore();

protected:
	ScoreManager();
	virtual ~ScoreManager();
	ScoreManager(const ScoreManager&) = delete;
	ScoreManager& operator=(const ScoreManager&) = delete;
};

