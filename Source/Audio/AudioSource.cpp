#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG
#include "System/Misc.h"
#include "Audio/AudioSource.h"

// コンストラクタ
AudioSource::AudioSource(IXAudio2* xaudio, std::shared_ptr<AudioResource>& resource, AudioParam param)
	: resource(resource), id(Allocate())
{
	HRESULT hr;

	// ソースボイスを生成
	hr = xaudio->CreateSourceVoice(&source_voice, &resource->GetWaveFormat());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	// ソースボイスにデータを送信
	XAUDIO2_BUFFER buffer = { 0 };
	buffer.AudioBytes = resource->GetAudioBytes();
	buffer.pAudioData = resource->GetAudioData();
	buffer.LoopCount = param.loop ? XAUDIO2_LOOP_INFINITE : 0;
	buffer.Flags = XAUDIO2_END_OF_STREAM;

	SetVolume(param.volume);
	source_voice->SubmitSourceBuffer(&buffer);

#ifdef _DEBUG
	filename = param.filename;
#endif // DEBUG
}

// デストラクタ
AudioSource::~AudioSource()
{
	if (source_voice != nullptr)
	{
		source_voice->DestroyVoice();
		source_voice = nullptr;
	}
}

// 再生
void AudioSource::Play()
{
	Stop();

	HRESULT hr = source_voice->Start();
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
}

// 停止
void AudioSource::Stop()
{
	source_voice->Stop();
}

bool AudioSource::IsAudioActive()
{
	XAUDIO2_VOICE_STATE state;
	this->source_voice->GetState(&state);

	// 再生待ちのバッファが存在するか
	return (0 < state.BuffersQueued);
}

void AudioSource::ClearBuffers()
{
	this->source_voice->Stop();
	this->source_voice->FlushSourceBuffers();
}

#ifdef _DEBUG

void AudioSource::DebugDrawGUI()
{
	std::string label;
	label = this->filename + "##Audio" + std::to_string(id);
	if (ImGui::CollapsingHeader(label.c_str()))
	{
		float volume{};
		this->source_voice->GetVolume(&volume);

		label = "volume##Audio" + std::to_string(id);
		if (ImGui::SliderFloat(label.c_str(), &volume, 0.0f, 1.0f))
		{
			this->source_voice->SetVolume(volume);
		}
		label = "Play##Audio" + std::to_string(id);
		if (ImGui::Button(label.c_str()))
		{
			Play();
		}
		ImGui::SameLine();
		label = "Stop##Audio" + std::to_string(id);
		if (ImGui::Button(label.c_str()))
		{
			Stop();
		}
	}
}

#endif // DEBUG