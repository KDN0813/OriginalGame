#pragma once
#include <memory>
#include <string>
#include <map>
#include "System/ClassBase/Singleton.h"
#include "Model/ModelResource.h"

class ModelResourceManager : public Singleton<ModelResourceManager>
{
public:
    ModelResourceManager() : Singleton(this) {}
    ~ModelResourceManager(){}
    
    // モデルリソース読み込み
    // Hack (08/15)Device引数に追加する
    std::shared_ptr<ModelResource> LoadModelResource(const char* filename);

private:
    using ModelMap = std::map<std::string, std::weak_ptr<ModelResource>>;

    ModelMap models;
};