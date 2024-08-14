#pragma once
#include "Component/Component.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include <memory>

class InstancingModelResource;

class InstancingModelComponent : public Component
{
public:
	InstancingModelComponent();
	
    void Update(float elapsedTime) override;

	const char* GetName()const override { return "InstancingModelComponent"; }

	void PlayAnimetion(int animeIndex, bool loop = true);
private:
	std::shared_ptr<InstancingModelResource> model_resource;

	DirectX::XMFLOAT4X4	transform;
	UINT anime_frame;
	UINT anime_index;
	bool anime_loop;
	bool anime_play;
};

