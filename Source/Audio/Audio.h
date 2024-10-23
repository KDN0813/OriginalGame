#pragma once
#include <xaudio2.h>
#include <vector>
#include "System/ClassBase/Singleton.h"

class Audio : public Singleton<Audio>
{
public:
    Audio();
    ~Audio() {};

private:


private:
    IXAudio2* xaudio = nullptr;
    IXAudio2MasteringVoice* masteringVoice = nullptr;

#ifdef _DEBUG
public:
    void DebugDrawGUI();

#endif // DEBUG
};

