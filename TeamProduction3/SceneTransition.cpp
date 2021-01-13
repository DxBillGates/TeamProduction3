#include "SceneTransition.h"

void SceneTransition::Play()
{
	if (!playFlag)
	{
		playFlag = true;
		increaseFlag = true;
	}
}

float SceneTransition::GetT()
{
	return t;
}

bool SceneTransition::GetPlayFlag()
{
	return playFlag;
}

bool SceneTransition::GetReturnFlag()
{
	return returnFlag;
}

bool SceneTransition::GetEndFlag()
{
	return endFlag;
}

bool SceneTransition::GetIncreaseFlag()
{
	return increaseFlag;
}
