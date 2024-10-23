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

    // 全てのオーディオを停止する
    void AllStop();
    // 全てのオーディオ再生させる
    void AllStart();
    // 全ての曲を削除する
    void AllClear();

    // 更新
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

