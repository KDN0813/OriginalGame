#pragma once

#include <memory>
#include <xaudio2.h>
#include "Audio/AudioResource.h"

// オーディオソース
class AudioSource
{
public:
	AudioSource(IXAudio2* xaudio, std::shared_ptr<AudioResource>& resource);
	~AudioSource();

	// 再生
	void Play();
	// 停止
	void Stop();

	void SetVolume(float v) { source_voice->SetVolume(v); }
	void GetVolume(float* v) { source_voice->GetVolume(v); }
	float GetVolume() { float v{}; source_voice->GetVolume(&v); return v; }
private:
	IXAudio2SourceVoice*			source_voice = nullptr;
	std::shared_ptr<AudioResource>	resource;

public:
#ifdef _DEBUG
	void DebugDrawGUI();

#endif // DEBUG
};
