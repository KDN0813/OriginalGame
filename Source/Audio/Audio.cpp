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

	// COMの初期化
	hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	UINT32 create_flags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	//createFlags |= XAUDIO2_DEBUG_ENGINE;
#endif

	// XAudio初期化
	hr = XAudio2Create(&this->xaudio, create_flags);
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	// マスタリングボイス生成
	hr = xaudio->CreateMasteringVoice(&this->mastering_voice);
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
}

Audio::~Audio()
{
	// オーディオソースの削除
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

	// マスタリングボイス破棄
	if (mastering_voice != nullptr)
	{
		mastering_voice->DestroyVoice();
		mastering_voice = nullptr;
	}

	// XAudio終了化
	if (xaudio != nullptr)
	{
		xaudio->Release();
		xaudio = nullptr;
	}

	// COM終了化
	CoUninitialize();
}

void Audio::Play(AudioParam param)
{
	// リソース作成
	AudioResourceManager::Instance audio_resource_manager = AudioResourceManager::GetInstance();
	if (!audio_resource_manager.Get()) return;
	std::shared_ptr<AudioResource> resource = audio_resource_manager->LoadAudioResource(param.filename.c_str());
	// オーディオソース作成
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
	// 再生終了した要素を削除する
	for (size_t i = 0; i < audio_source_pool.size(); ++i)
	{
		AudioSource* audio_source = audio_source_pool[i];

		if (!audio_source) continue;
		// オーディオが有効ならスキップ
		if (audio_source->IsAudioActive()) continue;

		auto it = std::find(this->audio_source_pool.begin(), this->audio_source_pool.end(), audio_source);
		if (it == this->audio_source_pool.end()) continue;
		
		// オーディオの削除
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

		// オーディオソース
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