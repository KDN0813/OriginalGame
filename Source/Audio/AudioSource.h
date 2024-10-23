#pragma once

#include <memory>
#include <xaudio2.h>
#include "Audio/AudioResource.h"

// �I�[�f�B�I�\�[�X
class AudioSource
{
public:
	AudioSource(IXAudio2* xaudio, std::shared_ptr<AudioResource>& resource);
	~AudioSource();

	// �Đ�
	void Play();
	// ��~
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
