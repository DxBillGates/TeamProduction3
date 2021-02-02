#pragma once
#include "SoundData.h"

class Sound : public IXAudio2VoiceCallback
{
private:
	static IXAudio2* xAudioEngine;
	static IXAudio2MasteringVoice* masterVoice;
	IXAudio2SourceVoice* sourceVoice;
	SoundData* pSoundData;
	bool isStart;
	bool isPlay;
	bool isEnd;
public:
	Sound(SoundData* soundData);
	~Sound();
	static void SetAudioEngine();
	void CreateVoice();
	void Start();
	bool GetIsStart();
	bool GetIsPlay();
	bool GetIsEnd();
private:
	void OnVoiceProcessingPassStart(UINT32 BytesRequired) {};
	STDMETHOD_(void, OnVoiceProcessingPassEnd) (THIS) {};
	STDMETHOD_(void, OnStreamEnd) (THIS) {};
	STDMETHOD_(void, OnBufferStart) (THIS_ void* pBufferContext)
	{
		isStart = true;
		isPlay = true;
		isEnd = false;
	};
	STDMETHOD_(void, OnBufferEnd) (THIS_ void* pBufferContext)
	{
		isStart = false;
		isPlay = false;
		isEnd = true;
	};
	STDMETHOD_(void, OnLoopEnd) (THIS_ void* pBufferContext) {};
	STDMETHOD_(void, OnVoiceError) (THIS_ void* pBufferContext, HRESULT Error) {};
};
