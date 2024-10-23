#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG
#include "System/Misc.h"
#include "Audio/AudioSource.h"

// �R���X�g���N�^
AudioSource::AudioSource(IXAudio2* xaudio, std::shared_ptr<AudioResource>& resource)
	: resource(resource)
{
	HRESULT hr;

	// �\�[�X�{�C�X�𐶐�
	hr = xaudio->CreateSourceVoice(&source_voice, &resource->GetWaveFormat());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
}

// �f�X�g���N�^
AudioSource::~AudioSource()
{
	if (source_voice != nullptr)
	{
		source_voice->DestroyVoice();
		source_voice = nullptr;
	}
}

// �Đ�
void AudioSource::Play(bool loop)
{
	Stop();

	// �\�[�X�{�C�X�Ƀf�[�^�𑗐M
	XAUDIO2_BUFFER buffer = { 0 };
	buffer.AudioBytes = resource->GetAudioBytes();
	buffer.pAudioData = resource->GetAudioData();
	buffer.LoopCount = loop ? XAUDIO2_LOOP_INFINITE : 0;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	
	source_voice->SubmitSourceBuffer(&buffer);

	HRESULT hr = source_voice->Start();
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
}

// ��~
void AudioSource::Stop()
{
	source_voice->FlushSourceBuffers();
	source_voice->Stop();
}

#ifdef _DEBUG

void AudioSource::DebugDrawGUI()
{
	float volume{};
	this->source_voice->GetVolume(&volume);
	if (ImGui::SliderFloat("volume", &volume, 0.0f, 1.0f))
	{
		this->source_voice->SetVolume(volume);
	}
}

#endif // DEBUG