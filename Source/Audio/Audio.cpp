#include "Audio.h"
#include "System/Misc.h"
#ifdef _DEBUG
#include <imgui.h>
#endif // DEBUG


Audio::Audio()
    :Singleton(this)
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
	hr = xaudio->CreateMasteringVoice(&this->masteringVoice);
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
}

#ifdef _DEBUG

void Audio::DebugDrawGUI()
{
    if (ImGui::Begin("Audio"))
    {

    }
    ImGui::End();
}

#endif // DEBUG