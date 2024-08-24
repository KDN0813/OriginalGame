#include "ModelResourceManager.h"

std::shared_ptr<ModelResource> ModelResourceManager::LoadModelResource(ID3D11Device* device,const char* filename)
{
    // モデル検索
    ModelMap::iterator it = models.find(filename);
    // 見つかった場合
    if (it != models.end())
    {
        // リンクが切れていない場合
        if (it->second.expired() == false)
        {
            // std::shared_ptrを作って返す
            return it->second.lock();
        }
    }

    // 新規モデルリソース作成＆読み込み
    std::shared_ptr<ModelResource>	model;
    model = std::make_shared<ModelResource>();
    model->Load(device, filename);
 
    // マップに登録
    // キーが登録されていなければ挿入し、あれば代入する
    // データの上書き・新規追加が自動で行われている
    models[filename] = std::weak_ptr<ModelResource>(model);
    return model;
}