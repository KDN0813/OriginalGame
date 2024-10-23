#pragma once
#include <xaudio2.h>
#include <vector>
#include <memory>
#include "System/ClassBase/Singleton.h"
#include "Audio/AudioSource.h"
#include "Audio/AudioParam.h"

class Audio : public Singleton<Audio>
{
public:
    Audio();
    ~Audio();

    void Play(AudioParam param);

    // �S�ẴI�[�f�B�I���~����
    void AllStop();
    // �S�ẴI�[�f�B�I�Đ�������
    void AllStart();
    // �S�Ă̋Ȃ��폜����
    void AllClear();

    // �X�V
    void Update();
private:
    std::vector<AudioSource*> audio_source_pool;

private:
    IXAudio2* xaudio = nullptr;
    IXAudio2MasteringVoice* mastering_voice = nullptr;

#ifdef _DEBUG
public:
    void DebugDrawGUI();

#endif // DEBUG
};

