#include "AudioResourceManager.h"

std::shared_ptr<AudioResource> AudioResourceManager::LoadAudioResource(const char* filename)
{
    AudioMap::iterator it = audios.find(filename);
    if (it != audios.end())
    {
        // リンクが切れていない場合
        if (it->second.expired() == false)
        {
            // std::shared_ptrを作って返す
            return it->second.lock();
        }
    }

    // 新規リソース作成＆読み込み
    std::shared_ptr<AudioResource>	audio;
    audio = std::make_shared<AudioResource>(filename);

    audios[filename] = std::weak_ptr<AudioResource>(audio);
    return audio;
}
