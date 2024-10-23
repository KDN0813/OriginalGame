#pragma once
#include <xaudio2.h>
#include <vector>
#include <string>
#include "System/ClassBase/Singleton.h"
#include "Audio/AudioSource.h"

class Audio : public Singleton<Audio>
{
public:
    struct SEParam
    {
        std::string filename{};
        float volume = 1.0f;
        bool loop = false;
    };

public:
    Audio();
    ~Audio();

    void Play(SEParam param);
private:
    std::vector<AudioSource> audio_source_vec;

private:
    IXAudio2* xaudio = nullptr;
    IXAudio2MasteringVoice* mastering_voice = nullptr;

#ifdef _DEBUG
public:
    void DebugDrawGUI();

#endif // DEBUG
};

