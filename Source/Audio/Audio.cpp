#ifdef _DEBUG
#include <imgui.h>
#endif // DEBUG
#include "Audio.h"
#include "System/Misc.h"
#include "Audio/AudioResourceManager.h"

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

void Audio::Play(AudioParam param)
{
	// ���\�[�X�쐬
	AudioResourceManager::Instance audio_resource_manager = AudioResourceManager::GetInstance();
	if (!audio_resource_manager.Get()) return;
	std::shared_ptr<AudioResource> resource = audio_resource_manager->LoadAudioResource(param.filename.c_str());
	// �I�[�f�B�I�\�[�X�쐬
	AudioSource* audio = this->audio_source_pool.emplace_back(new AudioSource(this->xaudio, resource, param));

	audio->Play();
}

void Audio::AllStop()
{
	for (AudioSource* audio_source : this->audio_source_pool)
	{
		audio_source->Stop();
	}
}

void Audio::AllStart()
{
	for (AudioSource* audio_source : this->audio_source_pool)
	{
		audio_source->Play();
	}
}

void Audio::AllClear()
{
	for (AudioSource* audio_source : this->audio_source_pool)
	{
		audio_source->ClearBuffers();
	}
}

void Audio::Update()
{
	// �Đ��I�������v�f���폜����
	for (size_t i = 0; i < audio_source_pool.size(); ++i)
	{
		AudioSource* audio_source = audio_source_pool[i];

		if (!audio_source) continue;
		// �I�[�f�B�I���L���Ȃ�X�L�b�v
		if (audio_source->IsAudioActive()) continue;

		auto it = std::find(this->audio_source_pool.begin(), this->audio_source_pool.end(), audio_source);
		if (it == this->audio_source_pool.end()) continue;
		
		// �I�[�f�B�I�̍폜
		delete audio_source;
		audio_source = nullptr;
		this->audio_source_pool.erase(it);
	}
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
		if (ImGui::Button("AllStart##Audio"))
		{
			AllStart();
		}
		ImGui::SameLine();
		if (ImGui::Button("AllStop##Audio"))
		{
			AllStop();
		}
		ImGui::SameLine();
		if (ImGui::Button("AllClear##Audio"))
		{
			AllClear();
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