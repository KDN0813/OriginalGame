#ifdef _DEBUG
#include <imgui.h>
#endif // DEBUG
#include "Audio.h"
#include "System/Misc.h"
#include "Audio/AudioResource.h"

Audio::Audio()
    : Singleton(this)
{
	HRESULT hr;

	// COM�̏�����
	hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	UINT32 create_flags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	//createFlags |= XAUDIO2_DEBUG_ENGINE;
#endif

	// XAudio������
	hr = XAudio2Create(&this->xaudio, create_flags);
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	// �}�X�^�����O�{�C�X����
	hr = xaudio->CreateMasteringVoice(&this->mastering_voice);
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
}

Audio::~Audio()
{
	// �I�[�f�B�I�\�[�X�̍폜
	{
		for (AudioSource* source : this->audio_source_pool)
		{
			if (source != nullptr)
			{
				delete source;
				source = nullptr;
			}
		}
	}

	// �}�X�^�����O�{�C�X�j��
	if (mastering_voice != nullptr)
	{
		mastering_voice->DestroyVoice();
		mastering_voice = nullptr;
	}

	// XAudio�I����
	if (xaudio != nullptr)
	{
		xaudio->Release();
		xaudio = nullptr;
	}

	// COM�I����
	CoUninitialize();
}

void Audio::Play(SEParam param)
{
	// ���\�[�X�쐬
	std::shared_ptr<AudioResource> resource = std::make_shared<AudioResource>(param.filename.c_str());
	// �I�[�f�B�I�\�[�X�쐬
	AudioSource* audio = this->audio_source_pool.emplace_back(new AudioSource(this->xaudio, resource));

	audio->Play();
}

#ifdef _DEBUG

void Audio::DebugDrawGUI()
{
    if (ImGui::Begin("Audio"))
    {
		float volume{};
		mastering_voice->GetVolume(&volume);
		if (ImGui::SliderFloat("Master Volume", &volume, 0.0f, 1.0f))
		{
			mastering_voice->SetVolume(volume);
		}

		// �I�[�f�B�I�\�[�X
		{
			for (auto audio_source : audio_source_pool)
			{
				audio_source->DebugDrawGUI();
			}
		}
    }
    ImGui::End();
}

#endif // DEBUG