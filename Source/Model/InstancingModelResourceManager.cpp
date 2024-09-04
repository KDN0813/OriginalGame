#include "InstancingModelResourceManager.h"
#include "Graphics/Graphics.h"

std::shared_ptr<InstancingModelResource> InstancingModelResourceManager::LoadModelResource(ID3D11Device* device,const char* filename)
{
    // モデル検索
    ModelMap::iterator it = instancing_models.find(filename);
    if (it != instancing_models.end())
    {
        // リンクが切れていない場合
        if (it->second.expired() == false)
        {
            // std::shared_ptrを作って返す
            return it->second.lock();
        }
    }

    // 新規モデルリソース作成＆読み込み
    std::shared_ptr<InstancingModelResource> instancing_model 
        = std::make_shared<InstancingModelResource>(device, filename);

    // モデルID設定
    instancing_model->SetModelId(ModelIDAllocate());

    // マップに登録
    instancing_models[filename] = std::weak_ptr<InstancingModelResource>(instancing_model);
    return instancing_model;
}
