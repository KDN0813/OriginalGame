#pragma once
#include "Component/Component.h"
#include <d3d11.h>
#include <DirectXMath.h>

class InstancingModelComponent : public Component
{
public:
    void Update(float elapsedTime);
private:
	DirectX::XMFLOAT4X4	transform;
	UINT anime_frame;
	UINT anime_index = 0;
	bool anime_loop = false;
	bool anime_play = false;
};

