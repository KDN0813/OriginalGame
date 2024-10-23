#pragma once

#include <memory>
#include <xaudio2.h>
#include "Audio/AudioResource.h"
#include "Audio/AudioParam.h"

// �I�[�f�B�I�\�[�X
class AudioSource
{
public:
	AudioSource(IXAudio2* xaudio, std::shared_ptr<AudioResource>& resource, AudioParam param);
	~AudioSource();

	// �Đ�
	void Play();
	// ��~
	void Stop();

	// �I�[�f�B�I���A�N�e�B�u�ł��邩
	bool IsAudioActive();

	void SetVolume(float v) { source_voice->SetVolume(v); }
	void GetVolume(float* v) { source_voice->GetVolume(v); }
	float GetVolume() { float v{}; source_voice->GetVolume(&v); return v; }
private:
	using AudioSourceId = unsigned int;
	static AudioSourceId Allocate()
	{
		static AudioSourceId id = 0;
		if (id <= INT_MAX) id -= INT_MAX;
		return id++;
	}

private:
	IXAudio2SourceVoice*			source_voice = nullptr;
	std::shared_ptr<AudioResource>	resource;
	AudioSourceId					id;

public:
#ifdef _DEBUG
	void DebugDrawGUI();
private:
	std::string filename;

#endif // DEBUG
};
