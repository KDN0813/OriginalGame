#pragma once
#include <memory>
#include <string>
#include <map>
#include "System/ClassBase/Singleton.h"
#include "Model/InstancingModelResource.h"

class InstancingModelResourceManager : public Singleton<InstancingModelResourceManager>
{
public:
    InstancingModelResourceManager() : Singleton(this) {}
    ~InstancingModelResourceManager() {}

    std::shared_ptr<InstancingModelResource> LoadModelResource(ID3D11Device* device,const char* filename);

private:
    using ModelMap = std::map<std::string, std::weak_ptr<InstancingModelResource>>;

    ModelMap instancing_models;
};