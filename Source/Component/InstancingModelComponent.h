#pragma once
#include "Component/Component.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include <memory>

class InstancingModelResource;
class ModelResource;

class Transform3DComponent;

class InstancingModelComponent : public Component
{
public:
public:
	InstancingModelComponent(ID3D11Device* device, const char* filename);

    void Update(float elapsedTime) override;

	const char* GetName()const override { return "InstancingModelComponent"; }
	// —Dæ“x
	const COMPONENT_PRIORITY GetPriority()const noexcept override { return COMPONENT_PRIORITY::HIGH; }

	void PlayAnimetion(int animeIndex, bool loop = true);

	// ŠeŽæ“¾EÝ’èŠÖ”
	InstancingModelResource* GetInstancingModelResource() { return this->instancing_model_resource.get(); }
	ModelResource* GetModelResource() { return this->model_resource.get(); }
	const UINT GetAnimeFrame() { return this->anime_frame; }
	const UINT GetAnimationStartOffset();
	const int GetModelId();
private:
	std::shared_ptr<InstancingModelResource> instancing_model_resource;
	std::shared_ptr<ModelResource> model_resource;

	UINT anime_frame = 0;
	UINT anime_index = 0;
	int anime_speed = 1;
	bool anime_loop = false;
	bool anime_play = false;

private:
	std::weak_ptr<Transform3DComponent> transform_Wptr;

#ifdef _DEBUG
public:
	void DrawDebugGUI()override;

#endif // _DEBUG
};

