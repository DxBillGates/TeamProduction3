#include "Sound.h"

IXAudio2* Sound::xAudioEngine = nullptr;
IXAudio2MasteringVoice* Sound::masterVoice = nullptr;

Sound::Sound(SoundData * soundData)
	:
	sourceVoice(nullptr),
	pSoundData(soundData),
	isStart(false),
	isPlay(false),
	isEnd(false)
{
	CreateVoice();
}

Sound::~Sound()
{
	//pSoundData = nullptr;
	if (sourceVoice != nullptr)sourceVoice->Stop();
}

void Sound::SetAudioEngine()
{
	XAudio2Create(&xAudioEngine, 0, XAUDIO2_DEFAULT_PROCESSOR);
	xAudioEngine->CreateMasteringVoice(&masterVoice);
}

void Sound::CreateVoice()
{
	HRESULT result = xAudioEngine->CreateSourceVoice(&sourceVoice, pSoundData->GetWaveFormatEx(), 0, 2.0f, &(*this));
}

void Sound::Start()
{
	if (!isPlay)
	{
		isEnd = false;
		sourceVoice->SubmitSourceBuffer(pSoundData->GetAudioBuffer());
		sourceVoice->Start();
	}
}

bool Sound::GetIsStart()
{
	return isStart;
}

bool Sound::GetIsPlay()
{
	//Ä¶ŠJn‚ª‚³‚ê‚Ä‚¢‚Ä’â~‚µ‚Ä‚¢‚È‚¢‚È‚çisPlay‚ğtrue‚É‚·‚é
	if (isStart && !isEnd)
	{
		isPlay = true;
	}
	else if (!isStart)
	{
		isPlay = false;
	}

	return isPlay;
}

bool Sound::GetIsEnd()
{
	return isEnd;
}
