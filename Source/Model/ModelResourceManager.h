#pragma once
#include <memory>
#include <string>
#include <map>
#include "System/ClassBase/Singleton.h"
#include "Model/ModelResource.h"

// ���\�[�X�}�l�[�W���[
class ModelResourceManager : public Singleton<ModelResourceManager>
{
public:
    ModelResourceManager() : Singleton(this) {}
    ~ModelResourceManager(){}
    
    // ���f�����\�[�X�ǂݍ���
    std::shared_ptr<ModelResource> LoadModelResource(const char* filename);

private:
    using ModelMap = std::map<std::string, std::weak_ptr<ModelResource>>;

    ModelMap models;
};