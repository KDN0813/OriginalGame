#pragma once
#include "Component/Component.h"
#include <d3d11.h>
#include <DirectXMath.h>

class InstancingModelResource;

class InstancingModelComponent : public Component
{
public:
	InstancingModelComponent() {};
	
    void Update(float elapsedTime) override;

	void PlayAnimetion(int animeIndex, bool loop = true);
private:
	InstancingModelResource* model_resource;

	DirectX::XMFLOAT4X4	transform;
	UINT anime_frame;
	UINT anime_index = 0;
	bool anime_loop = false;
	bool anime_play = false;
};

