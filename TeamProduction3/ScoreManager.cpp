#include "ScoreManager.h"

ScoreManager* ScoreManager::instance = nullptr;

ScoreManager * ScoreManager::GetInstance()
{
	return instance;
}

void ScoreManager::Create()
{
	if (!instance)instance = new ScoreManager();
}

void ScoreManager::Destroy()
{
	if (instance)delete instance;
}

void ScoreManager::LoadAsset(ID3D12Device * device, Dx12_CBVSRVUAVHeap * heap, LoadContents * loader)
{
	for (auto itr = scores.begin(); itr != scores.end(); ++itr)
	{
		(*itr)->LoadAsset(device, heap, loader);
	}
	currentScore->LoadAsset(device, heap, loader);
}

void ScoreManager::StaticLoadAsset(ID3D12Device * device, Dx12_CBVSRVUAVHeap * heap, LoadContents * loader)
{
	Score::StaticLoadAsset(device, heap, loader);
}

void ScoreManager::Initialize()
{
	for (auto itr = scores.begin(); itr != scores.end(); ++itr)
	{
		(*itr)->Initialize();
	}
	currentScore->Initialize();
}

void ScoreManager::Update()
{
	int i = 0;
	for (auto itr = scores.begin(); itr != scores.end(); ++itr,++i)
	{
		(*itr)->SetPosition(Vector3(0,(float)(64 * i), 0));
		(*itr)->Update();
	}
}

void ScoreManager::UpdateScore()
{
	int i = 0;
	bool isFirstUpdate = false;
	int oldScore = 0;
	for (auto itr = scores.begin(); itr != scores.end(); ++itr)
	{
		if (!isFirstUpdate)
		{
			if ((*itr)->GetScore() < currentScore->GetScore())
			{
				oldScore = (*itr)->GetScore();
				(*itr)->SetScore(currentScore->GetScore());
				isFirstUpdate = true;
			}
		}
		else
		{
			int olddScore = (*itr)->GetScore();
			(*itr)->SetScore(oldScore);
			oldScore = olddScore;
		}
	}
}

void ScoreManager::Draw(ID3D12GraphicsCommandList * cmdList)
{
	for (auto itr = scores.begin(); itr != scores.end(); ++itr)
	{
		(*itr)->Draw(cmdList);
	}
}

Score * ScoreManager::GetCurrentScore()
{
	return currentScore;
}

ScoreManager::ScoreManager()
{
	const int MAX_SOCRES = 5;
	for (int i = 0; i < MAX_SOCRES; ++i)
	{
		scores.push_back(new Score());
	}
	currentScore = new Score();
}

ScoreManager::~ScoreManager()
{
	Score::DestroyAsset();
	for (auto itr = scores.begin(); itr != scores.end(); ++itr)
	{
		delete (*itr);
	}
	delete currentScore;
}
