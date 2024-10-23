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
	std::shared_ptr<AudioResource> resource = AudioResourceManager::Instance()->LoadAudioResource(param.filename.c_str());
	// オーディオソース作成
	AudioSource* audio = this->audio_source_pool.emplace_back(new AudioSource(this->xaudio, resource, param));

	audio->Play();
}

void Audio::Update()
{
	// 再生終了した要素を削除する
	this->audio_source_pool.erase(
		std::remove_if(this->audio_source_pool.begin(), this->audio_source_pool.end(),
			[](AudioSource* audio_source)
			{
				return !audio_source->IsAudioActive();
			}),
		this->audio_source_pool.end());
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