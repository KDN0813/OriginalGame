#pragma once
#include <memory>
#include <string>
#include <map>
#include "System/ClassBase/Singleton.h"
#include "Model/ModelResource.h"

// リソースマネージャー
class ModelResourceManager : public Singleton<ModelResourceManager>
{
public:
    ModelResourceManager() : Singleton(this) {}
    ~ModelResourceManager(){}
    
    // モデルリソース読み込み
    std::shared_ptr<ModelResource> LoadModelResource(const char* filename);

private:
    using ModelMap = std::map<std::string, std::weak_ptr<ModelResource>>;

    ModelMap models;
};