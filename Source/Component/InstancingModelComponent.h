#pragma once
#include "Component/Component.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include <memory>

class InstancingModelResource;
class ModelResource;

class InstancingModelComponent : public Component
{
public:
	InstancingModelComponent(ID3D11Device* device, const char* filename);
	
    void Update(float elapsedTime) override;

	const char* GetName()const override { return "InstancingModelComponent"; }

	void PlayAnimetion(int animeIndex, bool loop = true);

	InstancingModelResource* GetInstancingModelResource() { return this->instancing_model_resource.get(); }
	ModelResource* GetModelResource() { return this->model_resource.get(); }
private:
	std::shared_ptr<InstancingModelResource> instancing_model_resource;
	std::shared_ptr<ModelResource> model_resource;

	DirectX::XMFLOAT4X4	transform;
	UINT anime_frame;
	UINT anime_index;
	bool anime_loop;
	bool anime_play;
};

