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

	/**
	* @fn ModelIDAllocate
	* @brief ƒ‚ƒfƒ‹ID‚ğŠ„‚èU‚é
	*
	* @return Š„‚èU‚ç‚ê‚½ID
	*/
	int ModelIDAllocate()
	{
		static int id = 0;
		return id++;
	}

    std::shared_ptr<InstancingModelResource> LoadModelResource(ID3D11Device* device,const char* filename);

private:
    using ModelMap = std::map<std::string, std::weak_ptr<InstancingModelResource>>;

    ModelMap instancing_models;
};