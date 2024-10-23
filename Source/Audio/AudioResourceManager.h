#pragma once
#include <map>
#include "System/ClassBase/Singleton.h"
#include "Audio/AudioResource.h"

class AudioResourceManager :public Singleton<AudioResourceManager>
{
public:
    AudioResourceManager() : Singleton(this) {}
    ~AudioResourceManager() {}

    // モデルリソース読み込み
    std::shared_ptr<AudioResource> LoadAudioResource(const char* filename);

private:
    using AudioMap = std::map<std::string, std::weak_ptr<AudioResource>>;

    AudioMap audios;
};