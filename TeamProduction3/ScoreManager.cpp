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

	myScore->LoadAsset(device, heap, loader);

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
	myScore->Initialize();
	a = 0;
	b = 1;
	firstFlag = false;
}

void ScoreManager::Update()
{
	const Vector3 displayPos = { 64,116,0 };

	int i = 0;
	for (auto itr = scores.begin(); itr != scores.end(); ++itr, ++i)
	{

		(*itr)->SetPosition(displayPos+Vector3(0, (float)(64 * i), 0));
		(*itr)->Update();
	}
	//Ž©•ª‚ÌƒXƒP[ƒ‹‚Ì•Ï“®
	b = 1 + sin(a)*0.05f;
	scores[updateIndex]->SetScale(Vector3(b, b, 1));
	a += 0.1f;


	scores[updateIndex]->SetPosition(displayPos+Vector3(50, (float)(64 * updateIndex), 0));
	scores[updateIndex]->Update();
	myScore->SetPosition(Vector3(600, 520, 0));
	myScore->SetScale(Vector3(4, 4, 4));
	myScore->SetColor(Vector3(0.2f, 0.2f, 1));
	myScore->Update();
}

void ScoreManager::UpdateScore()
{
	int i = 0;
	bool isFirstUpdate = false;
	int oldScore = 0;
	for (auto itr = scores.begin(); itr != scores.end(); ++itr, ++i)
	{
		if (!isFirstUpdate)
		{
			if ((*itr)->GetScore() <= currentScore->GetScore())
			{
				oldScore = (*itr)->GetScore();
				(*itr)->SetScore(currentScore->GetScore());
				updateIndex = i;
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
	myScore->SetScore(currentScore->GetScore());

	int firstFlagCount = 0;
	for (int j = 0; j < scores.size(); j++)
	{
		if (myScore->GetScore() < scores[j]->GetScore())
		{
			firstFlagCount++;
		}
	}
	if (firstFlagCount == 0)
	{
		firstFlag = true;
	}
	else
	{
		firstFlag = false;
	}

}

void ScoreManager::Draw(ID3D12GraphicsCommandList * cmdList)
{
	for (auto itr = scores.begin(); itr != scores.end(); ++itr)
	{
		(*itr)->Draw(cmdList);
	}
	myScore->Draw(cmdList);
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
	myScore = new Score();
}

ScoreManager::~ScoreManager()
{
	Score::DestroyAsset();
	for (auto itr = scores.begin(); itr != scores.end(); ++itr)
	{
		delete (*itr);
	}
	delete currentScore;
	delete myScore;
}
